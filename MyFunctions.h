#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H value

#include "CourseGradeNormer.h"
#include <TAxis.h>
#include <TString.h>
#include <map>

class TGraph;

namespace MyFunctions
{
	TString termName(int termCode);
	bool regularSemester(int termCode);
	int termYear(int termCode);
	TString academicYear(int termCode);
	TString fiscalYear(int termCode);
	
	double GradeToQuality(TString grade, int term = 202001);
	bool ValidGrade(TString grade);
	bool ValidGradeAny(TString grade);  // Includes P and S grades as well as F-A+
	
	void GradeLabels(TAxis* axis);
	void CollegeLabels(TAxis* axis);
	void AyLabels(TAxis* axis);
	
	void BuildGradeNormMap();
	
	double EvalCdf(TGraph* cdf, double x);
	double EvalInvCdf(TGraph* invCdf, double p);
		
//	extern std::map<TString, CourseGradeNormer> gradeNormMap;
	extern std::map<std::pair<TString, int>, CourseGradeNormer> gradeNormMap;
}


#endif