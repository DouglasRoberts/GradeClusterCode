//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 24 14:12:02 2016 by ROOT version 6.04/14
// from TTree StudentEnrollmentsByTerm/Student Enrollments By Term
// found on file: UmdLA.root
//////////////////////////////////////////////////////////

#ifndef StudentEnrollmentsByTerm_h
#define StudentEnrollmentsByTerm_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TString.h"

class StudentEnrollmentsByTerm {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           newid;
   Int_t           TERM;
   TString         *FULL_TIME_IND;
   TString         *OFFICIAL_ENROLLED_IND;
   TString         *DEG_SEEKING_IND;
   TString         *ENROLL_TYPE_CD;
   TString         *ENROLL_TYPE;
   TString         *ENROLL_TYPE_DETAIL;
   TString         *BEG_MATH_REQM_UG_CD;
   TString         *BEG_MATH_REQM_UG;
   TString         *BEG_ENGL_REQM_UG_CD;
   TString         *BEG_ENGL_REQM_UG;
   TString         *BEG_JR_ENGL_REQM_CD;
   TString         *BEG_JR_ENGL_REQM;
   TString         *SEM_END_CR_ERN;
   TString         *SEM_END_GPA;
   TString         *END_CUM_GPA_UG;
   TString         *END_CUM_CR_ERN_UG;
   TString         *END_CUM_CR_DEG_UG;

   // List of branches
   TBranch        *b_newid;   //!
   TBranch        *b_TERM;   //!
   TBranch        *b_FULL_TIME_IND;   //!
   TBranch        *b_OFFICIAL_ENROLLED_IND;   //!
   TBranch        *b_DEG_SEEKING_IND;   //!
   TBranch        *b_ENROLL_TYPE_CD;   //!
   TBranch        *b_ENROLL_TYPE;   //!
   TBranch        *b_ENROLL_TYPE_DETAIL;   //!
   TBranch        *b_BEG_MATH_REQM_UG_CD;   //!
   TBranch        *b_BEG_MATH_REQM_UG;   //!
   TBranch        *b_BEG_ENGL_REQM_UG_CD;   //!
   TBranch        *b_BEG_ENGL_REQM_UG;   //!
   TBranch        *b_BEG_JR_ENGL_REQM_CD;   //!
   TBranch        *b_BEG_JR_ENGL_REQM;   //!
   TBranch        *b_SEM_END_CR_ERN;   //!
   TBranch        *b_SEM_END_GPA;   //!
   TBranch        *b_END_CUM_GPA_UG;   //!
   TBranch        *b_END_CUM_CR_ERN_UG;   //!
   TBranch        *b_END_CUM_CR_DEG_UG;   //!

   StudentEnrollmentsByTerm(TTree *tree=0);
   virtual ~StudentEnrollmentsByTerm();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef StudentEnrollmentsByTerm_cxx
StudentEnrollmentsByTerm::StudentEnrollmentsByTerm(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("UmdLA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("UmdLA.root");
      }
      f->GetObject("StudentEnrollmentsByTerm",tree);

   }
   Init(tree);
}

StudentEnrollmentsByTerm::~StudentEnrollmentsByTerm()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t StudentEnrollmentsByTerm::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t StudentEnrollmentsByTerm::LoadTree(Long64_t entry)
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

void StudentEnrollmentsByTerm::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   FULL_TIME_IND = 0;
   OFFICIAL_ENROLLED_IND = 0;
   DEG_SEEKING_IND = 0;
   ENROLL_TYPE_CD = 0;
   ENROLL_TYPE = 0;
   ENROLL_TYPE_DETAIL = 0;
   BEG_MATH_REQM_UG_CD = 0;
   BEG_MATH_REQM_UG = 0;
   BEG_ENGL_REQM_UG_CD = 0;
   BEG_ENGL_REQM_UG = 0;
   BEG_JR_ENGL_REQM_CD = 0;
   BEG_JR_ENGL_REQM = 0;
   SEM_END_CR_ERN = 0;
   SEM_END_GPA = 0;
   END_CUM_GPA_UG = 0;
   END_CUM_CR_ERN_UG = 0;
   END_CUM_CR_DEG_UG = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("newid", &newid, &b_newid);
   fChain->SetBranchAddress("TERM", &TERM, &b_TERM);
   fChain->SetBranchAddress("FULL_TIME_IND", &FULL_TIME_IND, &b_FULL_TIME_IND);
   fChain->SetBranchAddress("OFFICIAL_ENROLLED_IND", &OFFICIAL_ENROLLED_IND, &b_OFFICIAL_ENROLLED_IND);
   fChain->SetBranchAddress("DEG_SEEKING_IND", &DEG_SEEKING_IND, &b_DEG_SEEKING_IND);
   fChain->SetBranchAddress("ENROLL_TYPE_CD", &ENROLL_TYPE_CD, &b_ENROLL_TYPE_CD);
   fChain->SetBranchAddress("ENROLL_TYPE", &ENROLL_TYPE, &b_ENROLL_TYPE);
   fChain->SetBranchAddress("ENROLL_TYPE_DETAIL", &ENROLL_TYPE_DETAIL, &b_ENROLL_TYPE_DETAIL);
   fChain->SetBranchAddress("BEG_MATH_REQM_UG_CD", &BEG_MATH_REQM_UG_CD, &b_BEG_MATH_REQM_UG_CD);
   fChain->SetBranchAddress("BEG_MATH_REQM_UG", &BEG_MATH_REQM_UG, &b_BEG_MATH_REQM_UG);
   fChain->SetBranchAddress("BEG_ENGL_REQM_UG_CD", &BEG_ENGL_REQM_UG_CD, &b_BEG_ENGL_REQM_UG_CD);
   fChain->SetBranchAddress("BEG_ENGL_REQM_UG", &BEG_ENGL_REQM_UG, &b_BEG_ENGL_REQM_UG);
   fChain->SetBranchAddress("BEG_JR_ENGL_REQM_CD", &BEG_JR_ENGL_REQM_CD, &b_BEG_JR_ENGL_REQM_CD);
   fChain->SetBranchAddress("BEG_JR_ENGL_REQM", &BEG_JR_ENGL_REQM, &b_BEG_JR_ENGL_REQM);
   fChain->SetBranchAddress("SEM_END_CR_ERN", &SEM_END_CR_ERN, &b_SEM_END_CR_ERN);
   fChain->SetBranchAddress("SEM_END_GPA", &SEM_END_GPA, &b_SEM_END_GPA);
   fChain->SetBranchAddress("END_CUM_GPA_UG", &END_CUM_GPA_UG, &b_END_CUM_GPA_UG);
   fChain->SetBranchAddress("END_CUM_CR_ERN_UG", &END_CUM_CR_ERN_UG, &b_END_CUM_CR_ERN_UG);
   fChain->SetBranchAddress("END_CUM_CR_DEG_UG", &END_CUM_CR_DEG_UG, &b_END_CUM_CR_DEG_UG);
   Notify();
}

Bool_t StudentEnrollmentsByTerm::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void StudentEnrollmentsByTerm::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t StudentEnrollmentsByTerm::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef StudentEnrollmentsByTerm_cxx
