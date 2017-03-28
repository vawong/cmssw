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
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HBHEChannelInfo.h"

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"


#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseContainmentManager.h"
#include "CondFormats/HcalObjects/interface/HcalRecoParams.h"
#include "CondFormats/HcalObjects/interface/HcalRecoParam.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"
#include "Geometry/HcalCommonData/interface/HcalHitRelabeller.h"
#include "Geometry/HcalCommonData/interface/HcalDDDRecConstants.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloHitResponse.h"

#include "Geometry/Records/interface/HcalRecNumberingRecord.h"
#include "CondFormats/HcalObjects/interface/HcalLogicalMap.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalLogicalMapGenerator.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"


#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameterMap.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class MakePhase1Plots : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit MakePhase1Plots(const edm::ParameterSet&);
      ~MakePhase1Plots();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

  double sigmaSiPMQIE10(double ifC);
  double sigmaHPDQIE8(double ifC);
  //  double getSiPMDarkCurrent(double darkCurrent, double fcByPE, double lambda);
  double getSiPMDarkCurrent();

   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;
      
      std::string int2string(int i);
      
      void ClearVariables();
      
      // some variables for storing information
      double Method0Energy;
      double RecHitEnergy;
      double RecHitEnergyM3;
      double RecHitTime;
      double RecHitChi2;
      int LumiBlock;
      double iEta;
      double iPhi; 
      int depth;
      int RunNumber;
      int EvtNumber;
      
      // create the output file
      edm::Service<TFileService> FileService;
      // create the token to retrieve hit information
      edm::EDGetTokenT<HBHERecHitCollection>    hRhToken;
      edm::EDGetTokenT<HBHERecHitCollection>    hRhTokenM3;
      edm::EDGetTokenT<HBHEChannelInfoCollection>    InfoToken;
      edm::EDGetTokenT<std::vector<PCaloHit>>   hSHitToken;
      edm::EDGetTokenT<bool> hIsoToken;
    
      TH2F *hCheckEnergyM0MX_barrel;
      TH2F *hCheckEnergyM0MX_endcap;

      TH2F *hCheckEnergyM2M3_endcap;
      TH2F *hCheckEnergyM2M3_endcap_zoom;
      TH2F *hCheckEnergyM2M3_barrel;
      TH2F *hCheckEnergyM2M3_barrel_zoom;
      TH1F *hCheckEnergyM2M3ratio_barrel;
      TH1F *hCheckEnergyM2M3ratio_endcap;
      TH2F *hCheckEnergyM2M3ratio_barrel_vsM2E;
      TH2F *hCheckEnergyM2M3ratio_endcap_vsM2E;
      TH2F *hCheckEnergyM2M3ratio_barrel_vsM2E_zoom;
      TH2F *hCheckEnergyM2M3ratio_endcap_vsM2E_zoom;

      TH1F *hCheckEnergyM2M3ratio_barrel_gt5;
      TH1F *hCheckEnergyM2M3ratio_endcap_gt5;

      TH2F *hCheckEnergyM2M0_endcap;
      TH2F *hCheckEnergyM2M0_endcap_zoom;
      TH2F *hCheckEnergyM2M0_barrel;
      TH2F *hCheckEnergyM2M0_barrel_zoom;
      TH1F *hCheckEnergyM2M0ratio_barrel;
      TH1F *hCheckEnergyM2M0ratio_endcap;
      TH2F *hCheckEnergyM2M0ratio_barrel_vsM2E;
      TH2F *hCheckEnergyM2M0ratio_endcap_vsM2E;
      TH2F *hCheckEnergyM2M0ratio_barrel_vsM2E_zoom;
      TH2F *hCheckEnergyM2M0ratio_endcap_vsM2E_zoom;

      TH1F *hCheckCharge_barrel;
      TH1F *hChecknoiseADC_barrel;
      TH1F *hCheckCharge_endcap;
      TH1F *hChecknoiseADC_endcap;
      TH1F *hChecknoiseDC_endcap;

      TH2F *hCheckEnergyADCnoise_VSfc_endcap;
      TH2F *hCheckEnergyADCnoise_VSfc_barrel;

      TH2F *hCheckEnergyPHnoise_VSfc_endcap;
      TH2F *hCheckEnergyPHnoise_VSfc_barrel;

      TH1F *hCheckEnergyM2_barrel;
      TH1F *hCheckEnergyM2_endcap;
      TH1F *hCheckEnergyM2_barrel_zoom;
      TH1F *hCheckEnergyM2_endcap_zoom;
      TH1F *hCheckEnergyM0_barrel;
      TH1F *hCheckEnergyM0_endcap;
      TH1F *hCheckEnergyM0_barrel_zoom;
      TH1F *hCheckEnergyM0_endcap_zoom;

      TH1F *hCheckEnergyM2_endcap_depth1;
      TH1F *hCheckEnergyM2_endcap_depth2;
      TH1F *hCheckEnergyM2_endcap_depth3;
      TH1F *hCheckEnergyM2_endcap_depth4;
      TH1F *hCheckEnergyM2_endcap_depth5;
      TH1F *hCheckEnergyM2_endcap_depth6;

      TH1F *hCheckTimingM2_barrel;
      TH1F *hCheckTimingM2_endcap;

      TH1F *hCheckTimingMX_barrel_gt5;
      TH1F *hCheckTimingMX_endcap_gt5;

      TH1F *hCheckChi2MX_barrel_gt5;
      TH1F *hCheckChi2MX_endcap_gt5;
      TH1F *hCheckChi2MX_barrel_lt5;
      TH1F *hCheckChi2MX_endcap_lt5;

      TH2F *hCheckTimeSIMHITM2_HB;  
      TH2F *hCheckTimeSIMHITM2_HE;  
      TH2F *hCheckTimeSIMHITM2_HB1;  
      TH2F *hCheckTimeSIMHITM2_HE1;  



      TH2F *hCheckEnergySIMHITM2_HB;
      TH2F *hCheckEnergySIMHITM2_HE;
      TH1F *hCheckM2Pull_HB;
      TH1F *hCheckM2Pull_HE;
      TH1F *hCheckM2Pull_HE_depth1;
      TH1F *hCheckM2Pull_HE_depth27;

      TH1F *hCheckTimingMX_endcap_depth1_gt5;
      TH1F *hCheckTimingMX_endcap_depth2_gt5;
      TH1F *hCheckTimingMX_endcap_depth3_gt5;
      TH1F *hCheckTimingMX_endcap_depth4_gt5;

      TH1F *hCheckChi2MX_endcap_depth1_gt5;
      TH1F *hCheckChi2MX_endcap_depth2_gt5;
      TH1F *hCheckChi2MX_endcap_depth3_gt5;
      TH1F *hCheckChi2MX_endcap_depth4_gt5;
      TH1F *hCheckChi2MX_endcap_depth5_gt5;
      TH1F *hCheckChi2MX_endcap_depth6_gt5;

      TH2F *hCheckEnergyTimingSIM_HB; 
      TH2F *hCheckEnergyTimingSIM_HE; 

      TH2F *hCheckEnergyTiming_barrel;
      TH2F *hCheckEnergyTiming_endcap;

      TH2F *hCheckT45ratioVST4_barrel;
      TH2F *hCheckT45ratioVST4_endcap;

      TH1F *hCheckT45ratio_barrel;
      TH1F *hCheckT45ratio_endcap;

      TH2F *hChi2Energy_barrel;
      TH2F *hChi2Energy_endcap;

      TProfile2D *hHBHEChi2;
      TProfile2D *hHBHEChi2_depth1;
      TProfile2D *hHBHEChi2_depth2;
      TProfile2D *hHBHEChi2_depth3;
      TProfile2D *hHBHEChi2_depth4;
      TProfile2D *hHBHEChi2_depth5;
      TProfile2D *hHBHEChi2_depth6;

      TH2F *occupancy_d1;
      TH2F *occupancy_d2;
      TH2F *occupancy_d3;
      TH2F *occupancy_d4;
      TH2F *occupancy_d5;
      TH2F *occupancy_d6;

  //const HcalDDDRecConstants *hcons;
  const HcalDDDRecConstants * theRecNumber;
  const CaloGeometry* theGeometry;
  CaloHitResponse* theResponse;

  /*  

      // 2-d profiles which will hold the average time per channel, 1 for each depth
      TProfile2D *hHBHETiming_Depth1;
      TProfile2D *hHBHETiming_Depth2;
      TProfile2D *hHBHETiming_Depth3;

      // pointer to pointer to pointers to make a 2-d array of histograms
      // These are used to hold the individual rechit timing histograms for each channel
      TH1F ***hTiming_Depth1 = new TH1F**[59];
      TH1F ***hTiming_Depth2 = new TH1F**[59];
      TH1F ***hTiming_Depth3 = new TH1F**[6];
      
      // Check for correlation between same iphi or adjacent iphi
      TH1F *hCheckTimingPhi67Plus;
      TH1F *hCheckTiming66to67P;
      TH2F *hCorrTimingPhi67Plus;
      TH2F *hCorrTiming66to67P;
      
      // Get energy distributions of channels
      TH1F *hCheckEnergyIT;
      TH1F *hCheckEnergyOOT1;
      TH1F *hCheckEnergyOOT2;
      
      TH1F *hCheckEnergyITip51;
      TH1F *hCheckEnergyOOT1ip51;
      TH1F *hCheckEnergyOOT2ip51;
      
      TH1F *hCheckEnergyITip54;
      TH1F *hCheckEnergyOOT1ip54;
      TH1F *hCheckEnergyOOT2ip54;
  */
      
      // Sample histogram for storing time slice information
      TH1F *hTimeSlices;
      TProfile *hTimeSlicesAverage_HB;
      TProfile *hTimeSlicesAverageFC_HB;

      TProfile *hTimeSlicesAverage_HE;
      TProfile *hTimeSlicesAverageFC_HE;

      TProfile *hTimeSlicesAverageFC_HE_depth1;
      TProfile *hTimeSlicesAverageFC_HE_depthgt1;

      TProfile *hTimeSlicesAverageFC_HB_lowE;
      TProfile *hTimeSlicesAverageFC_HB_highE;
      
      int runNumber_;
      double energyCut_;
      double timeLow_;
      double timeHigh_;
      std::vector<double> etas67;
      std::vector<double> etas66;

  bool isData=false;

  HcalSimParameterMap simParameterMap_;
        
  // crap for trouble-shooting, create a TCanvas here to print out pulse shapes of problem channels
  TCanvas *c1 = new TCanvas("c1","c1",900,700);
  int nProblems = 0;

};


MakePhase1Plots::MakePhase1Plots(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  usesResource("TFileService");

  // Tell which collection is consumed
  hRhToken = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbhereco"));
  hRhTokenM3 = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbheprerecoM3"));
  if(!isData) InfoToken = consumes<HBHEChannelInfoCollection>(iConfig.getUntrackedParameter<string>("HBHEChannelInfo","hbheprereco"));
  //  hRhToken = consumes<HBHERecHitCollection >(iConfig.getUntrackedParameter<string>("HBHERecHits","hbheprereco"));
  hIsoToken = consumes<bool >(iConfig.getUntrackedParameter<string>("HBHENoiseFilterResultProducer", "HBHEIsoNoiseFilterResult"));
  if(!isData) hSHitToken = consumes<edm::PCaloHitContainer>(edm::InputTag("g4SimHits","HcalHits"));

  // example histogram for storing time slice information
  /*
  // Get Configurable parameters
  runNumber_ = iConfig.getParameter<int>("runNumber");
  energyCut_ = iConfig.getParameter<double>("rechitEnergy");
  
  timeLow_ = iConfig.getParameter<double>("timeLowBound");
  timeHigh_ = iConfig.getParameter<double>("timeHighBound");
  */

  hCheckCharge_endcap = FileService->make<TH1F>("charge_endcap","charge_endcap", 5000., 0., 5000.);
  hChecknoiseADC_endcap = FileService->make<TH1F>("charge_noiseADC_endcap","charge_noiseADC_endcap", 5000., 0., 5000.);
  hChecknoiseDC_endcap = FileService->make<TH1F>("charge_noiseDC_endcap","charge_noiseDC_endcap", 5000., 0., 5000.);

  hCheckCharge_barrel = FileService->make<TH1F>("charge_barrel","charge_barrel", 1000., 0., 10.);
  hChecknoiseADC_barrel = FileService->make<TH1F>("charge_noiseADC_barrel","charge_noiseADC_barrel", 1000., 0., 10.);

  hCheckEnergyADCnoise_VSfc_endcap = FileService->make<TH2F>("ADCnoise_VSfc_endcap","ADCnoise_VSfc_endcap", 10000, 0., 100000., 1000, 0.005, 0.1);
  hCheckEnergyADCnoise_VSfc_barrel = FileService->make<TH2F>("ADCnoise_VSfc_barrel","ADCnoise_VSfc_barrel", 10000, 0., 100000., 1000, 0.005, 0.1);

  hCheckEnergyPHnoise_VSfc_endcap = FileService->make<TH2F>("PHnoise_VSfc_endcap","PHnoise_VSfc_endcap", 10000, 0., 100000., 1000, 0.005, 1.);
  hCheckEnergyPHnoise_VSfc_barrel = FileService->make<TH2F>("PHnoise_VSfc_barrel","PHnoise_VSfc_barrel", 10000, 0., 100000., 1000, 0.005, 1.);

  hChi2Energy_barrel = FileService->make<TH2F>("hChi2Energy_barrel","hChi2Energy_barrel",100,0.,500.,100,-2, 5.);
  hChi2Energy_endcap = FileService->make<TH2F>("hChi2Energy_endcap","hChi2Energy_endcap",100,0.,500.,100,-2, 5.);

  hChi2Energy_barrel->GetXaxis()->SetTitle("M2 energy");
  hChi2Energy_barrel->GetYaxis()->SetTitle("log10 (M2 chi2)");
  hChi2Energy_endcap->GetXaxis()->SetTitle("M2 energy");
  hChi2Energy_endcap->GetYaxis()->SetTitle("log10 (M2 chi2)");

  /////////

  hCheckChi2MX_barrel_gt5 = FileService->make<TH1F>("Chi2MX_barrel_gt5","chi2MX_barrel_gt5",100, -2, 5.);
  hCheckChi2MX_endcap_gt5 = FileService->make<TH1F>("Chi2MX_endcap_gt5","chi2MX_endcap_gt5",100, -2, 5.);

  hCheckChi2MX_barrel_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");
  hCheckChi2MX_endcap_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");

  hCheckChi2MX_barrel_lt5 = FileService->make<TH1F>("Chi2MX_barrel_lt5","chi2MX_barrel_lt5",100, -2, 5.);
  hCheckChi2MX_endcap_lt5 = FileService->make<TH1F>("Chi2MX_endcap_lt5","chi2MX_endcap_lt5",100, -2, 5.);

  hCheckChi2MX_barrel_lt5->GetXaxis()->SetTitle("log10 (M2 chi2)");
  hCheckChi2MX_endcap_lt5->GetXaxis()->SetTitle("log10 (M2 chi2)");

  /////////

  hCheckEnergyM0MX_barrel = FileService->make<TH2F>("M0MXplot_barrel","M0MXplot_barrel",100,0.,500.,100,0.,500.);
  hCheckEnergyM0MX_endcap = FileService->make<TH2F>("M0MXplot_endcap","M0MXplot_endcap",100,0.,500.,100,0.,500.);

  hCheckEnergyM0MX_barrel->GetXaxis()->SetTitle("M0 Energy");
  hCheckEnergyM0MX_endcap->GetXaxis()->SetTitle("M0 Energy");

  hCheckEnergyM0MX_barrel->GetYaxis()->SetTitle("M2 Energy");
  hCheckEnergyM0MX_endcap->GetYaxis()->SetTitle("M2 Energy");

  //////
  //////
  //////

  hCheckEnergyM2M3ratio_barrel_gt5 = FileService->make<TH1F>("EnergyM2M3ratio_barrel_gt5","EnergyM2M3ratio_barrel_gt5",100, 0.5, 1.5);
  hCheckEnergyM2M3ratio_endcap_gt5 = FileService->make<TH1F>("EnergyM2M3ratio_endcap_gt5","EnergyM2M3ratio_endcap_gt5",100, 0.5, 1.5);

  hCheckEnergyM2M3_barrel = FileService->make<TH2F>("M2M3plot_barrel","M2M3plot_barrel",100,0.,500.,100,0.,500.);
  hCheckEnergyM2M3_barrel_zoom = FileService->make<TH2F>("M2M3plot_barrel_zoom","M2M3plot_barrel_zoom",50,0.,5.,50,0.,5.);
  hCheckEnergyM2M3_endcap = FileService->make<TH2F>("M2M3plot_endcap","M2M3plot_endcap",100,0.,500.,100,0.,500.);
  hCheckEnergyM2M3_endcap_zoom = FileService->make<TH2F>("M2M3plot_endcap_zoom","M2M3plot_endcap_zoom",50,0.,5.,50,0.,5.);
  hCheckEnergyM2M3ratio_barrel = FileService->make<TH1F>("EnergyM2M3ratio_barrel","EnergyM2M3ratio_barrel",100,0.5,1.5);
  hCheckEnergyM2M3ratio_endcap = FileService->make<TH1F>("EnergyM2M3ratio_endcap","EnergyM2M3ratio_endcap",100,0.5,1.5);

  hCheckEnergyM2M3ratio_barrel_vsM2E = FileService->make<TH2F>("M2M3ratio_barrel_vsM2E","M2M3ratio_barrel_vsM2E",50,0.,500.,50,0.5,1.5);
  hCheckEnergyM2M3ratio_endcap_vsM2E = FileService->make<TH2F>("M2M3ratio_endcap_vsM2E","M2M3ratio_endcap_vsM2E ",50,0.,500.,50,0.5,1.5);

  hCheckEnergyM2M3ratio_barrel_vsM2E_zoom = FileService->make<TH2F>("M2M3ratio_barrel_vsM2E_zoom","M2M3ratio_barrel_vsM2E_zoom",100,0.,5.,40,0.,2.);
  hCheckEnergyM2M3ratio_endcap_vsM2E_zoom = FileService->make<TH2F>("M2M3ratio_endcap_vsM2E_zoom","M2M3ratio_endcap_vsM2E_zoom ",100,0.,5.,40,0.,2.);

  hCheckEnergyM2M3_barrel->GetYaxis()->SetTitle("M3 Energy");
  hCheckEnergyM2M3_barrel_zoom->GetYaxis()->SetTitle("M3 Energy");
  hCheckEnergyM2M3_endcap->GetYaxis()->SetTitle("M3 Energy");
  hCheckEnergyM2M3_endcap_zoom->GetYaxis()->SetTitle("M3 Energy");

  hCheckEnergyM2M3_barrel->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M3_barrel_zoom->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M3_endcap->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M3_endcap_zoom->GetXaxis()->SetTitle("M2 Energy");

  hCheckEnergyM2M3ratio_barrel->GetXaxis()->SetTitle("M3energy/M2energy");
  hCheckEnergyM2M3ratio_endcap->GetXaxis()->SetTitle("M3energy/M2energy");

  hCheckEnergyM2M3ratio_barrel_vsM2E->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M3ratio_endcap_vsM2E->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M3ratio_barrel_vsM2E->GetYaxis()->SetTitle("M3energy/M2energy");
  hCheckEnergyM2M3ratio_endcap_vsM2E->GetYaxis()->SetTitle("M3energy/M2energy");

  hCheckEnergyM2M3ratio_barrel_vsM2E_zoom->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M3ratio_endcap_vsM2E_zoom->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M3ratio_barrel_vsM2E_zoom->GetYaxis()->SetTitle("M3energy/M2energy");
  hCheckEnergyM2M3ratio_endcap_vsM2E_zoom->GetYaxis()->SetTitle("M3energy/M2energy");

  //////
  //////
  //////

  hCheckEnergyM2M0_barrel = FileService->make<TH2F>("M2M0plot_barrel","M2M0plot_barrel",100,0.,500.,100,0.,500.);
  hCheckEnergyM2M0_barrel_zoom = FileService->make<TH2F>("M2M0plot_barrel_zoom","M2M0plot_barrel_zoom",50,0.,5.,50,0.,5.);
  hCheckEnergyM2M0_endcap = FileService->make<TH2F>("M2M0plot_endcap","M2M0plot_endcap",100,0.,500.,100,0.,500.);
  hCheckEnergyM2M0_endcap_zoom = FileService->make<TH2F>("M2M0plot_endcap_zoom","M2M0plot_endcap_zoom",50,0.,5.,50,0.,5.);
  hCheckEnergyM2M0ratio_barrel = FileService->make<TH1F>("EnergyM2M0ratio_barrel","EnergyM2M0ratio_barrel",100,0.5,1.5);
  hCheckEnergyM2M0ratio_endcap = FileService->make<TH1F>("EnergyM2M0ratio_endcap","EnergyM2M0ratio_endcap",100,0.5,1.5);

  hCheckEnergyM2M0ratio_barrel_vsM2E = FileService->make<TH2F>("M2M0ratio_barrel_vsM2E","M2M0ratio_barrel_vsM2E",50,0.,500.,50,0.5,1.5);
  hCheckEnergyM2M0ratio_endcap_vsM2E = FileService->make<TH2F>("M2M0ratio_endcap_vsM2E","M2M0ratio_endcap_vsM2E ",50,0.,500.,50,0.5,1.5);

  hCheckEnergyM2M0ratio_barrel_vsM2E_zoom = FileService->make<TH2F>("M2M0ratio_barrel_vsM2E_zoom","M2M0ratio_barrel_vsM2E_zoom",100,0.,5.,40,0.,2.);
  hCheckEnergyM2M0ratio_endcap_vsM2E_zoom = FileService->make<TH2F>("M2M0ratio_endcap_vsM2E_zoom","M2M0ratio_endcap_vsM2E_zoom ",100,0.,5.,40,0.,2.);

  hCheckEnergyM2M0_barrel->GetYaxis()->SetTitle("M0 Energy");
  hCheckEnergyM2M0_barrel_zoom->GetYaxis()->SetTitle("M0 Energy");
  hCheckEnergyM2M0_endcap->GetYaxis()->SetTitle("M0 Energy");
  hCheckEnergyM2M0_endcap_zoom->GetYaxis()->SetTitle("M0 Energy");

  hCheckEnergyM2M0_barrel->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M0_barrel_zoom->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M0_endcap->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2M0_endcap_zoom->GetXaxis()->SetTitle("M2 Energy");

  hCheckEnergyM2M0ratio_barrel->GetXaxis()->SetTitle("M0energy/M2energy");
  hCheckEnergyM2M0ratio_endcap->GetXaxis()->SetTitle("M0energy/M2energy");

  hCheckEnergyM2M0ratio_barrel_vsM2E->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M0ratio_endcap_vsM2E->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M0ratio_barrel_vsM2E->GetYaxis()->SetTitle("M0energy/M2energy");
  hCheckEnergyM2M0ratio_endcap_vsM2E->GetYaxis()->SetTitle("M0energy/M2energy");

  hCheckEnergyM2M0ratio_barrel_vsM2E_zoom->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M0ratio_endcap_vsM2E_zoom->GetXaxis()->SetTitle("M2energy");
  hCheckEnergyM2M0ratio_barrel_vsM2E_zoom->GetYaxis()->SetTitle("M0energy/M2energy");
  hCheckEnergyM2M0ratio_endcap_vsM2E_zoom->GetYaxis()->SetTitle("M0energy/M2energy");

  //////
  //////
  //////

  //  hCheckEnergyM2_barrel = FileService->make<TH1F>("EnergyM2_barrel","EnergyM2_barrel", 500, 0., 500.);
  //  hCheckEnergyM2_endcap = FileService->make<TH1F>("EnergyM2_endcap","EnergyM2_endcap", 500, 0., 500.);

  hCheckEnergyM2_barrel = FileService->make<TH1F>("EnergyM2_barrel","EnergyM2_barrel",1000,0.,50.);
  hCheckEnergyM2_endcap = FileService->make<TH1F>("EnergyM2_endcap","EnergyM2_endcap",1000,0.,50.);

  hCheckEnergyM2_barrel->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2_endcap->GetXaxis()->SetTitle("M2 Energy");

  //////
  hCheckEnergyM2_barrel_zoom = FileService->make<TH1F>("EnergyM2_barrel_zoom","EnergyM2_barrel_zoom",1000,0.,1.);
  hCheckEnergyM2_endcap_zoom = FileService->make<TH1F>("EnergyM2_endcap_zoom","EnergyM2_endcap_zoom",1000,0.,1.);

  hCheckEnergyM2_barrel_zoom->GetXaxis()->SetTitle("M2 Energy");
  hCheckEnergyM2_endcap_zoom->GetXaxis()->SetTitle("M2 Energy");

  //////

  hCheckEnergyM0_barrel_zoom = FileService->make<TH1F>("EnergyM0_barrel_zoom","EnergyM0_barrel_zoom",1000,0.,1.);
  hCheckEnergyM0_endcap_zoom = FileService->make<TH1F>("EnergyM0_endcap_zoom","EnergyM0_endcap_zoom",1000,0.,1.);

  hCheckEnergyM0_barrel_zoom->GetXaxis()->SetTitle("M0 Energy");
  hCheckEnergyM0_endcap_zoom->GetXaxis()->SetTitle("M0 Energy");

  /////

  hCheckEnergyM2_endcap_depth1 = FileService->make<TH1F>("EnergyM2_endcap_depth1","EnergyM2_endcap_depth1",20,0.,100.);
  hCheckEnergyM2_endcap_depth2 = FileService->make<TH1F>("EnergyM2_endcap_depth2","EnergyM2_endcap_depth2",20,0.,100.);
  hCheckEnergyM2_endcap_depth3 = FileService->make<TH1F>("EnergyM2_endcap_depth3","EnergyM2_endcap_depth3",20,0.,100.);
  hCheckEnergyM2_endcap_depth4 = FileService->make<TH1F>("EnergyM2_endcap_depth4","EnergyM2_endcap_depth4",20,0.,100.);
  hCheckEnergyM2_endcap_depth5 = FileService->make<TH1F>("EnergyM2_endcap_depth5","EnergyM2_endcap_depth5",20,0.,100.);
  hCheckEnergyM2_endcap_depth6 = FileService->make<TH1F>("EnergyM2_endcap_depth6","EnergyM2_endcap_depth6",20,0.,100.);

  hCheckEnergyM0_barrel = FileService->make<TH1F>("EnergyM0_barrel","EnergyM0_barrel",1000,0.,50.);
  hCheckEnergyM0_endcap = FileService->make<TH1F>("EnergyM0_endcap","EnergyM0_endcap",1000,0.,50.);

  hCheckEnergyM0_barrel->GetXaxis()->SetTitle("M0 Energy");
  hCheckEnergyM0_endcap->GetXaxis()->SetTitle("M0 Energy");

  ////

  hCheckTimingM2_barrel = FileService->make<TH1F>("TimingM2_barrel","TimingM2_barrel",25,-12.5,12.5);
  hCheckTimingM2_endcap = FileService->make<TH1F>("TimingM2_endcap","TimingM2_endcap",25,-12.5,12.5);

  hCheckTimingMX_barrel_gt5 = FileService->make<TH1F>("TimingMX_barrel_gt5","TimingMX_barrel_gt5",25,-12.5,12.5);
  hCheckTimingMX_endcap_gt5 = FileService->make<TH1F>("TimingMX_endcap_gt5","TimingMX_endcap_gt5",25,-12.5,12.5);

  hCheckTimingMX_barrel_gt5->GetXaxis()->SetTitle("M2 Timing");
  hCheckTimingMX_endcap_gt5->GetXaxis()->SetTitle("M2 Timing");

  ///////

  hCheckChi2MX_endcap_depth1_gt5 = FileService->make<TH1F>("Chi2MX_endcap_depth1_gt5","Chi2MX_endcap_depth1_gt5",100, -2, 5.);
  hCheckChi2MX_endcap_depth2_gt5 = FileService->make<TH1F>("Chi2MX_endcap_depth2_gt5","Chi2MX_endcap_depth2_gt5",100, -2, 5.);
  hCheckChi2MX_endcap_depth3_gt5 = FileService->make<TH1F>("Chi2MX_endcap_depth3_gt5","Chi2MX_endcap_depth3_gt5",100, -2, 5.);
  hCheckChi2MX_endcap_depth4_gt5 = FileService->make<TH1F>("Chi2MX_endcap_depth4_gt5","Chi2MX_endcap_depth4_gt5",100, -2, 5.);
  hCheckChi2MX_endcap_depth5_gt5 = FileService->make<TH1F>("Chi2MX_endcap_depth5_gt5","Chi2MX_endcap_depth5_gt5",100, -2, 5.);
  hCheckChi2MX_endcap_depth6_gt5 = FileService->make<TH1F>("Chi2MX_endcap_depth6_gt5","Chi2MX_endcap_depth6_gt5",100, -2, 5.);
  hCheckChi2MX_endcap_depth1_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");
  hCheckChi2MX_endcap_depth2_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");
  hCheckChi2MX_endcap_depth3_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");
  hCheckChi2MX_endcap_depth4_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");
  hCheckChi2MX_endcap_depth5_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");
  hCheckChi2MX_endcap_depth6_gt5->GetXaxis()->SetTitle("log10 (M2 chi2)");

  hCheckEnergyTiming_barrel = FileService->make<TH2F>("EnergyTiming_barrel","EnergyTiming_barrel",100,0.,500., 40,-20.,20.);
  hCheckEnergyTiming_endcap = FileService->make<TH2F>("EnergyTiming_endcap","EnergyTiming_endcap",100,0.,500., 40,-20.,20.);
  hCheckEnergyTiming_barrel->GetXaxis()->SetTitle("M2 energy");
  hCheckEnergyTiming_barrel->GetYaxis()->SetTitle("M2 timing");
  hCheckEnergyTiming_endcap->GetXaxis()->SetTitle("M2 energy");
  hCheckEnergyTiming_endcap->GetYaxis()->SetTitle("M2 timing");


  hCheckEnergyTimingSIM_HB = FileService->make<TH2F>("EnergyTimingSIM_HPD","EnergyTimingSIM_HPD",100,0.,500.,40,-10.,30.);
  hCheckEnergyTimingSIM_HE = FileService->make<TH2F>("EnergyTimingSIM_SiPM","EnergyTimingSIM_SiPM",100,0.,500.,40,-10.,30.);
  hCheckEnergyTimingSIM_HB->GetXaxis()->SetTitle("SimHit energy");
  hCheckEnergyTimingSIM_HB->GetYaxis()->SetTitle("SimHit timing");
  hCheckEnergyTimingSIM_HE->GetXaxis()->SetTitle("SimHit energy");
  hCheckEnergyTimingSIM_HE->GetYaxis()->SetTitle("SimHit timing");


  hCheckT45ratioVST4_barrel = FileService->make<TH2F>("T45ratioVST4_barrel","T45ratioVST4_barrel", 30,0.,3., 500,0.,500.);
  hCheckT45ratioVST4_endcap = FileService->make<TH2F>("T45ratioVST4_endcap","T45ratioVST4_endcap", 30,0.,3., 500,0.,500.);

  hCheckT45ratio_barrel = FileService->make<TH1F>("T45ratio_barrel","T45ratio_barrel", 30,0.,3.);
  hCheckT45ratio_endcap = FileService->make<TH1F>("T45ratio_endcap","T45ratio_endcap", 30,0.,3.);

  hCheckTimingMX_endcap_depth1_gt5 = FileService->make<TH1F>("TimingMX_endcap_depth1_gt5","TimingMX_endcap_depth1_gt5",25,-12.5,12.5);
  hCheckTimingMX_endcap_depth2_gt5 = FileService->make<TH1F>("TimingMX_endcap_depth2_gt5","TimingMX_endcap_depth2_gt5",25,-12.5,12.5);
  hCheckTimingMX_endcap_depth3_gt5 = FileService->make<TH1F>("TimingMX_endcap_depth3_gt5","TimingMX_endcap_depth3_gt5",25,-12.5,12.5);
  hCheckTimingMX_endcap_depth4_gt5 = FileService->make<TH1F>("TimingMX_endcap_depth4_gt5","TimingMX_endcap_depth4_gt5",25,-12.5,12.5);

  hHBHEChi2 = FileService->make<TProfile2D>("hHBHEChi2","hHBHEChi2",59,-29.5,29.5,72,0.5,72.5, 0, 950.,"s");
  hHBHEChi2->GetXaxis()->SetTitle("i#eta");
  hHBHEChi2->GetYaxis()->SetTitle("i#phi");
  hHBHEChi2->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth1 = FileService->make<TProfile2D>("hHBHEChi2_depth1","hHBHEChi2_depth1",59,-29.5,29.5,72,0.5,72.5, 0, 950.,"s");
  hHBHEChi2_depth1->GetXaxis()->SetTitle("i#eta");
  hHBHEChi2_depth1->GetYaxis()->SetTitle("i#phi");
  hHBHEChi2_depth1->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth2 = FileService->make<TProfile2D>("hHBHEChi2_depth2","hHBHEChi2_depth2",59,-29.5,29.5,72,0.5,72.5, 0, 950.,"s");
  hHBHEChi2_depth2->GetXaxis()->SetTitle("i#eta");
  hHBHEChi2_depth2->GetYaxis()->SetTitle("i#phi");
  hHBHEChi2_depth2->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth3 = FileService->make<TProfile2D>("hHBHEChi2_depth3","hHBHEChi2_depth3",59,-29.5,29.5,72,0.5,72.5, 0, 950.,"s");
  hHBHEChi2_depth3->GetXaxis()->SetTitle("i#eta");
  hHBHEChi2_depth3->GetYaxis()->SetTitle("i#phi");
  hHBHEChi2_depth3->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth4 = FileService->make<TProfile2D>("hHBHEChi2_depth4","hHBHEChi2_depth4",59,-29.5,29.5,72,0.5,72.5, 0, 950.,"s");
  hHBHEChi2_depth4->GetXaxis()->SetTitle("i#eta");
  hHBHEChi2_depth4->GetYaxis()->SetTitle("i#phi");
  hHBHEChi2_depth4->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth5 = FileService->make<TProfile2D>("hHBHEChi2_depth5","hHBHEChi2_depth5",59,-29.5,29.5,72,0.5,72.5, 0, 950.,"s");
  hHBHEChi2_depth5->GetXaxis()->SetTitle("i#eta");
  hHBHEChi2_depth5->GetYaxis()->SetTitle("i#phi");
  hHBHEChi2_depth5->GetZaxis()->SetTitle("M2 chi2");

  hHBHEChi2_depth6 = FileService->make<TProfile2D>("hHBHEChi2_depth6","hHBHEChi2_depth6",59,-29.5,29.5,72,0.5,72.5, 0, 950.,"s");
  hHBHEChi2_depth6->GetXaxis()->SetTitle("i#eta");
  hHBHEChi2_depth6->GetYaxis()->SetTitle("i#phi");
  hHBHEChi2_depth6->GetZaxis()->SetTitle("M2 chi2");

  occupancy_d1 = FileService->make<TH2F>("occupancy_d1","occupancy_depth1",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d2 = FileService->make<TH2F>("occupancy_d2","occupancy_depth2",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d3 = FileService->make<TH2F>("occupancy_d3","occupancy_depth3",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d4 = FileService->make<TH2F>("occupancy_d4","occupancy_depth4",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d5 = FileService->make<TH2F>("occupancy_d5","occupancy_depth5",59,-29.5,29.5,72,0.5,72.5);
  occupancy_d6 = FileService->make<TH2F>("occupancy_d6","occupancy_depth6",59,-29.5,29.5,72,0.5,72.5);

  hTimeSlicesAverage_HB = FileService->make<TProfile>("hTimeSlicesAverage_HB","hTimeSlicesAverage_HB",10,0.,10,.0,500.); // 10,0,10 timeslices //.0,500. ADC
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverage_HB->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverage_HB->SetTitle("");
  hTimeSlicesAverage_HB->GetYaxis()->SetTitle("tsAdc");

  hTimeSlicesAverage_HE = FileService->make<TProfile>("hTimeSlicesAverage_HE","hTimeSlicesAverage_HE",10,0.,10,.0,500.); // 10,0,10 timeslices //.0,500. ADC
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverage_HE->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverage_HE->SetTitle("");
  hTimeSlicesAverage_HE->GetYaxis()->SetTitle("tsAdc");

  ///

  hTimeSlicesAverageFC_HB = FileService->make<TProfile>("hTimeSlicesAverageFC_HB","hTimeSlicesAverageFC_HB",10,0.,10,.0,1000.); // 10,0,10 timeslices //.0,500. fC
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverageFC_HB->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverageFC_HB->SetTitle("");
  hTimeSlicesAverageFC_HB->GetYaxis()->SetTitle("fC");


  hTimeSlicesAverageFC_HE = FileService->make<TProfile>("hTimeSlicesAverageFC_HE","hTimeSlicesAverageFC_HE",10,0.,10,.0,700000.); // 10,0,10 timeslices //.0,700000. fC
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverageFC_HE->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverageFC_HE->SetTitle("");
  hTimeSlicesAverageFC_HE->GetYaxis()->SetTitle("fC");


  hTimeSlicesAverageFC_HE_depth1 = FileService->make<TProfile>("hTimeSlicesAverageFC_HE_depth1","hTimeSlicesAverageFC_HE_depth1",10,0.,10,.0,700000.); // 10,0,10 timeslices //.0,700000. fC
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverageFC_HE_depth1->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverageFC_HE_depth1->SetTitle("");
  hTimeSlicesAverageFC_HE_depth1->GetYaxis()->SetTitle("fC");


  hTimeSlicesAverageFC_HE_depthgt1 = FileService->make<TProfile>("hTimeSlicesAverageFC_HE_depthgt1","hTimeSlicesAverageFC_HE_depthgt1",10,0.,10,.0,700000.); // 10,0,10 timeslices //.0,700000. fC
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverageFC_HE_depthgt1->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverageFC_HE_depthgt1->SetTitle("");
  hTimeSlicesAverageFC_HE_depthgt1->GetYaxis()->SetTitle("fC");

  /////
  /////

  hTimeSlicesAverageFC_HB_lowE = FileService->make<TProfile>("hTimeSlicesAverageFC_HB_lowE","hTimeSlicesAverageFC_HB_lowE",10,0.,10,.0,700000.); // 10,0,10 timeslices //.0,700000. fC
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverageFC_HB_lowE->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverageFC_HB_lowE->SetTitle("");
  hTimeSlicesAverageFC_HB_lowE->GetYaxis()->SetTitle("fC");


  /////
  hTimeSlicesAverageFC_HB_highE = FileService->make<TProfile>("hTimeSlicesAverageFC_HB_highE","hTimeSlicesAverageFC_HB_highE",10,0.,10,.0,700000.); // 10,0,10 timeslices //.0,700000. fC
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(1,"TS0");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(2,"TS1");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(3,"TS2");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(4,"TS3");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(5,"TS4");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(6,"TS5");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(7,"TS6");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(8,"TS7");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(9,"TS8");
  hTimeSlicesAverageFC_HB_highE->GetXaxis()->SetBinLabel(10,"TS9");
  hTimeSlicesAverageFC_HB_highE->SetTitle("");
  hTimeSlicesAverageFC_HB_highE->GetYaxis()->SetTitle("fC");

  // simHIT


  hCheckTimeSIMHITM2_HB = FileService->make<TH2F>("TimeSIMHITM2HB","TimeSIMHITM2HB",100,-5.,25.,100,-10.,10.);
  hCheckTimeSIMHITM2_HB->GetXaxis()->SetTitle("SIMHIT Time(HB)");
  hCheckTimeSIMHITM2_HB->GetYaxis()->SetTitle("M2 Time(HB)");

  hCheckTimeSIMHITM2_HE = FileService->make<TH2F>("TimeSIMHITM2HE","TimeSIMHITM2HE",100,-5.,25.,100,-10.,10.);
  hCheckTimeSIMHITM2_HE->GetXaxis()->SetTitle("SIMHIT Time(HE)");
  hCheckTimeSIMHITM2_HE->GetYaxis()->SetTitle("M2 Time(HE)");

  hCheckTimeSIMHITM2_HB1 = FileService->make<TH2F>("TimeSIMHITM2HB1","TimeSIMHITM2HB1",100,-5.,25.,100,-10.,10.);
  hCheckTimeSIMHITM2_HB1->GetXaxis()->SetTitle("SIMHIT Time(HB)");
  hCheckTimeSIMHITM2_HB1->GetYaxis()->SetTitle("M2 Time(HB)");

  hCheckTimeSIMHITM2_HE1 = FileService->make<TH2F>("TimeSIMHITM2HE1","TimeSIMHITM2HE1",100,-5.,25.,100,-10.,10.);
  hCheckTimeSIMHITM2_HE1->GetXaxis()->SetTitle("SIMHIT Time(HE)");
  hCheckTimeSIMHITM2_HE1->GetYaxis()->SetTitle("M2 Time(HE)");


  hCheckEnergySIMHITM2_HB = FileService->make<TH2F>("EnergySIMHITM2HB","EnergySIMHITM2HB",50,0.,500.,50,0.,500.);
  hCheckEnergySIMHITM2_HB->GetXaxis()->SetTitle("SIMHIT Energy (HB)");
  hCheckEnergySIMHITM2_HB->GetYaxis()->SetTitle("M2 Energy (HB)");

  hCheckEnergySIMHITM2_HE = FileService->make<TH2F>("EnergySIMHITM2HE","EnergySIMHITM2",50,0.,500.,50,0.,500.);
  hCheckEnergySIMHITM2_HE->GetXaxis()->SetTitle("SIMHIT Energy (HE)");
  hCheckEnergySIMHITM2_HE->GetYaxis()->SetTitle("M2 Energy (HE)");

  hCheckM2Pull_HB = FileService->make<TH1F>("PullM2_HB","PullM2_HB",200,-2,2.);
  hCheckM2Pull_HB->GetXaxis()->SetTitle("(M2-Sim)/Sim");
  hCheckM2Pull_HE = FileService->make<TH1F>("PullM2_HE","PullM2_HE",200,-2,2.);
  hCheckM2Pull_HE->GetXaxis()->SetTitle("(M2-Sim)/Sim");

  hCheckM2Pull_HE = FileService->make<TH1F>("PullM2_HE","PullM2_HE",200,-2.,2.);
  hCheckM2Pull_HE->GetXaxis()->SetTitle("(M2-Sim)/Sim");

  hCheckM2Pull_HE_depth27 = FileService->make<TH1F>("PullM2_HE_depth27","PullM2_HE_depth27",200,-2.,2.);
  hCheckM2Pull_HE_depth27->GetXaxis()->SetTitle("(M2-Sim)/Sim");

  hCheckM2Pull_HE_depth1 = FileService->make<TH1F>("PullM2_HE_depth1","PullM2_HE_depth1",200,-2.,2.);
  hCheckM2Pull_HE_depth1->GetXaxis()->SetTitle("(M2-Sim)/Sim");


}


MakePhase1Plots::~MakePhase1Plots(){} // destructor

// funtion so you can turn an integer into a string for labels and stuff
std::string MakePhase1Plots::int2string(int i) {
  stringstream ss;
  string ret;
  ss << i;
  ss >> ret;
  return ret;
}

double MakePhase1Plots::sigmaHPDQIE8(double ifC) {
  if(ifC < 75) return (0.577 + 0.0686*ifC)/3.;
  return (2.75  + 0.0373*ifC + 3e-6*ifC*ifC)/3.;
}


double MakePhase1Plots::sigmaSiPMQIE10(double ifC) {
  if(ifC < 200) return (0.7416 + 0.0358*ifC)/3.;
  return (15.225  + 0.0268*ifC + 9e-8*ifC*ifC)/3.;
}

//double MakePhase1Plots::getSiPMDarkCurrent(double darkCurrent, double fcByPE, double lambda) {
double MakePhase1Plots::getSiPMDarkCurrent() {

  //mu = darkCurrent*TS/photoelectronsToAnalog = 0.01*25/44.0 = 0.0056818182 pe = 1/176
  //  rms = sqrt(mu/(1-lambda)^3)*photoelectronsToAnalog = sqrt(1/176/(1-0.17)^3)*44 = 4.386 fC
  //  mean = mu/(1-lambda)*photoelectronsToAnalog = 0.3 fC

  double darkCurrent=0.01;
  double fcByPE=44.0;
  double lambda=0.17;
  double mu = darkCurrent * 25 / fcByPE;
  return sqrt(mu/pow(1-lambda,3)) * fcByPE;
}


// ------------ method called for each event  ------------
void
MakePhase1Plots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  // Read events
  Handle<HBHERecHitCollection> hRecHits; // create handle
  iEvent.getByToken(hRhToken, hRecHits); // get events based on token

  Handle<HBHERecHitCollection> hRecHitsM3; // create handle
  iEvent.getByToken(hRhTokenM3, hRecHitsM3); // get events based on token

  Handle<HBHEChannelInfoCollection> channelData; // create handle
  Handle<PCaloHitContainer> hSimHits;      // create handle

  edm::ESHandle<HcalDDDRecConstants> pHRNDC;
  edm::ESHandle<CaloGeometry> geometry;
  if(!isData) {
    iEvent.getByToken(InfoToken, channelData); // get events based on token
    iEvent.getByToken(hSHitToken, hSimHits);   // SimHits
    
    iSetup.get<HcalRecNumberingRecord>().get( pHRNDC );
    iSetup.get<CaloGeometryRecord>().get( geometry );
  
    theGeometry = &*geometry;
    theRecNumber= &*pHRNDC;

//    std::cout << typeid(theGeometry).name() << std::endl;

    theResponse = new CaloHitResponse(NULL, (CaloShapes*)NULL);

    theResponse->setGeometry(theGeometry);

  }
  if(!isData) {
  // Loop over all rechits in one event
  for(int i = 0; i < (int) channelData->size(); i++) {

    const unsigned cssize = (*channelData)[i].nSamples();
    //    cout << "cssize="<< cssize << endl;

    double tstrig=0;

    double noiseADC_HE=0;
    double noiseDC_HE=0;
    double noisePH_HE=0;
    double totCharge_HE=0;

    double noiseADC_HB=0;
    double noisePH_HB=0;
    double totCharge_HB=0;

    for(unsigned int ip=0; ip<cssize; ++ip){

      double charge = (*channelData)[i].tsRawCharge(ip);
      double ped = (*channelData)[i].tsPedestal(ip); // ped and gain are not function of the timeslices but of the det ?
      double gain = (*channelData)[i].tsGain(ip);
      double energy = (charge-ped)*gain;

      if( ip ==4 || ip==5 ){
	tstrig += energy;
      }

      if((*channelData)[i].hasTimeInfo()) totCharge_HE += charge-ped;
      if((*channelData)[i].hasTimeInfo()) noiseADC_HE += (1./sqrt(12))*(*channelData)[i].tsDFcPerADC(ip);
      if((*channelData)[i].hasTimeInfo()) noiseDC_HE += getSiPMDarkCurrent();
      if((*channelData)[i].hasTimeInfo()) noisePH_HE += 1./sqrt((charge-ped)/44);

      if(!(*channelData)[i].hasTimeInfo()) totCharge_HB += charge-ped;
      if(!(*channelData)[i].hasTimeInfo()) noiseADC_HB += (1./sqrt(12))*(*channelData)[i].tsDFcPerADC(ip);
      if((*channelData)[i].hasTimeInfo()) noisePH_HB += 1./sqrt((charge-ped)/(0.3305));

      if((*channelData)[i].hasTimeInfo()) hCheckEnergyADCnoise_VSfc_endcap->Fill(charge,(1./sqrt(12))*(*channelData)[i].tsDFcPerADC(ip)/charge);
      if(!(*channelData)[i].hasTimeInfo()) hCheckEnergyADCnoise_VSfc_barrel->Fill(charge,(1./sqrt(12))*(*channelData)[i].tsDFcPerADC(ip)/charge);

      if((*channelData)[i].hasTimeInfo()) hCheckEnergyPHnoise_VSfc_endcap->Fill(charge,1./sqrt((charge-ped)/44));
      if(!(*channelData)[i].hasTimeInfo()) hCheckEnergyPHnoise_VSfc_barrel->Fill(charge,1./sqrt((charge-ped)/(0.3305)));

//      noiseDCArr[ip] = 0;
//      if(channelData.hasTimeInfo() && (charge-ped)>channelData.tsPedestalWidth(ip)) {
//	noiseDCArr[ip] = psfPtr_->getSiPMDarkCurrent(channelData.darkCurrent(),channelData.fcByPE(),channelData.lambda());
//      }

    }

    if(tstrig<10 && tstrig<5 ) {
      hCheckCharge_endcap->Fill(totCharge_HE);
      hChecknoiseADC_endcap->Fill(noiseADC_HE);
      hChecknoiseDC_endcap->Fill(noiseDC_HE);

      hCheckCharge_barrel->Fill(totCharge_HB);
      hChecknoiseADC_barrel->Fill(noiseADC_HB);
    }

    for(unsigned int ip=0; ip<cssize; ++ip){

      double charge = (*channelData)[i].tsRawCharge(ip);
      double ped = (*channelData)[i].tsPedestal(ip); // ped and gain are not function of the timeslices but of the det ?
      //      if(tstrig>5) hTimeSlicesAverageFC->Fill(ip,charge-ped);

      if(tstrig>20) {
	if(!(*channelData)[i].hasTimeInfo()) hTimeSlicesAverageFC_HB->Fill(ip,charge-ped);
	if((*channelData)[i].hasTimeInfo()) hTimeSlicesAverageFC_HE->Fill(ip,charge-ped);
	if((*channelData)[i].id().depth()==1) hTimeSlicesAverageFC_HE_depth1->Fill(ip,charge-ped);
	if((*channelData)[i].id().depth()>1) hTimeSlicesAverageFC_HE_depthgt1->Fill(ip,charge-ped);
      }
    }
  }

  }// end ChanelInfo

  /////  /////   /////
  /////  /////   /////
  /////  /////   /////

  double TS4=-100;
  double TS5=0.;

  // Loop over all rechits in one event
  for(int i = 0; i < (int)hRecHits->size(); i++) {
    //    ClearVariables(); // sets a bunch of stuff to zero
    
    RunNumber = iEvent.id().run(); // get the run number for the event
    EvtNumber = iEvent.id().event(); // get the event number
    LumiBlock = iEvent.id().luminosityBlock();

    // Just in case the file you are running over contains events from multiple runs,
    // remove everything except the run you are interested in
    //  if(RunNumber != runNumber_) continue;
    
    // get ID information for the reconstructed hit
    HcalDetId detID_rh = (*hRecHits)[i].id().rawId();

    // ID information can get us detector coordinates
    depth = (*hRecHits)[i].id().depth();
    iEta = detID_rh.ieta();
    iPhi = detID_rh.iphi();
    
    bool HPD=true;
    if(detID_rh.ieta()>=16 && detID_rh.ieta()<=29 && detID_rh.iphi()>=63 && detID_rh.iphi()<=66 ) HPD=false;

    // get some variables
    Method0Energy = (*hRecHits)[i].eraw();
    RecHitEnergy = (*hRecHits)[i].energy();
    RecHitTime = (*hRecHits)[i].time();
    RecHitChi2 = (*hRecHits)[i].chi2();
    RecHitEnergyM3 = (*hRecHits)[i].eaux();

    //    cout << " energy M2= " << RecHitEnergy << " RecHitTime M2=" << RecHitTime << " RecHitChi2=" << RecHitChi2 << endl;
    //    if(Method0Energy>5) cout << "energy="<< Method0Energy << "  ; chi2=" << (*hRecHits)[i].chi2() << endl;
    
    // this block of code is to extract the charge in individual time slices from 
    // the auxiliary information 
                                                                                                                                                                         
    // Not  used at the moment, but useful for trouble-shooting if we identify a
    //  problem with some channels

    //////
    //////
    //////

    if(HPD )  hCheckT45ratioVST4_barrel->Fill(TS5/TS4,TS4);
    if(!HPD )  hCheckT45ratioVST4_endcap->Fill(TS5/TS4,TS4);

    if(HPD )  hCheckT45ratio_barrel->Fill(TS5/TS4);
    if(!HPD )  hCheckT45ratio_endcap->Fill(TS5/TS4);

    //    if(RecHitEnergy > energyCut_)
    if(HPD )  hCheckEnergyM0MX_barrel->Fill(Method0Energy,RecHitEnergy);
    if(!HPD )  hCheckEnergyM0MX_endcap->Fill(Method0Energy,RecHitEnergy);

    if(HPD)  hChi2Energy_barrel->Fill(RecHitEnergy,log10(RecHitChi2));
    if(!HPD )  hChi2Energy_endcap->Fill(RecHitEnergy,log10(RecHitChi2));

    if(depth==1) occupancy_d1->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
    if(depth==2) occupancy_d2->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
    if(depth==3) occupancy_d3->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
    if(depth==4) occupancy_d4->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
    if(depth==5) occupancy_d5->Fill(detID_rh.ieta(), detID_rh.iphi(),1);
    if(depth==6) occupancy_d6->Fill(detID_rh.ieta(), detID_rh.iphi(),1);

    if(RecHitEnergy!=0 && HPD ) {
      hCheckEnergyM2_barrel->Fill(RecHitEnergy);
      hCheckEnergyM2_barrel_zoom->Fill(RecHitEnergy);
      hCheckTimingM2_barrel->Fill(RecHitTime);
    }
    if(RecHitEnergy!=0 && !HPD ) {
      hCheckEnergyM2_endcap->Fill(RecHitEnergy);
      hCheckEnergyM2_endcap_zoom->Fill(RecHitEnergy);
      hCheckTimingM2_endcap->Fill(RecHitTime);
    }

    if(Method0Energy!=0 && HPD ) {
      hCheckEnergyM0_barrel->Fill(Method0Energy);
      hCheckEnergyM0_barrel_zoom->Fill(Method0Energy);
    }
    if(Method0Energy!=0 && !HPD ) {
      hCheckEnergyM0_endcap->Fill(Method0Energy);
      hCheckEnergyM0_endcap_zoom->Fill(Method0Energy);
    }

    if(Method0Energy>1 && !HPD ) {
      if(depth==1) hCheckEnergyM2_endcap_depth1->Fill(RecHitEnergy);
      if(depth==2) hCheckEnergyM2_endcap_depth2->Fill(RecHitEnergy);
      if(depth==3) hCheckEnergyM2_endcap_depth3->Fill(RecHitEnergy);
      if(depth==4) hCheckEnergyM2_endcap_depth4->Fill(RecHitEnergy);
      if(depth==5) hCheckEnergyM2_endcap_depth5->Fill(RecHitEnergy);
      if(depth==6) hCheckEnergyM2_endcap_depth6->Fill(RecHitEnergy);
    }

    if( HPD ) hCheckEnergyTiming_barrel->Fill(RecHitEnergy,RecHitTime);
    if( !HPD  ) hCheckEnergyTiming_endcap->Fill(RecHitEnergy,RecHitTime);

    if(Method0Energy<5) {
      if(HPD )  hCheckChi2MX_barrel_lt5->Fill(log10(RecHitChi2));
      if(!HPD )  hCheckChi2MX_endcap_lt5->Fill(log10(RecHitChi2));
    }

    if(Method0Energy>5) {

      if( HPD )  hCheckChi2MX_barrel_gt5->Fill(log10(RecHitChi2));
      if( !HPD )  hCheckChi2MX_endcap_gt5->Fill(log10(RecHitChi2));

      if(!HPD ) {
	if(Method0Energy>5 && depth==1) { hHBHEChi2_depth1->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2); hCheckChi2MX_endcap_depth1_gt5->Fill(log10(RecHitChi2)); }
	if(Method0Energy>5 && depth==2) { hHBHEChi2_depth2->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2); hCheckChi2MX_endcap_depth2_gt5->Fill(log10(RecHitChi2)); }
	if(Method0Energy>5 && depth==3) { hHBHEChi2_depth3->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2); hCheckChi2MX_endcap_depth3_gt5->Fill(log10(RecHitChi2)); }
	if(Method0Energy>5 && depth==4) { hHBHEChi2_depth4->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2); hCheckChi2MX_endcap_depth4_gt5->Fill(log10(RecHitChi2)); }
	if(Method0Energy>5 && depth==5) { hHBHEChi2_depth5->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2); hCheckChi2MX_endcap_depth5_gt5->Fill(log10(RecHitChi2)); }
	if(Method0Energy>5 && depth==6) { hHBHEChi2_depth6->Fill(detID_rh.ieta(), detID_rh.iphi(), RecHitChi2); hCheckChi2MX_endcap_depth6_gt5->Fill(log10(RecHitChi2)); }
      }

      if( HPD )  hCheckTimingMX_barrel_gt5->Fill(RecHitTime);
      if( !HPD )  hCheckTimingMX_endcap_gt5->Fill(RecHitTime);

      if( !HPD ) {
	if(depth==1) hCheckTimingMX_endcap_depth1_gt5->Fill(RecHitTime);
	if(depth==2) hCheckTimingMX_endcap_depth2_gt5->Fill(RecHitTime);
	if(depth==3) hCheckTimingMX_endcap_depth3_gt5->Fill(RecHitTime);
	if(depth==4) hCheckTimingMX_endcap_depth4_gt5->Fill(RecHitTime);

      }
    }

    //$$$$$$$$$$$$$$$
    // CORRELATION WITH M3
    //$$$$$$$$$$$$$$$

    if(!HPD) {

      if(RecHitEnergy>5) hCheckEnergyM2M3ratio_endcap_gt5->Fill(RecHitEnergyM3/RecHitEnergy);

      hCheckEnergyM2M3_endcap->Fill(RecHitEnergy,RecHitEnergyM3);
      hCheckEnergyM2M3_endcap_zoom->Fill(RecHitEnergy,RecHitEnergyM3);
      hCheckEnergyM2M3ratio_endcap_vsM2E->Fill(RecHitEnergy,RecHitEnergyM3/RecHitEnergy);
      hCheckEnergyM2M3ratio_endcap_vsM2E_zoom->Fill(RecHitEnergy,RecHitEnergyM3/RecHitEnergy);
      hCheckEnergyM2M3ratio_endcap->Fill(RecHitEnergyM3/RecHitEnergy);

      hCheckEnergyM2M0_endcap->Fill(RecHitEnergy,Method0Energy);
      hCheckEnergyM2M0_endcap_zoom->Fill(RecHitEnergy,Method0Energy);
      hCheckEnergyM2M0ratio_endcap_vsM2E->Fill(RecHitEnergy,Method0Energy/RecHitEnergy);
      hCheckEnergyM2M0ratio_endcap_vsM2E_zoom->Fill(RecHitEnergy,Method0Energy/RecHitEnergy);
      hCheckEnergyM2M0ratio_endcap->Fill(Method0Energy/RecHitEnergy);

    }
    if(HPD) {

      if(RecHitEnergy>5) hCheckEnergyM2M3ratio_barrel_gt5->Fill(RecHitEnergyM3/RecHitEnergy);

      hCheckEnergyM2M3_barrel->Fill(RecHitEnergy,RecHitEnergyM3);
      hCheckEnergyM2M3_barrel_zoom->Fill(RecHitEnergy,RecHitEnergyM3);
      hCheckEnergyM2M3ratio_barrel_vsM2E->Fill(RecHitEnergy,RecHitEnergyM3/RecHitEnergy);
      hCheckEnergyM2M3ratio_barrel_vsM2E_zoom->Fill(RecHitEnergy,RecHitEnergyM3/RecHitEnergy);
      hCheckEnergyM2M3ratio_barrel->Fill(RecHitEnergyM3/RecHitEnergy);

      hCheckEnergyM2M0_barrel->Fill(RecHitEnergy,Method0Energy);
      hCheckEnergyM2M0_barrel_zoom->Fill(RecHitEnergy,Method0Energy);
      hCheckEnergyM2M0ratio_barrel_vsM2E->Fill(RecHitEnergy,Method0Energy/RecHitEnergy);
      hCheckEnergyM2M0ratio_barrel_vsM2E_zoom->Fill(RecHitEnergy,Method0Energy/RecHitEnergy);
      hCheckEnergyM2M0ratio_barrel->Fill(Method0Energy/RecHitEnergy);
    }

    if(!isData) {
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    /// CORRELATION WITH SIMHITS
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    //  cout << " going to read SamplingFactor" << endl;

    double SamplingFactor = 1;
    if(detID_rh.subdet() == HcalBarrel) {
      SamplingFactor = simParameterMap_.hbParameters().samplingFactor(detID_rh);
    } else if (detID_rh.subdet() == HcalEndcap) {
      SamplingFactor = simParameterMap_.heParameters().samplingFactor(detID_rh);
    }

    //    cout << " SamplingFactor done" << endl;

    double SHitEn=0;
    double MaxEn=0; 
    int MaxEnk=-1; 
    HcalDetId simIdneed;
    double SHitTime = -99.; 
    for (int j=0; j < (int) hSimHits->size(); j++) {

      HcalDetId simId = HcalHitRelabeller::relabel((*hSimHits)[j].id(), theRecNumber);

      // check that the SimHits are in the same channel as the rechit
      //      if (simId == detID_rh && (*hSimHits)[j].time() > 0 && (*hSimHits)[j].time() < 40)
      if (simId == detID_rh ){
	SHitEn += SamplingFactor*((*hSimHits)[j].energy());
        if((*hSimHits)[j].energy() > MaxEn){
                MaxEnk = j;
                simIdneed = simId;
                MaxEn = (*hSimHits)[j].energy();
        }
      }
      //std::cout << theResponse->timeOfFlight(simId) << std::endl;
    }

    //    cout << "SHitEn = " << SHitEn << endl;

    if(MaxEn>0) {
	double tof = theResponse->timeOfFlight(simIdneed);
        SHitTime = (*hSimHits)[MaxEnk].time() - tof;
    }

    if(MaxEn>0 && fabs((*hRecHits)[i].time())<40. && HPD) hCheckTimeSIMHITM2_HB->Fill(SHitTime,(*hRecHits)[i].time());
    if(MaxEn>0 && SHitEn>0 && HPD) hCheckEnergyTimingSIM_HB->Fill(SHitEn,SHitTime);
    if(MaxEn>0 && fabs((*hRecHits)[i].time())<40. && !HPD)hCheckTimeSIMHITM2_HE->Fill(SHitTime,(*hRecHits)[i].time());
    if(MaxEn>0 && SHitEn>0 && !HPD)hCheckEnergyTimingSIM_HE->Fill(SHitEn,SHitTime);
    
    if(MaxEn>0 && SHitEn>10 && fabs((*hRecHits)[i].time())<40. && HPD) hCheckTimeSIMHITM2_HB1->Fill(SHitTime,(*hRecHits)[i].time());
    if(MaxEn>0 && SHitEn>10 && fabs((*hRecHits)[i].time())<40. && !HPD) hCheckTimeSIMHITM2_HE1->Fill(SHitTime,(*hRecHits)[i].time());


    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && HPD) hCheckEnergySIMHITM2_HB->Fill(SHitEn,(*hRecHits)[i].energy());
    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && !HPD) hCheckEnergySIMHITM2_HE->Fill(SHitEn,(*hRecHits)[i].energy());

    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && HPD) hCheckM2Pull_HB->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);
    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && !HPD) hCheckM2Pull_HE->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);

    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && !HPD && depth>1) hCheckM2Pull_HE_depth27->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);
    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && !HPD && depth>1) hCheckM2Pull_HE_depth27->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);
    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && !HPD && depth==1) hCheckM2Pull_HE_depth1->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);
    if(SHitEn>0 && (*hRecHits)[i].energy()>0 && !HPD && depth==1) hCheckM2Pull_HE_depth1->Fill(((*hRecHits)[i].energy()-SHitEn)/SHitEn);


    //$$$$$$$$$$$$$$$
    /// Plotting individual pulses
    //$$$$$$$$$$$$$$$


    if(SHitEn>0) {

      //    cout << "SHitEn = " << SHitEn << endl;
    // example on how to fill the time slices if you need it
    if( ((*hRecHits)[i].energy()/SHitEn)<0.6 && (*hRecHits)[i].energy()>50)  {

      hTimeSlices = FileService->make<TH1F>(Form("hTimeSlices_%4.2f",Method0Energy),Form("hTimeSlices_%4.2f",Method0Energy),10,-100,150);
      hTimeSlices->GetXaxis()->SetBinLabel(1,"TS0");
      hTimeSlices->GetXaxis()->SetBinLabel(2,"TS1");
      hTimeSlices->GetXaxis()->SetBinLabel(3,"TS2");
      hTimeSlices->GetXaxis()->SetBinLabel(4,"TS3");
      hTimeSlices->GetXaxis()->SetBinLabel(5,"TS4");
      hTimeSlices->GetXaxis()->SetBinLabel(6,"TS5");
      hTimeSlices->GetXaxis()->SetBinLabel(7,"TS6");
      hTimeSlices->GetXaxis()->SetBinLabel(8,"TS7");
      hTimeSlices->GetXaxis()->SetBinLabel(9,"TS8");
      hTimeSlices->GetXaxis()->SetBinLabel(10,"TS9");

      hTimeSlices->SetTitle("");
      hTimeSlices->GetYaxis()->SetTitle("fC");

      gStyle->SetOptStat(000000);
    }// end simHits
    }

    //    cout << "done with SimHit" << endl;

      double ped=0;
    /*  
    ///////////
      ///////////
      if(!isData) {
      // Loop over all rechits in one event
      for(int k = 0; k < (int) channelData->size(); k++) {

	if((*channelData)[k].id()!=detID_rh) continue;
	const unsigned cssize = (*channelData)[k].nSamples();

	for(unsigned int ip=0; ip<cssize; ++ip){
	  double charge = (*channelData)[k].tsRawCharge(ip);
	  ped= (*channelData)[k].tsPedestal(ip);
	  hTimeSlices->SetBinContent(ip+1,charge);
	  cout << "ip=" << ip << "charge=" << charge << endl;
	}
      }
      }
    */

      /*
      hTimeSlices->SetLineWidth(3);
      hTimeSlices->Draw();

      /////////////////
      //pedestal Line

      //      double ped=3;
      if(std::abs(iEta) < 14) ped=3.; // these are fC
      if(std::abs(iEta) >=19  && std::abs(iEta)<=26) ped=17.3; // these are fC

      TLine *line = new TLine(hTimeSlices->GetXaxis()->GetBinLowEdge(0),ped,hTimeSlices->GetXaxis()->GetBinUpEdge(hTimeSlices->GetXaxis()->GetNbins()),ped);
      line->SetLineColor(kRed);
      line->SetLineWidth(3);
      line->Draw("same");

      /////////////

      TLatex *t = new TLatex();
      t->SetTextFont(32);
      t->SetTextColor(1);
      //      t->SetTextSize(0.08);
      t->SetTextAlign(12);
      t->SetTextSize(0.05);
      t->DrawLatex(hTimeSlices->GetXaxis()->GetBinLowEdge(1)+1.0,1.1*hTimeSlices->GetMaximum(),Form("M0=%4.1f   M2=%4.1f   M2time=%4.1f  log_{10}(M2chi2)=%4.1f",Method0Energy,
												    RecHitEnergy,RecHitTime,log10(RecHitChi2)));
      t->DrawLatex(hTimeSlices->GetXaxis()->GetBinLowEdge(1)+20.0,0.95*hTimeSlices->GetMaximum(),Form("i#eta=%d",detID_rh.ieta()));
      t->DrawLatex(hTimeSlices->GetXaxis()->GetBinLowEdge(1)+20.0,0.9*hTimeSlices->GetMaximum(),Form("i#phi=%d",detID_rh.iphi()));
      t->DrawLatex(hTimeSlices->GetXaxis()->GetBinLowEdge(1)+20.0,0.85*hTimeSlices->GetMaximum(),Form("depth=%d",depth));

      //      t->DrawLatex(0.,0.75*hTimeSlices->GetMaximum(),Form("M0=%4.2f",Method0Energy));
      //      t->DrawLatex(0.15,0.65*hTimeSlices->GetMaximum(),Form("M2=%4.2f",Method2Energy));


      // comment the plotting
      //      if(std::abs(iEta) < 14) c1->SaveAs(Form("Pulse_Barrel_%d_%d_%d_Energy%4.2f.png",RunNumber,LumiBlock,EvtNumber,Method0Energy));
      if(std::abs(iEta) >=19  && std::abs(iEta)<=26) c1->SaveAs(Form("Pulse_Endcap_%d_%d_%d_Energy%4.2f.png",RunNumber,LumiBlock,EvtNumber,Method0Energy));
      //("test_phi"+int2string(detID_rh.iphi())+"eta"+int2string(detID_rh.ieta())+"energy"+int2string((int)RecHitEnergy)+"time"+int2string((int)RecHitTime)+".png").c_str());
      ///       ++nProblems;

      */
    }


    //$$$$$$$$$$$$$$$
    /// Plotting individual pulses
    //$$$$$$$$$$$$$$$

    int adc[10];

    int auxwd1 = (*hRecHits)[i].aux();  // TS = 0,1,2,3 info
    int auxwd2 = (*hRecHits)[i].auxHBHE();      // TS = 4,5,6,7 info
    int auxwd3 = (*hRecHits)[i].auxPhase1();      // TS = 8,9 info

    //    int flags = (*hRecHits)[i].flags();

    // 0x7F --> 7bits 127
    // 0xFF --> 8bits 255

    //
    adc[0] = (auxwd1)       & 0xFF;
    adc[1] = (auxwd1 >> 8)  & 0xFF;
    adc[2] = (auxwd1 >> 16) & 0xFF;
    adc[3] = (auxwd1 >> 24) & 0xFF;

    adc[4] = (auxwd2)       & 0xFF;
    adc[5] = (auxwd2 >> 8)  & 0xFF;
    adc[6] = (auxwd2 >> 16) & 0xFF;
    adc[7] = (auxwd2 >> 24) & 0xFF;

    adc[8] = (auxwd3 )      & 0xFF;
    adc[9] = (auxwd3 >> 8)  & 0xFF;

    TS4=adc[4];
    TS5=adc[5];

    /*
    if(Method0Energy>5){
      for(int nadc = 0; nadc < 10; nadc++){
	if(std::abs(iEta) < 15) {
	  hTimeSlicesAverage_HB->Fill(nadc,adc[nadc]);
	} else {
	  hTimeSlicesAverage_HE->Fill(nadc,adc[nadc]);
	}
      }
    }


    // example on how to fill the time slices if you need it
    if( ((*hRecHits)[i].energy()/SHitEn)<0.6 && (*hRecHits)[i].energy()>50)  {

    //    if(Method0Energy > 20){
    //    if((RecHitEnergy/Method0Energy)<0.8 && Method0Energy>200){
    //    if(RecHitChi2>20000 && Method0Energy>10){

      hTimeSlices = FileService->make<TH1F>(Form("hTimeSlices_%4.2f",Method0Energy),Form("hTimeSlices_%4.2f",Method0Energy),10,-100,150);

      std::cout << "event number is " << EvtNumber << std::endl;
      std::cout << "iphi, ieta = " << detID_rh.iphi() << " " << detID_rh.ieta() << " time = " << RecHitTime << " energy = " << RecHitEnergy << std::endl;
      for(int nadc = 0; nadc < 10; nadc++){
	hTimeSlices->SetBinContent(nadc+1,adc[nadc]);
	std::cout << adc[nadc] << "  " ;
      }
      std::cout << std::endl;

    }

    */

  } // recHit

}


// ------------ method called once each job just before starting event loop  ------------
void MakePhase1Plots::beginJob(){}

// ------------ method called once each job just after ending the event loop  ------------
void MakePhase1Plots::endJob(){}

void MakePhase1Plots::ClearVariables(){
 RecHitEnergy = 0;
 RunNumber = 0;
 depth=0;
 iEta = 0;
 iPhi = 0;
 RecHitTime = 0;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MakePhase1Plots::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakePhase1Plots);
