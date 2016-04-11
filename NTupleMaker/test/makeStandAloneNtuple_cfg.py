
import FWCore.ParameterSet.Config as cms


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

process.MessageLogger.cerr.FwkReport.reportEvery = 5000

# Two options for inputting files:
# 1: List the files by hand as a vector of strings in fileNames = cms.untracked.vstring(...) below
# 2: Specify a text file listing all input files, one file path per line in mylist = FileUtils.loadListFromFile(...) below. This list will be passed to PoolSource if you do fileNames = cms.untracked.vstring(readFiles).

import FWCore.Utilities.FileUtils as FileUtils
mylist = FileUtils.loadListFromFile ("sample.list") 
readFiles = cms.untracked.vstring( *mylist)

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(readFiles)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )


process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string('test_flat_tree.root'),
                                   )

from Configuration.AlCa.GlobalTag import GlobalTag

process.skim = cms.EDAnalyzer("NTupleMaker",
                              muons = cms.InputTag("muons","","RERECO"),
                              pvs = cms.InputTag("goodOfflinePrimaryVertices","","RERECO"),
                              genParticles = cms.InputTag("genParticles"),
                              )

process.p = cms.Path(
    process.skim
    )
