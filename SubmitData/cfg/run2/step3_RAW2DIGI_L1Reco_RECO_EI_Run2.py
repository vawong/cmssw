# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --runUnscheduled --conditions auto:run2_mc -s RAW2DIGI,L1Reco,RECO,EI,PAT --datatier GEN-SIM-RECO -n -1 --era Run2_2016 --eventcontent RECOSIM --filein file:step2_TTbar_13TeV.root --fileout file:step3.root --nThreads=16 --customise Validation/Performance/TimeMemoryInfo.py 
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RECO',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input Added Timing Monitoring
##process.Timing = cms.Service("Timing")

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('root://eoscms//eos/cms//store/cmst3/user/dalfonso/HCAL/8_1_0_pre13/step2_pi500_2016run2_ev10k.root'),
    fileNames = cms.untracked.vstring('root://eoscms.cern.ch//store/group/dpg_hcal/comm_hcal/RecoAlgos/SinglePion/LocalGen/Run2/step2.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)


#Method 2
process.hbherecoM2 = process.hbheprereco.clone()
#process.hbherecoM2.applyTimeConstraint = cms.bool(False)
#process.hbherecoM2.applyPedConstraint = cms.bool(False)
#process.hbherecoM2.timeSigma = cms.double(2.5)   #ns
process.hbherecoM2.puCorrMethod = cms.int32(2)
process.hbherecoM2.pulseShapeType = cms.int32(1)
##process.hbhereco.applyTimeSlew = cms.bool(False)

# Method 2 collection
process.hbherecoM2csv = process.hbheprereco.clone()
process.hbherecoM2csv.puCorrMethod = cms.int32(2)
process.hbherecoM2csv.pulseShapeType = cms.int32(2)
process.hbherecoM2lagcsv = process.hbheprereco.clone()
process.hbherecoM2lagcsv.puCorrMethod = cms.int32(2)
process.hbherecoM2lagcsv.pulseShapeType = cms.int32(3)

# Method 3 collection
process.hbherecoM3 = process.hbheprereco.clone()
process.hbherecoM3.puCorrMethod = cms.int32(3)

# Method 0 collection
#process.hbherecoM0 = process.hbheprereco.clone()
#process.hbherecoM0.puCorrMethod = cms.int32(0)

# MAHI collection
process.hbherecoMAHIlagcsv = process.hbheprereco.clone()
process.hbherecoMAHIlagcsv.puCorrMethod = cms.int32(10)
process.hbherecoMAHIlagcsv.pulseShapeType = cms.int32(3)

# Set Method 2 to use a single pulse fit
#process.hbhereco.puCorrMethod = cms.int32(2)
#process.hbhereco.ts4chi2 = cms.double(99999.)
#process.hbhereco.timeMin = cms.double(-100.)
#process.hbhereco.timeMax = cms.double(100.)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:../step3.root'),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.RECOSIMoutput.outputCommands.append( "drop *")
process.RECOSIMoutput.outputCommands.append( "keep HBHERecHits*_*_*_*")
process.RECOSIMoutput.outputCommands.append( "keep HORecHits*_*_*_*")
process.RECOSIMoutput.outputCommands.append( "keep HcalNoiseSummary*_*_*_*")
process.RECOSIMoutput.outputCommands.append( "keep *_g4SimHits*_*_*")

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
#process.reconstruction_step = cms.Path(process.reconstruction* process.hbherecoM2csv* process.hbherecoM2lagcsv * process.hbherecoM3 * process.hbherecoM0 * process.hbherecoMAHIcsv * process.hbherecoMAHIlagcsv )
process.reconstruction_step = cms.Path(process.reconstruction* process.hbherecoM2 * process.hbherecoM2csv* process.hbherecoM2lagcsv * process.hbherecoM3 * process.hbherecoMAHIlagcsv )
##process.reconstruction_step = cms.Path(process.reconstruction)
process.eventinterpretaion_step = cms.Path(process.EIsequence)

process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.eventinterpretaion_step,process.endjob_step,process.RECOSIMoutput_step)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)

from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)

# customisation of the process.

# Automatic addition of the customisation function from Validation.Performance.TimeMemoryInfo
from Validation.Performance.TimeMemoryInfo import customise

#call to customisation function customise imported from Validation.Performance.TimeMemoryInfo
#process = customise(process)

# Automatic addition of the customisation function from PhysicsTools.PatAlgos.slimming.miniAOD_tools

# Reduce printing delay
process.MessageLogger.cerr.FwkReport.reportEvery = 100

#call to customisation function miniAOD_customizeAllMC imported from PhysicsTools.PatAlgos.slimming.miniAOD_tools

# End of customisation functions

# Customisation from command line

dumpFile  = open("../DumpRECO_RUN2.py", "w")
dumpFile.write(process.dumpPython())
dumpFile.close()
