## Configuration file to CRAB3 cfA jobs
## Submit for src with: crab submit -c crabcfA.py

#dataset = '/VBF_HToMuMu_M-125_14TeV-powheg-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
#dataset = '/ZprimeSSMToMuMu_M-2500_TuneZ2star_14TeV-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
dataset = '/GluGluToHToZZTo4m_M-125_14TeV-powheg-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'

#jobname = dataset[1:].replace('/','__')
#jobname = jobname.replace(':','___')


from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'hzz_me21_0p0_me31_me41_0p0'
config.General.workArea = 'out_crab'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_rereco_cfg.py'
config.JobType.pyCfgParams = ['outputFile=file:rereco_fail.root', 'failRateME21=0.0', 'failRateME31=0.0', 'failRateME41=0.0']

config.section_("Data")
config.Data.inputDataset = dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.publishDBS = 'phys03'
config.Data.ignoreLocality = True
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-251252_13TeV_PromptReco_Collisions15_JSON.txt'

config.section_("Site")
config.Site.storageSite = 'T2_US_UCSD'
config.Site.whitelist = ['T2_US_Caltech','T2_US_Florida', 'T2_US_MIT', 'T2_US_Nebraska', 'T2_US_Purdue', 'T2_US_UCSD', 'T2_US_Vanderbilt', 'T2_US_Wisconsin', 'T1_US_FNAL','T2_US_MIT', 'T3_US_UCSB', 'T2_BE_IIHE', 'T2_PT_NCG_Lisbon', 'T2_BR_SPRACE']
#config.Site.whitelist = ['T2_US_UCSD']
config.Site.blacklist = ['T1_RU_JINR']
# you may want to uncomment this line and force jobs to run in the US
# only a few datasets (mostly very new ones) will not be accessible
