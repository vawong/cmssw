// -*- C++ -*-
//
// Package:    HBHETimingValidation/RecHitComp
// Class:      RecHitComp
// 
/**\class RecHitComp RecHitComp.cc HBHETimingValidation/RecHitComp/plugins/RecHitComp.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Stephanie Brandt
//         Created:  Mon, 24 Aug 2015 12:47:39 GMT
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

class RecHitComp : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit RecHitComp(const edm::ParameterSet&);
      ~RecHitComp();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
            std::string int2string(int i);
      
      void ClearVariables();
      void PrintHistograms();
      
      
      double Method0Energy;
      double RecHitEnergy;
      double RecHitTime;
      double iEta;
      double iPhi; 
      int depth;
      int RunNumber;
      int LumiSection;
      int EvtNumber;
      
      bool FillHBHE;
      
      
      string sHBHERecHitCollectionMethod3;      
      string sHBHERecHitCollectionMethod2; 
      edm::Service<TFileService> FileService;
      
      // pointer to pointer to pointers to make a 2-d array of histograms
      TH1F ***hTiming_Depth1 = new TH1F**[59];
      TH1F ***hTiming_Depth2 = new TH1F**[59];
      TH1F ***hTiming_Depth3 = new TH1F**[6];
      
      // Method 2 vs. Method 0
      TH1F **hM2M0_HB_5_10 = new TH1F*[3];
      TH1F **hM2M0_HB_10_20 = new TH1F*[3];
      TH1F **hM2M0_HB_20 = new TH1F*[3];
      
      TH1F **hM2M0_HE_5_10 = new TH1F*[3];
      TH1F **hM2M0_HE_10_20 = new TH1F*[3];
      TH1F **hM2M0_HE_20 = new TH1F*[3];
      
      // Method 3 vs. Method 0
      TH1F **hM3M0_HB_5_10 = new TH1F*[3];
      TH1F **hM3M0_HB_10_20 = new TH1F*[3];
      TH1F **hM3M0_HB_20 = new TH1F*[3];
      
      TH1F **hM3M0_HE_5_10 = new TH1F*[3];
      TH1F **hM3M0_HE_10_20 = new TH1F*[3];
      TH1F **hM3M0_HE_20 = new TH1F*[3];
      
      // Method 2 vs. Method 3
      TH1F **hM2M3_HB_5_10 = new TH1F*[3];
      TH1F **hM2M3_HB_10_20 = new TH1F*[3];
      TH1F **hM2M3_HB_20 = new TH1F*[3];
      
      TH1F **hM2M3_HE_5_10 = new TH1F*[3];
      TH1F **hM2M3_HE_10_20 = new TH1F*[3];
      TH1F **hM2M3_HE_20 = new TH1F*[3];
      
      int runNumber_;
      bool printHists_;
      int lumiBlock_;
};

//
// constants, enums and typedefs
//

std::string RecHitComp::int2string(int i) {
  stringstream ss;
  string ret;
  ss << i;
  ss >> ret;
  return ret;
}

//
// static data member definitions
//

//
// constructors and destructor
//
RecHitComp::RecHitComp(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");

  sHBHERecHitCollectionMethod3 = iConfig.getUntrackedParameter<string>("HBHERecHits","hbheprerecoHLT");
  sHBHERecHitCollectionMethod2 = iConfig.getUntrackedParameter<string>("HBHERecHits","hbhereco");
  
  runNumber_ = iConfig.getParameter<int>("runNumber");
  printHists_ = iConfig.getParameter<bool>("printHists");
  lumiBlock_ = iConfig.getParameter<int>("lumiSectionCut");

  for(int i = 0; i < 3; i++){
    // Method 2 vs. Method 0
    hM2M0_HB_5_10[i]  = FileService->make<TH1F>(("hM2M0_HB_5_10_p"+int2string(i+1)).c_str(),  ("M2/M0, HB part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM2M0_HB_10_20[i] = FileService->make<TH1F>(("hM2M0_HB_10_20_p"+int2string(i+1)).c_str(), ("M2/M0, HB part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM2M0_HB_20[i]    = FileService->make<TH1F>(("hM2M0_HB_20_p"+int2string(i+1)).c_str(),    ("M2/M0, HB part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
    
    hM2M0_HE_5_10[i]  = FileService->make<TH1F>(("hM2M0_HE_5_10_p"+int2string(i+1)).c_str(),  ("M2/M0, HE part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM2M0_HE_10_20[i] = FileService->make<TH1F>(("hM2M0_HE_10_20_p"+int2string(i+1)).c_str(), ("M2/M0, HE part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM2M0_HE_20[i]    = FileService->make<TH1F>(("hM2M0_HE_20_p"+int2string(i+1)).c_str(),    ("M2/M0, HE part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
    
    // Method 3 vs. MEthod 0
    hM3M0_HB_5_10[i]  = FileService->make<TH1F>(("hM3M0_HB_5_10_p"+int2string(i+1)).c_str(),  ("M3/M0, HB part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM3M0_HB_10_20[i] = FileService->make<TH1F>(("hM3M0_HB_10_20_p"+int2string(i+1)).c_str(), ("M3/M0, HB part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM3M0_HB_20[i]    = FileService->make<TH1F>(("hM3M0_HB_20_p"+int2string(i+1)).c_str(),    ("M3/M0, HB part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
    
    hM3M0_HE_5_10[i]  = FileService->make<TH1F>(("hM3M0_HE_5_10_p"+int2string(i+1)).c_str(),  ("M3/M0, HE part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM3M0_HE_10_20[i] = FileService->make<TH1F>(("hM3M0_HE_10_20_p"+int2string(i+1)).c_str(), ("M3/M0, HE part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM3M0_HE_20[i]    = FileService->make<TH1F>(("hM3M0_HE_20_p"+int2string(i+1)).c_str(),    ("M3/M0, HE part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
    
    // Method 2 vs. Method 3
    hM2M3_HB_5_10[i]  = FileService->make<TH1F>(("hM2M3_HB_5_10_p"+int2string(i+1)).c_str(),  ("M2/M3, HB part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM2M3_HB_10_20[i] = FileService->make<TH1F>(("hM2M3_HB_10_20_p"+int2string(i+1)).c_str(), ("M2/M3, HB part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM2M3_HB_20[i]    = FileService->make<TH1F>(("hM2M3_HB_20_p"+int2string(i+1)).c_str(),    ("M2/M3, HB part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
    
    hM2M3_HE_5_10[i]  = FileService->make<TH1F>(("hM2M3_HE_5_10_p"+int2string(i+1)).c_str(),  ("M2/M3, HE part"+int2string(i+1)+" 5-10GeV").c_str(),  50, 0.0, 2.0);
    hM2M3_HE_10_20[i] = FileService->make<TH1F>(("hM2M3_HE_10_20_p"+int2string(i+1)).c_str(), ("M2/M3, HE part"+int2string(i+1)+" 10-20GeV").c_str(), 50, 0.0, 2.0);
    hM2M3_HE_20[i]    = FileService->make<TH1F>(("hM2M3_HE_20_p"+int2string(i+1)).c_str(),    ("M2/M3, HE part"+int2string(i+1)+" >20GeV").c_str(),   50, 0.0, 2.0);
  }
}


RecHitComp::~RecHitComp()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}




//
// member functions
//

// ------------ method called for each event  ------------
void
RecHitComp::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  Handle<HBHERecHitCollection> hRecHitsMethod2;//, hRecHitsMethod3;
  Handle<PCaloHitContainer> hSimHits;
//   iEvent.getByLabel(InputTag(sHBHERecHitCollectionMethod3),hRecHitsMethod3);
 iEvent.getByLabel(InputTag(sHBHERecHitCollectionMethod2),hRecHitsMethod2);
  
  // get the run number of the event 
  RunNumber = iEvent.id().run();
  LumiSection = iEvent.id().luminosityBlock();
  if(LumiSection > 75)std::cout << LumiSection << std::endl;
  if(LumiSection < lumiBlock_) return;
  
  if(RunNumber != runNumber_) return;
  
  // Do the Method 2/ Method 0 comparison
  for(int i = 0; i < (int)hRecHitsMethod2->size(); i++)
  {
    ClearVariables();
    RunNumber = iEvent.id().run();
    EvtNumber = iEvent.id().event();
  //  if(RunNumber != runNumber_ /*&& RunNumber!=251244 && RunNumber!=251252 && RunNumber!=251562 && RunNumber!=251561 && RunNumber!=251643*/) continue;
    
//     if()
  
    HcalDetId detID_rh = (*hRecHitsMethod2)[i].id().rawId();
    depth = (*hRecHitsMethod2)[i].id().depth();
    Method0Energy = (*hRecHitsMethod2)[i].eraw();
    RecHitEnergy = (*hRecHitsMethod2)[i].energy();
    RecHitTime = (*hRecHitsMethod2)[i].time();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
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
    } else if (Method0Energy > 20){
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
  }
  
//   // now do the Method 3 / Method 0 comparison
//   for(int i = 0; i < (int)hRecHitsMethod3->size(); i++)
//   {
//     ClearVariables();
//     RunNumber = iEvent.id().run();
//     EvtNumber = iEvent.id().event();
//     //if(RunNumber != runNumber_ /*&& RunNumber!=251244 && RunNumber!=251252 && RunNumber!=251562 && RunNumber!=251561 && RunNumber!=251643*/) continue;
//     
//     HcalDetId detID_rh = (*hRecHitsMethod3)[i].id().rawId();
//     depth = (*hRecHitsMethod3)[i].id().depth();
//     Method0Energy = (*hRecHitsMethod3)[i].eraw();
//     RecHitEnergy = (*hRecHitsMethod3)[i].energy();
//     RecHitTime = (*hRecHitsMethod3)[i].time();
//     iEta = detID_rh.ieta();
//     iPhi = detID_rh.iphi();
//     
//     // Fill the Method 2/ Method 0 rechit plots
//     if(Method0Energy > 5 && Method0Energy < 10){
//       if(detID_rh.subdet() == HcalBarrel){
//         if(iPhi >=  3 && iPhi < 27) hM3M0_HB_5_10[0]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 27 && iPhi < 51) hM3M0_HB_5_10[1]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 51 || iPhi < 3 ) hM3M0_HB_5_10[2]->Fill(RecHitEnergy/Method0Energy);
//       } else if (detID_rh.subdet() == HcalEndcap) {
//         if(iPhi >=  3 && iPhi < 27) hM3M0_HE_5_10[0]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 27 && iPhi < 51) hM3M0_HE_5_10[1]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 51 || iPhi < 3 ) hM3M0_HE_5_10[2]->Fill(RecHitEnergy/Method0Energy);
//       }
//     } else if (Method0Energy > 10 && Method0Energy < 20) {
//       if(detID_rh.subdet() == HcalBarrel){
//         if(iPhi >=  3 && iPhi < 27) hM3M0_HB_10_20[0]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 27 && iPhi < 51) hM3M0_HB_10_20[1]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 51 || iPhi < 3 ) hM3M0_HB_10_20[2]->Fill(RecHitEnergy/Method0Energy);
//       } else if (detID_rh.subdet() == HcalEndcap) {
//         if(iPhi >=  3 && iPhi < 27) hM3M0_HE_10_20[0]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 27 && iPhi < 51) hM3M0_HE_10_20[1]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 51 || iPhi < 3 ) hM3M0_HE_10_20[2]->Fill(RecHitEnergy/Method0Energy);
//       }
//     } else if (Method0Energy > 20){
//       if(detID_rh.subdet() == HcalBarrel){
//         if(iPhi >=  3 && iPhi < 27) hM3M0_HB_5_10[0]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 27 && iPhi < 51) hM3M0_HB_5_10[1]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 51 || iPhi < 3 ) hM3M0_HB_5_10[2]->Fill(RecHitEnergy/Method0Energy);
//       } else if (detID_rh.subdet() == HcalEndcap) {
//         if(iPhi >=  3 && iPhi < 27) hM3M0_HE_5_10[0]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 27 && iPhi < 51) hM3M0_HE_5_10[1]->Fill(RecHitEnergy/Method0Energy);
//         if(iPhi >= 51 || iPhi < 3 ) hM3M0_HE_5_10[2]->Fill(RecHitEnergy/Method0Energy);
//       }
//     }
//   }

/*
  // add a loop to match method 2 and method 3
  for(int i = 0; i < (int)hRecHitsMethod2->size(); i++)
  {
    ClearVariables();
    RunNumber = iEvent.id().run();
    EvtNumber = iEvent.id().event();
    
    HcalDetId detID_rh = (*hRecHitsMethod2)[i].id().rawId();
    depth = (*hRecHitsMethod2)[i].id().depth();
    Method0Energy = (*hRecHitsMethod2)[i].eraw();
    RecHitEnergy = (*hRecHitsMethod2)[i].energy();
    RecHitTime = (*hRecHitsMethod2)[i].time();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    Method3Energy = (*hRecHitsMethod3)[i].energy();
   
    
    // Fill the Method 2/ Method 0 rechit plots
    if(Method0Energy > 5 && Method0Energy < 10){
      if(detID_rh.subdet() == HcalBarrel){
        if(iPhi >=  3 && iPhi < 27) hM2M3_HB_5_10[0]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M3_HB_5_10[1]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M3_HB_5_10[2]->Fill(RecHitEnergy/Method3Energy);
      } else if (detID_rh.subdet() == HcalEndcap) {
        if(iPhi >=  3 && iPhi < 27) hM2M3_HE_5_10[0]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M3_HE_5_10[1]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M3_HE_5_10[2]->Fill(RecHitEnergy/Method3Energy);
      }
    } else if (Method0Energy > 10 && Method0Energy < 20) {
      if(detID_rh.subdet() == HcalBarrel){
        if(iPhi >=  3 && iPhi < 27) hM2M3_HB_10_20[0]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M3_HB_10_20[1]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M3_HB_10_20[2]->Fill(RecHitEnergy/Method3Energy);
      } else if (detID_rh.subdet() == HcalEndcap) {
        if(iPhi >=  3 && iPhi < 27) hM2M3_HE_10_20[0]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M3_HE_10_20[1]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M3_HE_10_20[2]->Fill(RecHitEnergy/Method3Energy);
      }
    } else if (Method0Energy > 20){
      if(detID_rh.subdet() == HcalBarrel){
        if(iPhi >=  3 && iPhi < 27) hM2M3_HB_5_10[0]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M3_HB_5_10[1]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M3_HB_5_10[2]->Fill(RecHitEnergy/Method3Energy);
      } else if (detID_rh.subdet() == HcalEndcap) {
        if(iPhi >=  3 && iPhi < 27) hM2M3_HE_5_10[0]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 27 && iPhi < 51) hM2M3_HE_5_10[1]->Fill(RecHitEnergy/Method3Energy);
        if(iPhi >= 51 || iPhi < 3 ) hM2M3_HE_5_10[2]->Fill(RecHitEnergy/Method3Energy);
      }
    }
  }
*/

  
 
  ClearVariables();

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


void RecHitComp::ClearVariables(){
 RecHitEnergy = 0;
 RunNumber = 0;
 depth=0;
 iEta = 0;
 iPhi = 0;
 RecHitTime = 0;
}

void RecHitComp::PrintHistograms(){
  TCanvas *c = new TCanvas("c","c",800,600);
  for(int i = 0; i < 3; i++){
  hM2M0_HB_5_10[i]->Draw();
  c->Print(("HBHETimingValidation/RatioPlots/hM2M0_HB_5_10_p"+int2string(i+1)+"_run"+int2string(runNumber_)).c_str());
  c->Clear();
  
  hM2M0_HB_10_20[i]->Draw();
  c->Print(("HBHETimingValidation/RatioPlots/hM2M0_HB_10_20_p"+int2string(i+1)+"_run"+int2string(runNumber_)).c_str());
  c->Clear();
  
  hM2M0_HB_20[i]->Draw();
  c->Print(("HBHETimingValidation/RatioPlots/hM2M0_HB_20_p"+int2string(i+1)+"_run"+int2string(runNumber_)).c_str());
  c->Clear();
  
  hM2M0_HE_5_10[i]->Draw();
  c->Print(("HBHETimingValidation/RatioPlots/hM2M0_HE_5_10_p"+int2string(i+1)+"_run"+int2string(runNumber_)).c_str());
  c->Clear();
  
  hM2M0_HE_10_20[i]->Draw();
  c->Print(("HBHETimingValidation/RatioPlots/hM2M0_HE_10_20_p"+int2string(i+1)+"_run"+int2string(runNumber_)).c_str());
  c->Clear();
  
  hM2M0_HE_20[i]->Draw();
  c->Print(("HBHETimingValidation/RatioPlots/hM2M0_HE_20_p"+int2string(i+1)+"_run"+int2string(runNumber_)).c_str());
  c->Clear();
  
  }
}

// ------------ method called once each job just before starting event loop  ------------
void 
RecHitComp::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
RecHitComp::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
RecHitComp::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(RecHitComp);
