import FWCore.ParameterSet.Config as cms
from PhysicsTools.NanoAOD.common_cff import *
from PhysicsTools.NanoAOD.nano_eras_cff import *

losttracksTable = cms.EDProducer("SimplePATCandidateFlatTableProducer",
    src = cms.InputTag("lostTracks"),
    cut = cms.string("charge!=0 && pt>0.5"), 
    name = cms.string("LostTracks"),
    doc  = cms.string("lost tracks"),
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
    #externalVariables = cms.PSet(
    #    mvaTTH = ExtVar(cms.InputTag("muonMVATTH"),float, doc="TTH MVA lepton ID score",precision=14),
    #    mvaLowPt = ExtVar(cms.InputTag("muonMVALowPt"),float, doc="Low pt muon ID score",precision=14),
    #    fsrPhotonIdx = ExtVar(cms.InputTag("muonFSRassociation:fsrIndex"),int, doc="Index of the associated FSR photon"),
    #),
)


losttracksTask = cms.Task(losttracksTable)
