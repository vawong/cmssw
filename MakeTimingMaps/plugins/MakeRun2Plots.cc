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
      double RecHitEnergyCSV;
      double RecHitTime;
      double RecHitChi2;

      double Method0EnergyM0;
      double RecHitEnergyM0;
      double RecHitTimeM0;

      double RecHitEnergyMAHI;
      double RecHitEnergyM2csv;
      double RecHitEnergyM2csvlag;

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
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM2csv105;
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM2csvlag;
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM3;
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM0;
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenMAHI;

      edm::EDGetTokenT<HORecHitCollection>    hOToken;

      edm::EDGetTokenT<std::vector<PCaloHit>>   hSHitToken;


      TH1F *hCheckTimingM2;
      TH1F *hCheckTimingM2_gt5;
      TH1F *hCheckEnergyM2;
      TH1F *hCheckEnergyM2_HB;
      TH1F *hCheckEnergyM2_HE;
      TH1F *hCheckChi2M2_gt5;
      TH2F *hChi2Energy_barrel;
      TH2F *hChi2Energy_endcap;

      TH1F *hCheckTimingM3;
      TH1F *hCheckEnergyM3;

      TH1F *hCheckTimingM0;
      TH1F *hCheckEnergyM0;

      TH1F *hCheckEnergyMAHI_HB;
      TH1F *hCheckEnergyM2csv105_HB;
      TH1F *hCheckEnergyM2csvlag_HB;
    
      TH1F *hCheckEnergyMAHI_HE;
      TH1F *hCheckEnergyM2csv105_HE;
      TH1F *hCheckEnergyM2csvlag_HE;
    
      TH1F *hCheckTimingHO;

      TH2F *hCheckEnergyM2M0;
      TH2F *hCheckEnergyM2MAHI;
      TH2F *hCheckEnergySIMHITMAHI;
      TH2F *hCheckEnergySIMHITM2;

      TH1F *hCheckMAHIPull_HB;
      TH1F *hCheckMAHIPull_HE;
      TH1F *hCheckM2Pull_HB;
      TH1F *hCheckM2Pull_HE;

      TProfile2D *hHBHEChi2;
      TProfile2D *hHBHEChi2_depth1;
      TProfile2D *hHBHEChi2_depth2;
      TProfile2D *hHBHEChi2_depth3;

      TH2F* hCheckEnergyM2M2csv105_timing_HB;
      TH2F* hCheckEnergyM2M2csv105_M2_HB;
      TH2F* hCheckEnergyM2M2csvlag_timing_HB;
      TH2F* hCheckEnergyM2M2csvlag_M2_HB;
      TH2F* hCheckEnergyM2csvlagMAHIcsvlag_timing_HB;
      TH2F* hCheckEnergyM2csvlagMAHIcsvlag_M2_HB;

      TH2F* hCheckEnergyM2M2csv105_timing_HE;
      TH2F* hCheckEnergyM2M2csv105_M2_HE;
      TH2F* hCheckEnergyM2M2csvlag_timing_HE;
      TH2F* hCheckEnergyM2M2csvlag_M2_HE;
      TH2F* hCheckEnergyM2csvlagMAHIcsvlag_timing_HE;
      TH2F* hCheckEnergyM2csvlagMAHIcsvlag_M2_HE;



  HcalSimParameterMap simParameterMap_;

};


MakeRun2Plots::MakeRun2Plots(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  usesResource("TFileService");
  
  // Tell which collection is consumed
  hRhTokenM2 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoM2"));
  hRhTokenM2csv105 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoM2csv"));
  hRhTokenM2csvlag = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoM2lagcsv"));
  hRhTokenM3 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoM3"));
  hRhTokenM0 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoM0"));
  hRhTokenMAHI = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbherecoMAHIlagcsv"));

  hOToken = consumes<HORecHitCollection >(iConfig.getUntrackedParameter<string>("HORecHits","horeco"));

  hSHitToken = consumes<edm::PCaloHitContainer>(edm::InputTag("g4SimHits","HcalHits"));

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
  hCheckEnergyM2_HB = FileService->make<TH1F>("EnergyM2_HB","EnergyM2_HB",20,0.,100.);
  hCheckEnergyM2_HB->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2_HE = FileService->make<TH1F>("EnergyM2_HE","EnergyM2_HE",20,0.,100.);
  hCheckEnergyM2_HE->GetXaxis()->SetTitle("M2 Energy");

  hCheckChi2M2_gt5 = FileService->make<TH1F>("Chi2M2_gt5","Chi2M2_gt5",1000,-10,500);
  hCheckChi2M2_gt5->GetXaxis()->SetTitle("M2 chi2");


  ///// various chi2 plots for M2

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

  hChi2Energy_barrel = FileService->make<TH2F>("hChi2Energy_barrel","hChi2Energy_barrel",100,0.,500.,100,-2, 5.);
  hChi2Energy_endcap = FileService->make<TH2F>("hChi2Energy_endcap","hChi2Energy_endcap",100,0.,500.,100,-2, 5.);
  hChi2Energy_barrel->GetXaxis()->SetTitle("M2 energy");
  hChi2Energy_barrel->GetYaxis()->SetTitle("log10 (M2 chi2)");
  hChi2Energy_endcap->GetXaxis()->SetTitle("M2 energy");
  hChi2Energy_endcap->GetYaxis()->SetTitle("log10 (M2 chi2)");


  /////

  hCheckTimingM0 = FileService->make<TH1F>("TimingM0","TimingM0",25,-12.5,12.5);
  hCheckEnergyM0 = FileService->make<TH1F>("EnergyM0","EnergyM0",20,0.,100.);

  hCheckTimingM3 = FileService->make<TH1F>("TimingM3","TimingM3",25,-12.5,12.5);
  hCheckEnergyM3 = FileService->make<TH1F>("EnergyM3","EnergyM3",20,0.,100.);
  hCheckEnergyM3->GetXaxis()->SetTitle("M3 Energy");

  hCheckEnergyMAHI_HB = FileService->make<TH1F>("EnergyMAHI_HB","EnergyMAHI_HB",20,0.,100.);
  hCheckEnergyMAHI_HB->GetXaxis()->SetTitle("MAHI Energy");

  hCheckEnergyM2csvlag_HB = FileService->make<TH1F>("EnergyM2csvlag_HB","EnergyM2csvlag_HB",20,0.,100.);
  hCheckEnergyM2csvlag_HB->GetXaxis()->SetTitle("M2csvlag Energy");

  hCheckEnergyM2csv105_HB = FileService->make<TH1F>("EnergyM2csv105_HB","EnergyM2csv105_HB",20,0.,100.);
  hCheckEnergyM2csv105_HB->GetXaxis()->SetTitle("M2csv Energy");

  hCheckEnergyMAHI_HE = FileService->make<TH1F>("EnergyMAHI_HE","EnergyMAHI_HE",20,0.,100.);
  hCheckEnergyMAHI_HE->GetXaxis()->SetTitle("MAHI Energy");

  hCheckEnergyM2csvlag_HE = FileService->make<TH1F>("EnergyM2csvlag_HE","EnergyM2csvlag_HE",20,0.,100.);
  hCheckEnergyM2csvlag_HE->GetXaxis()->SetTitle("M2csvlag Energy");

  hCheckEnergyM2csv105_HE = FileService->make<TH1F>("EnergyM2csv105_HE","EnergyM2csv105_HE",20,0.,100.);
  hCheckEnergyM2csv105_HE->GetXaxis()->SetTitle("M2csv Energy");

  /////
  hCheckEnergyM2M2csv105_timing_HB = FileService->make<TH2F>("CheckEnergyM2M2csv105_timing_HB",";M2-M2_{csv105};M2 Timing",200,-2.,2.,26,-12.5,12.5);
  hCheckEnergyM2M2csv105_M2_HB = FileService->make<TH2F>("CheckEnergyM2M2csv105_M2_HB",";M2-M2_{csv105};M2 Energy (GeV)",200,-2.,2.,100,0,100.);

  hCheckEnergyM2M2csvlag_timing_HB = FileService->make<TH2F>("CheckEnergyM2M2csvlag_timing_HB",";M2-M2_{csvlag};M2 Timing",200,-2.,2.,26,-12.5,12.5);
  hCheckEnergyM2M2csvlag_M2_HB = FileService->make<TH2F>("CheckEnergyM2M2csvlag_M2_HB",";M2-M2_{csvlag};M2 Energy (GeV)",200,-2.,2.,100,0,100.);

  hCheckEnergyM2csvlagMAHIcsvlag_timing_HB = FileService->make<TH2F>("CheckEnergyM2csvlagMAHIcsvlag_timing_HB",";M2_{csvlag}-MAHI_{csvlag};M2 Timing",200,-2.,2.,26,-12.5,12.5);
  hCheckEnergyM2csvlagMAHIcsvlag_M2_HB = FileService->make<TH2F>("CheckEnergyM2csvlagMAHIcsvlag_M2_HB",";M2_{csvlag}-MAHI_{csvlag};M2 Energy (GeV)",200,-2.,2.,100,0,100.);

  hCheckEnergyM2M2csv105_timing_HE = FileService->make<TH2F>("CheckEnergyM2M2csv105_timing_HE",";M2-M2_{csv105};M2 Timing",200,-2.,2.,26,-12.5,12.5);
  hCheckEnergyM2M2csv105_M2_HE = FileService->make<TH2F>("CheckEnergyM2M2csv105_M2_HE",";M2-M2_{csv105};M2 Energy (GeV)",200,-2.,2.,100,0,100.);

  hCheckEnergyM2M2csvlag_timing_HE = FileService->make<TH2F>("CheckEnergyM2M2csvlag_timing_HE",";M2-M2_{csvlag};M2 Timing",200,-2.,2.,26,-12.5,12.5);
  hCheckEnergyM2M2csvlag_M2_HE = FileService->make<TH2F>("CheckEnergyM2M2csvlag_M2_HE",";M2-M2_{csvlag};M2 Energy (GeV)",200,-2.,2.,100,0,100.);

  hCheckEnergyM2csvlagMAHIcsvlag_timing_HE = FileService->make<TH2F>("CheckEnergyM2csvlagMAHIcsvlag_timing_HE",";M2_{csvlag}-MAHI_{csvlag};M2 Timing",200,-2.,2.,26,-12.5,12.5);
  hCheckEnergyM2csvlagMAHIcsvlag_M2_HE = FileService->make<TH2F>("CheckEnergyM2csvlagMAHIcsvlag_M2_HE",";M2_{csvlag}-MAHI_{csvlag};M2 Energy (GeV)",200,-2.,2.,100,0,100.);

  hCheckEnergyM2M0 = FileService->make<TH2F>("EnergyM2M0","EnergyM2M0",20,0.,100.,20,0.,100.);
  hCheckEnergyM2M0->GetYaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M0->GetXaxis()->SetTitle("M0 Energy");

  hCheckEnergyM2MAHI = FileService->make<TH2F>("EnergyM2MAHI","EnergyM2MAHI",20,0.,100.,20,0.,100.);
  hCheckEnergyM2MAHI->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2MAHI->GetYaxis()->SetTitle("MAHI Energy");

  hCheckEnergySIMHITMAHI = FileService->make<TH2F>("EnergySIMHITMAHI","EnergySIMHITMAHI",20,0.,100.,20,0.,100.);
  hCheckEnergySIMHITMAHI->GetXaxis()->SetTitle("SIMHIT Energy");
  hCheckEnergySIMHITMAHI->GetYaxis()->SetTitle("MAHI Energy");

  hCheckEnergySIMHITM2 = FileService->make<TH2F>("EnergySIMHITM2","EnergySIMHITM2",20,0.,100.,20,0.,100.);
  hCheckEnergySIMHITM2->GetXaxis()->SetTitle("SIMHIT Energy");
  hCheckEnergySIMHITM2->GetYaxis()->SetTitle("M2 Energy");

  /////
  /////

  hCheckMAHIPull_HB = FileService->make<TH1F>("PullMAHI_HB","PullMAHI_HB",200,-2,2.);
  hCheckMAHIPull_HB->GetXaxis()->SetTitle("(MAHI-Sim)/Sim");
  hCheckMAHIPull_HE = FileService->make<TH1F>("PullMAHI_HE","PullMAHI_HE",200,-2,2.);
  hCheckMAHIPull_HE->GetXaxis()->SetTitle("(MAHI-Sim)/Sim");

  hCheckM2Pull_HB = FileService->make<TH1F>("PullM2_HB","PullM2_HB",200,-2,2.);
  hCheckM2Pull_HB->GetXaxis()->SetTitle("(M2-Sim)/Sim");
  hCheckM2Pull_HE = FileService->make<TH1F>("PullM2_HE","PullM2_HE",200,-2,2.);
  hCheckM2Pull_HE->GetXaxis()->SetTitle("(M2-Sim)/Sim");

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

  Handle<HBHERecHitCollection> hRecHitsM2csv105; // create handle
  iEvent.getByToken(hRhTokenM2csv105, hRecHitsM2csv105); // get events based on token

  Handle<HBHERecHitCollection> hRecHitsM2csvlag; // create handle
  iEvent.getByToken(hRhTokenM2csvlag, hRecHitsM2csvlag); // get events based on token
  
  Handle<HBHERecHitCollection> hRecHitsM3; // create handle
  iEvent.getByToken(hRhTokenM3, hRecHitsM3); // get events based on token

//  Handle<HBHERecHitCollection> hRecHitsM0; // create handle
//  iEvent.getByToken(hRhTokenM0, hRecHitsM0); // get events based on token

  Handle<HBHERecHitCollection> hRecHitsMAHI; // create handle
  iEvent.getByToken(hRhTokenMAHI, hRecHitsMAHI); // get events based on token

  Handle<HORecHitCollection> hRecHitsHO; // create handle
  iEvent.getByToken(hOToken, hRecHitsHO); // get events based on token

  Handle<PCaloHitContainer> hSimHits;      // create handle
  iEvent.getByToken(hSHitToken, hSimHits);   // SimHits

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

    hCheckEnergyM2M0->Fill(RecHitEnergy,Method0Energy);
    hCheckEnergyM2->Fill(RecHitEnergy);
    hCheckTimingM2->Fill(RecHitTime);
    if(Method0Energy>5) hCheckTimingM2_gt5->Fill(RecHitTime);
    if(Method0Energy>5) hCheckChi2M2_gt5->Fill(RecHitChi2);

    if(Method0Energy>5) hHBHEChi2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);
    if(Method0Energy>5 && depth==1) hHBHEChi2_depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);
    if(Method0Energy>5 && depth==2) hHBHEChi2_depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);
    if(Method0Energy>5 && depth==3) hHBHEChi2_depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2);

    if(std::abs(detID_rh.ieta()) < 15 )  hChi2Energy_barrel->Fill(RecHitEnergy,log10(RecHitChi2));
    if(std::abs(detID_rh.ieta()) > 17 )  hChi2Energy_endcap->Fill(RecHitEnergy,log10(RecHitChi2));
    if(std::abs(detID_rh.ieta()) < 15 )  hCheckEnergyM2_HB->Fill(RecHitEnergy);
    if(std::abs(detID_rh.ieta()) > 17 )  hCheckEnergyM2_HE->Fill(RecHitEnergy);

    //$$$$$$$$$$$$$$$
    /// CORRELATION WITH SIMHITS
    //$$$$$$$$$$$$$$$

    double SamplingFactor = 1;
    if(detID_rh.subdet() == HcalBarrel) {
      SamplingFactor = simParameterMap_.hbParameters().samplingFactor(detID_rh);
    } else if (detID_rh.subdet() == HcalEndcap) {
      SamplingFactor = simParameterMap_.heParameters().samplingFactor(detID_rh);
    }

    double SHitEn=0;
    for (int j=0; j < (int) hSimHits->size(); j++) {
      // check that the SimHits are in the same channel as the rechit
      if ((HcalDetId)(*hSimHits)[j].id() == detID_rh && (*hSimHits)[j].time() > 0 && (*hSimHits)[j].time() < 40)
	SHitEn += SamplingFactor*((*hSimHits)[j].energy());
    }

    //    cout << "SHitEn = " << SHitEn << endl;

    hCheckEnergySIMHITM2->Fill(SHitEn,(*hRecHits)[i].energy());
    if(SHitEn>0 && std::abs(detID_rh.ieta())<15) hCheckM2Pull_HB->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);
    if(SHitEn>0 && std::abs(detID_rh.ieta())>17) hCheckM2Pull_HE->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);

  } // recHit
 
  for(int i = 0; i < (int)hRecHitsM2csv105->size(); i++) 
  {
    HcalDetId detID_rh = (*hRecHitsM2csv105)[i].id().rawId();
    RecHitEnergyM2csv = (*hRecHitsM2csv105)[i].energy();
    if (std::abs(detID_rh.ieta()) < 15) hCheckEnergyM2csv105_HB->Fill(RecHitEnergyM2csv);
    if (std::abs(detID_rh.ieta()) > 17) hCheckEnergyM2csv105_HE->Fill(RecHitEnergyM2csv);
  }

  for(int i = 0; i < (int)hRecHitsM2csvlag->size(); i++) 
  {
    HcalDetId detID_rh = (*hRecHitsM2csvlag)[i].id().rawId();
    RecHitEnergyM2csvlag = (*hRecHitsM2csvlag)[i].energy();
    if (std::abs(detID_rh.ieta()) < 15) hCheckEnergyM2csvlag_HB->Fill(RecHitEnergyM2csvlag);
    if (std::abs(detID_rh.ieta()) > 17) hCheckEnergyM2csvlag_HE->Fill(RecHitEnergyM2csvlag);
  }


  for(int i = 0; i < (int)hRecHits->size(); i++) 
  {
    for(int j = 0; j < (int)hRecHitsM2csv105->size(); j++) 
    {
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHits)[i].id().rawId();
    HcalDetId detID_rhcsv = (*hRecHitsM2csv105)[j].id().rawId();
    
    // ID information can get us detector coordinates
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    // get some variables
    Method0Energy = (*hRecHits)[i].eraw();
    RecHitEnergy = (*hRecHits)[i].energy();
    RecHitEnergyCSV = (*hRecHitsM2csv105)[j].energy();
    RecHitTime = (*hRecHits)[i].time();
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(iEta) < 15 && std::abs(detID_rhcsv.ieta())<15) hCheckEnergyM2M2csv105_timing_HB->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitTime);
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(iEta) < 15 && std::abs(detID_rhcsv.ieta())<15) hCheckEnergyM2M2csv105_M2_HB->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitEnergy);
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(iEta) > 17 && std::abs(detID_rhcsv.ieta())>17) hCheckEnergyM2M2csv105_timing_HE->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitTime);
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(iEta) > 17 && std::abs(detID_rhcsv.ieta())>17) hCheckEnergyM2M2csv105_M2_HE->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitEnergy);
    }
  }

  for(int i = 0; i < (int)hRecHits->size(); i++) 
  {
    for(int j = 0; j < (int)hRecHitsM2csvlag->size(); j++) 
    {
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHits)[i].id().rawId();
    HcalDetId detID_rhcsvlag = (*hRecHitsM2csvlag)[j].id().rawId();
    
    // get some variables
    Method0Energy = (*hRecHits)[i].eraw();
    RecHitEnergy = (*hRecHits)[i].energy();
    RecHitEnergyCSV = (*hRecHitsM2csvlag)[j].energy();
    RecHitTime = (*hRecHits)[i].time();
    if(Method0Energy>5 && detID_rh == detID_rhcsvlag && std::abs(detID_rh.ieta())<15 && std::abs(detID_rhcsvlag.ieta())<15) hCheckEnergyM2M2csvlag_timing_HB->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitTime);
    if(Method0Energy>5 && detID_rh == detID_rhcsvlag && std::abs(detID_rh.ieta())<15 && std::abs(detID_rhcsvlag.ieta())<15) hCheckEnergyM2M2csvlag_M2_HB->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitEnergy);
    if(Method0Energy>5 && detID_rh == detID_rhcsvlag && std::abs(detID_rh.ieta())>17 && std::abs(detID_rhcsvlag.ieta())>17) hCheckEnergyM2M2csvlag_timing_HE->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitTime);
    if(Method0Energy>5 && detID_rh == detID_rhcsvlag && std::abs(detID_rh.ieta())>17 && std::abs(detID_rhcsvlag.ieta())>17) hCheckEnergyM2M2csvlag_M2_HE->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitEnergy);
    }
  }

  for(int i = 0; i < (int)hRecHitsM2csvlag->size(); i++) 
  {
    for(int j = 0; j < (int)hRecHitsMAHI->size(); j++) 
    {
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHitsM2csvlag)[i].id().rawId();
    HcalDetId detID_rhcsv = (*hRecHitsMAHI)[j].id().rawId();
    
    // get some variables
    Method0Energy = (*hRecHitsM2csvlag)[i].eraw();
    RecHitEnergy = (*hRecHitsM2csvlag)[i].energy();
    RecHitEnergyCSV = (*hRecHitsMAHI)[j].energy();
    RecHitTime = (*hRecHitsM2csvlag)[i].time();
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(detID_rh.ieta())<15 && std::abs(detID_rhcsv.ieta())<15) hCheckEnergyM2csvlagMAHIcsvlag_timing_HB->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitTime);
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(detID_rh.ieta())<15 && std::abs(detID_rhcsv.ieta())<15) hCheckEnergyM2csvlagMAHIcsvlag_M2_HB->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitEnergy);
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(detID_rh.ieta())>17 && std::abs(detID_rhcsv.ieta())>17) hCheckEnergyM2csvlagMAHIcsvlag_timing_HE->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitTime);
    if(Method0Energy>5 && detID_rh == detID_rhcsv && std::abs(detID_rh.ieta())>17 && std::abs(detID_rhcsv.ieta())>17) hCheckEnergyM2csvlagMAHIcsvlag_M2_HE->Fill(RecHitEnergy-RecHitEnergyCSV,RecHitEnergy);
    }
  }
  ///////////
  /// $$$$
  /// $$$$
  ///

  // Loop over all rechits in one event
//  for(int i = 0; i < (int)hRecHitsM0->size(); i++) {
//    //    ClearVariables(); // sets a bunch of stuff to zero
//
//    // Just in case the file you are running over contains events from multiple runs,
//    // remove everything except the run you are interested in
//    //  if(RunNumber != runNumber_) continue;
//    
//    // get ID information for the reconstructed hit
//    HcalDetId detID_rh = (*hRecHitsM0)[i].id().rawId();
//    
//    // ID information can get us detector coordinates
//    depth = (*hRecHitsM0)[i].id().depth();
//    iEta = detID_rh.ieta();
//    iPhi = detID_rh.iphi();
//    
//    // get some variables
//    Method0EnergyM0 = (*hRecHitsM0)[i].eraw();
//    RecHitEnergyM0 = (*hRecHitsM0)[i].energy();
//    RecHitTimeM0 = (*hRecHitsM0)[i].time();
//
//    hCheckTimingM0->Fill(RecHitTime);
//    hCheckEnergyM0->Fill(RecHitEnergy);
//
//  } // recHitM0
//

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
    hCheckEnergyM3->Fill(RecHitEnergyM3);

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


  //////////////////////
  //////////////////////
  //////////////////////  //////////////////////
  //////////////////////
  //////////////////////
  //////////////////////


  // Loop over all rechits
  for(int i = 0; i < (int)hRecHitsMAHI->size(); i++) {

    double SHitEn=0;

    // get ID information for the reconstructed hit
    HcalDetId detID_rh_MAHI = (*hRecHitsMAHI)[i].id().rawId();

    if (std::abs(detID_rh_MAHI.ieta()) < 15) hCheckEnergyMAHI_HB->Fill((*hRecHitsMAHI)[i].energy());
    if (std::abs(detID_rh_MAHI.ieta()) > 17) hCheckEnergyMAHI_HE->Fill((*hRecHitsMAHI)[i].energy());

    //$$$$$$$$$$$$$$$
    /// CORRELATION WITH M2
    //$$$$$$$$$$$$$$$

    // Loop over all rechits in one event
    for(int i = 0; i < (int)hRecHits->size(); i++) {

      HcalDetId detID_rh_M2 = (*hRecHitsMAHI)[i].id().rawId();
      if(detID_rh_MAHI==detID_rh_M2) hCheckEnergyM2MAHI->Fill((*hRecHitsMAHI)[i].energy(),(*hRecHits)[i].energy());

    } // end M2

    //$$$$$$$$$$$$$$$
    /// CORRELATION WITH SIMHITS
    //$$$$$$$$$$$$$$$

    double SamplingFactor = 1;
    if(detID_rh_MAHI.subdet() == HcalBarrel) {
      SamplingFactor = simParameterMap_.hbParameters().samplingFactor(detID_rh_MAHI);
    } else if (detID_rh_MAHI.subdet() == HcalEndcap) {
      SamplingFactor = simParameterMap_.heParameters().samplingFactor(detID_rh_MAHI);
    }

    for (int j=0; j < (int) hSimHits->size(); j++) {
      // check that the SimHits are in the same channel as the rechit
      if ((HcalDetId)(*hSimHits)[j].id() == detID_rh_MAHI && (*hSimHits)[j].time() > 0 && (*hSimHits)[j].time() < 40)
	SHitEn += SamplingFactor*((*hSimHits)[j].energy());
    }

    //    cout << "SHitEn = " << SHitEn << endl;

    hCheckEnergySIMHITMAHI->Fill(SHitEn,(*hRecHitsMAHI)[i].energy());
    if(SHitEn>0 && std::abs(detID_rh_MAHI.ieta())<15) hCheckMAHIPull_HB->Fill(((*hRecHitsMAHI)[i].energy()-SHitEn)/SHitEn);
    if(SHitEn>0 && std::abs(detID_rh_MAHI.ieta())>17) hCheckMAHIPull_HE->Fill(((*hRecHitsMAHI)[i].energy()-SHitEn)/SHitEn);


  } // recHit MAHI



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
