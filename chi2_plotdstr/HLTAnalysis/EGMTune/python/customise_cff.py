import FWCore.ParameterSet.Config as cms

def customise_plotDistr(process):
	#process.load("Configuration.StandardSequences.Services_cff")

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
	return process

