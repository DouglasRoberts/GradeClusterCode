//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb 22 11:54:03 2016 by ROOT version 6.04/14
// from TTree StudentMajorsByTerm/Student Majors By Term
// found on file: UmdLA.root
//////////////////////////////////////////////////////////

#ifndef StudentMajorsByTerm_h
#define StudentMajorsByTerm_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TString.h"

class StudentMajorsByTerm {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           newid;
   Int_t           TERM;
   TString         *MAJOR_TYPE_CD;
   TString         *MAJOR_TYPE;
   TString         *MAJOR_CD;
   TString         *MAJOR;
   TString         *MAJOR_LD;
   TString         *OIRP_RPT_GRP_CD;
   TString         *OIRP_RPT_GRP;
   TString         *OIRP_RPT_GRP_SD;
   TString         *UNIT_CD;
   TString         *UNIT;
   TString         *DIVISION_CD;
   TString         *DIVISION;
   TString         *DIVISION_SD;

   // List of branches
   TBranch        *b_newid;   //!
   TBranch        *b_TERM;   //!
   TBranch        *b_MAJOR_TYPE_CD;   //!
   TBranch        *b_MAJOR_TYPE;   //!
   TBranch        *b_MAJOR_CD;   //!
   TBranch        *b_MAJOR;   //!
   TBranch        *b_MAJOR_LD;   //!
   TBranch        *b_OIRP_RPT_GRP_CD;   //!
   TBranch        *b_OIRP_RPT_GRP;   //!
   TBranch        *b_OIRP_RPT_GRP_SD;   //!
   TBranch        *b_UNIT_CD;   //!
   TBranch        *b_UNIT;   //!
   TBranch        *b_DIVISION_CD;   //!
   TBranch        *b_DIVISION;   //!
   TBranch        *b_DIVISION_SD;   //!

   StudentMajorsByTerm(TTree *tree=0);
   virtual ~StudentMajorsByTerm();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef StudentMajorsByTerm_cxx
StudentMajorsByTerm::StudentMajorsByTerm(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("UmdLA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("UmdLA.root");
      }
      f->GetObject("StudentMajorsByTerm",tree);

   }
   Init(tree);
}

StudentMajorsByTerm::~StudentMajorsByTerm()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t StudentMajorsByTerm::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t StudentMajorsByTerm::LoadTree(Long64_t entry)
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

void StudentMajorsByTerm::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   MAJOR_TYPE_CD = 0;
   MAJOR_TYPE = 0;
   MAJOR_CD = 0;
   MAJOR = 0;
   MAJOR_LD = 0;
   OIRP_RPT_GRP_CD = 0;
   OIRP_RPT_GRP = 0;
   OIRP_RPT_GRP_SD = 0;
   UNIT_CD = 0;
   UNIT = 0;
   DIVISION_CD = 0;
   DIVISION = 0;
   DIVISION_SD = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("newid", &newid, &b_newid);
   fChain->SetBranchAddress("TERM", &TERM, &b_TERM);
   fChain->SetBranchAddress("MAJOR_TYPE_CD", &MAJOR_TYPE_CD, &b_MAJOR_TYPE_CD);
   fChain->SetBranchAddress("MAJOR_TYPE", &MAJOR_TYPE, &b_MAJOR_TYPE);
   fChain->SetBranchAddress("MAJOR_CD", &MAJOR_CD, &b_MAJOR_CD);
   fChain->SetBranchAddress("MAJOR", &MAJOR, &b_MAJOR);
   fChain->SetBranchAddress("MAJOR_LD", &MAJOR_LD, &b_MAJOR_LD);
   fChain->SetBranchAddress("OIRP_RPT_GRP_CD", &OIRP_RPT_GRP_CD, &b_OIRP_RPT_GRP_CD);
   fChain->SetBranchAddress("OIRP_RPT_GRP", &OIRP_RPT_GRP, &b_OIRP_RPT_GRP);
   fChain->SetBranchAddress("OIRP_RPT_GRP_SD", &OIRP_RPT_GRP_SD, &b_OIRP_RPT_GRP_SD);
   fChain->SetBranchAddress("UNIT_CD", &UNIT_CD, &b_UNIT_CD);
   fChain->SetBranchAddress("UNIT", &UNIT, &b_UNIT);
   fChain->SetBranchAddress("DIVISION_CD", &DIVISION_CD, &b_DIVISION_CD);
   fChain->SetBranchAddress("DIVISION", &DIVISION, &b_DIVISION);
   fChain->SetBranchAddress("DIVISION_SD", &DIVISION_SD, &b_DIVISION_SD);
   Notify();
}

Bool_t StudentMajorsByTerm::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void StudentMajorsByTerm::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t StudentMajorsByTerm::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef StudentMajorsByTerm_cxx
