#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cstdlib>

int CountTabs(TString line) {

	int nTab = 0;
	
	for (int i = 0; i < line.Length(); i++) {
		if (line[i] == '\t')
			nTab++;
	}
	return nTab;
}

void ReadEnglishCodes()
{
	std::cout << "About to read English Codes..." << std::endl;
	TString line, code, code_ld, count;

	TTree* t1 = new TTree("EnglishCodes", "English FS Completion Codes");
	t1->Branch("EnglCode", &code);
	t1->Branch("EnglCode_ld", &code_ld);
	
	std::string filename = "data/codes_engl_req_unix.txt";
	std::ifstream myFile(filename);
	int iCount;
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	std::cout << "Line 1 = " << line << std::endl;
	while (!code.ReadToDelim(myFile, '\t').eof())
	{
		code_ld.ReadToDelim(myFile, '\t');
		count.ReadLine(myFile);
		iCount = count.Atoi();
		std::cout << "Read:  " << code << '\t' << code_ld << '\t' << iCount << std::endl;
		t1->Fill();
	}
	myFile.close();
	
	t1->MakeClass();
	
//	t1->Print();
	
	
	return;
}

void ReadCourseRules()
{
	std::cout << "About to read Course Rules..." << std::endl;
	TString line, CrsPrefixNum, StartTerm, EndTerm, VersionNum, CrsStat, RuleStatements;
	TString PrereqInd, CoreqInd, RecmdInd, RestrInd, PrereqFreeformTxtInd, CoreqFreeformTxtInd;
	TString RecmdFreeformTxtInd, RestrFreeformTxtInd;
	int iStartTerm, iEndTerm, iVersionNum;

	TTree* t1 = new TTree("CourseRules", "Course Rules");
	t1->Branch("CrsPrefixNum", &CrsPrefixNum);
	t1->Branch("StartTerm", &iStartTerm);
	t1->Branch("EndTerm", &iEndTerm);
	t1->Branch("VersionNum", &iVersionNum);
	t1->Branch("CrsStat", &CrsStat);
	t1->Branch("RuleStatements", &RuleStatements);
	t1->Branch("PrereqInd", &PrereqInd);
	t1->Branch("CoreqInd", &CoreqInd);
	t1->Branch("RecmdInd", &RecmdInd);
	t1->Branch("RestrInd", &RestrInd);
	t1->Branch("PrereqFreeformTxtInd", &PrereqFreeformTxtInd);
	t1->Branch("CoreqFreeformTxtInd", &CoreqFreeformTxtInd);
	t1->Branch("RecmdFreeformTxtInd", &RecmdFreeformTxtInd);
	t1->Branch("RestrFreeformTxtInd", &RestrFreeformTxtInd);
	
	std::string filename = "data/course_rules.txt";
	std::ifstream myFile(filename);

	int nLines = 0;
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	int defTabs = CountTabs(line);
	std::cout << "Number of tabs in first line = " << defTabs << std::endl;
	
	Ssiz_t from = 0;
	while (line.ReadLine(myFile))
	{
		/*
		int thisTabs = CountTabs(line);
		if (thisTabs != defTabs) {
			std::cout << "Found a line with different number of tabs =  " << thisTabs << ", line number " << nLines + 2 << std::endl;
			std::cout << line << std::endl;
			continue;
		}
		*/
		
		from = 0;
		line.Tokenize(CrsPrefixNum, from, "\t");
		line.Tokenize(StartTerm, from, "\t");
		iStartTerm = StartTerm.Atoi();
		line.Tokenize(EndTerm, from, "\t");
		iEndTerm = EndTerm.Atoi();
		line.Tokenize(VersionNum, from, "\t");
		iVersionNum = VersionNum.Atoi();
		line.Tokenize(CrsStat, from, "\t");
		line.Tokenize(RuleStatements, from, "\t");
		line.Tokenize(PrereqInd, from, "\t");
		line.Tokenize(CoreqInd, from, "\t");
		line.Tokenize(RecmdInd, from, "\t");
		line.Tokenize(RestrInd, from, "\t");
		line.Tokenize(PrereqFreeformTxtInd, from, "\t");
		line.Tokenize(CoreqFreeformTxtInd, from, "\t");
		line.Tokenize(RecmdFreeformTxtInd, from, "\t");
		line.Tokenize(RestrFreeformTxtInd, from, "\n");

//		std::cout << "RecmdFreeformTxtInd = " << RecmdFreeformTxtInd << std::endl;
//		std::cout << "RestrFreeformTxtInd = " << RestrFreeformTxtInd << std::endl;
		
		t1->Fill();
		nLines++;
		
		if (nLines % 1000 == 0)
			std::cout << "Read " << nLines << " lines.\r";
	}
	myFile.close();

	std::cout << "Read " << nLines << " lines." << std::endl;
		
//	t1->Print();
	t1->MakeClass();
	
	
	return;
}

void ReadStudentDegreesByTerm()
{
	std::cout << "About to read Student Degrees By Term..." << std::endl;
	TString line;
	TString newid, DEG_TERM, JOIN_TERM, DIB_HEADCOUNT_RPT_LVL_CD, DEGREE_CD, DEGREE, OTHER_MAJ_MINOR_IND;
	TString DEGREE_TYPE_CD, DEGREE_TYPE, OIS_DEG_LEVEL_CD, OIS_DEG_LEVEL, OIS_DEG_PRIMARY_UG_IND;
	TString MAJOR_CD, MAJOR, MAJOR_LD, OIRP_RPT_GRP_CD, OIRP_RPT_GRP, OIRP_RPT_GRP_SD;
	TString DIVISION_CD, DIVISION, DIVISION_SD;
	
	int inewid, iDEG_TERM, iJOIN_TERM, iDIB_HEADCOUNT_RPT_LVL_CD, iOIS_DEG_LEVEL_CD, iDEGREE_CD;
	
	TTree* t1 = new TTree("StudentDegreesByTerm", "Student Degrees By Term");
	t1->Branch("newid", &inewid);
	t1->Branch("DEG_TERM", &iDEG_TERM);
	t1->Branch("JOIN_TERM", &iJOIN_TERM);
	t1->Branch("DIB_HEADCOUNT_RPT_LVL_CD", &iDIB_HEADCOUNT_RPT_LVL_CD);
	t1->Branch("DEGREE_CD", &DEGREE_CD);
	t1->Branch("DEGREE", &DEGREE);
	t1->Branch("OTHER_MAJ_MINOR_IND", &OTHER_MAJ_MINOR_IND);
	t1->Branch("DEGREE_TYPE_CD", &DEGREE_TYPE_CD);
	t1->Branch("DEGREE_TYPE", &DEGREE_TYPE);
	t1->Branch("OIS_DEG_LEVEL_CD", &iOIS_DEG_LEVEL_CD);
	t1->Branch("OIS_DEG_LEVEL", &OIS_DEG_LEVEL);
	t1->Branch("OIS_DEG_PRIMARY_UG_IND", &OIS_DEG_PRIMARY_UG_IND);
	t1->Branch("MAJOR_CD", &MAJOR_CD);
	t1->Branch("MAJOR", &MAJOR);
	t1->Branch("MAJOR_LD", &MAJOR_LD);
	t1->Branch("OIRP_RPT_GRP_CD", &OIRP_RPT_GRP_CD);
	t1->Branch("OIRP_RPT_GRP", &OIRP_RPT_GRP);
	t1->Branch("OIRP_RPT_GRP_SD", &OIRP_RPT_GRP_SD);
	t1->Branch("DIVISION_CD", &DIVISION_CD);
	t1->Branch("DIVISION", &DIVISION);
	t1->Branch("DIVISION_SD", &DIVISION_SD);
 	
	std::string filename = "data/student_degrees_by_term.txt";
	std::ifstream myFile(filename);

	int nLines = 0;
	Ssiz_t from = 0;
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	int defTabs = CountTabs(line);
	std::cout << "Number of tabs in first line = " << defTabs << std::endl;

	while (line.ReadLine(myFile))
	{	
		/*	
		int thisTabs = CountTabs(line);
		if (thisTabs != defTabs) {
			std::cout << "Found a line with different number of tabs =  " << thisTabs << ", line number " << nLines + 2 << std::endl;
			std::cout << line << std::endl;
			continue;
		}
		*/
	
		from = 0;
		line.Tokenize(newid, from, "\t");
		inewid = newid.Atoi();
		line.Tokenize(DEG_TERM, from, "\t");
		iDEG_TERM = DEG_TERM.Atoi();
		line.Tokenize(JOIN_TERM, from, "\t");
		iJOIN_TERM = JOIN_TERM.Atoi();
		line.Tokenize(DIB_HEADCOUNT_RPT_LVL_CD, from, "\t");
		iDIB_HEADCOUNT_RPT_LVL_CD = DIB_HEADCOUNT_RPT_LVL_CD.Atoi();
		line.Tokenize(DEGREE_CD, from, "\t");
		iDEGREE_CD = DEGREE_CD.Atoi();
		line.Tokenize(DEGREE, from, "\t");
		line.Tokenize(OTHER_MAJ_MINOR_IND, from, "\t");
		line.Tokenize(DEGREE_TYPE_CD, from, "\t");
		line.Tokenize(DEGREE_TYPE, from, "\t");
		line.Tokenize(OIS_DEG_LEVEL_CD, from, "\t");
		line.Tokenize(OIS_DEG_LEVEL, from, "\t");
		line.Tokenize(OIS_DEG_PRIMARY_UG_IND, from, "\t");
		line.Tokenize(MAJOR_CD, from, "\t");
		line.Tokenize(MAJOR, from, "\t");
		line.Tokenize(MAJOR_LD, from, "\t");
		line.Tokenize(OIRP_RPT_GRP_CD, from, "\t");
		line.Tokenize(OIRP_RPT_GRP, from, "\t");
		line.Tokenize(OIRP_RPT_GRP_SD, from, "\t");
		line.Tokenize(DIVISION_CD, from, "\t");
		line.Tokenize(DIVISION, from, "\t");
		line.Tokenize(DIVISION_SD, from, "\n");

		t1->Fill();
		
		nLines++;
		
		if (nLines % 1000 == 0)
			std::cout << "Read " << nLines << " lines.\r";
		
	}
	myFile.close();

	std::cout << "Read " << nLines << " lines." << std::endl;
		
//	t1->Print();
	t1->MakeClass();
	
	
	return;
}

void ReadStudentEnrollmentsByTerm()
{
	std::cout << "About to read Student Enrollments By Term..." << std::endl;
	TString line;
	TString newid, TERM, FULL_TIME_IND, OFFICIAL_ENROLLED_IND, DEG_SEEKING_IND, ENROLL_TYPE_CD, ENROLL_TYPE, ENROLL_TYPE_DETAIL;
	TString BEG_MATH_REQM_UG_CD, BEG_MATH_REQM_UG, BEG_ENGL_REQM_UG_CD, BEG_ENGL_REQM_UG;
	TString BEG_JR_ENGL_REQM_CD, BEG_JR_ENGL_REQM, SEM_END_CR_ERN, SEM_END_GPA, END_CUM_GPA_UG, END_CUM_CR_ERN_UG;
	TString END_CUM_CR_DEG_UG;
	
	int inewid, iTERM, iBEG_JR_ENGL_REQM_CD, iSEM_END_CR_ERN, iEND_CUM_CR_ERN_UG, iEND_CUM_CR_DEG_UG;
	double dSEM_END_GPA, dEND_CUM_GPA_UG;
	
	TTree* t1 = new TTree("StudentEnrollmentsByTerm", "Student Enrollments By Term");
	t1->Branch("newid", &inewid);
	t1->Branch("TERM", &iTERM);
	t1->Branch("FULL_TIME_IND", &FULL_TIME_IND);
	t1->Branch("OFFICIAL_ENROLLED_IND", &OFFICIAL_ENROLLED_IND);
	t1->Branch("DEG_SEEKING_IND", &DEG_SEEKING_IND);
	t1->Branch("ENROLL_TYPE_CD", &ENROLL_TYPE_CD);
	t1->Branch("ENROLL_TYPE", &ENROLL_TYPE);
	t1->Branch("ENROLL_TYPE_DETAIL", &ENROLL_TYPE_DETAIL);
	t1->Branch("BEG_MATH_REQM_UG_CD", &BEG_MATH_REQM_UG_CD);
	t1->Branch("BEG_MATH_REQM_UG", &BEG_MATH_REQM_UG);
	t1->Branch("BEG_ENGL_REQM_UG_CD", &BEG_ENGL_REQM_UG_CD);
	t1->Branch("BEG_ENGL_REQM_UG", &BEG_ENGL_REQM_UG);
	t1->Branch("BEG_JR_ENGL_REQM_CD", &BEG_JR_ENGL_REQM_CD);
	t1->Branch("BEG_JR_ENGL_REQM", &BEG_JR_ENGL_REQM);
	t1->Branch("SEM_END_CR_ERN", &SEM_END_CR_ERN);
	t1->Branch("SEM_END_GPA", &SEM_END_GPA);
	t1->Branch("END_CUM_GPA_UG", &END_CUM_GPA_UG);
	t1->Branch("END_CUM_CR_ERN_UG", &END_CUM_CR_ERN_UG);
	t1->Branch("END_CUM_CR_DEG_UG", &END_CUM_CR_DEG_UG);
 	
	std::string filename = "data/student_enrollments_by_term.txt";
	std::ifstream myFile(filename);

	int nLines = 0;
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	int defTabs = CountTabs(line);
	std::cout << "Number of tabs in first line = " << defTabs << std::endl;

	Ssiz_t from = 0;
	while (line.ReadLine(myFile))
	{
		/*
		int thisTabs = CountTabs(line);
		if (thisTabs != defTabs) {
			std::cout << "Found a line with different number of tabs =  " << thisTabs << ", line number " << nLines + 2 << std::endl;
			std::cout << line << std::endl;
			continue;
		}
		*/
		from = 0;
		
		line.Tokenize(newid, from, "\t");
		inewid = newid.Atoi();
		line.Tokenize(TERM, from, "\t");
		iTERM = TERM.Atoi();
		line.Tokenize(FULL_TIME_IND, from, "\t");
		line.Tokenize(OFFICIAL_ENROLLED_IND, from, "\t");
		line.Tokenize(DEG_SEEKING_IND, from, "\t");
		line.Tokenize(ENROLL_TYPE_CD, from, "\t");
		line.Tokenize(ENROLL_TYPE, from, "\t");
		line.Tokenize(ENROLL_TYPE_DETAIL, from, "\t");
		line.Tokenize(BEG_MATH_REQM_UG_CD, from, "\t");
		line.Tokenize(BEG_MATH_REQM_UG, from, "\t");
		line.Tokenize(BEG_ENGL_REQM_UG_CD, from, "\t");
		line.Tokenize(BEG_ENGL_REQM_UG, from, "\t");
		line.Tokenize(BEG_JR_ENGL_REQM_CD, from, "\t");
		line.Tokenize(BEG_JR_ENGL_REQM, from, "\t");
		line.Tokenize(SEM_END_CR_ERN, from, "\t");
		line.Tokenize(SEM_END_GPA, from, "\t");
		line.Tokenize(END_CUM_GPA_UG, from, "\t");
		line.Tokenize(END_CUM_CR_ERN_UG, from, "\t");
		line.Tokenize(END_CUM_CR_DEG_UG, from, "\n");

		t1->Fill();
		
		nLines++;
		
		if (nLines % 1000 == 0)
			std::cout << "Read " << nLines << " lines.\r";
		
	}
	myFile.close();

	std::cout << "Read " << nLines << " lines." << std::endl;
		
//	t1->Print();
	t1->MakeClass();
	
	
	return;
}

void ReadStudentCoursesByTerm()
{
	
	std::cout << "About to read StudentCoursesByTerm..." << std::endl;
	TString line;
	int icnt, ifirst_term, iNOBS, inewid, iTERM, iJOIN_TERM;
	int iSECTION, iCRS_NUMBER, iCRS_CREDIT, iCRS_APPLICABLE_CD, iCRS_LEV_APPLIED_CD, iCRS_REPEAT_CD, iCRS_REPEAT_CR;
	TString cnt, first_term, NOBS, newid, TERM, JOIN_TERM, COURSE, SECTION;
	TString CRS_TITLE, CRS_PREFIX, CRS_NUMBER, CRS_SUFFIX, CRS_CREDIT, CRS_APPLICABLE_CD;
	TString CRS_APPLICABLE, CRS_LEV_APPLIED_CD, CRS_LEV_APPLIED, CRS_GRD_METH_CD, CRS_GRD_METH;
	TString CRS_GRADE, CRS_REPEAT_CD, CRS_REPEAT, CRS_REPEAT_CR, CRS_REPEAT_GRD;
	TString CREDIT_BY_EXAM_IND, OIS_INCLUDE_CRS_IND, GRAD_THESIS_CR_IND, CRS_INDIV_INSTR_IND;
	TString CORE_CD, CORE, DIVERSITY_IND;
	
	TTree* t = new TTree("StudentCoursesByTerm", "Student Courses By Term");
	t->Branch("cnt", &icnt);
	t->Branch("first_term", &ifirst_term);
	t->Branch("NOBS", &iNOBS);
	t->Branch("newid", &inewid);
	t->Branch("TERM", &iTERM);
	t->Branch("JOIN_TERM", &iJOIN_TERM);
	t->Branch("COURSE", &COURSE);
	t->Branch("SECTION", &iSECTION);
	t->Branch("CRS_TITLE", &CRS_TITLE);
	t->Branch("CRS_PREFIX", &CRS_PREFIX);
	t->Branch("CRS_NUMBER", &iCRS_NUMBER);
	t->Branch("CRS_SUFFIX", &CRS_SUFFIX);
	t->Branch("CRS_CREDIT", &iCRS_CREDIT);
	t->Branch("CRS_APPLICABLE_CD", &iCRS_APPLICABLE_CD);
	t->Branch("CRS_APPLICABLE", &CRS_APPLICABLE);
	t->Branch("CRS_LEV_APPLIED_CD", &iCRS_LEV_APPLIED_CD);
	t->Branch("CRS_LEV_APPLIED", &CRS_LEV_APPLIED);
	t->Branch("CRS_GRD_METH_CD", &CRS_GRD_METH_CD);
	t->Branch("CRS_GRD_METH", &CRS_GRD_METH);
	t->Branch("CRS_GRADE", &CRS_GRADE);
	t->Branch("CRS_REPEAT_CD", &iCRS_REPEAT_CD);
	t->Branch("CRS_REPEAT", &CRS_REPEAT);
	t->Branch("CRS_REPEAT_CR", &iCRS_REPEAT_CR);
	t->Branch("CRS_REPEAT_GRD", &CRS_REPEAT_GRD);
	t->Branch("CREDIT_BY_EXAM_IND", &CREDIT_BY_EXAM_IND);
	t->Branch("OIS_INCLUDE_CRS_IND", &OIS_INCLUDE_CRS_IND);
	t->Branch("GRAD_THESIS_CR_IND", &GRAD_THESIS_CR_IND);
	t->Branch("CRS_IND_INSTR_IND", &CRS_INDIV_INSTR_IND);
	t->Branch("CORE_CD", &CORE_CD);
	t->Branch("CORE", &CORE);
	t->Branch("DIVERSITY_IND", &DIVERSITY_IND);
 	
	std::string filename = "data/student_courses_by_term.txt";
	std::ifstream myFile(filename);
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	int defTabs = CountTabs(line);
	std::cout << "Number of tabs in first line = " << defTabs << std::endl;

	int nLines = 0;
	Ssiz_t from = 0;
	while (line.ReadLine(myFile))
	{
		/*
		int thisTabs = CountTabs(line);
		if (thisTabs != defTabs) {
			std::cout << "Found a line with different number of tabs =  " << thisTabs << ", line number " << nLines + 2 << std::endl;
			std::cout << line << std::endl;
			continue;
		}
		*/
		from = 0;
		
		line.Tokenize(cnt, from, "\t");
		icnt = cnt.Atoi();
		line.Tokenize(first_term, from, "\t");
		ifirst_term = first_term.Atoi();
		line.Tokenize(NOBS, from, "\t");
		iNOBS = NOBS.Atoi();
		line.Tokenize(newid, from, "\t");
		inewid = newid.Atoi();
		line.Tokenize(TERM, from, "\t");
		iTERM = TERM.Atoi();
		line.Tokenize(JOIN_TERM, from, "\t");
		iJOIN_TERM = JOIN_TERM.Atoi();
		line.Tokenize(COURSE, from, "\t");
		line.Tokenize(SECTION, from, "\t");
		iSECTION = SECTION.Atoi();
		line.Tokenize(CRS_TITLE, from, "\t");
		line.Tokenize(CRS_PREFIX, from, "\t");
		line.Tokenize(CRS_NUMBER, from, "\t");
		iCRS_NUMBER = CRS_NUMBER.Atoi();
		line.Tokenize(CRS_SUFFIX, from, "\t");
		line.Tokenize(CRS_CREDIT, from, "\t");
		iCRS_CREDIT = CRS_CREDIT.Atoi();
		line.Tokenize(CRS_APPLICABLE_CD, from, "\t");
		iCRS_APPLICABLE_CD = CRS_APPLICABLE_CD.Atoi();
		line.Tokenize(CRS_APPLICABLE, from, "\t");
		line.Tokenize(CRS_LEV_APPLIED_CD, from, "\t");
		iCRS_LEV_APPLIED_CD = CRS_LEV_APPLIED_CD.Atoi();
		line.Tokenize(CRS_LEV_APPLIED, from, "\t");
		line.Tokenize(CRS_GRD_METH_CD, from, "\t");
		line.Tokenize(CRS_GRD_METH, from, "\t");
		line.Tokenize(CRS_GRADE, from, "\t");
		line.Tokenize(CRS_REPEAT_CD, from, "\t");
		iCRS_REPEAT_CD = CRS_REPEAT_CD.Atoi();
		line.Tokenize(CRS_REPEAT, from, "\t");
		line.Tokenize(CRS_REPEAT_CR, from, "\t");
		iCRS_REPEAT_CR = CRS_REPEAT_CR.Atoi();
		line.Tokenize(CRS_REPEAT_GRD, from, "\t");
		line.Tokenize(CREDIT_BY_EXAM_IND, from, "\t");
		line.Tokenize(OIS_INCLUDE_CRS_IND, from, "\t");
		line.Tokenize(GRAD_THESIS_CR_IND, from, "\t");
		line.Tokenize(CRS_INDIV_INSTR_IND, from, "\t");
		line.Tokenize(CORE_CD, from, "\t");
		line.Tokenize(CORE, from, "\t");
		line.Tokenize(DIVERSITY_IND, from, "\n");
		
		t->Fill();
		nLines++;
		
		if (nLines % 1000 == 0)
			std::cout << "Read " << nLines << " lines.\r";
		
	}
	myFile.close();
	
	std::cout << "Read " << nLines << " lines." << std::endl;

	t->MakeClass();
	
	
	return;
}

void ReadStudentMajorsByTerm()
{
	
	std::cout << "About to read StudentMajorsByTerm..." << std::endl;
	TString line;
	int inewid, iTERM;
	TString newid, TERM, MAJOR_TYPE_CD, MAJOR_TYPE, MAJOR_CD, MAJOR, MAJOR_LD;
	TString OIRP_RPT_GRP_CD, OIRP_RPT_GRP, OIRP_RPT_GRP_SD, UNIT_CD, UNIT, DIVISION_CD, DIVISION, DIVISION_SD;
	
	TTree* t = new TTree("StudentMajorsByTerm", "Student Majors By Term");
	t->Branch("newid", &inewid);
	t->Branch("TERM", &iTERM);
	t->Branch("MAJOR_TYPE_CD", &MAJOR_TYPE_CD);
	t->Branch("MAJOR_TYPE", &MAJOR_TYPE);
	t->Branch("MAJOR_CD", &MAJOR_CD);
	t->Branch("MAJOR", &MAJOR);
	t->Branch("MAJOR_LD", &MAJOR_LD);
	t->Branch("OIRP_RPT_GRP_CD", &OIRP_RPT_GRP_CD);
	t->Branch("OIRP_RPT_GRP", &OIRP_RPT_GRP);
	t->Branch("OIRP_RPT_GRP_SD", &OIRP_RPT_GRP_SD);
	t->Branch("UNIT_CD", &UNIT_CD);
	t->Branch("UNIT", &UNIT);
	t->Branch("DIVISION_CD", &DIVISION_CD);
	t->Branch("DIVISION", &DIVISION);
	t->Branch("DIVISION_SD", &DIVISION_SD);
 	
	std::string filename = "data/student_majors_by_term.txt";
	std::ifstream myFile(filename);
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	int defTabs = CountTabs(line);
	std::cout << "Number of tabs in first line = " << defTabs << std::endl;

	int nLines = 0;
	Ssiz_t from = 0;
	while (line.ReadLine(myFile))
	{
		/*
		int thisTabs = CountTabs(line);
		if (thisTabs != defTabs) {
			std::cout << "Found a line with different number of tabs =  " << thisTabs << ", line number " << nLines + 2 << std::endl;
			std::cout << line << std::endl;
			continue;
		}
		*/
		from = 0;
		
		line.Tokenize(newid, from, "\t");
		inewid = newid.Atoi();
		line.Tokenize(TERM, from, "\t");
		iTERM = TERM.Atoi();
		line.Tokenize(MAJOR_TYPE_CD, from, "\t");
		line.Tokenize(MAJOR_TYPE, from, "\t");
		line.Tokenize(MAJOR_CD, from, "\t");
		line.Tokenize(MAJOR, from, "\t");
		line.Tokenize(MAJOR_LD, from, "\t");
		line.Tokenize(OIRP_RPT_GRP_CD, from, "\t");
		line.Tokenize(OIRP_RPT_GRP, from, "\t");
		line.Tokenize(OIRP_RPT_GRP_SD, from, "\t");
		line.Tokenize(UNIT_CD, from, "\t");
		line.Tokenize(UNIT, from, "\t");
		line.Tokenize(DIVISION_CD, from, "\t");
		line.Tokenize(DIVISION, from, "\t");
		line.Tokenize(DIVISION_SD, from, "\n");
		
		t->Fill();
		nLines++;
		
		if (nLines % 1000 == 0)
			std::cout << "Read " << nLines << " lines.\r";
		
	}
	myFile.close();
	
	std::cout << "Read " << nLines << " lines." << std::endl;
	
	t->MakeClass();
	
	return;
}

void ReadStudentTransferCoursesByTerm()
{
	
	std::cout << "About to read StudentTransferCoursesByTerm..." << std::endl;
	TString line;
	int icnt, ifirst_term, iNOBS, inewid, iEXTRACT_TERM, iTRANS_TERM, iINST_LEV_CD;
	int iSECTION, iCRS_NUMBER, iCRS_CREDIT, iCRS_APPLICABLE_CD, iCRS_LEV_APPLIED_CD, iCRS_REPEAT_CD, iCRS_REPEAT_CR;
	TString cnt, first_term, NOBS, newid, EXTRACT_TERM, TRANS_TERM;
	TString SIS_INTSITUTION_CD, INST_LEV_CD, TRANS_CRS, TRANS_CRS_EQUIV, SIS_INSTITUTION, INST_LEV, TRANS_EQUIV_UM_CRS_IND;
	TString TRANS_CRS_EQUIV_CR, TRANS_CRS_GRADE, CORE_CD, CORE, DIVERSITY_IND;
		
	TTree* t = new TTree("StudentTransferCoursesByTerm", "Student Transfer Courses By Term");
	t->Branch("cnt", &icnt);
	t->Branch("first_term", &ifirst_term);
	t->Branch("NOBS", &iNOBS);
	t->Branch("newid", &inewid);
	t->Branch("EXTRACT_TERM", &iEXTRACT_TERM);
	t->Branch("TRANS_TERM", &iTRANS_TERM);
	t->Branch("SIS_INTSITUTION_CD", &SIS_INTSITUTION_CD);
	t->Branch("INST_LEV_CD", &INST_LEV_CD);
	t->Branch("TRANS_CRS", &TRANS_CRS);
	t->Branch("TRANS_CRS_EQUIV", &TRANS_CRS_EQUIV);
	t->Branch("SIS_INSTITUTION", &SIS_INSTITUTION);
	t->Branch("INST_LEV", &INST_LEV);
	t->Branch("TRANS_EQUIV_UM_CRS_IND", &TRANS_EQUIV_UM_CRS_IND);
	t->Branch("TRANS_CRS_EQUIV_CR", &TRANS_CRS_EQUIV_CR);
	t->Branch("TRANS_CRS_GRADE", &TRANS_CRS_GRADE);
	t->Branch("CORE_CD", &CORE_CD);
	t->Branch("CORE", &CORE);
	t->Branch("DIVERSITY_IND", &DIVERSITY_IND);

	std::string filename = "data/student_transfer_courses_by_term.txt";
	std::ifstream myFile(filename);
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	int defTabs = CountTabs(line);
	std::cout << "Number of tabs in first line = " << defTabs << std::endl;

	int nLines = 0;
	Ssiz_t from = 0;
	while (line.ReadLine(myFile))
	{
		/*
		int thisTabs = CountTabs(line);
		if (thisTabs != defTabs) {
			std::cout << "Found a line with different number of tabs =  " << thisTabs << ", line number " << nLines + 2 << std::endl;
			std::cout << line << std::endl;
			continue;
		}
		*/
		from = 0;
		
		line.Tokenize(cnt, from, "\t");
		icnt = cnt.Atoi();
		line.Tokenize(first_term, from, "\t");
		ifirst_term = first_term.Atoi();
		line.Tokenize(NOBS, from, "\t");
		iNOBS = NOBS.Atoi();
		line.Tokenize(newid, from, "\t");
		inewid = newid.Atoi();
		line.Tokenize(EXTRACT_TERM, from, "\t");
		iEXTRACT_TERM = EXTRACT_TERM.Atoi();
		line.Tokenize(TRANS_TERM, from, "\t");
		iTRANS_TERM = TRANS_TERM.Atoi();
		line.Tokenize(SIS_INTSITUTION_CD, from, "\t");
		line.Tokenize(INST_LEV_CD, from, "\t");
		iINST_LEV_CD = INST_LEV_CD.Atoi();
		line.Tokenize(TRANS_CRS, from, "\t");
		line.Tokenize(TRANS_CRS_EQUIV, from, "\t");
		line.Tokenize(SIS_INSTITUTION, from, "\t");
		line.Tokenize(INST_LEV, from, "\t");
		line.Tokenize(TRANS_EQUIV_UM_CRS_IND, from, "\t");
		line.Tokenize(TRANS_CRS_EQUIV_CR, from, "\t");
		line.Tokenize(TRANS_CRS_GRADE, from, "\t");
		line.Tokenize(CORE_CD, from, "\t");
		line.Tokenize(CORE, from, "\t");
		line.Tokenize(DIVERSITY_IND, from, "\n");
		
		t->Fill();
		nLines++;
		
		if (nLines % 1000 == 0)
			std::cout << "Read " << nLines << " lines.\r";
		
	}
	myFile.close();
	
	std::cout << "Read " << nLines << " lines." << std::endl;

	t->MakeClass();
	
	return;
}

void ReadPrefixCollegeMap(TFile* outFile)
{
	std::cout << "About to read PrefixCollegeMap..." << std::endl;
	TString line;
	TString prefix, college;
	
	std::map<TString, TString> prefixCollegeMap;

	std::string filename = "data/PrefixCollegeMap.txt";
	std::ifstream myFile(filename);
	// Read first line of this file and discard...
	line.ReadLine(myFile);
	int defTabs = CountTabs(line);
	std::cout << "Number of tabs in first line = " << defTabs << std::endl;

	int nLines = 0;
	Ssiz_t from = 0;
	while (line.ReadLine(myFile))
	{
		/*
		int thisTabs = CountTabs(line);
		if (thisTabs != defTabs) {
			std::cout << "Found a line with different number of tabs =  " << thisTabs << ", line number " << nLines + 2 << std::endl;
			std::cout << line << std::endl;
			continue;
		}
		*/
		from = 0;
		
		line.Tokenize(prefix, from, "\t");
		line.Tokenize(college, from, "\r");
//		std::cout << "Ends with = " << college.EndsWith("\r") << ", size = " << college.Length() << std::endl;
		prefixCollegeMap.insert(std::make_pair(prefix, college));
//		std::cout << "College, Prefix = " << college << ", " << prefix << "." << std::endl;
		
		nLines++;
		
		if (nLines % 1000 == 0)
			std::cout << "Read " << nLines << " lines.\r";
		
	}
	myFile.close();
	
	std::cout << "Read " << nLines << " lines." << std::endl;
	
	outFile->WriteObject(&prefixCollegeMap, "PrefixCollegeMap");
	
	return;
}

void RawToRoot()
{
	std::cout << "This is a test" << std::endl;

	TFile* outFile = new TFile("UmdLA.root", "RECREATE");
	
	ReadEnglishCodes();
	ReadStudentCoursesByTerm();
	ReadCourseRules();
	ReadStudentDegreesByTerm();
	ReadStudentEnrollmentsByTerm();
	ReadStudentMajorsByTerm();
	ReadStudentTransferCoursesByTerm();
	ReadPrefixCollegeMap(outFile);
	
	outFile->Write();
	outFile->ls();
	
	outFile->Close();
	
	return;
}

