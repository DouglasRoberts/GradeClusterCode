#ifndef CumulativeDistribution_h
#define CumulativeDistribution_h value

#include "GradeHistogram.h"

#include <TGraphAsymmErrors.h>
#include <TH1D.h>
#include <TString.h>

class CumulativeDistributionInverse;

class CumulativeDistribution : public TGraphAsymmErrors {
public:
	// Construct from a Grade Histogram Histogram
	CumulativeDistribution(GradeHistogram* hist);
	CumulativeDistribution(const CumulativeDistributionInverse* inv);
	
	double Evaluate(double q) const;
	
private:
	
};

class CumulativeDistributionInverse : public TGraphAsymmErrors {
public:
	CumulativeDistributionInverse();
	CumulativeDistributionInverse(CumulativeDistribution* cdf);
	
	double Evaluate(double p) const;
	
	void Add(CumulativeDistributionInverse* other, double scale = 1.);  // Add 'other' to this guy
	void Scale(double scale);
	
	CumulativeDistribution Cdf() const {return CumulativeDistribution(this);}
	
private:
	void SetStyle();
	void SetErrors();
};
#endif