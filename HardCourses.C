#include "Student.h"
#include "MyFunctions.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <vector>
#include <map>
#include <iostream>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct CourseDelta {
	static const int minEntries = 10;
	TString name;
	int nEntries;
	double deltaSum;
	double deltaSum2;
	void AddDelta(double delta) {
		deltaSum += delta;
		deltaSum2 += delta*delta;
		nEntries++;
	}
	double average() const {
		if (nEntries >= minEntries)
			return deltaSum/nEntries;
		else
			return 0.;
	}
	double errorOnMean() const {
		return rms()/sqrt(nEntries);
	}
	
	double significance() const {
		if (rms() > 0.)
			return fabs(average())/errorOnMean();
		else
			return 0.;
	}
	
	double rms() const {
		if (nEntries > minEntries)
			return sqrt(deltaSum2/nEntries - average()*average());
		else
			return 9999999.;
	}
	CourseDelta& operator+= (const CourseDelta& other) {
		this->nEntries += other.nEntries;
		this->deltaSum += other.deltaSum;
		this->deltaSum2 += other.deltaSum2;
		return *this;
	}
};

void CreateStudentObjects() {
	
	std::cout << "About to create Student objects..." << std::endl;
//	std::vector<std::unique_ptr<Student>> students;	
	std::map<int, std::unique_ptr<Student>> students;
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
		students[student_id] = (make_unique<Student>(student_id, term));
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
	query = "SELECT newid, TERM, COURSE, CRS_CREDIT, CRS_GRADE FROM student_courses_by_term WHERE CRS_LEV_APPLIED_CD='1'";
	res = db->Query(query.Data());
	int nGrRecs = 0;
	while ((row = res->Next())) {
		int thisId = atoi(row->GetField(0));
		if (students.find(thisId) != students.end()) {
			nGrRecs++;
			Student::Grade gradeRecord;
			gradeRecord.grade = row->GetField(4);
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
	Student student;
	studentTree->Branch("student", &student);
	int nPrinted = 0;
	int nWeirdEnrollTypes = 0;
	for (it_type entry = students.begin(); entry != students.end(); entry++) {
		student = *(entry->second);
		// Should we filter out some students?  It looks like there are some records where the number of credits earned
		// and degree info doesn't really jive...
		cumCreditsHist->Fill(student.DegreeCredits());
		if (student.nDegrees() == 0) {
			std::cout << "Found a student without a degree!!! " << student.DegreeCredits() << std::endl; 
		}
		if (student.DegreeCredits() < 120.) {
			if (nPrinted < 0) {
				std::cout << "Less than 120 credits: " << student.Id() << ", " << student.DegreeCredits() << std::endl;
				nPrinted++;
				std::cout << "Term\tCUM_DEG_CREDITS" << std::endl;
				for (Student::Enrollment enrollment : student.Enrollments()) {
					std::cout << enrollment.term << "\t" << enrollment.cumDegCredits << std::endl;
				}
			}
			continue;
		}
		if (!student.ValidEnrollTypes()) {
			if (nPrinted < 0) {
				std::cout << "Weird Enroll Types: " << student.Id() << ", " << student.DegreeCredits() << std::endl;
				nPrinted++;
				std::cout << "Term\tENROLL_TYPE" << std::endl;
				for (Student::Enrollment enrollment : student.Enrollments()) {
					std::cout << enrollment.term << "\t" << enrollment.enrollType << std::endl;
				}
			}
			nWeirdEnrollTypes++;
			continue;
		}
		// Check for students with grades for a semester but no enrollment record.  Just toss them?
		bool goodStudent = true;
		for (Student::Grade grade : student.Grades()) {
			if (student.EnrollmentType(grade.term) == "UNKN") goodStudent = false;
		}
		if (!goodStudent) continue;
		student.Finalize();
//		if (student.EarnedCredits() >= 80)
		studentTree->Fill();
//		else
//			std::cout << "Student with less than 80 earned credits : " << student.Id() << std::endl;
	}
	
	std::cout << "Found a total of " << nWeirdEnrollTypes << " weird enrollment types" << std::endl;
	
	cumCreditsHist->DrawCopy();
	studentTree->Write();
	fOut->Close();
	
	return;
}

void LookAtStudents(int iterationsMax = 10) {
	// Just a quick look at the data in the Students TTree
	TFile* f = new TFile("Students.root");
	TTree* studentTree = (TTree*)f->Get("Students");
	Student* student = 0;
	studentTree->SetBranchAddress("student", &student);
	
	Long64_t nentries = studentTree->GetEntriesFast();
	
	std::cout << "Entries in Students TTree = " << nentries << std::endl;
	
	TH1D* gpaHist = new TH1D("gpaHist", "All Students GPA", 100, 0., 4.);
	TH1D* earnedHist = new TH1D("earnedHist", "Earned Credits", 100, 0., 200.);
	TH1D* attemptedHist = new TH1D("attemptedHist", "Attempted Credits", 100, 0., 200.);
	TH2D* eVaHist = new TH2D("eVaHist", "Earned vs. Attempted Credits", 100, 0., 200., 100, 0., 200.);
	TH2D* semGpaHist = new TH2D("semGpaHist", "Semester GPA vs. Unbiased GPA", 100, 0., 4., 100, 0., 4.);
	TH1D* deltaGpaHist = new TH1D("deltaGpaHist", "Difference betwen prediced and unbiased semester GPA", 100, -4., 4.);
	
	TProfile* gpaPredictionVsStatus = new TProfile("gpaPredictionVsStatus", "Predicted GPA Residual vs. Student Enrollment Status", 5, 0., 5.);
	gpaPredictionVsStatus->GetXaxis()->SetBinLabel(1, "NF");
	gpaPredictionVsStatus->GetXaxis()->SetBinLabel(2, "R1");
	gpaPredictionVsStatus->GetXaxis()->SetBinLabel(3, "R2");
	gpaPredictionVsStatus->GetXaxis()->SetBinLabel(4, "R3");
	gpaPredictionVsStatus->GetXaxis()->SetBinLabel(5, "R4");
	TH1D* enrollStatusHist = new TH1D("enrollStatusHist", "Enrollment Status Codes", 5, 0., 5.);
	enrollStatusHist->GetXaxis()->SetBinLabel(1, "NF");
	enrollStatusHist->GetXaxis()->SetBinLabel(2, "R1");
	enrollStatusHist->GetXaxis()->SetBinLabel(3, "R2");
	enrollStatusHist->GetXaxis()->SetBinLabel(4, "R3");
	enrollStatusHist->GetXaxis()->SetBinLabel(5, "R4");
	
	TH2D* courseCorrelation = new TH2D("courseCorrelation", "Matrix of courses taken simultaneously", 8814, 0., 8814., 8814, 0., 8814.);
	TH1D* courseCorrNorm = new TH1D("courseCorrNorm", "Normalization of courses taken simultaneously", 8814, 0., 8814.);
	
	std::map<TString, CourseDelta> emptyMap;
	std::map<TString, CourseDelta> hardCourses;
	std::map<TString, CourseDelta> hardPrefixes;
	
//	nentries = 100;
	TBenchmark myBenchmark;
	
	myBenchmark.Start("HardCourses, Loop 1");
	// First, let's loop over everything and create both the enrollment status correction and the seed for the hard courses calculations	
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		// Only use half the students for training
		if (student->Id() % 2 != 0) continue;
		for (auto const& enrollment : student->Enrollments()) {
			// Only look at Fall and Spring Terms
			if (MyFunctions::termName(enrollment.term) != "Fall" && MyFunctions::termName(enrollment.term) != "Spring") continue;
			// Only look at semesters with at least 12 attempted credits
			if (student->AttemptedCredits(enrollment.term) < 12.) continue;
			
			// Loop over all courses and create an initial map with CourseDelta objects for each course.
			for (auto const& grade : enrollment.grades) {
				// Only look at courses 3 or more credits
				if (grade.credits < 2.99) continue;
				emptyMap[grade.course].name = grade.course;
//				emptyMap[grade.course].credits = grade.credits;
				bool filled = false;
				for (auto const& grade2 : enrollment.grades) {
					if (grade2.credits < 2.99) continue;
					if (grade.course != grade2.course) {
						courseCorrelation->Fill(grade.course, grade2.course, 1.);
						filled = true;
					}
				}
				if (filled) courseCorrNorm->Fill(grade.course, 1.);
			}
			// Fill plot with GPA prediction vs. enrollment status for later use
			double delGpa = student->Gpa(enrollment.term) - student->Gpa(-enrollment.term);
			gpaPredictionVsStatus->Fill(enrollment.enrollType, delGpa);
			enrollStatusHist->Fill(enrollment.enrollType, 1.);
		}
	}
	
	std::cout << "Size of empty course map = " << emptyMap.size() << std::endl;
	
	//Create a map from enroll_type to GPA correction
	std::map<TString, double> enrollTypeCorrection;
	for (int iBin = 1; iBin <= gpaPredictionVsStatus->GetNbinsX(); iBin++) {
		TString label = gpaPredictionVsStatus->GetXaxis()->GetBinLabel(iBin);
		enrollTypeCorrection[label]= gpaPredictionVsStatus->GetBinContent(iBin);
	}
	
	//Plot some of this stuff
	TCanvas* canvas1 = new TCanvas("canvas1", "Hard Courses Plots");
	canvas1->Divide(2,2);
	canvas1->cd(1);
	gpaPredictionVsStatus->DrawCopy();
	canvas1->cd(2);
	enrollStatusHist->DrawCopy();
	canvas1->Modified();
	canvas1->Update();
	
//	TCanvas* correlationCanvas = new TCanvas("correlationCanvas", "Course Correlations", 1800, 1200);
//	correlationCanvas->cd();
	// Normalize the course correlation matrix
	for (int i = 1; i <= courseCorrNorm->GetXaxis()->GetNbins(); i++) {
		double norm = courseCorrNorm->GetBinContent(i);
		assert(courseCorrNorm->GetXaxis()->GetBinLabel(i) == courseCorrelation->GetXaxis()->GetBinLabel(i));
		if (norm == 0.) continue;
		for (int j = 1; j <= courseCorrNorm->GetXaxis()->GetNbins(); j++) {
			double normedContent = courseCorrelation->GetBinContent(i,j)/norm;
			courseCorrelation->SetBinContent(i,j, normedContent);
			if (norm > 100. && normedContent > 0.8) {
//				std::cout << "Found Highly Correlated Course Pair: " << courseCorrelation->GetXaxis()->GetBinLabel(i) <<
//					":" << courseCorrelation->GetYaxis()->GetBinLabel(j) << " : " << normedContent << " : " << norm << std::endl;
			}
		}
	}
//	courseCorrelation->GetXaxis()->LabelsOption("a");
//	courseCorrelation->GetXaxis()->SetTicks("-");
//	courseCorrelation->GetYaxis()->LabelsOption("a");
//	courseCorrelation->GetYaxis()->SetTicks("-");
//	courseCorrelation->DrawCopy();
//	correlationCanvas->Modified();
//	correlationCanvas->Update();

	if (gSystem->ProcessEvents()) return;
	
	myBenchmark.Show("HardCourses, Loop 1");
	
	// Now we have map of course deltas set up and enroll status correction calculated
	//  Loop over students again...
//	int iterationsMax = 10;
	myBenchmark.Start("HardCourses, Loop 2");
	TH1D* delGpa1 = new TH1D("delGpa1", "Full Semester GPA - Uncorrected, Unbiased Prediction", 100, -4., 4.);
	TH1D* delGpa2 = new TH1D("delGpa2", "Full Semester GPA - Corrected, Unbiased Prediction", 100, -4., 4.);
	TH1D* delGpa3 = new TH1D("delGpa3", "Semester GPA w/o Course - Corrected, Unbiased Prediction", 100, -4., 4.);
	TH1D* delEntries = new TH1D("delEntries", "Entries in every course delta object", 100, 0., 10000.);
	TH1D* iterChi2 = new TH1D("iterChi2", "Chi^2 vs. Iteration Number", iterationsMax, -0.5, iterationsMax - 0.5);
	TH1D* engl101 = new TH1D("engl101", "Evolution of delta for ENGL101", iterationsMax, -0.5, iterationsMax - 0.5);
	TH1D* math140 = new TH1D("math140", "Evolution of delta for MATH140", iterationsMax, -0.5, iterationsMax - 0.5);
	
	std::map<TString, CourseDelta> oldMap = emptyMap;
	std::map<TString, CourseDelta> newMap = emptyMap;
	TCanvas* canvas2 = new TCanvas("canvas2", "Hard Courses Plots", 1200, 800);
	canvas2->Divide(3,2);
	
	int iterations = 0;
	double epsilon = 0.25;
	
	do {
//		oldMap = epsilon*newMap + (1.-epsilon)*oldMap;    // Damp changes from previous iteration by epsilon
//		oldMap = newMap;
		for (auto& mapElement : oldMap) {
			TString name = mapElement.second.name;
			mapElement.second.name = newMap[name].name;
			mapElement.second.nEntries = newMap[name].nEntries;
			mapElement.second.deltaSum = epsilon*newMap[name].deltaSum + (1.-epsilon)*mapElement.second.deltaSum;
			// What should I do with sum^2?
			mapElement.second.deltaSum2 = newMap[name].deltaSum2;
		}
		newMap = emptyMap;
		delGpa1->Reset();
		delGpa2->Reset();
		delGpa3->Reset();
		delEntries->Reset();
		for (Long64_t jentry = 0; jentry < nentries; jentry++) {
			studentTree->GetEntry(jentry);
			if (student->Id() % 2 != 0) continue;
		
			// Loop over enrollments
			for (auto const& enrollment : student->Enrollments()) {
				if (MyFunctions::termName(enrollment.term) != "Fall" && MyFunctions::termName(enrollment.term) != "Spring") continue;
				double attemptedCredits = student->AttemptedCredits(enrollment.term);
				if (attemptedCredits < 12.) continue;
			
				// Calculate the student's GPA without this semester
				double gpaSemBar = student->Gpa(-enrollment.term);
				// Add correction for enrollment type
				double gpaSemBarCorr = gpaSemBar + enrollTypeCorrection[enrollment.enrollType];
			
				// For plotting, calculate the semester GPA (all courses this semester);
				double gpaSem = student->Gpa(enrollment.term);
			
				delGpa1->Fill(gpaSem - gpaSemBar);
				delGpa2->Fill(gpaSem - gpaSemBarCorr);

				for (auto const& grade : enrollment.grades) {
					// Make sure this is an F-A+ kind of grade
					if (!MyFunctions::ValidGrade(grade.grade)) continue;
					double coeff = (1. - grade.credits/attemptedCredits);
					if (coeff == 0.) {
//						std::cout << "Coeff = 0!" << std::endl;
//						std::cout << "Student ID, term = " << student->Id() << ", " << enrollment.term << std::endl;
//						std::cout << "grade.credits = " << grade.credits << std::endl;
//						std::cout << "attemptedCredits = " << attemptedCredits << std::endl;
						coeff = 0.;  // Gotta love renormalization, lol
					}
					else {
						coeff = 1./coeff;
					}
					// Get GPA for this semester, without this course. Is this right?  Prehaps using the course is ok?
					double gpaSemNoCourse = student->SemesterGpaWithoutCourse(grade.term, grade.course);
					double deltaSem = gpaSemNoCourse - gpaSemBarCorr;
					delGpa3->Fill(deltaSem);
					double sumDelta = 0.;
					double sumCrDelta = 0.;
					for (auto const& grade_i : enrollment.grades) {
						if (grade_i.course == grade.course) continue;
						auto search = oldMap.find(grade_i.course);
						if (search != oldMap.end()) {
							sumDelta += search->second.average();
							sumCrDelta += grade_i.credits*search->second.average();
						}
					}
					// Now calculate difficulty of this course, then add it to the map for this iteration.
					double delta = coeff*(deltaSem - (sumDelta - sumCrDelta/attemptedCredits));
					auto search = newMap.find(grade.course);
					if (search != newMap.end())
						search->second.AddDelta(delta);
				}
			
			}
		}
		// Now compare old and new maps to see if they are still changing...
		double sumChi2 = 0.;
		int nPrinted = 0;
		int nUsed = 0;
		for (auto const& newEntry : newMap) {
			auto const& newElement = newEntry.second;
			delEntries->Fill(newElement.nEntries);
			if (newElement.nEntries >= newElement.minEntries)
				nUsed++;
			auto const& oldElement = oldMap[newElement.name];
			double delta = newElement.average() - oldElement.average();
			double errOnMean = newElement.errorOnMean();
			if (newElement.nEntries > 0) {
				double chi2 = (delta/errOnMean)*(delta/errOnMean);
				sumChi2 += chi2;
				if (chi2 > 200. && nPrinted < 0) {
					nPrinted++;
					std::cout << "In comparing new vs. old, delta, errOnMean, nEntries = " << delta << ", " << errOnMean << ", " << newElement.nEntries << std::endl;
					std::cout << "chi2, sumChi2 = " << chi2 << ", " << sumChi2 << std::endl;
				}
			}
		}
		if (nUsed > 0) sumChi2 /= nUsed;
		std::cout << "Comparision between new and old deltas = " << sumChi2 << std::endl;
		engl101->Fill(iterations, newMap.at("ENGL101").average());
		engl101->SetBinError(iterations+1, newMap.at("ENGL101").errorOnMean());
		math140->Fill(iterations, newMap.at("MATH140").average());
		math140->SetBinError(iterations+1, newMap.at("MATH140").errorOnMean());
		iterChi2->Fill(iterations, sumChi2);
		if (iterations == 0) {
			std::cout << "Number of courses used = " << nUsed << std::endl;
			std::cout << "MATH140 entries = " << newMap.at("MATH140").nEntries << std::endl;
		}		
		//Plot some of this stuff
		canvas2->cd(1);
		delGpa1->DrawCopy();
		canvas2->cd(2);
		delGpa2->DrawCopy();
		canvas2->cd(3);
		delGpa3->DrawCopy();
		canvas2->cd(4);
//		delEntries->DrawCopy();
		iterChi2->DrawCopy("HIST");
		canvas2->cd(5);
		engl101->DrawCopy();
		canvas2->cd(6);
		math140->DrawCopy();
	
		canvas2->Modified();
		canvas2->Update();
		if (gSystem->ProcessEvents()) return;
		
		iterations++;
	
	} while (iterations < iterationsMax);
	
	myBenchmark.Show("HardCourses, Loop 2");
//	return;
	
	myBenchmark.Start("HardCourses, Loop 3");
	
	
/*	
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		gpaHist->Fill(student->Gpa());
		earnedHist->Fill(student->EarnedCredits());
		attemptedHist->Fill(student->AttemptedCredits());
		eVaHist->Fill(student->AttemptedCredits(), student->EarnedCredits());
//		for (Student::Enrollment const & enrollment : student->Enrollments()) {
//			if (MyFunctions::termName(enrollment.term) != "Fall" && MyFunctions::termName(enrollment.term) != "Spring") continue;
//			if (semAttemptedCredits < 12.) continue; 
//			double delGpa = student->Gpa(enrollment.term) - student->Gpa(-(enrollment.term));
//			semGpaHist->Fill(student->Gpa(-(enrollment.term)), student->Gpa(enrollment.term));
//		}
		
		// Loop over all courses...
		for (Student::Grade const & grade : student->Grades()) {
			// Get the number of attempted credits during the term in which they took this course
			double semAttemptedCredits = student->AttemptedCredits(grade.term);
			// Only look at semester where they attempted at least 12 credits
			if (semAttemptedCredits < 12.) continue;
			// Get the gpa in all terms except this one...
			double predictedGPA = student->Gpa(-(grade.term));
			// Apply a correction to the predicted GPA based on student's enrollment type
			double enrollCorrection = gpaPredictionVsStatus->GetBinContent(gpaPredictionVsStatus->GetXaxis()->FindBin(student->EnrollmentType(grade.term)));
			predictedGPA += enrollCorrection;
			// Get the semester gpa for this term, excluding this course...
			double semGPAunbiased = student->SemesterGpaWithoutCourse(grade.term, grade.course);
			double delta = semGPAunbiased - predictedGPA;
			deltaGpaHist->Fill(delta);
			hardCourses[grade.course].name = grade.course;
			hardCourses[grade.course].AddDelta(delta);
		}
		
		if (jentry == 0) {
			// Print some stuff for the first entry...
			std::cout << "Student ID = " << student->Id() << std::endl;
			std::cout << "First Term = " << student->FirstTerm() << std::endl;
			std::cout << "Number of grade records = " << student->Grades().size() << std::endl;
			std::cout << "GPA = " << student->Gpa() << std::endl;
			// Last enrollments should be last in time...
			Student::Enrollment lastEnroll = *(student->Enrollments().rbegin());
			std::cout << "Data end GPA = " << lastEnroll.cumGPA << std::endl;
			std::cout << "Last Enroll Term = " << lastEnroll.term << std::endl;
			std::cout << "Course\t\tCredits\t\tGrade\t\tTerm" << std::endl;
			for (Student::Grade const & grade : student->Grades()) {
//			for (std::size_t i = 0; i < student->Grades().size(); i++) {
//				Student::Grade grade = student->Grades()[i];
				std::cout << grade.course << "\t\t" << grade.credits << "\t\t" << grade.grade << "\t\t" << grade.term << std::endl;
			}
		}
	}
	
	std::cout << "Number of entries in hardCourses map = " << hardCourses.size() << std::endl;
*/
	
	hardCourses = newMap;
	
	TFile* outFile = new TFile("HardCourses.root", "RECREATE");
	TTree* hardTree = new TTree("HardCourses", "Info on course effects on student GPA");
	int nGradesUsed;
	double deltaSum, deltaSum2, average, rms;
	TString courseName;
	hardTree->Branch("Course", &courseName);
	hardTree->Branch("N", &nGradesUsed);
	hardTree->Branch("deltaSum", &deltaSum);
	hardTree->Branch("deltaSum2", &deltaSum2);
	hardTree->Branch("average", &average);
	hardTree->Branch("rms", &rms);
	std::vector<CourseDelta> cutList;
	for (auto const& entry : hardCourses) {
		courseName = entry.first;
		nGradesUsed = entry.second.nEntries;
		deltaSum = entry.second.deltaSum;
		deltaSum2 = entry.second.deltaSum2;
		average = entry.second.average();
		rms = entry.second.rms();
		hardTree->Fill();
		
		// While doing this, lets also glom together prefix codes.
		TString prefix = courseName(0,4);
		hardPrefixes[prefix] += entry.second;
		hardPrefixes[prefix].name = prefix;
		
		
		// Think about whether or not this is too strong of a requirement.
//		std::cout << "significance = " << entry.second.significance() << std::endl;
		if (entry.second.significance() > 2.5) {
//		if (nGradesUsed >= 1000) {
			cutList.push_back(entry.second);
		}
	}
	
	std::cout << "Number of course in cut list = " << cutList.size() << std::endl;
	std::cout << "Number of prefixes in list   = " << hardPrefixes.size() << std::endl;
	std::vector<CourseDelta> prefixList;
	for (auto const& entry : hardPrefixes) {
		if (entry.second.significance() > 2.5) 
//		if (entry.second.nEntries >=1000)
			prefixList.push_back(entry.second);
	}
	std::cout << "Got here 1" << std::endl;
	std::cout << "prefixList size = " << prefixList.size() << std::endl;
	
	struct sort_deltas {
		inline bool operator() (const CourseDelta& lhs, const CourseDelta& rhs) {
			return (lhs.average() < rhs.average());
		}
	};
	
	std::sort(cutList.begin(), cutList.end(), sort_deltas());
	std::sort(prefixList.begin(), prefixList.end(), sort_deltas());
	
	std::cout << "Got here 2" << std::endl;
	
//	for (auto const& entry : cutList) {
//		std::cout << entry.name << "\t\t" << entry.average() << std::endl;
//	}
	
	TH1D* hardestHist = new TH1D("hardestHist", "Top 20 Hardest Courses", 20, 0., 20.);
	TH1D* easiestHist = new TH1D("easiestHist", "Top 20 Easiest Courses", 20, 0., 20.);
//	hardestHist->Sumw2();
	int listSize = cutList.size();
	for (int i = 0; i < std::min(listSize, 20); i++) {
		int j = cutList.size() - i - 1;
		hardestHist->Fill(cutList[i].name, cutList[i].average());
		hardestHist->SetBinError(hardestHist->GetXaxis()->FindFixBin(cutList[i].name), cutList[i].rms()/sqrt(cutList[i].nEntries));
		easiestHist->Fill(cutList[j].name, cutList[j].average());
		easiestHist->SetBinError(easiestHist->GetXaxis()->FindFixBin(cutList[j].name), cutList[j].rms()/sqrt(cutList[j].nEntries));
	}
	std::cout << "Got here 3" << std::endl;
	
	TH1D* hardPrefixHist = new TH1D("hardPrefixHist", "Top 20 Hardest Prefixes", 20, 0., 20.);
	TH1D* easyPrefixHist = new TH1D("easyPrefixHist", "Top 20 Easiest Prefixes", 20, 0., 20.);
//	hardestHist->Sumw2();
	listSize = prefixList.size();
	for (int i = 0; i < std::min(listSize, 20); i++) {
		int j = prefixList.size() - i - 1;
		hardPrefixHist->Fill(prefixList[i].name, prefixList[i].average());
		hardPrefixHist->SetBinError(hardPrefixHist->GetXaxis()->FindFixBin(prefixList[i].name), prefixList[i].rms()/sqrt(prefixList[i].nEntries));
		easyPrefixHist->Fill(prefixList[j].name, prefixList[j].average());
		easyPrefixHist->SetBinError(easyPrefixHist->GetXaxis()->FindFixBin(prefixList[j].name), prefixList[j].rms()/sqrt(prefixList[j].nEntries));
	}
	std::cout << "Got here 4" << std::endl;
	
	hardTree->Print();
	hardTree->Write();
	
	TH1D* degOfDiffHist = new TH1D("degOfDiffHist", "Term Avg Degree of Difficulty", 100, -1., 1.);
	TH1D* delGpaHist = new TH1D("delGpaHist", "Semester Delta GPA", 100, -2., 2.);
	TH1D* delGpaCorrHist = new TH1D("delGpaCorrHist", "Semester Delta GPA, Corrected for Enroll_Type", 100, -2., 2.);
	TH2D* degOfDiffVsDeltaGpaHist = new TH2D("degOfDiffVsDeltaGpaHist", "Semester Avg Degree of Difficulty vs. Actual GPA Difference",
	100, -2., 2., 100, -2., 2.);
	TProfile* degOfDiffProf = new TProfile("degOfDiffProf", "Avg Degree of Difficulty vs. GPA Delta", 100, -2., 2.);
	TH1D* delMinusDodHist = new TH1D("delMinusDodHist", "Semester Delta GPA - Avg Degree of Difficulty", 100, -2., 2.);
	
	TH1D* newDegOfDiffHist = new TH1D("newDegOfDiffHist", "Term Degree of Difficulty, Credit Weighted", 100, -1., 1.);
	TH2D* newDegOfDiffVsDeltaGpaHist = new TH2D("newDegOfDiffVsDeltaGpaHist", "Semester Degree of Difficulty (credit weighted) vs. Actual GPA Difference",
	100, -2., 2., 100, -2., 2.);
	TH1D* newDelMinusDodHist = new TH1D("newDelMinusDodHist", "Semester Delta GPA - Degree of Difficulty (Credit Weighted)", 100, -2., 2.);
	
	//  Look at semester difficulty and compare that to predicted effect on GPA...
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		if (student->Id() % 2 == 0) continue;

		for (Student::Enrollment const& enrollment : student->Enrollments()) {
			int term = enrollment.term;
			if (MyFunctions::termName(term) != "Fall" && MyFunctions::termName(term) != "Spring") continue;
			if (student->AttemptedCredits(term) < 12.) continue; 
			std::vector<Student::Grade> termGrades = student->TermGradeList(enrollment.term);
			double degreeOfDifficulty = 0.;
			double sigDegOfDiff = 0.;
			double totalCredits = 0.;
			double sumCrDeltas = 0.;
			for (auto const& grade_i : termGrades) {
				totalCredits += grade_i.credits;
				double sumDeltas = 0.;
				for (auto const& grade_j : termGrades) {
					if (grade_i.course != grade_j.course) {
						auto search = hardCourses.find(grade_j.course);
						if (search != hardCourses.end())
							sumDeltas += search->second.average();
					}
				}
				sumCrDeltas += grade_i.credits*sumDeltas;
				
				
//				if (hardCourses[grade.course].errorOnMean() > 0.) {
//					double weight = 1./(hardCourses[grade.course].errorOnMean()*hardCourses[grade.course].errorOnMean());
//					degreeOfDifficulty += weight*hardCourses[grade.course].average();
//					sigDegOfDiff += weight;
//				}
			}
//			if (sigDegOfDiff > 0.)
//				degreeOfDifficulty = degreeOfDifficulty/sigDegOfDiff;
//			else
//				degreeOfDifficulty = 0.;
			degreeOfDifficulty = sumCrDeltas/totalCredits;
			degOfDiffHist->Fill(degreeOfDifficulty);
			sigDegOfDiff = sqrt(sigDegOfDiff);
			double deltaGpa = student->Gpa(enrollment.term) - student->Gpa(-enrollment.term);
			delGpaHist->Fill(deltaGpa);
			// Apply a correction to the predicted GPA based on student's enrollment type
			double enrollCorrection = gpaPredictionVsStatus->GetBinContent(gpaPredictionVsStatus->GetXaxis()->FindBin(student->EnrollmentType(term)));
			deltaGpa -= enrollCorrection;
			delGpaCorrHist->Fill(deltaGpa);
			delMinusDodHist->Fill(deltaGpa - degreeOfDifficulty);
			degOfDiffVsDeltaGpaHist->Fill(deltaGpa, degreeOfDifficulty);
			degOfDiffProf->Fill(deltaGpa, degreeOfDifficulty);
			
			// Try a different degree of difficulty calculation that should be net change in GPA
			double sumCredits = 0.;
			double sumAllDeltas = 0.;
			for (Student::Grade const& grade : termGrades) {
				auto search = hardCourses.find(grade.course);
				if (search != hardCourses.end())
					sumAllDeltas += search->second.average();
			}
			double numerator = 0.;
			for (Student::Grade const& grade : termGrades) {
				sumCredits += grade.credits;
				auto search = hardCourses.find(grade.course);
				if (search != hardCourses.end())
					numerator += grade.credits*(sumAllDeltas - search->second.average());
			}
			double newDegOfDiff = 0.;
			if (sumCredits > 0.) newDegOfDiff = numerator/sumCredits;
			newDegOfDiffHist->Fill(newDegOfDiff);
			newDegOfDiffVsDeltaGpaHist->Fill(deltaGpa, newDegOfDiff);
			newDelMinusDodHist->Fill(deltaGpa - newDegOfDiff);
			
			// Look at some wierd cases...
			/*
			if (degreeOfDifficulty > 2.) {
				std::cout << "Found a case with a very high DegreeOfDifficulty" << std::endl;
				std::cout << "Looking at term " << term << std::endl;
				// Print some stuff for the first entry...
				std::cout << "Student ID = " << student->Id() << std::endl;
				std::cout << "First Term = " << student->FirstTerm() << std::endl;
				std::cout << "Number of grade records = " << student->Grades().size() << std::endl;
				std::cout << "GPA               = " << student->Gpa() << std::endl;
				std::cout << "GPA this term     = " << student->Gpa(term) << std::endl;
				std::cout << "GPA w/o this term = " << student->Gpa(-term) << std::endl;
				// Last enrollments should be last in time...
				Student::Enrollment lastEnroll = *(student->Enrollments().rbegin());
				std::cout << "Data end GPA = " << lastEnroll.cumGPA << std::endl;
				std::cout << "Last Enroll Term = " << lastEnroll.term << std::endl;
				std::cout << "Course\t\tCredits\t\tGrade\t\tTerm" << std::endl;
				for (Student::Grade const & grade : student->Grades()) {
	//			for (std::size_t i = 0; i < student->Grades().size(); i++) {
	//				Student::Grade grade = student->Grades()[i];
					std::cout << grade.course << "\t\t" << grade.credits << "\t\t" << grade.grade << "\t\t" << grade.term << std::endl;
				}	
			}
			*/
		}
	}

	myBenchmark.Show("Hard Courses, Loop 3");
	
	TCanvas* c1 = new TCanvas();
	c1->Divide(3,3);
	c1->cd(1);
	gpaHist->DrawCopy();
	c1->cd(2);
	earnedHist->DrawCopy();
	c1->cd(3);
	attemptedHist->DrawCopy();
	c1->cd(4);
	eVaHist->DrawCopy();
	c1->cd(5);
	gpaPredictionVsStatus->DrawCopy();
	c1->cd(6);
	enrollStatusHist->DrawCopy();
	
	TCanvas* c2 = new TCanvas();
	c2->Divide(2,2);
	c2->cd(1);
	semGpaHist->DrawCopy("CANDLE");
	c2->cd(2);
	c2->cd(3);
	deltaGpaHist->DrawCopy();
	c2->cd(4);

	TCanvas* c3 = new TCanvas();
	c3->Divide(1,2);
	c3->cd(1);
	hardestHist->DrawCopy();
	c3->cd(2);
	easiestHist->DrawCopy();
	
	TCanvas* c4 = new TCanvas();
	c4->Divide(1,2);
	c4->cd(1);
	hardPrefixHist->DrawCopy();
	c4->cd(2);
	easyPrefixHist->DrawCopy();
	
	TCanvas* c5 = new TCanvas();
	c5->Divide(3,3);
	c5->cd(1);
	degOfDiffHist->DrawCopy();
	c5->cd(2);
	delGpaHist->DrawCopy();
	c5->cd(3);
	delGpaCorrHist->DrawCopy();
	c5->cd(4);
	degOfDiffVsDeltaGpaHist->DrawCopy();
	c5->cd(5);
	degOfDiffProf->DrawCopy();
	c5->cd(6);
	delMinusDodHist->DrawCopy();
	c5->cd(7);
	newDegOfDiffHist->DrawCopy();
	c5->cd(8);
	newDegOfDiffVsDeltaGpaHist->DrawCopy();
	c5->cd(9);
	newDelMinusDodHist->DrawCopy();
	
	hardTree->Write();
	gpaHist->Write();
	earnedHist->Write();
	attemptedHist->Write();
	eVaHist->Write();
	semGpaHist->Write();
	deltaGpaHist->Write();
	hardestHist->Write();
	easiestHist->Write();
	hardPrefixHist->Write();
	easyPrefixHist->Write();
	
	
	outFile->Close();
	f->Close();
	
}