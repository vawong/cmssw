# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --runUnscheduled --conditions auto:phase1_2017_hcaldev -n 10 --era Run2_2017_HCALdev --eventcontent RECOSIM -s RAW2DIGI,L1Reco,RECO,EI --datatier GEN-SIM-RECO --geometry Extended2017dev -n 100 --nThreads 16
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('CALMit',eras.Run2_2017)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(

'file:step2_pi500_2017_realistic.root'

# single pion without PU                                
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/SinglePion/LocalGen/Phase1/step2_pi1-100_2017_realistic.root'

## this is w/o PU
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/Samples_90_pre2/RelValTTbar_13_GEN-SIM-DIGI-RAW_90X_upgrade2017_realistic_v0-v1/34D0D955-66C3-E611-96C0-0025905A48F0.root',
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/Samples_90_pre2/RelValTTbar_13_GEN-SIM-DIGI-RAW_90X_upgrade2017_realistic_v0-v1/546FB65F-66C3-E611-9B5C-0025905A6110.root',
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/Samples_90_pre2/RelValTTbar_13_GEN-SIM-DIGI-RAW_90X_upgrade2017_realistic_v0-v1/6450855A-48C2-E611-9BF6-0CC47A7C3422.root',
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/Samples_90_pre2/RelValTTbar_13_GEN-SIM-DIGI-RAW_90X_upgrade2017_realistic_v0-v1/6AAA0160-48C2-E611-A4BD-0CC47A7C345C.root',
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/Samples_90_pre2/RelValTTbar_13_GEN-SIM-DIGI-RAW_90X_upgrade2017_realistic_v0-v1/C4D4696E-5CC2-E611-B705-0025905A6064.root',
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/Samples_90_pre2/RelValTTbar_13_GEN-SIM-DIGI-RAW_90X_upgrade2017_realistic_v0-v1/E0DA9B60-48C2-E611-AD54-0CC47A7C357E.root',
#'root://eoscms//eos/cms/store/group/dpg_hcal/comm_hcal/RecoAlgos/Samples_90_pre2/RelValTTbar_13_GEN-SIM-DIGI-RAW_90X_upgrade2017_realistic_v0-v1/FE69649A-48C2-E611-8DD2-0025905B85D0.root',


## these are w/o PU~35
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/008850AE-37C2-E611-A577-0025905A607A.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/06437686-37C2-E611-821B-0CC47A7C3408.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/0A7B6E48-37C2-E611-BF26-0CC47A4D7600.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/166972AE-37C2-E611-B2F4-0025905A60D0.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/20687634-5EC2-E611-AA8C-0025905B8604.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/2844817B-37C2-E611-8862-0025905B85A2.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/3230D35F-38C2-E611-9F10-0CC47A7C3430.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/36D7629E-37C2-E611-AEE6-0025905B85EC.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/38C7B138-39C2-E611-A119-003048FFCBB8.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/3A69C3A9-5DC2-E611-8939-0025905B855E.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/421C2E50-64C2-E611-AAEF-0CC47A4D76D2.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/44E966A2-37C2-E611-A9AE-0CC47A74525A.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/4A535A4A-5EC2-E611-B168-0025905A60D6.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/4EB7A34C-37C2-E611-A6DD-0025905A48BC.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/5482023E-39C2-E611-9F5A-0025905B859E.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/54D24048-37C2-E611-B33C-0025905A48C0.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/560D5E48-37C2-E611-AD0E-0CC47A4D7600.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/64847FB3-39C2-E611-804C-0025905A6088.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/661CE261-39C2-E611-A9FD-0CC47A4C8E34.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/66ED6357-37C2-E611-8E97-0025905A607A.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/683F095E-37C2-E611-88BA-0025905A48E4.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/6C70C53C-67C2-E611-8E57-0CC47A4D767E.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/70FD08B4-39C2-E611-946A-0025905B8604.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/72ACC93D-39C2-E611-B52E-0CC47A4D76B6.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/785D6C6B-57C2-E611-A6F7-0025905B857C.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/8463F29D-37C2-E611-A561-0CC47A78A436.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/924B47B0-39C2-E611-AC35-0025905A48BC.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/968B12C4-39C2-E611-8ABA-0CC47A4D761A.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/9EF25C65-39C2-E611-B4E9-0025905A60A0.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/AC7F8F53-37C2-E611-BE5B-0CC47A78A33E.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/B035F26D-37C2-E611-A575-0025905A48BC.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/B2B39EED-44C2-E611-95A2-0CC47A7C3628.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/B4E10253-37C2-E611-A585-0025905B860E.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/B88ACF63-39C2-E611-B23D-0CC47A4D7640.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/C0711798-37C2-E611-BB73-0025905A60A0.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/C207821B-39C2-E611-89EA-0CC47A4D7600.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/C6D5E582-37C2-E611-AA6D-0CC47A4D7640.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/C8BB0A59-63C2-E611-B3D4-0025905B85B8.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/D242B477-37C2-E611-A51F-0CC47A7C3628.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/D8A2055C-37C2-E611-9033-0025905A608E.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/D8B5F925-39C2-E611-9AC1-0025905A60D0.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/DA1924F4-5CC2-E611-BE32-0025905A6068.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/DC6C9CB3-39C2-E611-B3CE-0025905A607A.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/E4C3F4AD-37C2-E611-86B0-0CC47A4C8E34.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/E63ADC6B-37C2-E611-BA3E-0025905B8606.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/EADAC26B-39C2-E611-8076-0CC47A7C3628.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/EC8FD76D-39C2-E611-9E5D-0025905B8574.root',
#'root://eoscms//eos/cms/store/relval/CMSSW_9_0_0_pre2/RelValTTbar_13/GEN-SIM-DIGI-RAW/PU25ns_90X_upgrade2017_realistic_v0-v1/10000/F06CEF30-5FC2-E611-A31F-0CC47A4C8EC8.root'
),

                            secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-RECO'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('file:step3_timing.root'),
    outputCommands = process.RECOSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition
process.RECOSIMoutput.outputCommands.append( "keep *_g4SimHits*_*_*")
process.RECOSIMoutput.outputCommands.append( "keep *_hbheprereco*_*_*")
process.RECOSIMoutput.outputCommands.append( "keep HBHERecHits*_*_*_*")

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2017_realistic', '')
##process.GlobalTag = GlobalTag(process.GlobalTag, '81X_upgrade2017_realistic_v25', '')

# Save HBHEChannelInfo
process.hbheprereco.saveInfos = cms.bool(True)

process.hbheprerecoM2 = process.hbheprereco.clone()
process.hbheprerecoM2.algorithm.__setattr__('useM2',cms.bool(True))
process.hbheprerecoM2.algorithm.__setattr__('useM3',cms.bool(False))
process.hbheprerecoM2.algorithm.__setattr__('useMahi',cms.bool(False))
process.hbheprerecoM2.algorithm.__setattr__('pulseShapeType',cms.int32(1))

process.hbheprerecoM3 = process.hbheprereco.clone()
process.hbheprerecoM3.algorithm.__setattr__('useM2',cms.bool(False))
process.hbheprerecoM3.algorithm.__setattr__('useM3',cms.bool(True))
process.hbheprerecoM3.algorithm.__setattr__('useMahi',cms.bool(False))
process.hbheprerecoM3.algorithm.__setattr__('pulseShapeType',cms.int32(1))

process.hbheprerecoM2new = process.hbheprereco.clone()
process.hbheprerecoM2new.algorithm.__setattr__('useM2',cms.bool(True))
process.hbheprerecoM2new.algorithm.__setattr__('useM3',cms.bool(False))
process.hbheprerecoM2new.algorithm.__setattr__('useMahi',cms.bool(False))
process.hbheprerecoM2new.algorithm.__setattr__('pulseShapeType',cms.int32(2))

process.hbheprerecoM3new = process.hbheprereco.clone()
process.hbheprerecoM3new.algorithm.__setattr__('useM2',cms.bool(False))
process.hbheprerecoM3new.algorithm.__setattr__('useM3',cms.bool(True))
process.hbheprerecoM3new.algorithm.__setattr__('useMahi',cms.bool(False))
process.hbheprerecoM3new.algorithm.__setattr__('pulseShapeType',cms.int32(2))

process.hbherecoMAHI = process.hbheprereco.clone()
process.hbherecoMAHI.algorithm.__setattr__('useM2',cms.bool(False))
process.hbherecoMAHI.algorithm.__setattr__('useM3',cms.bool(False))
process.hbherecoMAHI.algorithm.__setattr__('useMahi',cms.bool(True))
#process.hbherecoMAHI.algorithm.__setattr__('pulseShapeType',cms.int32(3))

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstruction*process.hbheprerecoM2*process.hbheprerecoM3*process.hbheprerecoM2new*process.hbheprerecoM3new*process.hbherecoMAHI)
process.eventinterpretaion_step = cms.Path(process.EIsequence)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.eventinterpretaion_step,process.endjob_step,process.RECOSIMoutput_step)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(16)
#process.options.numberOfThreads=cms.untracked.uint32(1)
process.options.numberOfStreams=cms.untracked.uint32(0)

#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)


# Automatic addition of the customisation function from Validation.Performance.TimeMemoryInfo
from Validation.Performance.TimeMemoryInfo import customise

#call to customisation function customise imported from Validation.Performance.TimeMemoryInfo
process = customise(process)

##from SLHCUpgradeSimulations.Configuration.HCalCustoms import load_HcalHardcode
##process = load_HcalHardcode(process)
##process.es_hardcode.useHEUpgrade = cms.bool(True)
##process.es_hardcode.useHFUpgrade = cms.bool(True)
##process.es_hardcode.heUpgrade.darkCurrent = cms.double(0)
##process.es_hardcode.SiPMCharacteristics[2].crosstalk = cms.double(0.0)
##process.es_hardcode.SiPMCharacteristics[3].crosstalk = cms.double(0.0)
##process.es_hardcode.toGet = cms.untracked.vstring('GainWidths','SiPMParameters','SiPMCharacteristics')

# Customisation from command line

dumpFile  = open("DumpRECO_Phase1_step3_mahi_GT.py", "w")
dumpFile.write(process.dumpPython())
dumpFile.close()
