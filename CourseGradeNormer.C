#include "CourseGradeNormer.h"

#include "GradeHistogram.h"
#include "MyFunctions.h"

#include <TString.h>

int CourseGradeNormer::_instance = 0;

CourseGradeNormer::CourseGradeNormer() {
	++_instance;
	TString title;
	title.Form("grade_dist_%d", _instance);
	_gradeDistribution = new GradeHistogram(title, title);
}

void CourseGradeNormer::AddGrade(TString grade) {
	double qualityPoints = MyFunctions::GradeToQuality(grade);
	_nEntries++;
	_sumQ += qualityPoints;
	_sumQ2 += qualityPoints*qualityPoints;
	_gradeDistribution->Fill(grade, 1.);
}

double CourseGradeNormer::Average() const {
	if (_nEntries > 0)
		return _sumQ/_nEntries;
	else
		return 0.;
}

double CourseGradeNormer::StdDev() const {
	if (_nEntries > 1)
		return sqrt(_sumQ2/_nEntries - Average()*Average());
	else
		return 0.;
}

CumulativeDistribution* CourseGradeNormer::CumulativeGraph() {
	if (0 == _cumulativeGraph) {
		_cumulativeGraph = new CumulativeDistribution(_gradeDistribution);
	}
	return _cumulativeGraph;
}

CumulativeDistributionInverse* CourseGradeNormer::CumulativeGraphInverse() {
	if (0 == _cumulativeGraphInverse) {
		_cumulativeGraphInverse = new CumulativeDistributionInverse(CumulativeGraph());
	}
	return _cumulativeGraphInverse;	
}