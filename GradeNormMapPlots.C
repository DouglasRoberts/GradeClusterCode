#include "CourseGradeNormer.h"
#include "CumulativeDistribution.h"
#include "MyFunctions.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TString.h>

#include <iostream>
#include <vector>

void GradeNormMapPlots(TString course = "MATH140", int term = 0) {
	
	MyFunctions::BuildGradeNormMap();
	
	CourseGradeNormer& cgn = MyFunctions::gradeNormMap.at(std::make_pair(course, term));
	TCanvas* c1 = new TCanvas("c1", "Grade Norm Stuff", 1600, 1200);
	c1->Divide(1,3);
	c1->cd(1);
	cgn.GradeDistribution()->DrawCopy();
//	c1->cd(2);
//	cgn.CumulativeDistribution()->DrawCopy();
	c1->cd(2);
	cgn.CumulativeGraph()->Draw("AP");
	c1->cd(3);
	cgn.CumulativeGraphInverse()->Draw("AP");
		
	return;
}

void PracticeAdding() {
	
	MyFunctions::BuildGradeNormMap();

	std::vector<std::pair<TString, int>> courses = {std::make_pair("BMGT110B", 0), std::make_pair("CPSP118B", 0),
	std::make_pair("EDCP108O", 0), std::make_pair("ENGL101S", 0), std::make_pair("MATH141", 0), 
	std::make_pair("PSYC100", 0), std::make_pair("CPSP118B", 0)};
//	std::vector<std::pair<TString, int>> courses = {std::make_pair("MATH002", 199908), std::make_pair("PHIL140", 199908),
//					std::make_pair("BMGT110A", 199908), std::make_pair("EDCP108O", 199908), std::make_pair("ENGL101M", 199908)};
	
	TCanvas* c1 = new TCanvas("c1", "Adding Inverses", 1600, 1200);
	c1->Divide(3,3);
	int pad = 1;
	
	std::vector<std::pair<CumulativeDistributionInverse*, double>> list;
	double totalCredits = 0.;
	for (auto course : courses) {
		CourseGradeNormer& cgn = MyFunctions::gradeNormMap.at(course);
		CumulativeDistributionInverse* inv = cgn.CumulativeGraphInverse();
		c1->cd(pad);
		inv->Draw("AP");
		++pad;
		list.push_back(std::make_pair(inv, 3.));
	}
	CumulativeDistributionInverse* sumGraph = new CumulativeDistributionInverse();
	sumGraph->Add(list);
	c1->cd(pad++);
	sumGraph->Draw("AP");
	c1->cd(pad++);
	TGraph* temp = (TGraph*)sumGraph->Cdf().Clone();
	temp->Draw("AP");
	
	
	return;
}