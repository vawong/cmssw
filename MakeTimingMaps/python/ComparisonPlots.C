#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphErrors.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <TPaveStats.h>
#endif



void ComparisonPlots() {

  	TFile* f1 = TFile::Open("test_v0.root");			//open the root file

  	TDirectoryFile* d1 = (TDirectoryFile*)f1->Get("comparisonPlots");  //read the directory


  	TTree* t1 = (TTree*)d1->Get("HcalTree");			//read the tree

  	double Method0Energy_CPU, Method0Energy_GPU;
    double RecHitEnergy, RecHitEnergyGPU;
    double RecHitTime_CPU, RecHitTime_GPU;
    double iEta, iEtaGPU;
    double iPhi, iPhiGPU; 
    float count[10] = {0};
    int depth, depthGPU;
    int timeSliceRange = 10;

  	t1->SetBranchAddress("Method0Energy", &Method0Energy_CPU);			//Get the variables needed from the tree
  	t1->SetBranchAddress("Method0EnergyGPU", &Method0Energy_GPU);
	t1->SetBranchAddress("RecHitTime", &RecHitTime_CPU);
	t1->SetBranchAddress("RecHitTimeGPU", &RecHitTime_GPU);
	t1->SetBranchAddress("iEta", &iEta);
	t1->SetBranchAddress("count", &count);


	TH1D* M0_CPU = new TH1D("M0_CPU","M0_CPUGPU comparison",100,0.,25.);	//book histograms 
	TH1D* M0_GPU = new TH1D("M0_GPU","",100,0.,25.);
	TH2D* sp_M0_HB = new TH2D("sp_M0_HB","M0_cpBarrel",100,0.,20.,100,0.,20.);
	TH2D* sp_M0_HE = new TH2D("sp_M0_HE","M0_cpEndcap",100,0.,20.,100,0.,20.);

  	for (UInt_t k=0; k<t1->GetEntries(); k++) {
	//  for (UInt_t k=0; k<10000; k++) {
    		t1->GetEntry(k);

  		// std::cout << "Method0Energy Readouts: " << Method0Energy_CPU << endl;
		
		M0_CPU->Fill(Method0Energy_CPU);
		M0_GPU->Fill(Method0Energy_GPU);

		// scatterplots
		if(fabs(iEta)<17 && fabs(iEtaGPU)<17) sp_M0_HB->Fill(Method0Energy_CPU,Method0Energy_GPU); // barrel
		else sp_M0_HE->Fill(Method0Energy_CPU,Method0Energy_GPU); // endcap
	}

	M0_CPU->GetXaxis()->SetTitle("M0_Energy");
	M0_CPU->GetYaxis()->SetTitle("Events");
	M0_CPU->SetLineColor(kMagenta);
	M0_CPU->SetLineWidth(5);
	M0_CPU->Draw("colz");
	gPad->Update();

	TPaveStats *tps1 = (TPaveStats*) M0_CPU->FindObject("stats");
	tps1->SetName("CPU Method 0");
	double X1 = tps1->GetX1NDC();
	double Y1 = tps1->GetY1NDC();
	double X2 = tps1->GetX2NDC();
	double Y2 = tps1->GetY2NDC();

	M0_GPU->GetXaxis()->SetTitle("M0_Energy");
	M0_GPU->GetYaxis()->SetTitle("Events");
	M0_GPU->SetLineColor(kBlue);
	TCanvas *c1 = new TCanvas();
	M0_GPU->Draw("");
	gPad->Update();

	TPaveStats *tps2 = (TPaveStats*) M0_GPU->FindObject("stats");
	tps2->SetX1NDC(X1);
	tps2->SetX2NDC(X2);
	tps2->SetY1NDC(Y1-(Y2-Y1));
	tps2->SetY2NDC(Y1);

	TCanvas *c3 = new TCanvas();
	M0_CPU->Draw();
	M0_GPU->Draw("same");
	tps1->Draw("same");
	tps2->Draw("same");
	c3->SaveAs("comp_M2.png");

	TCanvas *c4 = new TCanvas("c4","c4",800,600);                   //Plot histograms
    c4->cd(1);
    c4->SetLogz();
    c4->SetGrid();
    sp_M0_HB->GetXaxis()->SetTitle("M0_CPU");
    sp_M0_HB->GetYaxis()->SetTitle("M0_GPU");
    sp_M0_HB->Draw("colz");
    c4->SaveAs("sp_M0_HB.png");

    TCanvas *c5 = new TCanvas("c5","c5",800,600);
    c5->cd(1);
    c5->SetLogz();
    c5->SetGrid();
    sp_M0_HE->GetXaxis()->SetTitle("M0_CPU");
    sp_M0_HE->GetYaxis()->SetTitle("M0_GPU");
    sp_M0_HE->Draw("colz");
    c5->SaveAs("sp_M0_HE.png");
}
