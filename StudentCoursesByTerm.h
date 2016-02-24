//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb 24 15:37:57 2016 by ROOT version 6.04/14
// from TTree StudentCoursesByTerm/Student Courses By Term
// found on file: UmdLA.root
//////////////////////////////////////////////////////////

#ifndef StudentCoursesByTerm_h
#define StudentCoursesByTerm_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TString.h"

class StudentCoursesByTerm {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           cnt;
   Int_t           first_term;
   Int_t           NOBS;
   Int_t           newid;
   Int_t           TERM;
   Int_t           JOIN_TERM;
   TString         *COURSE;
   Int_t           SECTION;
   TString         *CRS_TITLE;
   TString         *CRS_PREFIX;
   Int_t           CRS_NUMBER;
   TString         *CRS_SUFFIX;
   Int_t           CRS_CREDIT;
   Int_t           CRS_APPLICABLE_CD;
   TString         *CRS_APPLICABLE;
   Int_t           CRS_LEV_APPLIED_CD;
   TString         *CRS_LEV_APPLIED;
   TString         *CRS_GRD_METH_CD;
   TString         *CRS_GRD_METH;
   TString         *CRS_GRADE;
   Int_t           CRS_REPEAT_CD;
   TString         *CRS_REPEAT;
   Int_t           CRS_REPEAT_CR;
   TString         *CRS_REPEAT_GRD;
   TString         *CREDIT_BY_EXAM_IND;
   TString         *OIS_INCLUDE_CRS_IND;
   TString         *GRAD_THESIS_CR_IND;
   TString         *CRS_IND_INSTR_IND;
   TString         *CORE_CD;
   TString         *CORE;
   TString         *DIVERSITY_IND;

   // List of branches
   TBranch        *b_cnt;   //!
   TBranch        *b_first_term;   //!
   TBranch        *b_NOBS;   //!
   TBranch        *b_newid;   //!
   TBranch        *b_TERM;   //!
   TBranch        *b_JOIN_TERM;   //!
   TBranch        *b_COURSE;   //!
   TBranch        *b_SECTION;   //!
   TBranch        *b_CRS_TITLE;   //!
   TBranch        *b_CRS_PREFIX;   //!
   TBranch        *b_CRS_NUMBER;   //!
   TBranch        *b_CRS_SUFFIX;   //!
   TBranch        *b_CRS_CREDIT;   //!
   TBranch        *b_CRS_APPLICABLE_CD;   //!
   TBranch        *b_CRS_APPLICABLE;   //!
   TBranch        *b_CRS_LEV_APPLIED_CD;   //!
   TBranch        *b_CRS_LEV_APPLIED;   //!
   TBranch        *b_CRS_GRD_METH_CD;   //!
   TBranch        *b_CRS_GRD_METH;   //!
   TBranch        *b_CRS_GRADE;   //!
   TBranch        *b_CRS_REPEAT_CD;   //!
   TBranch        *b_CRS_REPEAT;   //!
   TBranch        *b_CRS_REPEAT_CR;   //!
   TBranch        *b_CRS_REPEAT_GRD;   //!
   TBranch        *b_CREDIT_BY_EXAM_IND;   //!
   TBranch        *b_OIS_INCLUDE_CRS_IND;   //!
   TBranch        *b_GRAD_THESIS_CR_IND;   //!
   TBranch        *b_CRS_IND_INSTR_IND;   //!
   TBranch        *b_CORE_CD;   //!
   TBranch        *b_CORE;   //!
   TBranch        *b_DIVERSITY_IND;   //!

   StudentCoursesByTerm(TTree *tree=0);
   virtual ~StudentCoursesByTerm();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef StudentCoursesByTerm_cxx
StudentCoursesByTerm::StudentCoursesByTerm(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("UmdLA.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("UmdLA.root");
      }
      f->GetObject("StudentCoursesByTerm",tree);

   }
   Init(tree);
}

StudentCoursesByTerm::~StudentCoursesByTerm()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t StudentCoursesByTerm::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t StudentCoursesByTerm::LoadTree(Long64_t entry)
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

void StudentCoursesByTerm::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   COURSE = 0;
   CRS_TITLE = 0;
   CRS_PREFIX = 0;
   CRS_SUFFIX = 0;
   CRS_APPLICABLE = 0;
   CRS_LEV_APPLIED = 0;
   CRS_GRD_METH_CD = 0;
   CRS_GRD_METH = 0;
   CRS_GRADE = 0;
   CRS_REPEAT = 0;
   CRS_REPEAT_GRD = 0;
   CREDIT_BY_EXAM_IND = 0;
   OIS_INCLUDE_CRS_IND = 0;
   GRAD_THESIS_CR_IND = 0;
   CRS_IND_INSTR_IND = 0;
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
   fChain->SetBranchAddress("TERM", &TERM, &b_TERM);
   fChain->SetBranchAddress("JOIN_TERM", &JOIN_TERM, &b_JOIN_TERM);
   fChain->SetBranchAddress("COURSE", &COURSE, &b_COURSE);
   fChain->SetBranchAddress("SECTION", &SECTION, &b_SECTION);
   fChain->SetBranchAddress("CRS_TITLE", &CRS_TITLE, &b_CRS_TITLE);
   fChain->SetBranchAddress("CRS_PREFIX", &CRS_PREFIX, &b_CRS_PREFIX);
   fChain->SetBranchAddress("CRS_NUMBER", &CRS_NUMBER, &b_CRS_NUMBER);
   fChain->SetBranchAddress("CRS_SUFFIX", &CRS_SUFFIX, &b_CRS_SUFFIX);
   fChain->SetBranchAddress("CRS_CREDIT", &CRS_CREDIT, &b_CRS_CREDIT);
   fChain->SetBranchAddress("CRS_APPLICABLE_CD", &CRS_APPLICABLE_CD, &b_CRS_APPLICABLE_CD);
   fChain->SetBranchAddress("CRS_APPLICABLE", &CRS_APPLICABLE, &b_CRS_APPLICABLE);
   fChain->SetBranchAddress("CRS_LEV_APPLIED_CD", &CRS_LEV_APPLIED_CD, &b_CRS_LEV_APPLIED_CD);
   fChain->SetBranchAddress("CRS_LEV_APPLIED", &CRS_LEV_APPLIED, &b_CRS_LEV_APPLIED);
   fChain->SetBranchAddress("CRS_GRD_METH_CD", &CRS_GRD_METH_CD, &b_CRS_GRD_METH_CD);
   fChain->SetBranchAddress("CRS_GRD_METH", &CRS_GRD_METH, &b_CRS_GRD_METH);
   fChain->SetBranchAddress("CRS_GRADE", &CRS_GRADE, &b_CRS_GRADE);
   fChain->SetBranchAddress("CRS_REPEAT_CD", &CRS_REPEAT_CD, &b_CRS_REPEAT_CD);
   fChain->SetBranchAddress("CRS_REPEAT", &CRS_REPEAT, &b_CRS_REPEAT);
   fChain->SetBranchAddress("CRS_REPEAT_CR", &CRS_REPEAT_CR, &b_CRS_REPEAT_CR);
   fChain->SetBranchAddress("CRS_REPEAT_GRD", &CRS_REPEAT_GRD, &b_CRS_REPEAT_GRD);
   fChain->SetBranchAddress("CREDIT_BY_EXAM_IND", &CREDIT_BY_EXAM_IND, &b_CREDIT_BY_EXAM_IND);
   fChain->SetBranchAddress("OIS_INCLUDE_CRS_IND", &OIS_INCLUDE_CRS_IND, &b_OIS_INCLUDE_CRS_IND);
   fChain->SetBranchAddress("GRAD_THESIS_CR_IND", &GRAD_THESIS_CR_IND, &b_GRAD_THESIS_CR_IND);
   fChain->SetBranchAddress("CRS_IND_INSTR_IND", &CRS_IND_INSTR_IND, &b_CRS_IND_INSTR_IND);
   fChain->SetBranchAddress("CORE_CD", &CORE_CD, &b_CORE_CD);
   fChain->SetBranchAddress("CORE", &CORE, &b_CORE);
   fChain->SetBranchAddress("DIVERSITY_IND", &DIVERSITY_IND, &b_DIVERSITY_IND);
   Notify();
}

Bool_t StudentCoursesByTerm::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void StudentCoursesByTerm::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t StudentCoursesByTerm::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef StudentCoursesByTerm_cxx
