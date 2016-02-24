//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Feb 22 11:54:12 2016 by ROOT version 6.04/14
// from TTree StudentTransferCoursesByTerm/Student Transfer Courses By Term
// found on file: UmdLA.root
//////////////////////////////////////////////////////////

#ifndef StudentTransferCoursesByTerm_h
#define StudentTransferCoursesByTerm_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TString.h"

class StudentTransferCoursesByTerm {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           cnt;
   Int_t           first_term;
   Int_t           NOBS;
   Int_t           newid;
   Int_t           EXTRACT_TERM;
   Int_t           TRANS_TERM;
   TString         *SIS_INTSITUTION_CD;
   TString         *INST_LEV_CD;
   TString         *TRANS_CRS;
   TString         *TRANS_CRS_EQUIV;
   TString         *SIS_INSTITUTION;
   TString         *INST_LEV;
   TString         *TRANS_EQUIV_UM_CRS_IND;
   TString         *TRANS_CRS_EQUIV_CR;
   TString         *TRANS_CRS_GRADE;
   TString         *CORE_CD;
   TString         *CORE;
   TString         *DIVERSITY_IND;

   // List of branches
   TBranch        *b_cnt;   //!
   TBranch        *b_first_term;   //!
   TBranch        *b_NOBS;   //!
   TBranch        *b_newid;   //!
   TBranch        *b_EXTRACT_TERM;   //!
   TBranch        *b_TRANS_TERM;   //!
   TBranch        *b_SIS_INTSITUTION_CD;   //!
   TBranch        *b_INST_LEV_CD;   //!
   TBranch        *b_TRANS_CRS;   //!
   TBranch        *b_TRANS_CRS_EQUIV;   //!
   TBranch        *b_SIS_INSTITUTION;   //!
   TBranch        *b_INST_LEV;   //!
   TBranch        *b_TRANS_EQUIV_UM_CRS_IND;   //!
   TBranch        *b_TRANS_CRS_EQUIV_CR;   //!
   TBranch        *b_TRANS_CRS_GRADE;   //!
   TBranch        *b_CORE_CD;   //!
   TBranch        *b_CORE;   //!
   TBranch        *b_DIVERSITY_IND;   //!

   StudentTransferCoursesByTerm(TTree *tree=0);
   virtual ~StudentTransferCoursesByTerm();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef StudentTransferCoursesByTerm_cxx
StudentTransferCoursesByTerm::StudentTransferCoursesByTerm(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("UmdLA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("UmdLA.root");
      }
      f->GetObject("StudentTransferCoursesByTerm",tree);

   }
   Init(tree);
}

StudentTransferCoursesByTerm::~StudentTransferCoursesByTerm()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t StudentTransferCoursesByTerm::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t StudentTransferCoursesByTerm::LoadTree(Long64_t entry)
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

void StudentTransferCoursesByTerm::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   SIS_INTSITUTION_CD = 0;
   INST_LEV_CD = 0;
   TRANS_CRS = 0;
   TRANS_CRS_EQUIV = 0;
   SIS_INSTITUTION = 0;
   INST_LEV = 0;
   TRANS_EQUIV_UM_CRS_IND = 0;
   TRANS_CRS_EQUIV_CR = 0;
   TRANS_CRS_GRADE = 0;
   CORE_CD = 0;
   CORE = 0;
   DIVERSITY_IND = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("cnt", &cnt, &b_cnt);
   fChain->SetBranchAddress("first_term", &first_term, &b_first_term);
   fChain->SetBranchAddress("NOBS", &NOBS, &b_NOBS);
   fChain->SetBranchAddress("newid", &newid, &b_newid);
   fChain->SetBranchAddress("EXTRACT_TERM", &EXTRACT_TERM, &b_EXTRACT_TERM);
   fChain->SetBranchAddress("TRANS_TERM", &TRANS_TERM, &b_TRANS_TERM);
   fChain->SetBranchAddress("SIS_INTSITUTION_CD", &SIS_INTSITUTION_CD, &b_SIS_INTSITUTION_CD);
   fChain->SetBranchAddress("INST_LEV_CD", &INST_LEV_CD, &b_INST_LEV_CD);
   fChain->SetBranchAddress("TRANS_CRS", &TRANS_CRS, &b_TRANS_CRS);
   fChain->SetBranchAddress("TRANS_CRS_EQUIV", &TRANS_CRS_EQUIV, &b_TRANS_CRS_EQUIV);
   fChain->SetBranchAddress("SIS_INSTITUTION", &SIS_INSTITUTION, &b_SIS_INSTITUTION);
   fChain->SetBranchAddress("INST_LEV", &INST_LEV, &b_INST_LEV);
   fChain->SetBranchAddress("TRANS_EQUIV_UM_CRS_IND", &TRANS_EQUIV_UM_CRS_IND, &b_TRANS_EQUIV_UM_CRS_IND);
   fChain->SetBranchAddress("TRANS_CRS_EQUIV_CR", &TRANS_CRS_EQUIV_CR, &b_TRANS_CRS_EQUIV_CR);
   fChain->SetBranchAddress("TRANS_CRS_GRADE", &TRANS_CRS_GRADE, &b_TRANS_CRS_GRADE);
   fChain->SetBranchAddress("CORE_CD", &CORE_CD, &b_CORE_CD);
   fChain->SetBranchAddress("CORE", &CORE, &b_CORE);
   fChain->SetBranchAddress("DIVERSITY_IND", &DIVERSITY_IND, &b_DIVERSITY_IND);
   Notify();
}

Bool_t StudentTransferCoursesByTerm::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void StudentTransferCoursesByTerm::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t StudentTransferCoursesByTerm::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef StudentTransferCoursesByTerm_cxx
