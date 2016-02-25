#define MakeCourseSections_cxx

#include "MakeCourseSections.h"
#include "GradeCluster.h"
#include "SectionInfo.h"
#include <iostream>
#include <map>
#include <tuple>
#include <TH1F.h>
#include <TTree.h>
#include <TFile.h>
#include <TError.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TBenchmark.h>

#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>

int MakeCourseSections::MakeSections(TString sectionFile)
{
	
	typedef std::tuple<TString, int, int> keyType;
	typedef std::map<keyType, SectionInfo> mapType;
	typedef std::map<TString, TString> prefixMapType;
	
	// Only look at courses that were 3 or more credits...
	int creditCut = 3;
	// Only look at courses taken for "Regular" grading method
	TString grMethod = "R";
	
	gBenchmark->Start("sql");
	// Try sql version
	TSQLServer* db = TSQLServer::Connect("sqlite://LearningAnalytics.db", "", "");
	if (0 == db) {
		std::cout << "Error connecting to database" << std::endl;
		return 0;
	}
	TSQLRow * row;
	TSQLResult* res;
	
	printf("\nList of all tables in LearningAnalytics.db\n");
	res = db->GetTables("LearningAnalytics");
	while ((row = res->Next())) {
		printf("%s\n", row->GetField(0));
		delete row;
	}
	delete res;
	
	printf("\nList all columns in student_courses_by_term table\n");
	res = db->GetColumns("LearningAnalytics", "student_courses_by_term");
	while ((row = res->Next())) {
		printf("%s\n", row->GetField(1));
		delete row;
	}
	delete res;
	
	TString query = "SELECT ";
	query += "s.CRS_CREDIT, ";			//0
	query += "s.CRS_PREFIX, ";			//1
	query += "s.CRS_GRD_METH_CD, ";		//2
	query += "s.COURSE, ";				//3
	query += "s.SECTION, ";				//4
	query += "s.TERM, ";				//5
	query += "s.CRS_GRADE, ";			//6
	query += "p.Coll ";					//7
	
	query += "FROM student_courses_by_term AS s ";
	query += "LEFT JOIN PrefixCollegeMap AS p ";
	query += "ON s.CRS_PREFIX = p.CrsPrefix ";
//	query += "LIMIT 100";
	res = db->Query(query.Data());
//	res = db->Query("SELECT s.CRS_CREDIT, s.CRS_PREFIX, p.Coll FROM student_courses_by_term AS s LEFT JOIN PrefixCollegeMap AS p ON s.CRS_PREFIX = p.CrsPrefix LIMIT 10");
//	res = db->Query("SELECT s.CRS_CREDIT, s.CRS_PREFIX FROM student_courses_by_term AS s LIMIT 10");
//	int nRows = res->GetRowCount();
	int nRows = 0;
	int nFields = res->GetFieldCount();
	
	while ((row = res->Next())) {
		nRows++;
		
		float credits = atof(row->GetField(0));
		TString prefix = row->GetField(1);
		TString grd_meth_cd = row->GetField(2);
		TString course = row->GetField(3);
		TString section = row->GetField(4);
		TString term = row->GetField(5);
		TString grade = row->GetField(6);
		TString college = row->GetField(7);
//		printf("CRS_CREDIT = %f, CRS_PREFIX = %s, COLLEGE = %s\n", credits, prefix.Data(), college.Data());
		
		if ((credits < creditCut) || (grd_meth_cd != grMethod)) {
			delete row;
			continue;
		}
		delete row;
		if (college == "") college = "UNKN";
//		keyType key(course, section, term);
	}
	delete res;
	std::cout << "Rows, Fields = " << nRows << ", " << nFields << std::endl;
	
	gBenchmark->Show("sql");
	
//	return 0;
	
	if (fChain == 0) return 0;
	
	gBenchmark->Start("TTree");
	TFile* file = new TFile("UmdLA.root");
	prefixMapType* prefixCollegeMap = 0;
	file->GetObject("PrefixCollegeMap", prefixCollegeMap);
	std::cout << "Prefix Map Entries = " << prefixCollegeMap->size() << std::endl;
	
	Long64_t nentries = fChain->GetEntriesFast();
	
	std::cout << "Entries in StudentCoursesByTerm = " << nentries << std::endl;
	
	
	mapType sectionMap;
	
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		fChain->GetEntry(jentry);
		
		// Apply some filters...
		if (CRS_CREDIT < creditCut) continue;
		if (*CRS_GRD_METH_CD != grMethod) continue;
		
		// Look for this course, section in the map...
		TString college;
		auto prefixIter = prefixCollegeMap->find(*CRS_PREFIX);
		if (prefixIter == prefixCollegeMap->end()) {
//			std::cout << "Can't find prefix " << *CRS_PREFIX << std::endl;
			college = "UNKN";
		}
		else
			college = prefixCollegeMap->at(*CRS_PREFIX);
		keyType key(*COURSE, SECTION, TERM);
		
		//Should insert new info if key isn't there.  Either way, returns iterator pointing to object with this key.
		SectionInfo info;
		auto insertPair = sectionMap.insert(std::make_pair(key, info));
		SectionInfo& thisInfo = insertPair.first->second;
		if (insertPair.second) {
			thisInfo.Initialize(*COURSE, SECTION, TERM, college);
		}
		SectionInfo::GrdRecord record;
		record.grade = *CRS_GRADE;
		record.gradeMethod = *CRS_GRD_METH_CD;
		thisInfo.AddRecord(record);
				
	}

	gBenchmark->Show("TTree");

	double avgEnroll = (double)nentries/(double)sectionMap.size();
	std::cout << "Average enrollment (?) = " <<  avgEnroll << std::endl;
	
	TFile* fOut = new TFile(sectionFile, "RECREATE");
	TH1F* h1 = new TH1F("h1", "Enrollments", 100, 0., 1000.);
	TTree* secTree = new TTree("Sections", "Grade Info on Sections");
	TH1F* gradeHist = new TH1F("grades", "Grades", 13, 0., 13.);
	
	MyFunctions::GradeLabels(gradeHist->GetXaxis());
		
	SectionInfo info;
	secTree->Branch("secInfo", &info);
	int maxEnrollment = 0;
	for (auto const item: sectionMap) {
		info = item.second;
		h1->Fill(info.nRecords());
		for (auto &i : info.Grades()) {
			if (gradeHist->GetXaxis()->FindFixBin(i.grade) != -1)
				gradeHist->Fill(i.grade, 1.);
		}
		maxEnrollment = (info.nRecords() > maxEnrollment ? info.nRecords() : maxEnrollment);
		secTree->Fill();
	}
	gradeHist->LabelsDeflate("X");
	std::cout << "maxEnrollment = " << maxEnrollment << std::endl;
	
	h1->DrawCopy();
	gradeHist->DrawCopy();
	
	h1->Write();
	gradeHist->Write();
	secTree->Write();
	fOut->Close();
	
//	delete fOut;
//	delete h1;
//	delete secTree;
//	delete gradeHist;
	
	return sectionMap.size();
}

/*
void GradingClusters::DoClustering(int maxSize) {
	
	
	gBenchmark->Start("DoClustering");
	
	// Read section information from root file created by method above
	TFile* fIn = new TFile("Sections.root", "READ");
	TTree* secTree = 0;
	fIn->GetObject("Sections", secTree);
	SectionInfo* info = 0;
	secTree->SetBranchAddress("secInfo", &info);
	
	Long64_t nentries = secTree->GetEntriesFast();
	
	std::cout << "Entries in secTree = " << nentries << std::endl;
	
	int fallTerm = 8, springTerm = 1;
	int minEnroll = 10;
	
	std::vector<GradeCluster*> clusters;
	int nGoodSections = 0;
	for (Long64_t jentry = 0; jentry < nentries; jentry++) {
		secTree->GetEntry(jentry);
		int termTerm = info->Term() % 100;
		if (termTerm != fallTerm && termTerm != springTerm) continue;
		if (info->nRecords() < minEnroll) continue;
		if (info->Hist()->GetEntries() < minEnroll) continue;
//		std::cout << "Records, entries = " << info->nRecords() << ", " << info->Hist()->GetEntries() << std::endl;
		nGoodSections++;
		
		// Create a grade cluster for each good section to seed the clustering algorithm...
		GradeCluster* cluster = new GradeCluster(info);
		clusters.push_back(cluster);
		
		if (nGoodSections == maxSize) break;
	}
	
	std::cout << "Number of good sections used to seed clustering = " << nGoodSections << std::endl;
	std::cout << "Size of cluster vector = " << clusters.size() << std::endl;
	
	// Now we have a vector of grade clusters.
	// Find the two clusters that are nearest to each other
	int i, j;
	double minDist;
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
	std::cout << "Made it here (1)" << std::endl;
	while (clusters.size() > 15) {
		std::cout << "Made it here (n)" << std::endl;
		
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
//		std::cout << "Returned from FindNearestNeighbors: " << std::endl;
//		std::cout << "\t nPairs = " << nPairs << std::endl;
//		std::cout << "\t i, j = " << i << ", " << j << std::endl;
//		std::cout << "\t minDist = " << minDist << std::endl;
		std::cout << "Made it here (m)" << std::endl;
		
		// Merge two clusters
		clusters[i]->Merge(clusters[j]);
		delete clusters[j];
		clusters.erase(clusters.begin() + j);
		std::cout << "Made it here (l)" << std::endl;
		
		std::random_shuffle(clusters.begin(), clusters.end());
		
		countIters++;
	}
	std::cout << "Made it here (2)" << std::endl;

	c2->cd(1);
	minChi2Hist->DrawCopy("HIST");
	c2->cd(2);
	nPairsHist->DrawCopy("HIST");
	c2->Modified();
	c2->Update();
	
	std::cout << "Clusters size = " << clusters.size() << std::endl;
	std::cout << "Final minDist = " << minDist << std::endl;

	TFile* fOut = new TFile("Clusters.root", "RECREATE");
	TTree* clusterTree = new TTree("Clusters", "Grade Clusters");
	GradeCluster* aCluster = 0;
	clusterTree->Branch("cluster", &aCluster);
	
	TCanvas* c1 = new TCanvas("c1", "c1", 50, 50, 1200, 800);
	c1->Divide(3,5);
	for (int iclus = 0; iclus < 15; iclus++) {
		c1->cd(iclus + 1);
		clusters[iclus]->gradeHist()->DrawCopy();
		aCluster = clusters[iclus];
		clusterTree->Fill();
	}
	
	fIn->Close();
	fOut->Write();
	fOut->Close();
	
	gBenchmark->Show("DoClustering");

	return;
}

int GradingClusters::FindNearestNeighbors(std::vector<GradeCluster*> clusters, int& iNearest, int& jNearest, double& distMin)
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
	std::cout << "1" << std::endl;
	long long nPairs = 0;
	for (int i = 0; i < length - 1; i++) {
		std::cout << "2" << std::endl;
		TH1F* iHist = clusters[i]->gradeHist();
		std::cout << "Entries in iHist = " << iHist << ", i = " << i << std::endl;
//		iHist->DrawCopy();
		std::cout << "3" << std::endl;
//		if (iHist->GetEntries() == 0)
		std::cout << "Entries in iHist = " << iHist << ", i = " << i << std::endl;

			std::cout << "Entries in iHist = " << iHist->GetEntries() << ", i = " << i << std::endl;
		std::cout << "4" << std::endl;
		for (int j = i + 1; j < length; j++) {
			TH1F* jHist = clusters[j]->gradeHist();
			std::cout << "5" << std::endl;
			std::cout << "Entries in jHist = " << jHist->GetEntries() << std::endl;
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

		std::cout << "Returning from FindNearestNeighbor, nPairs = " << nPairs << ", distMin = " << distMin << std::endl;
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

*/