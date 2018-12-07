import FWCore.ParameterSet.Config as cms

process = cms.Process('Demo')

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

from Configuration.AlCa.GlobalTag import GlobalTag
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag = GlobalTag(process.GlobalTag, '101X_dataRun2_Prompt_v9', '')

# input
inputFilesData = cms.untracked.vstring(
'/store/user/psiddire/DoubleMuon/ZMMDeadCh2018B/181105_140835/0000/ZmmRecFilter_98.root'
)

inputFiles = inputFilesData
outputFile = "ZSkimTree.root"
#process.source = cms.Source ("PoolSource", fileNames = inputFiles )                             
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring())

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.zskimtree = cms.EDAnalyzer(
    'ZSkimTree',
    #muonCollection = cms.InputTag('generalTracks'),
    muonCollection = cms.InputTag('muons'),
    vertices     = cms.InputTag('offlinePrimaryVertices'),
    metCollection = cms.InputTag('pfMet'),
    )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string( outputFile )
                                   )

process.p = cms.Path(process.zskimtree)
