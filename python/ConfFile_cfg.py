import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/uscms_data/d2/ntran/physics/BigData/tmpdat/984DF8BC-0B0D-E611-B1F8-0CC47A78A3D8.root'
    )
)

process.demo = cms.EDAnalyzer('AvroProducer',
	JetTag  = cms.InputTag('slimmedJets')
)


process.p = cms.Path(process.demo)
