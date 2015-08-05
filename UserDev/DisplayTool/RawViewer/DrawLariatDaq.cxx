#ifndef LARLITE_DRAWLARIATDAQ_CXX
#define LARLITE_DRAWLARIATDAQ_CXX

#include "DrawLariatDaq.h"
#include "DataFormat/wire.h"
#include <fstream>
#include "TChain.h"

namespace evd {

  DrawLariatDaq::DrawLariatDaq(int ticks){ 
    branches.resize(64);
    if (ticks == -1)
      _n_time_ticks = 3072;
    else
      _n_time_ticks = ticks;


    c = new TChain("DataQuality/v1740");
    initialize();
  }

  DrawLariatDaq::~DrawLariatDaq(){
    delete c;

  }

  void DrawLariatDaq::initialize() {

    _event_no=0;
    _run=0;
    _spill=0;

    // set up the wire and drift dimensions
    // resize to the right number of planes
    for (unsigned int p = 0; p < geoService -> Nviews(); p ++){
      setXDimension(geoService->Nwires(p), p);
      setYDimension(_n_time_ticks, p);
    }

    // Initialize the place to hold data:
    initDataHolder();

    return;

  }
  
  void DrawLariatDaq::readData(){


    // Want to be sure that the TChain is ready to go...
    // Do that later.


    std::map<int,int> lar_channel_usage;

    std::vector<std::vector<unsigned short> > * wiredataIN2;
    wiredataIN2 = new std::vector<std::vector<unsigned short>>;
    wiredataIN2->resize(_n_channels);
    for (auto & wire : *wiredataIN2){
      wire.resize(_n_time_ticks);
    }

    // Need to loop over the file 8 times to get all the cards
    int _boards_found = 0;
    // make a guess at where the first card will be:
    unsigned int _n_entry = _current_event*(_n_cards+_card_offset);
    while(_boards_found < 8){



      // First, get the basic information about this entry:
      c -> SetBranchAddress("run",&_run);
      c -> SetBranchAddress("event_counter",&_event_no);
      c -> SetBranchAddress("spill",&_spill);
      c -> SetBranchAddress("board_id",&_board_id);

      c -> GetEntry(_n_entry);
      // std::cout << "Board id: " << _board_id << std::endl;
      // Make some checks:
      // Just skip board 24, always
      if (_board_id == 24){
        _n_entry ++;
        // std::cout << "Continuing\n";
        continue;
      }
      if (_event_no != _current_event){
        std::cout << "event " << _event_no << " in file, but on event " << _current_event << std::endl;
        // then that's an issue, move on
        _n_entry ++;
        continue;
      }

      // std::cout << "Board id is " << _board_id << " (root event " 
      //           << _n_entry 
      //           << ", lariat event " << _event_no << ")"
      //           << std::endl;

      // Now, we know it's not board 24 and it's the right event
      // Set up the channels to read into the storage area
      for(int channel = 0; channel < _n_channels; channel ++ ){
        int plane(0),wire(0);
        int ch = _board_id*_n_channels + channel;
        if (ch < 240 ){
          plane = 0;
          wire = 239 - ch;
        }
        else if (ch < 480 && ch >= 240){
          plane = 1;
          wire = 479-ch;
        }
        if (ch > 480){
          break;
        }
        lar_channel_usage[ch] ++;
        // std::cout << "Mapped board, channel ("<< _board_id << ", " << channel << ") to " 
        //           << "plane, wire (" << plane << ", " << wire << ")\n";

        // Now we know which part of the data to read this channel into;
        char name[20];
        sprintf(name,"channel_%i",channel);
        c -> SetBranchAddress(name,
            &(wiredataIN2->at(channel)[0]));

        if (lar_channel_usage[ch] > 1){
          std::cout << "Found a duplicate channel at " << ch << std::endl;
        }
      }

      // Read in the data:
      c -> GetEntry(_n_entry);

      // Now, pedestal subtract and then copy the data into place
      for(int channel = 0; channel < _n_channels; channel ++ ){
        int plane(0),wire(0);
        // Map the channel again
        int ch = _board_id*_n_channels + channel;
        if (ch < 240 ){
          plane = 0;
          wire = 239 - ch;
        } 
        else if (ch < 480 && ch >= 240){
          plane = 1;
          wire = 479-ch;
        }
        if (ch > 480){
          break;
        }
        auto & wireVec = wiredataIN2 -> at(channel);
        float pedestal =0;
        for (auto & tick : wireVec){
          pedestal += tick;
        }
        pedestal /= _n_time_ticks;
        for (unsigned int tick = 0; tick < wireVec.size(); tick++){
          _planeData.at(plane).at(wire*_y_dimensions.at(plane) + tick) = wireVec.at(tick) - pedestal;
          // And, delete the data that was just copied:
          // wireVec.at(tick) = 0;
        }
      }



      if (_board_id == 24 )
        std::cerr << "Got board_id 24, might be an issue ..." << std::endl;

      _boards_found ++;
      _n_entry ++;
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
      _n_events  = c -> GetEntries() / (_n_cards + _card_offset);
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
