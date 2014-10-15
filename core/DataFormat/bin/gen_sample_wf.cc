
#include "DataFormat-TypeDef.hh"
#include <TF1.h>
#include <TMath.h>

int main(){

  //
  // A simple routine to generate a fake PMT waveform data file.
  //

  // Prepare a fake waveform function
  TF1* f1 = new TF1("f1","100*(TMath::Poisson(x,[0]))",-1,21);
  f1->SetParameter(0,3);

  // OK Let's create LArLight file and fill waveform
  larlight::storage_manager my_storage;

  my_storage.set_io_mode(larlight::storage_manager::WRITE);

  my_storage.set_out_filename("sample_pmt_waveform.root");

  my_storage.open();

  if(!my_storage.is_ready_io()) {
    std::cerr << "I/O preparation failed!" << std::endl;
  }

  // Let's fill event_fifo for 100 events.
  larlight::event_pmtfifo* my_event_fifo = (larlight::event_pmtfifo*)(my_storage.get_data(larlight::DATA::PMTFIFO));
  int run_id = 1;
  int subrun_id = 1;

  for( int i=0; i<100; i++){

    int event_id = i;
    my_event_fifo->set_run(run_id);
    my_event_fifo->set_subrun(subrun_id);
    my_event_fifo->set_event_id(event_id);

    // Let's fill 30 channels of PMT waveform all with cosmic discriminator threshold
    int n_channels=30;
    my_event_fifo->reserve(n_channels);
    for( int ch=0; ch<n_channels; ++ch) {

      larlight::pmtfifo wf;
      
      wf.set_channel_number(ch);
      //wf.set_disc_id(larlight::FEM::COSMIC_DISC);
      wf.reserve(20);
      for(size_t wf_index=0; wf_index<20; ++wf_index)

	wf.push_back(f1->Eval((double)(wf_index - i%6)));

      // Append 
      my_event_fifo->push_back(wf);
    }

    // Store & go next event
    my_storage.next_event();

  }

  // End of event loop
  my_storage.close();
  return 1;
}


