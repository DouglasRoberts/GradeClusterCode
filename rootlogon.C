// This is a local rootlogon.C file
{
	printf("\n Beginning a new ROOT session\n");
	
	gROOT->SetStyle("Modern");
	// US Letter
	gStyle->SetPaperSize(20,24);
	gStyle->SetPalette(55, 0);
//	gStyle->SetOptStat("e");
	gStyle->SetHistMinimumZero(kTRUE);
//	gStyle->SetLabelSize(0.1);
//	gStyle->SetLabelOffset(0.01);
//	gStyle->SetTitleSize(0.1, "t");
	gROOT->ForceStyle();
	
	
	
	// Load Learing Analytics shared library
	gSystem->Load("libLrnAna.so");
}