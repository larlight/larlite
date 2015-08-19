#ifndef LARLITE_DRAWRAWDIGIT_CXX
#define LARLITE_DRAWRAWDIGIT_CXX


#include "LArUtil/DetectorProperties.h"
#include "DrawRawDigit.h"
#include "DataFormat/rawdigit.h"

namespace evd {

  DrawRawDigit::DrawRawDigit(){ 
    _name="DrawRawDigit"; 
    producer = "daq";
    // Py_InitModule("DrawRawDigit",NULL);

  }



  bool DrawRawDigit::initialize() {

    //
    // This function is called in the beggining of event loop
    // Do all variable initialization you wish to do here.
    // If you have a histogram to fill in the event loop, for example,
    // here is a good place to create one on the heap (i.e. "new TH1D"). 
    //
    //
    

    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      setXDimension(geoService->Nwires(p), p);
      setYDimension(detProp -> ReadOutWindowSize(), p);
    }
    initDataHolder();

    return true;

  }
  
  bool DrawRawDigit::analyze(larlite::storage_manager* storage) {
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //


    // This is an event viewer.  In particular, this handles raw wire signal drawing.
    // So, obviously, first thing to do is to get the wires.
    auto RawDigitHandle = storage->get_data<larlite::event_rawdigit>(producer);

    run = RawDigitHandle->run();
    subrun = RawDigitHandle->subrun();
    event = RawDigitHandle->event_id();

    // Save one file per event ...
    char name[100];
    sprintf(name,"RawDigitAna_%i_%i_%i.root",run, subrun,event);
    _fout= new TFile(name,"RECREATE");
    _fout -> cd();
    
    // Set up the TTree:
    TTree * tree = new TTree("waveformsub","waveformsub");

    tree -> Branch("run",&run);
    tree -> Branch("subrun",&subrun);
    tree -> Branch("event",&event);

    std::vector<float> x_axis;
    x_axis.resize(detProp->ReadOutWindowSize());
    for (unsigned int i = 0; i < x_axis.size(); i++)
      x_axis.at(i) = i;

    // Determine how many steps are needed, and set up to write out waveforms
    _subtractionWaveForm.resize(geoService -> Nplanes());
    _graphSubtractionWaveForm.resize(geoService -> Nplanes());
    
    for (unsigned int p = 0; p < geoService -> Nplanes(); p++){
      int nSteps = geoService -> Nwires(p) / stepSize;
      _subtractionWaveForm.at(p).resize(nSteps);
      _graphSubtractionWaveForm.at(p).resize(geoService ->Nwires(p));
      for (auto & vec : _subtractionWaveForm.at(p))
        vec.resize(detProp->ReadOutWindowSize());
    }


    float rmsMinBadWire = 1.0;

    std::vector<std::vector<bool>> badWireMapByPlane;
    badWireMapByPlane.resize(geoService->Nplanes());

    std::cout << "badWireMapByPlane.size() " << badWireMapByPlane.size() << std::endl;
    for (auto const& rawdigit: *RawDigitHandle){
      unsigned int ch = rawdigit.Channel();
      if (ch >= 8254) continue;

      int wire = geoService->ChannelToWire(ch);
      int plane = geoService->ChannelToPlane(ch);
      if (badWireMapByPlane.at(plane).size() < geoService -> Nwires(plane))
        badWireMapByPlane.at(plane).resize(geoService->Nwires(plane));

      if (wire < 0  || wire > geoService -> Nwires(plane))
        continue;




      int offset = wire * detProp -> ReadOutWindowSize();
      // convert short ADCs to float
      
      // Get the pedestal for this channel:
      float ped = 0.0;
      for (int j = 0; j < 500; j++){
        ped += rawdigit.ADC(j);
      }
      ped /= 500.0;

      int i = 0;
      // Calculate an rms here to spot bad wires
      float rms = 0.0;
      for (auto & adc : rawdigit.ADCs()){
        _planeData.at(plane).at(offset + i) = adc - ped;
        rms += (adc-ped)*(adc-ped);
        i++;
      }
      rms /= detProp -> ReadOutWindowSize();
      if (plane == 2){
        if (wire > 1333 && wire < 1350){
          std::cout << "RMS of wire " << wire << " is " << rms << std::endl;
        }
      }
      if (rms < rmsMinBadWire)
        badWireMapByPlane.at(plane).at(wire) = true;
    }

    
    // Do a median subtraction in the simplest possible way, across 96 channels

    for (unsigned int plane = 0; plane < 3; plane ++){

      unsigned int nSteps = geoService -> Nwires(plane) / stepSize;
      for (unsigned int step = 0; step < nSteps; step ++){
        // Within each step, loop over the ticks.
        // For each tick, loop over each wire and find the median.
        // Then, subtract the median from each tick.
        for (unsigned int tick = 0; tick < detProp -> ReadOutWindowSize(); tick ++){
          // Get the median across this step's set of wires.
          int wireStart = step*stepSize;
          std::vector<float> vals;
          vals.resize(stepSize);
          for (unsigned int wire = wireStart; wire < wireStart + stepSize; wire ++){
            // skip bad wires
            if (badWireMapByPlane.at(plane)[wire]) continue;
            int offset = wire*detProp->ReadOutWindowSize();
            if (plane != 2)
              vals.at(wire-wireStart) = _planeData.at(plane).at(offset + tick);
            // do a rolling average for collection plane:
            else{
              int averageSize = 10;
              if (tick < averageSize || tick > detProp->ReadOutWindowSize() - averageSize)
                vals.at(wire-wireStart) = _planeData.at(plane).at(offset + tick);
              else{
                for (int workingTick = tick - averageSize; workingTick <= tick + averageSize; workingTick ++ )
                  vals.at(wire-wireStart) += _planeData.at(plane).at(offset + workingTick);
                vals.at(wire-wireStart) /= (2*averageSize + 1);
              }
            }
          }
          // Calculate the median:
          sort(vals.begin(), vals.end());
          float median = 0.5* vals.at(stepSize/2) + 0.5*vals.at(stepSize/2 -1);
          // Now subtract the median from the values:
          for (unsigned int wire = wireStart; wire < wireStart + stepSize; wire ++){
            // Skip bad wires
            if (badWireMapByPlane.at(plane)[wire]) continue;
            int offset = wire*detProp->ReadOutWindowSize();
            _planeData.at(plane).at(offset + tick) -= median;
          }

          // Save the subtraction waveform:
          _subtractionWaveForm.at(plane).at(step).at(tick) = median;

          // // Do a subtraction of the U plane wires with V plane values
          // if(plane == 0) {
          //   if ( step <= 7 || step >= 25-7) continue;
          //   int U_wire = wireStart - 672;

          //   for (unsigned int wire = U_wire; wire < U_wire + stepSize; wire ++){
          //     int offset = wire*detProp->ReadOutWindowSize();
          //     _planeData.at(plane+1).at(offset + tick) += median;
          //   }
          // }
        } // loop over ticks

        // Now that the tick loop is finished, the wave form is finalized
        // Copy the data to the tgraph so that it can be stored, and set up the branches
        
        // initialize the graph to point to the right vector:
        _graphSubtractionWaveForm.at(plane).at(step) 
          = new TGraph(x_axis.size(), &(x_axis[0]), &(_subtractionWaveForm.at(plane).at(step)[0]));

        char name[100];
        sprintf(name,"subwaveform_%i_%i",plane,step);
        tree -> Branch(name, &(_subtractionWaveForm.at(plane).at(step)) );
        sprintf(name,"graphwaveform_%i_%i",plane,step);
        tree -> Branch(name, &(_graphSubtractionWaveForm.at(plane).at(step)));
        _graphSubtractionWaveForm.at(plane).at(step)->Write(name);
      } // loop over steps
    } // loop over planes

    tree -> Fill();

    std::cout << "Finalized called\n";
    std::cout << "_fout is " << _fout << std::endl;

    tree -> Write();
    _fout -> Close();

    return true;
  }

  bool DrawRawDigit::finalize() {

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
  


    return true;
  }



}
#endif



      // // TEMPORARY: fix collection mapping.
      // if (plane == 2){
      //   // std::cout << "Wire from " << wire;
      //   int wireAnchor = wire - (wire % 32);
      //   wire = wireAnchor + 32 - (wire % 32) - 1;
      //   // std::cout << " to " << wire <<std::endl;
      // }
      // if (plane == 1){
      //   if (wire < 672){
      //      int wireTemp = 0;
      //      /*// 0
      //      if (wire > -1 && wire <= 31)
      //         {wireTemp = wire + 64; }
      //      // 1
      //      if (wire > 31 && wire <=63)
      //         { wireTemp = wire - 32; }
      //      // 2   
      //      if (wire > 63 && wire  <= 95)
      //         { wireTemp = wire - 32; }
              
      //      // 3   
      //      if (wire > 95 && wire <= 127)
      //         { wireTemp = wire + 256; }
      //      // 4
      //      if (wire > 127 && wire <= 159)
      //         { wireTemp = wire + 320; } 
      //      // 5     
      //      if (wire > 159 && wire <= 191)
      //         { wireTemp = wire + 256;}
           
      //      // 6   
      //      if (wire > 191 && wire <= 223)
      //         { wireTemp = wire +320; } 
      //      // 7   
      //      if (wire > 223 && wire <= 255)
      //         { wireTemp = wire  +256; } 
      //      // 8  
      //      if (wire > 255 && wire <= 287)
      //         { wireTemp = wire +320; }
           
      //      // 9   
      //      if (wire > 287 && wire <= 319)
      //         { wireTemp = wire +256; }
      //      // 10
      //      if (wire > 319 && wire <= 351)
      //         { wireTemp = wire  +320; }
      //      // 11
      //      if (wire > 351 && wire <= 383)
      //         { wireTemp = wire +256; }
           
      //      // 12   
      //      if (wire > 383 && wire <= 415)
      //         { wireTemp = wire -256; }
           
      //      // 13
      //      if (wire > 415 && wire <= 447)
      //         { wireTemp = wire -320; }
      //      // 14
      //      if (wire > 447 && wire <= 479)
      //         { wireTemp = wire -256; }
             
      //      // 15   
      //      if (wire > 479 && wire <= 511)
      //         { wireTemp = wire  -320; }
      //      // 16                                          
      //      if (wire > 511 && wire <= 543)
      //         { wireTemp = wire -256; }
      //      // 17   
      //      if (wire > 543 && wire <= 575)
      //         { wireTemp = wire  -320; }
           
      //      // 18                 
      //      if (wire > 575 && wire <= 607)
      //         { wireTemp = wire  -256; }
      //      // 19                 
      //      if (wire > 607 && wire <= 639)
      //         { wireTemp = wire  -320; }
      //      // 20   
      //      if (wire > 639 && wire <= 671)
      //         { wireTemp = wire  -256; }                 
      //      */
           
           
      //      // 0
      //      if (wire > -1 && wire <= 31)
      //         {wireTemp = wire + 32; }
      //      // 1
      //      if (wire > 31 && wire <=63)
      //         { wireTemp = wire + 32; }
      //      // 2   
      //      if (wire > 63 && wire  <= 95)
      //         { wireTemp = wire - 64; }
              
      //      // 3   
      //      if (wire > 95 && wire <= 127)
      //         { wireTemp = wire + 320; }
      //      // 4
      //      if (wire > 127 && wire <= 159)
      //         { wireTemp = wire + 256; } 
      //      // 5     
      //      if (wire > 159 && wire <= 191)
      //         { wireTemp = wire + 320;}
           
      //      // 6   
      //      if (wire > 191 && wire <= 223)
      //         { wireTemp = wire +256; } 
      //      // 7   
      //      if (wire > 223 && wire <= 255)
      //         { wireTemp = wire  +320; } 
      //      // 8  
      //      if (wire > 255 && wire <= 287)
      //         { wireTemp = wire +256; }
           
      //      // 9   
      //      if (wire > 287 && wire <= 319)
      //         { wireTemp = wire +320; }
      //      // 10
      //      if (wire > 319 && wire <= 351)
      //         { wireTemp = wire  +256; }
      //      // 11
      //      if (wire > 351 && wire <= 383)
      //         { wireTemp = wire -256; }
           
      //      // 12   
      //      if (wire > 383 && wire <= 415)
      //         { wireTemp = wire +256; }
           
      //      // 13
      //      if (wire > 415 && wire <= 447)
      //         { wireTemp = wire -256; }
      //      // 14
      //      if (wire > 447 && wire <= 479)
      //         { wireTemp = wire -320; }
             
      //      // 15   
      //      if (wire > 479 && wire <= 511)
      //         { wireTemp = wire  -256; }
      //      // 16                                          
      //      if (wire > 511 && wire <= 543)
      //         { wireTemp = wire -320; }
      //      // 17   
      //      if (wire > 543 && wire <= 575)
      //         { wireTemp = wire  -256; }
           
      //      // 18                 
      //      if (wire > 575 && wire <= 607)
      //         { wireTemp = wire  -320; }
      //      // 19                 
      //      if (wire > 607 && wire <= 639)
      //         { wireTemp = wire  -256; }
      //      // 20   
      //      if (wire > 639 && wire <= 671)
      //         { wireTemp = wire  -320; } 
                            
      //       // int wireTemp = wire - 32;
      //      int wireAnchor = wireTemp - (wireTemp % 32);
      //      wireTemp = wireAnchor + 32 - (wire % 32) - 1;
           
      //      if(wireTemp > 63 && wireTemp <=95)
      //         {wireTemp += 320;}
      //      if(wireTemp >383 && wireTemp <=447)
      //         {wireTemp -= 320;}
           
      //       wire = wireTemp;
      //     // }
      //     // int wireAnchor = wire - (wire % 256);
      //     // wire = wireAnchor + 256 - (wire % 256) - 1 ;
      //     //int wireAnchor = wire - (wire % 64) + 16;
      //     //wire = wireAnchor + 64 - (wire % 64) - 1;
      //     //wire = wireTemp;
      //   }

      // }

      // if (plane == 0){
      //   if (wire > 1727){
      //   int wireAnchor = wire - (wire % 32);
      //   wire = wireAnchor + 32 - (wire % 32) - 1;

      //   }
      // }

      // // Catch the situation where the hanging wires are getting set:
      // if (plane == 2){
      //   if (wire > 1727 && wire < 1824){
      //     continue;
      //   }
      // }

      // // Now map the induction blocks that should be collection to collection:
      // if (plane == 1){
      //   if (wire > 1535 && wire < 1584){
      //     plane = 2;
      //     wire = 1728 + (wire - 1535)*2 - 2;
      //     // std::cout << "wire set to " << wire << std::endl;
      //   }
      // }

      // if (plane == 0){
      //   if (wire > 863 && wire < 912){
      //     plane = 2;
      //     wire = 1728 + (wire - 863)*2 - 1 ;
      //   }
      // }

