#ifndef SHOWERRECOALGMODULAR_CXX
#define SHOWERRECOALGMODULAR_CXX

#include "ShowerRecoAlgModular.h"
#include "LArUtil/Geometry.h"
#include <iomanip>

namespace showerreco {

// initialize the various algorithms
void ShowerRecoAlgModular::Initialize()
{
  for (auto & module : _modules) {
    module->initialize();
    _module_time_v.push_back(0.);
    _module_ctr_v.push_back(0);
  }
  return;
}

Shower_t ShowerRecoAlgModular::RecoOneShower(const ::protoshower::ProtoShower& proto_shower) {
  // Run over the shower reco modules:
  Shower_t result;

  // Make sure the result shower has the right size of all it's elements
  auto geom = larutil::Geometry::GetME();
  int nPlanes = geom -> Nplanes();
  result.fTotalEnergy_v.resize(nPlanes);
  result.fSigmaTotalEnergy_v.resize(nPlanes);
  result.fdEdx_v.resize(nPlanes);
  result.fdQdx_v.resize(nPlanes);
  result.fSigmadEdx_v.resize(nPlanes);
  result.fSigmadQdx_v.resize(nPlanes);
  result.fHitdQdx_v.resize(nPlanes);

  result.fShoweringLength.resize(nPlanes); // resizing Showering Length Vector
  result.fTotalMIPEnergy_v.resize(nPlanes);
  result.fSigmaTotalMIPEnergy_v.resize(nPlanes);

  result.fPlaneIsBad.resize(nPlanes);

  Shower_t localCopy = result;

  for (size_t n = 0; n < _modules.size(); n++) {
    _watch.Start();
    try {
      _modules[n] -> do_reconstruction(proto_shower, result);
    }
    catch (ShowerRecoException e) {
      result.fPassedReconstruction = false;
      if (_debug)
	std::cout << e.what() << std::endl;
      return result;
    }
    _module_time_v[n] += _watch.RealTime();
    _module_ctr_v[n] += 1;
    if (_debug && _verbose) {
      printChanges(localCopy, result, _modules[n]->name());
      localCopy = result;
    }

  }

  // If things are this far, then set the flag to true for passing reco
  result.fPassedReconstruction = true;

  return result;
}

void ShowerRecoAlgModular::AddShowerRecoModule( ShowerRecoModuleBase * module) {
  _modules.push_back(module);
}

void ShowerRecoAlgModular::InsertShowerRecoModule(ShowerRecoModuleBase * module, unsigned int index) {
  if (_modules.size() > index) {
    _modules.insert(_modules.begin() + index, module);
  }
  else {
    std::cerr << "WARNING: Request to insert module at index " << index << " which is beyond the last index." << std::endl;
  }
}

void ShowerRecoAlgModular::InsertShowerRecoModule(ShowerRecoModuleBase * module, std::string name) {
  for (unsigned int index = 0; index < _modules.size(); index ++) {
    if (_modules.at(index)->name() == name) {
      _modules.insert(_modules.begin() + index, module);
      return;
    }
  }
  std::cerr << "WARNING: Request to insert module after non existing module \"" << name << "\"" << std::endl;

}


void ShowerRecoAlgModular::ReplaceShowerRecoModule( ShowerRecoModuleBase * module, unsigned int index) {
  if (_modules.size() > index) {
    _modules.at(index) = module;
  }
  else {
    std::cerr << "WARNING: Request to remove non existing module at index " << index << std::endl;
  }
}


void ShowerRecoAlgModular::ReplaceShowerRecoModule( ShowerRecoModuleBase * module, std::string name) {

  for (unsigned int index = 0; index < _modules.size(); index ++) {
    if (_modules.at(index)->name() == name) {
      _modules.at(index) = module;
      return;
    }
  }
  std::cerr << "WARNING: Request to remove non existing module \"" << name << "\"" << std::endl;

}

void ShowerRecoAlgModular::PrintModuleList() {

  std::cout << "Print the list of modules to run in Shower Reco Alg Modular:\n";
  int i = 0;
  for (auto & module : _modules) {
    std::cout << "\t" << i << ") " << module -> name() << "\n";
    i++;
  }

}

void ShowerRecoAlgModular::printChanges(const Shower_t & localCopy, const Shower_t result, std::string moduleName) {

  bool changed;

  // Look at each value of Shower_t and if it has changed, print out that change
  std::cout << "\nPrinting the list of changes made by module " << moduleName << std::endl;

  // Compare vectors by x/y/z/ values
  // Doing comparisons in the order of values in Shower_t
  // Cos Start:
  if (localCopy.fDCosStart.X() != result.fDCosStart.X() ||
      localCopy.fDCosStart.Y() != result.fDCosStart.Y() ||
      localCopy.fDCosStart.X() != result.fDCosStart.X() ) {
    std::cout << "\tfDCosStart has changed from ("
              << localCopy.fDCosStart.X() << ", "
              << localCopy.fDCosStart.Y() << ", "
              << localCopy.fDCosStart.Z() << ") to ("
              << result.fDCosStart.X() << ", "
              << result.fDCosStart.Y() << ", "
              << result.fDCosStart.Z() << ") "
              << std::endl;
  }

  // Sigma Cos Start:
  if (localCopy.fSigmaDCosStart.X() != result.fSigmaDCosStart.X() ||
      localCopy.fSigmaDCosStart.Y() != result.fSigmaDCosStart.Y() ||
      localCopy.fSigmaDCosStart.X() != result.fSigmaDCosStart.X() ) {
    std::cout << "\tfSigmaDCosStart has changed from ("
              << localCopy.fSigmaDCosStart.X() << ", "
              << localCopy.fSigmaDCosStart.Y() << ", "
              << localCopy.fSigmaDCosStart.Z() << ") to ("
              << result.fSigmaDCosStart.X() << ", "
              << result.fSigmaDCosStart.Y() << ", "
              << result.fSigmaDCosStart.Z() << ") "
              << std::endl;
  }

  // XYZ Start:
  if (localCopy.fXYZStart.X() != result.fXYZStart.X() ||
      localCopy.fXYZStart.Y() != result.fXYZStart.Y() ||
      localCopy.fXYZStart.X() != result.fXYZStart.X() ) {
    std::cout << "\tfXYZStart has changed from ("
              << localCopy.fXYZStart.X() << ", "
              << localCopy.fXYZStart.Y() << ", "
              << localCopy.fXYZStart.Z() << ") to ("
              << result.fXYZStart.X() << ", "
              << result.fXYZStart.Y() << ", "
              << result.fXYZStart.Z() << ") "
              << std::endl;
  }

  // Sigma XYZ Start
  if (localCopy.fSigmaXYZStart.X() != result.fSigmaXYZStart.X() ||
      localCopy.fSigmaXYZStart.Y() != result.fSigmaXYZStart.Y() ||
      localCopy.fSigmaXYZStart.X() != result.fSigmaXYZStart.X() ) {
    std::cout << "\tfSigmaXYZStart has changed from ("
              << localCopy.fSigmaXYZStart.X() << ", "
              << localCopy.fSigmaXYZStart.Y() << ", "
              << localCopy.fSigmaXYZStart.Z() << ") to ("
              << result.fSigmaXYZStart.X() << ", "
              << result.fSigmaXYZStart.Y() << ", "
              << result.fSigmaXYZStart.Z() << ") "
              << std::endl;
  }

  // Length
  if (localCopy.fLength != result.fLength) {
    std::cout << "\tfDCosStart has changed from " << localCopy.fLength
              << " to "  << result.fLength << std::endl;
  }

  // BestdQdx
  if (localCopy.fBestdQdx != result.fBestdQdx) {
    std::cout << "\tfBestdQdx has changed from " << localCopy.fBestdQdx
              << " to "  << result.fBestdQdx << std::endl;
  }

  // Opening Angle
  if (localCopy.fOpeningAngle != result.fOpeningAngle) {
    std::cout << "\tfDCosStart has changed from " << localCopy.fOpeningAngle
              << " to "  << result.fOpeningAngle << std::endl;
  }

  // Since these should be length = nplanes, checking each plane for change
  // If changed, print out

  // Total Energy
  changed = false;
  if (localCopy.fTotalEnergy != result.fTotalEnergy)
    changed = true;
  if (changed) {
    std::cout << "\tfTotalEnergy has changed from "
              << localCopy.fTotalEnergy << " to "
              << result.fTotalEnergy << std::endl;
  }

  //Total Energy vector
  changed = false;
  for (unsigned int i = 0; i < localCopy.fTotalEnergy_v.size(); i++) {
    if (localCopy.fTotalEnergy_v[i] != result.fTotalEnergy_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfTotalEnergy_v has changed from (";
    for (auto & val : localCopy.fTotalEnergy_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fTotalEnergy_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }



  // Sigma Total Energy
  changed = false;
  if (localCopy.fSigmaTotalEnergy != result.fSigmaTotalEnergy)
    changed = true;
  if (changed) {
    std::cout << "\tfSigmaTotalEnergy has changed from "
              << localCopy.fSigmaTotalEnergy << " to "
              << result.fSigmaTotalEnergy << std::endl;
  }

  // Sigma Total Energy vector
  changed = false;
  for (unsigned int i = 0; i < localCopy.fSigmaTotalEnergy_v.size(); i++) {
    if (localCopy.fSigmaTotalEnergy_v[i] != result.fSigmaTotalEnergy_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfSigmaTotalEnergy_v has changed from (";
    for (auto & val : localCopy.fSigmaTotalEnergy_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fSigmaTotalEnergy_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }

  // dEdx
  changed = false;
  if (localCopy.fdEdx != result.fdEdx)
    changed = true;
  if (changed) {
    std::cout << "\tfdEdx has changed from "
              << localCopy.fdEdx << " to "
              << result.fdEdx << std::endl;
  }

  // dEdx_v
  changed = false;
  for (unsigned int i = 0; i < localCopy.fdEdx_v.size(); i++) {
    if (localCopy.fdEdx_v[i] != result.fdEdx_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfdEdx_v has changed from (";
    for (auto & val : localCopy.fdEdx_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fdEdx_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }

  // dQdx
  changed = false;
  if (localCopy.fdQdx_v != result.fdQdx_v)
    changed = true;
  if (changed) {
    std::cout << "\tfdQdx has changed from"
              << localCopy.fdQdx << " to "
              << result.fdQdx << std::endl;
  }

  // dQdx_v
  changed = false;
  for (unsigned int i = 0; i < localCopy.fdQdx_v.size(); i++) {
    if (localCopy.fdQdx_v[i] != result.fdQdx_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfdQdx_v has changed from (";
    for (auto & val : localCopy.fdQdx_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fdQdx_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }

  // HitdQdx_v
  changed = false;
  for (unsigned int i = 0; i < localCopy.fHitdQdx_v.size(); i++) {
    for (unsigned int j = 0; j < localCopy.fHitdQdx_v[i].size(); j++) {
      if (localCopy.fHitdQdx_v[i][j] != result.fHitdQdx_v[i][j]) {
        changed = true;
        break;
      }
    }
  }
  if (changed) {
    std::cout << "\tfHitdQdx_v has changed from (";
    for (auto & val : localCopy.fHitdQdx_v )
      for (auto & v : val) std::cout << v << " ";
    std::cout << ") to (";
    for (auto & val : result.fHitdQdx_v )
      for (auto & v : val )std::cout << v << " ";
    std::cout << ")" << std::endl;
  }


  // sigma dEdx
  changed = false;
  if (localCopy.fSigmadEdx != result.fSigmadEdx)
    changed = true;
  if (changed) {
    std::cout << "\tfSigmadEdx has changed from "
              << localCopy.fSigmadEdx << " to "
              << result.fSigmadEdx << std::endl;
  }

  // sigma dEdx_v
  changed = false;
  for (unsigned int i = 0; i < localCopy.fSigmadEdx_v.size(); i++) {
    if (localCopy.fSigmadEdx_v[i] != result.fSigmadEdx_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfSigmadEdx_v has changed from (";
    for (auto & val : localCopy.fSigmadEdx_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fSigmadEdx_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }


  // sigma dQdx
  changed = false;
  if (localCopy.fSigmadQdx != result.fSigmadQdx)
    changed = true;
  if (changed) {
    std::cout << "\tfSigmadQdx has changed from "
              << localCopy.fSigmadQdx << " to "
              << result.fSigmadQdx << std::endl;
  }

  // sigma dQdx_v
  changed = false;
  for (unsigned int i = 0; i < localCopy.fSigmadQdx_v.size(); i++) {
    if (localCopy.fSigmadQdx_v[i] != result.fSigmadQdx_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfSigmadQdx_v has changed from (";
    for (auto & val : localCopy.fSigmadQdx_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fSigmadQdx_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }

  // Total MIP Energy
  changed = false;
  for (unsigned int i = 0; i < localCopy.fTotalMIPEnergy_v.size(); i++) {
    if (localCopy.fTotalMIPEnergy_v[i] != result.fTotalMIPEnergy_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfTotalMIPEnergy has changed from (";
    for (auto & val : localCopy.fTotalMIPEnergy_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fTotalMIPEnergy_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }

  // Sigma Total MIP Energy
  changed = false;
  for (unsigned int i = 0; i < localCopy.fSigmaTotalMIPEnergy_v.size(); i++) {
    if (localCopy.fSigmaTotalMIPEnergy_v[i] != result.fSigmaTotalMIPEnergy_v[i]) {
      changed = true;
      break;
    }
  }
  if (changed) {
    std::cout << "\tfSigmaTotalMIPEnergy has changed from (";
    for (auto & val : localCopy.fSigmaTotalMIPEnergy_v ) std::cout << val << " ";
    std::cout << ") to (";
    for (auto & val : result.fSigmaTotalMIPEnergy_v ) std::cout << val << " ";
    std::cout << ")" << std::endl;
  }

  if (localCopy.fBestPlane != result.fBestPlane) {
    std::cout << "\tfBestPlane has changed from " << localCopy.fBestPlane.Plane
              << " to "  << result.fBestPlane.Plane << std::endl;
  }


  std::cout << std::endl;

}


// finalize function
void ShowerRecoAlgModular::Finalize(TFile* fout)
{

  // loop through algos and evaluate time-performance
  std::cout << std::endl
            << "=================== Time Report =====================" << std::endl;
  for (size_t n = 0; n < _modules.size(); n++) {
    double module_time = _module_time_v[n] / ((double)_module_ctr_v[n]);
    std::cout <<  std::setw(25) << _modules[n]->name() << "\t Algo Time: "
              << std::setw(10) << module_time * 1.e6     << " [us/proto_shower]"
              << "\t Proto-Showers Scanned: " << _module_ctr_v[n] << std::endl;
  }

  std::cout << "=====================================================" << std::endl
            << std::endl;

  // for each algorithm, get its tree and write it to output
  if (fout) {
    fout->cd();
    for (auto & module : _modules) {
      auto tree = module->GetTree();
      if (tree) { tree->Write(); }
    }// for each modular algo
  }// if output file exists

  return;
}

} // showerreco

#endif
