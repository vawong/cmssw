import FWCore.ParameterSet.Config as cms

EgammaObjectsProducerPhotons = cms.EDAnalyzer("EgammaObjects",
    particleID = cms.int32(22),
    hist_min_EOverTruth = cms.double(0.5),
    hist_max_deltaEta = cms.double(0.05),
    RecoCollection = cms.InputTag("correctedPhotons"),
    hist_min_Eta = cms.double(-2.7),
    hist_bins_EtaOverTruth = cms.int32(250),
    hist_min_deltaEt = cms.double(-12.5),
    hist_max_deltaEt = cms.double(12.5),
    hist_min_Phi = cms.double(-3.14159),
    MCTruthCollection = cms.InputTag("source"),
    hist_bins_deltaEt = cms.int32(251),
    hist_bins_E = cms.int32(100),
    hist_bins_Phi = cms.int32(64),
    hist_bins_PhiOverTruth = cms.int32(250),
    hist_max_deltaPhi = cms.double(0.025),
    outputFile = cms.string('SDPV_Photons.root'),
    hist_max_EtaOverTruth = cms.double(1.5),
    hist_min_EtaOverTruth = cms.double(0.5),
    hist_max_E = cms.double(400.0),
    hist_max_recoMass = cms.double(240.0),
    hist_bins_recoMass = cms.int32(240),
    hist_min_EtOverTruth = cms.double(0.5),
    hist_max_PhiOverTruth = cms.double(1.05),
    hist_bins_deltaEta = cms.int32(100),
    hist_min_recoMass = cms.double(0.0),
    hist_min_deltaEta = cms.double(-0.05),
    hist_max_EtOverTruth = cms.double(1.5),
    EtCut = cms.int32(5),
    hist_max_Eta = cms.double(2.7),
    hist_bins_Et = cms.int32(120),
    hist_bins_deltaPhi = cms.int32(100),
    hist_min_Et = cms.double(0.0),
    hist_max_Phi = cms.double(3.14159),
    hist_min_PhiOverTruth = cms.double(0.95),
    hist_min_deltaPhi = cms.double(-0.025),
    hist_max_Et = cms.double(120.0),
    hist_bins_Eta = cms.int32(55),
    hist_bins_EOverTruth = cms.int32(250),
    hist_min_E = cms.double(0.0),
    hist_bins_EtOverTruth = cms.int32(250),
    hist_max_EOverTruth = cms.double(1.5)
)


