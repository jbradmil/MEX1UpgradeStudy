import FWCore.ParameterSet.Config as cms

###
# Defaults
###
## from FWCore.ParameterSet.VarParsing import VarParsing
## options = VarParsing ('analysis')

## #options.inputFiles = "file:GEM2019Upg14_CFEBBufferOverload_0p01.root"
## #options.inputFiles = "file:/hdfs/store/user/dntaylor/BufferOverload_HZZ_0p10_CFEB_v8-CFEBBufferOverload_cfg/CFEBBufferOverload_cfg-GEM2019Upg14_9.root"
## options.outputFile = "CFEBBufferOverload_StandAloneMuon.root"
## options.parseArguments()

###
# Run Skim
###
process = cms.Process("StandAloneMuonSkim")

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
#process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
                                     #    SkipEvent = cms.untracked.vstring('ProductNotFound')
    )

process.MessageLogger.cerr.FwkReport.reportEvery = 1000

# load long list of files from txt
import FWCore.Utilities.FileUtils as FileUtils
mylist = FileUtils.loadListFromFile ('rereco_me31f0p25_me41_0p25.list') 
readFiles = cms.untracked.vstring( *mylist)

process.source = cms.Source("PoolSource",
                            fileNames = readFiles
                            )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('ME31F0p25_ME41_F0p25.root'),
                                   )

from Configuration.AlCa.GlobalTag import GlobalTag

process.skim = cms.EDAnalyzer("NTupleMaker",
                              muons = cms.InputTag("standAloneMuons","","RERECO"),
                              genParticles = cms.InputTag("genParticles"),
                              )

process.p = cms.Path(
    process.skim
    )
