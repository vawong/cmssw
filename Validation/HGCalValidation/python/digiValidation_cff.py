import FWCore.ParameterSet.Config as cms

from Validation.HGCalValidation.hgcalDigiValidationEE_cfi import *

hgcalDigiValidationHEF = hgcalDigiValidationEE.clone(
    DetectorName = cms.string("HGCalHESiliconSensitive"),
    DigiSource   = cms.InputTag("mix","HGCDigisHEfront"))

hgcalDigiValidationHEB = hgcalDigiValidationEE.clone(
    DetectorName = cms.string("HCal"),
    DigiSource   = cms.InputTag("mix","HGCDigisHEback"))

from Configuration.ProcessModifiers.premix_stage2_cff import premix_stage2
premix_stage2.toModify(hgcalDigiValidationHEF, DigiSource = "mixData:HGCDigisHEfront")
premix_stage2.toModify(hgcalDigiValidationHEB, DigiSource = "mixData:HGCDigisHEback")

from Configuration.Eras.Modifier_phase2_hgcalV9_cff import phase2_hgcalV9
phase2_hgcalV9.toModify(hgcalDigiValidationHEB, DetectorName = cms.string("HGCalHEScintillatorSensitive"))
