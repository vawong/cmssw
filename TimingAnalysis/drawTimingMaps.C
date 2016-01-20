#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <TH1F.h>
#include <TFile.h>
#include <TROOT.h>
#include <TString.h>
#include <TSystem.h>
#include <Rtypes.h>
#include <TStyle.h>

#include <TMath.h>
#include <TAxis.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAttLine.h>
#include <TPaveText.h>
#include <TColor.h>

#include <TTree.h>
#include <TH2F.h>
#include <TH1D.h>
// #include <TH1.h>
#include <TProfile.h>
#include <TProfile2D.h>

#include <TGraph.h>
#include <TGraphErrors.h>

string int2string(int i) {
  stringstream ss;
  string ret;
  ss << i;
  ss >> ret;
  return ret;
}


void drawTimingMaps(std::string inputfile, std::string outputDir, std::string datasetInfo){
  
  // Read the specified input file
  TFile *_file1 = new TFile(inputfile.c_str());
  // Take care of making the output diretory (if it doesn't already exist)
  gSystem->mkdir((outputDir).c_str(),true);
  gSystem->mkdir((outputDir+"/OutlierTimingPlot").c_str(),true);
  
  // create some TProfile and 2-d plots
  TProfile2D **hChTiming     = new TProfile2D*[3];
  TProfile2D **hRatios       = new TProfile2D*[3];
  
  TH2D       **hChOccupy     = new TH2D*[3];
  TH2D       **hRMS          = new TH2D*[3];
  TH1D       **hRMSHist      = new TH1D*[3];
  TH1D       **hTimeHist     = new TH1D*[3];
  
  // make a loop to book the histograms
  for(int it = 0; it < 3; ++it){
    hChTiming[it]     = new TProfile2D(("hTimeDepth"+int2string(it+1)).c_str(),("hTimeDepth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
    hChOccupy[it]     = new TH2D(("hOccDepth"+int2string(it+1)).c_str(),("hOccDepth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5);
    hRMS[it]          = new TH2D(("hRMS_Depth"+int2string(it+1)).c_str(),("hRMS_Depth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5);
    
    hRMSHist[it] = new TH1D(("hRMS_Part"+int2string(it+1)).c_str(),("hRMS_Part"+int2string(it+1)).c_str(),50, 0, 25);
    hTimeHist[it] = new TH1D(("hTime_Part"+int2string(it+1)).c_str(),("hTime_Part"+int2string(it+1)).c_str(),50, -10, 10);
  }
  
  
  // make a bunch of histograms
  TH1F *hTimeAll   = new TH1F("hTimeAll"  ,"hTimeAll"  ,50, -15, 15);
  TH1F *hTimePart1 = new TH1F("hTimePart1","hTimePart1",50, -10, 10);
  TH1F *hTimePart2 = new TH1F("hTimePart2","hTimePart2",50, -10, 10);
  TH1F *hTimePart3 = new TH1F("hTimePart3","hTimePart3",50, -10, 10);
  
  TH1F *hRMSdepth1 = new TH1F("hRMSdepth1","hRMSdepth1",50, 0, 25);
  TH1F *hRMSdepth2 = new TH1F("hRMSdepth2","hRMSdepth2",50, 0, 25);
  TH1F *hRMSdepth3 = new TH1F("hRMSdepth3","hRMSdepth3",50, 0, 25);
  
  TH2D *hTimeFromMeanD1 = new TH2D("hTimeFromMeanD1","hTimeFromMeanD1",59,-29.5,29.5,72,0.5,72.5);
  TH2D *hTimeFromMeanD2 = new TH2D("hTimeFromMeanD2","hTimeFromMeanD2",59,-29.5,29.5,72,0.5,72.5);
  TH2D *hTimeFromMeanD3 = new TH2D("hTimeFromMeanD3","hTimeFromMeanD3",59,-29.5,29.5,72,0.5,72.5);
  
  TH1F *hAllRechitTimeHB = new TH1F("hAllRechitTimeHB","hAllRechitTimeHB", 60, -15.0, 15.0);
  TH1F *hAllRechitTimeHE = new TH1F("hAllRechitTimeHE","hAllRechitTimeHE", 60, -15.0, 15.0);
  
  
  // get the plots from the root file
  for(int j = 0; j < 3; ++j){
    hChTiming[j]     = (TProfile2D*)_file1->Get(("timingMaps/hHBHETiming_Depth"+int2string(j+1)).c_str());
    hChOccupy[j]     = (TH2D*)_file1->Get(("timingMaps/occupancy_d"+int2string(j+1)).c_str());
  }
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  
  for(int d = 0; d < 3; ++d){ // loop over all 3 depths
    for(int y=1; y < hChTiming[d]->GetNbinsY()+1; ++y){ // loop through iphi
      for(int x=1; x < hChTiming[d]->GetNbinsX()+1; ++x){ // loop through ieta
        int ieta = (int)hChTiming[d]->GetXaxis()->GetBinCenter(x); // get ieta value corresponding to x
        int iphi = (int)hChTiming[d]->GetYaxis()->GetBinCenter(y); // get iphi value corresponding to y
        // ---  skip channels which don't exist at each depth  --- 
        if(ieta == 0) continue;
        if((d+1==1) && fabs(ieta)>20 && (iphi%2==0)) continue;
        if((d+1==2) && ((fabs(ieta)<16 || fabs(ieta)==17) || (fabs(ieta)>20 && (iphi%2)==0))) continue;
        if((d+1==3) && ((fabs(ieta) < 16 ||( fabs(ieta) > 16 && fabs(ieta) < 27)) || (fabs(ieta) > 26 && iphi%2==0) || (fabs(ieta) > 28))) continue;
        // ------------ now proceed to do stuff --------------
        // get the time, rms, etc per channel
        double time = hChTiming[d]->GetBinContent(x,y);
        double RMS  = hChTiming[d]->GetBinError(x,y);
        hRMS[d]->SetBinContent(x,y,RMS);
        
        if(time ==0) continue;
        hTimeAll->Fill(time);
        
        // fill by partitions
        if(y >= 3  && y <  27) {
          hTimeHist[0]->Fill(time);
          hRMSHist[0]->Fill(RMS);
        }
        if(y >= 27 && y <  51) {
          hTimeHist[1]->Fill(time);
          hRMSHist[1]->Fill(RMS);
        }
        if((y <  3  || y >= 51) ) {
          hTimeHist[2]->Fill(time);
          hRMSHist[2]->Fill(RMS);
        }
        
        
//         print histograms for any channels which seem to be big outliers
        if(fabs(time) > 8.0 || (RMS>11)) {
          TH1F *hTemp = new TH1F(("OT_D"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str(),("OT_D"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str(),75,-37.5,37.5);
          hTemp = (TH1F*)_file1->Get(("timingMaps/Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str());
          c1->cd();
          hTemp->SetTitle((datasetInfo+" Depth="+int2string(d+1)+" ieta="+int2string(ieta)+" iphi="+int2string(iphi)).c_str());
          hTemp->Draw();
          c1->Print((outputDir+"/"+"OutlierTimingPlot"+"/Outlier_"+datasetInfo+"_Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)+".png").c_str());
          c1->Clear();
          delete hTemp;
        }
      } // End loop through ieta
    } // End loop through iphi
  } // End loop over depths
  // fill the depth 2 rms map
  
  
  // ================================================================================
  // ~~~~~~~~~~~~          Print plots                           ~~~~~~~~~~~~~~~~~~~~
  // --------------------------------------------------------------------------------
  
  TCanvas *canv = new TCanvas("canv","canv",800,600);
  canv->cd();
  
  // Loop over all three depths
  // one plot of each quantity per HBHE depth
  // here we just set up the axis labels, histogram title, and stuff like that
  // also print the histograms to .png file
  for(int depth = 0; depth < 3; ++depth){
    
    hChOccupy[depth]->SetStats(kFALSE);
    hChOccupy[depth]->GetXaxis()->SetTitle("ieta");
    hChOccupy[depth]->GetYaxis()->SetTitle("iphi");
    hChOccupy[depth]->SetTitle(("Occupancy, Depth "+int2string(depth+1)).c_str());
    hChOccupy[depth]->Draw("colz");
    canv->Print((outputDir+"/"+datasetInfo+"_Depth"+int2string(depth+1)+"_occupancy.png").c_str());
    canv->Clear();
    
    // Set some of the axes labels before drawing 
    hChTiming[depth]->SetStats(kFALSE);
    hChTiming[depth]->GetXaxis()->SetTitle("ieta");
    hChTiming[depth]->GetYaxis()->SetTitle("iphi");
    hChTiming[depth]->GetZaxis()->SetRangeUser(-6.0, 6.0);
    hChTiming[depth]->SetTitle(("HBHE Average Channel Timing, Depth "+int2string(depth+1) +" (Run 256677)").c_str());
    hChTiming[depth]->Draw("colz");
    canv->Print((outputDir+"/"+datasetInfo+"_Depth"+int2string(depth+1)+"_AverageTime.png").c_str());
    canv->Clear();    
    
    hRMS[depth]->SetStats(kFALSE);
    hRMS[depth]->GetXaxis()->SetTitle("ieta");
    hRMS[depth]->GetYaxis()->SetTitle("iphi");
    hRMS[depth]->GetZaxis()->SetRangeUser(0.0, 25.0);
    hRMS[depth]->SetTitle(("HBHE Time RMS, Depth "+int2string(depth+1)).c_str());
    hRMS[depth]->Draw("colz");
    canv->Print((outputDir+"/"+datasetInfo+"_Depth"+int2string(depth+1)+"_RMS.png").c_str());
    canv->Clear();
    
    hRMSHist[depth]->SetStats(kTRUE);
    hRMSHist[depth]->GetXaxis()->SetTitle("RMS");
    hRMSHist[depth]->GetYaxis()->SetTitle("Number of Channels");
    hRMSHist[depth]->GetZaxis()->SetRangeUser(0.0, 25.0);
    if(depth == 0) hRMSHist[depth]->SetTitle("Channels with iphi 3-26");
    if(depth == 1) hRMSHist[depth]->SetTitle("Channels with iphi 27-50");
    if(depth == 2) hRMSHist[depth]->SetTitle("Channels with iphi 51-72, 1-2");
    hRMSHist[depth]->Draw("colz");
    canv->Print((outputDir+"/"+datasetInfo+"_RMS_Partition"+int2string(depth+1)+".png").c_str());
    canv->Clear();
    
    hTimeHist[depth]->SetStats(kTRUE);
    hTimeHist[depth]->GetXaxis()->SetTitle("Average Time [ns]");
    hTimeHist[depth]->GetYaxis()->SetTitle("Number of Channels");
    hTimeHist[depth]->GetZaxis()->SetRangeUser(0.0, 25.0);
    if(depth == 0) hTimeHist[depth]->SetTitle("Channels with iphi 3-26");
    if(depth == 1) hTimeHist[depth]->SetTitle("Channels with iphi 27-50");
    if(depth == 2) hTimeHist[depth]->SetTitle("Channels with iphi 51-72, 1-2");
    hTimeHist[depth]->Draw();
    canv->Print((outputDir+"/"+datasetInfo+"_AverageTime_Partition"+int2string(depth+1)+".png").c_str());
    canv->Clear();
    
  }

  canv->SetLogy(kTRUE);
  hTimeAll->SetStats(kFALSE);
  hTimeAll->GetXaxis()->SetTitle("Average time per channel [ns]");
  hTimeAll->GetYaxis()->SetTitle("Number of channels");
  hTimeAll->SetTitle("Average Time of Individual HBHE Channels (Run 256677)");
  hTimeAll->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_timeHist_All.png").c_str());

  // done
  return;
}
