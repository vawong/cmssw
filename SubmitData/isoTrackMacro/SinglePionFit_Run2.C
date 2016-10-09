#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "TProfile.h"
#include "TGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TH1.h"
#include "TStyle.h"
#include "TAxis.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
#include <string>
#include <vector>
#include <math.h>
#include <iostream>
using namespace RooFit ;

void setParam(TF1 * function , TH1 * histo){

  function->SetParNames("Mean","Sigma","Area");
  function->SetParameters( histo->GetMean(), histo->GetRMS(), histo->Integral());
  function->SetParLimits(0, histo->GetMean() - 0.5* histo->GetRMS() , histo->GetMean() + 0.5* histo->GetRMS() );
  function->SetParLimits(1, 0.5*histo->GetRMS(), 2*histo->GetRMS());
  
}

void SinglePionFit_Run2(){


  //TFile *f1 = TFile::Open("$CMSSW_BASE/src/Calibration/HcalCalibAlgos/test/output_1-100GeV.root");
TFile *f1 = TFile::Open("$CMSSW_BASE/src/Calibration/HcalCalibAlgos/test/output_500GeV_noCrossTalk.root");

TTree* t1 = (TTree*)f1->Get("HcalIsoTrkAnalyzer/CalibTree");


Double_t t_p, t_eMipDR, t_eHcal, ratio;
Int_t t_ieta;

t1->SetBranchAddress("t_p", &t_p);
t1->SetBranchAddress("t_eMipDR", &t_eMipDR);
t1->SetBranchAddress("t_eHcal", &t_eHcal);
t1->SetBranchAddress("t_ieta", &t_ieta);

TH1D *r_HB = new TH1D("r_HB","p_{Track} > 5 GeV",60,0.0,3.0);
TH1D *r525_HB = new TH1D("r525_HB","5 < p_{Track} < 25 GeV",60,0.0,3.0);
TH1D *r2550_HB = new TH1D("r2550_HB","25 < p_{Track} < 50 GeV",60,0.0,3.0);
TH1D *r5075_HB = new TH1D("r5075_HB","50 < p_{Track} < 75 GeV",60,0.0,3.0);
TH1D *r75inf_HB = new TH1D("r75inf_HB","p_{Track} > 75 GeV",60,0.0,3.0);

TH1D *r_HE = new TH1D("r_HE","p_{Track} > 5 GeV",60,0.0,3.0);
TH1D *r525_HE = new TH1D("r525_HE","5 < p_{Track} < 25 GeV",60,0.0,3.0);
TH1D *r2550_HE = new TH1D("r2550_HE","25 < p_{Track} < 50 GeV",60,0.0,3.0);
TH1D *r5075_HE = new TH1D("r5075_HE","50 < p_{Track} < 75 GeV",60,0.0,3.0);
TH1D *r75inf_HE = new TH1D("r75inf_HE","p_{Track} > 75 GeV",60,0.0,3.0);

//std::cout << "nentries = " << t1->GetEntries() << endl;
for(UInt_t i=0; i < t1->GetEntries(); i++) {
    t1->GetEntry(i);
    //cout << "t_p = " << t_p << "\t t_eHcal = " << t_eHcal << "\tt_eMipDR = " << t_eMipDR << endl;
    {
        if (t_p>5.0 && t_eHcal>1.0)
        {
        ratio = t_eHcal/(t_p - t_eMipDR);
        if(ratio>0.3)
            {       
                if (abs(t_ieta) < 15)   r_HB->Fill(ratio);
                if (abs(t_ieta) > 16)   r_HE->Fill(ratio);
            }
        }
        if (t_p>5.0 && t_p<25.0 && t_eHcal>1.0)
        {
        ratio = t_eHcal/(t_p - t_eMipDR);
        if(ratio>0.3)
            {       
                if (abs(t_ieta) < 15)   r525_HB->Fill(ratio);
                if (abs(t_ieta) > 16)   r525_HE->Fill(ratio);
            }
        }
        if (t_p>25.0 && t_p<50.0 && t_eHcal>10.0)
        {
        ratio = t_eHcal/(t_p - t_eMipDR);
        if(ratio>0.3) 
            {       
                if (abs(t_ieta) < 15)   r2550_HB->Fill(ratio);
                if (abs(t_ieta) > 16)   r2550_HE->Fill(ratio);
            }
        }
        if (t_p>50.0 && t_p<75.0 && t_eHcal>10.0)
        {
        ratio = t_eHcal/(t_p - t_eMipDR);
        if(ratio>0.3)   
            {       
                if (abs(t_ieta) < 15)   r5075_HB->Fill(ratio);
                if (abs(t_ieta) > 16)   r5075_HE->Fill(ratio);
            }
        }
        if (t_p>75.0 && t_eHcal>10.0)
        {
        ratio = t_eHcal/(t_p - t_eMipDR);
        if(ratio>0.3)    
            {       
                if (abs(t_ieta) < 15)   r75inf_HB->Fill(ratio);
                if (abs(t_ieta) > 16)   r75inf_HE->Fill(ratio);
                //cout << "ratio75 " << ratio << endl;
            }
        }
    }
    
}

//double integral = r->Integral(0,100);
//cout << "total area is" << integral << endl;

TF1 *gf_HB = new TF1("gf_HB","[2]*TMath::Gaus(x,[0],[1],0)",0.3,1.4);
TF1 *gf525_HB = new TF1("gf525_HB","[2]*TMath::Gaus(x,[0],[1],0)",0.3,1.4);
TF1 *gf2550_HB = new TF1("gf2550_HB","[2]*TMath::Gaus(x,[0],[1],0)",0.5,1.5);
TF1 *gf5075_HB = new TF1("gf5075_HB","[2]*TMath::Gaus(x,[0],[1],0)",0.5,1.5);
TF1 *gf75inf_HB = new TF1("gf75inf_HB","[2]*TMath::Gaus(x,[0],[1],0)",0.5,1.5);

TF1 *gf_HE = new TF1("gf_HE","[2]*TMath::Gaus(x,[0],[1],0)",0.3,1.4);
TF1 *gf525_HE = new TF1("gf525_HE","[2]*TMath::Gaus(x,[0],[1],0)",0.0,1.4);
TF1 *gf2550_HE = new TF1("gf2550_HE","[2]*TMath::Gaus(x,[0],[1],0)",0.5,1.5);
TF1 *gf5075_HE = new TF1("gf5075_HE","[2]*TMath::Gaus(x,[0],[1],0)",0.5,1.5);
TF1 *gf75inf_HE = new TF1("gf75inf_HE","[2]*TMath::Gaus(x,[0],[1],0)",1,2.5);

 setParam(gf_HB, r_HB);
 setParam(gf_HE, r_HE);

 setParam(gf525_HB, r525_HB);
 setParam(gf525_HE, r525_HE);

 setParam(gf2550_HB, r2550_HB);
 setParam(gf2550_HE, r2550_HE);

 setParam(gf5075_HB, r5075_HB);
 setParam(gf5075_HE, r5075_HE);

 setParam(gf75inf_HB, r75inf_HB);
 setParam(gf75inf_HE, r75inf_HE);


TCanvas *tmp = new TCanvas("tmp","",600,600); // For some stupid reason ROOT just plots the histogram when doing fit and the gStyle can't apply to the last histogram being fit.
tmp->cd();
r_HB->Fit("gf_HB","RBO");
r_HE->Fit("gf_HE","RBO");
r525_HB->Fit("gf525_HB","RBO");
r2550_HB->Fit("gf2550_HB","RBO");
r5075_HB->Fit("gf5075_HB","RBO");
r75inf_HB->Fit("gf75inf_HB","RBO");
r525_HE->Fit("gf525_HE","RBO");
r2550_HE->Fit("gf2550_HE","RBO");
r5075_HE->Fit("gf5075_HE","RBO");
r75inf_HE->Fit("gf75inf_HE","RBO");
delete tmp;

/*
RooRealVar x ("x","x",0.3,3.0);
RooDataHist ep("ep","ep",x, Import(*r));
RooRealVar mean ("mean","mean",0.95,0.85,1.15);
RooRealVar width ("width", "width", 0.1,0.01,1.0);
RooGaussian gauss("gauss","gauss",x,mean,width);
gauss.fitTo(ep);
*/

gStyle->SetStatStyle(0); 
gStyle->SetOptStat(2210);
gStyle->SetOptFit(111);
gStyle->SetStatY(0.9);
gStyle->SetStatX(0.9);
gStyle->SetStatW(0.2);
gStyle->SetStatH(0.15); 
gStyle->SetStatBorderSize(0); 


TCanvas *c = new TCanvas("c","c",800,600);
c->cd(1);
//RooPlot* frame = x.frame( Title("Ratio")) ;
//gauss.plotOn(frame,LineColor(kBlue));
//ep.plotOn(frame);
r525_HB->SetLineColor(1);
r525_HB->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r525_HB->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r525_HB->GetXaxis()->SetTitleSize(0.05);
r525_HB->GetXaxis()->SetRangeUser(0,3.0);
r525_HB->Draw("E");
//gf->Draw("same");
//fDraw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_525_HB.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r2550_HB->SetLineColor(1);
r2550_HB->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r2550_HB->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r2550_HB->GetXaxis()->SetTitleSize(0.05);
r2550_HB->GetXaxis()->SetRangeUser(0,3.0);
//r2550_HB->SetTitle("ratio");
r2550_HB->SetTitleSize(0.05);
r2550_HB->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_2550_HB.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r5075_HB->SetLineColor(1);
r5075_HB->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r5075_HB->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r5075_HB->GetXaxis()->SetTitleSize(0.05);
r5075_HB->GetXaxis()->SetRangeUser(0,3.0);
//r5075_HB->SetTitle("ratio");
r5075_HB->SetTitleSize(0.05);
r5075_HB->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_5075_HB.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r75inf_HB->SetLineColor(1);
r75inf_HB->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r75inf_HB->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r75inf_HB->GetXaxis()->SetTitleSize(0.05);
r75inf_HB->GetXaxis()->SetRangeUser(0,3.0);
//r75inf_HB->SetTitle("ratio");
r75inf_HB->SetTitleSize(0.05);
r75inf_HB->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_75inf_HB.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
//RooPlot* frame = x.frame( Title("Ratio")) ;
//gauss.plotOn(frame,LineColor(kBlue));
//ep.plotOn(frame);
r525_HE->SetLineColor(1);
r525_HE->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r525_HE->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r525_HE->GetXaxis()->SetTitleSize(0.05);
r525_HE->GetXaxis()->SetRangeUser(0,3.0);
r525_HE->Draw("E");
//gf->Draw("same");
//fDraw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_525_HE.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r2550_HE->SetLineColor(1);
r2550_HE->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r2550_HE->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r2550_HE->GetXaxis()->SetTitleSize(0.05);
r2550_HE->GetXaxis()->SetRangeUser(0,3.0);
//r2550_HE->SetTitle("ratio");
r2550_HE->SetTitleSize(0.05);
r2550_HE->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_2550_HE.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r5075_HE->SetLineColor(1);
r5075_HE->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r5075_HE->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r5075_HE->GetXaxis()->SetTitleSize(0.05);
r5075_HE->GetXaxis()->SetRangeUser(0,3.0);
//r5075_HE->SetTitle("ratio");
r5075_HE->SetTitleSize(0.05);
r5075_HE->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_5075_HE.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r75inf_HE->SetLineColor(1);
r75inf_HE->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r75inf_HE->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r75inf_HE->GetXaxis()->SetTitleSize(0.05);
r75inf_HE->GetXaxis()->SetRangeUser(0,3.0);
//r75inf_HE->SetTitle("ratio");
r75inf_HE->SetTitleSize(0.05);
r75inf_HE->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p_75inf_HE.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r_HB->SetLineColor(1);
r_HB->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r_HB->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r_HB->GetXaxis()->SetTitleSize(0.05);
r_HB->GetXaxis()->SetRangeUser(0,3.0);
//r_HB->SetTitle("ratio");
r_HB->SetTitleSize(0.05);
r_HB->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p__HB.png");
delete c;

c = new TCanvas("c","c",800,600);
c->cd(1);
r_HE->SetLineColor(1);
r_HE->SetLineWidth(2);
//r->GetYaxis()->SetRangeUser(0.,250.);
r_HE->GetXaxis()->SetTitle("eHCAL/(p-eECAL)");
r_HE->GetXaxis()->SetTitleSize(0.05);
r_HE->GetXaxis()->SetRangeUser(0,3.0);
//r_HE->SetTitle("ratio");
r_HE->SetTitleSize(0.05);
r_HE->Draw("E");
//gf->Draw("same");
//frame->Draw();
gPad->SetLeftMargin(0.15);
c->SaveAs("Run2_M2_fit_p__HE.png");
delete c;

ofstream fitResults("Run2_FitResults.txt");
if (!fitResults.is_open()) cout << "Not open\n";
fitResults << "Run2 Fit Results \n"
            << "HB\n" 
            << "525 \t Mean: " << gf525_HB->GetParameter(0) << " +/- " << gf525_HB->GetParError(0)
                << "\t Sigma: " << gf525_HB->GetParameter(1) << " +/- " << gf525_HB->GetParError(1)
                << "\t Resolution: " << gf525_HB->GetParameter(1)/gf525_HB->GetParameter(0) << " +/- " << gf525_HB->GetParameter(1)/gf525_HB->GetParameter(0)*sqrt(pow(gf525_HB->GetParError(1)/gf525_HB->GetParameter(1),2)+ pow(gf525_HB->GetParError(0)/gf525_HB->GetParameter(0),2)) << endl 
            << "2550 \t Mean: " << gf2550_HB->GetParameter(0) << " +/- " << gf2550_HB->GetParError(0)
                << "\t Sigma: " << gf2550_HB->GetParameter(1) << " +/- " << gf2550_HB->GetParError(1)
                << "\t Resolution: " << gf2550_HB->GetParameter(1)/gf2550_HB->GetParameter(0) << " +/- " << gf2550_HB->GetParameter(1)/gf2550_HB->GetParameter(0)*sqrt(pow(gf2550_HB->GetParError(1)/gf2550_HB->GetParameter(1),2)+ pow(gf2550_HB->GetParError(0)/gf2550_HB->GetParameter(0),2)) << endl 
            << "5075 \t Mean: " << gf5075_HB->GetParameter(0) << " +/- " << gf5075_HB->GetParError(0)
                << "\t Sigma: " << gf5075_HB->GetParameter(1) << " +/- " << gf5075_HB->GetParError(1)
                << "\t Resolution: " << gf5075_HB->GetParameter(1)/gf5075_HB->GetParameter(0) << " +/- " << gf5075_HB->GetParameter(1)/gf5075_HB->GetParameter(0)*sqrt(pow(gf5075_HB->GetParError(1)/gf5075_HB->GetParameter(1),2)+ pow(gf5075_HB->GetParError(0)/gf5075_HB->GetParameter(0),2)) << endl 
            << "75inf \t Mean: " << gf75inf_HB->GetParameter(0) << " +/- " << gf75inf_HB->GetParError(0)
                << "\t Sigma: " << gf75inf_HB->GetParameter(1) << " +/- " << gf75inf_HB->GetParError(1)
                << "\t Resolution: " << gf75inf_HB->GetParameter(1)/gf75inf_HB->GetParameter(0) << " +/- " << gf75inf_HB->GetParameter(1)/gf75inf_HB->GetParameter(0)*sqrt(pow(gf75inf_HB->GetParError(1)/gf75inf_HB->GetParameter(1),2)+ pow(gf75inf_HB->GetParError(0)/gf75inf_HB->GetParameter(0),2)) << endl

            << "HE\n" 
            << "525 \t Mean: " << gf525_HE->GetParameter(0) << " +/- " << gf525_HE->GetParError(0)
                << "\t Sigma: " << gf525_HE->GetParameter(1) << " +/- " << gf525_HE->GetParError(1)
                << "\t Resolution: " << gf525_HE->GetParameter(1)/gf525_HE->GetParameter(0) << " +/- " << gf525_HE->GetParameter(1)/gf525_HE->GetParameter(0)*sqrt(pow(gf525_HE->GetParError(1)/gf525_HE->GetParameter(1),2)+ pow(gf525_HE->GetParError(0)/gf525_HE->GetParameter(0),2)) << endl
            << "2550 \t Mean: " << gf2550_HE->GetParameter(0) << " +/- " << gf2550_HE->GetParError(0)
                << "\t Sigma: " << gf2550_HE->GetParameter(1) << " +/- " << gf2550_HE->GetParError(1)
                << "\t Resolution: " << gf2550_HE->GetParameter(1)/gf2550_HE->GetParameter(0) << " +/- " << gf2550_HE->GetParameter(1)/gf2550_HE->GetParameter(0)*sqrt(pow(gf2550_HE->GetParError(1)/gf2550_HE->GetParameter(1),2)+ pow(gf2550_HE->GetParError(0)/gf2550_HE->GetParameter(0),2)) << endl
            << "5075 \t Mean: " << gf5075_HE->GetParameter(0) << " +/- " << gf5075_HE->GetParError(0)
                << "\t Sigma: " << gf5075_HE->GetParameter(1) << " +/- " << gf5075_HE->GetParError(1)
                << "\t Resolution: " << gf5075_HE->GetParameter(1)/gf5075_HE->GetParameter(0) << " +/- " << gf5075_HE->GetParameter(1)/gf5075_HE->GetParameter(0)*sqrt(pow(gf5075_HE->GetParError(1)/gf5075_HE->GetParameter(1),2)+ pow(gf5075_HE->GetParError(0)/gf5075_HE->GetParameter(0),2)) << endl
            << "75inf \t Mean: " << gf75inf_HE->GetParameter(0) << " +/- " << gf75inf_HE->GetParError(0)
                << "\t Sigma: " << gf75inf_HE->GetParameter(1) << " +/- " << gf75inf_HE->GetParError(1)
                << "\t Resolution: " << gf75inf_HE->GetParameter(1)/gf75inf_HE->GetParameter(0) << " +/- " << gf75inf_HE->GetParameter(1)/gf75inf_HE->GetParameter(0)*sqrt(pow(gf75inf_HE->GetParError(1)/gf75inf_HE->GetParameter(1),2)+ pow(gf75inf_HE->GetParError(0)/gf75inf_HE->GetParameter(0),2)) << endl << endl
            << "Vectors: \n" 
            << "Mean HB: {" << gf525_HB->GetParameter(0) << ", " << gf2550_HB->GetParameter(0) << ", " <<  gf5075_HB->GetParameter(0) << ", " <<  gf75inf_HB->GetParameter(0) << "};\n"
            << "Mean HE: {" << gf525_HE->GetParameter(0) << ", " << gf2550_HE->GetParameter(0) << ", " <<  gf5075_HE->GetParameter(0) << ", " <<  gf75inf_HE->GetParameter(0) << "};\n"
            << "Error HB: {" << gf525_HB->GetParError(0) << ", " << gf2550_HB->GetParError(0) << ", " <<  gf5075_HB->GetParError(0) << ", " <<  gf75inf_HB->GetParError(0) << "};\n"
            << "Error HE: {" << gf525_HE->GetParError(0) << ", " << gf2550_HE->GetParError(0) << ", " <<  gf5075_HE->GetParError(0) << ", " <<  gf75inf_HE->GetParError(0) << "};\n"
            << "Sigma HB: {" << gf525_HB->GetParameter(1) << ", " << gf2550_HB->GetParameter(1) << ", " <<  gf5075_HB->GetParameter(1) << ", " <<  gf75inf_HB->GetParameter(1) << "};\n"
            << "Sigma HE: {" << gf525_HE->GetParameter(1) << ", " << gf2550_HE->GetParameter(1) << ", " <<  gf5075_HE->GetParameter(1) << ", " <<  gf75inf_HE->GetParameter(1) << "};\n"
            << "Error Sigma HB: {" << gf525_HB->GetParError(1) << ", " << gf2550_HB->GetParError(1) << ", " <<  gf5075_HB->GetParError(1) << ", " <<  gf75inf_HB->GetParError(1) << "};\n"
            << "Error Sigma HE: {" << gf525_HE->GetParError(1) << ", " << gf2550_HE->GetParError(1) << ", " <<  gf5075_HE->GetParError(1) << ", " <<  gf75inf_HE->GetParError(1) << "};\n"
            << "Resolution HB: {" << gf525_HB->GetParameter(1)/gf525_HB->GetParameter(0) << ", "  << gf2550_HB->GetParameter(1)/gf2550_HB->GetParameter(0) << ", " << gf5075_HB->GetParameter(1)/gf5075_HB->GetParameter(0) << ", " << gf75inf_HB->GetParameter(1)/gf75inf_HB->GetParameter(0) << "};\n" 
            << "Resolution HE: {" << gf525_HE->GetParameter(1)/gf525_HE->GetParameter(0) << ", "  << gf2550_HE->GetParameter(1)/gf2550_HE->GetParameter(0) << ", " << gf5075_HE->GetParameter(1)/gf5075_HE->GetParameter(0) << ", " << gf75inf_HE->GetParameter(1)/gf75inf_HE->GetParameter(0) << "};\n" 
            << "Resolution Error HB: {" <<  gf525_HB->GetParameter(1)/gf525_HB->GetParameter(0)*sqrt(pow(gf525_HB->GetParError(1)/gf525_HB->GetParameter(1),2)+ pow(gf525_HB->GetParError(0)/gf525_HB->GetParameter(0),2)) << ", " << gf2550_HB->GetParameter(1)/gf2550_HB->GetParameter(0)*sqrt(pow(gf2550_HB->GetParError(1)/gf2550_HB->GetParameter(1),2)+ pow(gf2550_HB->GetParError(0)/gf2550_HB->GetParameter(0),2)) << ", " << gf5075_HB->GetParameter(1)/gf5075_HB->GetParameter(0)*sqrt(pow(gf5075_HB->GetParError(1)/gf5075_HB->GetParameter(1),2)+ pow(gf5075_HB->GetParError(0)/gf5075_HB->GetParameter(0),2)) << ", " << gf75inf_HB->GetParameter(1)/gf75inf_HB->GetParameter(0)*sqrt(pow(gf75inf_HB->GetParError(1)/gf75inf_HB->GetParameter(1),2)+ pow(gf75inf_HB->GetParError(0)/gf75inf_HB->GetParameter(0),2)) << "};\n"
            << "Resolution Error HE: {" <<  gf525_HE->GetParameter(1)/gf525_HE->GetParameter(0)*sqrt(pow(gf525_HE->GetParError(1)/gf525_HE->GetParameter(1),2)+ pow(gf525_HE->GetParError(0)/gf525_HE->GetParameter(0),2)) << ", " << gf2550_HE->GetParameter(1)/gf2550_HE->GetParameter(0)*sqrt(pow(gf2550_HE->GetParError(1)/gf2550_HE->GetParameter(1),2)+ pow(gf2550_HE->GetParError(0)/gf2550_HE->GetParameter(0),2)) << ", " << gf5075_HE->GetParameter(1)/gf5075_HE->GetParameter(0)*sqrt(pow(gf5075_HE->GetParError(1)/gf5075_HE->GetParameter(1),2)+ pow(gf5075_HE->GetParError(0)/gf5075_HE->GetParameter(0),2)) << ", " << gf75inf_HE->GetParameter(1)/gf75inf_HE->GetParameter(0)*sqrt(pow(gf75inf_HE->GetParError(1)/gf75inf_HE->GetParameter(1),2)+ pow(gf75inf_HE->GetParError(0)/gf75inf_HE->GetParameter(0),2)) << "};\n";
    
fitResults.close(); 
}
//
