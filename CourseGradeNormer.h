#ifndef COURSEGRADENORMER
#define COURSEGRADENORMER value

#include <TGraph.h>
#include <TH1D.h>

// Light-weight structure for calculating course grade norming information
class CourseGradeNormer {
public:
	
	CourseGradeNormer();
	
	void AddGrade(TString grade);
	double Average() const;
	double StdDev() const;
	int Entries() const {return nEntries;}
	
	void MakeCumulativeDist();
	
	TH1D* GradeDistribution() {return gradeDistribution;}
	
	TH1D* CumulativeDistribution();
	
	TGraph* CumulativeGraph();
	
	TGraph* CumulativeGraphInverse();
	
private:
	int nEntries = 0;
	double sumQ = 0.;
	double sumQ2 = 0.;
	TH1D* gradeDistribution = 0;
	TH1D* cumulativeGradeDistribution = 0;
	TGraph* cumulativeGraph = 0;
	TGraph* cumulativeGraphInverse = 0;
	static int instance;
};

#endif