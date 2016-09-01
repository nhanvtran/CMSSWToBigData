import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/uscms_data/d2/ntran/physics/BigData/CMSSW8019_test1/src/4E40D2E2-9E3A-E611-8C5B-00259081FB18.root'
    )
)

process.demo = cms.EDAnalyzer('AvroProducer',
	JetTagCHSAK4 = cms.InputTag('slimmedJets'),
	JetTagPUPAK4 = cms.InputTag('slimmedJetsPuppi')
)


process.p = cms.Path(process.demo)
