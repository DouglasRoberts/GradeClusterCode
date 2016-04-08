#ifndef COURSEGRADENORMER_H
#define COURSEGRADENORMER_H value

#include "CumulativeDistribution.h"
#include "GradeHistogram.h"

#include <TGraph.h>
#include <TH1D.h>
class TString;

// Light-weight structure for calculating course grade norming information
class CourseGradeNormer {
public:
	
	CourseGradeNormer();
	
	void AddGrade(TString grade);
	double Average() const;
	double StdDev() const;
	int Entries() const {return nEntries;}
	
//	void MakeCumulativeDist();
	
	GradeHistogram* GradeDistribution() {return gradeDistribution;}
	
//	TH1D* CumulativeDist();
	
	CumulativeDistribution* CumulativeGraph();
	
	CumulativeDistributionInverse* CumulativeGraphInverse();
	
private:
	int nEntries = 0;
	double sumQ = 0.;
	double sumQ2 = 0.;
	GradeHistogram* gradeDistribution = 0;
//	TH1D* cumulativeGradeDistribution = 0;
	CumulativeDistribution* cumulativeGraph = 0;
	CumulativeDistributionInverse* cumulativeGraphInverse = 0;
	static int instance;
};

#endif