from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'HLTPhys2-2016B'
config.General.workArea = 'DataRereco2016_HLTPhys_Run2016B_272760'

#optional
#config.General.transferOutputs
#config.General.transferLogs
#config.General.failureLimit = 

#Expert use
#config.General.instance
#config.General.activity

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'RECO_RAW2DIGI_RECO_DATA.py'
config.JobType.outputFiles = ['DataRereco.root']
config.JobType.allowUndistributedCMSSW = True
config.section_("Data")
#config.Data.inputDataset = '/JetHT/Run2015B-v1/RAW'
config.Data.inputDataset = '/HLTPhysics2/Run2016B-v1/RAW'
#config.Data.primaryDataset = ''
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 1 
#config.Data.ignoreLocality = False
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2.txt'
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/DCSOnly/json_DCSONLY.txt'
#config.Data.runRange = '268955,268958,268992,269025,269059,269062,269071,269073,269074,269224,269603'
config.Data.runRange = '272760'
config.Data.totalUnits = -1
config.Data.publication = False
#config.Data.publishDBS = '' default for the moment
config.Data.outLFNDirBase = '/store/user/sabrandt/HCAL/Run2Data2016/'

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'
#config.Site.blacklist = ['T1_US_FNAL']
#config.Site.whitelist = ['T2_DE_DESY','T2_ES_IFCA','T2_IT_Rome','T2_RU_JINR','T2_US_Florida']

#config.section_("User")
#config.section_("Debug")
