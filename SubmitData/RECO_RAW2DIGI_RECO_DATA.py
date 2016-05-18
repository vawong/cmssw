import FWCore.ParameterSet.Config as cms


from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_25ns)

# import of standard configurations                                             
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
#'file:root://cmsxrootd.fnal.gov//store/data/Run2016B/JetHT/RAW/v1/000/272/760/00000/7A42AE55-D513-E611-9D36-02163E011976.root'),
    #'root://cmsxrootd.fnal.gov//store/data/Run2016B/HLTPhysics3/RAW/v1/000/272/022/00000/6E3580B0-C70D-E611-9629-02163E014468.root'),
'file:root://eoscms.cern.ch//store/data/Run2012C/JetHT/RAW/v1/000/199/812/0C450FA0-6BD8-E111-BB28-BCAEC5329727.root'),
    skipEvents= cms.untracked.uint32(0),
    #eventsToProcess = cms.untracked.VEventRange('208353:332796231'),
    #lumisToProcess = cms.untracked.VLuminosityBlockRange('208353:278')
)

process.options = cms.untracked.PSet(
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
   version = cms.untracked.string('$Revision: 1.19 $'),
   annotation = cms.untracked.string('RECO nevts:1'),
   name = cms.untracked.string('Applications')
)

process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')

# HBHE Noise Filter

process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResultRun2Loose'),
    reverseDecision = cms.bool(False)
)

process.HBHENoiseFilterResultProducer = cms.EDProducer(
   'HBHENoiseFilterResultProducer',
    noiselabel = cms.InputTag('hcalnoise'),
    minHPDHits = cms.int32(17),
    minHPDNoOtherHits = cms.int32(10),
    minZeros = cms.int32(999),
    IgnoreTS4TS5ifJetInLowBVRegion = cms.bool(False),
    defaultDecision = cms.string("HBHENoiseFilterResultRun2Loose"),
    minNumIsolatedNoiseChannels = cms.int32(10),
    useBunchSpacingProducer = cms.bool(True),
    minIsolatedNoiseSumE = cms.double(50.0),
    minIsolatedNoiseSumEt = cms.double(25.0)
)

#process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
#    inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
#    reverseDecision = cms.bool(False)
#)

#process.load('RecoLocalCalo.HcalRecProducers.HcalHitReconstructorM3_hbhe_cfi')
#process.hcalLocalRecoSequenceHLT = cms.Sequence(process.hbheprerecoM3)

#process.load('RecoLocalCalo.HcalRecProducers.HcalHitReconstructorM2Single_hbhe_cfi')
#process.hcalLocalRecoSequenceSingle = cms.Sequence(process.hbheprerecoM2Single)

#process.load('RecoLocalCalo.HcalRecProducers.HcalHitReconstructorM0_hbhe_cfi')
#process.hcalLocalRecoSequenceMethod0 = cms.Sequence(process.hbheprerecoM0)


# Output definition                                                                                                                                
process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(1048576),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    fileName = cms.untracked.string('file:DataRereco.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    ),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('reconstruction_step'))
)

process.FEVTDEBUGHLToutput.outputCommands.append( "drop *")
process.FEVTDEBUGHLToutput.outputCommands.append( "keep HBHERecHits*_*_*_*")
process.FEVTDEBUGHLToutput.outputCommands.append( "keep HcalNoiseSummary*_*_*_*")


# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')


process.hbhereco = process.hbheprereco.clone()
process.hbherecoM3 = process.hbheprereco.clone()
process.hbherecoM0 = process.hbheprereco.clone()

# Method 3 collection
process.hbherecoM3.puCorrMethod = cms.int32(3)

#Method 0 collection
process.hbherecoM0.puCorrMethod = cms.int32(0)

# Set Method 2 to use a single pulse fit
process.hbhereco.puCorrMethod = cms.int32(2)
process.hbhereco.ts4chi2 = cms.double(99999.)
process.hbhereco.timeMin = cms.double(-100.)
process.hbhereco.timeMax = cms.double(100.)
process.hbhereco.applyTimeConstraint = cms.bool(False)


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction * process.hbherecoM3 * process.hbherecoM0 *process.hcalnoise* process.HBHENoiseFilterResultProducer * process.ApplyBaselineHBHENoiseFilter)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)


# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.postLS1Customs
from SLHCUpgradeSimulations.Configuration.postLS1Customs import customisePostLS1

#call to customisation function customisePostLS1 imported from SLHCUpgradeSimulations.Configuration.postLS1Customs
#process = customisePostLS1(process)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.FEVTDEBUGHLToutput_step,process.endjob_step)
    
