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
    fileNames = cms.untracked.vstring()
)

events = cms.untracked.VEventRange()
for line in open('checkingIntEvt700.txt'):
    events.append(line.rstrip("\n"))

process.source.eventsToProcess = cms.untracked.VEventRange(events)
        

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.out=cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("mergedRawReco.root")
)

process.ep = cms.EndPath(process.out)

