import os, bisect
from optparse import OptionParser
from FWCore.PythonUtilities.LumiList import LumiList

# define options
parser = OptionParser()
parser.add_option("-n", "--nFiles", dest="nFiles", default=1,
                                    help="number of files to process per job (default = %default)")

parser.add_option("-f","--filesConfig",dest="filesConfig", default="2023SHCalNoTaper_PU140.DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola",
                                       help="config file from which to retrieve the full file list (default = %default)")

parser.add_option("-o","--outputDir",dest="outputDir", default="",
                                       help="path to ouput directory in which root files will be stored  (default = %default)")

parser.add_option("-s","--submit",dest="submit", default=False,action="store_true",
                                       help="submit jobs to condor once they are configured  (default = %default)")

#parser.add_option("-c","--scenario",dest="scenario", default="Spring15",
#                                       help="scenario: Spring15, Spring15v2, Spring15Fast, Spring15Fastv2, 2015C, re2015C, 2015D, re2015D, 2015Db  (default = %default)")
                                       
parser.add_option("-j","--firstJob",dest="firstJob", default=0,
                                       help="first job to submit  (default = %default)")
                                       

(options, args) = parser.parse_args()

if options.outputDir=="":
    raise Exception, 'No ouput directory (-o) specified'
    
# fix malformed options
if options.filesConfig[-7:]=="_cff.py":
    options.filesConfig = options.filesConfig[:-7]
elif options.filesConfig[-4:]=="_cff":
    options.filesConfig = options.filesConfig[:-4]
    
# verify specified options
print "nFiles: ",options.nFiles
print "filesConfig: ",options.filesConfig
#print "scenario: ",options.scenario
print "submit: ",options.submit
print "firstJob: ",options.firstJob



# grab full file list from config files
readFiles = getattr(__import__("MEX1UpgradeStudy.Production."+options.filesConfig+"_cff",fromlist=["readFiles"]),"readFiles")

# to keep track of how many data files have been divied up
fileListLen = len(readFiles)

print "There are "+str(fileListLen)+" files in your sample"

# calculate the number of jobs necessary
if options.nFiles==-1:
    nJobs = 1
else:
    nJobs = fileListLen / int( options.nFiles )
    if ( fileListLen % int( options.nFiles ) != 0 ) :
        nJobs += 1

netJobs = nJobs - int(options.firstJob)
print "I will create "+str(netJobs)+" jobs for you!"
if options.firstJob>0: print "(starting from job "+str(options.firstJob)+")"

# start loop over N jobs
nActualJobs = 0
for iJob in range( int(options.firstJob), nJobs ) :
    # get starting file number
    nstart = iJob*int(options.nFiles)

    # replace placeholders in template files
    jobname = options.filesConfig
    if nJobs>1: jobname = jobname+"_"+str(iJob)
    os.system("sed -e 's|CMSSWVER|'${CMSSW_VERSION}'|g' "\
                 +"-e 's~OUTDIR~"+options.outputDir+"~g' "\
                 +"-e 's|JOBNAME|"+jobname+"|g' "\
                 +"-e 's|SAMPLE|"+options.filesConfig+"|g' "\
                 +"-e 's|NPART|"+str(iJob)+"|g' "\
                 +"-e 's|NSTART|"+str(nstart)+"|g' "\
                 +"-e 's|NFILES|"+str(options.nFiles)+"|g' "\
                 #+"-e 's|SCENARIO|"+options.scenario+"|g' "\
                 +"< jobExecCondor.jdl > jobExecCondor_"+jobname+".jdl")

    # submit jobs to condor, if -s was specified
    if ( options.submit ) :
        os.system("condor_submit jobExecCondor_"+jobname+".jdl")
        
    nActualJobs += 1
