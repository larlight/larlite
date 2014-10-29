#ifndef RECOTOOL_CLUSTERPARAMSEXECUTOR_CXX
#define RECOTOOL_CLUSTERPARAMSEXECUTOR_CXX

#include "ClusterParamsExecutor.h"

namespace cluster {


  ClusterParamsExecutor::ClusterParamsExecutor() : ClusterParamsAlg()
  {
    hCurrentHit = 0;

    _useHitBlurring = false;

    _blurFunction = 0;

  }

  int ClusterParamsExecutor::LoadCluster(::larlite::storage_manager* mgr,
					 const std::string cluster_producer_name,
					 const size_t cluster_index)
  {

    auto ev_cluster = mgr->get_data< ::larlite::event_cluster>(cluster_producer_name);

    auto hit_ass_names = ev_cluster->association_keys(::larlite::data::kHit);

    if(!(hit_ass_names.size()) ||
       !(mgr->get_data< ::larlite::event_hit>(hit_ass_names[0]))) {
      throw cluster::CRUException("Associated hits not found!");
      return -1;
    }

    auto ev_hits = mgr->get_data< ::larlite::event_hit>(hit_ass_names[0]);

    auto const hit_index_v = ev_cluster->association(larlite::data::kHit,hit_ass_names[0],cluster_index);

    if(!hit_index_v.size()) {
      throw cluster::CRUException("Associated hits not found!");
      return -1;
    }

    cluster_hits.clear();

    UChar_t plane = ::larutil::Geometry::GetME()->ChannelToPlane(ev_hits->at(hit_index_v[0]).Channel());

    cluster_hits.reserve(hit_index_v.size());

    for(auto const index : hit_index_v)

      cluster_hits.push_back((const ::larlite::hit*)(&(ev_hits->at(index))));
    
   return Execute(ev_hits->event_id(), cluster_index, plane);
  }

  int ClusterParamsExecutor::LoadAllHits(const ::larlite::event_hit *hits, const UChar_t plane_id)
  {
    
    cluster_hits.clear();
    cluster_hits.reserve(hits->size());

    for(auto &h : *hits) {

      if( larutil::Geometry::GetME()->ChannelToPlane(h.Channel()) == plane_id )
	cluster_hits.push_back((const ::larlite::hit*)(&h));

    }

    return Execute(hits->event_id(), -1, plane_id);
  }

  int ClusterParamsExecutor::Execute(Int_t event_id, Int_t cluster_id, UChar_t plane_id) 
  {

    if(!(cluster_hits.size())) return -1;


    if(hCurrentHit) delete hCurrentHit;
    if(_blurFunction) delete _blurFunction;

    double wire_min=1e9;
    double wire_max=0;
    double time_min=1e9;
    double time_max=0;
    for(auto const h : cluster_hits) {
      
      double hit_wire = h->Wire();
      double hit_time = h->PeakTime();
      
      wire_min = hit_wire < wire_min ? hit_wire : wire_min;
      wire_max = hit_wire > wire_max ? hit_wire : wire_max;
      time_min = hit_time < time_min ? hit_time : time_min;
      time_max = hit_time > time_max ? hit_time : time_max;

    }

    wire_min -= ((wire_max - wire_min)*0.25);
    wire_max += ((wire_max - wire_min)*0.25);
    time_min -= ((time_max - time_min)*0.25);
    time_max += ((time_max - time_min)*0.25);
    //if(wire_min<0) wire_min = 0;
    //if(time_min<0) time_min = 0;

    hCurrentHit = new TH2D("hCurrentHit",
			   Form("Event %d Cluster %d (Plane=%d); Wire [cm]; Time [cm]",
				event_id,
				cluster_id,
				plane_id),
			   100, (fGSer->WireToCm())*wire_min, (fGSer->WireToCm())*wire_max,
			   100, (fGSer->TimeToCm())*time_min, (fGSer->TimeToCm())*time_max);
    			   //int(wire_max - wire_min), (fGSer->WireToCm())*wire_min, (fGSer->WireToCm())*wire_max,
			   //int(time_max - time_min), (fGSer->TimeToCm())*time_min, (fGSer->TimeToCm())*time_max);
    hCurrentHit->SetTitleFont(22);
    hCurrentHit->GetXaxis()->SetTitleFont(22);
    hCurrentHit->GetXaxis()->SetTitleSize(0.05);
    hCurrentHit->GetXaxis()->SetLabelFont(22);
    hCurrentHit->GetXaxis()->SetLabelSize(0.05);
    hCurrentHit->GetYaxis()->SetTitleFont(22);
    hCurrentHit->GetYaxis()->SetTitleSize(0.05);
    hCurrentHit->GetYaxis()->SetLabelFont(22);
    hCurrentHit->GetYaxis()->SetLabelSize(0.05);

    //for blurring in the y (time) direction:
    _blurFunction = new TF1("_blurFunction","[0]*exp(-0.5*((x-[1])/[2])**2)",0.,99999.);

    for(auto h : cluster_hits) {

      if(!_useHitBlurring)
	hCurrentHit->Fill(h->Wire()*(fGSer->WireToCm()), 
			  h->PeakTime()*(fGSer->TimeToCm()), 
			  h->Charge());


      else{
	//center of peak = hit.PeakTime
	_blurFunction->SetParameter(1,h->PeakTime()*(fGSer->TimeToCm()));

	//width of peak (estimated) = avg time b/t peak&start, and peak&end
	double blurfnsigma = 
	  ( (h->PeakTime() - h->StartTime() )
	   +(h->EndTime()  - h->PeakTime()) ) / 2;
	blurfnsigma *= (fGSer->TimeToCm());
	_blurFunction->SetParameter(2,blurfnsigma);
	
	//height of peak = changes to normalize gaussian area to 1 (2.5066 is sqrt(2pi))
	_blurFunction->SetParameter(0, (1/(blurfnsigma*2.5066)) );

	//fill the histogram in y outward from the peak until 97% of charge is added
	Int_t peak_bin = hCurrentHit->GetYaxis()->FindBin(h->PeakTime()*(fGSer->TimeToCm()));
	
	double peak_bin_low  = hCurrentHit->GetYaxis()->GetBinLowEdge(peak_bin);
	double peak_bin_high = hCurrentHit->GetYaxis()->GetBinUpEdge(peak_bin);
	double bin_width = hCurrentHit->GetYaxis()->GetBinWidth(peak_bin);

	double charge_filled = 0.; 
	unsigned int counter = 0;
	double charge_in_bin, bin_low, bin_high;
	
	while( (charge_filled)/(h->Charge()) < 0.97
	       && counter < (unsigned int)(hCurrentHit->GetNbinsY()) ){

	  //if you're dealing with the one peak bin
	  if(!counter){
	    charge_in_bin = (_blurFunction->Integral(peak_bin_low,peak_bin_high)) * h->Charge();
	    
	    hCurrentHit->Fill(h->Wire()*(fGSer->WireToCm()), 
			      h->PeakTime()*(fGSer->TimeToCm()), 
			      charge_in_bin);
	    
	    charge_filled += charge_in_bin;
	  }

	  //if not dealing with the one peak bin, fill next bin above/below
	  else{

	    Int_t current_bin_above_peak = 
	      hCurrentHit->GetYaxis()->FindBin(h->PeakTime()*(fGSer->TimeToCm()) + (counter*bin_width));

	    bin_low = hCurrentHit->GetYaxis()->GetBinLowEdge(current_bin_above_peak);
	    bin_high = hCurrentHit->GetYaxis()->GetBinUpEdge(current_bin_above_peak);
	    
	    charge_in_bin = (_blurFunction->Integral(bin_low,bin_high)) * h->Charge();

	    if(charge_in_bin > 1e-4)
	      hCurrentHit->Fill(h->Wire()*(fGSer->WireToCm()),
				hCurrentHit->GetYaxis()->GetBinCenter(current_bin_above_peak),
				charge_in_bin);
	    charge_filled += charge_in_bin;

	    Int_t current_bin_below_peak = 
	      hCurrentHit->GetYaxis()->FindBin(h->PeakTime()*(fGSer->TimeToCm()) - (counter*bin_width));
	    bin_low = hCurrentHit->GetYaxis()->GetBinLowEdge(current_bin_below_peak);
	    bin_high = hCurrentHit->GetYaxis()->GetBinUpEdge(current_bin_below_peak);

	    charge_in_bin = (_blurFunction->Integral(bin_low,bin_high)) * h->Charge();

	    if(charge_in_bin > 1e-4)
	      hCurrentHit->Fill(h->Wire()*(fGSer->WireToCm()),
				hCurrentHit->GetYaxis()->GetBinCenter(current_bin_below_peak),
				charge_in_bin);
	    charge_filled += charge_in_bin;

	  }
	  counter++;

	}
      }
    }
    
    this->Initialize();
    if(this->SetHits(cluster_hits) == -1 )
    {
     std::cout << "ClusterParamsExecutor: error setting hits. Hitlist may be too small.  " <<std::endl;
     return -1;
    }
   return 0; 
  }

}

#endif
