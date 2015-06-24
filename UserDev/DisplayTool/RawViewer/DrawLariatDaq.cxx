#ifndef LARLITE_DRAWLARIATDAQ_CXX
#define LARLITE_DRAWLARIATDAQ_CXX

#include "DrawLariatDaq.h"
#include "DataFormat/wire.h"
#include <fstream>
#include "TChain.h"

namespace evd {

  DrawLariatDaq::DrawLariatDaq(int ticks){ 
    wiredataIN = new std::vector<std::vector<std::vector<unsigned short> > > ;
    branches.resize(64);
    if (ticks == -1)
      _n_time_ticks = 3072;
    else
      _n_time_ticks = ticks;
    c = new TChain("DataQuality/v1740");
    initialize();
  }

  DrawLariatDaq::~DrawLariatDaq(){
    // std::cout << "\n\n\nDestructing the drawer!\n\n\n";
    delete wiredataIN;
    delete c;
    // for (auto branch : branches){
      // delete branch;
    // }
  }

  void DrawLariatDaq::initialize() {

    _event_no=0;
    _run=0;
    _spill=0;


    // Initialize data holder:
    // Resize data holder to accomodate planes and wires:
    if (wiredataIN -> size() != geoService -> Nviews())
      wiredataIN->resize(geoService -> Nviews());
     
    // resize to the right number of planes
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      // resize to the right number of wires
      if (wiredataIN->at(p).size() != geoService->Nwires(p) )
        wiredataIN->at(p).resize(geoService->Nwires(p));
        // Resize the wires to the right length
        for (auto & vec : wiredataIN->at(p)){
          vec.resize(_n_time_ticks);
        }
    }

    if (wiredata -> size() != geoService -> Nviews())
      wiredata->resize(geoService -> Nviews());
     
    // resize to the right number of planes
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      // resize to the right number of wires
      if (wiredata->at(p).size() != geoService->Nwires(p) )
        wiredata->at(p).resize(geoService->Nwires(p));
        // Resize the wires to the right length
        for (auto & vec : wiredata->at(p)){
          vec.resize(_n_time_ticks);
        }
    }

    return;

  }
  
  void DrawLariatDaq::readData(){


    // Want to be sure that the TChain is ready to go...
    // Do that later.

    std::map<int,int> lar_channel_usage;

    // Need to loop over the file 7 times to get all the cards
    for (int i_card = 0; i_card < _n_cards; i_card ++){
      // Set all the branch addresses for this card.
      // For each channel, can use card + channel to map
      // to larsoft channel.  Then, use larsoft channel
      // to map to the wire location.
      c -> SetBranchAddress("run",&_run);
      c -> SetBranchAddress("event_counter",&_event_no);
      c -> SetBranchAddress("spill",&_spill);

      for(int channel = 0; channel < _n_channels; channel ++ ){

        int plane(0),wire(0);
        int ch = i_card*_n_channels + channel;
        if (ch < 240 ){
          plane = 0;
          wire = 239 - ch;
        }
        else if (ch < 480 && ch >= 240){
          plane = 1;
          wire = 479-ch;
        }

        // Now we know which part of the data to read this channel into;
        char name[20];
        sprintf(name,"channel_%i",channel);
        c -> SetBranchAddress(name,
            &(wiredataIN->at(plane).at(wire)[0]),
            &(branches.at(channel)) );

        if (lar_channel_usage[ch] > 1){
          std::cout << "Found a duplicate channel at " << ch << std::endl;
        }
      }
      c -> GetEntry(_current_event*_n_cards + i_card);
    }

    // The wire data needs to be pedestal subtracted.
    int i_plane = 0;
    for (auto & plane : *wiredataIN){
      float pedestal =0;
      int i_wire = 0;
      for (auto & wire : plane){

        for (auto & tick : wire){
          pedestal += tick;
        }
        pedestal /= _n_time_ticks;
        for (unsigned int tick = 0; tick < wire.size(); tick++){
          wiredata->at(i_plane).at(i_wire).at(tick) = wire.at(tick) - pedestal;
        }
        i_wire ++;
      }
      i_plane ++;
    }

  }

  // This is the function that actually reads in an event
  void DrawLariatDaq::nextEvent(){

    if (_current_event >= _n_events){
      std::cout << "On Event " << _current_event << std::endl;
      std::cout << "Warning, end of file reached, select a new file.\n";
      return;
    }
    else{
      _current_event ++;
      readData();
    }

    return;
  }

  void DrawLariatDaq::prevEvent(){

    if (_current_event <= 0){
      std::cout << "On event " << _current_event << std::endl;
      std::cout << "Warning, at beginning of file, can not go backwards.\n";
      return;
    }
    else{
      _current_event --;
      readData();
    }

    return;

  }

  void DrawLariatDaq::goToEvent(int e){
    if (e < 0){
      std::cout << "Selected event is too low.\n";
      return;
    }
    if (e >= _n_events){
      std::cout << "Selected event is too high.\n";
      return;
    }
    _current_event = e;
    readData();

  }

  // override the default behavior
  void DrawLariatDaq::setInput(std::string s){
    // if the file isn't new, do nothing:
    if (s == producer) return;
    // check to see if this file exists.
    std::cout << "Attempting to open file " << s << std::endl;
    std::ifstream ifile(s);
    if (!ifile.is_open()){
      std::cerr << "ERROR: Input file failed to open.\n";
      return;
    }
    else{
      // The file exists, try to read it.
      producer = s;
      _current_event = 0;
      c -> Reset();
      c -> Add(producer.c_str());
      _n_events  = c -> GetEntries() / _n_cards;
      if (_n_events == 0){
        _run = 0;
        _event_no = 0;
        return;
      }
      readData();
    }
  }

  int DrawLariatDaq::getLarsoftChannel(int & asic, int & channelOnBoard){
    int lar_channel = asic*_n_channels + channelOnBoard;
    if (lar_channel < 240)
      lar_channel = 239 - lar_channel;
    else{
      lar_channel = 479 - (lar_channel-240);
    }
    return lar_channel;
  }



}
#endif
