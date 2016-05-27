import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet (wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    
     
     # 2016B HLTPhysics* datasets
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT0.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT1.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT2.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272760HLT3.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT0.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT1.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT2.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272761HLT3.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT0.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT1.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT2.root",
     "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run272762HLT3.root",

     
     
    #run254532_SinglePulse.root"
    )
)

process.timingMaps = cms.EDAnalyzer('MakeTimingMaps')
process.timingMaps.runNumber = cms.int32(273450)
process.timingMaps.rechitEnergy = cms.double(5.0)
process.timingMaps.timeLowBound = cms.double(-12.5)
process.timingMaps.timeHighBound = cms.double(12.5)

process.TFileService = cms.Service('TFileService', fileName = cms.string('run2016B_HLT.root') )


process.p = cms.Path(process.timingMaps)
