#ifndef LARLITE_SEED_CXX
#define LARLITE_SEED_CXX

#include "seed.h"

namespace larlite {

  //----------------------------------------------------------------------------
  seed::seed() : data_base(data::kSeed)
  {
    fIsValid=false;
  }

  //----------------------------------------------------------------------------
  seed::seed(double* Pt, double* Dir) : data_base(data::kSeed)
  {
    for(int i=0; i!=3; i++)
      {
	fSeedPoint[i]          = Pt[i];
	fSeedDirection[i]      = Dir[i];
	fSeedPointError[i]     = 0;
	fSeedDirectionError[i] = 0;
      }
    fIsValid=true;
  }

  //----------------------------------------------------------------------------
  seed::seed(double* Pt, double* Dir, double* PtErr, double* DirErr) : data_base(data::kSeed)
  {
    for(int i=0; i!=3; i++)
      {
	fSeedPoint[i]          = Pt[i];
	fSeedDirection[i]      = Dir[i];
	fSeedPointError[i]     = PtErr[i];
	fSeedDirectionError[i] = DirErr[i];
      }
    fIsValid=true;
  }

  //----------------------------------------------------------------------------
  bool seed::IsValid() const
  {
    return fIsValid;
  }

  //----------------------------------------------------------------------------
  seed seed::Reverse()
  {
    double NewSeedDir[3];
    for(size_t n=0; n!=3; ++n)
      {
	NewSeedDir[n]=-fSeedDirection[n];
      }
    return seed(fSeedPoint, NewSeedDir, fSeedPointError, fSeedDirectionError);

  }

  //----------------------------------------------------------------------------
  void seed::SetValidity(bool Validity)
  {
    fIsValid=Validity;
  }



  //----------------------------------------------------------------------------
  void seed::GetDirection(double * rDir, double* rDirErr) const
  {
    for(int i=0; i!=3; i++)
      {
	rDir[i]    = fSeedDirection[i];
	rDirErr[i] = fSeedDirectionError[i];
      }
  }

  //----------------------------------------------------------------------------
  void seed::GetPoint(double * rPt, double* rPtErr) const
  {
    for(int i=0; i!=3; i++)
      {
	rPt[i]    = fSeedPoint[i];
	rPtErr[i] = fSeedPointError[i];
      }
  }


  //----------------------------------------------------------------------------
  void seed::SetDirection( double * Dir)
  {
    double Empty[3]={0,0,0};
    SetDirection(Dir, Empty);
  }

  //----------------------------------------------------------------------------
  void seed::SetPoint( double * Pt)
  {
    double Empty[3]={0,0,0};
    SetPoint(Pt, Empty);
  }

  //----------------------------------------------------------------------------
  void seed::SetDirection( double * Dir, double * Err)
  {
    for(int i=0; i!=3; i++)
      {
	fSeedDirection[i]      = Dir[i];
	fSeedDirectionError[i] = Err[i];
      }
    fIsValid=true;
  }

  //----------------------------------------------------------------------------
  void seed::SetPoint(double* Pt, double* Err)
  {
    for(int i=0; i!=3; i++)
      {
	fSeedPoint[i]      = Pt[i];
	fSeedPointError[i] = Err[i];
      }
    fIsValid=true;
  }

  //----------------------------------------------------------------------------
  double seed::GetLength() const
  {
    return pow(pow(fSeedDirection[0],2)+
	       pow(fSeedDirection[1],2)+
	       pow(fSeedDirection[2],2),0.5);
  }

  //----------------------------------------------------------------------------
  double seed::GetProjAngleDiscrepancy(seed const & AnotherSeed) const
  {
    double OtherPt[3];
    double OtherPtErr[3];
    
    AnotherSeed.GetPoint(     OtherPt,  OtherPtErr  );

    TVector3 OtherPtV(  OtherPt[0],  OtherPt[1],  OtherPt[2]  );
    TVector3 ThisDirV(  fSeedDirection[0], fSeedDirection[1], fSeedDirection[2] );
    TVector3 ThisPtV(   fSeedPoint[0],     fSeedPoint[1],     fSeedPoint[2]     );

    return (OtherPtV-ThisPtV).Angle(ThisDirV.Unit());
  }

  //----------------------------------------------------------------------------
  void seed::GetVectorBetween(seed const & AnotherSeed, double* xyz) const
  {
    double xyzother[3], err[3];
    AnotherSeed.GetPoint(xyzother,err);

    xyz[0] = xyzother[0]-fSeedPoint[0];
    xyz[1] = xyzother[1]-fSeedPoint[1];
    xyz[2] = xyzother[2]-fSeedPoint[2];

  }

  //----------------------------------------------------------------------------
  double seed::GetAngle( seed const & AnotherSeed) const
  {
    double OtherDir[3];
    double OtherDirErr[3];
    AnotherSeed.GetDirection(OtherDir,OtherDirErr);

    double OtherMag = AnotherSeed.GetLength();
    double ThisMag  = GetLength();

    /*
      std::cout<<"seed angle calc: " <<
      OtherMag<< " " << ThisMag << " " <<
      ( (OtherDir[0]*fSeedDirection[0] +
   OtherDir[1]*fSeedDirection[1] +
   OtherDir[2]*fSeedDirection[2] ) / (ThisMag*OtherMag)) <<
      std::endl;
    */


    // Need a tiny offset, as acos(1.0) gives unpredictable results due to
    // floating point precision
    double eta = 0.00001;

    return std::acos( ( OtherDir[0]*fSeedDirection[0] +
			OtherDir[1]*fSeedDirection[1] +
			OtherDir[2]*fSeedDirection[2] - eta) / (ThisMag*OtherMag));
  }

  //----------------------------------------------------------------------------
  double seed::GetProjDiscrepancy(seed const & AnotherSeed) const
  {
    double OtherPt[3];
    double OtherPtErr[3];

    AnotherSeed.GetPoint(     OtherPt,  OtherPtErr  );

    TVector3 OtherPtV(  OtherPt[0],  OtherPt[1],  OtherPt[2]  );
    TVector3 ThisDirV(  fSeedDirection[0], fSeedDirection[1], fSeedDirection[2] );
    TVector3 ThisPtV(   fSeedPoint[0],     fSeedPoint[1],     fSeedPoint[2]     );

    
    return ((OtherPtV-ThisPtV) - ThisDirV.Unit()*(ThisDirV.Unit().Dot(OtherPtV-ThisPtV))).Mag();
  }

  //----------------------------------------------------------------------------
  double seed::GetDistance(seed const & AnotherSeed) const
  {
    double OtherPt[3];
    double OtherPtErr[3];

    AnotherSeed.GetPoint(     OtherPt,  OtherPtErr  );

    return pow( pow(OtherPt[0]-fSeedPoint[0],2)+
		pow(OtherPt[1]-fSeedPoint[1],2)+
		pow(OtherPt[2]-fSeedPoint[2],2), 0.5);
  }


  //----------------------------------------------------------------------------
  double seed::GetDistanceFrom(::larlite::spacepoint const & SomePoint) const
  {
    double SPxyz[3];
    SPxyz[0]=SomePoint.XYZ()[0];
    SPxyz[1]=SomePoint.XYZ()[1];
    SPxyz[2]=SomePoint.XYZ()[2];

    //  std::cout<<"seed Dir Vec " << fSeedDirection[0]<<" " <<fSeedDirection[1]<< " " << fSeedDirection[2]<<std::endl;

    double ThisSeedLength =
      pow( pow(fSeedDirection[0],2) +
	   pow(fSeedDirection[1],2) +
	   pow(fSeedDirection[2],2) , 0.5);


    double SPProjOnSeed  =
      ( fSeedDirection[0] * ( SPxyz[0] - fSeedPoint[0] ) +
	fSeedDirection[1] * ( SPxyz[1] - fSeedPoint[1] ) +
	fSeedDirection[2] * ( SPxyz[2] - fSeedPoint[2] ) )
      / ThisSeedLength;
    
    // std::cout<<"proj : " <<SPProjOnSeed<<std::endl;
    // std::cout<<"seed len :" << ThisSeedLength<<std::endl;

    if(SPProjOnSeed > (ThisSeedLength))
      {
	//  std::cout<<"seed over end"<<std::endl;
	return
	  pow(pow(fSeedPoint[0] + fSeedDirection[0] - SPxyz[0], 2) +
	      pow(fSeedPoint[1] + fSeedDirection[1] - SPxyz[1], 2) +
	      pow(fSeedPoint[2] + fSeedDirection[2] - SPxyz[2], 2), 0.5);
      }
    else if(SPProjOnSeed<(0-ThisSeedLength))
      {
	//  std::cout<<"seed under end"<<std::endl;
	return
	  pow(pow(fSeedPoint[0] - fSeedDirection[0] - SPxyz[0], 2) +
	      pow(fSeedPoint[1] - fSeedDirection[1] - SPxyz[1], 2) +
	      pow(fSeedPoint[2] - fSeedDirection[2] - SPxyz[2], 2), 0.5);
	
      }
    else
      {
	//  std::cout<<"seed valid region"<<std::endl;
	double crossprod[3];
	CrossProd( fSeedPoint[0]+fSeedDirection[0]-SPxyz[0],
		   fSeedPoint[1]+fSeedDirection[1]-SPxyz[1],
		   fSeedPoint[2]+fSeedDirection[2]-SPxyz[2],
		   SPxyz[0]-fSeedPoint[0],
		   SPxyz[1]-fSeedPoint[1],
		   SPxyz[2]-fSeedPoint[2],
		   crossprod[0], crossprod[1], crossprod[2]);
	
	return
	  pow( pow(crossprod[0],2) +
	       pow(crossprod[1],2) +
	       pow(crossprod[2],2), 0.5) /
	  pow( pow(fSeedDirection[0],2) +
	       pow(fSeedDirection[1],2) +
	       pow(fSeedDirection[2],2), 0.5);
	
      }
  }

  //----------------------------------------------------------------------------
  int seed::GetPointingSign(seed const & AnotherSeed) const
  {
    double OtherPos[3], OtherErr[3];
    AnotherSeed.GetPoint(OtherPos,OtherErr);
    double DotProd = 
      (OtherPos[0]-fSeedPoint[0])*fSeedDirection[0]+
      (OtherPos[1]-fSeedPoint[1])*fSeedDirection[1]+
      (OtherPos[2]-fSeedPoint[2])*fSeedDirection[2];
    return ((DotProd>0)-(DotProd<0));
  }

  //----------------------------------------------------------------------------
  void seed::CrossProd(double x1,    double x2,    double x3,
		       double y1,    double y2,    double y3,
		       double& out1, double& out2, double& out3) const
  {
    out1 = (x2*y3-x3*y2);
    out2 = (x3*y1-x1*y3);
    out3 = (x1*y2-x2*y1);
  }
}


#endif
