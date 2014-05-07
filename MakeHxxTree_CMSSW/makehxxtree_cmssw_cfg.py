import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:../../data/CMSSW/8TeV/GluGluToHToZZTo4L/outfile_1_1_QDW.root'
    )
)

process.demo = cms.EDAnalyzer("MakeHxxTree_CMSSW",

  electronSrc = cms.untracked.InputTag("cleanPatElectrons"),
  muonSrc     = cms.untracked.InputTag("cleanPatMuons"),
  metSrc      = cms.untracked.InputTag("patMETs")                                             
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('GluGluToHToZZTo4L.root')
                                   )


process.p = cms.Path(process.demo)
