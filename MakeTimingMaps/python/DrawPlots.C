
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                        // access to gROOT, entry point to ROOT system
#include <TSystem.h>                      // interface to OS
#include <TStyle.h>                       // class to handle ROOT plotting styles
#include <TFile.h>                        // file handle class
#include <TTree.h>                        // class to access ntuples
#include <TChain.h>                        // class to access ntuples
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TBenchmark.h>                   // class to track macro running statistics
#include <vector>                         // STL vector class
#include <iostream>                       // standard I/O
#include <iomanip>                        // functions to format standard I/O
#include <fstream>                        // functions for file I/O
#include <string>                         // C++ string class
#include <sstream> 
#include <ROOT/RDataFrame.hxx>
#endif

// #include <TRoot.h>

using namespace std;

void DrawPlots()
{
	ROOT::EnableImplicitMT();

	TString baseDir = "/afs/cern.ch/user/v/vawong/data/CMSSW_10_2_X_2018-06-28-2300/src/HBHETimingValidation/"; 
	TString fn = "Energy_Comp";

	auto filePtr = TFile::Open("test_v0.root");
	ROOT::RDataFrame d("comparisonPlots", filePtr);
	TTree *treePtr = nullptr;
	filePtr->GetObject("HcalTree", treePtr);
	ROOT::RDataFrame d2(*treePtr);
	//TFile* f1 = TFile::Open("test_v0.root");
	//TDirectoryFile* d1 = (TDirectoryFile*)f1->Get("comparisonPlots");
	int n = 1;

	auto c = new TCanvas();

	// auto df = RDataFrame("HcalTree", d1, {"Method0Energy", "Method0EnergyGPU", "RecHitTime", "RecHitTimeGPU"});
	auto h1 = d2.Histo2D({"Method0Energy_RecHitTime", "", 100, 0., 500.,100, 0., 500.}, "RecHitTime", "Method0Energy");
	auto h2 = d2.Histo2D({"Method0EnergyGPU_RecHitTimeGPU","",100,0.,500.,100, 0., 500.},"RecHitTime","Method0EnergyGPU");
	c->cd(1);
	h1->Draw();
	h2->Draw("SAME");

	c->SaveAs(Form(baseDir+"/ComparisonHistograms/"+fn+"_0_cor.png", n));
}