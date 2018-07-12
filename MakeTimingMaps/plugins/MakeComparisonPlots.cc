// -*- C++ -*-
//
// Package:    ComparsionPlots/MakeComparisonPlots
// Class:      MakeComparisonPlots
// 
/**\class MakeComparisonPlots MakeComparisonPlots.cc ComparisonPlots/MakeComparisonPlots/plugins/MakeComparisonPlots.cc

 Description: Quick example code for comparing CPU and GPU output
*/
//
// Original Author:  Vanessa Wong
//         Created:  Fri, 29 Jun 2018 11:42:17 GMT
//
//

// system include files
#include <utility>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

#include "TTree.h"
#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TProfile2D.h"
#include <TROOT.h>
#include <TChain.h>
#include "TSystem.h"

// user include files
#include "PhysicsTools/UtilAlgos/interface/BasicAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseShapes.h"

#include "Geometry/CaloTopology/interface/HcalTopology.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameterMap.h"

//#include "RecoLocalCalo/HcalRecAlgos/test/SimpleHBHEPhase1AlgoDebug.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseContainmentManager.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalCorrectionFunctions.h"

#include "RecoLocalCalo/HcalRecAlgos/interface/PulseShapeFitOOTPileupCorrection.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalDeterministicFit.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/MahiFit.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalTimeSlew.h"

#include "RecoLocalCalo/HcalRecAlgos/interface/parseHBHEPhase1AlgoDescription.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/fetchHcalAlgoData.h"

class MakeComparisonPlots : public edm::one::EDAnalyzer<edm::one::SharedResources> {
   public:
      explicit MakeComparisonPlots(const edm::ParameterSet&);
      ~MakeComparisonPlots() override;

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      void beginJob() override;
      void analyze(const edm::Event&, const edm::EventSetup&) override;
      void endJob() override;

      void ClearVariables();
      
      // some variables for storing information
    public:
      double Method0Energy, Method0EnergyGPU;
      double RecHitEnergy, RecHitEnergyGPU;
      double RecHitTime, RecHitTimeGPU;
      double iEta, iEtaGPU;
      double iPhi, iPhiGPU; 
      int depth, depthGPU;
      int numEvents;
      float count[10];
      
    private:
      // create the output file
      edm::Service<TFileService> FileService;
      TTree *newTree;

      // create the token to retrieve hit information
      edm::EDGetTokenT<HBHERecHitCollection>    hRhToken;
      edm::EDGetTokenT<HBHERecHitCollection> hRhTokenGPU;
        
    // crap for trouble-shooting, create a TCanvas here to print out pulse shapes of problem channels
   TCanvas *c1 = new TCanvas("c1","c1",900,700);
   int nProblems = 0;
};

MakeComparisonPlots::MakeComparisonPlots(const edm::ParameterSet& iConfig)
{
  usesResource("TFileService");
  hRhToken = consumes<HBHERecHitCollection>(edm::InputTag("hbheprereco",""));
  hRhTokenGPU = consumes<HBHERecHitCollection>(edm::InputTag("hbheprerecogpu",""));
  // place to book histograms
}

MakeComparisonPlots::~MakeComparisonPlots(){} // destructor


// ------------ method called for each event  ------------
void
MakeComparisonPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  cout << "This is the newTree: " << newTree << endl;
  // Read events
  Handle<HBHERecHitCollection> hRecHits; // create handle
  iEvent.getByToken(hRhToken, hRecHits); // get events based on token

  Handle<HBHERecHitCollection> hRecHitsGPU;
  iEvent.getByToken(hRhTokenGPU, hRecHitsGPU);
  
  // Loop over all rechits in one event
  for(int i = 0; i < (int)hRecHits->size(); i++) {
    if (i==0) cout << "CPU RecHits ---------------------" << endl;
    //ClearVariables(); // sets a bunch of stuff to zero
    
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHits)[i].id().rawId();
   
    for(int j = 0; j < (int)hRecHitsGPU->size(); j++) {
      if (j==0) cout << "GPU RecHits ----------------------" << endl;
      
      // get ID information for the reconstructed hit
      HcalDetId detID_rhGPU = (*hRecHitsGPU)[j].id().rawId();
      

      if (detID_rh == detID_rhGPU) {

        // ID information can get us detector coordinates - CPU
        Method0Energy = (*hRecHits)[i].eraw();
        RecHitEnergy = (*hRecHits)[i].energy();
        RecHitTime = (*hRecHits)[i].time();
        depth = (*hRecHits)[i].id().depth();
        iEta = detID_rh.ieta();
        iPhi = detID_rh.iphi();

        Method0EnergyGPU = (*hRecHitsGPU)[j].energy();
        RecHitTimeGPU = (*hRecHitsGPU)[j].time();
        depthGPU = (*hRecHitsGPU)[i].id().depth();
        iEtaGPU = detID_rhGPU.ieta();
        iPhiGPU = detID_rhGPU.iphi();

        // cout << "Run " << i << ": " << endl;
        // cout << "Method0Energy: " << Method0Energy << endl;
        // cout << "RecHitEnergy: " << RecHitEnergy << endl;

        // cout << "Run for GPU " << j << ": " << endl;
        // cout << "Method0Energy: " << Method0EnergyGPU << endl;

        newTree->Fill();
      }
    }
  }

  numEvents++;
  cout<<"-- END LOOP --"<<endl;
}

// ------------ method called once each job just before starting event loop  ------------
void MakeComparisonPlots::beginJob()
{
  numEvents = 0;
  cout << "ENTERING HERE! -------" << endl;
  newTree = FileService->make<TTree>("HcalTree", "HcalTree");
  cout<<"newTree is"<<newTree<<endl;

  newTree->Branch("iEta",  &iEta,  "iEta/D");       
  newTree->Branch("iPhi",  &iPhi,  "iPhi/D");
  newTree->Branch("depth", &depth, "depth/I");
  newTree->Branch("RecHitTime", &RecHitTime, "RecHitTime/D");
  newTree->Branch("RecHitTimeGPU", &RecHitTimeGPU, "RecHitTimeGPU/D");
  newTree->Branch("iEtaGPU",  &iEtaGPU,  "iEtaGPU/D");
  newTree->Branch("iPhiGPU",  &iPhiGPU,  "iPhiGPU/D");
  newTree->Branch("depthGPU", &depthGPU, "depthGPU/I");

  newTree->Branch("Method0Energy", &Method0Energy, "Method0Energy/D");
  newTree->Branch("Method0EnergyGPU", &Method0EnergyGPU, "Method0EnergyGPU/D");

}

// ------------ method called once each job just after ending the event loop  ------------
void MakeComparisonPlots::endJob(){
  cout << "Number of Events: " << numEvents << endl;
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MakeComparisonPlots::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void MakeComparisonPlots::ClearVariables(){
 RecHitEnergy = 0;
 RecHitEnergyGPU = 0;
 Method0EnergyGPU = 0;
 depth=0;
 depthGPU = 0;
 iEtaGPU = 0;
 iEta = 0;
 iPhi = 0;
 iPhiGPU = 0;
 RecHitTime = 0;
 RecHitTimeGPU = 0;
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeComparisonPlots);