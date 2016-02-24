#ifndef SECTIONINFO_H
#define SECTIONINFO_H

#include <TObject.h>
#include <TString.h>
#include <vector>
#include <TH1F.h>
#include "MyFunctions.h"

class SectionInfo : public TObject {
public:
	SectionInfo() : _gradeHist(0) { }
	~SectionInfo() { }
	
	struct GrdRecord {
		TString grade;
		TString gradeMethod;
	};
	
	TString CrsName() const {return _crsName;}
	int SecNum() const {return _secNum;}
	int Term() const {return _term;}
	TString AcademicYear() {return MyFunctions::academicYear(_term);}
	TString College() const {return _collegeSD;}
	std::vector<GrdRecord> Grades() const {return _grades;}
	TH1F* HistClone() const {return (TH1F*)(_gradeHist->Clone());}
	TH1F* Hist() const {return _gradeHist;}
	
	void Initialize(TString course, int section, int term, TString college);
	void SetCrsName(TString name) {_crsName = name;}
	void SetSecNum(int sec) {_secNum = sec;}
	void SetTerm(int term) {_term = term;}
	void SetCollege(TString collegeSD) {_collegeSD = collegeSD;}
	
	void AddRecord(GrdRecord record);
	int nRecords() const {return _grades.size();}
	
private:
	TString _crsName;
	int _secNum;
	int _term;
	std::vector<GrdRecord> _grades;
	TH1F* _gradeHist;
	TString _collegeSD;
	
	ClassDef(SectionInfo,1)
};
#endif