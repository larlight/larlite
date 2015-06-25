#ifndef POINTS_H
#define POINTS_H

#include <vector>
#include <iostream>


class Points{

 public:

  Points() { _points.clear(); }

  void AddPoint(float x, float y)
    {
      std::pair<float,float> thisPair;
      thisPair = std::make_pair(x,y);
      _points.push_back(thisPair);
    }

  void PrintPoints()
    {
      std::cout << "Points: [ ";
      for (auto const& p : _points)
	std::cout << "(" << p.first << ", " << p.second << ") ";
      std::cout << " ]" << std::endl;
    }

  std::pair<float,float> GetSinglePoint(float x, float y) 
  {
    std::pair<float,float> thisPair;
    thisPair = std::make_pair(x,y);
    return thisPair;
  }

  std::vector<std::pair<float,float> > GetPoints() { return _points; }
  
  private:
  
  std::vector<std::pair<float,float> > _points;

};

#endif
