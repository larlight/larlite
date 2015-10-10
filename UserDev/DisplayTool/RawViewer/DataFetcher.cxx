#ifndef DATA_FETCHER_CXX
#define DATA_FETCHER_CXX





#include "DataFetcher.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TLeaf.h>
#include <TLorentzVector.h>

#include "TreeElementReader.h"

namespace evd {

DataFetcher::DataFetcher(std::string file_path)
  : file_path_(file_path) {

  gROOT->ProcessLine("gErrorIgnoreLevel = kError;");

  const std::string tree_name = "Events";

  TFile * file = new TFile(file_path_.c_str(), "READ");
  tree_ = (TTree *) file->Get(tree_name.c_str());
  entries_ = tree_->GetEntries();

  // get_entry();

};

DataFetcher::~DataFetcher() {}

void DataFetcher::get_entry(unsigned int entry) {

  adc_.clear();
  channel_.clear();
  pedestal_.clear();
  pdg_code_.clear();
  track_id_.clear();
  parent_id_.clear();
  process_.clear();
  start_momentum_.clear();
  trajectory_length_.clear();
  particle_x_.clear();
  particle_y_.clear();
  particle_z_.clear();
  particle_t_.clear();
  particle_px_.clear();
  particle_py_.clear();
  particle_pz_.clear();
  particle_energy_.clear();

  tree_->GetEntry(entry);

  std::vector<std::string>
      raw_digit_leaf_names = FindLeavesOfType("vector<raw::RawDigit>");

  for (size_t i = 0; i < raw_digit_leaf_names.size(); ++i) {
    std::string name = raw_digit_leaf_names[i];

    TreeElementReader adc_branch(tree_, name + "obj.fADC");
    if (!adc_branch.ok()) continue;

    adc_rows_ = 0;
    adc_cols_ = 0;
    for (size_t j = 0; j < adc_branch.entries(); ++j) {
      const std::vector<short> *
          adc_ptr = adc_branch.get< std::vector<short> >(j);
      adc_.insert(adc_.end(), adc_ptr->begin(), adc_ptr->end());
      adc_cols_ = adc_ptr->size();
      adc_rows_ += 1;
    }

    TreeElementReader channel_branch(tree_, name + "obj.fChannel");
    if (!channel_branch.ok()) continue;

    for (size_t j = 0; j < channel_branch.entries(); ++j) {
      const uint32_t * channel_ptr = channel_branch.get<uint32_t>(j);
      channel_.push_back(* channel_ptr);
    }

    TreeElementReader pedestal_branch(tree_, name + "obj.fPedestal");
    if (!pedestal_branch.ok()) continue;

    for (size_t j = 0; j < pedestal_branch.entries(); ++j) {
      const double * pedestal_ptr = pedestal_branch.get<double>(j);
      pedestal_.push_back(* pedestal_ptr);
    }

    TreeElementReader pedestal_sigma_branch(tree_, name + "obj.fSigma");
    if (!pedestal_sigma_branch.ok()) continue;
  }

  // std::vector<std::string>
  //     mc_particle_leaf_names = FindLeavesOfType("vector<simb::MCParticle>");
  // // NOTE: For some odd reason, there is another leaf of type
  // //       vector<simb::MCParticle> in the LArSoft ROOT files.
  // //       This is a temporary fix to ignore that leaf for the
  // //       time being.
  // // TODO: PERMANENTLY FIX THIS.
  // mc_particle_leaf_names.pop_back();

  // for (size_t i = 0; i < mc_particle_leaf_names.size(); ++i) {
  //   std::string name = mc_particle_leaf_names[i];

  //   TreeElementReader pdg_code_branch(tree_, name + "obj.fpdgCode");
  //   TreeElementReader trajectory_branch(tree_, name +
  //       "obj.ftrajectory.ftrajectory");
  //   TreeElementReader track_id_branch(tree_, name + "obj.ftrackId");
  //   TreeElementReader parent_id_branch(tree_, name + "obj.fmother");
  //   TreeElementReader process_branch(tree_, name + "obj.fprocess");

  //   if (!pdg_code_branch.ok() or !trajectory_branch.ok() or
  //       !track_id_branch.ok() or !parent_id_branch.ok() or
  //       !process_branch.ok()) continue;

  //   number_particles_ = pdg_code_branch.entries();

  //   for (size_t j = 0; j < number_particles_; ++j) {
  //     const int * pdg_code_ptr = pdg_code_branch.get<int>(j);
  //     const int * track_id_ptr = track_id_branch.get<int>(j);
  //     const int * parent_id_ptr = parent_id_branch.get<int>(j);
  //     const std::string * process_ptr = process_branch.get<std::string>(j);
  //     const std::vector< std::pair<TLorentzVector, TLorentzVector> > *
  //         trajectory = trajectory_branch.get< std::vector<
  //         std::pair<TLorentzVector, TLorentzVector> > >(j);
  //     size_t number_points = trajectory->size();

  //     if (number_points < 1) continue;

  //     std::vector<double> trajectory_x;
  //     std::vector<double> trajectory_y;
  //     std::vector<double> trajectory_z;
  //     std::vector<double> trajectory_t;
  //     std::vector<double> trajectory_px;
  //     std::vector<double> trajectory_py;
  //     std::vector<double> trajectory_pz;
  //     std::vector<double> trajectory_energy;

  //     double trajectory_length = 0;

  //     for (size_t k = 0; k < number_points; ++k) {
  //       const TLorentzVector & position = (* trajectory)[k].first;
  //       const TLorentzVector & momentum = (* trajectory)[k].second;
  //       trajectory_x.push_back(position.X());
  //       trajectory_y.push_back(position.Y());
  //       trajectory_z.push_back(position.Z());
  //       trajectory_t.push_back(position.T());
  //       trajectory_px.push_back(momentum.X());
  //       trajectory_py.push_back(momentum.Y());
  //       trajectory_pz.push_back(momentum.Z());
  //       trajectory_energy.push_back(momentum.T());
  //       if (k == 0) {
  //         double p = sqrt(momentum.X()*momentum.X() +
  //                         momentum.Y()*momentum.Y() +
  //                         momentum.Z()*momentum.Z());
  //         start_momentum_.push_back(p);
  //       }
  //       if (k < number_points-1) {
  //         const TLorentzVector & _position = (* trajectory)[k+1].first;
  //         double segment_length = sqrt(pow(_position.X() - position.X(), 2) +
  //                                      pow(_position.Y() - position.Y(), 2) +
  //                                      pow(_position.Z() - position.Z(), 2));

  //         trajectory_length += segment_length;
  //       }
  //     }

  //     pdg_code_.push_back(* pdg_code_ptr);
  //     track_id_.push_back(* track_id_ptr);
  //     parent_id_.push_back(* parent_id_ptr);
  //     process_.push_back(* process_ptr);
  //     trajectory_length_.push_back(trajectory_length);
  //     particle_x_.push_back(trajectory_x);
  //     particle_y_.push_back(trajectory_y);
  //     particle_z_.push_back(trajectory_z);
  //     particle_t_.push_back(trajectory_t);
  //     particle_px_.push_back(trajectory_px);
  //     particle_py_.push_back(trajectory_py);
  //     particle_pz_.push_back(trajectory_pz);
  //     particle_energy_.push_back(trajectory_energy);
  //   }
  // }

}

// This piece of code is borrowed from Argo written by Nathaniel Tagg
std::vector<std::string> DataFetcher::FindLeavesOfType(std::string pattern) {
  /// Look in the tree and try to find a leaf element that matches 'pattern'.
  /// Return the full name of that leaf.
  std::vector<std::string> leaf_names;

  // Strip whitespace from pattern.
  pattern.erase(std::remove_if(pattern.begin(), pattern.end(), ::isspace),
      pattern.end());

  TObjArray * list = tree_->GetListOfLeaves();
  for (int i = 0; i < list->GetEntriesFast(); ++i) {
    TObject * o = list->At(i);
    TLeaf * leaf = (TLeaf *) o;
    std::string name = leaf->GetTypeName();
    // Strip whitespace from pattern.
    name.erase(std::remove_if(name.begin(), name.end(), ::isspace),
        name.end());
    size_t found = name.find(pattern);
    if (found != std::string::npos) {
      // Return this match.
      leaf_names.push_back(leaf->GetName());
    }
  }
  return leaf_names;
}

} // evd

#endif // DATA_FETCHER_CXX
