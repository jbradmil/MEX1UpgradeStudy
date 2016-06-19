# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: RECO -s RAW2DIGI,L1Reco,RECO,ALCA:TkAlMuonIsolated+DtCalib+MuAlCalIsolatedMu+MuAlOverlaps+MuAlZMuMu+HcalCalIterativePhiSym,EI,PAT,DQM:@standardDQM+@miniAODDQM --runUnscheduled --nThreads 4 --data --scenario pp --conditions 76X_dataRun2_v15 --eventcontent RECO,AOD,MINIAOD,DQM --datatier RECO,AOD,MINIAOD,DQMIO --customise Configuration/DataProcessing/RecoTLR.customiseDataRun2Common_25ns --filein blah.root -n 100 --no_exec --python_filename=reco_Run2015D_SingleMuon.py --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('RERECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.Reconstruction_Data_cff')
process.load('Configuration.StandardSequences.AlCaRecoStreams_cff')
process.load('CommonTools.ParticleFlow.EITopPAG_cff')
process.load('PhysicsTools.PatAlgos.slimming.metFilterPaths_cff')
process.load('DQMOffline.Configuration.DQMOffline_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
process.MessageLogger.cerr.default.limit = 1000


options.inputFiles = "file:/hadoop/cms/store/user/jbradmil/CSCData/SingleMuon-Run2015C-RECO.root"
options.outputFile = "rereco_fail_me21_me31_me41_0p0000.root"
options.maxEvents = "-1"
options.register ('isData', True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "Run on data or MC")
options.register ('failRateME21', 0.0, VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME2/1 failure probability")
options.register ('failRateME31', 0.0, VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME3/1 failure probability")
options.register ('failRateME41', 0.0, VarParsing.multiplicity.singleton, VarParsing.varType.float, "ME4/1 failure probability")
options.register ('doUniformFailure', True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "run failure in all chambers evenly")
options.register ('doCFEBFailure', True, VarParsing.multiplicity.singleton, VarParsing.varType.bool, "explicitly set CFEB failure")
options.register ('jsonfile', 'Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt', VarParsing.multiplicity.singleton, VarParsing.varType.string, "JSON file")
options.parseArguments()

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:/hadoop/cms/store/user/jbradmil/CSCData/SingleMuon-Run2015D-RAW-256675.root'),
    #fileNames = cms.untracked.vstring('/store/data/Run2015C/SingleMuon/RAW/v1/000/254/227/00000/52C0ABA8-8441-E511-A398-02163E014390.root'),
    fileNames = cms.untracked.vstring(options.inputFiles),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('RECO nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

import FWCore.PythonUtilities.LumiList as LumiList
if len(options.jsonfile)>0: process.source.lumisToProcess = LumiList.LumiList(filename = options.jsonfile).getVLuminosityBlockRange()


from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '76X_dataRun2_v15', '')

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

process.muons.PFCandidates = cms.InputTag("particleFlow")
process.muons.FillPFIsolation = False
process.muons.FillSelectorMaps = False 
process.muons.FillCosmicsIdMap =  False
process.muons.FillTimingInfo = False
process.muons.FillDetectorBasedIsolation = False 
process.muons.FillShoweringInfo = False

# Path and EndPath definitions
process.reconstruction_step = cms.Path(process.csc2DRecHitsOverload+process.cscSegments+process.globalMuons+process.tevMuons+process.muons1stStep+process.muons)
process.eventinterpretaion_step = cms.Path(process.EIsequence)
process.Flag_goodVertices = cms.Path(process.primaryVertexFilter)

process.tree = cms.EDAnalyzer("NTupleMaker",
                              isData = cms.bool(True),
                              muons = cms.InputTag("muons","","RERECO"),
                              bits = cms.InputTag("TriggerResults","","HLT"),
                              objects = cms.InputTag("selectedPatTrigger"),
                              pvs = cms.InputTag("offlineSlimmedPrimaryVertices"),
                              )
process.output_step = cms.EndPath(process.tree)


process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile),
    )


# Schedule definition
process.schedule = cms.Schedule(process.reconstruction_step,process.eventinterpretaion_step,process.output_step)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.RecoTLR
from Configuration.DataProcessing.RecoTLR import customiseDataRun2Common_25ns 

#call to customisation function customiseDataRun2Common_25ns imported from Configuration.DataProcessing.RecoTLR
process = customiseDataRun2Common_25ns(process)

# End of customisation functions
#do not add changes to your config after this point (unless you know what you are doing)
from FWCore.ParameterSet.Utilities import convertToUnscheduled
process=convertToUnscheduled(process)
process.load('Configuration.StandardSequences.PAT_cff')
from FWCore.ParameterSet.Utilities import cleanUnscheduled
process=cleanUnscheduled(process)

# customisation of the process.
from PhysicsTools.PatAlgos.tools.trigTools import switchOnTriggerStandAlone
switchOnTriggerStandAlone( process, outputModule = '' )
process.patTrigger.packTriggerPathNames = cms.bool(True)

