import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *
from PhysicsTools.NanoAOD.nano_eras_cff import *

packedPFCandidatesUpdated = cms.EDProducer("IsMatchedToHSProducer",
    packedPFCandidates1 = cms.InputTag("packedPFCandidates"),
    packedGenParticles1 = cms.InputTag("packedGenParticles"),
)


chargedPFTable = cms.EDProducer("SimplePATCandidateFlatTableProducer",
    src = cms.InputTag("packedPFCandidates"),
    cut = cms.string("charge!=0 && pt>0.5"), 
    name = cms.string("ChargedPFCandidates"),
    doc  = cms.string("packedPFCandidates after basic selection"),
    singleton = cms.bool(False), # the number of entries is variable
    extension = cms.bool(False), # this is the main table for the tracks
    variables = cms.PSet(CandVars,
        lostInnerHits   = Var("lostInnerHits()", int, doc = "lostInnerHits"),
	trackHighPurity = Var("trackHighPurity()", bool, doc = "trackHighPurity"),
        fromPV   = Var("fromPV()", int, doc = "fromPV"),
        dz   = Var("dz()", float, doc = "dz", precision=14),
        dzError   = Var("?hasTrackDetails()?dzError():-1", float, doc = "dzError", precision=10),
        dxy   = Var("dxy()", float, doc = "dxy", precision=10),
        ),
    externalVariables = cms.PSet(
         isMatchedToGenHS = ExtVar("packedPFCandidatesUpdated:isMatchedToGen",bool,doc="if"),
    ),
)


chargedPFTask = cms.Task(packedPFCandidatesUpdated,chargedPFTable)
