from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'tree_Zmm_2018B'
config.General.transferLogs = True
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'deadCrysAnalysis_cfg.py'
config.JobType.maxMemoryMB=2500
config.section_("Data")
#config.Data.inputDataset = '/EGamma/Run2018B-ZElectron-PromptReco-v1/RAW-RECO'
config.Data.inputDataset = '/DoubleMuon/Run2018B-v1/RAW' 
config.Data.splitting = 'Automatic'
config.Data.publication = True
config.Data.outputDatasetTag = 'tree_Zmm_2018B'
config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
config.section_("User")
config.section_("Debug")
