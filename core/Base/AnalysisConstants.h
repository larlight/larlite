#ifndef LARLITE_ANALYSISCONSTANTS_H
#define LARLITE_ANALYSISCONSTANTS_H

namespace larlite{

  namespace anab{

    typedef enum cosmic_tag_id{
      kUnknown=-1,
      kNotTagged=0,
      kGeometry_YY=1,
      kGeometry_YZ,
      kGeometry_ZZ,
      kGeometry_XX,
      kGeometry_XY,
      kGeometry_XZ,
      kGeometry_Y=21,
      kGeometry_Z,
      kGeometry_X,
      kOutsideDrift_Partial=100,
      kOutsideDrift_Complete,
      kFlash_BeamIncompatible=200,
      kFlash_Match=300
    } CosmicTagID_t;
  }
}
#endif
