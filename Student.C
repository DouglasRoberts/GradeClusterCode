#include "Student.h"
#include "MyFunctions.h"
#include <iostream>

ClassImp(Student)
	
//
void Student::Finalize() {
	for (auto &enrollment : _enrollments) {
		for (auto grade : _grades) {
			if (grade.term == enrollment.term)
				enrollment.grades.push_back(grade);
		}
	}
	return;
} 

bool Student::ValidEnrollTypes() {

	// Limit this to new freshman who continuously return without changing status somehow.
	for (Enrollment enrollment : _enrollments) {
		if (enrollment.enrollType != "NF" &&
			enrollment.enrollType != "R1" &&
			enrollment.enrollType != "R2" &&
			enrollment.enrollType != "R3" &&
			enrollment.enrollType != "R4")
				return false;
	}
	return true;
} 

double Student::Gpa(int term) {
	
	// Function calculates GPA from entire Grades list.  No attempt to deal with repeated courses is made.
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	for (Grade const& grade : _grades) {
		// Is this a valid grade?
		if (MyFunctions::ValidGrade(grade.grade)) {
			if (term == 0) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*MyFunctions::GradeToQuality(grade.grade, grade.term);
			}
			else if (grade.term == term) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*MyFunctions::GradeToQuality(grade.grade, grade.term);				
			}
			else if (term < 0 && grade.term != -term) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*MyFunctions::GradeToQuality(grade.grade, grade.term);								
			}
		}
	}
	if (creditsAttempted > 0.)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

std::vector<Student::Grade> Student::TermGradeList(int term) {
	std::vector<Student::Grade> retVal;
	for (Grade const& grade : _grades) {
		// Find valid posted grades corresponding to the passed term
		if (grade.term == term && MyFunctions::ValidGrade(grade.grade))
			retVal.push_back(grade);
	}
	return retVal;
}

double Student::SemesterGpaWithoutCourse(int term, TString course) {
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	for (Grade const& grade : _grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (grade.term != term) continue;
		if (grade.course == course) continue;
		
		creditsAttempted += grade.credits;
		qualityPoints += grade.credits*MyFunctions::GradeToQuality(grade.grade, grade.term);
	}
	
	if (creditsAttempted > 0)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

double Student::EarnedCredits() {
	// This is currently a little bogus.  Grades of "P" or "S" would count as earned
	double earned = 0.;
	for (Grade const& grade : _grades) {
		if (MyFunctions::ValidGrade(grade.grade) && grade.grade != "F")
			earned += grade.credits;
	}
	return earned;
}

double Student::AttemptedCredits(int term) {
	double attempted = 0.;
	for (Grade const& grade : _grades) {
		if (MyFunctions::ValidGrade(grade.grade)) {
			if (term == 0)
				attempted += grade.credits;
			else if (term == grade.term)
				attempted += grade.credits;
		}
				
	}
	return attempted;
}

double Student::DegreeCredits() {
	double crMax = 0.;
	for (Enrollment const& enrollment : _enrollments) {
		if (enrollment.cumDegCredits > crMax) crMax = enrollment.cumDegCredits;
	}
	return crMax;
}

TString Student::EnrollmentType(int term) {
	for (Enrollment const& enrollment : _enrollments) {
		if (enrollment.term == term) return enrollment.enrollType;
	}
	std::cout << "Couldn't find enrollment for term " << term << ", ID = " << _id << std::endl;
	return "UNKN";
}
