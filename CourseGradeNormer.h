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
	int Entries() const {return _nEntries;}
	
	GradeHistogram* GradeDistribution() {return _gradeDistribution;}
	
	CumulativeDistribution* CumulativeGraph();
	
	CumulativeDistributionInverse* CumulativeGraphInverse();
	
private:
	int _nEntries = 0;
	double _sumQ = 0.;
	double _sumQ2 = 0.;
	GradeHistogram* _gradeDistribution = 0;
	CumulativeDistribution* _cumulativeGraph = 0;
	CumulativeDistributionInverse* _cumulativeGraphInverse = 0;
	static int _instance;
	mutable double _average;
	mutable double _stddev;
	mutable bool _goodAvg;
	mutable bool _goodStd;
};

#endif