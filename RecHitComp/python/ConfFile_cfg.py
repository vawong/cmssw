import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet (wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "root://eoscms.cern.ch//store/user/sabrandt/HCAL_Timing_Study/2015_08_21/Method2/Method2_21.root",
    )
)

process.RecHitComp = cms.EDAnalyzer('RecHitComp')
process.RecHitComp.runNumber = cms.int32(251721)
process.RecHitComp.printHists = cms.bool(True)
process.RecHitComp.lumiSectionCut = cms.int32(75)

process.TFileService = cms.Service('TFileService', fileName = cms.string('rhComp_251271_m2m0.root') )


process.p = cms.Path(process.RecHitComp)