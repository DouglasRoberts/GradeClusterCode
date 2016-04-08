#include "GradeHistogram.h"

#include "MyFunctions.h"

#include "TString.h"

GradeHistogram::GradeHistogram(TString name, TString title) : TH1D(name, title, 13, 0., 13.) {
	MyFunctions::GradeLabels(this->GetXaxis());
	this->SetDirectory(0);
}

