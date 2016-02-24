#ifndef GRADECLUSTER_H
#define GRADECLUSTER_H

#include <TObject.h>
#include <TH1F.h>
#include <vector>
#include "SectionInfo.h"

// Grade cluster is basically a collection of section objects
class GradeCluster : public TObject {

public:
	GradeCluster() : _hist(0) { }
	GradeCluster(SectionInfo sect);
	~GradeCluster() { }
	
	void Merge(GradeCluster* otherCluster);
	
	std::vector<SectionInfo> Cluster() {return _cluster;}
	TH1F* gradeHist() {return _hist;}
	
	
	
private:

	std::vector<SectionInfo> _cluster;
	TH1F* _hist;
	
	ClassDef(GradeCluster,2)
};
#endif