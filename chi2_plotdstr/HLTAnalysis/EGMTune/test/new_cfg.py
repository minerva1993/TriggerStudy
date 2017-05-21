import FWCore.ParameterSet.Config as cms

process = cms.Process("ExREG")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = '90X_upgrade2017_TSG_Hcal_V2'
#PRE_ST62_V8::All'
#START53_V10::All'

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )


process.source = cms.Source("PoolSource",
                           # fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/j/jipark/work/private/triggerstudy/CMSSW_9_0_2/src/HLTrigger/Configuration/test/outputA.root')
				#fileNames = cms.untracked.vstring('file:DQMIO.root')

	fileNames = cms.untracked.vstring('file:/afs/cern.ch/user/j/jipark/work/private/CMSSW_9_0_2/src/HLTAnalysis/EGMTune/test/outputFULL_TEST.root')
                            )

process.options = cms.untracked.PSet(
       SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.plotDistr = cms.EDAnalyzer("plotDistr",
				OutputFileName = cms.string("mc_TEST.root"),
                                  isData = cms.bool(False),
                                  #activateNewClustering = cms.bool(False),
                                  #activateOldClustering = cms.bool(True),
				  trgResults = cms.InputTag("TriggerResults", "", "TEST"),
                                  saveReco = cms.bool(False),
				 # trgSelection= cms.vstring("HLT_Ele17_WPNone_Gsf_v0",)
				trgSelection= cms.vstring("HLT_Ele27_WPTight_Gsf_v7")

)
process.plotVar = cms.EndPath(process.plotDistr)

#process.p = cms.Path(process.plotDistr)

