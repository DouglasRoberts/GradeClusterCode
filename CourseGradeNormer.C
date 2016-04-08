#include "CourseGradeNormer.h"

#include "GradeHistogram.h"
#include "MyFunctions.h"

//#include <TGraph.h>
//#include <TGraphAsymmErrors.h>
//#include <TH1D.h>
#include <TString.h>
//#include <iostream>

int CourseGradeNormer::instance = 0;

CourseGradeNormer::CourseGradeNormer() {
	++instance;
	TString title;
	title.Form("grade_dist_%d", instance);
	gradeDistribution = new GradeHistogram(title, title);
//	gradeDistribution = new TH1D(title, title, 13, 0., 13.);
//	MyFunctions::GradeLabels(gradeDistribution->GetXaxis());
//	gradeDistribution->SetDirectory(0);  // We want this histo to live even if a file gets closed.
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

/*
void CourseGradeNormer::MakeCumulativeDist() {
	if (0 != gradeDistribution) {
		double integral = gradeDistribution->Integral();
		cumulativeGradeDistribution = (TH1D*)gradeDistribution->GetCumulative();
		cumulativeGradeDistribution->Scale(1./integral);
		cumulativeGradeDistribution->SetDirectory(0);
	}	
}


TH1D* CourseGradeNormer::CumulativeDist() {
	if (cumulativeGradeDistribution == 0)
		MakeCumulativeDist();
	return cumulativeGradeDistribution;
}
*/
CumulativeDistribution* CourseGradeNormer::CumulativeGraph() {
	if (0 == cumulativeGraph) {
		cumulativeGraph = new CumulativeDistribution(gradeDistribution);
	}
	return cumulativeGraph;
}
		/*
		if (cumulativeGradeDistribution == 0)
			MakeCumulativeDist();
		if (cumulativeGradeDistribution == 0)
			return 0;
		double xVals[13] = {0};
		double yVals[13] = {0};
		double xErr[13] = {0};
		double yErr[13] = {0};
		double xPrev = 0., yPrev = 0.;
		int j = 0;
		for (int i = 0; i < 13; i++) {
			TString grade = cumulativeGradeDistribution->GetXaxis()->GetBinLabel(i+1);
			if (gradeDistribution->GetBinContent(i+1) != 0) {
				xVals[j] = MyFunctions::GradeToQuality(grade);
//				std::cout << "xVals[j] = " << xVals[j] << std::endl;
				yVals[j] = cumulativeGradeDistribution->GetBinContent(i+1);
				yErr[j] = yVals[j] - yPrev;
				yPrev = yVals[j];
				if (j > 0)
					xErr[j-1] = xVals[j] - xPrev;
				else
					xErr[j] = 0.;
				xPrev = xVals[j];
				++j;
			}

		}
		cumulativeGraph = new CumulativeDistribution(gradeDistribution);
		cumulativeGraphInverse = new CumulativeDistributionInverse(cumulativeGraph);
//		cumulativeGraph = new TGraphAsymmErrors(j, xVals, yVals, 0, xErr);
//		cumulativeGraph->SetTitle("Cumulative Grade Distribution;Grade Quality Points;Percentile");
//		cumulativeGraph->SetMarkerSize(0.5);
//		cumulativeGraph->SetMarkerStyle(20);
//		cumulativeGraph->SetMarkerColor(kBlue);
//		cumulativeGraphInverse = new TGraphAsymmErrors(j, yVals, xVals, yErr);
//		cumulativeGraphInverse->SetTitle("Inverse Cumulative Grade Distribution;Percentile;Grade Quality Points");
//		cumulativeGraphInverse->SetMarkerSize(0.5);
//		cumulativeGraphInverse->SetMarkerStyle(20);
//		cumulativeGraphInverse->SetMarkerColor(kBlue);
	}
	return cumulativeGraph;
}
*/

CumulativeDistributionInverse* CourseGradeNormer::CumulativeGraphInverse() {
	if (0 == cumulativeGraphInverse) {
		cumulativeGraphInverse = new CumulativeDistributionInverse(CumulativeGraph());
	}
	return cumulativeGraphInverse;	
}