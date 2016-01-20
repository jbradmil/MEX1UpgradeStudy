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


options.inputFiles = 'root://cmsxrootd.fnal.gov//store/group/upgrade/muon/ME0GlobalReco/ME0MuonReRun_DY_SLHC23patch1_SegmentReRunFullRun_ForPublish/M-20_TuneZ2star_14TeV_6_2_0_SLHC23patch1_2023/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO/b52ce42d5986c94dc336f39e015d825e/output_112_3_4s6.root'
#options.inputFiles = "file:GEM2019Upg14.root"
options.outputFile = "test_rereco.root"
#options.register ('failureRate', 0.15, VarParsing.multiplicity.singleton, VarParsing.varType.float, "choose failure probability")
options.register ('failRateME31', 0., VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME3/1 failure probability")
options.register ('failRateME41', 0., VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME4/1 failure probability")
options.register ('doUniformFailure', True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "run failure in all chambers evenly")
options.register ('doCFEBFailure', True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "explicitly set CFEB failure")
#options.register ('maxEvents', -1, VarParsing.multiplicity.singleton, VarParsing.varType.int, "max number of events to process") -- this is already registered by default
options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(options.inputFiles)
)

## process.source = cms.Source("PoolSource",
##     secondaryFileNames = cms.untracked.vstring(),
##     #fileNames = cms.untracked.vstring('root://cmseos.fnal.gov//store/user/jbradmil/CSC/GluGluToHToZZTo4m_M-125_14TeV-powheg-pythia6/GluGluToHToZZTo4m_M-125_14TeV-powheg-pythia6-GEN-SIM-RECO-test.root')
##     fileNames = cms.untracked.vstring('root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_100_2_p9i.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_101_1_PUU.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_102_2_UU9.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_103_1_wht.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_104_1_YDt.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_105_2_XvG.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_106_1_Mzi.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_107_1_kWi.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_108_2_LF6.root',
##                                       'root://cmseos.fnal.gov//store/user/jbradmil/CSC/DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_dnash-DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola_2023SHCalNoTaper_PU140_Selectors_RECO-b52ce42d5986c94dc336f39e015d825e_USER/output_109_1_Td4.root')
## )

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
    failRateME31 = cms.untracked.double(options.failRateME31),
    failRateME41 = cms.untracked.double(options.failRateME41),
    doUniformFailure = cms.untracked.bool(options.doUniformFailure),
    doCFEBFailure = cms.untracked.bool(options.doCFEBFailure),
)

#process.cscSegments.inputObjects = "csc2DRecHitsOverload"

# change input to cscSegments
process.cscSegments.inputObjects = "csc2DRecHitsOverload"
## process.cscSegments = cms.EDProducer("CSCSegmentProducer",
##     inputObjects = cms.InputTag("csc2DRecHitsOverload"),
##     algo_psets = cms.VPSet(cms.PSet(
##         chamber_types = cms.vstring('ME1/a', 
##             'ME1/b', 
##             'ME1/2', 
##             'ME1/3', 
##             'ME2/1', 
##             'ME2/2', 
##             'ME3/1', 
##             'ME3/2', 
##             'ME4/1'),
##         algo_name = cms.string('CSCSegAlgoSK'),
##         parameters_per_chamber_type = cms.vint32(2, 1, 1, 1, 1, 
##             1, 1, 1, 1),
##         algo_psets = cms.VPSet(cms.PSet(
##             dPhiFineMax = cms.double(0.025),
##             verboseInfo = cms.untracked.bool(True),
##             chi2Max = cms.double(99999.0),
##             dPhiMax = cms.double(0.003),
##             wideSeg = cms.double(3.0),
##             minLayersApart = cms.int32(2),
##             dRPhiFineMax = cms.double(8.0),
##             dRPhiMax = cms.double(8.0)
##         ), 
##             cms.PSet(
##                 dPhiFineMax = cms.double(0.025),
##                 verboseInfo = cms.untracked.bool(True),
##                 chi2Max = cms.double(99999.0),
##                 dPhiMax = cms.double(0.025),
##                 wideSeg = cms.double(3.0),
##                 minLayersApart = cms.int32(2),
##                 dRPhiFineMax = cms.double(3.0),
##                 dRPhiMax = cms.double(8.0)
##             ))
##     ), 
##         cms.PSet(
##             chamber_types = cms.vstring('ME1/a', 
##                 'ME1/b', 
##                 'ME1/2', 
##                 'ME1/3', 
##                 'ME2/1', 
##                 'ME2/2', 
##                 'ME3/1', 
##                 'ME3/2', 
##                 'ME4/1'),
##             algo_name = cms.string('CSCSegAlgoTC'),
##             parameters_per_chamber_type = cms.vint32(2, 1, 1, 1, 1, 
##                 1, 1, 1, 1),
##             algo_psets = cms.VPSet(cms.PSet(
##                 dPhiFineMax = cms.double(0.02),
##                 verboseInfo = cms.untracked.bool(True),
##                 SegmentSorting = cms.int32(1),
##                 chi2Max = cms.double(6000.0),
##                 dPhiMax = cms.double(0.003),
##                 chi2ndfProbMin = cms.double(0.0001),
##                 minLayersApart = cms.int32(2),
##                 dRPhiFineMax = cms.double(6.0),
##                 dRPhiMax = cms.double(1.2)
##             ), 
##                 cms.PSet(
##                     dPhiFineMax = cms.double(0.013),
##                     verboseInfo = cms.untracked.bool(True),
##                     SegmentSorting = cms.int32(1),
##                     chi2Max = cms.double(6000.0),
##                     dPhiMax = cms.double(0.00198),
##                     chi2ndfProbMin = cms.double(0.0001),
##                     minLayersApart = cms.int32(2),
##                     dRPhiFineMax = cms.double(3.0),
##                     dRPhiMax = cms.double(0.6)
##                 ))
##         ), 
##         cms.PSet(
##             chamber_types = cms.vstring('ME1/a', 
##                 'ME1/b', 
##                 'ME1/2', 
##                 'ME1/3', 
##                 'ME2/1', 
##                 'ME2/2', 
##                 'ME3/1', 
##                 'ME3/2', 
##                 'ME4/1'),
##             algo_name = cms.string('CSCSegAlgoDF'),
##             parameters_per_chamber_type = cms.vint32(3, 1, 2, 2, 1, 
##                 2, 1, 2, 1),
##             algo_psets = cms.VPSet(cms.PSet(
##                 preClustering = cms.untracked.bool(False),
##                 maxRatioResidualPrune = cms.double(3.0),
##                 dPhiFineMax = cms.double(0.025),
##                 chi2Max = cms.double(5000.0),
##                 dXclusBoxMax = cms.double(8.0),
##                 tanThetaMax = cms.double(1.2),
##                 tanPhiMax = cms.double(0.5),
##                 minHitsPerSegment = cms.int32(3),
##                 minHitsForPreClustering = cms.int32(10),
##                 minLayersApart = cms.int32(2),
##                 dRPhiFineMax = cms.double(8.0),
##                 nHitsPerClusterIsShower = cms.int32(20),
##                 CSCSegmentDebug = cms.untracked.bool(False),
##                 Pruning = cms.untracked.bool(False),
##                 dYclusBoxMax = cms.double(8.0)
##             ), 
##                 cms.PSet(
##                     preClustering = cms.untracked.bool(False),
##                     maxRatioResidualPrune = cms.double(3.0),
##                     dPhiFineMax = cms.double(0.025),
##                     chi2Max = cms.double(5000.0),
##                     dXclusBoxMax = cms.double(8.0),
##                     tanThetaMax = cms.double(2.0),
##                     tanPhiMax = cms.double(0.8),
##                     minHitsPerSegment = cms.int32(3),
##                     minHitsForPreClustering = cms.int32(10),
##                     minLayersApart = cms.int32(2),
##                     dRPhiFineMax = cms.double(12.0),
##                     nHitsPerClusterIsShower = cms.int32(20),
##                     CSCSegmentDebug = cms.untracked.bool(False),
##                     Pruning = cms.untracked.bool(False),
##                     dYclusBoxMax = cms.double(12.0)
##                 ), 
##                 cms.PSet(
##                     preClustering = cms.untracked.bool(False),
##                     maxRatioResidualPrune = cms.double(3.0),
##                     dPhiFineMax = cms.double(0.025),
##                     chi2Max = cms.double(5000.0),
##                     dXclusBoxMax = cms.double(8.0),
##                     tanThetaMax = cms.double(1.2),
##                     tanPhiMax = cms.double(0.5),
##                     minHitsPerSegment = cms.int32(3),
##                     minHitsForPreClustering = cms.int32(30),
##                     minLayersApart = cms.int32(2),
##                     dRPhiFineMax = cms.double(8.0),
##                     nHitsPerClusterIsShower = cms.int32(20),
##                     CSCSegmentDebug = cms.untracked.bool(False),
##                     Pruning = cms.untracked.bool(False),
##                     dYclusBoxMax = cms.double(8.0)
##                 ))
##         ), 
##         cms.PSet(
##             chamber_types = cms.vstring('ME1/a', 
##                 'ME1/b', 
##                 'ME1/2', 
##                 'ME1/3', 
##                 'ME2/1', 
##                 'ME2/2', 
##                 'ME3/1', 
##                 'ME3/2', 
##                 'ME4/1', 
##                 'ME4/2'),
##             algo_name = cms.string('CSCSegAlgoST'),
##             parameters_per_chamber_type = cms.vint32(2, 1, 1, 1, 1, 
##                 1, 1, 1, 1, 1),
##             algo_psets = cms.VPSet(cms.PSet(
##                 maxRatioResidualPrune = cms.double(3),
##                 yweightPenalty = cms.double(1.5),
##                 maxRecHitsInCluster = cms.int32(20),
##                 dPhiFineMax = cms.double(0.025),
##                 preClusteringUseChaining = cms.bool(True),
##                 ForceCovariance = cms.bool(False),
##                 hitDropLimit6Hits = cms.double(0.3333),
##                 NormChi2Cut2D = cms.double(20.0),
##                 BPMinImprovement = cms.double(10000.0),
##                 Covariance = cms.double(0.0),
##                 tanPhiMax = cms.double(0.5),
##                 SeedBig = cms.double(0.0015),
##                 onlyBestSegment = cms.bool(False),
##                 dRPhiFineMax = cms.double(8.0),
##                 SeedSmall = cms.double(0.0002),
##                 curvePenalty = cms.double(2.0),
##                 dXclusBoxMax = cms.double(4.0),
##                 BrutePruning = cms.bool(True),
##                 curvePenaltyThreshold = cms.double(0.85),
##                 CorrectTheErrors = cms.bool(True),
##                 hitDropLimit4Hits = cms.double(0.6),
##                 useShowering = cms.bool(False),
##                 CSCDebug = cms.untracked.bool(False),
##                 tanThetaMax = cms.double(1.2),
##                 NormChi2Cut3D = cms.double(10.0),
##                 minHitsPerSegment = cms.int32(3),
##                 ForceCovarianceAll = cms.bool(False),
##                 yweightPenaltyThreshold = cms.double(1.0),
##                 prePrunLimit = cms.double(3.17),
##                 hitDropLimit5Hits = cms.double(0.8),
##                 preClustering = cms.bool(True),
##                 prePrun = cms.bool(True),
##                 maxDPhi = cms.double(999.0),
##                 maxDTheta = cms.double(999.0),
##                 Pruning = cms.bool(True),
##                 dYclusBoxMax = cms.double(8.0)
##             ), 
##                 cms.PSet(
##                     maxRatioResidualPrune = cms.double(3),
##                     yweightPenalty = cms.double(1.5),
##                     maxRecHitsInCluster = cms.int32(24),
##                     dPhiFineMax = cms.double(0.025),
##                     preClusteringUseChaining = cms.bool(True),
##                     ForceCovariance = cms.bool(False),
##                     hitDropLimit6Hits = cms.double(0.3333),
##                     NormChi2Cut2D = cms.double(20.0),
##                     BPMinImprovement = cms.double(10000.0),
##                     Covariance = cms.double(0.0),
##                     tanPhiMax = cms.double(0.5),
##                     SeedBig = cms.double(0.0015),
##                     onlyBestSegment = cms.bool(False),
##                     dRPhiFineMax = cms.double(8.0),
##                     SeedSmall = cms.double(0.0002),
##                     curvePenalty = cms.double(2.0),
##                     dXclusBoxMax = cms.double(4.0),
##                     BrutePruning = cms.bool(True),
##                     curvePenaltyThreshold = cms.double(0.85),
##                     CorrectTheErrors = cms.bool(True),
##                     hitDropLimit4Hits = cms.double(0.6),
##                     useShowering = cms.bool(False),
##                     CSCDebug = cms.untracked.bool(False),
##                     tanThetaMax = cms.double(1.2),
##                     NormChi2Cut3D = cms.double(10.0),
##                     minHitsPerSegment = cms.int32(3),
##                     ForceCovarianceAll = cms.bool(False),
##                     yweightPenaltyThreshold = cms.double(1.0),
##                     prePrunLimit = cms.double(3.17),
##                     hitDropLimit5Hits = cms.double(0.8),
##                     preClustering = cms.bool(True),
##                     prePrun = cms.bool(True),
##                     maxDPhi = cms.double(999.0),
##                     maxDTheta = cms.double(999.0),
##                     Pruning = cms.bool(True),
##                     dYclusBoxMax = cms.double(8.0)
##                 ))
##         )),
##     algo_type = cms.int32(4)
## )

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
process.endjob_step = cms.EndPath(process.endOfProcess)

process.RECOSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM-RECO')
    ),
    outputCommands = cms.untracked.vstring( ('drop *', 
        'drop *',
        'keep *_standAloneMuons_*_RERECO',
        #'keep *_globalMuons_*_RERECO',
        #'keep *_muons_*_RERECO',
        'keep *_csc2DRecHitsOverload_*_RERECO',
        'keep *_csc2DRecHits_*_RERECO',
        'keep *_cscSegments_*_RERECO',
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
