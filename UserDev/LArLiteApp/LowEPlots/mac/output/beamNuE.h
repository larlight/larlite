//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Sep 18 12:18:09 2015 by ROOT version 5.34/32
// from TTree beamNuE/Result Tree
// found on file: singleE_nue_selection.root
//////////////////////////////////////////////////////////

#ifndef beamNuE_h
#define beamNuE_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class beamNuE {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           _numEvts;
   Int_t           _neutrinos;
   Double_t        _e_nuReco;
   Double_t        _e_dep;
   Double_t        _weight;

   // List of branches
   TBranch        *b_numEvts;   //!
   TBranch        *b_neutrinos;   //!
   TBranch        *b_e_nuReco;   //!
   TBranch        *b_e_dep;   //!
   TBranch        *b_weight;   //!

   beamNuE(TTree *tree=0);
   virtual ~beamNuE();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef beamNuE_cxx
beamNuE::beamNuE(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("singleE_nue_selection.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("singleE_nue_selection.root");
      }
      f->GetObject("beamNuE",tree);

   }
   Init(tree);
}

beamNuE::~beamNuE()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t beamNuE::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t beamNuE::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void beamNuE::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("_numEvts", &_numEvts, &b_numEvts);
   fChain->SetBranchAddress("_neutrinos", &_neutrinos, &b_neutrinos);
   fChain->SetBranchAddress("_e_nuReco", &_e_nuReco, &b_e_nuReco);
   fChain->SetBranchAddress("_e_dep", &_e_dep, &b_e_dep);
   fChain->SetBranchAddress("_weight", &_weight, &b_weight);
   Notify();
}

Bool_t beamNuE::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void beamNuE::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t beamNuE::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef beamNuE_cxx
