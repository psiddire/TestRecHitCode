# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions 101X_dataRun2_Prompt_v9 -n -1 --era Run2_2018 --eventcontent RAWRECO --data -s RAW2DIGI,RECO --datatier RAW-RECO --python testReRecoZSkimData_fromRawReco.py --filein /store/data/Run2018B/EGamma/RAW-RECO/ZElectron-PromptReco-v1/000/317/864/00000/8A838407-EC71-E811-8525-FA163E54B47A.root --fileout file:step3_2018.root --scenario pp --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RERECO',eras.Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
readFiles=cms.untracked.vstring()

lines=[]
for line in open('skimmedFiles.txt'):
    lines.append('file:%s' %(line.rstrip('\n')))
    
readFiles.extend(lines)

process.MessageLogger.cerr.FwkReport.reportEvery = 1
# Input source
process.source = cms.Source("PoolSource",
    fileNames = readFiles,
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step3 nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

from Configuration.EventContent.EventContent_cff import RAWRECOEventContent
process.skimContent = process.RAWRECOEventContent.clone()
process.load("DPGAnalysis.Skims.filterRecHitsRecovery_cfi")
process.recoveryfilter = cms.Path(process.recHitRecoveryFilter)
from RecoLocalCalo.EcalRecProducers.ecalRecHit_cfi import ecalRecHit
process.ecalRecHit.singleChannelRecoveryThreshold=0.7

# Output definition

process.RAWRECOoutput = cms.OutputModule("PoolOutputModule",
                             
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('RAW-RECO'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step3_skimmed.root'),
    outputCommands = process.RAWRECOEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('recoveryfilter'))
)
process.RAWRECOoutput.outputCommands.append('drop *_*_*_RECO')

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '101X_dataRun2_Prompt_v9', '')

# Path and EndPath definitions

process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstruction)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWRECOoutput_step = cms.EndPath(process.RAWRECOoutput)

##process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step, process.recoveryfilter, process.endjob_step, process.RAWRECOoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


### Customisation from command line
##
###Have logErrorHarvester wait for the same EDProducers to finish as those providing data for the OutputModule
##from FWCore.Modules.logErrorHarvester_cff import customiseLogErrorHarvesterUsingOutputCommands
##process = customiseLogErrorHarvesterUsingOutputCommands(process)
##
### Add early deletion of temporary data products to reduce peak memory need
##from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
##process = customiseEarlyDelete(process)
### End adding early deletion
