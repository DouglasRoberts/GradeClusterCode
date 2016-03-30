// My includes
#include "Student.h"
#include "MyFunctions.h"
// ROOT includes
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
// Other includes
#include <iostream>

void GradePredictionTest() {
	// This macro just tests various ways of predicting a student's GPA in a semester.
	
	std::cout << "About to build GradeNormMap..." << std::endl;
	MyFunctions::BuildGradeNormMap();
	
	// Open File with Student objects.  This is currently created in "HardCourse.C".  Should probably check that it exists?
	
	TFile* studentFile = new TFile("Students.root");
	TTree* studentTree = (TTree*)studentFile->Get("Students");
	Student* student = 0;
	studentTree->SetBranchAddress("student", &student);
	
	// Book some histograms
	TFile* outFile = new TFile("GradePredictions.root", "RECREATE");
	TH1D* hDeltaGpaBiased = new TH1D("hDeltaGpaBiase", "Biased, Raw #DeltaGPA", 100, -4., 4.);
	TH1D* hDeltaGpaRaw = new TH1D("hDeltaGpaRaw", "Unbiased, Raw #DeltaGPA", 100, -4., 4.);
	TH1D* hDeltaGpaNormed = new TH1D("hDeltaGpaNormed", "Course (Mean, #sigma) based grade normalization #DeltaGPA", 100, -4., 4.);
	
	TH1D* hDeltaCourseBiased = new TH1D("hDeltaCourseBiased", "Single Course Prediction, Biased", 100, -4., 4.);
	TH1D* hDeltaCourseRaw = new TH1D("hDeltaCourseRaw", "Single Course Prediction, Unbiased", 100, -4., 4.);
	TH1D* hDeltaCourseNormed = new TH1D("hDeltaCourseNormed", "Single Course Prediction, Normed", 100, -4., 4.);
	
	long nentries = studentTree->GetEntriesFast();
	std::cout << "Look at Semester Grade Predictions using " << nentries << " students." << std::endl;
	for (long jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		student->Finalize();  // This populates the enrollment.grades stuff which is just references
		
		double gpaFullRaw = student->Gpa();  // Standard GPA, all semesters
		
		// Loop over semesters and calculate different ways of predicting the semester's GPA
		for (auto const& enrollment : student->Enrollments()) {
			int term = enrollment.term;
			// Only look at Fall and Spring (regular) semesters
			if (!MyFunctions::regularSemester(term)) continue;
			
			// Only look at semesters where student has attempted at least 12 grade credits
			double attemptedCredits = student->AttemptedCredits(enrollment);
			if (attemptedCredits < 12.) continue;
			
			// Gpa, this term only.  This is what we are trying to predict	
			double gpaSemRaw = student->Gpa(enrollment.term);
			
			// Standard GPA without this term
			double gpaSemBarRaw = student->Gpa(-enrollment.term);
			
			// Semester prediction based on simple course (mean, sigma) normalizing
			double gpaSemPredictionNormed = student->NormedGpaPrediction(term);

			hDeltaGpaBiased->Fill(gpaSemRaw - gpaFullRaw);  // Biased residual
			hDeltaGpaRaw->Fill(gpaSemRaw - gpaSemBarRaw);   // Unbiased residual
			hDeltaGpaNormed->Fill(gpaSemRaw - gpaSemPredictionNormed);  
			
			// Now look at seom single course predictive measures
			for (Student::Grade const& grade : enrollment.grades) {
				if (!MyFunctions::ValidGrade(grade.grade)) continue;
				double thisQuality = MyFunctions::GradeToQuality(grade.grade);
				
				// Standard cumulative GPA without this course
				double gpaCourseBar = student->CumGpaWithoutCourse(grade.term, grade.course);
				
				// Single course prediction based on simple course normalizing
				double qualityPredictionNormed = student->NormedCoursePrediction(&grade);
				std::cout << "Normed course prection: thisCourse, prediction = " << thisQuality << ", " << qualityPredictionNormed << std::endl;
				
				hDeltaCourseBiased->Fill(thisQuality - gpaFullRaw);
				hDeltaCourseRaw->Fill(thisQuality - gpaCourseBar);
				hDeltaCourseNormed->Fill(thisQuality - qualityPredictionNormed);
			}
		}
	}
	
	// Plot some results
	TCanvas* c1 = new TCanvas("cGradePrediction1", "Predicting Semester GPA", 1600, 1200);
	c1->Divide(2,2);
	c1->cd(1);
	hDeltaGpaBiased->Fit("gaus");
	hDeltaGpaBiased->DrawCopy();
	c1->cd(2);
	hDeltaGpaRaw->Fit("gaus");
	hDeltaGpaRaw->DrawCopy();
	c1->cd(3);
	hDeltaGpaNormed->Fit("gaus");
	hDeltaGpaNormed->DrawCopy();
	
	TCanvas* c2 = new TCanvas("cGradePrediction2", "Predicting Single Course Grade", 1600, 1200);
	c2->Divide(2,2);
	c2->cd(1);
	hDeltaCourseBiased->Fit("gaus");
	hDeltaCourseBiased->DrawCopy();
	c2->cd(2);
	hDeltaCourseRaw->Fit("gaus");
	hDeltaCourseRaw->DrawCopy();
	c2->cd(3);
	hDeltaCourseNormed->Fit("gaus");
	hDeltaCourseNormed->DrawCopy();
	
	outFile->Write();
	outFile->Close();
	studentFile->Close();
	
	return;
}