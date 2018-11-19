from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'filteredZMMDeadCh2018B'
config.General.transferLogs = True
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = 'testZmm.py'
config.JobType.maxMemoryMB = 5000
config.section_("Data")
config.Data.inputDataset = '/DoubleMuon/Run2018B-v1/RAW'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 2
config.Data.totalUnits = -1
config.Data.publication = True
config.Data.outputDatasetTag = 'ZMMDeadCh2018B'
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
config.section_("User")
config.section_("Debug")
