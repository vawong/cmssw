// -*- C++ -*-
//
// Package:    HBHETimingValidation/MakeTimingMaps
// Class:      MakeTimingMaps
// 
/**\class MakeTimingMaps MakeTimingMaps.cc HBHETimingValidation/MakeTimingMaps/plugins/MakeTimingMaps.cc

 Description: Quick example code for making HBHE timing monitoring plots
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
#include "TLine.h"
#include "TStyle.h"
#include "TTree.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TProfile2D.h"
#include "TLatex.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HORecHit.h"
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

class MakeRun2Plots : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MakeRun2Plots(const edm::ParameterSet&);
      ~MakeRun2Plots();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      std::string int2string(int i);
      
      void ClearVariables();
      
      // some variables for storing information
      double Method0Energy;
      double RecHitEnergy;
      double RecHitTime;
      double RecHitChi2;

      double Method0EnergyM0;
      double RecHitEnergyM0;
      double RecHitTimeM0;

      double Method0EnergyM3;
      double RecHitEnergyM3;
      double RecHitTimeM3;

      int LumiBlock;
      double iEta;
      double iPhi; 
      int depth;
      int RunNumber;
      int EvtNumber;
      
      // create the output file
      edm::Service<TFileService> FileService;
      // create the token to retrieve hit information
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM2;
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM3;
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM0;
      edm::EDGetTokenT<HORecHitCollection>    hOToken;

      TH1F *hCheckTimingM2;
      TH1F *hCheckTimingM2_gt5;
      TH1F *hCheckEnergyM2;
      TH1F *hCheckChi2M2_gt5;

      TH1F *hCheckTimingM3;
      TH1F *hCheckEnergyM3;

      TH1F *hCheckTimingM0;
      TH1F *hCheckEnergyM0;
    
      TH1F *hCheckTimingHO;

      TH2F *hCheckEnergyM2M3;

      TProfile2D *hHBHEChi2;
      TProfile2D *hHBHEChi2_depth1;
      TProfile2D *hHBHEChi2_depth2;
      TProfile2D *hHBHEChi2_depth3;
};


MakeRun2Plots::MakeRun2Plots(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  usesResource("TFileService");
  
  // Tell which collection is consumed
  hRhTokenM2 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbhereco"));
  hRhTokenM3 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoM3"));
  hRhTokenM0 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoM0"));

  hOToken = consumes<HORecHitCollection >(iConfig.getUntrackedParameter<string>("HORecHits","horeco"));

  // example histogram for storing time slice information
  /*
  // Get Configurable parameters
  runNumber_ = iConfig.getParameter<int>("runNumber");
  energyCut_ = iConfig.getParameter<double>("rechitEnergy");
  
  timeLow_ = iConfig.getParameter<double>("timeLowBound");
  timeHigh_ = iConfig.getParameter<double>("timeHighBound");
  */


  hCheckTimingHO = FileService->make<TH1F>("TimingHO","TimingHO",100,60.,40.);

  hCheckTimingM2 = FileService->make<TH1F>("TimingM2","TimingM2",25,-12.5,12.5);
  hCheckTimingM2->GetXaxis()->SetTitle("M2 Timing");
  hCheckTimingM2_gt5 = FileService->make<TH1F>("TimingM2_gt5","TimingM2_gt5",25,-12.5,12.5);
  hCheckTimingM2_gt5->GetXaxis()->SetTitle("M2 Timing");

  hCheckEnergyM2 = FileService->make<TH1F>("EnergyM2","EnergyM2",20,0.,100.);
  hCheckEnergyM2->GetXaxis()->SetTitle("M2 Energy");

  hCheckChi2M2_gt5 = FileService->make<TH1F>("Chi2M2_gt5","Chi2M2_gt5",1000,-10,500);
  hCheckChi2M2_gt5->GetXaxis()->SetTitle("M2 chi2");

  /////

  hHBHEChi2 = FileService->make<TProfile2D>("hHBHEChi2","hHBHEChi2",59,-29.5,29.5,72,0.5,72.5, 0, 500.,"s");
  hHBHEChi2->GetXaxis()->SetTitle("#eta");
  hHBHEChi2->GetYaxis()->SetTitle("#phi");
  hHBHEChi2->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth1 = FileService->make<TProfile2D>("hHBHEChi2_depth1","hHBHEChi2_depth1",59,-29.5,29.5,72,0.5,72.5, 0, 500.,"s");
  hHBHEChi2_depth1->GetXaxis()->SetTitle("#eta");
  hHBHEChi2_depth1->GetYaxis()->SetTitle("#phi");
  hHBHEChi2_depth1->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth2 = FileService->make<TProfile2D>("hHBHEChi2_depth2","hHBHEChi2_depth2",59,-29.5,29.5,72,0.5,72.5, 0, 500.,"s");
  hHBHEChi2_depth2->GetXaxis()->SetTitle("#eta");
  hHBHEChi2_depth2->GetYaxis()->SetTitle("#phi");
  hHBHEChi2_depth2->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth3 = FileService->make<TProfile2D>("hHBHEChi2_depth3","hHBHEChi2_depth3",59,-29.5,29.5,72,0.5,72.5, 0, 500.,"s");
  hHBHEChi2_depth3->GetXaxis()->SetTitle("#eta");
  hHBHEChi2_depth3->GetYaxis()->SetTitle("#phi");
  hHBHEChi2_depth3->GetZaxis()->SetTitle("M2 chi2");

  /////

  hCheckTimingM0 = FileService->make<TH1F>("TimingM0","TimingM0",25,-12.5,12.5);
  hCheckEnergyM0 = FileService->make<TH1F>("EnergyM0","EnergyM0",20,0.,100.);

  hCheckTimingM3 = FileService->make<TH1F>("TimingM3","TimingM3",25,-12.5,12.5);
  hCheckEnergyM3 = FileService->make<TH1F>("EnergyM3","EnergyM3",20,0.,100.);
  hCheckEnergyM3->GetXaxis()->SetTitle("M3 Energy");

  hCheckEnergyM2M3 = FileService->make<TH2F>("EnergyM2M3","EnergyM2M3",20,0.,100.,20,0.,100.);
  hCheckEnergyM2M3->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M3->GetXaxis()->SetTitle("M3 Energy");


}


MakeRun2Plots::~MakeRun2Plots(){} // destructor

// ------------ method called for each event  ------------
void
MakeRun2Plots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  // Read events
  Handle<HBHERecHitCollection> hRecHits; // create handle
  iEvent.getByToken(hRhTokenM2, hRecHits); // get events based on token

  Handle<HBHERecHitCollection> hRecHitsM3; // create handle
  iEvent.getByToken(hRhTokenM3, hRecHitsM3); // get events based on token

  Handle<HBHERecHitCollection> hRecHitsM0; // create handle
  iEvent.getByToken(hRhTokenM0, hRecHitsM0); // get events based on token

  Handle<HORecHitCollection> hRecHitsHO; // create handle
  iEvent.getByToken(hOToken, hRecHitsHO); // get events based on token

  RunNumber = iEvent.id().run(); // get the run number for the event
  EvtNumber = iEvent.id().event(); // get the event number
  LumiBlock = iEvent.id().luminosityBlock();
  
  // Loop over all rechits in one event
  for(int i = 0; i < (int)hRecHits->size(); i++) {
    //    ClearVariables(); // sets a bunch of stuff to zero

    // Just in case the file you are running over contains events from multiple runs,
    // remove everything except the run you are interested in
    //  if(RunNumber != runNumber_) continue;
    
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHits)[i].id().rawId();
    
    // ID information can get us detector coordinates
    depth = (*hRecHits)[i].id().depth();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    // get some variables
    Method0Energy = (*hRecHits)[i].eraw();
    RecHitEnergy = (*hRecHits)[i].energy();
    RecHitTime = (*hRecHits)[i].time();
    RecHitChi2 = (*hRecHits)[i].chi2();

    hCheckEnergyM2->Fill(RecHitEnergy);
    hCheckTimingM2->Fill(RecHitTime);
    if(Method0Energy>5) hCheckTimingM2_gt5->Fill(RecHitTime);
    if(Method0Energy>5) hCheckChi2M2_gt5->Fill(RecHitChi2);

    if(Method0Energy>5) hHBHEChi2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);
    if(Method0Energy>5 && depth==1) hHBHEChi2_depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);
    if(Method0Energy>5 && depth==2) hHBHEChi2_depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);
    if(Method0Energy>5 && depth==3) hHBHEChi2_depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);

  } // recHit

  ///////////
  /// $$$$
  /// $$$$
  ///

  // Loop over all rechits in one event
  for(int i = 0; i < (int)hRecHitsM0->size(); i++) {
    //    ClearVariables(); // sets a bunch of stuff to zero
    
    // Just in case the file you are running over contains events from multiple runs,
    // remove everything except the run you are interested in
    //  if(RunNumber != runNumber_) continue;
    
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHitsM0)[i].id().rawId();
    
    // ID information can get us detector coordinates
    depth = (*hRecHitsM0)[i].id().depth();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    // get some variables
    Method0EnergyM0 = (*hRecHitsM0)[i].eraw();
    RecHitEnergyM0 = (*hRecHitsM0)[i].energy();
    RecHitTimeM0 = (*hRecHitsM0)[i].time();

    hCheckTimingM0->Fill(RecHitTime);
    hCheckEnergyM0->Fill(RecHitEnergy);

  } // recHitM0


  // Loop over all rechits in one event
  for(int i = 0; i < (int)hRecHitsM3->size(); i++) {
    //    ClearVariables(); // sets a bunch of stuff to zero
    
    // Just in case the file you are running over contains events from multiple runs,
    // remove everything except the run you are interested in
    //  if(RunNumber != runNumber_) continue;
    
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHitsM3)[i].id().rawId();
    
    // ID information can get us detector coordinates
    depth = (*hRecHitsM3)[i].id().depth();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    // get some variables
    Method0EnergyM3 = (*hRecHitsM3)[i].eraw();
    RecHitEnergyM3 = (*hRecHitsM3)[i].energy();
    RecHitTimeM3 = (*hRecHitsM3)[i].time();

    hCheckTimingM3->Fill(RecHitTime);
    hCheckEnergyM3->Fill(RecHitEnergy);

  } // recHit M3

  ///// checking HO

  // Loop over all rechits in one event
  for(int i = 0; i < (int)hRecHitsHO->size(); i++) {
    //    ClearVariables(); // sets a bunch of stuff to zero
    
    // Just in case the file you are running over contains events from multiple runs,
    // remove everything except the run you are interested in
    //  if(RunNumber != runNumber_) continue;
    
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHitsHO)[i].id().rawId();
    
    // ID information can get us detector coordinates
    depth = (*hRecHitsHO)[i].id().depth();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    // get some variables
    //    Method0EnergyM0 = (*hRecHitsHO)[i].eraw();
    //    RecHitEnergyM0 = (*hRecHitsM0)[i].energy();
    //    RecHitTimeM0 = (*hRecHitsM0)[i].time();

    hCheckTimingHO->Fill((*hRecHitsHO)[i].time());

  } // recHitM0



}


// ------------ method called once each job just before starting event loop  ------------
void MakeRun2Plots::beginJob(){}

// ------------ method called once each job just after ending the event loop  ------------
void MakeRun2Plots::endJob(){}

void MakeRun2Plots::ClearVariables(){
 RecHitEnergy = 0;
 RunNumber = 0;
 depth=0;
 iEta = 0;
 iPhi = 0;
 RecHitTime = 0;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MakeRun2Plots::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeRun2Plots);
