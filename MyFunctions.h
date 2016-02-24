#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H value

#include <TString.h>
#include <TH1.h>
#include <TAxis.h>

namespace MyFunctions
{
	TString termName(int termCode);
	int termYear(int termCode);
	TString academicYear(int termCode);
	TString fiscalYear(int termCode);
	
	double GradeToQuality(TString grade);
	
	void GradeLabels(TAxis* axis);
	void CollegeLabels(TAxis* axis);
	void AyLabels(TAxis* axis);
}
#endif