#include "GradeCluster.h"

ClassImp(GradeCluster)
	
GradeCluster::GradeCluster(SectionInfo sect) {	
	_cluster.push_back(sect);
	_hist = sect.HistClone();
	_hist->SetTitle("Grading Cluster");
	return;
}

void GradeCluster::Merge(GradeCluster* otherCluster) {
	
	// Put other clusters Section Infos on to this one's list...
	for (auto &i : otherCluster->_cluster) {
		_cluster.push_back(i);
	}
	
	_hist->Add(otherCluster->gradeHist());
	
}
