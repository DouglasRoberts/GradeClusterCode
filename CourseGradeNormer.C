#include "CourseGradeNormer.h"
#include "MyFunctions.h"
#include <TH1D.h>
#include <TString.h>
#include <iostream>

int CourseGradeNormer::instance = 0;

CourseGradeNormer::CourseGradeNormer() {
	++instance;
	TString title;
	title.Form("grade_dist_%d", instance);
	gradeDistribution = new TH1D(title, title, 13, 0., 13.);
	MyFunctions::GradeLabels(gradeDistribution->GetXaxis());
	gradeDistribution->SetDirectory(0);  // We want this histo to live even if a file gets closed.
}

void CourseGradeNormer::AddGrade(TString grade) {
	double qualityPoints = MyFunctions::GradeToQuality(grade);
	nEntries++;
	sumQ += qualityPoints;
	sumQ2 += qualityPoints*qualityPoints;
	gradeDistribution->Fill(grade, 1.);
}

double CourseGradeNormer::Average() const {
	if (nEntries > 0)
		return sumQ/nEntries;
	else
		return 0.;
}

double CourseGradeNormer::StdDev() const {
	if (nEntries > 1)
		return sqrt(sumQ2/nEntries - Average()*Average());
	else
		return 0.;
}

void CourseGradeNormer::MakeCumulativeDist() {
	if (0 != gradeDistribution) {
		double integral = gradeDistribution->Integral();
		cumulativeGradeDistribution = (TH1D*)gradeDistribution->GetCumulative();
		cumulativeGradeDistribution->Scale(1./integral);
		cumulativeGradeDistribution->SetDirectory(0);
	}	
}

TH1D* CourseGradeNormer::CumulativeDistribution() {
	if (cumulativeGradeDistribution == 0)
		MakeCumulativeDist();
	return cumulativeGradeDistribution;
}

TGraph* CourseGradeNormer::CumulativeGraph() {
	if (cumulativeGraph == 0) {
		if (cumulativeGradeDistribution == 0)
			MakeCumulativeDist();
		if (cumulativeGradeDistribution == 0)
			return 0;
		double xVals[13], yVals[13];
		for (int i = 0; i < 13; i++) {
			TString grade = cumulativeGradeDistribution->GetXaxis()->GetBinLabel(i+1);
			xVals[i] = MyFunctions::GradeToQuality(grade);
			yVals[i] = cumulativeGradeDistribution->GetBinContent(i+1);
		}
		cumulativeGraph = new TGraph(13, xVals, yVals);
		cumulativeGraph->SetTitle("Cumulative Grade Distribution;Grade Quality Points;Percentile");
		cumulativeGraph->SetMarkerSize(1.);
		cumulativeGraph->SetMarkerStyle(20);
		cumulativeGraph->SetMarkerColor(kBlue);
		cumulativeGraphInverse = new TGraph(13, yVals, xVals);
		cumulativeGraphInverse->SetTitle("Inverse Cumulative Grade Distribution;Percentile;Grade Quality Points");
		cumulativeGraphInverse->SetMarkerSize(1.);
		cumulativeGraphInverse->SetMarkerStyle(20);
		cumulativeGraphInverse->SetMarkerColor(kBlue);
	}
	return cumulativeGraph;
}

TGraph* CourseGradeNormer::CumulativeGraphInverse() {
	if (cumulativeGraphInverse == 0) {
		CumulativeGraph();
	}
	return cumulativeGraphInverse;	
}