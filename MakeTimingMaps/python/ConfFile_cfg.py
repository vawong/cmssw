import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet (wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/run254532_SinglePulse.root"
    )
)

process.timingMaps = cms.EDAnalyzer('MakeTimingMaps')
process.timingMaps.runNumber = cms.int32(254532)

process.TFileService = cms.Service('TFileService', fileName = cms.string('runTime_254532.root') )


process.p = cms.Path(process.timingMaps)
