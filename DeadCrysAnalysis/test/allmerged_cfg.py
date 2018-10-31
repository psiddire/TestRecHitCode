import FWCore.ParameterSet.Config as cms
process = cms.Process("Validation")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)

# Geometry
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff" )
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '101X_dataRun2_Prompt_v9', '')


process.source = cms.Source("PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_0.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_1.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_2.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_3.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_4.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_5.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_6.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_7.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_8.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_9.root ',
'file:/eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/step3_skimmed_10.root',

)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.out=cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("allMergedStep3.root")
)

process.ep = cms.EndPath(process.out)

