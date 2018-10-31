#!/bin/env python
from os import popen

#numero iniziale
i=0

#inizio ciclo
while i<22:
    cfg="""
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
count=0
start=10*"""+str(i)+"""
end=10+10*"""+str(i)+"""

lines=[]
for line in open('skimmedFiles.txt'):
   if (count >= start and count < end):
       lines.append('file:%s' %(line.rstrip('\\n')))
   count+=1

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
    fileName = cms.untracked.string('file:step3_skimmed_"""+str(i)+""".root'),
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


"""
    cfg_file = open("skimmed-"+str(i)+"_cfg.py","w")
    cfg_file.write(cfg)
    cfg_file.close()

    sh="""#!/bin/tcsh -f
setenv W_DIR "/afs/cern.ch/work/t/taroni/private/DeadChFinal/src/TestingMyCode/TestRecHitRecovery/test"
setenv CFG "/afs/cern.ch/work/t/taroni/private/DeadChFinal/src/TestingMyCode/TestRecHitRecovery/test/skimmed-"""+str(i)+"""_cfg.py"
setenv InFile "/afs/cern.ch/work/t/taroni/private/DeadChFinal/src/TestingMyCode/TestRecHitRecovery/test/skimmedFiles.txt"
cd $W_DIR
eval `scramv1 runtime -csh`
cd -
cp $InFile . 
cmsRun $CFG
cp step3_skimmed_"""+str(i)+""".root /eos/cms/store/user/taroni/EGamma/SkimmmingEvents700/test/.

exit

"""

    #scrive script
    sh_file = open("jobskimmed-"+str(i)+".sh","w")
    sh_file.write(sh)
    sh_file.close()

    #sottomette script
    popen("chmod a+x jobskimmed-"+str(i)+".sh" )
    popen("bsub -q cmscaf1nh jobskimmed-"+str(i)+".sh" )
    
    i+=1
