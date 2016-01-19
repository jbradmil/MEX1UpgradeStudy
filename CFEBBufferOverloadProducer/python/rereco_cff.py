import FWCore.ParameterSet.Config as cms
import sys,os
def RERECO(
        process,
        outfile,
        reportfreq=10,
        dataset="",
           #globaltag="",
           numevents=1000,
           failureRate=0.15,
           doUniformFailure=True,
           doCFEBFailure=True
        ):
    
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

    process.load('Configuration.StandardSequences.Reconstruction_cff')
    process.load( "DQMServices/Core/DQMStore_cfg" )
    #process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')


    process.UpdaterService = cms.Service("UpdaterService")

    process.MessageLogger.cerr.FwkReport.reportEvery = reportfreq
    process.options = cms.untracked.PSet(
        allowUnscheduled = cms.untracked.bool(True),
        wantSummary = cms.untracked.bool(True)
        )

    # files to process
    process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(numevents)
        )
    process.source = cms.Source("PoolSource",
                                fileNames = cms.untracked.vstring(dataset)
                                )
    
    # output file
    process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string(outfile+".root")
                                       )

    ## process.mix.input.nbPileupEvents.averageNumber = cms.double(140.000000)
    ## process.mix.bunchspace = cms.int32(25)
    ## process.mix.minBunch = cms.int32(-12)
    ## process.mix.maxBunch = cms.int32(3)
    ## process.mix.input.fileNames = cms.untracked.vstring(['/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/0010AE1F-6676-E411-8F16-002618943860.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/0035CDEE-5C76-E411-8214-0023AEFDEEEC.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/004B2C7D-6876-E411-ABFA-002618943949.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/006DDC01-6276-E411-9E66-00259073E4E4.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/008F6D89-5976-E411-A05E-549F35AC7DEE.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/02133DBD-6176-E411-967A-002590A8882A.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/0253431B-4F76-E411-ABFE-0025904C66F4.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/02758CA9-5F76-E411-A1D8-0015172C07E1.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/02C7F040-7176-E411-B19E-0023AEFDEE68.root', '/store/mc/TP2023HGCALGS/MinBias_TuneZ2star_14TeV-pythia6/GEN-SIM/DES23_62_V1-v3/00000/02DE880D-5576-E411-AE26-002590200A00.root'])


    # sequence for baseline producers
    #process.Baseline = cms.Sequence()

    process.offlineBeamSpot = cms.EDProducer("BeamSpotProducer")

    # generate CFEB failure pattern
    process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
                                                       csc2DRecHitsOverload = cms.PSet(
                                                           initialSeed = cms.untracked.uint32(81)
                                                           )
                                                           )

    # CFEB failure module
    process.csc2DRecHitsOverload = cms.EDProducer('CFEBBufferOverloadProducer',
                                                  cscRecHitTag = cms.InputTag("csc2DRecHits"),
                                                  failureRate = cms.untracked.double(failureRate),
                                                  doUniformFailure = cms.untracked.bool(doUniformFailure),
                                                  doCFEBFailure = cms.untracked.bool(doCFEBFailure),
                                                  )
    process.cscSegments.inputObjects = "csc2DRecHitsOverload"
    # change input to cscSegments
    ## from RecoLocalMuon.CSCSegment.cscSegments_cfi import cscSegments
    ## process.cscSegments = cscSegments.clone(inputObjects = cms.InputTag("csc2DRecHitsOverload"))


    ## from RecoMuon.MuonSeedGenerator.ancientMuonSeed_cfi import ancientMuonSeed
    ## process.ancientMuonSeed = ancientMuonSeed.clone()
    ## from RecoMuon.StandAloneMuonProducer.standAloneMuons_cfi import standAloneMuons
    ## process.standAloneMuons = standAloneMuons.clone()
    #process.standAloneMuonSeeds = cms.Sequence(process.ancientMuonSeed)

    #process.cscSegments.inputObjects = "csc2DRecHitsOverload"
    ## process.Baseline += process.offlineBeamSpot
    ## process.Baseline += process.csc2DRecHitsOverload
    ## process.Baseline += process.cscSegments
    ## process.Baseline += process.ancientMuonSeed
    ## process.Baseline += process.standAloneMuons



    #process.csclocalreco = cms.Sequence(process.csc2DRecHitsOverload*process.cscSegments)
    #process.muonlocalreco = cms.Sequence(process.muonDTDigis+process.dtlocalreco+process.csclocalreco+process.rpcRecHits+process.gemRecHits)


    process.logErrorHarvester = cms.EDProducer("LogErrorHarvester")
    ## process.Baseline += process.logErrorHarvester 
    #process.reconstruction_step = cms.Sequence(process.offlineBeamSpot+process.csclocalreco+process.standAloneMuonSeeds+process.standAloneMuons+process.logErrorHarvester)

    #process.Baseline+=process.reconstruction_step

    process.MEtoEDMConverter = cms.EDProducer("MEtoEDMConverter",
                                              deleteAfterCopy = cms.untracked.bool(True),
                                              Verbosity = cms.untracked.int32(0),
                                              Frequency = cms.untracked.int32(50),
                                              Name = cms.untracked.string('MEtoEDMConverter'),
                                              MEPathToSave = cms.untracked.string('')
                                              )
    ## process.Baseline+=process.MEtoEDMConverter
    
    #process.endOfProcess = cms.Sequence(process.MEtoEDMConverter)
    #process.endjob_step = cms.Sequence(process.endOfProcess)

    #process.Baseline+=process.endjob_step

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
                                             
                                             fileName = cms.untracked.string(outfile+".root") 
                                             )
    #process.RECOSIMoutput_step = cms.Sequence(process.RECOSIMoutput)

    #process.Baseline+=process.RECOSIMoutput


    #process.schedule = cms.Schedule(*[ process.reconstruction_step, process.endjob_step, process.RECOSIMoutput_step ])
    #process.schedule = cms.Schedule(*[ process.endjob_step, process.RECOSIMoutput_step ])
    process.WriteTree = cms.Path(
        #process.Baseline
        process.offlineBeamSpot *
        process.csc2DRecHitsOverload *
        process.cscSegments *
        process.ancientMuonSeed *
        process.standAloneMuons *
        process.logErrorHarvester *
        process.MEtoEDMConverter
        )
    
    return process

