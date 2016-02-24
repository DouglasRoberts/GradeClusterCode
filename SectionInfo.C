#include "SectionInfo.h"
#include "MyFunctions.h"
#include <iostream>

ClassImp(SectionInfo)
	
void SectionInfo::Initialize(TString course, int section, int term, TString college)
{
	_crsName = course;
	_secNum = section;
	_term = term;
	_collegeSD = college;
	
	TString histName = "grades_" + _crsName;
	histName += TString::Format("_%d_%d", _secNum, _term);

	_gradeHist = new TH1F(histName, "Grades for " + course, 13, 0., 13.);
	MyFunctions::GradeLabels(_gradeHist->GetXaxis());
	
	return;
}		

void SectionInfo::AddRecord(GrdRecord record)
{
	_grades.push_back(record);
	if (_gradeHist->GetXaxis()->FindFixBin(record.grade) != -1)
		_gradeHist->Fill(record.grade, 1.);
	
	return;
}