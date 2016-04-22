#include "MyFunctions.C"
#include "Student.h"

#include <TBenchmark.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
#include <TString.h>
#include <TTree.h>

#include <iostream>
#include <map>
#include <vector>

struct CorrelationCalculator {
public:
	void Add(double x, double y) {
		++_n;
		_sumX += x;
		_sumY += y;
		_sumX2 += x*x;
		_sumY2 += y*y;
		_sumXY += x*y;
	}
	double r() const {
		if (_n < 2) return 0.;
		double sigx2 = (_sumX2 - _sumX*_sumX/_n)/(_n);
		double sigy2 = (_sumY2 - _sumY*_sumY/_n)/(_n);
		if (sigx2 <= 0. || sigy2 <= 0.) {
//			std::cout << "ZERO: sigx = " << sigx << ", sigy = " << sigy << ", n= " << _n << ", _sumX = " << _sumX << ", _sumY = " << _sumY 
//				<< "_sumX2 = " << _sumX2 << ", _sumY2 = " << _sumY2 << ", _sumXY = " << _sumXY << std::endl;
			return 0.;
		}
		if (sigx2*sigy2 != sigx2*sigy2) {
			std::cout << "NAN: sigx = " << sigx2 << ", sigy = " << sigy2 << ", n= " << _n << ", _sumX = " << _sumX << ", _sumY = " << _sumY 
				<< ", _sumX2 = " << _sumX2 << ", _sumY2 = " << _sumY2 << ", _sumXY = " << _sumXY << std::endl;
			return 1.1;
		}
		double retVal = (_sumXY - _sumX*_sumY/(double)_n)/(sqrt(sigx2*sigy2)*(_n));
		
		if (fabs(retVal) > 1.01) {
			std::cout << std::endl;
			std::cout << "r = " << retVal << std::endl;
			std::cout << "OOB: sigx2 = " << sigx2 << ", sigy2 = " << sigy2 << ", n= " << _n << ", _sumX = " << _sumX << ", _sumY = " << _sumY 
				<< "_sumX2 = " << _sumX2 << ", _sumY2 = " << _sumY2 << ", _sumXY = " << _sumXY << std::endl;
			std::cout << std::endl;			
		}
		return retVal;
	}
	
	double p() const {
		if (_n < 2) 
			return -1.;
		else {
			double rho = r();
			if (1. - rho*rho <= 0.) return -2.;
			double tstar = rho*sqrt(_n - 2)/sqrt(1. - rho*rho);
			return TMath::StudentI(tstar, _n - 2);
		}
	}
	
	int n() const {return _n;}
	
private:
	int _n = 0;
	double _sumX = 0.;
	double _sumY = 0.;
	double _sumX2 = 0.;
	double _sumY2 = 0.;
	double _sumXY = 0.;
};

bool sortFunc(std::pair<std::pair<TString, TString>, CorrelationCalculator> first, 
				std::pair<std::pair<TString, TString>, CorrelationCalculator> second) {
	return first.second.p() > second.second.p();
}


void GradeCorrelation(double prob = 0.10, int nCut = 50) {
	
	std::cout << "Starting Grade Correlation..." << std::endl;
	TBenchmark* myBenchmark = new TBenchmark();
	
	myBenchmark->Start("NormMap");
	// Get the student TTree prepared in another macro.
	if (MyFunctions::gradeNormMap.size() == 0)
		MyFunctions::BuildGradeNormMap();
	myBenchmark->Stop("NormMap");
	
	std::cout << "Made GradeNormMap..." << std::endl;
	
	// Just a quick look at the data in the Students TTree
	TFile* f = new TFile("Students.root");
	TTree* studentTree = (TTree*)f->Get("Students");
	Student* student = 0;
	studentTree->SetBranchAddress("student", &student);
	
	Long64_t nentries = studentTree->GetEntriesFast();
	
	std::cout << "Entries in Students TTree = " << nentries << std::endl;
	
	// We want to loop over students and look at pairs of courses taken in different semesters to see if there is
	// any correlation between graede performance.
	
	std::map<std::pair<TString, TString>, CorrelationCalculator> corrMap;
	
//	nentries = 1000;
	
	myBenchmark->Start("Main Loop");
	int nPairAll = 0;
	for (Long64_t jentry = 0; jentry < nentries; ++jentry) {
		studentTree->GetEntry(jentry);
		if (jentry % 1000 == 0)
			std::cout << "At student " << jentry << std::endl;
//		std::cout << "Looking at student " << student->Id() << std::endl;
		student->Finalize();  // Regenerates non-persisted references
		int nTerms = student->Enrollments().size();
//		std::cout << "nTerms = " << nTerms << std::endl;
		
		for (int iTerm = 0; iTerm < nTerms - 1; ++iTerm) {
			const Student::Enrollment iEnrollment = student->Enrollments()[iTerm];
			if (!MyFunctions::regularSemester(iEnrollment.term)) continue;
			for (Student::Grade iGrade : iEnrollment.grades) {
				if (!MyFunctions::ValidGrade(iGrade.grade)) continue;
				// Get a prediction for this grade using norm-corrected grades
//				std::cout << "1" << std::endl;
				double prediction_i = student->CourseGradePrediction(iGrade, Student::DISTRIBUTION);
//				std::cout << "2" << std::endl;
				double delta_i = iGrade.quality - prediction_i;
			
				// Find next regular term only
				bool foundRegTerm = false;
				for (int jTerm = iTerm + 1; jTerm < nTerms && !foundRegTerm; ++jTerm) {
					const Student::Enrollment jEnrollment = student->Enrollments()[jTerm];
					if (!MyFunctions::regularSemester(jEnrollment.term)) continue;
					foundRegTerm = true;
					for (Student::Grade jGrade : jEnrollment.grades) {
						if (jGrade.course == iGrade.course) continue;
						if (!MyFunctions::ValidGrade(jGrade.grade)) continue;
						
						double prediction_j = student->CourseGradePrediction(jGrade, Student::DISTRIBUTION);
						double delta_j = jGrade.quality - prediction_j;
						
						corrMap[std::make_pair(iGrade.course, jGrade.course)].Add(delta_i, delta_j);
						++nPairAll;
						
					}
				}
			} 
		}
		
	}
	myBenchmark->Stop("Main Loop");
	
	std::cout << "nPairAll     = " << nPairAll << std::endl;
	std::cout << "Unique Pairs = " << corrMap.size() << std::endl;
	
	TH1D* rHist = new TH1D("rHist", "Correlation Coefficient, #rho", 120, -1.2, 1.2);
	TH1D* pHist = new TH1D("pHist", "Probablity Distribution", 100, 0., 1.);
	TH1D* nHist = new TH1D("nHist", "Number of entries", 100, 0., 2000.);
	TH2D* pVrHist = new TH2D("pVrHist", "Prob vs. #rho", 100, -1., 1., 100, 0., 1.);
	
	myBenchmark->Start("Prune");
	for (auto iter = corrMap.begin(); iter != corrMap.end();) {
		if (iter->second.n() < nCut) {
			corrMap.erase(iter++);
			continue;
		}
		double p = iter->second.p();
		double r = iter->second.r();

		// Test for nan?
		if (p != p) {
			std::cout << "Found p = nan: n = " << iter->second.n() << std::endl;
			corrMap.erase(iter++);
			continue;
		}
		if (p < 0.) {
			corrMap.erase(iter++);
			continue;
		}
		rHist->Fill(r);
		pHist->Fill(p);
		nHist->Fill(iter->second.n());
		pVrHist->Fill(r, p);
		if (p < 1. - prob && p > prob) {
			corrMap.erase(iter++);
		}
		else {
			std::cout << "r = " << iter->second.r() << ", p = " << p << std::endl;
			++iter;
		}
	}
	myBenchmark->Stop("Prune");
	std::cout << "Post Cut     = " << corrMap.size() << std::endl;
	
	myBenchmark->Start("Sort");
	std::vector<std::pair<std::pair<TString, TString>, CorrelationCalculator>> corrVec(corrMap.begin(), corrMap.end());
	std::sort(corrVec.begin(), corrVec.end(), &sortFunc);
	myBenchmark->Stop("Sort");
	
	int printTop = 50;
	int printed = 0;
	for (auto const& entry : corrVec) {
		std::cout << entry.first.first << " : " << entry.first.second << "\t, n = " << entry.second.n() << "\t, r = " << entry.second.r() 
			<< "\t, p = " << entry.second.p() << std::endl;
		++printed;
		if (printed >= printTop) break;
	}

	TCanvas* c1 = new TCanvas("c1", "Grade Correlation", 1600, 1200);
	c1->Divide(2,2);
	c1->cd(1);
	TF1* myGaus = new TF1("myGaus", "gaus", -1., 1.);
	myGaus->SetParameters(600., 0., 0.2);
	myGaus->FixParameter(1, 0.);
	rHist->Fit(myGaus, "0B", "", -1., 0.);
	rHist->DrawCopy();
	myGaus->DrawCopy("SAME");
	c1->cd(2);
	pHist->DrawCopy();
	c1->cd(3);
	nHist->DrawCopy();
	c1->cd(4);
	pVrHist->DrawCopy();
	
	float rt, cp;
	myBenchmark->Summary(rt, cp);
	
}
