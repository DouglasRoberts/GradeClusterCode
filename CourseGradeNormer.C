#include "CourseGradeNormer.h"

#include "GradeHistogram.h"
#include "MyFunctions.h"

#include <TString.h>

int CourseGradeNormer::_instance = 0;

CourseGradeNormer::CourseGradeNormer() : _average(0.), _stddev(0.), _goodAvg(false), _goodStd(false) {
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
	_goodAvg = false;
	_goodStd = false;
}

double CourseGradeNormer::Average() const {
	if (!_goodAvg) {
		if (_nEntries > 0)
			_average = _sumQ/_nEntries;
		else
			_average = 0.;

		_goodAvg = true;
	}
	return _average;
}

double CourseGradeNormer::StdDev() const {
	if (!_goodStd) {
		if (_nEntries > 1)
			_stddev = sqrt(_sumQ2/_nEntries - Average()*Average());
		else
			_stddev = 0.;
		_goodStd = true;
	}
	return _stddev;
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