#include "MyFunctions.h"
#include "Student.h"
#include <TFile.h>
#include <TGraph.h>
#include <TTree.h>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <map>
#include <unordered_map>

namespace MyFunctions {
// std::map<TString, CourseGradeNormer> gradeNormMap;
 std::map<std::pair<TString, int>, CourseGradeNormer> gradeNormMap;
};

void MyFunctions::BuildGradeNormMap() {
	// This function build the gradeNormMap if it isn't already filled
	if (gradeNormMap.size() > 0) return;  // Already filled
	
	// This is build from the Students root file.  Maybe this should be from a course file instead?
	TFile* f = new TFile("Students.root");
	TTree* studentTree = (TTree*)f->Get("Students");
	Student* student = 0;
	studentTree->SetBranchAddress("student", &student);
	
	Long64_t nentries = studentTree->GetEntriesFast();
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		//Loop over all terms for this student and accumulate grade distribution info
		for (auto const& grade : student->Grades()) {
			// Only look at Fall and Spring Terms
			int term = grade.term;
			if (!regularSemester(term)) continue;
			if (!ValidGrade(grade.grade)) continue;
			auto thisKey = std::make_pair(grade.course, 0);
			auto thisKeyAll = std::make_pair("AllCourses", 0);
			MyFunctions::gradeNormMap[thisKey].AddGrade(grade.grade);  // Will automatically create entry in map if it doesn't exist
			MyFunctions::gradeNormMap[thisKeyAll].AddGrade(grade.grade);
			auto termKey = std::make_pair(grade.course, term);
			auto termKeyAll = std::make_pair("AllCourses", term);
			MyFunctions::gradeNormMap[termKey].AddGrade(grade.grade);
			MyFunctions::gradeNormMap[termKeyAll].AddGrade(grade.grade);
		}			
	}
	
	f->Close();
}

bool MyFunctions::ValidGrade(TString grade) {
	// Try to speen this up?
	return (
		(grade == "A+") || (grade == "A") || (grade == "A-") ||
		(grade == "B+") || (grade == "B") || (grade == "B-") ||
		(grade == "C+") || (grade == "C") || (grade == "C-") ||
		(grade == "D+") || (grade == "D") || (grade == "D-") ||
		(grade == "F")
			);				
}

bool MyFunctions::ValidGradeAny(TString grade) {
	return (MyFunctions::ValidGrade(grade) || (grade == "S") || (grade == "P"));
}

double MyFunctions::GradeToQuality(TString grade, int term) {
	
	if (!ValidGrade(grade)) return -1.;
	
//	static const int firstNewTerm = 201208;   // This is the actual term where plus/minus grades counted in GPA calc
	static const int firstNewTerm = 180008;   // This disables using non-plus/minus grade quality points
	
	// NOTE: I've modified this to count A+ as 4.3 !!!!!!!!  Campus actually uses A+ = 4.0
	static std::map<TString, double> const gradesOld = {
		{"F", 0.}, 
		{"D-", 1.}, {"D", 1.}, {"D+", 1.},
		{"C-", 2.}, {"C", 2.}, {"C+", 2.},
		{"B-", 3.}, {"B", 3.}, {"B+", 3.},
		{"A-", 4.}, {"A", 4.}, {"A+", 4.}};
	static std::map<TString, double> const gradesNew = {
		{"F", 0.}, 
		{"D-", 0.7}, {"D", 1.0}, {"D+", 1.3},
		{"C-", 1.7}, {"C", 2.0}, {"C+", 2.3},
		{"B-", 2.7}, {"B", 3.0}, {"B+", 3.3},
		{"A-", 3.7}, {"A", 4.0}, {"A+", 4.3}};
		
	if (term >= firstNewTerm) {
		return gradesNew.at(grade);
	}
	else {
		return gradesOld.at(grade);
	}
}

void MyFunctions::GradeLabels(TAxis* axis) {
	axis->SetBinLabel(13, "A+");
	axis->SetBinLabel(12, "A");
	axis->SetBinLabel(11, "A-");
	axis->SetBinLabel(10, "B+");
	axis->SetBinLabel(9, "B");
	axis->SetBinLabel(8, "B-");
	axis->SetBinLabel(7, "C+");
	axis->SetBinLabel(6, "C");
	axis->SetBinLabel(5, "C-");
	axis->SetBinLabel(4, "D+");
	axis->SetBinLabel(3, "D");
	axis->SetBinLabel(2, "D-");
	axis->SetBinLabel(1, "F");
	axis->SetCanExtend(false);
	
	return;
}

void MyFunctions::AyLabels(TAxis* axis) {
	axis->SetBinLabel(1, "AY97-98");
	axis->SetBinLabel(2, "AY98-99");
	axis->SetBinLabel(3, "AY99-00");
	axis->SetBinLabel(4, "AY00-01");
	axis->SetBinLabel(5, "AY01-02");
	axis->SetBinLabel(6, "AY02-03");
	axis->SetBinLabel(7, "AY03-04");
	axis->SetBinLabel(8, "AY04-05");
	axis->SetBinLabel(9, "AY05-06");
	axis->SetBinLabel(10, "AY06-07");
	axis->SetBinLabel(11, "AY07-08");
	axis->SetBinLabel(12, "AY08-09");
	axis->SetBinLabel(13, "AY09-10");
	axis->SetBinLabel(14, "AY10-11");
	axis->SetBinLabel(15, "AY11-12");
	axis->SetBinLabel(16, "AY12-13");
	axis->SetBinLabel(17, "AY13-14");
	axis->SetBinLabel(18, "AY14-15");
	axis->SetCanExtend(false);
	
	return;
}

void MyFunctions::CollegeLabels(TAxis* axis) {
	int bin = 0;
	axis->SetBinLabel(++bin, "AGNR");
	axis->SetBinLabel(++bin, "ARCH");
	axis->SetBinLabel(++bin, "ARHU");
	axis->SetBinLabel(++bin, "BSOS");
	axis->SetBinLabel(++bin, "BMGT");
	axis->SetBinLabel(++bin, "CMNS");
	axis->SetBinLabel(++bin, "EDUC");
	axis->SetBinLabel(++bin, "ENGR");
	axis->SetBinLabel(++bin, "JOUR");
//	axis->SetBinLabel(++bin, "INFO");
	axis->SetBinLabel(++bin, "SPHL");
//	axis->SetBinLabel(++bin, "PUAF");
	axis->SetBinLabel(++bin, "UGST");
	axis->SetBinLabel(++bin, "OTHR");
	axis->SetCanExtend(false);
	
	return;
}

TString MyFunctions::termName(int termCode) {
	// Get last two digits of termCode
	int last2 = termCode % 100;
	
	TString retVal;
	switch (last2) {
		case 1 : retVal = "Spring";
		break;
		case 8 : retVal = "Fall";
		break;
		case 12 : retVal = "Winter";
		break;
		case 5 : retVal = "Summer I";
		break;
		case 7 : retVal = "Summer II";
		break;
		default : retVal = "Unknown";
		break;
	}
	return retVal;
}

bool MyFunctions::regularSemester(int termCode) {
	return (termName(termCode) == "Fall" || termName(termCode) == "Spring");
}

int MyFunctions::termYear(int termCode) {
	//Should be first 4 digits.  Special case for winter term, though.
	int last2 = termCode % 100;
	int year = termCode/100;
	if (last2 == 12) year++;
	return year;
}

TString MyFunctions::academicYear(int termCode) {
	TString retVal = "AY";
	int term = termCode % 100;
	int thisYear = termYear(termCode);
	int year1 = thisYear;
	int year2 = thisYear;
	
	switch (term) {
		case 8 :
		year2++;
		break;
		case 12 :
		case 1 :
		case 5 :
		case 7 :
		year1--;
		break;
		default :
		break;
	}
	std::stringstream s_year1, s_year2;
	year1 = year1 % 100;
	year2 = year2 % 100;
	s_year1 << std::setfill('0') << std::setw(2) << year1;
	s_year2 << std::setfill('0') << std::setw(2) << year2;
	retVal += TString(s_year1.str());
	retVal += "-";
	retVal += TString(s_year2.str());
	
	return retVal;
}

TString MyFunctions::fiscalYear(int termCode) {
	TString retVal = "FY";
	int term = termCode % 100;
	int thisYear = termYear(termCode) % 100;
	switch (term) {
		case 8 : thisYear++;
		break;
		default :
		break;
	}

	retVal += thisYear;
	
	return retVal;
}