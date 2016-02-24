// This is a local rootlogon.C file
{
	printf("\n Beginning a new ROOT session\n");
	
	gROOT->SetStyle("Modern");
	// US Letter
	gStyle->SetPaperSize(20,24);
	gStyle->SetPalette(55, 0);
	
	
	// Load Learing Analytics shared library
	gSystem->Load("libLrnAna.so");
}