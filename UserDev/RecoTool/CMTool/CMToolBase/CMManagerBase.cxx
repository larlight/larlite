#ifndef RECOTOOL_CMMANAGERBASE_CXX
#define RECOTOOL_CMMANAGERBASE_CXX

#include "CMManagerBase.h"

namespace cmtool {

  CMManagerBase::CMManagerBase()
  {
    _fout = 0;
    _debug_mode = kNone;
    _priority_algo = nullptr;
    _min_nhits = 0;
    _merge_till_converge = false;
    Reset();
    _time_report=false;
  }

  void CMManagerBase::Reset()
  {
    _planes.clear();
    _in_clusters.clear();
    if(_priority_algo) _priority_algo->Reset();
  }

  void CMManagerBase::SetClusters(const std::vector<std::vector<larutil::PxHit> > &clusters)
  {

    TStopwatch localWatch;

    // Reset
    this->Reset();

    // Clear & fill cluster info

    _in_clusters.clear();

    _in_clusters.reserve(clusters.size());

    ::cluster::ClusterParamsAlg tmp_alg;
    tmp_alg.SetMinNHits(_min_nhits);
    tmp_alg.SetVerbose(false);

    for(auto const &c : clusters) {
      
      _in_clusters.push_back(tmp_alg);
      (*_in_clusters.rbegin()).Initialize();

      if((*_in_clusters.rbegin()).SetHits(c) < 3) continue;
      (*_in_clusters.rbegin()).DisableFANN();
      //(*_in_clusters.rbegin()).FillParams(true,true,true,true,true,false);
      (*_in_clusters.rbegin()).FillParams(false,false,false,false,false,false);
      (*_in_clusters.rbegin()).FillPolygon();

    }

    if(_time_report) {
      std::cout << Form("  CMManagerBase Time Report: SetClusters (CPAN computation) = %g [s]",
			localWatch.RealTime())
		<< " ... details below." << std::endl;

      for(auto const& c : _in_clusters)

	c.TimeReport();

    }
    
  }

  void CMManagerBase::SetClusters(const std::vector<cluster::ClusterParamsAlg> &clusters)
  {
    TStopwatch localWatch;

    localWatch.Start();

    _in_clusters = clusters;

    if(_time_report) std::cout << Form("  CMManagerBase Time Report: SetClusters (copy) = %g [s]",
				       localWatch.RealTime())
			       << std::endl;
  }

  void CMManagerBase::Process()
  {
    
    if(!(_in_clusters.size()))

      return;

    TStopwatch localWatch;

    localWatch.Start();

    EventBegin();

    if(_time_report) std::cout << Form("  CMManagerBase Time Report: EventBegin = %g [s]",
				     localWatch.RealTime())
			       << std::endl;

    bool keep_going = true;

    while(keep_going) {

      localWatch.Start();

      IterationBegin();

      if(_time_report) std::cout << Form("  CMManagerBase Time Report: IterationBegin = %g [s]",
				       localWatch.RealTime())
				 << std::endl;

      localWatch.Start();
      
      keep_going = IterationProcess();

      if(_time_report) std::cout << Form("  CMManagerBase Time Report: IterationProcess = %g [s]",
					 localWatch.RealTime())
				 << std::endl;
      localWatch.Start();

      IterationEnd();

      if(_time_report) std::cout << Form("  CMManagerBase Time Report: IterationEnd = %g [s]",
					 localWatch.RealTime())
				 << std::endl;

      if(!_merge_till_converge) {

	if(_debug_mode <= kPerIteration)

	  std::cout << "\033[93m  Iterative approach = OFF ... exiting from iteration loop. \033[00m" << std::endl;
	
	break;
      }
    }

    localWatch.Start();

    EventEnd();

    if(_time_report) std::cout << Form("  CMManagerBase Time Report: EventEnd = %g [s]",
				       localWatch.RealTime())
			       << std::endl;
    
  }

  void CMManagerBase::ComputePriority(const std::vector<cluster::ClusterParamsAlg> &clusters) {

    TStopwatch localWatch;
    localWatch.Start();

    _priority.clear();
    _planes.clear();

    if(!clusters.size()) return;

    // Priority is computed cluster-by-cluster. In case of two clusters having the same priority 
    // value the one with lower cluster index gets the priority. Also, clusters with priority < 0
    // are not logged (assumed not to be used)

    for(size_t i=0; i<clusters.size(); ++i) {

      size_t c_index = clusters.size() - i - 1;

      float priority = clusters.at(c_index).GetNHits();
      
      if(_priority_algo) {
	
	priority = _priority_algo->Priority(clusters.at(c_index));

      }
	
      if(priority>0) {
	
	_priority.insert(std::make_pair(priority,c_index));

	if( _planes.find(clusters.at(c_index).Plane()) == _planes.end() )

	  _planes.insert(clusters.at(c_index).Plane());

      }
      
    }
    
    if(_time_report) std::cout << Form("  CMManagerBase Time Report: ComputePriority = %g [s]",
				       localWatch.RealTime())
			       << std::endl;

  }


}

#endif
