#include "CumulativeDistribution.h"

#include "GradeHistogram.h"
#include "MyFunctions.h"

#include <cassert>

CumulativeDistribution::CumulativeDistribution(GradeHistogram* hist) {
	assert(0 != hist);
	assert(hist->GetMinimum() >= 0.);
	// First, make histogram version of cdf
	double integral = hist->Integral();
	assert(integral > 0.);  // Only makes sense if integral is positive.
	
	double x[13] = {0.};
	double y[13] = {0.};
	double e[13] = {0.};
	double sum = 0.;
	int j = 0;
	for (int i = 1; i <= 13; ++i) {
		if (hist->GetBinContent(i) > 0.) {
			TString grade = hist->GetXaxis()->GetBinLabel(i);
			sum += hist->GetBinContent(i)/integral;
			x[j] = MyFunctions::GradeToQuality(grade);
			y[j] = sum;
			++j;
		}
	}
	this->Set(j);
	for (int i = 0; i < j; ++i) {
		if (i == j - 1)
			e[i] = 0.;
		else
			e[i] = x[i + 1] - x[i];
		this->SetPoint(i, x[i], y[i]);
		this->SetPointEXhigh(i, e[i]);
	}
	SetStyle();
}

CumulativeDistribution::CumulativeDistribution(const CumulativeDistributionInverse* inv) {
	// Construct cdf from it's inverse
	assert(0 != inv);
	
	double* P = inv->GetX();
	double* Q = inv->GetY();
	
	// Just flip x <-> y
	this->Set(inv->GetN());
	for (int i = 0; i < inv->GetN(); ++i) {
		this->SetPoint(i, Q[i], P[i]);
		if (i == inv->GetN() - 1)
			this->SetPointEXhigh(i, 0.);
		else
			this->SetPointEXhigh(i, Q[i + 1] - Q[i]);
	}
	SetStyle();
}

double CumulativeDistribution::Evaluate(double q) const {
	
//	return this->Eval(q);
	
	// Find first point greater than q, then return p of previous point
	double* Q = this->GetX();
	double* P = this->GetY();
	for (int i = 0; i < this->GetN(); ++i) {
		if (q < Q[i])  { 
			if (i > 0)
				return P[i-1];
			else 
				assert(0);
		}
	}
	// Should never get here!  (Or should we?  Test is <, not <=)
//	std::cout << "Shouldn't be here, last point, x = (" << X[cdf->GetN()-1] << ", " << Y[cdf->GetN()-1] << ") : " << x << std::endl;
	return Q[this->GetN()-1];
}

void CumulativeDistribution::SetStyle() {
	SetTitle("Cumulative Grade Distribution;Grade Quality Points;Percentile");
	SetMarkerSize(0.5);
	SetMarkerStyle(20);
	SetMarkerColor(kRed);
//	GetXaxis()->SetLimits(0., 4.3);
//	GetYaxis()->SetLimits(0., 1.0);	
}


CumulativeDistributionInverse::CumulativeDistributionInverse(CumulativeDistribution* cdf) {
	assert(0 != cdf);
	
	double* Q = cdf->GetX();
	double* P = cdf->GetY();
	
	// Just flip x <-> y
	this->Set(cdf->GetN());
	for (int i = 0; i < cdf->GetN(); ++i) {
		this->SetPoint(i, P[i], Q[i]);
		if (i == 0)
			this->SetPointEXlow(i, P[i] - 0.);
		else
			this->SetPointEXlow(i, P[i] - P[i-1]);
	}
	SetStyle();	
}

CumulativeDistributionInverse::CumulativeDistributionInverse() {
	SetStyle();
}

void CumulativeDistributionInverse::SetStyle() {
	SetTitle("Inverse Cumulative Distribution;Percentile;Grade Quality Points");
	SetMarkerSize(0.5);
	SetMarkerStyle(20);
	SetMarkerColor(kMagenta);
//	GetYaxis()->SetLimits(0., 4.3);
//	GetXaxis()->SetLimits(0., 1.0);	
}

void CumulativeDistributionInverse::SetErrors() {
	double* x = GetX();
	SetPointEXlow(0, x[0]);
	for (int i = 1; i < this->GetN(); ++i) {
		SetPointEXlow(i, x[i] - x[i - 1]);
	}
}

double CumulativeDistributionInverse::Evaluate(double p) const {
	
	double* x = GetX();
	double* y = GetY();
	int n = GetN();
	for (int i = 0; i < n; ++i) {
		if (p <= x[i])
			return y[i];
	}
	return y[n - 1];
}

void CumulativeDistributionInverse::Add(std::vector<std::pair<CumulativeDistributionInverse*, double>> list) {
	assert(this->GetN() == 0);
	
	// Iterate over things in list and put them into a vector
	double scaleTotal = 0.;
//	std::vector<std::pair<double, double>> points;
	std::vector<double> newX;
	std::vector<double> newY;
	for (auto thing : list) {
		double* x = thing.first->GetX();
		int n = thing.first->GetN();
		double scale = thing.second;
		scaleTotal += scale;
		for (int i = 0; i < n; ++i) {
			newX.push_back(x[i]);
//			points.push_back(std::make_pair(x[i], 0.));
		}
	}
	// Now sort points
	std::sort(newX.begin(), newX.end());
//	std::sort(points.begin(), points.end());
	// Iterate again to evaluate
	for (double x : newX) {
//	for (auto point : points) {
		double yVal = 0.;
		for (auto thing : list) {
			double val = thing.first->Evaluate(x);
			val *= thing.second;
			yVal += val;
//			yVal += thing.second*(thing.first->Evaluate(point.first));
		}
//		newX.push_back(point.first);
		newY.push_back(yVal);
	}
	Set(newX.size());
	for (unsigned long i = 0; i < newX.size(); ++i) {
		SetPoint(i, newX[i], newY[i]);
	}
	Scale(1./scaleTotal);
	SetErrors();
	SetStyle();
}

void CumulativeDistributionInverse::Add(CumulativeDistributionInverse* other, double scale) {
	
	// Adds a cdf^-1 function to this one, multiplied by an optional scale factor
	
	if (this->GetN() == 0) {
		*this = *other;
		this->Scale(scale);
	}
	else {
		double* x1 = this->GetX();
		double* x2 = other->GetX();
		double* y1 = this->GetY();
		double* y2 = other->GetY();
		std::vector<std::pair<double, double>> points;
		for (int i = 0; i < this->GetN(); ++i)
			points.push_back(std::make_pair(x1[i], y1[i]));
		for (int i = 0; i < other->GetN(); ++i)
			points.push_back(std::make_pair(x2[i], y2[i]));
		
		std::sort(points.begin(), points.end());
		std::vector<double> newX;
		std::vector<double> newY;
		for (auto point : points) {
			double y = this->Evaluate(point.first) + scale * other->Evaluate(point.first);
			newX.push_back(point.first);
			newY.push_back(y);
		}
		this->Set(newX.size());
		for (unsigned long i = 0; i < newX.size(); ++i) {
			this->SetPoint(i, newX[i], newY[i]);
		}
		
	}
	this->SetErrors();
	
	return;
}

void CumulativeDistributionInverse::Scale(double scale) {
	for (int i = 0; i < this->GetN(); ++i) {
		double x, y;
		this->GetPoint(i, x, y);
		this->SetPoint(i, x, scale*y);
	}
}
