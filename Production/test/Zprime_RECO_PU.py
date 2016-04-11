# Auto generated configuration file
# using: 
# Revision: 1.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Zprime -s RECO --eventcontent=RECOSIM --pileup=AVE_140_BX_25ns --conditions=PH2_1K_FB_V6::All --filein=file:Zprime_RECO.root --fileout=test_rereco.root --customise=SLHCUpgradeSimulations/Configuration/combinedCustoms.cust_2023SHCal --magField=38T_PostLS1 --geometry=Extended2023SHCalNoTaper,Extended2023SHCalNoTaperReco --datatier=GEN-SIM-RECO
import FWCore.ParameterSet.Config as cms

process = cms.Process('RERECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.Geometry.GeometryExtended2023SHCalNoTaperReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('RecoMuon.MuonIdentification.me0MuonReco_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(10)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:Zprime_RECO.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.20 $'),
    annotation = cms.untracked.string('Zprime nevts:1'),
    name = cms.untracked.string('Applications')
)

# Output definition

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *',
        #'keep *_standAloneMuons__RERECO',
        #'keep *_globalMuons_*_RERECO',
        #'keep *_muons_*_RERECO',
        'keep *_*_*_RERECO',
        #'keep *_csc2DRecHitsOverload_*_RERECO',
        #'keep *_csc2DRecHits_*_RERECO',
        #'keep *_cscSegments_*_RERECO',
        'keep *_genParticles_*_*') ),
    fileName = cms.untracked.string('test_rereco.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    )
)

# Additional output definition

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(140.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'PH2_1K_FB_V6::All', '')

# Path and EndPath definitions
#process.reconstruction_step = cms.Path(process.reconstruction_fromRECO)
process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    csc2DRecHitsOverload = cms.PSet(
        initialSeed = cms.untracked.uint32(81)
    )
)

process.csc2DRecHitsOverload = cms.EDProducer('CFEBBufferOverloadProducer',
    cscRecHitTag = cms.InputTag("csc2DRecHits"),
    #failureRate = cms.untracked.double(options.failureRate),
    failRateME21 = cms.untracked.double(0.5),
    failRateME31 = cms.untracked.double(0.5),
    failRateME41 = cms.untracked.double(0.5)
)
# change input to cscSegments
process.cscSegments.inputObjects = "csc2DRecHitsOverload"

process.csclocalreco = cms.Sequence(process.csc2DRecHitsOverload*process.cscSegments)

#from RecoMuon.MuonIdentification.me0SegmentMatcher_cfi import *
#from RecoMuon.MuonIdentification.me0MuonConverter_cfi import *

process.particleFlowClusterEBEKMerger = cms.EDProducer('PFClusterCollectionMerger',
                                                       inputs = cms.VInputTag(cms.InputTag('particleFlowClusterECALUncorrected'),
                                                                              cms.InputTag('particleFlowClusterEKUncorrected')
                                                                              )
                                                                              )   

process.mustachePhotonCore = process.photonCore.clone(
    scHybridBarrelProducer = cms.InputTag("particleFlowSuperClusterECAL","particleFlowSuperClusterECALBarrel"),
    scIslandEndcapProducer = cms.InputTag("particleFlowSuperClusterECAL","particleFlowSuperClusterECALEndcapWithPreshower")
    )
process.mustachePhotons = process.photons.clone(
    photonCoreProducer = cms.InputTag('mustachePhotonCore'),
    endcapEcalHits = cms.InputTag("ecalRecHit","EcalRecHitsEK"))

process.muons.PFCandidates = cms.InputTag("particleFlow")
process.muons.FillPFIsolation = False
process.muons.FillSelectorMaps = False 
process.muons.FillCosmicsIdMap =  False
process.muons.FillTimingInfo = False
process.muons.FillDetectorBasedIsolation = False 
process.muons.FillShoweringInfo = False

#from RecoTracker.FinalTrackSelectors.preDuplicateMergingGeneralTracks_cfi import *

#process.reconstruction_step = cms.Path(process.csc2DRecHitsOverload*process.reconstruction_fromRECO)
process.reconstruction_step = cms.Path(process.siPixelRecHits*process.siStripMatchedRecHits*process.clusterSummaryProducer*process.dt4DSegments*process.dt4DCosmicSegments*process.csclocalreco*process.HGCalUncalibRecHit*process.HGCalRecHit*process.offlineBeamSpot*process.ancientMuonSeed*process.standAloneMuons*process.refittedStandAloneMuons*process.pixelTracks*process.pixelVertices*process.particleFlowRecHitPS*process.particleFlowClusterPS*process.particleFlowRecHitECAL*process.particleFlowClusterECALUncorrected*process.particleFlowRecHitEK*process.particleFlowClusterEKUncorrected*process.particleFlowClusterEBEKMerger*process.particleFlowClusterECAL*process.particleFlowClusterECAL*process.offlinePrimaryVertices*process.particleFlowRecHitHBHE*process.particleFlowRecHitHF*process.particleFlowClusterHBHE*process.particleFlowClusterHF*process.particleFlowClusterHCAL*process.particleFlowRecHitHO*process.particleFlowClusterHO*process.cleanedHybridSuperClusters*process.uncleanedHybridSuperClusters*process.hybridSuperClusters*process.correctedHybridSuperClusters*process.uncleanedOnlyCorrectedHybridSuperClusters*process.multi5x5BasicClustersCleaned*process.multi5x5SuperClustersCleaned*process.multi5x5BasicClustersUncleaned*process.multi5x5SuperClustersUncleaned*process.multi5x5SuperClusters*process.multi5x5SuperClustersWithPreshower*process.correctedMulti5x5SuperClustersWithPreshower*process.multi5x5PreshowerClusterShape*process.uncleanedOnlyMulti5x5SuperClustersWithPreshower*process.uncleanedOnlyCorrectedMulti5x5SuperClustersWithPreshower*process.particleFlowSuperClusterECAL*process.towerMaker*process.towerMakerWithHO*process.offlinePrimaryVertices*process.offlinePrimaryVerticesWithBS*process.generalV0Candidates*process.globalMuons*process.tevMuons*process.glbTrackQual*process.muons1stStep*process.muons*process.logErrorHarvester)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOSIMoutput_step = cms.EndPath(process.RECOSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.reconstruction_step,process.endjob_step,process.RECOSIMoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from SLHCUpgradeSimulations.Configuration.combinedCustoms
from SLHCUpgradeSimulations.Configuration.combinedCustoms import cust_2023SHCal 

#call to customisation function cust_2023SHCal imported from SLHCUpgradeSimulations.Configuration.combinedCustoms
process = cust_2023SHCal(process)

# End of customisation functions
