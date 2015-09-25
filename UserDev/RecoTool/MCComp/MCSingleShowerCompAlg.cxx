#ifndef LARLITE_MCSINGLESHOWERCOMPALG_CXX
#define LARLITE_MCSINGLESHOWERCOMPALG_CXX

#include "MCSingleShowerCompAlg.h"

namespace larlite {

MCSingleShowerCompAlg::MCSingleShowerCompAlg() {
    ResetShowerClusterHitCharges();
}

bool MCSingleShowerCompAlg::InitializeForEvent(storage_manager* storage, std::string fShowerProducer) {

    // Retrieve shower data product
    auto ev_shower = storage->get_data<event_shower>(fShowerProducer);
    if (!ev_shower) {
        std::cout << Form("Did not find shower produced by \"%s\"\n", fShowerProducer.c_str());
        return false;
    }

    if (!ev_shower->size())
        return false;

    // Get associated clusters
    event_cluster* ev_cluster = nullptr;
    auto const& ass_cluster_v = storage->find_one_ass(ev_shower->id(), ev_cluster, ev_shower->name());

    if (!ev_cluster)
        std::cout << Form("No associated cluster found to a shower produced by \"%s\"\n", fShowerProducer.c_str());
    else if (ev_cluster->size() < 1) {
        std::cout << Form("There are 0 associated clusters in this event! Skipping......\n");
        return false;
    }

    // Get associated hits
    event_hit* ev_hit = nullptr;
    auto const& ass_hit_v = storage->find_one_ass(ev_cluster->id(), ev_hit, ev_cluster->name());

    if (!ev_hit)
        std::cout << Form("No associated hit found to a shower produced by \"%s\"\n", ev_cluster->name().c_str());
    else if (ev_hit->size() < 1) {
        std::cout << Form("There are 0 associated hits in this event! Skipping......\n");
        return false;
    }

    // Separately retrieve hit data product with the same producer name
    // (we want all hits in the event, not just hits associated w/ clusters)
    auto full_ev_hit = storage->get_data<event_hit>(ev_hit->name());
    if (!full_ev_hit) {
        std::cout << Form("Did not find hit produced by \"%s\"\n", ev_hit->name().c_str());
        return false;
    }

    // Loop over hits, compute total charge per plane, add it into the _total_hit_Qs map
    _total_hit_Qs_per_plane.clear();
    for (auto const &hit : *full_ev_hit) {
        int iplane = (int)hit.View();
        double icharge = hit.Integral();
        if ( !_total_hit_Qs_per_plane.count(iplane) ) _total_hit_Qs_per_plane[iplane] = icharge;
        else _total_hit_Qs_per_plane[iplane] += icharge;
    }

    if (_total_hit_Qs_per_plane.size() != 3)
        std::cout << "Uhhh why does this map size not equal number of planes?" << std::endl;

    // std::cout << "Total hit Qs per plane calculated to be: "
    //           << _total_hit_Qs_per_plane[0] << ", "
    //           << _total_hit_Qs_per_plane[1] << ", "
    //           << _total_hit_Qs_per_plane[2] << std::endl;

    ResetShowerClusterHitCharges();

    //Loop over reconstructed showers
    for (size_t shower_index = 0; shower_index < ass_cluster_v.size(); ++shower_index) {
        //Make an empty map of plane ==> summed associated cluster charge on that plane
        _shower_plane_hitcharges[shower_index] = std::map<int, double>();

        //Loop over clusters associated with that shower
        for (size_t icluster = 0; icluster < ass_cluster_v[shower_index].size(); ++icluster) {
            int cluster_index = ass_cluster_v.at(shower_index).at(icluster);
            //Loop over hits associated with that cluster to sum the total charge of the cluster
            for (size_t ihit = 0; ihit < ass_hit_v[cluster_index].size(); ++ihit) {
                int hit_index = ass_hit_v[cluster_index].at(ihit);
                auto myhit = ev_hit->at(hit_index);
                int myplane = (int)myhit.View();

                if ( !_shower_plane_hitcharges[shower_index].count(myplane) )
                    _shower_plane_hitcharges[shower_index][myplane] = myhit.Integral();
                else
                    _shower_plane_hitcharges[shower_index][myplane] += myhit.Integral();

            } //End loop over hits associated with that cluster
        } //End loop over clusters associated with that shower

    } //End loop over reconstructed showers

    // std::cout << "the 3 elements in zeroth element are "
    //           << _shower_plane_hitcharges[0][0] << ", "
    //           << _shower_plane_hitcharges[0][1] << ", "
    //           << _shower_plane_hitcharges[0][2] << "." << std::endl;

    return true;

}



std::vector< std::pair<double, double> > MCSingleShowerCompAlg::ClusterEP(size_t shower_index) {

    std::vector< std::pair<double, double> > returnvector;

    if (!_shower_plane_hitcharges.size()) {
        std::cout << "It appears you didn't initialize the MCSingleShowerCompAlg instance you're using!" << std::endl;
        return returnvector;
    }

    // For this given reconstructed shower (at shower_index), loop over planes
    // and compute an efficiency per-plane
    for (int iplane = 0; iplane < 3; iplane++) {
        double myeff = _shower_plane_hitcharges[shower_index][iplane] / _total_hit_Qs_per_plane[iplane];
        double mypur = 1.;
        returnvector.push_back(std::pair<double, double> (myeff, mypur));
    }

    return returnvector;
}

void MCSingleShowerCompAlg::ResetShowerClusterHitCharges() {
    //Clear everything
    // for (auto blah : _shower_plane_hitcharges)
    //     blah.clear();
    _shower_plane_hitcharges.clear();
}
}
#endif
