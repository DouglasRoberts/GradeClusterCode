#include "MyFunctions.h"
#include "Student.h"

#include <TFile.h>
#include <TH1D.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TSQLServer.h>
#include <TString.h>
#include <TTree.h>

#include <map>
#include <iostream>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


void CreateStudentTree() {
	
	std::cout << "About to create Student objects..." << std::endl;
//	std::vector<std::unique_ptr<Student>> students;	
	std::map<int, Student*> students;
//	std::map<int, std::unique_ptr<Student>> students;
	typedef std::map<int, std::unique_ptr<Student>>::iterator it_type;
	// Connect to database...
	TSQLServer* db = TSQLServer::Connect("sqlite://LearningAnalytics.db", "", "");
	if (0 == db) {
		std::cout << "Error connecting to database" << std::endl;
		return;
	}
	TSQLRow* row;
	TSQLResult* res;
	
	// Get distinct list of student ids from student_enrollments_by_term (is this the best table to start from?)
	// Also, limit this at this point to students that received a degree
	TString query = "SELECT DISTINCT e.newid, e.TERM FROM student_enrollments_by_term AS e";
	query += " JOIN student_degrees_by_term AS d ON e.newid = d.newid";
	query += " WHERE e.ENROLL_TYPE_CD = 'NF'";
	query += " AND e.FULL_TIME_IND = 'Y'";
	query += " AND e.DEG_SEEKING_IND = 'Y'";
	query += " AND e.OFFICIAL_ENROLLED_IND = 'Y'";
	query += " AND e.TERM LIKE '____08'";
	query += " AND (d.DEGREE_CD LIKE '1%' OR d.DEGREE_CD LIKE '2%')";
	res = db->Query(query.Data());
	int nRows = 0;
	while ((row = res->Next())) {
		++nRows;
		int student_id = atoi(row->GetField(0));
		int term = atoi(row->GetField(1));
//		students[student_id] = (make_unique<Student>(student_id, term));
		students[student_id] = new Student(student_id, term);
//		int degree_cd = atoi(row->GetField(1));
//		if (degree_cd < 10 || degree_cd >= 30)
//			std::cout << "Degree code = " << row->GetField(1) << std::endl;
		delete row;
	}
	delete res;
	std::cout << "Found " << nRows << " distinct student ids in table" << std::endl;
	
	// Degree Data...
	query = "SELECT newid, DEG_TERM, DEGREE_CD, MAJOR_CD FROM student_degrees_by_term";
	res = db->Query(query.Data());
	while ((row = res->Next())) {
		int thisId = atoi(row->GetField(0));
		if (students.find(thisId) != students.end()) {
			Student::Degree degree;
			degree.term = atoi(row->GetField(1));
			degree.degree_cd = row->GetField(2);
			degree.major_cd = row->GetField(3);
			students[thisId]->AddDegree(degree);
		}
		delete row;
	}
	delete res;
	
	// Enrollment Data...
	query = "SELECT newid, TERM, SEM_END_CR_ERN, SEM_END_GPA, END_CUM_GPA_UG, END_CUM_CR_ERN_UG, END_CUM_CR_DEG_UG, ENROLL_TYPE_CD FROM student_enrollments_by_term";
	res = db->Query(query.Data());
	while ((row = res->Next())) {
		int thisId = atoi(row->GetField(0));
		if (students.find(thisId) != students.end()) {
			Student::Enrollment enrollment;
			enrollment.term = atoi(row->GetField(1));
			enrollment.semCredits = atof(row->GetField(2));
			enrollment.semGPA = atof(row->GetField(3));
			enrollment.cumGPA = atof(row->GetField(4));
			enrollment.cumCredits = atof(row->GetField(5));
			enrollment.cumDegCredits = atof(row->GetField(6));
			enrollment.enrollType = row->GetField(7);
			students[thisId]->AddEnrollment(enrollment);			
		}
		delete row;
	}
	delete res;
	
	// Now fill the students with interesting things...
	query = "SELECT newid, TERM, COURSE, CRS_CREDIT, CRS_GRADE FROM student_courses_by_term WHERE CRS_LEV_APPLIED_CD='1' AND CRS_APPLICABLE_CD='01'";
	res = db->Query(query.Data());
	int nGrRecs = 0;
	while ((row = res->Next())) {
		int thisId = atoi(row->GetField(0));
		if (students.find(thisId) != students.end()) {
			nGrRecs++;
			Student::Grade gradeRecord;
			gradeRecord.grade = row->GetField(4);
			gradeRecord.quality = MyFunctions::GradeToQuality(gradeRecord.grade);
			gradeRecord.course = row->GetField(2);
			gradeRecord.credits = atof(row->GetField(3));
			gradeRecord.term = atoi(row->GetField(1));
			students[thisId]->AddGrade(gradeRecord);
		}
		delete row;
	}
	std::cout << "Number of course grades found = " << nGrRecs << std::endl;
	delete res;

	// Write Student list out to a ROOT file for safe keeping...
	TFile* fOut = new TFile("Students.root", "RECREATE");
	TTree* studentTree = new TTree("Students", "Student Information Tree");
	TH1D* cumCreditsHist = new TH1D("cumCreditsHist", "Cum Deg Credits", 100, 0., 200.);
	Student* student = 0;
	studentTree->Branch("student", &student);
	
	int nPrinted = 0;
	int nWeirdEnrollTypes = 0;
	for (auto entry : students) {
		student = entry.second;
		// Should we filter out some students?  It looks like there are some records where the number of credits earned
		// and degree info doesn't really jive...
		cumCreditsHist->Fill(student->DegreeCredits());
		if (student->nDegrees() == 0) {
			std::cout << "Found a student without a degree!!! " << student->DegreeCredits() << std::endl; 
		}
		if (student->DegreeCredits() < 120.) {
			if (nPrinted < 0) {
				std::cout << "Less than 120 credits: " << student->Id() << ", " << student->DegreeCredits() << std::endl;
				nPrinted++;
				std::cout << "Term\tCUM_DEG_CREDITS" << std::endl;
				for (Student::Enrollment enrollment : student->Enrollments()) {
					std::cout << enrollment.term << "\t" << enrollment.cumDegCredits << std::endl;
				}
			}
			continue;
		}
		if (!student->ValidEnrollTypes()) {
			if (nPrinted < 0) {
				std::cout << "Weird Enroll Types: " << student->Id() << ", " << student->DegreeCredits() << std::endl;
				nPrinted++;
				std::cout << "Term\tENROLL_TYPE" << std::endl;
				for (Student::Enrollment enrollment : student->Enrollments()) {
					std::cout << enrollment.term << "\t" << enrollment.enrollType << std::endl;
				}
			}
			nWeirdEnrollTypes++;
			continue;
		}
		// Check for students with grades for a semester but no enrollment record.  Just toss them?
		bool goodStudent = true;
		for (Student::Grade grade : student->Grades()) {
			if (student->EnrollmentType(grade.term) == "UNKN") goodStudent = false;
		}
		if (!goodStudent) continue;
		studentTree->Fill();
	}
	
	std::cout << "Found a total of " << nWeirdEnrollTypes << " weird enrollment types" << std::endl;
	cumCreditsHist->DrawCopy();
	studentTree->Print();
	studentTree->Write();
	fOut->Close();
	
	return;
}

