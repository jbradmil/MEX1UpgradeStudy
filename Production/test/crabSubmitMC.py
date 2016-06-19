## Configuration file to CRAB3 cfA jobs
## Submit for src with: crab submit -c crabSubmitMC.py

dataset = '/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIIFall15DR75-Asympt25nsFlat10to30RawReco_HCALDebug_75X_mcRun2_asymptotic_v8-v1/GEN-SIM-RECO'

# next, set the desired fail rates
failRateME21 = 0.0
failRateME31 = 1.0
failRateME41 = 1.0

# this will automatically format job title
jobname = dataset[1:dataset.find("/",1)] + '_fr21_%.2f_fr31_%.2f_fr41_%.2f_reco_to_rereco' % (failRateME21, failRateME31, failRateME41)
jobname = jobname.replace(".","p")

from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = jobname # 
config.General.workArea = 'out_crab'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_reco_on_reco_mc_cfg.py'

input_fr21 = 'failRateME21=%.2f' % (failRateME21)
input_fr31 = 'failRateME31=%.2f' % (failRateME31)
input_fr41 = 'failRateME41=%.2f' % (failRateME41)
input_isMC = 'isData=False'

config.JobType.pyCfgParams = ['outputFile=rereco_fail.root', input_fr21, input_fr31, input_fr41, input_isMC] # fail rates set above, now passed to config params

config.section_("Data")
config.Data.inputDataset = dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
config.Data.publishDBS = 'phys03'
#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt'
## Note: setting ignoreLocality to true allows the jobs to run at all of the whitelisted sites via xrootd. If you want all the jobs to run locally, set this to false and comment out the whitelist
#config.Data.ignoreLocality = True

config.section_("Site")
config.Site.storageSite = 'T2_US_UCSD'
#config.Site.whitelist = ['T2_US_UCSD']
#config.Site.whitelist = ['T2_US_UCSD', 'T2_US_Caltech','T2_US_Florida', 'T2_US_MIT', 'T2_US_Nebraska', 'T2_US_Purdue', 'T2_US_Vanderbilt', 'T2_US_Wisconsin', 'T1_US_FNAL','T2_US_MIT', 'T3_US_UCSB', 'T2_CH_CERN', 'T2_BR_SPRACE']
