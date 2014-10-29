#ifndef POINTTOLINEDIST_CXX
#define POINTTOLINEDIST_CXX

#include "PointToLineDist.h"

double PointToLineDist::Distance(){

  if ( (_point.size() == 0) or (_segmentStart.size() == 0) or (_segmentEnd.size() == 0) ){
    std::cout << "Error: point and segment not initialized." << std::endl;
    return 0;
  }

  if ( (_point.size() != 3) or (_segmentStart.size() != 3) or (_segmentEnd.size() != 3) ){
    std::cout << "Error: Did not provide 3D points." << std::endl;
    return 0;
  }

  //Distance Algorithm: see "Real-Time Collision Analysis" book, section 5.1.2.1

  std::vector<float> line; // End-Start
  std::vector<float> pToStart; // point - Start
  std::vector<float> pToEnd;  // point - End

  for (size_t i=0; i < _point.size(); i++){
    line.push_back(_segmentEnd.at(i)-_segmentStart.at(i));
    pToStart.push_back(_point.at(i)-_segmentStart.at(i));
    pToEnd.push_back(_point.at(i)-_segmentEnd.at(i));
  }

  float e = DotProduct(pToStart,line);
  float segLenSquared = DotProduct(line,line);
  if (e <= 0.) return sqrt(DotProduct(pToStart,pToStart));
  else if (e > segLenSquared) return sqrt(DotProduct(pToEnd,pToEnd));
  else return sqrt( DotProduct(pToStart,pToStart)-(e*e)/segLenSquared );

}


double PointToLineDist::DotProduct(std::vector<float> A, std::vector<float> B){
    
    if (A.size() != B.size()){
      std::cout << "Error: Did not provide equal-dimension points" << std::endl;
      return 0;
    }

    double dotProd = 0.;
    for (size_t i=0; i < A.size(); i++)
      dotProd += A.at(i)*B.at(i);

    return dotProd;
  }

void PointToLineDist::TestDist(){

  bool cont = true;
  int next;

  std::vector<float> point(3,1);
  std::vector<float> A;
  std::vector<float> B;
  float Ax,Ay,Az,Bx,By,Bz;
  
  while (cont){

    std::cout << "A - x coord:" << std::endl;
    std::cin >> Ax;
    std::cout << "A - y coord:" << std::endl;
    std::cin >> Ay;
    std::cout << "A - z coord:" << std::endl;
    std::cin >> Az;
    std::cout << "B - x coord:" << std::endl;
    std::cin >> Bx;
    std::cout << "B - y coord:" << std::endl;
    std::cin >> By;
    std::cout << "B - z coord:" << std::endl;
    std::cin >> Bz;
    A.clear();
    B.clear();
    A.push_back(Ax);
    A.push_back(Ay);
    A.push_back(Az);
    B.push_back(Bx);
    B.push_back(By);
    B.push_back(Bz);

    double D = Distance(point,A,B);
    std::cout << "Calculated distance is: " << D << std::endl;

    std::cout << std::endl << "Next Event? 1: Yes, 0: No." << std::endl;
    std::cin >> next;
    if (next) { cont = true; }
    else { cont = false; }
  }

  return;
}

#endif
