#ifndef ERTOOL_EREFFCORRECTANA_CXX
#define ERTOOL_EREFFCORRECTANA_CXX

#include "EREffCorrectAna.h"

namespace ertool {

  EREffCorrectAna::EREffCorrectAna(const std::string& name) : AnaBase(name)
  , _1pi0MassReco(nullptr)
  , _1pi0MomentumReco(nullptr)
  , _1pi0MomentumRecoX(nullptr)
  , _1pi0MomentumRecoY(nullptr)
  , _1pi0MomentumRecoZ(nullptr)
  , _1pi0CosBeamReco(nullptr)
  , _1pi0MassMC(nullptr)
  , _1pi0MomentumMC(nullptr)
  , _1pi0CosBeamMC(nullptr)
  , _1pi0MomentumEFF(nullptr)
  , _1pi0CosBeamEFF(nullptr)
  {
	
if(_recon){	
    delete _1pi0MassReco;
    _1pi0MassReco = new TH1D("MASSRECO","#pi^{0} Mass ; Mass [MeV]; Events/MeV ",25,0,500);
    //_1pi0MassReco = new TH1D("MASSRECO","#pi^{0} Mass ; Mass [MeV]; Events/MeV ",200,0,500);
    delete _1pi0MomentumReco;
    _1pi0MomentumReco = new TH1D("MOMENTUMRECO","#pi^{0} Momentum ; Momentum [MeV]; Events/MeV ",12,0,1200);
    delete _1pi0MomentumRecoX;
    _1pi0MomentumRecoX = new TH1D("MOMENTUMRECOX","#pi^{0} Momentum ; Momentum [MeV]; Events/MeV ",20,-1,1);
    delete _1pi0MomentumRecoY;
    _1pi0MomentumRecoY = new TH1D("MOMENTUMRECOY","#pi^{0} Momentum ; Momentum [MeV]; Events/MeV ",20,-1,1);
    delete _1pi0MomentumRecoZ;
    _1pi0MomentumRecoZ = new TH1D("MOMENTUMRECOZ","#pi^{0} Momentum ; Momentum [MeV]; Events/MeV ",20,-1,1);
    delete _1pi0CosBeamReco;
    _1pi0CosBeamReco = new TH1D("COSBEAMRECO","#pi^{0} CosBeam ; Cos(theta); Events ",10,-1,1);
    delete _1pi0MassMC;
    _1pi0MassMC = new TH1D("MASSMC","#pi^{0} Mass ; Mass [MeV]; Events/MeV ",200,0,500);
    delete _1pi0MomentumMC;
    _1pi0MomentumMC = new TH1D("MOMENTUMMC","#pi^{0} Momentum ; Momentum [MeV]; Events/MeV ",12,0,1200);
    delete _1pi0CosBeamMC;
    _1pi0CosBeamMC = new TH1D("COSBEAMMC","#pi^{0} CosBeam ; Cos(theta); Events ",10,-1,1);

    delete _1pi0MomentumEFF;
    _1pi0MomentumEFF = new TH1D("MOMENTUMEFF","#pi^{0} Momentum ; Momentum [MeV]; Events/MeV ",12,0.,1200.);
    delete _1pi0CosBeamEFF;
    _1pi0CosBeamEFF = new TH1D("COSBEAMEFF","#pi^{0} CosBeam ; Cos(theta); Events ",10,-1.,1.);
}

	}

  void EREffCorrectAna::Reset()
  {}

  void EREffCorrectAna::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void EREffCorrectAna::ProcessBegin()
  { 
     _nGoodEvents = 0;
     _nRecoEvents = 0;
     _nEvents = 0;
	}

  bool EREffCorrectAna::Analyze(const EventData &data, const ParticleGraph &graph)
  { 
	_nEvents++;
        auto mcgraph = MCParticleGraph();
        auto mceventdata = MCEventData();
//====================================================================================================================================
// Just do the pi0 info first
        auto particles = graph.GetParticleArray();
	unsigned int pi0id =-999;	
	int  pi0counter = 0;
	// check to see if we have reco a pi0 that is primary 
//	for(auto const & p : particles){if(p.PdgCode()==111 && p.Primary() ){pi0id = p.ID(); pi0counter++;}}
	for(auto const & p : particles){
//		if(p.PdgCode()==111  ){	
		if(p.PdgCode()==111 && p.Primary() ){	
				auto sibs = graph.GetSiblingNodes(p.ID());
				for( auto const  sib: sibs){
					auto sibpart = graph.GetParticle(sib);
					std::cout<<"Siblings Pid "<< sibpart.PdgCode()<<std::endl;
					}
auto DetW = larutil::Geometry::GetME()->DetHalfWidth();
auto DetH = larutil::Geometry::GetME()->DetHalfHeight();
auto DetL = larutil::Geometry::GetME()->DetLength();
                double x = p.Vertex()[0];
                double y = p.Vertex()[1];
                double z = p.Vertex()[2];
		std::cout<< " ###x "<< x <<std::endl;
		std::cout<< " ###y "<< y <<std::endl;
		std::cout<< " ###z "<< z <<std::endl;
                if(x<2.*DetW-_fid && x>0.+_fid && y<DetH-_fid && y>-DetH+_fid && z<DetL-_fid && z> 0.0 +_fid){
		std::cout<<"IN HERE NOW RG BREAK CHECK "<<std::endl;
				pi0id = p.ID(); pi0counter++;}
				}
}


	//if(pi0counter!=1) return false;
	// Now for MC
	auto mcp =  mcgraph.GetParticleArray();
	unsigned int mcid =-999;	
	int mcpi0counter = 0;
	for(auto const & m : mcp){
		if(m.PdgCode()==111 && m.Generation()==1){
		auto DetW = larutil::Geometry::GetME()->DetHalfWidth();
		auto DetH = larutil::Geometry::GetME()->DetHalfHeight();
		auto DetL = larutil::Geometry::GetME()->DetLength();
                double x = m.Vertex()[0];
                double y = m.Vertex()[1];
                double z = m.Vertex()[2];
                if(x<2.*DetW-_fid && x>0.+_fid && y<DetH-_fid && y>-DetH+_fid && z<DetL-_fid && z> 0.0 +_fid){
		 mcid = m.ID(); mcpi0counter++;}
		}
		}
//		 mcid = m.ID(); mcpi0counter++;}}// for single particle
	//for(auto const & m : mcp){if(m.PdgCode()==111 && m.Generation()==1){ mcid = m.ID(); mcpi0counter++;}}
	
//if(mcpi0counter!=1){ std::cout<<" PI0 COUNTER NOT ONE"<<std::endl; std::cout<<mcgraph.Diagram();}
	// Fill for Reco
	if(pi0counter==1){ 
	 auto pi0 = graph.GetParticle(pi0id);
	double CosBeam = pi0.Momentum()[2]/pi0.Momentum().Length();
	_1pi0MassReco->Fill(pi0.Mass());
	//_1pi0MassReco->Fill(pi0.Mass()*1000.);
	_1pi0MomentumReco->Fill(pi0.Momentum().Length());
	_1pi0MomentumRecoX->Fill(pi0.Momentum()[0]/pi0.Momentum().Length());
	_1pi0MomentumRecoY->Fill(pi0.Momentum()[1]/pi0.Momentum().Length());
	_1pi0MomentumRecoZ->Fill(pi0.Momentum()[2]/pi0.Momentum().Length());
	_1pi0CosBeamReco->Fill(CosBeam);
	}
	// FIll For MC
	if(mcpi0counter==1){ 
	auto mcpi0 = mcgraph.GetParticle(mcid);
	double MCCosBeam = mcpi0.Momentum()[2]/mcpi0.Momentum().Length();
	_1pi0MassMC->Fill(mcpi0.Mass());
	_1pi0MomentumMC->Fill(mcpi0.Momentum().Length());
	_1pi0CosBeamMC->Fill(MCCosBeam);
	}
	
	// For filtering out bg events use line below
	if(mcpi0counter!=1) return false;

	return true; }



			 //        auto pi0 = graph.GetParticle(recopi0id);
		//		 auto dr = mcpi0.Vertex() - pi0.Vertex();
		//		auto pad = acos(pi0.Momentum()[2]/pi0.Momentum().Length())-acos(mcpi0.Momentum()[2]/mcpi0.Momentum().Length());
		//		 auto dmass = mcpi0.Mass() - pi0.Mass();
		//		auto massval = pi0.Mass();
		//		 auto dmomentum = mcpi0.Momentum().Length() - pi0.Momentum().Length();
//
//
//				 _VertexResolution3D->Fill(dr.Length());
//				 _VertexResolutionX->Fill(dr[0]);
//				 _VertexResolutionY->Fill(dr[1]);
//				 _VertexResolutionZ->Fill(dr[2]);
//				 _ProductionAngleResolution->Fill(pad*180./3.14159265);	
//				 _MASS->Fill(massval);	
//				 _MassResolution->Fill(dmass);	
//				 _MomentumResolution->Fill(dmomentum);	
//
//	
//				_Pi0EffvsMomentumRECO->Fill(mcpi0.Momentum().Length());








  void EREffCorrectAna::ProcessEnd(TFile* fout)
  {
	//--Perm. Histos--
if(_recon){



	_1pi0MassReco->SetLineWidth(2);
	_1pi0MassReco->SetLineColor(2);
	_1pi0MassReco->Write();
	_1pi0MomentumReco->SetLineWidth(2);
	_1pi0MomentumReco->SetLineColor(2);
	_1pi0MomentumReco->Write();
	_1pi0MomentumRecoX->SetLineWidth(2);
	_1pi0MomentumRecoX->SetLineColor(2);
	_1pi0MomentumRecoX->Write();
	_1pi0MomentumRecoY->SetLineWidth(2);
	_1pi0MomentumRecoY->SetLineColor(2);
	_1pi0MomentumRecoY->Write();
	_1pi0MomentumRecoZ->SetLineWidth(2);
	_1pi0MomentumRecoZ->SetLineColor(2);
	_1pi0MomentumRecoZ->Write();
	_1pi0CosBeamReco->SetLineWidth(2);
	_1pi0CosBeamReco->SetLineColor(2);
	_1pi0CosBeamReco->Write();

	_1pi0MassMC->SetLineWidth(2);
	_1pi0MassMC->SetLineColor(1);
	_1pi0MassMC->Write();
	_1pi0MomentumMC->SetLineWidth(2);
	_1pi0MomentumMC->SetLineColor(1);
	_1pi0MomentumMC->Write();
	_1pi0CosBeamMC->SetLineWidth(2);
	_1pi0CosBeamMC->SetLineColor(1);
	_1pi0CosBeamMC->Write();






	// Calculate Eff Histograms

	_1pi0MomentumEFF->Add(_1pi0MomentumReco);
	_1pi0MomentumEFF->Divide(_1pi0MomentumMC);
	_1pi0MomentumEFF->SetMinimum(0.0);
	_1pi0MomentumEFF->SetMaximum(1.0);
	_1pi0MomentumEFF->Write();

	_1pi0CosBeamEFF->Add(_1pi0CosBeamReco);
	_1pi0CosBeamEFF->Divide(_1pi0CosBeamMC);
	_1pi0CosBeamEFF->SetMinimum(0.0);
	_1pi0CosBeamEFF->SetMaximum(1.0);
	_1pi0CosBeamEFF->Write();

/*
	_MASS->SetLineWidth(2);
	_MASS->SetLineColor(2);
	_MASS->Write();
	_MassResolution->SetLineWidth(2);
	_MassResolution->SetLineColor(2);
	_MassResolution->Write();
	_MomentumResolution->SetLineWidth(2);
	_MomentumResolution->SetLineColor(2);
	_MomentumResolution->Write();
	_VertexResolution3D->SetLineWidth(2);
	_VertexResolution3D->SetLineColor(2);
	_VertexResolution3D->Write();
	_VertexResolutionX->SetLineWidth(2);
	_VertexResolutionX->SetLineColor(2);
	_VertexResolutionX->Write();
	_VertexResolutionY->SetLineWidth(2);
	_VertexResolutionY->SetLineColor(2);
	_VertexResolutionY->Write();
	_VertexResolutionZ->SetLineWidth(2);
	_VertexResolutionZ->SetLineColor(2);
	_VertexResolutionZ->Write();
	 _ProductionAngleResolution->SetLineWidth(2);
	 _ProductionAngleResolution->SetLineColor(2);
	 _ProductionAngleResolution->Write();

*/
}
	


/*



	_MASS->SetLineWidth(2);
	_MASS->SetLineColor(4);
	_MASS->Write();
	_MassResolution->SetLineWidth(2);
	_MassResolution->SetLineColor(4);
	_MassResolution->Write();
	_MomentumResolution->SetLineWidth(2);
	_MomentumResolution->SetLineColor(4);
	_MomentumResolution->Write();
	_VertexResolution3D->SetLineWidth(2);
	_VertexResolution3D->SetLineColor(4);
	_VertexResolution3D->Write();
	_VertexResolutionX->SetLineWidth(2);
	_VertexResolutionX->SetLineColor(4);
	_VertexResolutionX->Write();
	_VertexResolutionY->SetLineWidth(2);
	_VertexResolutionY->SetLineColor(4);
	_VertexResolutionY->Write();
	_VertexResolutionZ->SetLineWidth(2);
	_VertexResolutionZ->SetLineColor(4);
	_VertexResolutionZ->Write();
	 _ProductionAngleResolution->SetLineWidth(2);
	 _ProductionAngleResolution->SetLineColor(4);
	 _ProductionAngleResolution->Write();


	_Pi0EffvsMomentumMC->Write();
	_Pi0EffvsMomentumRECO->Write();
	_Pi0EffvsMomentum->Clone("Pi0EffvsMomentumRECO");
	_Pi0EffvsMomentum->Divide(_Pi0EffvsMomentumMC);
	_Pi0EffvsMomentum->Write();


*/
	


/*
	_mc->Write();
	_reco->Write();
	_comp->Write();
	
	std::cout<<"Total Events in the file "<< _nEvents<<std::endl;
	std::cout<<"Total Good Events "<< _nGoodEvents<<std::endl;
	std::cout<<"Reco Efficiency"<< (double)_nRecoEvents/_nGoodEvents<<std::endl;
*/
	}

}

#endif
