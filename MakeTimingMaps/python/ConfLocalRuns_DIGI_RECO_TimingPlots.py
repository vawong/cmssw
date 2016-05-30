import FWCore.ParameterSet.Config as cms
# run DIGI-RECO and then plots on the local runs 

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load('CondCore.CondDB.CondDB_cfi')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.Geometry.GeometryExtended2016Reco_cff")
#process.load('Configuration.Geometry.GeometryIdeal_cff')
#process.load("Configuration.Geometry.GeometryRecoDB_cff")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.EventContent.EventContent_cff')
#process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.options = cms.untracked.PSet (wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
process.MessageLogger.cerr.FwkReport.reportEvery = 50


process.source = cms.Source("HcalTBSource",
    #fileNames = cms.untracked.vstring('file:root://eoscms.cern.ch//store/group/dpg_hcal/comm_hcal/LS1/USC_273617.root'),
fileNames = cms.untracked.vstring('file:root://eoscms.cern.ch//store/group/dpg_hcal/comm_hcal/USC/USC_184254.root'),
)

process.hcalDigis.InputLabel = cms.InputTag("source")


# Other statements
process.GlobalTag.globaltag = '80X_dataRun2_HLT_v12'

process.hbheprereco.puCorrMethod = cms.int32(2)
process.hbheprereco.ts4chi2 = cms.double(99999.)
process.hbheprereco.timeMin = cms.double(-100.)
process.hbheprereco.timeMax = cms.double(100.)
process.hbheprereco.applyTimeConstraint = cms.bool(False)
process.hcalLocalRecoSequence = cms.Sequence(process.hbheprereco)
#process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring(
    
     #'file:../Laser/test.root'
     
    ##run254532_SinglePulse.root"
    #)
#)

#process.raw2digi_step = cms.Path(process.hcalDigis)
#process.reco_step1 = cms.Path(process.hcalLocalRecoSequence)
#process.endjob_step = cms.EndPath(process.endOfProcess)
#process.RECOoutput_step = cms.EndPath(process.RECOoutput)

process.timingMaps = cms.EDAnalyzer('MakeTimingMaps')
process.timingMaps.runNumber = cms.int32(272762)
process.timingMaps.rechitEnergy = cms.double(5.0)
process.timingMaps.timeLowBound = cms.double(-37.5)
process.timingMaps.timeHighBound = cms.double(37.5)

process.TFileService = cms.Service('TFileService', fileName = cms.string('USC_184254.root') )


process.p = cms.Path(process.hcalDigis * process.hcalLocalRecoSequence * process.timingMaps)
