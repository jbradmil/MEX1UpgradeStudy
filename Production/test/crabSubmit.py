## Configuration file to CRAB3 cfA jobs
## Submit for src with: crab submit -c crabSubmit.py

# first choose a dataset to process
#dataset = '/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
#dataset = '/VBF_HToMuMu_M-125_14TeV-powheg-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
#dataset = '/ZprimeSSMToMuMu_M-2500_TuneZ2star_14TeV-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
dataset = '/GluGluToHToZZTo4m_M-125_14TeV-powheg-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'

# next, set the desired fail rates
failRateME21 = 1.0
failRateME31 = 1.0
failRateME41 = 1.0

# this will automatically format job title
jobname = dataset[1:dataset.find("/",1)] + '_fr21_%.2f_fr31_%.2f_fr41_%.2f' % (failRateME21, failRateME31, failRateME41)
jobname = jobname.replace(".","p")

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = jobname # 
config.General.workArea = 'out_crab'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_rereco_cfg.py'

input_fr21 = 'failRateME21=%.2f' % (failRateME21)
input_fr31 = 'failRateME31=%.2f' % (failRateME31)
input_fr41 = 'failRateME41=%.2f' % (failRateME41)

config.JobType.pyCfgParams = ['outputFile=file:rereco_fail.root', input_fr21, input_fr31, input_fr41] # fail rates set above, now passed to config params

config.section_("Data")
config.Data.inputDataset = dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.publishDBS = 'phys03'
## Note: setting ignoreLocality to true allows the jobs to run at all of the whitelisted sites via xrootd. If you want all the jobs to run locally, set this to false and comment out the whitelist
config.Data.ignoreLocality = True

config.section_("Site")
config.Site.storageSite = 'T2_US_UCSD'
config.Site.whitelist = ['T2_US_UCSD', 'T2_US_Caltech','T2_US_Florida', 'T2_US_MIT', 'T2_US_Nebraska', 'T2_US_Purdue', 'T2_US_Vanderbilt', 'T2_US_Wisconsin', 'T1_US_FNAL','T2_US_MIT', 'T3_US_UCSB', 'T2_BE_IIHE', 'T2_PT_NCG_Lisbon', 'T2_BR_SPRACE', 'T2_IN_TIFR']
