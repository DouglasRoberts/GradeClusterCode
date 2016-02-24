#ifndef MakeCourseSections_h
#define MakeCourseSections_h

#include "StudentCoursesByTerm.h"
#include "GradeCluster.h"
#include <TString.h>

class MakeCourseSections : public StudentCoursesByTerm {
	
public:
	MakeCourseSections(TTree* tree=0) : StudentCoursesByTerm(tree){;}
	~MakeCourseSections(){;}
	int MakeSections(TString sectionFile = "Sections.root");
//	void DoClustering(int maxSize = -1);
	
//	int FindNearestNeighbors(std::vector<GradeCluster*> clusters, int& i, int&j, double& minDist);
	
private:
	

};

#endif