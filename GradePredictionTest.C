// My includes
#include "Student.h"

#include "CumulativeDistribution.h"
#include "MyFunctions.h"

// ROOT includes
#include <TArrow.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLine.h>
#include <TMarker.h>
#include <TSystem.h>
#include <TTree.h>
// Other includes
#include <iostream>
#include <vector>

void BasicTest(int entry = 0) {
	
	std::cout << "About to run some basic tests..." << std::endl;
	MyFunctions::BuildGradeNormMap();
	
	TFile* studentFile = new TFile("Students.root");
	TTree* studentTree = (TTree*)studentFile->Get("Students");
	Student* student = 0;
	studentTree->SetBranchAddress("student", &student);
	
	if (entry >= studentTree->GetEntriesFast() || entry < 0) {
		std::cout << "Invalid entry number!" << std::endl;
		return;
	}
	
	// Look at this student
	studentTree->GetEntry(entry);
	student->Finalize();
	
	TCanvas* c1 = new TCanvas("c1","Basic Tests", 1600, 1200);
	c1->Divide(3,2);
	for (int i = 1; i <=9; ++i)
		c1->cd(i)->Divide(2,2);
	
	// Look at one term
	Student::Enrollment enrollment = student->Enrollments().at(0);
	std::vector<Student::Grade> gradeList;
	int iPad  = 0;
	std::cout << "Course\tGrade\tCredits\tTerm" << std::endl;
	for (Student::Grade const& grade : enrollment.grades) {
		++iPad;
		std::cout << grade.course << "\t" << grade.grade << "\t" << grade.credits << "\t" << grade.term <<std::endl;
		gradeList.push_back(grade);
		CourseGradeNormer cgn = MyFunctions::gradeNormMap.at(std::make_pair(grade.course, 0));
		c1->cd(iPad)->cd(1);
		cgn.CumulativeGraph()->Draw("AP");
		c1->cd(iPad)->cd(2);
		cgn.CumulativeGraphInverse()->Draw("AP");
		c1->cd(iPad)->cd(3);
		CumulativeDistribution cdf = student->CombinedCdf(gradeList);
		TGraph* cdfClone = (TGraph*)cdf.Clone();
		cdfClone->Draw("AP");
		c1->cd(iPad)->cd(4);
		CumulativeDistributionInverse inv = student->CombinedCdfInv(gradeList);
		TGraph* invClone = (TGraph*)inv.Clone();
		invClone->Draw("AP");
	}
	
	// Invertability test of single CDF.  Passed!
	bool first = true;
	for (int jentry = 0; jentry < studentTree->GetEntriesFast(); ++jentry) {
		studentTree->GetEntry(entry);
		student->Finalize();
		
		for (Student::Grade const& grade : student->Grades()) {
			if (!MyFunctions::ValidGrade(grade.grade)) continue;
			if (!MyFunctions::regularSemester(grade.term)) continue;
			double qualityIn = MyFunctions::GradeToQuality(grade.grade);
			CourseGradeNormer cgn = MyFunctions::gradeNormMap.at(std::make_pair(grade.course, 0));
			double qualityOut = cgn.CumulativeGraphInverse()->Evaluate(cgn.CumulativeGraph()->Evaluate(qualityIn));
			// Test equality double?
			if (qualityIn != qualityOut) {
				if (first) {
					first = false;
					std::cout << "Failed Inversion: In = " << qualityIn << ", Out = " << qualityOut << std::endl;
					std::cout << "CDF Evaluate = " << cgn.CumulativeGraph()->Evaluate(qualityIn) << std::endl;
					std::cout << "CDF:" << std::endl;
					cgn.CumulativeGraph()->Print();
					std::cout << "CDF-1:" << std::endl;
					cgn.CumulativeGraphInverse()->Print();
				}
			}
		}
	}
	if (first)
		std::cout << "Passed Invertability Test!!!" << std::endl;
}

void SingleStudent(int studentEntry, int studentTerm) {
	// This macro just tests various ways of predicting a student's GPA in a semester.
	
	std::cout << "About to build GradeNormMap..." << std::endl;
	MyFunctions::BuildGradeNormMap();
	
	// Open File with Student objects.  This is currently created in "HardCourse.C".  Should probably check that it exists?
	
	TFile* studentFile = new TFile("Students.root");
	TTree* studentTree = (TTree*)studentFile->Get("Students");
	Student* student = 0;
	studentTree->SetBranchAddress("student", &student);
	
	TCanvas* cStudent = new TCanvas("cStudent", "Single Student, One Semester Prediction", 1600, 1200);
	cStudent->Divide(2,2);
//	int studentEntry = 1;
//	int studentTerm = 4;

	studentTree->GetEntry(studentEntry);
	student->Finalize();  // This populates the enrollment.grades stuff which is just references
	
	int enrollCount = 0;
	for (auto const& enrollment : student->Enrollments()) {
		++enrollCount;
		
		int term = enrollment.term;

		// Only look at Fall and Spring (regular) semesters
		if (!MyFunctions::regularSemester(term)) continue;
		
		// Only look at semesters where student has attempted at least 12 grade credits
		double attemptedCredits = student->AttemptedCredits(enrollment);
		if (attemptedCredits < 12.) continue;

		std::cout << "Term = " << term << std::endl;
		for (Student::Grade const& grade : enrollment.grades) {
			std::cout << grade.course << "\t" << grade.grade << "\t" << grade.credits << "\t" << std::endl;
		}

		if (enrollCount != studentTerm) continue; 
		
		// Gpa, this term only.  This is what we are trying to predict	
		double gpaSemRaw = student->Gpa(enrollment.term);
	
		std::vector<Student::Grade> gradesThisSemester = student->TermLetterGradeList(term);
		std::vector<Student::Grade> gradesThisSemesterBar = student->TermLetterGradeList(-term);
		double gpaSemBar = student->Gpa(gradesThisSemesterBar);

		CumulativeDistribution cdfSemBar = student->CombinedCdf(gradesThisSemesterBar);		
		double pctSemBar = cdfSemBar.Evaluate(gpaSemBar);
		CumulativeDistributionInverse cdfInvSem = student->CombinedCdfInv(gradesThisSemester);
		double gpaSemPredictionCdf = cdfInvSem.Evaluate(pctSemBar);
		
		std::cout << "GPA(semester)  = " << gpaSemRaw << std::endl;
		std::cout << "GPA(semBar)    = " << gpaSemBar << std::endl;
		std::cout << "GPA(predicted) = " << gpaSemPredictionCdf << std::endl;
		std::cout << "GPA(overall)   = " << student->Gpa() << std::endl;
				
		cStudent->cd(1);
		TGraph* graph = (TGraph*)student->CombinedCdfInv(gradesThisSemesterBar).Clone();
		graph->Draw("AP");
		cStudent->cd(2);
		graph = (TGraph*)cdfSemBar.Clone();
		graph->Draw("AP");
		TArrow arrow(gpaSemBar, graph->GetYaxis()->GetXmin(), gpaSemBar, pctSemBar, 0.005, "|>");
		arrow.SetLineColor(kBlue);
		arrow.SetLineWidth(2);
		arrow.SetAngle(40);
		TArrow* temp = (TArrow*)arrow.Clone();
		temp->Draw();
		temp->DrawArrow(gpaSemBar, pctSemBar, graph->GetXaxis()->GetXmin(), pctSemBar, 0.005, "|>");
		cStudent->cd(3);
		graph = (TGraph*)cdfInvSem.Clone();
		graph->Draw("AP");
		temp->DrawArrow(pctSemBar, graph->GetYaxis()->GetXmin(), pctSemBar, gpaSemPredictionCdf, 0.005, "|>");
		temp->DrawArrow(pctSemBar, gpaSemPredictionCdf, graph->GetXaxis()->GetXmin(), gpaSemPredictionCdf, 0.005, "|>");
		temp->SetLineColor(kRed);
		temp->DrawArrow(graph->GetXaxis()->GetXmin() + 0.2, gpaSemRaw, graph->GetXaxis()->GetXmin(), gpaSemRaw, 0.005, "|>");
		cStudent->cd(4);
		graph = (TGraph*)student->CombinedCdf(gradesThisSemester).Clone();
		graph->Draw("AP");
	}
	
	
	
	
}
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
	
	TH1D* hAllGpa = new TH1D("hAllGpa", "All Student GPAs", 100, 0., 4.3);
	TH1* hAllGpaCdf = 0;
	TH1D* hDiffRank = new TH1D("hDiffRank", "#Delta_{Rank}, CDF - RAW", 100, -.5, .5);
	TH2D* hDiffRank2D = new TH2D("hDiffRank2D", "Rank_{CDF} vs. Rank_{Raw}", 100, 0., 1., 100, 0., 1.);
	TH1D* hDiffGpaRaw = new TH1D("hDiffGpaRaw", "#Delta between using Eval pct and and GPA", 100, -4., 4.);
	TH1D* hDiffGpaCdf = new TH1D("hDiffGpaCdf", "#Delta between using Cdf pct and and GPA", 100, -0.2, 0.2);
	
	
	TH1D* hDeltaGpaBiased = new TH1D("hDeltaGpaBiase", "Biased, Raw #DeltaGPA", 100, -4., 4.);
	TH1D* hDeltaGpaRaw = new TH1D("hDeltaGpaRaw", "Unbiased, Raw #DeltaGPA", 100, -4., 4.);
	TH1D* hDeltaGpaNormed = new TH1D("hDeltaGpaNormed", "Course (Mean, #sigma) based grade normalization #DeltaGPA", 100, -4., 4.);
	TH1D* hDeltaGpaCdf = new TH1D("hDeltaGpaCdf", "Grade CDF based #DeltaGPA", 100, -4., 4.);
	
	TH1D* hDeltaCourseBiased = new TH1D("hDeltaCourseBiased", "Single Course Prediction, Biased", 100, -4., 4.);
	TH1D* hDeltaCourseRaw = new TH1D("hDeltaCourseRaw", "Single Course Prediction, Unbiased", 100, -4., 4.);
	TH1D* hDeltaCourseNormed = new TH1D("hDeltaCourseNormed", "Single Course Prediction, Normed", 100, -4., 4.);
	TH1D* hDeltaCourseCdf = new TH1D("hDeltaCourseCdf", "Single Course Prediction, CDF", 100, -4., 4.);
	
	long nentries = studentTree->GetEntriesFast();
	std::cout << "Look at Semester Grade Predictions using " << nentries << " students." << std::endl;
	
	// First, do one loop over students to get their overall GPA distribution
	for (long jentry = 0; jentry < nentries; ++jentry) {
		studentTree->GetEntry(jentry);
		student->Finalize();
		hAllGpa->Fill(student->Gpa());
	}
	hAllGpaCdf = hAllGpa->GetCumulative();
	hAllGpaCdf->Scale(1./hAllGpa->Integral());
	
	TCanvas* cGpa = new TCanvas("cGpa", "Testing overall GPA stuff", 1600, 1200);
	cGpa->Divide(3,3);
	cGpa->cd(1);
	hAllGpa->DrawCopy();
	cGpa->cd(2);
	hAllGpaCdf->DrawCopy();
	
	TCanvas* c0 = new TCanvas("cGradePrediction_0", "A Few Student Cumulative Grade Distribution Inverses", 1600, 1200);
	c0->Divide(3,3);
	for (int i = 1; i <=9; ++i)
		c0->cd(i)->Divide(1,2);
		
	TCanvas* cStudent = new TCanvas("cStudent", "Single Student, One Semester Prediction", 1600, 1200);
	cStudent->Divide(2,2);
	int studentEntry = 1;
	int studentTerm = 4;
	
	int iPad = 0;
	char* s = new char[1];
	
	for (long jentry = 0; jentry < nentries; jentry++) {
		if (jentry % 1000 == 0)
			std::cout << "Looking at student " << jentry << std::endl;
		
		studentTree->GetEntry(jentry);
		student->Finalize();  // This populates the enrollment.grades stuff which is just references
		
		double gpaFullRaw = student->Gpa();  // Standard GPA, all semesters
		// Percent rank based on raw, overall GPA
		double pctRankGpaRaw = hAllGpaCdf->Interpolate(gpaFullRaw);
		
		// This is % rank based more on the student's actuall course load.  These two should be comparable?
		CumulativeDistribution fullCdf = student->CombinedCdf();
		double pctRankGpaCdf = fullCdf.Evaluate(gpaFullRaw);
		hDiffRank->Fill(pctRankGpaCdf - pctRankGpaRaw);
		hDiffRank2D->Fill(pctRankGpaRaw, pctRankGpaCdf);
		
		// Some predicted GPA values
		CumulativeDistributionInverse fullCdfInv = student->CombinedCdfInv();
		double predGpaEval = fullCdfInv.Evaluate(pctRankGpaRaw);
		double predGpaCdf = fullCdfInv.Evaluate(pctRankGpaCdf);
		hDiffGpaRaw->Fill(predGpaEval - gpaFullRaw);
		hDiffGpaCdf->Fill(predGpaCdf - gpaFullRaw);
		
		// Try to 
		// Draw the first few distributions.
		if (iPad < 9) {
			iPad++;
			c0->cd(iPad)->cd(1);
			TGraph* graph = (TGraph*)fullCdf.Clone();
			graph->Draw("AP");
			c0->cd(iPad)->cd(2);
			graph = (TGraph*)fullCdfInv.Clone();
			graph->Draw("AP");
		}
		
		int enrollCount = 0;
		// Loop over semesters and calculate different ways of predicting the semester's GPA
		for (auto const& enrollment : student->Enrollments()) {
			++enrollCount;
			int term = enrollment.term;
			// Only look at Fall and Spring (regular) semesters
			if (!MyFunctions::regularSemester(term)) continue;
			
			// Only look at semesters where student has attempted at least 12 grade credits
			double attemptedCredits = student->AttemptedCredits(enrollment);
			if (attemptedCredits < 12.) continue;
			
			// Gpa, this term only.  This is what we are trying to predict	
			double gpaSemRaw = student->Gpa(enrollment.term);
			
			// Standard GPA without this term
			double gpaSemBarRaw = student->TermGpaPrediction(term, Student::RAW);
			
			// Semester prediction based on simple course (mean, sigma) normalizing
			double gpaSemPredictionNormed = student->TermGpaPrediction(term, Student::NORMED);
			
			// Use Cumulative Distribution Function methed...
			double gpaSemPredictionCdf = student->TermGpaPrediction(term, Student::DISTRIBUTION);
			
			hDeltaGpaBiased->Fill(gpaSemRaw - gpaFullRaw);  // Biased residual
			hDeltaGpaRaw->Fill(gpaSemRaw - gpaSemBarRaw);   // Unbiased residual
			hDeltaGpaNormed->Fill(gpaSemRaw - gpaSemPredictionNormed);
			hDeltaGpaCdf->Fill(gpaSemRaw - gpaSemPredictionCdf);
						
			if (jentry == studentEntry && enrollCount == studentTerm) {
				Student::GradeVector gradesThisSemester = student->TermLetterGradeList(term);
				Student::GradeVector gradesThisSemesterBar = student->TermLetterGradeList(-term);
				CumulativeDistribution cdfSemBar = student->CombinedCdf(gradesThisSemesterBar);
				double gpaSemBar = student->Gpa(gradesThisSemesterBar);
				double pctSemBar = cdfSemBar.Evaluate(gpaSemBar);
				CumulativeDistributionInverse cdfInvSem = student->CombinedCdfInv(gradesThisSemester);

				cStudent->cd(1);				
				TGraph* graph = (TGraph*)student->CombinedCdfInv(gradesThisSemesterBar).Clone();
				for (auto grade : gradesThisSemesterBar) {
					std::cout << "Grade: " << grade.course << "\t" << grade.grade << "\t" << grade.term << "\t" << grade.credits << std::endl;
				}
				graph->Draw("AP");
				cStudent->cd(2);
				graph = (TGraph*)cdfSemBar.Clone();
				graph->Draw("AP");
				TArrow arrow(gpaSemBarRaw, 0., gpaSemBarRaw, pctSemBar, 0.005, "|>");
				arrow.SetLineColor(kBlue);
				arrow.SetLineWidth(2);
				arrow.SetAngle(40);
				TArrow* temp = (TArrow*)arrow.Clone();
				temp->Draw();
				temp->DrawArrow(gpaSemBarRaw, pctSemBar, 0., pctSemBar, 0.005, "|>");
				cStudent->cd(3);
				graph = (TGraph*)cdfInvSem.Clone();
				graph->Draw("AP");
				temp->DrawArrow(pctSemBar, 0., pctSemBar, gpaSemPredictionCdf, 0.005, "|>");
				temp->DrawArrow(pctSemBar, gpaSemPredictionCdf, 0., gpaSemPredictionCdf, 0.005, "|>");
				temp->SetLineColor(kRed);
				temp->DrawArrow(0.2, gpaSemRaw, 0., gpaSemRaw, 0.005, "|>");
				cStudent->cd(4);
				graph = (TGraph*)student->CombinedCdf(gradesThisSemester).Clone();
				graph->Draw("AP");
			}
			
			// Now look at some single course predictive measures
			for (Student::Grade const& grade : enrollment.grades) {
				if (!MyFunctions::ValidGrade(grade.grade)) continue;
	
				if (jentry == 0) {
					std::cout << "Student 0, Course = " << grade.course << ", term = " << grade.term << std::endl;
				}
				double thisQuality = MyFunctions::GradeToQuality(grade.grade);
				
				// Standard cumulative GPA without this course
				double gpaCourseBar = student->CourseGradePrediction(grade, Student::RAW);
				
				// Single course prediction based on simple course normalizing
				double qualityPredictionNormed = student->CourseGradePrediction(grade, Student::NORMED);

				// Try CDF prediction:
				double qualityPredictionCdf = student->CourseGradePrediction(grade, Student::DISTRIBUTION);
				
				hDeltaCourseBiased->Fill(thisQuality - gpaFullRaw);
				hDeltaCourseRaw->Fill(thisQuality - gpaCourseBar);
				hDeltaCourseNormed->Fill(thisQuality - qualityPredictionNormed);
				hDeltaCourseCdf->Fill(thisQuality - qualityPredictionCdf);
			}
			
		}
	}
	
	// Plot some results
	
	cGpa->cd(3);
	hDiffRank->DrawCopy();
	cGpa->cd(4);
	hDiffRank2D->DrawCopy();
	cGpa->cd(5);
	hDiffGpaRaw->DrawCopy();
	cGpa->cd(6);
	hDiffGpaCdf->DrawCopy();
	
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
	c1->cd(4);
	hDeltaGpaCdf->Fit("gaus");
	hDeltaGpaCdf->DrawCopy();
	
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
	c2->cd(4);
	hDeltaCourseCdf->Fit("gaus");
	hDeltaCourseCdf->DrawCopy();
	
	outFile->Write();
	outFile->Close();
	studentFile->Close();
	
	return;
}
