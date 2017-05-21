import FWCore.ParameterSet.Config as cms

process = cms.Process("ANA")#, era="")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = '90X_upgrade2017_TSG_Hcal_V2'
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("file:outputFULL.root"),
    #secondaryFileNames = cms.untracked.vstring("/store/mc/PhaseIFall16DR/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/GEN-SIM-RAW/FlatPU28to62HcalNZSRAW_90X_upgrade2017_realistic_v6_C1-v1/130001/FE64C02E-A70E-E711-B368-FA163E1C071E.root"),
)

#process.options = cms.untracked.PSet(
#       SkipEvent = cms.untracked.vstring('ProductNotFound') //added
#)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("hist.root"),
)

process.egmTune = cms.EDAnalyzer("HLTEGMTuneAnalyzer",
    triggerResults = cms.InputTag("triggerResults::HLT2"),
#    hltPaths = cms.vstring("HLT_Ele17_WPNone_Gsf_v0"),
    hltPaths = cms.vstring("HLT_Ele27_WPTight_Gsf_v7"),

    hltEBHits = cms.InputTag("hltEcalRecHit:EcalRecHitsEB"),
    hltEEHits = cms.InputTag("hltEcalRecHit:EcalRecHitsEE"),
    processName = cms.string("HLT2"),  

    genParticles = cms.InputTag("genParticles"),
    genEleMinPt = cms.double(27),
    genEleMaxEta = cms.double(3.0),
)

process.p = cms.Path(process.egmTune)
