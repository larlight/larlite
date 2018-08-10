#ifndef LARLITE_SELIIA_CXX
#define LARLITE_SELIIA_CXX

#include "SelIIA.h"
#include "DataFormat/opflash.h"
#include "DataFormat/mctruth.h"
#include "DataFormat/mcflux.h"

namespace larlite {

	SelIIA::SelIIA() {

		_name = "SelIIA";

		_filetype = kINPUT_FILE_TYPE_MAX;

		_fidvolBox = CCFidVolBox();

		_vtx_sphere_radius = 3.;

	}

	double SelIIA::flashDistZ(const track & longest_track, const double flash_z) {

		// If flash is between start/end of track in Z (including flipping track direction)
		// return true

		/// I'm jsut going to copy christoph's exactly to try to duplicate
		// double FlashTrackDist(double flash, double start, double end) {
		double end = longest_track.End().Z();
		double start = longest_track.Vertex().Z();
		double flash = flash_z;
		if (end >= start) {
			if (flash < end && flash > start) return 0;
			else return TMath::Min(fabs(flash - start), fabs(flash - end));
		}
		else {
			if (flash > end && flash < start) return 0;
			else return TMath::Min(fabs(flash - start), fabs(flash - end));
		}
	}

	bool SelIIA::trackAssociatedWithVtx(const larlite::track &trk,
	        const ::geoalgo::Sphere &vtx_sphere) {

		if (!trk.NumberTrajectoryPoints()) return false;

		::geoalgo::Vector trkstart = ::geoalgo::Vector( trk.Vertex() );
		::geoalgo::Vector trkend   = ::geoalgo::Vector( trk.End()    );
		
		// If this track doesn't start/end near the reco vertex, ignore it
		if ( !vtx_sphere.Contain(trkstart) && !vtx_sphere.Contain(trkend) )
			return false;

		return true;
	}

	bool SelIIA::isMichelMID(const ::geoalgo::Sphere &vtx_sphere,
	                               const std::vector<size_t> associated_track_idx_vec,
	                               const larlite::event_track * ev_track,
	                               const larlite::event_calorimetry * ev_calo,
	                               const larlite::AssSet_t & ass_calo_v) {

		if (associated_track_idx_vec.size() != 2) {
			std::string errMsg(">2 tracks asstd with vtx in Michel MID check");
			throw std::runtime_error(errMsg);
		}

		// Loop over the two tracks that are associated with the vertex
		// Figure out which is the long one, which is the short one
		// store their indices as "long_track_idx", "short_track_idx"
		auto const& trk0 = ev_track->at( associated_track_idx_vec.at(0) );
		auto const& trk1 = ev_track->at( associated_track_idx_vec.at(1) );
		size_t long_track_idx = 0;
		size_t short_track_idx = 0;
		if (trk0.Length() > trk1.Length()) {
			long_track_idx = associated_track_idx_vec.at(0);
			short_track_idx = associated_track_idx_vec.at(1);
		}
		else {
			long_track_idx = associated_track_idx_vec.at(1);
			short_track_idx = associated_track_idx_vec.at(0);
		}

		// idea: maybe cosmic MIDs are at gaps... try loweirng 3cm

		// Make a cut on dE/dx of longer track
		double vertex_dedx = 0.;
		double far_dedx = 0.;
		double far_max_y = -999.;
		calorimetry thecalo;
		auto const& long_trk = ev_track->at(long_track_idx);
		auto const& short_trk = ev_track->at(short_track_idx);

		// Choose the calo object for this track by the one
		// with the most number of hits in the dEdx vector
		// (this is how analysis tree does it)
		size_t tmp_nhits = 0;
		for (size_t i = 0; i < 3; ++i)
			if (ev_calo->at(ass_calo_v[long_track_idx][i]).dEdx().size() > tmp_nhits) {
				thecalo = ev_calo->at(ass_calo_v[long_track_idx][i]);
				tmp_nhits = thecalo.dEdx().size();
			}

		// now we have a calorimetry object for this track
		// compute the dedx of the track for the portion closest to the vertex
		// and the dedx for the portion far from the vertex
		::geoalgo::Vector trkstart = ::geoalgo::Vector( long_trk.Vertex() );
		::geoalgo::Vector trkend   = ::geoalgo::Vector( long_trk.End()    );


		// xiao says:
		// use first ten hits and last ten hits if track has more than 50 hits
		// otherwise use first 50% of hits and last 50% of hits
		double avg_front_dedx = 0.;
		double avg_back_dedx = 0.;
		if (tmp_nhits > 50) {
			for (size_t i = 0; i < 20; ++i) {
				avg_front_dedx += thecalo.dEdx()[i];
				avg_back_dedx += thecalo.dEdx()[tmp_nhits - 1 - i];
			}
			avg_front_dedx /= 20.;
			avg_back_dedx /= 20.;
		}
		else {
			size_t nhits_to_use = (size_t)tmp_nhits / 5.;
			for (size_t i = 0; i < nhits_to_use; ++i) {
				avg_front_dedx += thecalo.dEdx()[i];
				avg_back_dedx += thecalo.dEdx()[tmp_nhits - 1 - i];
			}
			avg_front_dedx /= (double)nhits_to_use;
			avg_back_dedx /= (double)nhits_to_use;
		}


		// If track is fully contained in the 3cm sphere then throw it out it
		if ( vtx_sphere.Contain(trkstart) && vtx_sphere.Contain(trkend) )
			return true;
		if (vtx_sphere.Contain(trkstart)) {
			vertex_dedx = avg_front_dedx;
			far_dedx    = avg_back_dedx;
			far_max_y   = trkend.at(1);
		}
		else if (vtx_sphere.Contain(trkend)) {
			vertex_dedx = avg_back_dedx;
			far_dedx    = avg_front_dedx;
			far_max_y   = trkstart.at(1);
		}

		// Final tagging (docdb 5724 slide 7):
		if ( ( (vertex_dedx > far_dedx && vertex_dedx > 2.5 && far_dedx < 4.) ||
		        far_max_y > _fidvolBox.Max()[1] ) &&
		        short_trk.Length() < 30. )
			return true;

		return false;
	}

	::geoalgo::Sphere SelIIA::getVertexSphere(const vertex & vtx) {

		::geoalgo::Sphere vtx_sphere;
		::geoalgo::Vector vertex = ::geoalgo::Vector(vtx.X(), vtx.Y(), vtx.Z());
		vtx_sphere.Radius(_vtx_sphere_radius); //4cm (default) sphere around each vertex
		vtx_sphere.Center(vertex);

		return vtx_sphere;

	}

	bool SelIIA::setBGWTimes() {

		if (_filetype == kINPUT_FILE_TYPE_MAX) {
			print(larlite::msg::kERROR, __FUNCTION__, Form("DID NOT SET INPUT FILE TYPE!"));
			return false;
		}
		else if ( _filetype == kOnBeam ) {
			BGW_mintime = 3.3;
			BGW_maxtime = 4.9;
		}
		else if ( _filetype == kOffBeam ) {
			BGW_mintime = 3.65;
			BGW_maxtime = 5.25;
		}
		else if ( _filetype == kCorsikaInTime ) {
			BGW_mintime = 3.2;
			BGW_maxtime = 4.8;
		}
		else if ( _filetype == kBNBOnly ) {
			BGW_mintime = 3.55;
			BGW_maxtime = 5.15;
		}
		else if ( _filetype == kBNBCosmic ) {
			BGW_mintime = 3.55;
			BGW_maxtime = 5.15;
		}
		return true;
	}

	CCNuItxn_t SelIIA::findNeutrino(const event_track *ev_track,
	        const event_calorimetry *ev_calo,
	        const larlite::AssSet_t & ass_calo_v,
	        const event_vertex *ev_vtx,
	        const event_opflash *ev_opflash) {

		CCNuItxn_t result;

		if (!setBGWTimes()) {
			print(larlite::msg::kERROR,
			      __FUNCTION__,
			      Form("Trying to find Neutrino with SelIIA but you haven't set the input file type!"));
			throw std::exception();
		}

		// Loop over flashes, store the brightest flash in the BGW
		opflash theflash;
		bool _flash_in_bgw = false;
		for (auto const& flash : *ev_opflash) {
			// std::cout<<"flash! time is "<<flash.Time()<< " (BGW is "<<BGW_mintime<<" to "<<BGW_maxtime<<")"
			// <<" and total PE is "<<flash.TotalPE()<<" (cut is on 50) "
			// <<" with Z position "<<flash.ZCenter()<<std::endl;
			if (flash.Time() > BGW_mintime && flash.Time() < BGW_maxtime && flash.TotalPE() > 50.) {
				_flash_in_bgw = true;
				// Keep track of the brightest flash in the BGW
				if (flash.TotalPE() > theflash.TotalPE())
					theflash = flash;
			}
		}
		// Require there is at least one >50 PE flash inside of BGW
		if (!_flash_in_bgw) throw std::exception();

		// Loop over vertices.
		// For each vertex in fiducial volume, loop over reco tracks
		// If you find a reco track starting or ending w/in 3cm from the vertex
		// Also keep track of multiplicities
		// Keep track of number of vertices that pass all cuts
		size_t n_viable_vertices = 0;
		// ::geoalgo::Sphere thevertexsphere(0, 0, 0, 3.);
		for (auto const& vtx : *ev_vtx) {

			// Make a 3cm geosphere around the vertex
			::geoalgo::Sphere vtx_sphere = getVertexSphere(vtx);

			// Make sure vertex is in fiducial volume
			if (!_fidvolBox.Contain(vtx_sphere.Center())) continue;
			// Loop over tracks, store index of the ones associated with this vertex
			std::vector<size_t> associated_track_idx_vec;
			associated_track_idx_vec.clear();
			bool _at_least_one_track_matches_flash = false;

			for (size_t i = 0; i < ev_track->size(); ++i) {
				auto const trk = ev_track->at(i);

				if ( trackAssociatedWithVtx(trk, vtx_sphere) ) {

					associated_track_idx_vec.push_back(i);

					if ( flashDistZ(trk, theflash.ZCenter()) < 70. )
						_at_least_one_track_matches_flash = true;
				}

			} //done looping over tracks
			
			// Require at least one vertex-associated track matches the BGW flash
			// (and, in there too is that there exists at least 1 such track)

			// Case by case for # of tracks associated with vertex:
			//   * If only one track is associated with vertex, need to code up what is
			//       described in section 6.2.2 of the CC inclusive note ... haven't done this
			//       yet because I'm currently only interested in >=2 track events.
			//   * If two tracks are associated with vertex, additional Michel removal is done
			//   * If > 2 tracks are associated with vertex, the event passes.
			if (_at_least_one_track_matches_flash &&
			        associated_track_idx_vec.size() == 2) {

				// Now we have a vertex (vtx) that has exactly two tracks associated with it
				// Let's do some michel removal!
				if (isMichelMID(vtx_sphere,
				                associated_track_idx_vec,
				                ev_track,
				                ev_calo,
				                ass_calo_v)) {
					throw std::exception();
				}

				// Now let's add in the minimum angle cut that Xiao uses:
				// Now that we found a neutrino interaction,
				// there is now an additional cut requiring the dot product between the two track directions
				// is less than 0.95, to reduce broken tracks being recod as 2track events (cosmic background)
				// Make a unit TVector3 for each of the two tracks, ensuring each are pointing away from the vertex
				auto const &track1 = ev_track->at(associated_track_idx_vec[0]);
				auto const &track2 = ev_track->at(associated_track_idx_vec[1]);

				auto const &geovtx = ::geoalgo::Vector(result.first.X(), result.first.Y(), result.first.Z());
				auto track1dir = ::geoalgo::Vector(track1.Vertex()).SqDist(geovtx) <
				                 ::geoalgo::Vector(track1.End()).SqDist(geovtx) ?
				                 ::geoalgo::Vector(track1.VertexDirection()) :
				                 ::geoalgo::Vector(track1.EndDirection()) * -1.;
				auto track2dir = ::geoalgo::Vector(track2.Vertex()).SqDist(geovtx) <
				                 ::geoalgo::Vector(track2.End()).SqDist(geovtx) ?
				                 ::geoalgo::Vector(track2.VertexDirection()) :
				                 ::geoalgo::Vector(track2.EndDirection()) * -1.;
				track1dir.Normalize();
				track2dir.Normalize();

				// Cut on dot product less than 0.95
				if ( track1dir.Dot(track2dir) > 0.95 ) throw std::exception();

				//Woohoo we have passed Michel cutting and minimum angle cutting. Let's store the result
				n_viable_vertices++;
				result.first = vtx;
				std::vector< std::pair<KalekoPID_t, track> > temp;
				temp.clear();
				temp.push_back(std::make_pair(kKALEKO_PID_MAX, track1));
				temp.push_back(std::make_pair(kKALEKO_PID_MAX, track2));
				result.second = temp;
			}// End mult == 2

			// If >2 tracks are associated with the vertex, keep the event
			else if (_at_least_one_track_matches_flash &&
			         associated_track_idx_vec.size() > 2) {

				n_viable_vertices++;
				result.first = vtx;
				std::vector< std::pair<KalekoPID_t, track> > temp;
				temp.clear();
				for (size_t i = 0; i < associated_track_idx_vec.size(); ++i)
					temp.push_back(std::make_pair(kKALEKO_PID_MAX, ev_track->at(associated_track_idx_vec[i])));
				result.second = temp;
			}// End mult > 2

		} //done looping over vertices

		// If this event doesn't contain any possible neutrino vertices that pass cuts
		// Or if it contains several, throw that out too! (KALEKO)
		// std::cout << "n_viable_vertices = " << n_viable_vertices << std::endl;
		if (n_viable_vertices != 1) throw std::exception();

		return result;
	}
}
#endif
