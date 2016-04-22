#include "Student.h"

#include "CumulativeDistribution.h"
#include "MyFunctions.h"
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TH1D.h>
#include <cassert>
#include <iostream>

ClassImp(Student)
	
	
void Student::Finalize() {
	for (auto &enrollment : _enrollments) {
		for (auto &grade : _grades) {
//			std::cout << "Grade = " << grade.grade << ", Term = " << enrollment.term << std::endl;
			if (grade.term == enrollment.term)
				enrollment.grades.push_back(grade);
		}
	}
} 

bool Student::ValidEnrollTypes() const {

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

double Student::Gpa(int term, bool normed, const Student::Grade* gradeToExclude) const {
	
	// This will calculate a student's gpa with various options:
	//		term = 0, => all terms
	//		term > 0, => only that term
	//		term < 0, => exclude that term
	//		bool normed controls whether or not course normalization takes place.  If true, returned value is in "normed" gpa space (beware!)
	//		gradeToExclude can exclude a single course from calculation.
	
	// Function calculates GPA from entire Grades list.  No attempt to deal with repeated courses is made.
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	double alpha = 1.;
	double beta = 0.;
	
	int useTerm = 0;
	for (Grade const& grade : _grades) {
		if (gradeToExclude != 0 && grade.course == gradeToExclude->course && grade.term == gradeToExclude->term) continue;
		// Is this a valid grade?
		if (grade.quality > -0.5) {
//		if (MyFunctions::ValidGrade(grade.grade)) {
			if (!_useAllTerms) useTerm = grade.term;
			if (normed) {
				alpha = 1.; beta = 0.;  // Default values
				auto allCourses = MyFunctions::gradeNormMap.find(std::make_pair("AllCourses", useTerm));
				auto thisCourse = MyFunctions::gradeNormMap.find(std::make_pair(grade.course, useTerm));
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
				qualityPoints += grade.credits*(alpha*grade.quality + beta);
			}
			else if (grade.term == term) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*(alpha*grade.quality + beta);				
			}
			else if (term < 0 && grade.term != -term) {
				creditsAttempted += grade.credits;
				qualityPoints += grade.credits*(alpha*grade.quality + beta);								
			}
		}
	}
	if (creditsAttempted > 0.)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

double Student::Gpa(const GradeVector grades) const {
	// Calculate GPA (raw) from a list of grade objects.  This will use all of them (if they are valid grades).
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	for (auto grade : grades) {
		if (grade.quality < -0.5) continue;
//		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		creditsAttempted += grade.credits;
		qualityPoints += grade.credits*grade.quality;
	}
	if(creditsAttempted > 0.)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

Student::GradeVector Student::TermLetterGradeList(int term) const {
	GradeVector retVal;
	for (auto const& grade : _grades) {
		// Find valid posted grades corresponding to the passed term
		// If term is negative, return all valid grades that aren't from this term
		if (MyFunctions::ValidGrade(grade.grade)) {
			if (term > 0 && grade.term == term)
				retVal.push_back(grade);
			if (term < 0 && grade.term != -term) {
				retVal.push_back(grade);
			}
		}
	}
	return retVal;
}

double Student::NormedGpaPrediction(int term) const {
	return UnNormedGpa(NormedGpa(-term), term);
}

double Student::NormedCoursePrediction(const Student::Grade* gradeToExclude) const {
	assert(MyFunctions::ValidGrade(gradeToExclude->grade));
	
	double primedPrediction = Gpa(0, true, gradeToExclude);  // GPA for all courses except this one, in "normed" gpa space
	// Now, un-norm this prediction for this course
	double alpha = 1.;
	double beta = 0.;
	auto allCourses = MyFunctions::gradeNormMap.find(std::make_pair("AllCourses", gradeToExclude->term));
	auto thisCourse = MyFunctions::gradeNormMap.find(std::make_pair(gradeToExclude->course, gradeToExclude->term));
	if (thisCourse != MyFunctions::gradeNormMap.end() && allCourses != MyFunctions::gradeNormMap.end()) {
		if (thisCourse->second.StdDev() > 0.) {
			alpha = allCourses->second.StdDev()/thisCourse->second.StdDev();
			beta = allCourses->second.Average() - alpha*thisCourse->second.Average();
		}
	}
	return (primedPrediction - beta)/alpha;
}

double Student::UnNormedGpa(double normedPrediction, int term, TString courseToExclude) const {
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	double alpha = 1.;
	double beta = 0.;
	int useTerm = 0;
	for (Grade const& grade : _grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (grade.term != term) continue;
		if (grade.course == courseToExclude) continue;
		if (!_useAllTerms) useTerm = grade.term;
		auto allCourses = MyFunctions::gradeNormMap.find(std::make_pair("AllCourses", useTerm));
		auto thisCourse = MyFunctions::gradeNormMap.find(std::make_pair(grade.course, useTerm));
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

double Student::SemesterGpaWithoutCourse(int term, TString course) const {
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	for (Grade const& grade : _grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (grade.term != term) continue;
		if (grade.course == course) continue;
		
		creditsAttempted += grade.credits;
		qualityPoints += grade.credits*grade.quality;
	}
	
	if (creditsAttempted > 0)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

double Student::CumGpaWithoutCourse(int term, TString course) const {
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	for (auto const& grade : _grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (grade.course == course && grade.term == term) continue;
		creditsAttempted += grade.credits;
		qualityPoints += grade.credits*grade.quality;
	}
	if (creditsAttempted > 0.)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

double Student::EarnedCredits() const {
	// This is currently a little bogus.  Grades of "P" or "S" would count as earned
	double earned = 0.;
	for (Grade const& grade : _grades) {
		if (MyFunctions::ValidGrade(grade.grade) && grade.grade != "F")
			earned += grade.credits;
	}
	return earned;
}

double Student::AttemptedCredits(int term) const {
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

double Student::AttemptedCredits(const Enrollment& enrollment) const {
	double attempted = 0.;
	for (Grade const& grade : enrollment.grades) {
		if (MyFunctions::ValidGrade(grade.grade))
			attempted += grade.credits;
	}
	return attempted;
}

double Student::AllAttemptedCredits(int term) const {
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

double Student::AvgAttemptedCredits() const {
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

double Student::DegreeCredits() const {
	double crMax = 0.;
	for (Enrollment const& enrollment : _enrollments) {
		if (enrollment.cumDegCredits > crMax) crMax = enrollment.cumDegCredits;
	}
	return crMax;
}

TString Student::EnrollmentType(int term) const {
	for (Enrollment const& enrollment : _enrollments) {
		if (enrollment.term == term) return enrollment.enrollType;
	}
	std::cout << "Couldn't find enrollment for term " << term << ", ID = " << _id << std::endl;
	return "UNKN";
}

CumulativeDistributionInverse Student::CombinedCdfInv(const GradeVector grades, const Student::Grade* gradeToExclude) const {
	MyFunctions::BuildGradeNormMap();  // Just in case it wasn't built before.  It should be cached and return right away if it's there.
	CumulativeDistributionInverse retVal = CumulativeDistributionInverse();
	
	// Just add together the inverse cdfs from list of grades passed.
	std::vector<std::pair<CumulativeDistributionInverse*, double>> list;
	int useTerm = 0;
	for (auto grade : grades) {
		if (gradeToExclude != 0 && grade.course == gradeToExclude->course && grade.term == gradeToExclude->term) continue;
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (!_useAllTerms) useTerm = grade.term;
		auto search = MyFunctions::gradeNormMap.find(std::make_pair(grade.course, useTerm));
		if (search == MyFunctions::gradeNormMap.end()) continue;
		CourseGradeNormer& cgn = search->second;
		list.push_back(std::make_pair(cgn.CumulativeGraphInverse(), grade.credits));
	}
	retVal.Add(list);
	
	return retVal;
}

double Student::CourseGradePrediction(Student::Grade grade, PredictionMethod method) const {
	
	switch (method)
	{
		case RAW:
		// Predicted grade is just overall GPA without this course (unbiased)
		return Gpa(0, false, &grade);
		
		case NORMED:
		return NormedCoursePrediction(&grade);
		
		case DISTRIBUTION:
//		std::cout << "3" << std::endl;
		CumulativeDistribution cdf = CombinedCdfWithoutCourse(&grade);
//		std::cout << "4" << std::endl;
		double unbiasedGpa = CourseGradePrediction(grade, RAW);
		double effectivePct = cdf.Evaluate(unbiasedGpa);
//		std::cout << "5" << std::endl;
		GradeVector oneClass;
		oneClass.push_back(grade);
		CumulativeDistributionInverse invCdfCourse = CombinedCdfInv(oneClass);
//		std::cout << "6" << std::endl;
		return invCdfCourse.Evaluate(effectivePct);
	}	
}

double Student::TermGpaPrediction(int term, PredictionMethod method) const {

	switch (method)
	{
		case RAW:
		// Predicted semester GPA is just overall GPA without this term (unbiased)
		return Gpa(-term, false);
		
		case NORMED:
		return NormedGpaPrediction(term);
		
		case DISTRIBUTION:
		// Find student's effective % rank, excluding this term, based on the rest of their terms' CDFs...
		GradeVector useThese = termGrades(-term);
		CumulativeDistribution cdf = CombinedCdf(useThese);
		double unbiasedGpa = Gpa(useThese);
		double effectivePct = cdf.Evaluate(unbiasedGpa);
		CumulativeDistributionInverse invCdfTerm = CombinedCdfInv(termGrades(term));
		return invCdfTerm.Evaluate(effectivePct);		
	}
}

Student::GradeVector Student::termGrades(int term) const {
	GradeVector retVal;
	for (auto const& enrollment : _enrollments) {
		if (term > 0) {
			if (enrollment.term != term) continue;
		}
		else if (term < 0) {
			if (enrollment.term == -term) continue;
		}
		for (Student::Grade grade : enrollment.grades)
			retVal.push_back(grade);
		
	}
	return retVal;
}
