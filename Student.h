#ifndef STUDENT_H
#define STUDENT_H

#include <TObject.h>
#include <TString.h>
#include <vector>
#include <set>

class Student : public TObject {
	
public:
	Student() : _id(-1) { }
	Student(int id, int firstTerm) : _id(id), _firstTerm(firstTerm) { }
	~Student() { }
	
	struct Grade {
		TString grade;
		TString course;
		float credits;
		int term;
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
		std::vector<Grade> grades;  // This duplicates other grade list?
		inline bool operator< (const Enrollment& rhs) const {return term < rhs.term;}
	};
	
	struct Degree {
		int term;
		TString degree_cd;
		TString major_cd;
		inline bool operator< (const Degree& rhs) const {return term < rhs.term;}
	};
	
	
	void Finalize();  // This just puts copies of the grade items into their respective Enrollment items
	int Id() {return _id;}
	int FirstTerm() {return _firstTerm;}
	// For Gpa, if term == 0, use all terms.  If term > 0, use only that term.  If term < 0, exclude that term.
	double Gpa(int term = 0);
	double SemesterGpaWithoutCourse(int term, TString course);
	double EarnedCredits();
	double AttemptedCredits(int term = 0);
	double DegreeCredits();
	
	int nDegrees() {return _degrees.size();}
	
	bool ValidEnrollTypes();
	TString EnrollmentType(int term);
	
	void AddGrade(Grade grade) {_grades.push_back(grade);}
	std::vector<Grade> Grades() {return _grades;}
	
	void AddEnrollment(Enrollment enrollment) {_enrollments.push_back(enrollment);}
	std::vector<Enrollment> Enrollments() {return _enrollments;}
	
	void AddDegree(Degree degree) {_degrees.push_back(degree);}
	std::vector<Degree> Degrees() {return _degrees;}
	
	std::vector<Grade> TermGradeList(int term);
	
private:
	
	int _id;
	int _firstTerm;
	std::vector<Grade> _grades;
	std::vector<Enrollment> _enrollments;
	std::vector<Degree> _degrees;

	ClassDef(Student,2)
};
#endif