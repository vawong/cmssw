import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet (wantSummary = cms.untracked.bool(False))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_117.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_118.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_122.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_123.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_124.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_128.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_129.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_130.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_134.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_135.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_336.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_337.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_338.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_339.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_340.root",
    "root://eoscms.cern.ch//store/user/kfiekas/HCAL/Run2Data/JetHT/crab_DataRerecoMethod2SinglePulseRun254532V2/150821_090253/0000/DataRereco_341.root",
    )
)

process.timingMaps = cms.EDAnalyzer('MakeTimingMaps')
process.timingMaps.runNumber = cms.int32(254532)

process.TFileService = cms.Service('TFileService', fileName = cms.string('runTime_254532_test2.root') )


process.p = cms.Path(process.timingMaps)
