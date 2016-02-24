//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 24 14:11:52 2016 by ROOT version 6.04/14
// from TTree CourseRules/Course Rules
// found on file: UmdLA.root
//////////////////////////////////////////////////////////

#ifndef CourseRules_h
#define CourseRules_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TString.h"

class CourseRules {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   TString         *CrsPrefixNum;
   Int_t           StartTerm;
   Int_t           EndTerm;
   Int_t           VersionNum;
   TString         *CrsStat;
   TString         *RuleStatements;
   TString         *PrereqInd;
   TString         *CoreqInd;
   TString         *RecmdInd;
   TString         *RestrInd;
   TString         *PrereqFreeformTxtInd;
   TString         *CoreqFreeformTxtInd;
   TString         *RecmdFreeformTxtInd;
   TString         *RestrFreeformTxtInd;

   // List of branches
   TBranch        *b_CrsPrefixNum;   //!
   TBranch        *b_StartTerm;   //!
   TBranch        *b_EndTerm;   //!
   TBranch        *b_VersionNum;   //!
   TBranch        *b_CrsStat;   //!
   TBranch        *b_RuleStatements;   //!
   TBranch        *b_PrereqInd;   //!
   TBranch        *b_CoreqInd;   //!
   TBranch        *b_RecmdInd;   //!
   TBranch        *b_RestrInd;   //!
   TBranch        *b_PrereqFreeformTxtInd;   //!
   TBranch        *b_CoreqFreeformTxtInd;   //!
   TBranch        *b_RecmdFreeformTxtInd;   //!
   TBranch        *b_RestrFreeformTxtInd;   //!

   CourseRules(TTree *tree=0);
   virtual ~CourseRules();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef CourseRules_cxx
CourseRules::CourseRules(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("UmdLA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("UmdLA.root");
      }
      f->GetObject("CourseRules",tree);

   }
   Init(tree);
}

CourseRules::~CourseRules()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t CourseRules::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CourseRules::LoadTree(Long64_t entry)
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

void CourseRules::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   CrsPrefixNum = 0;
   CrsStat = 0;
   RuleStatements = 0;
   PrereqInd = 0;
   CoreqInd = 0;
   RecmdInd = 0;
   RestrInd = 0;
   PrereqFreeformTxtInd = 0;
   CoreqFreeformTxtInd = 0;
   RecmdFreeformTxtInd = 0;
   RestrFreeformTxtInd = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("CrsPrefixNum", &CrsPrefixNum, &b_CrsPrefixNum);
   fChain->SetBranchAddress("StartTerm", &StartTerm, &b_StartTerm);
   fChain->SetBranchAddress("EndTerm", &EndTerm, &b_EndTerm);
   fChain->SetBranchAddress("VersionNum", &VersionNum, &b_VersionNum);
   fChain->SetBranchAddress("CrsStat", &CrsStat, &b_CrsStat);
   fChain->SetBranchAddress("RuleStatements", &RuleStatements, &b_RuleStatements);
   fChain->SetBranchAddress("PrereqInd", &PrereqInd, &b_PrereqInd);
   fChain->SetBranchAddress("CoreqInd", &CoreqInd, &b_CoreqInd);
   fChain->SetBranchAddress("RecmdInd", &RecmdInd, &b_RecmdInd);
   fChain->SetBranchAddress("RestrInd", &RestrInd, &b_RestrInd);
   fChain->SetBranchAddress("PrereqFreeformTxtInd", &PrereqFreeformTxtInd, &b_PrereqFreeformTxtInd);
   fChain->SetBranchAddress("CoreqFreeformTxtInd", &CoreqFreeformTxtInd, &b_CoreqFreeformTxtInd);
   fChain->SetBranchAddress("RecmdFreeformTxtInd", &RecmdFreeformTxtInd, &b_RecmdFreeformTxtInd);
   fChain->SetBranchAddress("RestrFreeformTxtInd", &RestrFreeformTxtInd, &b_RestrFreeformTxtInd);
   Notify();
}

Bool_t CourseRules::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void CourseRules::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t CourseRules::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef CourseRules_cxx
