#include "Student.h"
#include "MyFunctions.h"
#include <cassert>
#include <iostream>

ClassImp(Student)
	
//
void Student::Finalize() {
	for (auto &enrollment : _enrollments) {
		for (auto &grade : _grades) {
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

double Student::ExpectedGpa(double pctRank) {
	double sumCredits = 0.;
	double sumQpCr = 0.;
	for (Grade const& grade : _grades) {
		// Is this a valid grade?
		if (MyFunctions::ValidGrade(grade.grade)) {
			// Only count courses that we can find on the normed course map?
			auto thisCourse = MyFunctions::gradeNormMap.find(grade.course);
			if (thisCourse != MyFunctions::gradeNormMap.end()) {
				double credits = grade.credits;
				TH1D* cumDist = thisCourse->second.CumulativeDistribution();
				assert(0 != cumDist);
				int expectedBin = cumDist->FindFirstBinAbove(pctRank);
				TString expectedGrade = cumDist->GetXaxis()->GetBinLabel(expectedBin);
				sumCredits += credits;
				sumQpCr += credits*MyFunctions::GradeToQuality(expectedGrade);
			}
		}
	}
	if (sumCredits > 0.)
		return sumQpCr/sumCredits;
	else
		return 0.;
}

double Student::Gpa(int term, bool normed) {
	
	// Function calculates GPA from entire Grades list.  No attempt to deal with repeated courses is made.
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	double alpha = 1.;
	double beta = 0.;
	for (Grade const& grade : _grades) {
		// Is this a valid grade?
		if (MyFunctions::ValidGrade(grade.grade)) {
			if (normed) {
				alpha = 1.; beta = 0.;  // Default values
				auto allCourses = MyFunctions::gradeNormMap.find("AllCourses");
				auto thisCourse = MyFunctions::gradeNormMap.find(grade.course);
				if (allCourses != MyFunctions::gradeNormMap.end() && thisCourse != MyFunctions::gradeNormMap.end()) {
					double allAvg = allCourses->second.Average();
					double allStdDev = allCourses->second.StdDev();
					double thisAvg = thisCourse->second.Average();
					double thisStdDev = thisCourse->second.StdDev();
					if (thisStdDev > 0.) {
						alpha = allStdDev/thisStdDev;
						beta = allAvg - alpha*thisAvg;
					}
				}
			}
			if (term == 0) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*(alpha*MyFunctions::GradeToQuality(grade.grade, grade.term) + beta);
			}
			else if (grade.term == term) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*(alpha*MyFunctions::GradeToQuality(grade.grade, grade.term) + beta);				
			}
			else if (term < 0 && grade.term != -term) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*(alpha*MyFunctions::GradeToQuality(grade.grade, grade.term) + beta);								
			}
		}
	}
	if (creditsAttempted > 0.)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

std::vector<Student::Grade> Student::TermLetterGradeList(int term) const {
	std::vector<Student::Grade> retVal;
	for (Grade const& grade : _grades) {
		// Find valid posted grades corresponding to the passed term
		if (grade.term == term && MyFunctions::ValidGrade(grade.grade))
			retVal.push_back(grade);
	}
	return retVal;
}

double Student::UnNormedGpa(double normedPrediction, int term, TString courseToExclude) {
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	double alpha = 1.;
	double beta = 0.;
	for (Grade const& grade : _grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (grade.term != term) continue;
		if (grade.course == courseToExclude) continue;
		auto allCourses = MyFunctions::gradeNormMap.find("AllCourses");
		auto thisCourse = MyFunctions::gradeNormMap.find(grade.course);
		alpha = 1.; beta = 0.;
		if (allCourses != MyFunctions::gradeNormMap.end() && thisCourse != MyFunctions::gradeNormMap.end()) {
			double allAvg = allCourses->second.Average();
			double allStdDev = allCourses->second.StdDev();
			double thisAvg = thisCourse->second.Average();
			double thisStdDev = thisCourse->second.StdDev();
			if (thisStdDev > 0.) {
				alpha = allStdDev/thisStdDev;
				beta = allAvg - alpha*thisAvg;
			}
		}
		
		
		creditsAttempted += grade.credits;
		qualityPoints += grade.credits*(normedPrediction - beta)/alpha;
	}
	
	if (creditsAttempted > 0)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
	
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

double Student::AllAttemptedCredits(int term) {
	// This includes grades of S and P as valid
	double attempted = 0.;
	for (Grade const& grade : _grades) {
		if (MyFunctions::ValidGradeAny(grade.grade)) {
			if (term == 0)
				attempted += grade.credits;
			else if (term == grade.term)
				attempted += grade.credits;
		}
				
	}
	return attempted;
	
}

double Student::AvgAttemptedCredits() {
	double retVal = 0.;
	int nTerms = 0;
	for (auto const& enrollment : _enrollments) {
		// Only look at Fall and Spring terms
		TString termName = MyFunctions::termName(enrollment.term);
		if (termName == "Fall" || termName == "Spring") {
			nTerms++;
			// Count credits for all courses with grades posted
			for (Student::Grade const& grade : enrollment.grades) {
				if (MyFunctions::ValidGradeAny(grade.grade)) {
					retVal += grade.credits;
				}
			}
		}
	}
	if (nTerms > 0)
		return retVal/nTerms;
	else
		return 0.;
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
