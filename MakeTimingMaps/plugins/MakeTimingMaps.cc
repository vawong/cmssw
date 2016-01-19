// -*- C++ -*-
//
// Package:    HBHETimingValidation/MakeTimingMaps
// Class:      MakeTimingMaps
// 
/**\class MakeTimingMaps MakeTimingMaps.cc HBHETimingValidation/MakeTimingMaps/plugins/MakeTimingMaps.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Stephanie Brandt
//         Created:  Fri, 21 Aug 2015 11:42:17 GMT
//
//


// system include files
#include <memory>
#include <string>
#include <map>
#include <iostream>
using namespace std;

// user include files
#include "TTree.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TProfile2D.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameterMap.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class MakeTimingMaps : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MakeTimingMaps(const edm::ParameterSet&);
      ~MakeTimingMaps();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      std::string int2string(int i);
      
      void ClearVariables();
      void SaveHistToFile();
      
      double Method0Energy;
      double RecHitEnergy;
      double RecHitTime;
      double iEta;
      double iPhi; 
      int depth;
      int RunNumber;
      int EvtNumber;
      
      bool FillHBHE;
      
      string sHBHERecHitCollection;      
      edm::Service<TFileService> FileService;
      
      TTree    *OutputTree; 

      TProfile *hHBHETiming_SmallStats;
      
      TProfile2D *hHBHETiming_Depth1;
      TProfile2D *hHBHETiming_Depth2;
      TProfile2D *hHBHETiming_Depth3;
      
      TProfile2D *hHBHETimingRat_Depth1;
      TProfile2D *hHBHETimingRat_Depth2;
      TProfile2D *hHBHETimingRat_Depth3;
      
      TProfile2D *hHBHETiming_wide_Depth1;
      TProfile2D *hHBHETiming_wide_Depth2;
      TProfile2D *hHBHETiming_wide_Depth3;
      
      TH2F *hHBHEEnergy_Depth1;
      TH2F *hHBHEEnergy_Depth2;
      TH2F *hHBHEEnergy_Depth3;
      
      // pointer to pointer to pointers to make a 2-d array of histograms
      TH1F ***hTiming_Depth1 = new TH1F**[59];
      TH1F ***hTiming_Depth2 = new TH1F**[59];
      TH1F ***hTiming_Depth3 = new TH1F**[6];
      
      TH1F **hM2M0_HB_5_10 = new TH1F*[3];
      TH1F **hM2M0_HB_10_20 = new TH1F*[3];
      TH1F **hM2M0_HB_20 = new TH1F*[3];
      
      TH1F **hM2M0_HE_5_10 = new TH1F*[3];
      TH1F **hM2M0_HE_10_20 = new TH1F*[3];
      TH1F **hM2M0_HE_20 = new TH1F*[3];
      
      TH1F **hGoodTimeADC = new TH1F*[10];
      TH1F **hOffTimeADC = new TH1F*[10];
      TH1F **hSingleChanADC = new TH1F*[10];
      
      TH1F **hGoodChanEnergy = new TH1F*[10];
      TH1F **hBadChanEnergy = new TH1F*[10];
      
      int runNumber_;
      
      TH1F *hPart1;
      TH1F *hPart3;
      TH1F *hPart2;
      
      TH2F *occupancy_d1;
      TH2F *occupancy_d2;
      TH2F *occupancy_d3;
      
      TH1F *hAllRechitTimeHB;
      TH1F *hAllRechitTimeHE;
      
      const double nBinsX =  100;
      const double nBinsY =  100;
      const double minValX =   0;
      const double maxValX = 100;
      const double minValY = -25;
      const double maxValY =  25;
      
      int nHistsGood =0;
      int nHistsBad =0;
      int nHistsSing = 0;

      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MakeTimingMaps::MakeTimingMaps(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
    
  for (int i = 0; i < 59; ++i){
    hTiming_Depth1[i] = new TH1F*[72];
    hTiming_Depth2[i] = new TH1F*[72];
  }
  
  for(int i = 0; i < 6; ++i){
    hTiming_Depth3[i] = new TH1F*[72]; 
  }
  
  sHBHERecHitCollection = iConfig.getUntrackedParameter<string>("HBHERecHits","hbheprereco");
  runNumber_ = iConfig.getParameter<int>("runNumber");
   
  // Make the output tree
  OutputTree = FileService->make<TTree>("testTree","testTree");
  OutputTree->Branch("RunNumber",&RunNumber,"RunNumber/I");
  OutputTree->Branch("EvtNumber",&EvtNumber,"EvtNumber/I");
  OutputTree->Branch("RecHitEnergy",&RecHitEnergy,"RecHitEnergy/D");
  
  OutputTree->Branch("RecHitTime",&RecHitTime,"RecHitTime/D");
  OutputTree->Branch("iEta",&iEta,"iEta/D");
  OutputTree->Branch("iPhi",&iPhi,"iPhi/D");
  OutputTree->Branch("depth",&depth,"depth/I");
  
  
  for(int i = 0; i < 72; ++i){
    for(int j = 0; j < 59; ++j){
       hTiming_Depth1[j][i] = FileService->make<TH1F>(("Depth1_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),("Depth1_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),75,-37.5,37.5);
       hTiming_Depth2[j][i] = FileService->make<TH1F>(("Depth2_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),("Depth2_ieta"+int2string(j-29)+"_iphi"+int2string(i+1)).c_str(),75,-37.5,37.5);
    } 
    for(int j = 0; j < 6; ++j){
      int eta = 0;
      if(j==0) eta = -28;
      if(j==1) eta = -27;
      if(j==2) eta = -16;
      if(j==3) eta =  16;
      if(j==4) eta =  27;
      if(j==5) eta =  28;
      hTiming_Depth3[j][i] = FileService->make<TH1F>(("Depth3_ieta"+int2string(eta)+"_iphi"+int2string(i+1)).c_str(),("Depth3_ieta"+int2string(eta)+"_iphi"+int2string(i+1)).c_str(),75,-37.5,37.5);
    }
  }
  
  for(int i = 0; i< 10; ++i){
    hGoodTimeADC[i] = FileService->make<TH1F>(("hADCGoodTime_"+int2string(i+1)).c_str(),("hADCGoodTime_"+int2string(i+1)).c_str(),10,-100,150);
    hOffTimeADC[i] = FileService->make<TH1F>(("hADCOffTime_"+int2string(i+1)).c_str(),("hADCOffTime_"+int2string(i+1)).c_str(),10,-100,150);
    hSingleChanADC[i] = FileService->make<TH1F>(("hSingleChanADC_"+int2string(i+1)).c_str(),("hSingleChanADC_"+int2string(i+1)).c_str(),10,-100,150);
    
    hGoodChanEnergy[i] = FileService->make<TH1F>(("hGoodChanEnergy_"+int2string(i+1)).c_str(),("hGoodChanEnergy_"+int2string(i+1)).c_str(),10,-100,150);
    hBadChanEnergy[i] = FileService->make<TH1F>(("hBadChanEnergy_"+int2string(i+1)).c_str(),("hBadChanEnergy_"+int2string(i+1)).c_str(),10,-100,150);
  }
  
  
  // run 251244
  hHBHETiming_wide_Depth1 = FileService->make<TProfile2D>("hHBHETiming_wide_Depth1","hHBHETiming_wide_Depth1",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  hHBHETiming_wide_Depth2 = FileService->make<TProfile2D>("hHBHETiming_wide_Depth2","hHBHETiming_wide_Depth2",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  hHBHETiming_wide_Depth3 = FileService->make<TProfile2D>("hHBHETiming_wide_Depth3","hHBHETiming_wide_Depth3",59,-29.5,29.5,72,0.5,72.5, -37.5, 37.5,"s");
  
  hHBHETiming_Depth1 = FileService->make<TProfile2D>("hHBHETiming_Depth1","hHBHETiming_Depth1",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  hHBHETiming_Depth2 = FileService->make<TProfile2D>("hHBHETiming_Depth2","hHBHETiming_Depth2",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  hHBHETiming_Depth3 = FileService->make<TProfile2D>("hHBHETiming_Depth3","hHBHETiming_Depth3",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  
  hHBHETimingRat_Depth1 = FileService->make<TProfile2D>("hHBHETimingRat_Depth1","hHBHETimingRat_Depth1",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  hHBHETimingRat_Depth2 = FileService->make<TProfile2D>("hHBHETimingRat_Depth2","hHBHETimingRat_Depth2",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  hHBHETimingRat_Depth3 = FileService->make<TProfile2D>("hHBHETimingRat_Depth3","hHBHETimingRat_Depth3",59,-29.5,29.5,72,0.5,72.5, -12.5, 12.5,"s");
  
  hHBHETiming_SmallStats = FileService->make<TProfile>("hHBHETiming_SmallStats","hHBHETiming_SmallStats",3,0.5,3.5, -37.5, 37.5,"s");
  
  hHBHEEnergy_Depth1 = FileService->make<TH2F>("hHBHEEnergy_Depth1","hHBHEEnergy_Depth1",59,-29.5,29.5,72,0.5,72.5);
  hHBHEEnergy_Depth2 = FileService->make<TH2F>("hHBHEEnergy_Depth2","hHBHEEnergy_Depth2",59,-29.5,29.5,72,0.5,72.5);
  hHBHEEnergy_Depth3 = FileService->make<TH2F>("hHBHEEnergy_Depth3","hHBHEEnergy_Depth3",59,-29.5,29.5,72,0.5,72.5);
  
  occupancy_d1 = FileService->make<TH2F>("occupancy_d1","occupancy_depth1",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d2 = FileService->make<TH2F>("occupancy_d2","occupancy_depth2",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d3 = FileService->make<TH2F>("occupancy_d3","occupancy_depth3",59,-29.5,29.5,72,0.5,72.5);
  
  hPart1 = FileService->make<TH1F>("hPart1","average avg time partition 1", 25, -12.5, 12.5);
  hPart2 = FileService->make<TH1F>("hPart2","average avg time partition 2", 25, -12.5, 12.5);
  hPart3 = FileService->make<TH1F>("hPart3","average avg time partition 3", 25, -12.5, 12.5);
  
  hAllRechitTimeHB = FileService->make<TH1F>("hAllRechitTimeHB","hAllRechitTimeHB", 60, -15.0, 15.0);
  hAllRechitTimeHE = FileService->make<TH1F>("hAllRechitTimeHE","hAllRechitTimeHE", 60, -15.0, 15.0);
  
  for(int i = 0; i < 3; i++){
    hM2M0_HB_5_10[i]  = FileService->make<TH1F>(("hM2M0_HB_5_10_p"+int2string(i+1)).c_str(),  ("M2/M0, HB part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM2M0_HB_10_20[i] = FileService->make<TH1F>(("hM2M0_HB_10_20_p"+int2string(i+1)).c_str(), ("M2/M0, HB part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM2M0_HB_20[i]    = FileService->make<TH1F>(("hM2M0_HB_20_p"+int2string(i+1)).c_str(),    ("M2/M0, HB part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
    
    hM2M0_HE_5_10[i]  = FileService->make<TH1F>(("hM2M0_HE_5_10_p"+int2string(i+1)).c_str(),  ("M2/M0, HE part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM2M0_HE_10_20[i] = FileService->make<TH1F>(("hM2M0_HE_10_20_p"+int2string(i+1)).c_str(), ("M2/M0, HE part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM2M0_HE_20[i]    = FileService->make<TH1F>(("hM2M0_HE_20_p"+int2string(i+1)).c_str(),    ("M2/M0, HE part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
  }

}


MakeTimingMaps::~MakeTimingMaps()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

std::string MakeTimingMaps::int2string(int i) {
  stringstream ss;
  string ret;
  ss << i;
  ss >> ret;
  return ret;
}

// ------------ method called for each event  ------------
void
MakeTimingMaps::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

  Handle<HBHERecHitCollection> hRecHits;
   Handle<PCaloHitContainer> hSimHits;
   iEvent.getByLabel(InputTag(sHBHERecHitCollection),hRecHits);
   
   // get the run number of the event 
   RunNumber = iEvent.id().run();
   
  double nrh2 = 0;
  double nrh3 = 0;
  double nrh1 = 0;
  double np1 = 0;
  double np2 = 0;
  double np3 = 0;
  for(int i = 0; i < (int)hRecHits->size(); i++)
  {
    ClearVariables();
    RunNumber = iEvent.id().run();
    EvtNumber = iEvent.id().event();
//     std::cout << EvtNumber << std::endl;
    if(RunNumber != runNumber_ /*&& RunNumber!=251244 && RunNumber!=251252 && RunNumber!=251562 && RunNumber!=251561 && RunNumber!=251643*/) continue;
//     if(EvtNumber!=1209440761&&EvtNumber!=1645183324&&EvtNumber!=1646392152&&EvtNumber!=342259059&&EvtNumber!=752726526&&EvtNumber!=626136425&&EvtNumber!=1907093580&&EvtNumber!=478641879&&EvtNumber!=489395114&&EvtNumber!=420416571&&EvtNumber!=453077625&&EvtNumber!=156112502&&EvtNumber!=211728729&&EvtNumber!=282325790&&EvtNumber!=349577246&&EvtNumber!=268455303&&EvtNumber!=199531636&&EvtNumber!=478641879&&EvtNumber!=489395114) continue;//quit if we aren't on a pre-firing event 
    
    HcalDetId detID_rh = (*hRecHits)[i].id().rawId();
    depth = (*hRecHits)[i].id().depth();
    Method0Energy = (*hRecHits)[i].eraw();
    RecHitEnergy = (*hRecHits)[i].energy();
    RecHitTime = (*hRecHits)[i].time();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    int adc[8];
    
    int auxwd1 = (*hRecHits)[i].auxHBHE();  // TS = 0,1,2,3 info
    int auxwd2 = (*hRecHits)[i].aux();      // TS = 4,5,6,7 info
    
    adc[0] = (auxwd1)       & 0x7F;
    adc[1] = (auxwd1 >> 7)  & 0x7F;
    adc[2] = (auxwd1 >> 14) & 0x7F;
    adc[3] = (auxwd1 >> 21) & 0x7F;

    adc[4] = (auxwd2)       & 0x7F;
    adc[5] = (auxwd2 >> 7)  & 0x7F;
    adc[6] = (auxwd2 >> 14) & 0x7F;
    adc[7] = (auxwd2 >> 21) & 0x7F;
    
    //for(int k=0; k < 8; k++) std::cout << adc[k] << std::endl;
   
    if(RecHitEnergy> 1.0){
      if(detID_rh.iphi()>=3 && detID_rh.iphi() < 27){
        np1+=RecHitTime;
        ++nrh1;
        }
        if(detID_rh.iphi()>=27 && detID_rh.iphi() < 51){
          np2+=RecHitTime;
          ++nrh2;
        }
        if(detID_rh.iphi()>=51 || detID_rh.iphi() < 3){
          np3+=RecHitTime;
          ++nrh3;
        }
    }
    
    
    // Fill the Method 2/ Method 0 rechit plots
    if(Method0Energy > 5 && Method0Energy < 10){
      if(detID_rh.subdet() == HcalBarrel){
        if(iPhi >=  3 && iPhi < 27) hM2M0_HB_5_10[0]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M0_HB_5_10[1]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M0_HB_5_10[2]->Fill(RecHitEnergy/Method0Energy);
      } else if (detID_rh.subdet() == HcalEndcap) {
        if(iPhi >=  3 && iPhi < 27) hM2M0_HE_5_10[0]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M0_HE_5_10[1]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M0_HE_5_10[2]->Fill(RecHitEnergy/Method0Energy);
      }
    } else if (Method0Energy > 10 && Method0Energy < 20) {
      if(detID_rh.subdet() == HcalBarrel){
        if(iPhi >=  3 && iPhi < 27) hM2M0_HB_10_20[0]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M0_HB_10_20[1]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M0_HB_10_20[2]->Fill(RecHitEnergy/Method0Energy);
      } else if (detID_rh.subdet() == HcalEndcap) {
        if(iPhi >=  3 && iPhi < 27) hM2M0_HE_10_20[0]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M0_HE_10_20[1]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M0_HE_10_20[2]->Fill(RecHitEnergy/Method0Energy);
      }
    } else if (Method0Energy < 20){
      if(detID_rh.subdet() == HcalBarrel){
        if(iPhi >=  3 && iPhi < 27) hM2M0_HB_5_10[0]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M0_HB_5_10[1]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M0_HB_5_10[2]->Fill(RecHitEnergy/Method0Energy);
      } else if (detID_rh.subdet() == HcalEndcap) {
        if(iPhi >=  3 && iPhi < 27) hM2M0_HE_5_10[0]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M0_HE_5_10[1]->Fill(RecHitEnergy/Method0Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M0_HE_5_10[2]->Fill(RecHitEnergy/Method0Energy);
      }
    }
    
    if(adc[4]>15&&adc[5]>0){
      if(depth==1) hHBHETimingRat_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), adc[4]/adc[5]);
      if(depth==2) hHBHETimingRat_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), adc[4]/adc[5]);
      if(depth==3) hHBHETimingRat_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), adc[4]/adc[5]);
    }
    
    
    
    if(RecHitEnergy > 0.8){
      if(depth==1)hHBHEEnergy_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
      if(depth==2)hHBHEEnergy_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
      if(depth==3)hHBHEEnergy_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
    }
    
    if(RecHitEnergy > 5.0) {
      
      if(fabs(detID_rh.ieta()) < 15) hAllRechitTimeHB->Fill(RecHitTime);
      if(fabs(detID_rh.ieta()) > 16) hAllRechitTimeHE->Fill(RecHitTime);
      
      if(depth==1){
        if(RecHitTime < -8&& nHistsSing < 10){
          for(int nadc = 0; nadc < 8; nadc++){
            hSingleChanADC[nHistsSing]->SetBinContent(nadc+1,adc[nadc]);
            hSingleChanADC[nHistsSing]->SetTitle(("test channel, energy = "+int2string((int)RecHitEnergy)+" [GeV], time = "+int2string((int)RecHitTime)+" [ns], ieta = "+int2string(detID_rh.ieta())+", iphi = "+int2string(detID_rh.iphi())).c_str());
          }
          ++nHistsSing;
        }
          if(RecHitTime < -5 && RecHitTime > -20 && RecHitEnergy > 100 && nHistsBad < 10){
            for(int nadc = 0; nadc < 8; nadc++){
              hOffTimeADC[nHistsBad]->SetBinContent(nadc+1,adc[nadc]);
              hOffTimeADC[nHistsBad]->SetTitle(("OOT Hit, energy = "+int2string((int)RecHitEnergy)+" [GeV], time = "+int2string((int)RecHitTime)+" [ns], ieta = "+int2string(detID_rh.ieta())+", iphi = "+int2string(detID_rh.iphi())).c_str());
            }
            ++nHistsBad;
          }
         else if(RecHitEnergy > 100 && fabs(RecHitTime) < 4 && nHistsGood < 10){
          for(int nadc = 0; nadc < 8; nadc++){
            hGoodTimeADC[nHistsGood]->SetBinContent(nadc+1,adc[nadc]);
            hGoodTimeADC[nHistsGood]->SetTitle(("In-Time Hit, energy = "+int2string((int)RecHitEnergy)+" [GeV], time = "+int2string((int)RecHitTime)+" [ns], ieta = "+int2string(detID_rh.ieta())+", iphi = "+int2string(detID_rh.iphi())).c_str());
          }
          ++nHistsGood;
          
        }
        
        hHBHETiming_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
//         hHBHETimingRat_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), adc[4]/adc[5]);
        if(RecHitEnergy > 10.0) hHBHETiming_wide_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
//         hHBHEEnergy_Depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitEnergy);
        occupancy_d1->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
        hTiming_Depth1[detID_rh.ieta()+29][detID_rh.iphi()-1]->Fill(RecHitTime);
      }
      if(depth==2){
        hHBHETiming_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        if(RecHitEnergy > 10.0) hHBHETiming_wide_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
//         hHBHEEnergy_Depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitEnergy);
        occupancy_d2->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
        hTiming_Depth2[detID_rh.ieta()+29][detID_rh.iphi()-1]->Fill(RecHitTime);
      }
      if(depth==3){
        hHBHETiming_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
        if(RecHitEnergy > 10.0) hHBHETiming_wide_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitTime);
//         hHBHEEnergy_Depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitEnergy);
        occupancy_d3->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
        
        int bin = 0;
        if(detID_rh.ieta() == -28) bin = 0;
        if(detID_rh.ieta() == -27) bin = 1;
        if(detID_rh.ieta() == -16) bin = 2;
        if(detID_rh.ieta() ==  16) bin = 3;
        if(detID_rh.ieta() ==  27) bin = 4;
        if(detID_rh.ieta() ==  28) bin = 5;
        hTiming_Depth3[bin][detID_rh.iphi()-1]->Fill(RecHitTime);
      }
    }
    
    
//      OutputTree->Fill(); // comment in when looking at a few events
  }
  
  
   
     if(nrh1 > 0){
          hHBHETiming_SmallStats->Fill(1,np1/nrh1);
          hPart1->Fill(np1/nrh1);
     }
     if(nrh2> 0){
          hHBHETiming_SmallStats->Fill(2,np2/nrh2);
          hPart2->Fill(np2/nrh2);
     }
     if(nrh3 > 0) {
          hHBHETiming_SmallStats->Fill(3,np3/nrh3);
          hPart3->Fill(np3/nrh3);
     }
   

   
   
   
#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
MakeTimingMaps::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MakeTimingMaps::endJob() 
{
}

void MakeTimingMaps::ClearVariables(){
 RecHitEnergy = 0;
 RunNumber = 0;
 depth=0;
 iEta = 0;
 iPhi = 0;
 RecHitTime = 0;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MakeTimingMaps::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeTimingMaps);
