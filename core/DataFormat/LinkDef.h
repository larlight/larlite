//
// cint script to generate libraries
// Declaire namespace & classes you defined
// #pragma statement: order matters! Google it ;)
//

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ class std::vector<std::vector<float> >+;
#pragma link C++ class std::vector<std::vector<double> >+;

//#pragma link C++ class std::pair<size_t,size_t>+;
#pragma link C++ class std::pair<unsigned short,string>+;
//#pragma link C++ class std::pair<string,bool>+;
#pragma link C++ class std::pair<string,string>+;

#pragma link C++ class std::map<string,bool>+;
#pragma link C++ class std::map<string,string>+;
#pragma link C++ class std::map<string,std::vector<bool> >+;
#pragma link C++ class std::map<string,std::vector<int > >+;
#pragma link C++ class std::map<string,std::vector<double> >+;
#pragma link C++ class std::map<string,std::vector<string> >+;
#pragma link C++ class std::map<larlite::data::DataType_t,std::set<string> >+;
#pragma link C++ class std::vector<TVector3>+;
#pragma link C++ class std::vector<TMatrixD>+;
#pragma link C++ class std::vector<TLorentzVector>+;

//#pragma link C++ class std::vector<unsigned int>+;
//#pragma link C++ class std::vector<std::vector<unsigned int> >+;
//#pragma link C++ class larlite::AssUnit_t+;
#pragma link C++ class std::vector<larlite::AssUnit_t>+;
#pragma link C++ class larlite::AssSet_t+;

#pragma link C++ class std::pair<larlite::data::DataType_t,string>+;
#pragma link C++ class larlite::product_id+;
#pragma link C++ class std::vector<larlite::product_id>+;
#pragma link C++ class std::pair<larlite::product_id,larlite::product_id>+;
#pragma link C++ class std::vector<std::pair<larlite::product_id,larlite::product_id> >+;

#pragma link C++ class larlite::DataFormatException+;

#pragma link C++ class larlite::data_base+;
#pragma link C++ class larlite::output_base+;
#pragma link C++ class larlite::run_base+;
#pragma link C++ class larlite::subrun_base+;
#pragma link C++ class larlite::event_base+;
//#pragma link C++ class larlite::association+;

#pragma link C++ class larlite::sparse_data<float>+;
#pragma link C++ class std::vector<larlite::sparse_data<float> >+;
#pragma link C++ class larlite::sparse_vector<float>+;

#pragma link C++ class larlite::potsummary+;

#pragma link C++ class larlite::rawdigit+;
#pragma link C++ class std::vector<larlite::rawdigit>+;
#pragma link C++ class larlite::event_rawdigit+;

#pragma link C++ class larlite::daqheadertimeuboone+;
#pragma link C++ class std::vector<larlite::daqheadertimeuboone>+;
#pragma link C++ class larlite::event_daqheadertimeuboone+;

#pragma link C++ class larlite::wire+;
#pragma link C++ class std::vector<larlite::wire>+;
#pragma link C++ class larlite::event_wire+;

#pragma link C++ class larlite::hit+;
#pragma link C++ class std::vector<larlite::hit>+;
#pragma link C++ class larlite::event_hit+;

#pragma link C++ class larlite::crthit+;
#pragma link C++ class std::vector<larlite::crthit>+;
#pragma link C++ class larlite::event_crthit+;

#pragma link C++ class larlite::crttrack+;
#pragma link C++ class std::vector<larlite::crttrack>+;
#pragma link C++ class larlite::event_crttrack+;

#pragma link C++ class larlite::t0+;
#pragma link C++ class std::vector<larlite::t0>+;
#pragma link C++ class larlite::event_t0+;

#pragma link C++ class larlite::ophit+;
#pragma link C++ class std::vector<larlite::ophit>+;
#pragma link C++ class larlite::event_ophit+;

#pragma link C++ class larlite::opflash+;
#pragma link C++ class std::vector<larlite::opflash>+;
#pragma link C++ class larlite::event_opflash+;

#pragma link C++ class larlite::cosmictag+;
#pragma link C++ class std::vector<larlite::cosmictag>+;
#pragma link C++ class larlite::event_cosmictag+;

#pragma link C++ class larlite::cluster+;
#pragma link C++ class std::vector<larlite::cluster>+;
#pragma link C++ class larlite::event_cluster+;

#pragma link C++ class larlite::endpoint2d+;
#pragma link C++ class std::vector<larlite::endpoint2d>+;
#pragma link C++ class larlite::event_endpoint2d+;

#pragma link C++ class larlite::seed+;
#pragma link C++ class std::vector<larlite::seed>+;
#pragma link C++ class larlite::event_seed+;

#pragma link C++ class larlite::spacepoint+;
#pragma link C++ class std::vector<larlite::spacepoint>+;
#pragma link C++ class larlite::event_spacepoint+;

#pragma link C++ class larlite::track+;
#pragma link C++ class std::vector<larlite::track>+;
#pragma link C++ class larlite::event_track+;

#pragma link C++ class larlite::trackmom+;
#pragma link C++ class std::vector<larlite::trackmom>+;
#pragma link C++ class larlite::event_trackmom+;

#pragma link C++ class larlite::shower+;
#pragma link C++ class std::vector<larlite::shower>+;
#pragma link C++ class larlite::event_shower+;

#pragma link C++ class larlite::vertex+;
#pragma link C++ class std::vector<larlite::vertex>+;
#pragma link C++ class larlite::event_vertex+;

#pragma link C++ class larlite::calorimetry+;
#pragma link C++ class std::vector<larlite::calorimetry>+;
#pragma link C++ class larlite::event_calorimetry+;

#pragma link C++ class larlite::pfpart+;
#pragma link C++ class std::vector<larlite::pfpart>+;
#pragma link C++ class larlite::event_pfpart+;

#pragma link C++ class larlite::partid+;
#pragma link C++ class std::vector<larlite::partid>+;
#pragma link C++ class larlite::event_partid+;

#pragma link C++ class larlite::user_info+;
#pragma link C++ class std::vector<larlite::user_info>+;
#pragma link C++ class larlite::event_user+;

#pragma link C++ class larlite::gtruth+;
#pragma link C++ class std::vector<larlite::gtruth>+;
#pragma link C++ class larlite::event_gtruth+;

#pragma link C++ class larlite::mcshower+;
#pragma link C++ class std::vector<larlite::mcshower>+;
#pragma link C++ class larlite::event_mcshower+;

#pragma link C++ class larlite::mcpart+;
#pragma link C++ class std::vector<larlite::mcpart>+;
#pragma link C++ class larlite::event_mcpart+;

#pragma link C++ class treenode+;
#pragma link C++ class std::vector<treenode>+;
#pragma link C++ class larlite::mctree+;
#pragma link C++ class std::vector<larlite::mctree>+;
#pragma link C++ class larlite::event_mctree+;

#pragma link C++ class larlite::mcflux+;
#pragma link C++ class std::vector<larlite::mcflux>+;
#pragma link C++ class larlite::event_mcflux+;

#pragma link C++ class larlite::mcstep+;
#pragma link C++ class std::vector<larlite::mcstep>+;
#pragma link C++ class larlite::mctrajectory+;

#pragma link C++ class larlite::mctrack+;
#pragma link C++ class std::vector<larlite::mctrack>+;
#pragma link C++ class larlite::event_mctrack+;

#pragma link C++ class larlite::roi+;
#pragma link C++ class std::vector<larlite::roi>+;
#pragma link C++ class larlite::event_roi+;

#pragma link C++ class larlite::mceventweight+;
#pragma link C++ class std::vector<larlite::mceventweight>+;
#pragma link C++ class larlite::event_mceventweight+;

#pragma link C++ class larlite::mcnu+;
#pragma link C++ class std::vector<larlite::mcnu>+;

#pragma link C++ class larlite::mctruth+;
#pragma link C++ class std::vector<larlite::mctruth>+;
#pragma link C++ class larlite::event_mctruth+;

#pragma link C++ class larlite::ide+;
#pragma link C++ class larlite::simch+;
#pragma link C++ class std::vector<larlite::ide>+;
#pragma link C++ class std::map<UShort_t,larlite::simch>+;
#pragma link C++ class std::vector<larlite::simch>+;
#pragma link C++ class larlite::event_simch+;

#pragma link C++ class larlite::auxide+;
#pragma link C++ class larlite::auxsimch+;
#pragma link C++ class std::vector<larlite::auxide>+;
#pragma link C++ class std::map<UShort_t,larlite::auxsimch>+;
#pragma link C++ class std::vector<larlite::auxsimch>+;
#pragma link C++ class larlite::event_auxsimch+;

#pragma link C++ class larlite::chstatus+;
#pragma link C++ class std::vector<larlite::chstatus>+;
#pragma link C++ class larlite::event_chstatus+;

#pragma link C++ class treenode+;

#pragma link C++ class larlite::trigger+;
#pragma link C++ class larlite::swtrigger+;

#pragma link C++ class larlite::minos+;
#pragma link C++ class std::vector<larlite::minos>+;
#pragma link C++ class larlite::event_minos+;

#pragma link C++ class larlite::pcaxis+;
#pragma link C++ class std::vector<larlite::pcaxis>+;
#pragma link C++ class larlite::event_pcaxis+;

#pragma link C++ class larlite::flashmatch+;
#pragma link C++ class std::vector<larlite::flashmatch>+;
#pragma link C++ class larlite::event_flashmatch+;

#pragma link C++ class larlite::event_ass+;

#pragma link C++ class larlite::fifo+;
#pragma link C++ class std::vector<larlite::fifo>+;
#pragma link C++ class larlite::event_fifo+;

#pragma link C++ class larlite::opdetwaveform+;
#pragma link C++ class std::vector<larlite::opdetwaveform>+;
#pragma link C++ class larlite::event_opdetwaveform+;

#pragma link C++ class larlite::onephoton+;
#pragma link C++ class std::vector<larlite::onephoton>+;
#pragma link C++ class larlite::simphotons+;
#pragma link C++ class std::vector<larlite::simphotons>+;
#pragma link C++ class larlite::event_simphotons+;

#pragma link C++ class larlite::mucsdata+;
#pragma link C++ class std::vector<larlite::mucsdata>+;
#pragma link C++ class larlite::event_mucsdata+;
#pragma link C++ class larlite::mucsreco+;
#pragma link C++ class std::vector<larlite::mucsreco>+;
#pragma link C++ class larlite::event_mucsreco+;

#pragma link C++ class larlite::storage_manager+;
#pragma link C++ class larlite::storage_store+;

#pragma link C++ class larlite::larflow3dhit+;
#pragma link C++ class std::vector<larlite::larflow3dhit>+;
#pragma link C++ class larlite::event_larflow3dhit+;

#pragma link C++ class larlite::larflowcluster+;
#pragma link C++ class std::vector<larlite::larflowcluster>+;
#pragma link C++ class larlite::event_larflowcluster+;

#pragma link C++ class larlite::pixelmask+;
#pragma link C++ class std::vector<larlite::pixelmask>+;
#pragma link C++ class larlite::event_pixelmask+;

/*
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_gtruth>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_mctruth>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_mcpart>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_mcflux>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_simch>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_mcshower>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_rawdigit>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_wire>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_hit>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_ophit>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_opflash>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_cluster>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_seed>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_spacepoint>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_track>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_shower>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_vertex>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_endpoint2d>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_calorimetry>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_partid>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_pfpart>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_user>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_minos>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_ass>();
#pragma link C++ function larlite::storage_manager::data_type<larlite::event_fifo>();

#pragma link C++ function larlite::storage_manager::get_data<larlite::event_gtruth>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_mctruth>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_mcpart>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_mcflux>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_simch>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_mcshower>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_rawdigit>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_wire>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_hit>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_ophit>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_opflash>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_cluster>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_seed>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_spacepoint>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_track>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_shower>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_vertex>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_endpoint2d>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_calorimetry>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_partid>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_pfpart>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_user>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_minos>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_ass>(string);
#pragma link C++ function larlite::storage_manager::get_data<larlite::event_fifo>(string);
#pragma link C++ function larlite::storage_manager::subrundata_type<larlite::potsummary>();
#pragma link C++ function larlite::storage_manager::get_subrundata<larlite::potsummary>(string);
*/
#endif
