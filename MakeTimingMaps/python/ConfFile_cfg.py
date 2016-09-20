import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet (wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

        'file:$CMSSW_BASE/src/HBHETimingValidation/SubmitData/cfg/step3_phase1.root'

    )
)

process.timingMaps = cms.EDAnalyzer('MakePhase1Plots')

process.TFileService = cms.Service('TFileService', fileName = cms.string('phase1Plot.root') )


process.p = cms.Path(process.timingMaps)
