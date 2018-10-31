from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'filteredZEEDeadCh2018BIntEvt_PierreGlobalTag'
config.General.transferLogs = True
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = 'testIntEvt.py'
config.JobType.maxMemoryMB = 2500
config.section_("Data")
config.Data.inputDataset = '/EGamma/Run2018B-ZElectron-PromptReco-v1/RAW-RECO'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 6
config.Data.totalUnits = -1
config.Data.publication = True
config.Data.outputDatasetTag = 'ZEEDeadCh2018BIntEvt_PierreGlobalTag'
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
config.section_("User")
config.section_("Debug")
