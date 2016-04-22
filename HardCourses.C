#include "Student.h"
#include "MyFunctions.h"

#include <TROOT.h>

#include <TBenchmark.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <THStack.h>
#include <TProfile.h>
#include <TSQLResult.h>
#include <TSQLRow.h>
#include <TSQLServer.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>

#include <vector>
#include <map>
#include <iostream>
#include <random>

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

void LookAtStudents(int iterationsMax = 10) {
	
	gStyle->SetErrorX(0);
	
	// Set up random number generator for future use
//    std::random_device randomDevice;  // Use this for non-deterministic random seeding
//    std::mt19937 randomGenerator(randomDevice());
	std::mt19937 randomGenerator;   //Use default seed.  This is deterministic
	
	
	if (MyFunctions::gradeNormMap.size() == 0)
		MyFunctions::BuildGradeNormMap();
	
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
	TH1D* nfVsIterHist = new TH1D("nfVsIterHist", "NF Enrollment Code GPA Bias", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* r1VsIterHist = new TH1D("r1VsIterHist", "R1 Enrollment Code GPA Bias", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* r2VsIterHist = new TH1D("r2VsIterHist", "R2 Enrollment Code GPA Bias", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* r3VsIterHist = new TH1D("r3VsIterHist", "R3 Enrollment Code GPA Bias", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* r4VsIterHist = new TH1D("r4VsIterHist", "R4 Enrollment Code GPA Bias", iterationsMax, 0.5, iterationsMax + 0.5);
	nfVsIterHist->SetLineColor(kBlue);
	r1VsIterHist->SetLineColor(kRed);
	r2VsIterHist->SetLineColor(kViolet);
	r3VsIterHist->SetLineColor(kOrange);
	r4VsIterHist->SetLineColor(kMagenta);
	THStack* enrollVsIterStack = new THStack("enrollVsIterStack","");
	enrollVsIterStack->Add(nfVsIterHist);
	enrollVsIterStack->Add(r1VsIterHist);
	enrollVsIterStack->Add(r2VsIterHist);
	enrollVsIterStack->Add(r3VsIterHist);
	enrollVsIterStack->Add(r4VsIterHist);
	
	TH1D* interceptHist = new TH1D("interceptHist", "Intercept From Load Correction", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* slopeHist = new TH1D("slopeHist", "Slope From Load Correction", iterationsMax, 0.5, iterationsMax + 0.5);
	THStack* loadStack = new THStack("loadStack", "");
	interceptHist->SetLineColor(kBlue);
	slopeHist->SetLineColor(kRed);
	loadStack->Add(interceptHist);
	loadStack->Add(slopeHist);
	
	TH1D* allVsAttCredits = new TH1D("allVsAttCredits", "All credits - attemtped credits (used in GPA calc)", 15, 0., 15.);
	TProfile* gpaPredictionVsAllAttCredits = new TProfile("gpaPredictionVsAllAttCredits", "Predicted GPA Residual vs. All Attempted Credits", 25, 10., 35.);
	TH1D* relativeLoadHist = new TH1D("relativeLoadHist", "Relative Course Load in Credits", 50, -10., 10.);
	TProfile* relativeLoadProf = new TProfile("relativeLoadProf", "Predicted GPA vs. Relative Course Load in Credits", 50, -10., 10.);
	
	TH1D* delGpaRawHist = new TH1D("delGpaRawHist", "GPA_{SEM} - GPA_{#bar{SEM}}, Raw", 100, -4., 4.);
	TH1D* delGpaRenormedHist = new TH1D("delGpaRenormedHist", "GPA_{SEM} - GPA_{#bar{SEM}}, Re-Normed", 100, -4., 4.);
	TH1D* delGpaEnrollHist = new TH1D("delGpaEnrollHist", "GPA_{SEM} - GPA_{#bar{SEM}}, With Enroll Type Correction", 100, -4., 4.);
	TH1D* delGpaLoadHist = new TH1D("delGpaLoadHist", "GPA_{SEM} - GPA_{#bar{SEM}}, With Course Load Correction", 100, -4., 4.);
	TH1D* delGpaHardHist = new TH1D("delGpaHardHist", "GPA_{SEM} - GPA_{#bar{SEM}}, With Course Difficulty Correction", 100, -4., 4.);
	TH1D* delGpaAllHist = new TH1D("delGpaAllHist", "GPA_{SEM} - GPA_{#bar{SEM}}, With All Corrections", 100, -4., 4.);
	
	TH1D* rmsRawHist = new TH1D("rmsRawHist", "RMS of Uncorrected #DeltaGPA", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* rmsEnrollHist = new TH1D("rmsEnrollHist", "RMS of Enroll Type Corrected #DeltaGPA", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* rmsLoadHist = new TH1D("rmsLoadHist", "RMS of Load Corrected #DeltaGPA", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* rmsHardHist = new TH1D("rmsHardHist", "RMS of Difficulty Corrected #DeltaGPA", iterationsMax, 0.5, iterationsMax + 0.5);
	TH1D* rmsAllHist = new TH1D("rmsAllHist", "RMS of Fully Corrected #DeltaGPA", iterationsMax, 0.5, iterationsMax + 0.5);
	rmsRawHist->SetLineColor(kBlue);
	rmsEnrollHist->SetLineColor(kRed);
	rmsLoadHist->SetLineColor(kViolet);
	rmsHardHist->SetLineColor(kMagenta);
	rmsAllHist->SetLineColor(kOrange);
	THStack* rmsStack = new THStack("rmsStack", "");
	rmsStack->Add(rmsRawHist);
	rmsStack->Add(rmsEnrollHist);
	rmsStack->Add(rmsLoadHist);
	rmsStack->Add(rmsHardHist);
	rmsStack->Add(rmsAllHist);
	rmsStack->SetMinimum(0.35);
	
	TH1D* iterChi2 = new TH1D("iterChi2", "Chi^2 vs. Iteration Number", iterationsMax, 0.5, iterationsMax + 0.5);
	
	std::map<TString, CourseDelta> emptyMap;
	std::map<TString, CourseDelta> oldMap;
	std::map<TString, CourseDelta> newMap;
	std::map<TString, CourseDelta> hardCourses;
	std::map<TString, CourseDelta> hardPrefixes;
	
//	nentries = 100;
	TBenchmark myBenchmark;
	
	myBenchmark.Start("HardCourses, Loop 1");
	int iterations = 0;
	double damping = 0.1;   // Damp changes per iteration to prevent ocsillations
	std::map<TString, double> prevEnrollTypeCorrection = {{"NF", 0.}, {"R1", 0.}, {"R2", 0.}, {"R3", 0}, {"R4", 0.}};
	std::map<TString, double> enrollTypeCorrection = prevEnrollTypeCorrection;
	
	double loadCorrIntercept = 0.;
	double loadCorrSlope = 0.;
	double loadCorrInterceptError = 0.;
	double loadCorrSlopeError = 0.;
	double prevLoadIntercept = 0.;
	double prevLoadSlope = 0.;

	TCanvas* cIter = new TCanvas("cIter", "Iteration Tracking", 1600, 1200);
	cIter->Divide(1,4);
	TCanvas* cIter2 = new TCanvas("cIter2", "Iteration Tracking", 1600, 1200);
	cIter2->Divide(2,3);
	
	TF1* myLine = new TF1("myLine", "pol1");
	myLine->SetParameters(0., 0.);
	myLine->FixParameter(0, 0.); // Fix intercept to 0, otherwise we get coupling with the enrollment type corrections
	do {
		++iterations;
		std::cout << "Starting iteration " << iterations << std::endl;
		//Create a map from enroll_type to GPA correction based on previous iteration
		for (int iBin = 1; iBin <= gpaPredictionVsStatus->GetNbinsX(); iBin++) {
			TString label = gpaPredictionVsStatus->GetXaxis()->GetBinLabel(iBin);
			// Get info from last iteration
			double content = gpaPredictionVsStatus->GetBinContent(iBin);
			double prevContent = prevEnrollTypeCorrection[label];
			prevEnrollTypeCorrection[label] = enrollTypeCorrection[label];
			// Damp this motion
			content = damping*content + (1. - damping)*prevContent;
			double error = gpaPredictionVsStatus->GetBinError(iBin);  // Errors on previous and current are totally correlated
			enrollTypeCorrection[label] = content;
			switch (iBin) {
				case 1:
				nfVsIterHist->SetBinContent(iterations, content);
				nfVsIterHist->SetBinError(iterations, error);
				break;
				case 2:
				r1VsIterHist->SetBinContent(iterations, content);
				r1VsIterHist->SetBinError(iterations, error);
				break;
				case 3:
				r2VsIterHist->SetBinContent(iterations, content);
				r2VsIterHist->SetBinError(iterations, error);
				break;
				case 4:
				r3VsIterHist->SetBinContent(iterations, content);
				r3VsIterHist->SetBinError(iterations, error);
				break;
				case 5:
				r4VsIterHist->SetBinContent(iterations, content);
				r4VsIterHist->SetBinError(iterations, error);
				break;
				
				default:
				break;
			}
		}
		
		relativeLoadProf->Fit(myLine, "BQ0");
		if (iterations > 1) {
			double intercept = myLine->GetParameter(0);
			double slope = myLine->GetParameter(1);
			prevLoadIntercept = loadCorrIntercept;
			prevLoadSlope = loadCorrSlope;
			loadCorrIntercept = damping*intercept + (1. - damping)*loadCorrIntercept;
			loadCorrSlope = damping*slope + (1. - damping)*loadCorrSlope;
			loadCorrInterceptError = myLine->GetParError(0);
			loadCorrSlopeError = myLine->GetParError(1);
		}
		interceptHist->SetBinContent(iterations, loadCorrIntercept);
		interceptHist->SetBinError(iterations, loadCorrInterceptError);
		slopeHist->SetBinContent(iterations, loadCorrSlope);
		slopeHist->SetBinError(iterations, loadCorrSlopeError);
		
		// Update course delta map
		oldMap = emptyMap;
		for (auto& mapElement : oldMap) {
			TString name = mapElement.second.name;
			mapElement.second.name = newMap[name].name;
			mapElement.second.nEntries = newMap[name].nEntries;
			mapElement.second.deltaSum = damping*newMap[name].deltaSum + (1. - damping)*mapElement.second.deltaSum;
			// What should I do with sum^2?
			mapElement.second.deltaSum2 = newMap[name].deltaSum2;
		}
		newMap = emptyMap;
		
		// Now reset histograms for next iteration
		gpaPredictionVsStatus->Reset();
		relativeLoadProf->Reset();
		gpaPredictionVsAllAttCredits->Reset();
		delGpaRawHist->Reset();
		delGpaEnrollHist->Reset();
		delGpaLoadHist->Reset();
		delGpaHardHist->Reset();
		delGpaAllHist->Reset();
				
		// First, let's loop over everything and create both the enrollment status correction and the seed for the hard courses calculations
		std::vector<int> courseToUseIndex(nentries);	
		for (Long64_t jentry = 0; jentry < nentries; jentry++) {
			studentTree->GetEntry(jentry);
			student->Finalize();  // This regenerates the enrollment.grades stuff that doesn't persist
			// Only use half the students for training
			//		if (student->Id() % 2 != 0) continue;
			for (auto const& enrollment : student->Enrollments()) {
				// Only look at Fall and Spring Terms
				int term = enrollment.term;
				if (!MyFunctions::regularSemester(term)) continue;
				// Only look at semesters with at least 12 attempted credits
				double attemptedCredits = student->AttemptedCredits(term);
				if (attemptedCredits < 12.) continue;

				double allAttemptedCredits = student->AllAttemptedCredits(enrollment.term);
				double avgAttemptedCredits = student->AvgAttemptedCredits();
				double relativeLoad = allAttemptedCredits - avgAttemptedCredits;
				
				double semesterGpa = student->Gpa(enrollment.term);
				double delGpaRaw = semesterGpa - student->Gpa(-enrollment.term);
				double normedGpaPrediction = student->NormedGpa(-enrollment.term);
				double unNormedPrediction = student->UnNormedGpa(normedGpaPrediction, enrollment.term);
//				unNormedDeltaGpa->Fill(student->Gpa(enrollment.term) - unNormedPrediction);
				double delGpaRenormed = semesterGpa - unNormedPrediction;
				double enrollCorr = enrollTypeCorrection[enrollment.enrollType];
				double loadCorr = loadCorrIntercept + loadCorrSlope*relativeLoad;
				double hardCorr = 0.;
//				double gpaSemBarCorr = student->Gpa(-enrollment.term) + enrollCorr + loadCorr;
				double gpaSemBarCorr = unNormedPrediction + enrollCorr + loadCorr;
			
				// Here we are going to calculate both the individual course deltas and the overall semester Delta for trackcing.
				double degreeOfDifficulty = 0.;
				double sigDegOfDiff = 0.;
				double totalCredits = 0.;
				double sumCrDeltas = 0.;
				
				auto termGrades = student->TermLetterGradeList(enrollment.term);  // Note that this will only pull grades in the F - A+ range
				
				// On first iteration, flag one randomly chosen course per semester to feed into the delta calculation
				if (1 == iterations) {
					std::uniform_int_distribution<> uniformInt(0, termGrades.size() - 1);
					int useThisClass = uniformInt(randomGenerator);
					courseToUseIndex[jentry] = useThisClass;
				}
				
				int nDeltasAdded = 0;
				for (Student::Grade const& grade : termGrades) {
					// Make sure this is an F-A+ kind of grade
//					if (!MyFunctions::ValidGrade(grade.grade)) continue;
					totalCredits += grade.credits;
					// Only look at courses 3 or more credits
//					if (grade.credits < 2.99) continue;
					// Some stuff to do only on first iteration...
					auto thisIndex = &grade - &termGrades[0];
					if (iterations == 1) {
						if (courseToUseIndex[jentry] == thisIndex) emptyMap[grade.course].name = grade.course;
						enrollStatusHist->Fill(enrollment.enrollType, 1.);
						allVsAttCredits->Fill(allAttemptedCredits - attemptedCredits);
						relativeLoadHist->Fill(relativeLoad);
					}
					double coeff = (1. - grade.credits/attemptedCredits);
					if (coeff == 0.) {
						coeff = 0.;  // Gotta love renormalization, lol
					}
					else {
						coeff = 1./coeff;
					}
					// Get GPA for this semester, without this course. Is this right?  Prehaps using the course is ok?
					double gpaSemNoCourse = student->SemesterGpaWithoutCourse(grade.term, grade.course);
//					double deltaSem = gpaSemNoCourse - gpaSemBarCorr;
					double gpaPrediction = student->UnNormedGpa(normedGpaPrediction, enrollment.term, grade.course);
					double deltaSem = gpaSemNoCourse - (gpaPrediction + enrollCorr + loadCorr);
//					delGpa3->Fill(deltaSem);
					double sumDelta = 0.;
					double sumCrDelta = 0.;
					for (Student::Grade const& grade_i : termGrades) {
						if (grade_i.course == grade.course) continue;
						if (!MyFunctions::ValidGrade(grade_i.grade)) continue;
						auto search = oldMap.find(grade_i.course);
						if (search != oldMap.end()) {
//							std::cout << "About to add a delta = " << search->second.average() << std::endl;
							sumDelta += search->second.average();
							sumCrDelta += grade_i.credits*search->second.average();
						}
					}
//					std::cout << "sumDelta = " << sumDelta << std::endl;
					sumCrDeltas += grade.credits*sumDelta;
					// Now calculate difficulty of this course, then add it to the map for this iteration.
					double delta = coeff*(deltaSem - (sumDelta - sumCrDelta/attemptedCredits));
					if (thisIndex == courseToUseIndex[jentry]) {
						auto search = newMap.find(grade.course);
						if (search != newMap.end()) {
							search->second.AddDelta(delta);
							nDeltasAdded++;
						}
					}
				}
				hardCorr = sumCrDeltas/totalCredits;
//				std::cout << "nDeltasAdded = " << nDeltasAdded << std::endl;
				
//				std::cout << "hardCorr, sumCrDeltas = " << hardCorr << ", " << sumCrDeltas << std::endl;

				// Fill plot with GPA prediction vs. enrollment status for later use
				delGpaRawHist->Fill(delGpaRaw);
				delGpaRenormedHist->Fill(delGpaRenormed);
				delGpaEnrollHist->Fill(delGpaRenormed - enrollCorr);
				delGpaLoadHist->Fill(delGpaRenormed - loadCorr);
				delGpaHardHist->Fill(delGpaRenormed - hardCorr);
				delGpaAllHist->Fill(delGpaRenormed - loadCorr - enrollCorr - hardCorr);
				
				gpaPredictionVsStatus->Fill(enrollment.enrollType, delGpaRenormed - loadCorr - hardCorr);
				gpaPredictionVsAllAttCredits->Fill(allAttemptedCredits, delGpaRenormed - enrollCorr - hardCorr);
				relativeLoadProf->Fill(relativeLoad, delGpaRenormed - enrollCorr);
			}
		}
		
		// Now compare old and new maps to see if they are still changing...
		double sumChi2 = 0.;
		int nPrinted = 0;
		int nUsed = 0;
		for (auto const& newEntry : newMap) {
			auto const& newElement = newEntry.second;
//			delEntries->Fill(newElement.nEntries);
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
//		engl101->Fill(iterations, newMap.at("ENGL101").average());
//		engl101->SetBinError(iterations+1, newMap.at("ENGL101").errorOnMean());
//		math140->Fill(iterations, newMap.at("MATH140").average());
//		math140->SetBinError(iterations+1, newMap.at("MATH140").errorOnMean());
		iterChi2->Fill(iterations, sumChi2);
		if (iterations == 0) {
			std::cout << "Number of courses used = " << nUsed << std::endl;
			std::cout << "MATH140 entries = " << newMap.at("MATH140").nEntries << std::endl;
		}		
		
		
		// Plot some stuff
		rmsRawHist->SetBinContent(iterations, delGpaRawHist->GetStdDev());
		rmsRawHist->SetBinError(iterations, delGpaRawHist->GetStdDevError());
		rmsEnrollHist->SetBinContent(iterations, delGpaEnrollHist->GetStdDev());
		rmsEnrollHist->SetBinError(iterations, delGpaEnrollHist->GetStdDevError());
		rmsLoadHist->SetBinContent(iterations, delGpaLoadHist->GetStdDev());
		rmsLoadHist->SetBinError(iterations, delGpaLoadHist->GetStdDevError());
		rmsHardHist->SetBinContent(iterations, delGpaHardHist->GetStdDev());
		rmsHardHist->SetBinError(iterations, delGpaHardHist->GetStdDevError());
		rmsAllHist->SetBinContent(iterations, delGpaAllHist->GetStdDev());
		rmsAllHist->SetBinError(iterations, delGpaAllHist->GetStdDevError());
		
		cIter->cd(1);
		enrollVsIterStack->Draw("nostack E1P");
		cIter->cd(1)->BuildLegend();
		enrollVsIterStack->Draw("nostack L HIST SAME");
		
		cIter->cd(2);
		loadStack->Draw("nostack E1P");
		cIter->cd(2)->BuildLegend();
		loadStack->Draw("nostack L HIST SAME");
		
		cIter->cd(3);
		rmsStack->Draw("nostack E1P");
		cIter->cd(3)->BuildLegend();
		rmsStack->Draw("nostack L HIST SAME");
		
		cIter->cd(4);
		iterChi2->DrawCopy("HIST");
		
		cIter->Modified();
		cIter->Update();
		
		cIter2->cd(1);
		delGpaRawHist->DrawCopy();
		cIter2->cd(2);
		delGpaRenormedHist->DrawCopy();
		cIter2->cd(3);
		delGpaEnrollHist->DrawCopy();
		cIter2->cd(4);
		delGpaLoadHist->DrawCopy();
		cIter2->cd(5);
		delGpaHardHist->DrawCopy();
		cIter2->cd(6);
		delGpaAllHist->DrawCopy();
		
		cIter2->Modified();
		cIter2->Update();
		
		if (gSystem->ProcessEvents()) return;
		
	} while (iterations < iterationsMax);
	
	std::cout << "Size of empty course map = " << emptyMap.size() << std::endl;
	
	// Next, let's loop over everything and look at dependence on credit load, corrected for the enrollmentStatus effect (does order matter?)	
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		student->Finalize();
		// Only use half the students for training
//		if (student->Id() % 2 != 0) continue;
		for (auto const& enrollment : student->Enrollments()) {
			// Only look at Fall and Spring Terms
			if (MyFunctions::termName(enrollment.term) != "Fall" && MyFunctions::termName(enrollment.term) != "Spring") continue;
			// Only look at semesters with at least 12 attempted credits
			double attemptedCredits = student->AttemptedCredits(enrollment.term);
			double allAttemptedCredits = student->AllAttemptedCredits(enrollment.term);
			if (attemptedCredits < 12.) continue;
//			allVsAttCredits->Fill(allAttemptedCredits - attemptedCredits);
			
			double delGpa = student->Gpa(enrollment.term) - student->Gpa(-enrollment.term) - enrollTypeCorrection[enrollment.enrollType];
//			double delGpa = student->Gpa(enrollment.term) - student->Gpa(-enrollment.term);
			gpaPredictionVsAllAttCredits->Fill(allAttemptedCredits, delGpa);
			// Look at load relative to the student's average course load
//			double relativeLoad = allAttemptedCredits - student->AvgAttemptedCredits();
//			relativeLoadHist->Fill(relativeLoad);
//			relativeLoadProf->Fill(relativeLoad, delGpa);
		}
	}
	
	
	
	//Plot some of this stuff
	TCanvas* canvas1 = new TCanvas("canvas1", "Hard Courses Plots");
	canvas1->Divide(2,3);
	canvas1->cd(1);
	gpaPredictionVsStatus->DrawCopy();
	canvas1->cd(2);
	enrollStatusHist->DrawCopy();
	canvas1->cd(3);
	allVsAttCredits->DrawCopy();
	canvas1->cd(4);
	gpaPredictionVsAllAttCredits->DrawCopy();
	canvas1->cd(5);
	relativeLoadHist->DrawCopy();
	canvas1->cd(6);
	relativeLoadProf->Fit("pol1");
	relativeLoadProf->DrawCopy();
	canvas1->Modified();
	canvas1->Update();
	
//	return;
	
//	TCanvas* correlationCanvas = new TCanvas("correlationCanvas", "Course Correlations", 1800, 1200);
//	correlationCanvas->cd();
	// Normalize the course correlation matrix
//	for (int i = 1; i <= courseCorrNorm->GetXaxis()->GetNbins(); i++) {
//		double norm = courseCorrNorm->GetBinContent(i);
//		assert(courseCorrNorm->GetXaxis()->GetBinLabel(i) == courseCorrelation->GetXaxis()->GetBinLabel(i));
//		if (norm == 0.) continue;
//		for (int j = 1; j <= courseCorrNorm->GetXaxis()->GetNbins(); j++) {
//			double normedContent = courseCorrelation->GetBinContent(i,j)/norm;
//			courseCorrelation->SetBinContent(i,j, normedContent);
//			if (norm > 100. && normedContent > 0.8) {
//				std::cout << "Found Highly Correlated Course Pair: " << courseCorrelation->GetXaxis()->GetBinLabel(i) <<
//					":" << courseCorrelation->GetYaxis()->GetBinLabel(j) << " : " << normedContent << " : " << norm << std::endl;
//			}
//		}
//	}
//	courseCorrelation->GetXaxis()->LabelsOption("a");
//	courseCorrelation->GetXaxis()->SetTicks("-");
//	courseCorrelation->GetYaxis()->LabelsOption("a");
//	courseCorrelation->GetYaxis()->SetTicks("-");
//	courseCorrelation->DrawCopy();
//	correlationCanvas->Modified();
//	correlationCanvas->Update();

//	if (gSystem->ProcessEvents()) return;
	
	myBenchmark.Show("HardCourses, Loop 1");
	
	/*
	// Now we have map of course deltas set up and enroll status correction calculated
	//  Loop over students again...
//	int iterationsMax = 10;
	myBenchmark.Start("HardCourses, Loop 2");
	TH1D* delGpa1 = new TH1D("delGpa1", "Full Semester GPA - Uncorrected, Unbiased Prediction", 100, -4., 4.);
	TH1D* delGpa2 = new TH1D("delGpa2", "Full Semester GPA - Corrected, Unbiased Prediction", 100, -4., 4.);
	TH1D* delGpa3 = new TH1D("delGpa3", "Semester GPA w/o Course - Corrected, Unbiased Prediction", 100, -4., 4.);
	TH1D* delEntries = new TH1D("delEntries", "Entries in every course delta object", 100, 0., 10000.);
	TH1D* engl101 = new TH1D("engl101", "Evolution of delta for ENGL101", iterationsMax, -0.5, iterationsMax - 0.5);
	TH1D* math140 = new TH1D("math140", "Evolution of delta for MATH140", iterationsMax, -0.5, iterationsMax - 0.5);
	
//	std::map<TString, CourseDelta> oldMap = emptyMap;
//	std::map<TString, CourseDelta> newMap = emptyMap;
	TCanvas* canvas2 = new TCanvas("canvas2", "Hard Courses Plots", 1200, 800);
	canvas2->Divide(3,2);
	
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
			student->Finalize();
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

				for (Student::Grade const& grade : enrollment.grades) {
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
					for (Student::Grade const& grade_i : enrollment.grades) {
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
	*/
	
	myBenchmark.Show("HardCourses, Loop 2");
//	return;
	
	myBenchmark.Start("HardCourses, Loop 3");
	
	
/*	
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		studentTree->GetEntry(jentry);
		student->Finalize();
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
		student->Finalize();
		if (student->Id() % 2 == 0) continue;

		for (Student::Enrollment const& enrollment : student->Enrollments()) {
			int term = enrollment.term;
			if (MyFunctions::termName(term) != "Fall" && MyFunctions::termName(term) != "Spring") continue;
			if (student->AttemptedCredits(term) < 12.) continue; 
			auto termGrades = student->TermLetterGradeList(enrollment.term);
			double degreeOfDifficulty = 0.;
			double sigDegOfDiff = 0.;
			double totalCredits = 0.;
			double sumCrDeltas = 0.;
			for (Student::Grade const& grade_i : termGrades) {
				totalCredits += grade_i.credits;
				double sumDeltas = 0.;
				for (Student::Grade const& grade_j : termGrades) {
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