#ifndef STUDENT_H
#define STUDENT_H

#include "CumulativeDistribution.h"

#include <TObject.h>
#include <TString.h>

#include <functional>
#include <iostream>
#include <vector>

class TGraph;

class Student : public TObject {
	
public:
	Student() : _id(-1) { }
	Student(int id, int firstTerm) : _id(id), _firstTerm(firstTerm) { }
	~Student() { }
	
	struct Grade {
		TString grade;
		double quality;
		TString course;
		float credits;
		int term;
		TString applicableCode;
		inline bool operator< (const Grade& rhs) const {return term < rhs.term;}
	};
	
	struct Enrollment {
		int term;
		float semCredits;
		float semGPA;
		float cumGPA;
		float cumCredits;
		float cumDegCredits;
		TString enrollType;
		std::vector<std::reference_wrapper<Grade>> grades;  //! This duplicates other grade list?
		inline bool operator< (const Enrollment& rhs) const {return term < rhs.term;}
	};
	
	struct Degree {
		int term;
		TString degree_cd;
		TString major_cd;
		inline bool operator< (const Degree& rhs) const {return term < rhs.term;}
	};
	
	typedef std::vector<Grade> GradeVector;
	
	void Finalize();  // This just puts copies of the grade items into their respective Enrollment items
	int Id() const {return _id;}
	int FirstTerm() const {return _firstTerm;}

	// For Gpa, if term == 0, use all terms.  If term > 0, use only that term.  If term < 0, exclude that term.
	double Gpa(int term = 0, bool normed = false, const Student::Grade* gradeToExclude = 0) const;
	// Calculate GPA from a list of grades
	double Gpa(const GradeVector grades) const;
	
	double SemesterGpaWithoutCourse(int term, TString course) const;
	double CumGpaWithoutCourse(int term, TString course) const;   // Need to include term as argument in case course was repeated.
	double EarnedCredits() const;
	double AttemptedCredits(int term = 0) const;
	double AttemptedCredits(const Enrollment& enrollment) const;
	double AllAttemptedCredits(int term = 0) const;   // Include courses taken S/P as well as Regular grading as a measure of total work load
	double AvgAttemptedCredits() const;               // This also include S/P grades
	double DegreeCredits() const;
	
	enum PredictionMethod {RAW, NORMED, DISTRIBUTION};
	
	double CourseGradePrediction(Student::Grade grade, PredictionMethod method) const;
	double TermGpaPrediction(int term, PredictionMethod method) const;
	
	// This first one is the real function.  The rest are just different signatures/names that call this
	CumulativeDistributionInverse CombinedCdfInv(const GradeVector grades, const Student::Grade* gradeToExclude = 0) const;
	CumulativeDistributionInverse CombinedCdfInv() const {return CombinedCdfInv(_grades);}
	
	CumulativeDistribution CombinedCdf(const GradeVector grades, const Student::Grade* gradeToExclude = 0) const
		{return CombinedCdfInv(grades, gradeToExclude).Cdf();}
	CumulativeDistribution CombinedCdf() const {return CombinedCdf(_grades);}
	CumulativeDistribution CombinedCdfWithoutCourse(const Student::Grade* grade) const {return CombinedCdf(_grades, grade);}

	int nDegrees() const {return _degrees.size();}
	
	bool ValidEnrollTypes() const;
	TString EnrollmentType(int term) const;
	
	void AddGrade(Grade grade) {_grades.push_back(grade);}
	GradeVector Grades() const {return _grades;}
	
	void AddEnrollment(Enrollment enrollment) {_enrollments.push_back(enrollment);}
	std::vector<Enrollment> Enrollments() const {return _enrollments;}
	Enrollment EnrollmentForTerm(int iterm) const;
	
	void AddDegree(Degree degree) {_degrees.push_back(degree);}
	std::vector<Degree> Degrees() const {return _degrees;}
	
	GradeVector TermLetterGradeList(int term) const;
	
private:
	
	int _id;
	int _firstTerm;
	GradeVector _grades;
	std::vector<Enrollment> _enrollments;
	std::vector<Degree> _degrees;
	
	const static bool _useAllTerms = true;
	
	GradeVector termGrades(int term) const;

	double NormedGpa(int term = 0) const {return Gpa(term, true);}
	double NormedGpaPrediction(int term) const;
	double NormedCoursePrediction(const Student::Grade* gradeToExclude) const;  // Single course prediction, in quality points
	double UnNormedGpa(double normedPrediction, int term, TString courseToExclude = "") const;

	ClassDef(Student,3)
};
#endif