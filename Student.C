#include "Student.h"
#include "MyFunctions.h"
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
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

double Student::ExpectedGpa(double pctRank) const {
	double sumCredits = 0.;
	double sumQpCr = 0.;
	for (Grade const& grade : _grades) {
		// Is this a valid grade?
		if (MyFunctions::ValidGrade(grade.grade)) {
			// Only count courses that we can find on the normed course map?
			auto thisCourse = MyFunctions::gradeNormMap.find(std::make_pair(grade.course, 0));
			if (thisCourse != MyFunctions::gradeNormMap.end()) {
				double credits = grade.credits;
				TH1D* cumDist = thisCourse->second.CumulativeDistribution();
				assert(0 != cumDist);
				int expectedBin = cumDist->FindFirstBinAbove(pctRank);
				TString expectedGrade = cumDist->GetXaxis()->GetBinLabel(expectedBin);
				sumCredits += credits;
				sumQpCr += credits*MyFunctions::GradeToQuality(expectedGrade);
			}
			else {
				std::cout << "Course not found in NormMap : " << grade.course << std::endl;
			}
		}
	}
	if (sumCredits > 0.)
		return sumQpCr/sumCredits;
	else
		return 0.;
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
	for (Grade const& grade : _grades) {
		if (gradeToExclude != 0 && grade.course == gradeToExclude->course && grade.term == gradeToExclude->term) continue;
		// Is this a valid grade?
		if (MyFunctions::ValidGrade(grade.grade)) {
			if (normed) {
				alpha = 1.; beta = 0.;  // Default values
				auto allCourses = MyFunctions::gradeNormMap.find(std::make_pair("AllCourses", 0));
				auto thisCourse = MyFunctions::gradeNormMap.find(std::make_pair(grade.course, 0));
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

double Student::Gpa(const std::vector<Student::Grade> grades) const {
	// Calculate GPA (raw) from a list of grade objects.  This will use all of them (if they are valid grades).
	double creditsAttempted = 0.;
	double qualityPoints = 0.;
	for (auto grade : grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		creditsAttempted += grade.credits;
		qualityPoints += grade.credits*MyFunctions::GradeToQuality(grade.grade, grade.term);
	}
	if(creditsAttempted > 0.)
		return qualityPoints/creditsAttempted;
	else
		return 0.;
}

std::vector<Student::Grade> Student::TermLetterGradeList(int term) const {
	std::vector<Student::Grade> retVal;
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
	auto allCourses = MyFunctions::gradeNormMap.find(std::make_pair("AllCourses", 0));
	auto thisCourse = MyFunctions::gradeNormMap.find(std::make_pair(gradeToExclude->course, 0));
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
	for (Grade const& grade : _grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (grade.term != term) continue;
		if (grade.course == courseToExclude) continue;
		auto allCourses = MyFunctions::gradeNormMap.find(std::make_pair("AllCourses", 0));
		auto thisCourse = MyFunctions::gradeNormMap.find(std::make_pair(grade.course, 0));
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
		qualityPoints += grade.credits*MyFunctions::GradeToQuality(grade.grade, grade.term);
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
		qualityPoints += grade.credits*MyFunctions::GradeToQuality(grade.grade, grade.term);
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

TGraph* Student::CombinedCdf(const std::vector<Student::Grade> grades, bool inverse, const Student::Grade* exclude) const {
	MyFunctions::BuildGradeNormMap();  // Just in case it wasn't built before.  It should be cached and return right away if it's there.
	
	std::vector<std::pair<double, double>> deltas;
	double totalCredits = 0.;
	for (auto grade : grades) {
		if (!MyFunctions::ValidGrade(grade.grade)) continue;
		if (exclude != 0 && exclude->course == grade.course && exclude->term == grade.term) continue;
		auto search = MyFunctions::gradeNormMap.find(std::make_pair(grade.course, 0));
		if (search == MyFunctions::gradeNormMap.end()) continue;
		CourseGradeNormer& cgn = search->second;
		TGraph* inv = cgn.CumulativeGraphInverse();
		
		double yPrev = 0.;
		int n = inv->GetN();
		double* x = inv->GetX();
		double* y = inv->GetY();
		for (int i = 0; i < n; ++i) {
			double delY = y[i] - yPrev;
			delY *= grade.credits;
			yPrev = y[i];
			deltas.push_back(std::make_pair(x[i], delY));
		}
		totalCredits += grade.credits;
	}
	std::sort(deltas.begin(), deltas.end());
	double yTotal = 0.;
	std::vector<double> xvals;
	std::vector<double> yvals;
//	std::vector<double> errEmpty;
	std::vector<double> errXminus;
	double xPrev = 0.;
	for (auto point : deltas) {
		yTotal += point.second/totalCredits;
		// Try to clean up the distribution?  There should only be one point at a given x value
		if (xPrev >= point.first) {
			yvals.back() = yTotal; 
		}
		else {
			xvals.push_back(point.first);
			yvals.push_back(yTotal);
		}
		xPrev = point.first;
	}
	xPrev = 0.;
	for (auto x : xvals) {
		errXminus.push_back(x - xPrev);
		xPrev = x;
	}

	TGraph* retVal = 0;
	if (inverse) {
		retVal = new TGraphAsymmErrors(xvals.size(), &xvals[0], &yvals[0], &errXminus[0]);
		retVal->SetTitle("Inverse Cumulative Distribution Function;Percentile;GPA");
	}
	else {
		retVal = new TGraphAsymmErrors(xvals.size(), &yvals[0], &xvals[0], 0, 0, &errXminus[0]);
		retVal->SetTitle("Cumulative Distribution Function;GPA;Percentile");
	}
	retVal->SetMarkerSize(0.5);
	retVal->SetMarkerStyle(20);
	retVal->SetMarkerColor(kBlue);
	return retVal;
}
