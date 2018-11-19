from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'filteredZTree'
config.General.transferLogs = True
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = 'runZtree.py'
config.JobType.maxMemoryMB = 2500
config.section_("Data")
config.Data.inputDataset = '/DoubleMuon/psiddire-ZMMDeadCh2018B-488ef73e29aa6cceca30574316a2e120/USER'
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 10
config.Data.totalUnits = -1
config.Data.publication = True
config.Data.outputDatasetTag = 'filteredZTree'
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
config.section_("User")
config.section_("Debug")
