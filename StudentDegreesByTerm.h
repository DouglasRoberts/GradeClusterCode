//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 24 15:37:59 2016 by ROOT version 6.04/14
// from TTree StudentDegreesByTerm/Student Degrees By Term
// found on file: UmdLA.root
//////////////////////////////////////////////////////////

#ifndef StudentDegreesByTerm_h
#define StudentDegreesByTerm_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TString.h"

class StudentDegreesByTerm {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           newid;
   Int_t           DEG_TERM;
   Int_t           JOIN_TERM;
   Int_t           DIB_HEADCOUNT_RPT_LVL_CD;
   TString         *DEGREE_CD;
   TString         *DEGREE;
   TString         *OTHER_MAJ_MINOR_IND;
   TString         *DEGREE_TYPE_CD;
   TString         *DEGREE_TYPE;
   Int_t           OIS_DEG_LEVEL_CD;
   TString         *OIS_DEG_LEVEL;
   TString         *OIS_DEG_PRIMARY_UG_IND;
   TString         *MAJOR_CD;
   TString         *MAJOR;
   TString         *MAJOR_LD;
   TString         *OIRP_RPT_GRP_CD;
   TString         *OIRP_RPT_GRP;
   TString         *OIRP_RPT_GRP_SD;
   TString         *DIVISION_CD;
   TString         *DIVISION;
   TString         *DIVISION_SD;

   // List of branches
   TBranch        *b_newid;   //!
   TBranch        *b_DEG_TERM;   //!
   TBranch        *b_JOIN_TERM;   //!
   TBranch        *b_DIB_HEADCOUNT_RPT_LVL_CD;   //!
   TBranch        *b_DEGREE_CD;   //!
   TBranch        *b_DEGREE;   //!
   TBranch        *b_OTHER_MAJ_MINOR_IND;   //!
   TBranch        *b_DEGREE_TYPE_CD;   //!
   TBranch        *b_DEGREE_TYPE;   //!
   TBranch        *b_OIS_DEG_LEVEL_CD;   //!
   TBranch        *b_OIS_DEG_LEVEL;   //!
   TBranch        *b_OIS_DEG_PRIMARY_UG_IND;   //!
   TBranch        *b_MAJOR_CD;   //!
   TBranch        *b_MAJOR;   //!
   TBranch        *b_MAJOR_LD;   //!
   TBranch        *b_OIRP_RPT_GRP_CD;   //!
   TBranch        *b_OIRP_RPT_GRP;   //!
   TBranch        *b_OIRP_RPT_GRP_SD;   //!
   TBranch        *b_DIVISION_CD;   //!
   TBranch        *b_DIVISION;   //!
   TBranch        *b_DIVISION_SD;   //!

   StudentDegreesByTerm(TTree *tree=0);
   virtual ~StudentDegreesByTerm();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef StudentDegreesByTerm_cxx
StudentDegreesByTerm::StudentDegreesByTerm(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("UmdLA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("UmdLA.root");
      }
      f->GetObject("StudentDegreesByTerm",tree);

   }
   Init(tree);
}

StudentDegreesByTerm::~StudentDegreesByTerm()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t StudentDegreesByTerm::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t StudentDegreesByTerm::LoadTree(Long64_t entry)
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

void StudentDegreesByTerm::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   DEGREE_CD = 0;
   DEGREE = 0;
   OTHER_MAJ_MINOR_IND = 0;
   DEGREE_TYPE_CD = 0;
   DEGREE_TYPE = 0;
   OIS_DEG_LEVEL = 0;
   OIS_DEG_PRIMARY_UG_IND = 0;
   MAJOR_CD = 0;
   MAJOR = 0;
   MAJOR_LD = 0;
   OIRP_RPT_GRP_CD = 0;
   OIRP_RPT_GRP = 0;
   OIRP_RPT_GRP_SD = 0;
   DIVISION_CD = 0;
   DIVISION = 0;
   DIVISION_SD = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("newid", &newid, &b_newid);
   fChain->SetBranchAddress("DEG_TERM", &DEG_TERM, &b_DEG_TERM);
   fChain->SetBranchAddress("JOIN_TERM", &JOIN_TERM, &b_JOIN_TERM);
   fChain->SetBranchAddress("DIB_HEADCOUNT_RPT_LVL_CD", &DIB_HEADCOUNT_RPT_LVL_CD, &b_DIB_HEADCOUNT_RPT_LVL_CD);
   fChain->SetBranchAddress("DEGREE_CD", &DEGREE_CD, &b_DEGREE_CD);
   fChain->SetBranchAddress("DEGREE", &DEGREE, &b_DEGREE);
   fChain->SetBranchAddress("OTHER_MAJ_MINOR_IND", &OTHER_MAJ_MINOR_IND, &b_OTHER_MAJ_MINOR_IND);
   fChain->SetBranchAddress("DEGREE_TYPE_CD", &DEGREE_TYPE_CD, &b_DEGREE_TYPE_CD);
   fChain->SetBranchAddress("DEGREE_TYPE", &DEGREE_TYPE, &b_DEGREE_TYPE);
   fChain->SetBranchAddress("OIS_DEG_LEVEL_CD", &OIS_DEG_LEVEL_CD, &b_OIS_DEG_LEVEL_CD);
   fChain->SetBranchAddress("OIS_DEG_LEVEL", &OIS_DEG_LEVEL, &b_OIS_DEG_LEVEL);
   fChain->SetBranchAddress("OIS_DEG_PRIMARY_UG_IND", &OIS_DEG_PRIMARY_UG_IND, &b_OIS_DEG_PRIMARY_UG_IND);
   fChain->SetBranchAddress("MAJOR_CD", &MAJOR_CD, &b_MAJOR_CD);
   fChain->SetBranchAddress("MAJOR", &MAJOR, &b_MAJOR);
   fChain->SetBranchAddress("MAJOR_LD", &MAJOR_LD, &b_MAJOR_LD);
   fChain->SetBranchAddress("OIRP_RPT_GRP_CD", &OIRP_RPT_GRP_CD, &b_OIRP_RPT_GRP_CD);
   fChain->SetBranchAddress("OIRP_RPT_GRP", &OIRP_RPT_GRP, &b_OIRP_RPT_GRP);
   fChain->SetBranchAddress("OIRP_RPT_GRP_SD", &OIRP_RPT_GRP_SD, &b_OIRP_RPT_GRP_SD);
   fChain->SetBranchAddress("DIVISION_CD", &DIVISION_CD, &b_DIVISION_CD);
   fChain->SetBranchAddress("DIVISION", &DIVISION, &b_DIVISION);
   fChain->SetBranchAddress("DIVISION_SD", &DIVISION_SD, &b_DIVISION_SD);
   Notify();
}

Bool_t StudentDegreesByTerm::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void StudentDegreesByTerm::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t StudentDegreesByTerm::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef StudentDegreesByTerm_cxx
