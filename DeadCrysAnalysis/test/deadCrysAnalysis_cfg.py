import FWCore.ParameterSet.Config as cms

process = cms.Process("PROdTPA")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag,'101X_dataRun2_Prompt_v9', '')

process.load('Configuration.Geometry.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 10
process.load("Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi")

process.load("Geometry.CMSCommonData.cmsIdealGeometryXML_cfi")

process.load("CalibCalorimetry.EcalTPGTools.ecalTPGScale_cff")

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        '/store/data/Run2018B/EGamma/RAW-RECO/ZElectron-PromptReco-v1/000/317/182/00000/08C8D1B6-7966-E811-B307-FA163EAF8008.root',
        )
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

process.deadCrysAnalyzer = cms.EDAnalyzer("DeadCrysAnalysis",
                                          AnalyzeElectrons = cms.bool(False),
                                          Debug = cms.bool(False),
                                          usePhotons = cms.bool(False),
                                          storeLogE = cms.bool(False),  
                                          e3x3Cut = cms.double(0.),
                                          rhECut  = cms.double(0.), 
                                          do5x5 = cms.bool(False),
                                          useHighestEtRH = cms.bool(False), 
                                          inputRecHitsEB = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                          eleCollection = cms.InputTag("gedGsfElectrons"),
                                          phoCollection = cms.InputTag("gedPhotons")
                                          )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('recHitTree.root')
                                   )

process.p = cms.Path(process.deadCrysAnalyzer)
