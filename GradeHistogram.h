#ifndef GRADEHISTOGRAM_H
#define GRADEHISTOGRAM_H value

#include <TH1D.h>
class TString;
	
// Special histogram class used for grade distributions
class GradeHistogram : public TH1D {
public:
	GradeHistogram(TString name, TString title);
	
private:
	
};
#endif