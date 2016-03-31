#include "MyFunctions.h"
#include "CourseGradeNormer.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>

#include <iostream>
#include <vector>

void GradeNormMapPlots(TString course = "MATH140") {
	
	MyFunctions::BuildGradeNormMap();
	
	CourseGradeNormer& cgn = MyFunctions::gradeNormMap.at(course);
	TCanvas* c1 = new TCanvas("c1", "Grade Norm Stuff", 1600, 1200);
	c1->Divide(2,2);
	c1->cd(1);
	cgn.GradeDistribution()->DrawCopy();
	c1->cd(2);
	cgn.CumulativeDistribution()->DrawCopy();
	c1->cd(3);
	cgn.CumulativeGraph()->Draw();
	c1->cd(4);
	cgn.CumulativeGraphInverse()->Draw();
	
	TGraph* inv = cgn.CumulativeGraphInverse();
	int n = inv->GetN();
	double* x = inv->GetX();
	double* y = inv->GetY();
	
	double yPrev = 0.;
	std::vector<std::pair<double,double>> delta;
	for (int i = 0; i < n; ++i) {
		std::cout << "x, y = " << x[i] << ", " << y[i] << std::endl;
		double delY = y[i] - yPrev;
		yPrev = y[i];
		delta.push_back(std::make_pair(x[i], delY));
	}
	std::sort(delta.begin(), delta.end());
	for (auto p : delta) {
		std::cout << p.first << ", " << p.second << std::endl;
	}
	
	return;
}

void PracticeAdding() {
	
	MyFunctions::BuildGradeNormMap();
	
	std::vector<TString> courses = {"ENGL101", "MATH140", "COMM107", "UNIV101"};
	
	TCanvas* c1 = new TCanvas("c1", "Adding Inverses", 1600, 1200);
	c1->Divide(2,3);
	int pad = 1;
	
	std::vector<std::pair<double, double>> deltas;
	double totalCredits = 0.;
	for (auto course : courses) {
		CourseGradeNormer& cgn = MyFunctions::gradeNormMap.at(course);
		TGraph* inv = cgn.CumulativeGraphInverse();
		c1->cd(pad);
		inv->Draw();
		++pad;
		
		double yPrev = 0.;
		int n = inv->GetN();
		double* x = inv->GetX();
		double* y = inv->GetY();
		for (int i = 0; i < n; ++i) {
			double delY = y[i] - yPrev;
			delY *= 3.;
			yPrev = y[i];
			deltas.push_back(std::make_pair(x[i], delY));
		}
		totalCredits += 3.;
	}
	std::sort(deltas.begin(), deltas.end());
	double yTotal = 0.;
	std::vector<double> xvals;
	std::vector<double> yvals;
	std::vector<double> errEmpty;
	std::vector<double> errXminus;
	double xPrev = 0.0;
	for (auto point : deltas) {
		yTotal += point.second/totalCredits;
		xvals.push_back(point.first);
		yvals.push_back(yTotal);
		errEmpty.push_back(0.);
		errXminus.push_back(point.first - xPrev);
		xPrev = point.first;
	}
	TGraph* sumGraph = new TGraphAsymmErrors(xvals.size(), &xvals[0], &yvals[0], &errXminus[0]);
	sumGraph->SetTitle("Test Adding Inverse Functions;Percentile;GPA");
	sumGraph->SetMarkerSize(1.);
	sumGraph->SetMarkerStyle(20);
	sumGraph->SetMarkerColor(kBlue);
	c1->cd(pad);
	sumGraph->Draw("AP");
	
	
	return;
}