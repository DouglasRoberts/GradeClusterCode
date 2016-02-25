// This is the file of function related to doing the grading clusters code
#include <iostream>
#include "MakeCourseSections.h"
#include "MyFunctions.h"
#include "GradeCluster.h"
#include <TString.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TBenchmark.h>
#include <TSystem.h>
#include <TError.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <vector>

TString sectionFile = "Sections.root";

bool ClusterCompare(GradeCluster* lhs, GradeCluster* rhs) {
	double lhs_mean = lhs->gradeHist()->GetMean();
	double rhs_mean = rhs->gradeHist()->GetMean();
	return lhs_mean > rhs_mean;
}

void CreateSectionsFile() {

	MakeCourseSections secObject;
	
	int nSections = secObject.MakeSections(sectionFile);
	
	std::cout << "Number of sections = " << nSections << std::endl;
	
	return;
}

void PlotSectionStuff() {
	
	TFile* secFile = new TFile(sectionFile, "READ");
	TTree* secTree = (TTree*)secFile->Get("Sections");
	SectionInfo* secInfo = 0;
	secTree->SetBranchAddress("secInfo", &secInfo);
	
	secTree->Show(0);
	std::cout << "Course Name = " << secInfo->CrsName() << std::endl;
	std::cout << "Course Term = " << secInfo->Term() << std::endl;
	std::cout << "College     = " << secInfo->College() << ", " << secInfo->College().Length() << std::endl;
	std::cout << "Number of grades = " << secInfo->nRecords() << std::endl;
	secInfo->Hist()->DrawCopy();
	
	
	TFile* outFile = new TFile("SectionPlots.root", "RECREATE");
	TH1F* gradeHist = new TH1F("allGrades", "All Grades", 13, 0., 13.);
	TH1F* collegeHist = new TH1F("allColleges", "All College Distribution", 14, 0., 14.);
	TH1F* ayHist = new TH1F("allAY", "Academic Year Distribution", 18, 0., 18.);
	TH2F* grVayHist = new TH2F("grVayHist", "Grades vs. Academic Year", 18, 0., 18., 13, 0., 13.);
	TH2F* numgrVayHist = new TH2F("numgrVayHist", "Grade Quality Points vs. Academic Year", 18, 0., 10., 41*5, -.05, 4.05);
	TProfile* grVayProf = new TProfile("grVayProf", "Grade Quality Points vs. Academic Year", 18, 0., 10.);
	TH2F* numgrVcollegeHist = new TH2F("numgrVcollegeHist", "Grade Quality Points vs. College", 14, 0., 14., 41*5, -.05, 4.05);
	TProfile* grVcollegeProf = new TProfile("grVcollegeProf", "Grade Quality Points vs. College", 14, 0., 14.);
	
	MyFunctions::GradeLabels(gradeHist->GetXaxis());
	MyFunctions::CollegeLabels(collegeHist->GetXaxis());
	MyFunctions::AyLabels(ayHist->GetXaxis());
	MyFunctions::GradeLabels(grVayHist->GetYaxis());
	MyFunctions::AyLabels(grVayHist->GetXaxis());
	MyFunctions::AyLabels(numgrVayHist->GetXaxis());
	MyFunctions::AyLabels(grVayProf->GetXaxis());
	MyFunctions::CollegeLabels(numgrVcollegeHist->GetXaxis());
	MyFunctions::CollegeLabels(grVcollegeProf->GetXaxis());
	
	// Loop over all sections
	int nSections = secTree->GetEntries();
	for (int i = 0; i < nSections; i++) {
		secTree->GetEvent(i);
		gradeHist->Add(secInfo->Hist());
		TString thisCollege = secInfo->College();
		if (collegeHist->GetXaxis()->FindFixBin(secInfo->College()) == -1)
			thisCollege = "OTHR";		
		collegeHist->Fill(thisCollege, secInfo->Hist()->Integral());
		
		TString ay = secInfo->AcademicYear();
		ayHist->Fill(ay, 1.);
		for (int iBin = 1; iBin <= secInfo->Hist()->GetNbinsX(); iBin++) {
			TString gradeLabel = secInfo->Hist()->GetXaxis()->GetBinLabel(iBin);
			grVayHist->Fill(ay, gradeLabel, secInfo->Hist()->GetBinContent(iBin));
			// Convert gradeLabel to number quality point value
			double qualPoints = MyFunctions::GradeToQuality(gradeLabel);
			numgrVayHist->Fill(ay, qualPoints, secInfo->Hist()->GetBinContent(iBin));
			grVayProf->Fill(ay, qualPoints, secInfo->Hist()->GetBinContent(iBin));
			numgrVcollegeHist->Fill(thisCollege, qualPoints, secInfo->Hist()->GetBinContent(iBin));
			grVcollegeProf->Fill(thisCollege, qualPoints, secInfo->Hist()->GetBinContent(iBin));
		}
	}

	gradeHist->DrawCopy();
	collegeHist->DrawCopy();
	ayHist->DrawCopy();
	grVayHist->DrawCopy();
	numgrVayHist->DrawCopy("CANDLE");
	
	gradeHist->Write();
	collegeHist->Write();
	ayHist->Write();
	grVayHist->Write();
	numgrVayHist->Write();
	grVayProf->Write();
	
	numgrVcollegeHist->Write();
	grVcollegeProf->Write();
	
	outFile->Close();
	secFile->Close();
	
	return;
}

int FindNearestNeighbors(std::vector<GradeCluster*> clusters, int& iNearest, int& jNearest, double& distMin)
{
	iNearest = -1;
	jNearest = -1;
	distMin = 999999.;
	
	int length = clusters.size();
	if (length % 100 == 0)
		std::cout << "Length of cluster vector = " << length << std::endl;
	
	gErrorIgnoreLevel = kWarning;
	static double reallySmall = 0.4;
	double reallySmallIncrement = 0.1;
	
	static bool notReallySmallFirst = true;
	long long nPairs = 0;
	for (int i = 0; i < length - 1; i++) {
		TH1F* iHist = clusters[i]->gradeHist();
		for (int j = i + 1; j < length; j++) {
			TH1F* jHist = clusters[j]->gradeHist();
			double chi2 = iHist->Chi2Test(jHist, "UU CHI2/NDF");
			if (chi2 < distMin) {
				distMin = chi2;
				iNearest = i;
				jNearest = j;
			}
			nPairs++;
			if (distMin < reallySmall) return nPairs;			
		}
	}

	if (notReallySmallFirst) {
		notReallySmallFirst = false;
		std::cout << "First pass not invoking really small: " << length << ", " << distMin << std::endl;
	}
	
	// We just looped over entire set looking for min, and none were below the current "reallySmall" threshold.
	// If the list is larger than 1000 elements, bump up the "reallySmall" threshold.
	if (length > 1000) {
		reallySmall += reallySmallIncrement;
		std::cout << "New threshold is now " << reallySmall << std::endl;
	}
	
	return nPairs;
}


void DoGradingClusters(int maxSize = -1) {
	
	gBenchmark->Start("DoGradingClusters");
	
	// Read section information from root file created by method above
	TFile* secFile = new TFile(sectionFile, "READ");
	TTree* secTree = (TTree*)secFile->Get("Sections");
	SectionInfo* secInfo = 0;
	secTree->SetBranchAddress("secInfo", &secInfo);
	
	Long64_t nentries = secTree->GetEntriesFast();
	
	std::cout << "Entries in secTree = " << nentries << std::endl;
	
	int fallTerm = 8, springTerm = 1;
	int minEnroll = 10;
	
	std::vector<GradeCluster*> clusters;
	int nGoodSections = 0;
	
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		secTree->GetEntry(jentry);
		int termTerm = secInfo->Term() % 100;
		if (termTerm != fallTerm && termTerm != springTerm) continue;
		if (secInfo->nRecords() < minEnroll) continue;
		if (secInfo->Hist()->GetEntries() < minEnroll) continue;
		nGoodSections++;
		// Create a grade cluster for each good section to seed the clustering algorithm...
		GradeCluster* cluster = new GradeCluster(*secInfo);
		clusters.push_back(cluster);
		
		if (nGoodSections == maxSize) break;
	}
	std::cout << "Number of good sections used to seed clustering = " << nGoodSections << std::endl;
	std::cout << "Size of cluster vector = " << clusters.size() << std::endl;
	
	// Now we have a vector of grade clusters.
	// Find the two clusters that are nearest to each other
	int i, j;
	double minDist = 9999.;
	TCanvas* c2 = new TCanvas("c2", "c2", 1200, 800);
	TH1F* minChi2Hist = new TH1F("minChi2Hist", "Minimum Chi^2/DOF vs. Iteration Number", clusters.size(), 0., clusters.size());
	TH1F* nPairsHist = new TH1F("nPairsHist", "Number of Pairs considered vs.Iteration Number", clusters.size(), 0., clusters.size());
	c2->Divide(1,2);
	c2->cd(1);
	minChi2Hist->DrawCopy("HIST");
	c2->cd(2);
	nPairsHist->DrawCopy("HIST");
	c2->Modified();
	c2->Update();
	if (gSystem->ProcessEvents()) return;
	int countIters = 0;

	while (clusters.size() > 15) {
		
		int nPairs = FindNearestNeighbors(clusters, i, j, minDist);
		minChi2Hist->Fill(countIters, minDist);
		nPairsHist->Fill(countIters, nPairs);
		if (countIters % 100 == 0) {
			c2->cd(1);
			minChi2Hist->DrawCopy("HIST");
			c2->cd(2);
			nPairsHist->DrawCopy("HIST");
			c2->Modified();
			c2->Update();
			if (gSystem->ProcessEvents()) break;
		}
		// Merge two clusters
		clusters[i]->Merge(clusters[j]);
		delete clusters[j];
		clusters.erase(clusters.begin() + j);
		
		std::random_shuffle(clusters.begin(), clusters.end());
		
		countIters++;
	}
	
	// Sort final clusters
	std::sort(clusters.begin(), clusters.end(), ClusterCompare);
	
	c2->cd(1);
	minChi2Hist->DrawCopy("HIST");
	c2->cd(2);
	nPairsHist->DrawCopy("HIST");
	c2->Modified();
	c2->Update();
	
	std::cout << "Clusters size = " << clusters.size() << std::endl;
	std::cout << "Final minDist = " << minDist << std::endl;
	
	//Write clusters out to a file
	TFile* clusterFile = new TFile("Clusters.root", "RECREATE");
	TTree* clusterTree = new TTree("Clusters", "Grade Clusters");
	GradeCluster aCluster;
	clusterTree->Branch("cluster", &aCluster);
	for (std::size_t iCluster = 0; iCluster < clusters.size(); iCluster++) {
		aCluster = *(clusters[iCluster]);
		TString newTitle; newTitle.Form("Grading Cluster %d", (int)iCluster);
		aCluster.gradeHist()->SetTitle(newTitle);
//		aCluster.gradeHist()->DrawCopy();
		clusterTree->Fill();
	}
	
	TCanvas* c1 = new TCanvas("c1", "c1", 50, 50, 1200, 800);
	c1->Divide(3,5);
	for (int iclus = 0; iclus < 15; iclus++) {
		c1->cd(iclus + 1);
		clusters[iclus]->gradeHist()->DrawCopy();
	}

	clusterTree->Print();
	clusterTree->Scan();
	clusterTree->Write();
	clusterFile->Close();
	secFile->Close();
	gBenchmark->Show("DoGradingClusters");
	
	return;
}

void PlotClusters() {
	
	TFile* clusterFile = new TFile("Clusters.root", "READ");
	TTree* clusterTree = (TTree*)clusterFile->Get("Clusters");
	GradeCluster* aCluster = 0;
	clusterTree->SetBranchAddress("cluster", &aCluster);
	Long64_t nentries = clusterTree->GetEntriesFast();
	std::cout << "Entries in clusterTree = " << nentries << std::endl;
	
	TFile* outFile = new TFile("ClusterPlots.root", "RECREATE");
	
	TCanvas* c1 = new TCanvas("c1", "All Clusters", 50, 50, 788, 532);
	TCanvas* c2 = new TCanvas("c2", "Cluster Information", 100, 100, 788, 532);
	TCanvas* c3 = new TCanvas("c3", "Cluster AY Contributions", 100, 100, 788, 532);
	TCanvas* c4 = new TCanvas("c4", "Cluster College Contributions", 100, 100, 788, 532);
	TCanvas* c5 = new TCanvas("c5", "College Cluster Usage", 100, 100, 788, 532);
	TH1F* clusSizeSec = new TH1F("clusSizeSec", "Cluster Size in Sections", 15, -.5, 14.5);
	TH1F* clusSizeGrade = new TH1F("clusSizeGrade", "Cluster Size in Grades", 15, -.5, 14.5);
	c1->Divide(3,5);
	c3->Divide(3,5);
	c4->Divide(3,5);
	c5->Divide(3,5);
	
	TH1F* collegeHistAll = new TH1F("collegeHistAll", "All Clusters College Contribution", 14, 0., 14.);
	MyFunctions::CollegeLabels(collegeHistAll->GetXaxis());
	std::vector<TH1F*> collegeHists;

	// Create a histogram for each college to store what clusters they prefer.
	// Should do this more programmatically in the future...
	std::map<TString, TH1F*> collegeClusterMap;
	TString college;
	college = "AGNR";
	TH1F* AGNRClusHist = new TH1F("AGNRClusHist", "AGNR Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = AGNRClusHist;
	college = "ARCH";
	TH1F* ARCHClusHist = new TH1F("ARCHClusHist", "ARCH Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = ARCHClusHist;
	college = "ARHU";
	TH1F* ARHUClusHist = new TH1F("ARHUClusHist", "ARHU Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = ARHUClusHist;
	college = "BSOS";
	TH1F* BSOSClusHist = new TH1F("BSOSClusHist", "BSOS Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = BSOSClusHist;
	college = "BMGT";
	TH1F* BMGTClusHist = new TH1F("BMGTClusHist", "BMGT Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = BMGTClusHist;
	college = "CMNS";
	TH1F* CMNSClusHist = new TH1F("CMNSClusHist", "CMNS Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = CMNSClusHist;
	college = "EDUC";
	TH1F* EDUCClusHist = new TH1F("EDUCClusHist", "EDUC Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = EDUCClusHist;
	college = "ENGR";
	TH1F* ENGRClusHist = new TH1F("ENGRClusHist", "ENGR Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = ENGRClusHist;
	college = "JOUR";
	TH1F* JOURClusHist = new TH1F("JOURClusHist", "JOUR Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = JOURClusHist;
	college = "INFO";
	TH1F* INFOClusHist = new TH1F("INFOClusHist", "INFO Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = INFOClusHist;
	college = "SPHL";
	TH1F* SPHLClusHist = new TH1F("SPHLClusHist", "SPHL Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = SPHLClusHist;
	college = "PUAF";
	TH1F* PUAFClusHist = new TH1F("PUAFClusHist", "PUAF Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = PUAFClusHist;
	college = "UGST";
	TH1F* UGSTClusHist = new TH1F("UGSTClusHist", "UGST Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = UGSTClusHist;
	college = "OTHR";
	TH1F* OTHRClusHist = new TH1F("OTHRClusHist", "OTHR Cluster Usage", 15, -0.5, 14.5);
	collegeClusterMap[college] = OTHRClusHist;
	
	gStyle->SetOptStat("e");
	gStyle->SetHistMinimumZero(kTRUE);
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		clusterTree->GetEntry(jentry);
		std::cout << aCluster->Cluster().size() << std::endl;
		c1->cd(jentry + 1);
		aCluster->gradeHist()->SetFillColor(38);
		aCluster->gradeHist()->DrawCopy();
		aCluster->gradeHist()->Write();
	
		clusSizeSec->Fill(jentry, aCluster->Cluster().size());
		clusSizeGrade->Fill(jentry, aCluster->gradeHist()->Integral());
		
		TString clusNum; clusNum.Form("%d", (int)jentry);
		TString clusAyName = "clusAyName_" + clusNum;
		TString clusAyTitle = "Cluster " + clusNum + " Contiribution by AY";
		TH1F* clusAyHist = new TH1F(clusAyName, clusAyTitle, 18, 0., 18.);
		TString collegeHistName = "collegHist_" + clusNum;
		TString collegeHistTitle = "Cluster " + clusNum + "Contributions by College";
		TH1F* collegeHist = new TH1F(collegeHistName, collegeHistTitle, 14, 0., 14.);
		MyFunctions::CollegeLabels(collegeHist->GetXaxis());
//		TPie* collegePie = new TPie(pieName, pieTitle);
		MyFunctions::AyLabels(clusAyHist->GetXaxis());
		for (auto section : aCluster->Cluster()) {
			clusAyHist->Fill(section.AcademicYear(), section.Hist()->Integral());
			if (collegeHist->GetXaxis()->FindFixBin(section.College()) != -1) {
				collegeHist->Fill(section.College(), section.Hist()->Integral());
				collegeHistAll->Fill(section.College(), section.Hist()->Integral());
				collegeClusterMap[section.College()]->Fill(jentry, section.Hist()->Integral());
			}	
			else {
				collegeHist->Fill("OTHR", section.Hist()->Integral());
				collegeHistAll->Fill("OTHR", section.Hist()->Integral());
				collegeClusterMap["OTHR"]->Fill(jentry, section.Hist()->Integral());
				
			}
		}
		c3->cd(jentry + 1);
		clusAyHist->DrawCopy();
		clusAyHist->Write();
//		c4->cd(jentry + 1);
//		collegeHist->DrawCopy();
//		collegeHist->Write();
		collegeHists.push_back((TH1F*)(collegeHist->Clone()));
	}
	c2->cd();
	c2->Divide(1,2);
	c2->cd(1);
	clusSizeSec->SetMinimum(0.);
	clusSizeSec->DrawCopy("HIST");
	c2->cd(2);
	clusSizeGrade->SetMinimum(0.);
	clusSizeGrade->DrawCopy("HIST");
	
	for (int ipad = 0; ipad < 15; ipad++) {
		c4->cd(ipad + 1);
		TH1F* hist = collegeHists[ipad];
		hist->Divide(hist, collegeHistAll, 1., 1., "B");
		hist->DrawCopy("E1");
		hist->Write();
	}
	
	int ipad = 0;
	for (auto i : collegeClusterMap) {
		ipad++;
		c5->cd(ipad);
		i.second->SetFillColor(38);
		i.second->DrawCopy("HIST");
		i.second->Write();
	}
	
	c1->Write();
	c2->Write();
	c3->Write();
	c4->Write();
	c5->Write();
	clusSizeSec->Write();
	clusSizeGrade->Write();
	outFile->Close();
	clusterFile->Close();
	
	return;
}

