/**
 * \file SelIIA.h
 *
 * \ingroup CCInclusive
 *
 * \brief Kaleko wrote this based on Xiao's description of CCInclusive Selection IIA Filter.
 *        He wrote this for his own personal analysis so if there are bugs or if performance differs
 *        slightly from what the CCInclusive group has shown, the author is sorry but isn't 
 *        necessarily responsible for fixing them!
 *
 * @author davidkaleko
 */

/** \addtogroup CCInclusive

    @{*/
#ifndef LARLITE_SELIIA_H
#define LARLITE_SELIIA_H

#include <iostream>
#include "Analysis/ana_base.h"
#include "DataFormat/track.h"
#include "DataFormat/vertex.h"
#include "DataFormat/calorimetry.h"
#include "GeoAlgo/GeoVector.h"
#include "GeoAlgo/GeoSphere.h"
#include "GeoAlgo/GeoAABox.h"
#include "CCInclConstants.h"
#include "CCFidVolBox.h"

/**
   \class SelIIA
   User defined class SelIIA ... these comments are used to generate
   doxygen documentation!
 */

namespace larlite {

	class SelIIA : public larlite_base {

	public:

		void setInputType(InputFileType_t filetype) { _filetype = filetype; }

		/// Default constructor
		SelIIA();

		/// Default destructor
		~SelIIA() {}

		CCNuItxn_t findNeutrino(const event_track *ev_track,
		                            const event_calorimetry *ev_calo,
		                            const larlite::AssSet_t & ass_calo_v,
		                            const event_vertex *ev_vtx,
		                            const event_opflash *ev_flash);

		void setVtxSphereRadius(double myradius) { _vtx_sphere_radius = myradius; }

	protected:

		double flashDistZ(const track &longest_track, const double flash_z);

		bool trackAssociatedWithVtx(const track &trk,
		                            const ::geoalgo::Sphere &vtx_sphere);

		bool isMichelMID(const ::geoalgo::Sphere &vtx_sphere,
		                 const std::vector<size_t> trk_idx_vec,
		                 const event_track *ev_track,
		                 const event_calorimetry *ev_calo,
		                 const larlite::AssSet_t & ass_calo_v);

		::geoalgo::Sphere getVertexSphere(const vertex &vtx);

		bool setBGWTimes();

		InputFileType_t _filetype;

 		// Beam spill window times (different for different input filetypes,
 		// set with setBGWTimes function)
		double BGW_mintime;
		double BGW_maxtime;

		// Fiducial volume box
		geoalgo::AABox _fidvolBox;

		// Radius around reco vertex to determine if a nearby track
		// is associated with that vertex
		double _vtx_sphere_radius;
	};

}
#endif
/** @} */ // end of doxygen group

