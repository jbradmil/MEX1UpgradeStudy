## Configuration file to CRAB3 cfA jobs
## Submit for src with: crab submit -c crabSubmit.py

# first choose a dataset to process
#dataset = '/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
#dataset = '/VBF_HToMuMu_M-125_14TeV-powheg-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
#dataset = '/ZprimeSSMToMuMu_M-2500_TuneZ2star_14TeV-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
#dataset = '/GluGluToHToZZTo4m_M-125_14TeV-powheg-pythia6/TP2023SHCALDR-SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'
#dataset = '/PYTHIA6_Tauola_TTbar_TuneZ2star_14TeV/TP2023SHCALDR-SHCALJan23_PU140BX25_PH2_1K_FB_V6-v1/GEN-SIM-RECO'

#dataset = '/SingleMuon/Run2015C-v1/RAW'
#dataset = '/SingleMuon/Run2015D-v1/RAW'

#dataset = '/SingleMuon/Run2015C_25ns-16Dec2015-v1/RECO'
dataset = '/SingleMuon/Run2015D-16Dec2015-v1/RECO'


# next, set the desired fail rates
failRateME21 = 0.5
failRateME31 = 0.0
failRateME41 = 0.0

# this will automatically format job title
#jobname = dataset[1:dataset.find("/",1)] + '_fr21_%.2f_fr31_%.2f_fr41_%.2f' % (failRateME21, failRateME31, failRateME41)
jobname = dataset + '_fr21_%.2f_fr31_%.2f_fr41_%.2f' % (failRateME21, failRateME31, failRateME41)
jobname = jobname.replace("/","-")
jobname = jobname.replace(".","p")

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = jobname # 
config.General.workArea = 'out_crab'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_reco_on_reco_cfg.py'

input_fr21 = 'failRateME21=%.2f' % (failRateME21)
input_fr31 = 'failRateME31=%.2f' % (failRateME31)
input_fr41 = 'failRateME41=%.2f' % (failRateME41)

config.JobType.pyCfgParams = ['outputFile=rereco.root', input_fr21, input_fr31, input_fr41] # fail rates set above, now passed to config params

config.section_("Data")
config.Data.inputDataset = dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50
config.Data.publication = False
config.Data.publishDBS = 'phys03'
config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt'
## Note: setting ignoreLocality to true allows the jobs to run at all of the whitelisted sites via xrootd. If you want all the jobs to run locally, set this to false and comment out the whitelist
config.Data.ignoreLocality = True

config.section_("Site")
config.Site.storageSite = 'T2_US_UCSD'
config.Site.whitelist = ['T2_US_UCSD', 'T1_US_FNAL', 'T2_US_Purdue']
#config.Site.whitelist = ['T2_US_UCSD', 'T2_US_Caltech','T2_US_Florida', 'T2_US_MIT', 'T2_US_Nebraska', 'T2_US_Purdue', 'T2_US_Vanderbilt', 'T2_US_Wisconsin', 'T1_US_FNAL','T2_US_MIT', 'T3_US_UCSB', 'T2_BE_IIHE', 'T2_PT_NCG_Lisbon', 'T2_BR_SPRACE', 'T2_IN_TIFR', 'T2_FR_IPHC', 'T2_IT_Legnaro']
