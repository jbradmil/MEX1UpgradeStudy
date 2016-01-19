# Read parameters
from MEX1UpgradeStudy.Utils.CommandLineParams import CommandLineParams
parameters = CommandLineParams()
#scenarioName=parameters.value("scenario","")
inputFilesConfig=parameters.value("inputFilesConfig","")
dataset=parameters.value("dataset",[])
nstart = parameters.value("nstart",0)
nfiles = parameters.value("nfiles",-1)
numevents=parameters.value("numevents",-1)
reportfreq=parameters.value("reportfreq",1000)
outfile=parameters.value("outfile","test_run")

# CFEB failure options
failureRate=parameters.value('failureRate', 0.15)
doUniformFailure=parameters.value('doUniformFailure', True)
doCFEBFailure=parameters.value('doCFEBFailure', True)


import FWCore.ParameterSet.Config as cms
## all-important process definition, after reading in parameters
process = cms.Process("RERECO")

# configure geometry & conditions
process.load('Configuration.Geometry.GeometryExtended2023HGCalMuonReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')

# Load input files
readFiles = cms.untracked.vstring()

if inputFilesConfig!="" :
    if nfiles==-1:
        process.load("MEX1UpgradeStudy.Production."+inputFilesConfig+"_cff")
        readFiles.extend( process.source.fileNames )
    else:
        readFilesImport = getattr(__import__("MEX1UpgradeStudy.Production."+inputFilesConfig+"_cff",fromlist=["readFiles"]),"readFiles")
        readFiles.extend( readFilesImport[nstart:(nstart+nfiles)] )

if dataset!=[] :    
    readFiles.extend( [dataset] )

#temporary redirector fix
redir="root://cmsxrootd.fnal.gov/"
    
# print out settings
print "***** SETUP ************************************"
print " dataset: "+str(readFiles)
print " outfile: "+outfile+"_RERECO.root"
print " "
print " failureRate:: "+str(failureRate)
print " doUniformFailure: "+str(doUniformFailure)
print " doCFEBFailure: "+str(doCFEBFailure)

from MEX1UpgradeStudy.CFEBBufferOverloadProducer.rereco_cff import RERECO
process = RERECO(process,
    outfile=outfile+"_RERECO",
    reportfreq=reportfreq,
    dataset=readFiles,
    #globaltag=globaltag,
    numevents=numevents,
    failureRate=failureRate,
    doUniformFailure=doUniformFailure,
    doCFEBFailure=doCFEBFailure
)

# final tweaks to process
print " final tweaks..."
process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')
process.TFileService.closeFileFast = cms.untracked.bool(True)
