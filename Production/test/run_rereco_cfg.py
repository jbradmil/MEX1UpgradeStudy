import FWCore.ParameterSet.Config as cms

process = cms.Process("RERECO")

process.load( "DQMServices/Core/DQMStore_cfg" )
#process.load( "MagneticField/Engine/data/uniformMagneticField.cfi")
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
#process.load('RecoTracker.CkfPattern.CkfTrackCandidates_cff')

###
# Defaults
###
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')


#options.inputFiles = "files.list"
#options.inputFiles = "file:GEM2019Upg14.root"
options.outputFile = "file:rereco_fail_me21_me31_me41_0p0000.root"
options.maxEvents = "-1"
#options.histFile = "validation_hists_0p0.root"
#options.register ('failureRate', 0.15, VarParsing.multiplicity.singleton, VarParsing.varType.float, "choose failure probability")
#options.register ('txt_input', "files.list", VarParsing.multiplicity.singleton, VarParsing.varType.string, "List of input files")
#options.register ('histFile', "validation_hists_me21_me32_me41_0p0.root", VarParsing.multiplicity.singleton, VarParsing.varType.string, "Output validation histograms file")
options.register ('failRateME21', 0.0, VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME2/1 failure probability")
options.register ('failRateME31', 0.0, VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME3/1 failure probability")
options.register ('failRateME41', 0.0, VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME4/1 failure probability")
options.register ('doUniformFailure', True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "run failure in all chambers evenly")
options.register ('doCFEBFailure', True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "explicitly set CFEB failure")
#options.register ('maxEvents', -1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "max number of events to process") -- this is already registered by default
options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

## import FWCore.Utilities.FileUtils as FileUtils
## mylist = FileUtils.loadListFromFile ("files1.list") 
## readFiles = cms.untracked.vstring( *mylist)

# Input source
#process.source = cms.Source("PoolSource",
#    secondaryFileNames = cms.untracked.vstring(),
#    fileNames = cms.untracked.vstring(options.inputFiles)
#)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("/store/mc/TP2023SHCALDR/ZprimeSSMToMuMu_M-2500_TuneZ2star_14TeV-pythia6/GEN-SIM-RECO/SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/10000/087C3395-70D9-E411-8076-00266CF9B274.root")
)

process.options = cms.untracked.PSet(

)

process.GlobalTag = cms.ESSource("PoolDBESSource",
    DBParameters = cms.PSet(
        authenticationPath = cms.untracked.string(''),
        connectionRetrialTimeOut = cms.untracked.int32(60),
        idleConnectionCleanupPeriod = cms.untracked.int32(10),
        messageLevel = cms.untracked.int32(0),
        enablePoolAutomaticCleanUp = cms.untracked.bool(False),
        enableConnectionSharing = cms.untracked.bool(True),
        enableReadOnlySessionOnUpdateConnection = cms.untracked.bool(False),
        connectionTimeOut = cms.untracked.int32(60),
        authenticationSystem = cms.untracked.int32(0),
        connectionRetrialPeriod = cms.untracked.int32(10)
    ),
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    toGet = cms.VPSet(),
    connect = cms.string('frontier://FrontierProd/CMS_COND_31X_GLOBALTAG'),
    globaltag = cms.string('PH2_1K_FB_V6::All')
)

process.UpdaterService = cms.Service("UpdaterService")


process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    csc2DRecHitsOverload = cms.PSet(
        initialSeed = cms.untracked.uint32(81)
    )
)

process.csc2DRecHitsOverload = cms.EDProducer('CFEBBufferOverloadProducer',
    cscRecHitTag = cms.InputTag("csc2DRecHits"),
    #failureRate = cms.untracked.double(options.failureRate),
    failRateME21 = cms.untracked.double(options.failRateME21),
    failRateME31 = cms.untracked.double(options.failRateME31),
    failRateME41 = cms.untracked.double(options.failRateME41),
    doUniformFailure = cms.untracked.bool(options.doUniformFailure),
    doCFEBFailure = cms.untracked.bool(options.doCFEBFailure),
)

# change input to cscSegments
process.cscSegments.inputObjects = "csc2DRecHitsOverload"
process.csclocalreco = cms.Sequence(process.csc2DRecHitsOverload*process.cscSegments)
#process.muonlocalreco = cms.Sequence(process.muonDTDigis+process.dtlocalreco+process.csclocalreco+process.rpcRecHits+process.gemRecHits)


process.logErrorHarvester = cms.EDProducer("LogErrorHarvester")
process.reconstruction_step = cms.Path(process.offlineBeamSpot+process.csclocalreco+process.standAloneMuonSeeds+process.standAloneMuons+process.logErrorHarvester)

process.MEtoEDMConverter = cms.EDProducer("MEtoEDMConverter",
    deleteAfterCopy = cms.untracked.bool(True),
    Verbosity = cms.untracked.int32(0),
    Frequency = cms.untracked.int32(50),
    Name = cms.untracked.string('MEtoEDMConverter'),
    MEPathToSave = cms.untracked.string('')
)
process.endOfProcess = cms.Sequence(process.MEtoEDMConverter)

## process.cscValidation = cms.EDAnalyzer("CSCValidation",
##     rootFileName = cms.untracked.string(options.histFile),
##     isSimulation = cms.untracked.bool(False),
##     writeTreeToFile = cms.untracked.bool(True),
##     useDigis = cms.untracked.bool(False),
##     detailedAnalysis = cms.untracked.bool(False),
##     useTriggerFilter = cms.untracked.bool(False),
##     useQualityFilter = cms.untracked.bool(False),
##     makeStandalonePlots = cms.untracked.bool(True),
##     makeTimeMonitorPlots = cms.untracked.bool(True),
##     alctDigiTag = cms.InputTag("muonCSCDigis","MuonCSCALCTDigi"),
##     clctDigiTag = cms.InputTag("muonCSCDigis","MuonCSCCLCTDigi"),
##     corrlctDigiTag = cms.InputTag("muonCSCDigis","MuonCSCCorrelatedLCTDigi"),
##     stripDigiTag = cms.InputTag("muonCSCDigis","MuonCSCStripDigi"),
##     wireDigiTag = cms.InputTag("muonCSCDigis","MuonCSCWireDigi"),
##     compDigiTag = cms.InputTag("muonCSCDigis","MuonCSCComparatorDigi"),
##     cscRecHitTag = cms.InputTag("csc2DRecHitsOverload"),
##     cscSegTag = cms.InputTag("cscSegments"),
##     saMuonTag = cms.InputTag("standAloneMuons"),
##     l1aTag = cms.InputTag("gtDigis"),
##     hltTag = cms.InputTag("TriggerResults::RECO"),
##     makeHLTPlots = cms.untracked.bool(True),
##     simHitTag = cms.InputTag("g4SimHits", "MuonCSCHits")
## )

process.endjob_step = cms.EndPath(process.endOfProcess)
#process.endjob_step = cms.EndPath(process.endOfProcess+process.cscValidation)

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    ),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *',
        'keep *_standAloneMuons__RERECO',
        'keep *_globalMuons_*_RERECO',
        'keep *_muons_*_RERECO',
        #'keep *_csc2DRecHitsOverload_*_RERECO',
        #'keep *_csc2DRecHits_*_RERECO',
        #'keep *_cscSegments_*_RERECO',
        'keep *_genParticles_*_*') ),
        
    ## fileName = cms.untracked.string('file:DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_RERECO.root')
    fileName = cms.untracked.string(options.outputFile)
)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)


process.schedule = cms.Schedule(*[ process.reconstruction_step, process.endjob_step, process.RECOSIMoutput_step ])
#process.schedule = cms.Schedule(*[ process.endjob_step, process.RECOSIMoutput_step ])
