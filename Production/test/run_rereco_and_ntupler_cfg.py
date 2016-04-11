import FWCore.ParameterSet.Config as cms

process = cms.Process("RERECO")

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.Geometry.GeometryExtended2023SHCalNoTaperReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')


###
# Defaults
###
from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')


#options.inputFiles = "files.list"
#options.inputFiles = "file:GEM2019Upg14.root"
#options.outputFile = "file:rereco_fail_me21_me31_me41_0p0000.root"
options.outputFile = "rereco_fail_me21_me31_me41_0p0000.root"
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

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        "/store/mc/TP2023SHCALDR/ZprimeSSMToMuMu_M-2500_TuneZ2star_14TeV-pythia6/GEN-SIM-RECO/SHCALMar26_PU140BX25_PH2_1K_FB_V6-v1/10000/087C3395-70D9-E411-8076-00266CF9B274.root"
    #"file:Zprime_RECO.root"
)
)

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(140.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PH2_1K_FB_V6::All', '')

#process.UpdaterService = cms.Service("UpdaterService")

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


#select good primary vertex
process.load('CommonTools.ParticleFlow.goodOfflinePrimaryVertices_cfi')

#process.reconstruction_step = cms.Path(process.offlineBeamSpot*process.csclocalreco*process.standAloneMuonSeeds*process.standAloneMuons*process.muonGlobalReco*process.goodOfflinePrimaryVertices*process.logErrorHarvester)
process.reconstruction_step = cms.Path(process.siPixelRecHits*process.siStripMatchedRecHits*process.clusterSummaryProducer*process.dt4DSegments*process.dt4DCosmicSegments*process.csclocalreco*process.offlineBeamSpot*process.ancientMuonSeed*process.standAloneMuons*process.refittedStandAloneMuons*process.pixelTracks*process.pixelVertices*process.particleFlowRecHitPS*process.offlinePrimaryVertices*process.offlinePrimaryVerticesWithBS*process.goodOfflinePrimaryVertices*process.generalV0Candidates*process.globalMuons*process.tevMuons*process.logErrorHarvester)

process.MEtoEDMConverter = cms.EDProducer("MEtoEDMConverter",
    deleteAfterCopy = cms.untracked.bool(True),
    Verbosity = cms.untracked.int32(0),
    Frequency = cms.untracked.int32(50),
    Name = cms.untracked.string('MEtoEDMConverter'),
    MEPathToSave = cms.untracked.string('')
)

process.endOfProcess = cms.Sequence(process.MEtoEDMConverter)


process.endjob_step = cms.EndPath(process.endOfProcess)
#process.endjob_step = cms.EndPath(process.endOfProcess+process.cscValidation)


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string(options.outputFile),
                                   )

process.tree = cms.EDAnalyzer("NTupleMaker",
                              #muons = cms.InputTag("muons1stStep","","RERECO"),
                              samuons = cms.InputTag("standAloneMuons","","RERECO"),
                              gmuons = cms.InputTag("globalMuons","","RERECO"),
                              pvs = cms.InputTag("goodOfflinePrimaryVertices","","RERECO"),
                              genParticles = cms.InputTag("genParticles"),
                              )

process.output_step = cms.EndPath(process.tree)



process.options = cms.untracked.PSet(
    SkipEvent = cms.untracked.vstring('ProductNotFound')
)


process.schedule = cms.Schedule(*[ process.reconstruction_step, process.endjob_step, process.output_step ])

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2023SHCal 

#call to customisation function cust_2023SHCal imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2023SHCal(process)

# End of customisation functions
