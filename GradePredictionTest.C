// My includes
#include "Student.h"
#include "MyFunctions.h"
// ROOT includes
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLine.h>
#include <TMarker.h>
#include <TSystem.h>
#include <TTree.h>
// Other includes
#include <iostream>

void GradePredictionTest(bool scan = false) {
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
	TH1D* hDiffRank = new TH1D("hDiffRank", "#Delta_{Rank}, CDF - RAW", 100, -.02, .02);
	TH2D* hDiffRank2D = new TH2D("hDiffRank2D", "Rank_{CDF} vs. Rank_{Raw}", 100, 0., 1., 100, 0., 1.);
	TH1D* hDiffGpaRaw = new TH1D("hDiffGpaRaw", "#Delta between using Eval pct and and GPA", 100, -.02, .02);
	TH1D* hDiffGpaCdf = new TH1D("hDiffGpaCdf", "#Delta between using Cdf pct and and GPA", 100, -.02, .02);
	
	
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
	TCanvas* c4 = new TCanvas("cGradePrediction_4", "Student Sem and Sem Bar CDFs", 1600, 1200);
	TCanvas* c5 = new TCanvas("cGradePrediction_5", "Student Course and Course Bar CDFs", 1600, 1200);
	c0->Divide(3,3);
	for (int i = 1; i <=9; ++i)
		c0->cd(i)->Divide(1,2);
		
	c4->Divide(1,2);
	c5->Divide(1,2);
	int iPad = 0;
	char* s = new char[1];
	
	for (long jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		student->Finalize();  // This populates the enrollment.grades stuff which is just references
		
		double gpaFullRaw = student->Gpa();  // Standard GPA, all semesters
		double pctRankGpaRaw = hAllGpaCdf->Interpolate(gpaFullRaw);
		// This is % rank based more on the sutdent's actuall course load.  These two should be comparable?
		double pctRankGpaCdf = MyFunctions::EvalCdf(student->CombinedCdf(), gpaFullRaw);
		double pctRankGpaEval = student->CombinedCdf()->Eval(gpaFullRaw);
		hDiffRank->Fill(pctRankGpaCdf - pctRankGpaEval);
		hDiffRank2D->Fill(pctRankGpaRaw, pctRankGpaCdf);
		
		double predGpaEval = student->CombinedCdfInv()->Eval(pctRankGpaEval);
//		double predGpaEval = MyFunctions::EvalInvCdf(student->CombinedCdfInv(), pctRankGpaRaw);
		double predGpaCdf = MyFunctions::EvalInvCdf(student->CombinedCdfInv(), pctRankGpaCdf);
		hDiffGpaRaw->Fill(predGpaEval - gpaFullRaw);
		hDiffGpaCdf->Fill(predGpaCdf - gpaFullRaw);
		
		// Try to 
		// Draw the first few distributions.
		if (iPad < 9) {
			iPad++;
			c0->cd(iPad)->cd(1);
			student->CombinedCdf()->Draw("AP");
			c0->cd(iPad)->cd(2);
			student->CombinedCdfInv()->Draw("AP");
		}
		
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
			
			// Use Cumulative Distribution Function methed...
			std::vector<Student::Grade> gradesThisSemester = student->TermLetterGradeList(term);
			std::vector<Student::Grade> gradesThisSemesterBar = student->TermLetterGradeList(-term);
			TGraph* cdfSemBar = student->CombinedCdf(gradesThisSemesterBar);
			double gpaSemBar = student->Gpa(gradesThisSemesterBar);
			// Instead of Eval, find first point that is greater than pct.  Then value is pct from previous point
			double pctSemBar = cdfSemBar->Eval(gpaSemBar);  // This is the students expected percent ranking, based on grades not including this semester
//			double pctSemBar = MyFunctions::EvalCdf(cdfSemBar, gpaSemBar);
			TGraph* cdfInvSem = student->CombinedCdfInv(gradesThisSemester);
//			double gpaSemPredictionCdf = MyFunctions::EvalInvCdf(cdfInvSem, pctSemBar);
			double gpaSemPredictionCdf = cdfInvSem->Eval(pctSemBar);
			if (scan) {
				c4->cd(1);
				cdfSemBar->Draw("AP");
				TMarker m(gpaSemBar, pctSemBar, 29);
				m.Draw();
				TLine l(gpaSemBar, 0., gpaSemBar, pctSemBar);
				l.Draw();
				l.DrawLine(0., pctSemBar, gpaSemBar, pctSemBar);
				c4->cd(2);
				cdfInvSem->Draw("AP");
				m.DrawMarker(pctSemBar, gpaSemPredictionCdf);
				l.DrawLine(0., gpaSemRaw, 1., gpaSemRaw);
				std::cout << "gpaSmeBar = " << gpaSemBar << ", pctSemBar = " << pctSemBar << ", gpaSemPred = " << gpaSemPredictionCdf << ", gpaSem = " << gpaSemRaw << std::endl;
				c4->Modified();
				c4->Update();
				std::cout << "Next (q to quit):";
				if (gSystem->ProcessEvents()) return;
				gets(s);
				if (s[0] == 'q') return;
			}
			
			hDeltaGpaBiased->Fill(gpaSemRaw - gpaFullRaw);  // Biased residual
			hDeltaGpaRaw->Fill(gpaSemRaw - gpaSemBarRaw);   // Unbiased residual
			hDeltaGpaNormed->Fill(gpaSemRaw - gpaSemPredictionNormed);
			hDeltaGpaCdf->Fill(gpaSemRaw - gpaSemPredictionCdf);
			
			// Now look at seom single course predictive measures
			for (Student::Grade const& grade : enrollment.grades) {
				if (!MyFunctions::ValidGrade(grade.grade)) continue;
	
				if (jentry == 0) {
					std::cout << "Student 0, Course = " << grade.course << std::endl;
				}
				double thisQuality = MyFunctions::GradeToQuality(grade.grade);
				
				// Standard cumulative GPA without this course
				double gpaCourseBar = student->CumGpaWithoutCourse(grade.term, grade.course);
				
				// Single course prediction based on simple course normalizing
				double qualityPredictionNormed = student->NormedCoursePrediction(&grade);

				// Try CDF prediction:
				TGraph* cdfCourseBar = student->CombinedCdfWithoutCourse(&grade);
				double pctCourseBar = cdfCourseBar->Eval(gpaCourseBar);
//				double pctCourseBar = MyFunctions::EvalCdf(cdfCourseBar, gpaCourseBar);
				std::vector<Student::Grade> oneCourse;
				oneCourse.push_back(grade);
				TGraph* cdfInvCourse = student->CombinedCdfInv(oneCourse);
//				double qualityPredictionCdf = MyFunctions::EvalInvCdf(cdfInvCourse, pctCourseBar);
				double qualityPredictionCdf = cdfInvCourse->Eval(pctCourseBar);
				
				hDeltaCourseBiased->Fill(thisQuality - gpaFullRaw);
				hDeltaCourseRaw->Fill(thisQuality - gpaCourseBar);
				hDeltaCourseNormed->Fill(thisQuality - qualityPredictionNormed);
				hDeltaCourseCdf->Fill(thisQuality - qualityPredictionCdf);
				
				if (scan) {
					c5->cd(1);
					cdfCourseBar->Draw("AP");
					TMarker m(gpaCourseBar, pctCourseBar, 29);
					m.Draw();
					TLine l(gpaCourseBar, 0., gpaCourseBar, pctCourseBar);
					l.Draw();
					l.DrawLine(0., pctCourseBar, gpaCourseBar, pctCourseBar);
					c5->cd(2);
					cdfInvCourse->Draw("AP");
					m.DrawMarker(pctCourseBar, qualityPredictionCdf);
					l.DrawLine(0., thisQuality, 1., thisQuality);
					std::cout << "gpaCourseBar = " << gpaCourseBar << ", pctCourseBar = " << pctCourseBar << ", gpaCoursePred = " << qualityPredictionCdf << ", gpaSem = " << thisQuality << std::endl;
					c5->Modified();
					c5->Update();
					std::cout << "Next (q to quit):";
					if (gSystem->ProcessEvents()) return;
					gets(s);
					if (s[0] == 'q') return;
				}
			
				
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