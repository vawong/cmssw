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


void drawTimingMaps(std::string inputfile, std::string outputDir, std::string datasetInfo, bool printChannels){
  
  int nOutliers = 0;
  TFile *_file1 = new TFile(inputfile.c_str());
  gSystem->mkdir((outputDir).c_str(),true);
  gSystem->mkdir((outputDir+"/OutlierTimingPlot").c_str(),true);
  
  TProfile2D **hChTiming     = new TProfile2D*[3];
  TProfile2D **hChTimingWide = new TProfile2D*[3];
  
  TH2D       **hChOccupy     = new TH2D*[3];
  TH2D       **hRMS          = new TH2D*[3];
  TH2D       **hRMSWide      = new TH2D*[3];
  
  // make a loop to book the histograms
  for(int it = 0; it < 3; ++it){
    hChTiming[it]     = new TProfile2D(("hTimeDepth"+int2string(it+1)).c_str(),("hTimeDepth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
    hChTimingWide[it] = new TProfile2D(("hTimeWideDepth"+int2string(it+1)).c_str(),("hTimeWideDepth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
    hChOccupy[it]     = new TH2D(("hOccDepth"+int2string(it+1)).c_str(),("hOccDepth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5);
    hRMS[it] = new TH2D(("hRMS_Depth"+int2string(it+1)).c_str(),("hRMS_Depth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5);
    hRMSWide[it] = new TH2D(("hRMSWide_Depth"+int2string(it+1)).c_str(),("hRMSWide_Depth"+int2string(it+1)).c_str(),59,-29.5,29.5,72,0.5,72.5);
  }
  
  TProfile2D *depth1 = new TProfile2D("depth1","depth1",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  TProfile2D *depth2 = new TProfile2D("depth2","depth2",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  TProfile2D *depth3 = new TProfile2D("depth3","depth3",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  
  
  TH2D *occupancy1 = new TH2D("occupancy1","occupancy1",59,-29.5,29.5,72,0.5,72.5);
  TH2D *occupancy2 = new TH2D("occupancy2","occupancy2",59,-29.5,29.5,72,0.5,72.5);
  TH2D *occupancy3 = new TH2D("occupancy3","occupancy3",59,-29.5,29.5,72,0.5,72.5);
  
  TH2D *hRMS1 = new TH2D("hRMS1","hRMS1",59,-29.5,29.5,72,0.5,72.5);
  TH2D *hRMS2 = new TH2D("hRMS2","hRMS2",59,-29.5,29.5,72,0.5,72.5);
  TH2D *hRMS3 = new TH2D("hRMS3","hRMS3",59,-29.5,29.5,72,0.5,72.5);
   
  TH2D *energy1 = new TH2D("energy1","energy1",59,-29.5,29.5,72,0.5,72.5);
  TH2D *energy2 = new TH2D("energy2","energy2",59,-29.5,29.5,72,0.5,72.5);
  TH2D *energy3 = new TH2D("energy3","energy3",59,-29.5,29.5,72,0.5,72.5);
  
  TH2D *avgenergy1 = new TH2D("avgenergy1","avgenergy1",59,-29.5,29.5,72,0.5,72.5);
  TH2D *avgenergy2 = new TH2D("avgenergy2","energy2",59,-29.5,29.5,72,0.5,72.5);
  TH2D *avgenergy3 = new TH2D("avgenergy3","energy3",59,-29.5,29.5,72,0.5,72.5);
  
  TH1F *hTimeAll   = new TH1F("hTimeAll"  ,"hTimeAll"  ,50, -10, 10);
  TH1F *hTimePart1 = new TH1F("hTimePart1","hTimePart1",50, -10, 10);
  TH1F *hTimePart2 = new TH1F("hTimePart2","hTimePart2",50, -10, 10);
  TH1F *hTimePart3 = new TH1F("hTimePart3","hTimePart3",50, -10, 10);
  
  TH1F *hRMSdepth1 = new TH1F("hRMSdepth1","hRMSdepth1",50, 0, 25);
  TH1F *hRMSdepth2 = new TH1F("hRMSdepth2","hRMSdepth2",50, 0, 25);
  TH1F *hRMSdepth3 = new TH1F("hRMSdepth3","hRMSdepth3",50, 0, 25);
  
  TH2D *hTimeFromMeanD1 = new TH2D("hTimeFromMeanD1","hTimeFromMeanD1",59,-29.5,29.5,72,0.5,72.5);
  TH2D *hTimeFromMeanD2 = new TH2D("hTimeFromMeanD2","hTimeFromMeanD2",59,-29.5,29.5,72,0.5,72.5);
  TH2D *hTimeFromMeanD3 = new TH2D("hTimeFromMeanD3","hTimeFromMeanD3",59,-29.5,29.5,72,0.5,72.5);
  
  
  for(int j = 0; j < 3; ++j){
    hChTiming[j]     = (TProfile2D*)_file1->Get(("timingMaps/hHBHETiming_Depth"+int2string(j+1)).c_str());
    hChTimingWide[j] = (TProfile2D*)_file1->Get(("timingMaps/hHBHETiming_wide_Depth"+int2string(j+1)).c_str());
    hChOccupy[j]     = (TH2D*)_file1->Get(("timingMaps/occupancy_d"+int2string(j+1)).c_str());
  }
  depth1 = (TProfile2D*)_file1->Get("timingMaps/hHBHETiming_Depth1");
  depth2 = (TProfile2D*)_file1->Get("timingMaps/hHBHETiming_Depth2");
  depth3 = (TProfile2D*)_file1->Get("timingMaps/hHBHETiming_Depth3");
  
  occupancy1 = (TH2D*)_file1->Get("timingMaps/occupancy_d1");
  occupancy2 = (TH2D*)_file1->Get("timingMaps/occupancy_d2");
  occupancy3 = (TH2D*)_file1->Get("timingMaps/occupancy_d3");
  
  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  
  for(int d = 0; d < 2; ++d){ // loop over all 3 depths
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
        double RMS_wide = hChTimingWide[d]->GetBinError(x,y);//= hChTiming[d]_wide->GetBinError(x,y);
        hRMS[d]->SetBinContent(x,y,RMS);
        hRMSWide[d]->SetBinContent(x,y,RMS_wide);
        // print crap if the time is off from zero by a lot
        if(printChannels && time>5) std::cout << "  depth " << d+1 << ", ieta = " << ieta << ",  iphi = " << iphi << std::endl;
        if(time ==0) continue;
        hRMS1->SetBinContent(x,y,RMS);
        hTimeAll->Fill(time);
        if(y >= 3  && y <  27) hTimePart1->Fill(time);
        if(y >= 27 && y <  51) hTimePart2->Fill(time);
        if((y <  3  || y >= 51) && !(y==54&&ieta>0&&ieta<17&&d+1==1)) hTimePart3->Fill(time);
        
        
        // print histograms for any odd channel timings
        if(time > 5 || RMS_wide > 11) {
          TH1F *holdTime = new TH1F(("Outlier_Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str(),("Outlier_Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str(),75,-37.5,37.5);
          std::cout << "looking for... " << ("timingMaps/Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str() << std::endl;
          holdTime = (TH1F*)_file1->Get(("timingMaps/Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str());
          c1->cd();
          holdTime->SetTitle((datasetInfo+"_Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)).c_str());
          holdTime->Draw();
          c1->Print((outputDir+"/"+"OutlierTimingPlot"+"/Outlier_"+datasetInfo+"_Depth"+int2string(d+1)+"_ieta"+int2string(ieta)+"_iphi"+int2string(iphi)+".png").c_str());
          c1->Clear();
          delete holdTime;
        }
      } // End loop through ieta
    } // End loop through iphi
  } // End loop over depths
  // fill the depth 2 rms map
  
  double meanPart1 = 0;
  double meanPart2 = 0;
  double meanPart3 = 0;
  
  
  
  for(int y=1; y < depth1->GetNbinsY()+1; ++y){
    for(int x=1; x < depth1->GetNbinsX()+1; ++x){
      if(depth1->GetXaxis()->GetBinCenter(x) == 0) continue;
      if(fabs(depth1->GetXaxis()->GetBinCenter(x)) > 20 && ((int)depth1->GetYaxis()->GetBinCenter(y))%2==0) continue;
      double time = depth1->GetBinContent(x,y);
      if(y >= 3  && y <  27) hTimeFromMeanD1->SetBinContent(x,y,time-meanPart1);
      if(y >= 27 && y <  51) hTimeFromMeanD1->SetBinContent(x,y,time-meanPart2);
      if(y <  3  || y >= 51) hTimeFromMeanD1->SetBinContent(x,y,time-meanPart3);
      
      if(printChannels == true){
        if(/*fabs(time-meanPart1) > 1  && fabs(time-meanPart1) < 5.0 && */y >= 3  && y <  27) {std::cout << "depth 1, iphi " << depth1->GetYaxis()->GetBinCenter(y)
          << ", ieta " << depth1->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl; 
        }
        if(/*fabs(time-meanPart2) > 1  && fabs(time-meanPart2) < 5.0 && */y >= 27 && y <  51) {std::cout << "depth 1, iphi " << depth1->GetYaxis()->GetBinCenter(y)
          << ", ieta " << depth1->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
        if(/*fabs(time-meanPart3) > 1  && fabs(time-meanPart3) < 5.0 && */(y <  3  || y >= 51)) {std::cout << "depth 1, iphi " << depth1->GetYaxis()->GetBinCenter(y)
          << ", ieta " << depth1->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
      }
      if(fabs(time-meanPart1) > 5.0 && y >= 3  && y <  27) ++nOutliers;
      if(fabs(time-meanPart2) > 5.0 && y >= 27 && y <  51) ++nOutliers;
      if(fabs(time-meanPart3) > 5.0 && (y < 3 || y >= 51)) ++nOutliers;
    }
  }
  
  for(int y=1; y < depth2->GetNbinsY()+1; ++y){
//     std:: cout << "y " << y << "  phi " << depth2->GetYaxis()->GetBinCenter(y) << std::endl;
    for(int x=1; x < depth2->GetNbinsX()+1; ++x){
      if(fabs(depth2->GetXaxis()->GetBinCenter(x)) < 16 || fabs(depth2->GetXaxis()->GetBinCenter(x)) == 17) continue;
      if(fabs(depth2->GetXaxis()->GetBinCenter(x)) > 20 && ((int)depth2->GetYaxis()->GetBinCenter(y))%2==0) continue;
      double time = depth2->GetBinContent(x,y);
      if(y >= 3  && y <  27) hTimeFromMeanD2->SetBinContent(x,y,time-meanPart1);
      if(y >= 27 && y <  51) hTimeFromMeanD2->SetBinContent(x,y,time-meanPart2);
      if(y <  3  || y >= 51) hTimeFromMeanD2->SetBinContent(x,y,time-meanPart3);
      
      if(printChannels == true){
        if(/*fabs(time-meanPart1) > 1  && fabs(time-meanPart1) < 5.0  &&*/ y >= 3  && y <  27) {std::cout << "depth 2, iphi " << depth2->GetYaxis()->GetBinCenter(y)
          << ", ieta " << depth2->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
        if(/*fabs(time-meanPart2) > 1  && fabs(time-meanPart2) < 5.0  &&*/ y >= 27 && y <  51) {std::cout << "depth 2, iphi " << depth2->GetYaxis()->GetBinCenter(y)
          << ", ieta " << depth2->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
        if(/*fabs(time-meanPart3) > 1  && fabs(time-meanPart3) < 5.0  && */(y <  3  || y >= 51)) {std::cout << "depth 2, iphi " << depth2->GetYaxis()->GetBinCenter(y)
          << ", ieta " << depth2->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
      }
      
      if(fabs(time-meanPart1) > 5.0 && y >= 3  && y <  27) ++nOutliers;
      if(fabs(time-meanPart2) > 5.0 && y >= 27 && y <  51) ++nOutliers;
      if(fabs(time-meanPart3) > 5.0  && (y <  3  || y >= 51)) ++nOutliers;
    }
  }
  
  for(int y=1; y < depth3->GetNbinsY()+1; ++y){
    for(int x=1; x < depth3->GetNbinsX()+1; ++x){
      if(fabs(depth3->GetXaxis()->GetBinCenter(x)) < 16 ||( fabs(depth3->GetXaxis()->GetBinCenter(x)) > 16 && fabs(depth3->GetXaxis()->GetBinCenter(x)) < 27)) continue;
      if(fabs(depth3->GetXaxis()->GetBinCenter(x)) >= 27 && ((int)depth3->GetYaxis()->GetBinCenter(y))%2==0) continue;
      if(fabs(depth3->GetXaxis()->GetBinCenter(x)) > 28) continue;
      double time = depth3->GetBinContent(x,y);
      if(y >= 3  && y <  27) hTimeFromMeanD3->SetBinContent(x,y,time-meanPart1);
      if(y >= 27 && y <  51) hTimeFromMeanD3->SetBinContent(x,y,time-meanPart2);
      if(y <  3  || y >= 51) hTimeFromMeanD3->SetBinContent(x,y,time-meanPart3);
      
      if (printChannels == true){
        if(/*fabs(time-meanPart1) > 1  && fabs(time-meanPart1) < 5.0*//* &&*/ y >= 3  && y <  27) {std::cout << "depth 3, iphi " << depth3->GetYaxis()->GetBinCenter(y) 
          << ", ieta " << depth3->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
        if(/*fabs(time-meanPart2) > 1  && fabs(time-meanPart2) < 5.0 && */y >= 27 && y <  51) {std::cout << "depth 3, iphi " << depth3->GetYaxis()->GetBinCenter(y) 
          << ", ieta " << depth3->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
        if(/*fabs(time-meanPart3) > 1  && fabs(time-meanPart3) < 5.0  && */(y <  3  || y >= 51)) {std::cout << "depth 3, iphi " << depth3->GetYaxis()->GetBinCenter(y) 
          << ", ieta " << depth3->GetXaxis()->GetBinCenter(x) << ", time " << time << std::endl;
        }
      }
      
      if(fabs(time-meanPart1) > 5.0 && y >= 3  && y <  27) ++nOutliers;
      if(fabs(time-meanPart2) > 5.0 && y >= 27 && y <  51) ++nOutliers;
      if(fabs(time-meanPart3) > 5.0  && (y <  3  || y >= 51)) ++nOutliers;
    }
  }
  
  TCanvas *canv = new TCanvas("canv","canv",800,600);
  canv->cd();
  
  if(printChannels) std::cout << "Number of channels where time from 0 is > 5ns" << nOutliers << std::endl;
  
  
  // print all depths
  for(int depth = 0; depth < 2; ++depth){
    
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
    hChTiming[depth]->SetTitle(("HBHE Average Time, Depth "+int2string(depth+1)).c_str());
    hChTiming[depth]->Draw("colz");
    canv->Print((outputDir+"/"+datasetInfo+"_Depth"+int2string(depth+1)+"_AverageTime.png").c_str());
    canv->Clear();
    
    hChTimingWide[depth]->SetStats(kFALSE);
    hChTimingWide[depth]->GetXaxis()->SetTitle("ieta");
    hChTimingWide[depth]->GetYaxis()->SetTitle("iphi");
    hChTimingWide[depth]->GetZaxis()->SetRangeUser(-6.0, 6.0);
    hChTimingWide[depth]->SetTitle(("HBHE Average Time (wide window), Depth "+int2string(depth+1)).c_str());
    hChTimingWide[depth]->Draw("colz");
    canv->Print((outputDir+"/"+datasetInfo+"_Depth"+int2string(depth+1)+"_AverageTimeWideWindow.png").c_str());
    canv->Clear();
  }
  
  
  hRMS1->SetStats(kFALSE);
  hRMS1->GetXaxis()->SetTitle("ieta");
  hRMS1->GetYaxis()->SetTitle("iphi");
  hRMS1->SetTitle("HBHE Timing RMS, Depth 1");
  hRMS1->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeRMS.png").c_str());
  
  hRMS2->SetStats(kFALSE);
  hRMS2->GetXaxis()->SetTitle("ieta");
  hRMS2->GetYaxis()->SetTitle("iphi");
  hRMS2->SetTitle("HBHE Timing RMS, Depth 2");
  hRMS2->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth2_timeRMS.png").c_str());
  
  hRMS3->SetStats(kFALSE);
  hRMS3->GetXaxis()->SetTitle("ieta");
  hRMS3->GetYaxis()->SetTitle("iphi");
  hRMS3->SetTitle("HBHE Timing RMS, Depth 3");
  hRMS3->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth3_timeRMS.png").c_str());
  
  depth1->SetStats(kFALSE);
  depth1->GetXaxis()->SetTitle("ieta");
  depth1->GetYaxis()->SetTitle("iphi");
  depth1->GetZaxis()->SetRangeUser(-6.0, 6.0);
  depth1->SetTitle("HBHE Average Time, Depth 1");
  depth1->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeProf.png").c_str());
  
  depth1->SetStats(kFALSE);
  depth2->GetXaxis()->SetTitle("ieta");
  depth2->GetYaxis()->SetTitle("iphi");
  depth2->GetZaxis()->SetRangeUser(-6.0, 6.0);
  depth2->SetTitle("HBHE Average Time, Depth 2");
  depth2->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth2_timeProf.png").c_str());
  
  depth1->SetStats(kFALSE);
  depth3->GetXaxis()->SetTitle("ieta");
  depth3->GetYaxis()->SetTitle("iphi");
  depth3->GetZaxis()->SetRangeUser(-6.0, 6.0);
  depth3->SetTitle("HBHE Average Time, Depth 3");
  depth3->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth3_timeProf.png").c_str());
  
  hTimeAll->SetStats(kTRUE);
  hTimeAll->GetXaxis()->SetTitle("Average time per channel [ns]");
  hTimeAll->GetYaxis()->SetTitle("Number of channels");
  hTimeAll->SetTitle("All Channels");
  hTimeAll->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeHist_All.png").c_str());

  hTimePart1->SetStats(kTRUE);
  hTimePart1->GetXaxis()->SetTitle("Average time per channel [ns]");
  hTimePart1->GetYaxis()->SetTitle("Number of channels");
  hTimePart1->SetTitle("Channels with iphi 3-26");
  hTimePart1->Draw("");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeHist_Part1.png").c_str());
  
  hTimePart2->SetStats(kTRUE);
  hTimePart2->GetXaxis()->SetTitle("Average time per channel [ns]");
  hTimePart2->GetYaxis()->SetTitle("Number of channels");
  hTimePart2->SetTitle("Channels with iphi 27-50");
  hTimePart2->Draw("");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeHist_Part2.png").c_str());
  
  hTimePart3->SetStats(kTRUE);
  hTimePart3->GetXaxis()->SetTitle("Average time per channel [ns]");
  hTimePart3->GetYaxis()->SetTitle("Number of channels");
  hTimePart3->SetTitle("Channels with iphi 51-72, 1-2");
  hTimePart3->Draw("");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeHist_Part3.png").c_str());
  
  hRMSdepth1->SetStats(kTRUE);
  hRMSdepth1->GetXaxis()->SetTitle("RMS per channel");
  hRMSdepth1->GetYaxis()->SetTitle("Number of channels");
  hRMSdepth1->SetTitle("RMS, Depth 1");
  hRMSdepth1->Draw("");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeRMS_hist.png").c_str());
  
  hRMSdepth2->SetStats(kTRUE);
  hRMSdepth2->GetXaxis()->SetTitle("RMS per channel");
  hRMSdepth2->GetYaxis()->SetTitle("Number of channels");
  hRMSdepth2->SetTitle("RMS, Depth 2");
  hRMSdepth2->Draw("");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth2_timeRMS_hist.png").c_str());
  
  hRMSdepth3->SetStats(kTRUE);
  hRMSdepth3->GetXaxis()->SetTitle("RMS per channel");
  hRMSdepth3->GetYaxis()->SetTitle("Number of channels");
  hRMSdepth3->SetTitle("RMS, Depth 3");
  hRMSdepth3->Draw("");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth3_timeRMS_hist.png").c_str());
  
  hTimeFromMeanD1->SetStats(kFALSE);
  hTimeFromMeanD1->GetXaxis()->SetTitle("ieta");
  hTimeFromMeanD1->GetYaxis()->SetTitle("iphi");
  hTimeFromMeanD1->SetTitle("timing offset from mean");
  hTimeFromMeanD1->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth1_timeOffset_hist.png").c_str());
  
  hTimeFromMeanD2->SetStats(kFALSE);
  hTimeFromMeanD2->GetXaxis()->SetTitle("ieta");
  hTimeFromMeanD2->GetYaxis()->SetTitle("iphi");
  hTimeFromMeanD2->SetTitle("timing offset from mean");
  hTimeFromMeanD2->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth2_timeOffset_hist.png").c_str());
  
  hTimeFromMeanD3->SetStats(kFALSE);
  hTimeFromMeanD3->GetXaxis()->SetTitle("ieta");
  hTimeFromMeanD3->GetYaxis()->SetTitle("iphi");
  hTimeFromMeanD3->SetTitle("timing offset from mean");
  hTimeFromMeanD3->Draw("colz");
  canv->Print((outputDir+"/"+datasetInfo+"_Depth3_timeOffset_hist.png").c_str());
  
  
  // done
  return;
}
