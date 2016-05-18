import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet (wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    
    #2016B JetHT
    #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760JetHT.root",
    #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761JetHT.root",
    #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762JetHT.root"
    
    #2015 JetHT
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_1.root",
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_2.root",
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_3.root",
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_4.root",
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_5.root",
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_6.root",
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_7.root",
    # "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run256677v2_8.root",
    
     #2012C JetHT
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_1.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_2.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_3.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_4.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_5.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_6.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_7.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run199812JetHT_8.root",
     
     #2016B ZeroBias dataset
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run268958wNF_1.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run268958wNF_2.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run268958wNF_3.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run268958wNF_4.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run268958wNF_5.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run268958wNF_6.root",    
     
     # 2016B HLTPhysics* datasets
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT0.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT1.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT2.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT3.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT0.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT1.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT2.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT3.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT0.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT1.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT2.root",
     #"root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT3.root",
     
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run198588zb.root",
     
     
    #run254532_SinglePulse.root"
    )
)

process.timingMaps = cms.EDAnalyzer('MakeTimingMaps')
process.timingMaps.runNumber = cms.int32(272762)
process.timingMaps.rechitEnergy = cms.double(5.0)
process.timingMaps.timeLowBound = cms.double(-37.5)
process.timingMaps.timeHighBound = cms.double(37.5)

process.TFileService = cms.Service('TFileService', fileName = cms.string('run198588zb.root') )


process.p = cms.Path(process.timingMaps)
