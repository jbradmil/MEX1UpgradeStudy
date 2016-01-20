#!/bin/bash

#
# variables from arguments string in jdl
#

echo "Starting job on " `date` #Only to display the starting of production date
echo "Running on " `uname -a` #Only to display the machine where the job is running
echo "System release " `cat /etc/redhat-release` #And the system release
echo "CMSSW on Condor"

CMSSWVER=$1
OUTDIR=$2
SAMPLE=$3
NPART=$4
NSTART=$5
NFILES=$6
SCENARIO=$7
FAILRATEME31=$7

EOSDIR=root://cmseos.fnal.gov/${OUTDIR}
LOCALDIR=/eos/uscms${OUTDIR}

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "OUTDIR:     $OUTDIR"
echo "SAMPLE:     $SAMPLE"
echo "NPART:      $NPART"
echo "NSTART:     $NSTART"
echo "NFILES:     $NFILES"

echo "ME31 FAIL RATE: $FAILRATEME31"
echo "ME41 FAIL RATE: $FAILRATEME41"

#echo "SCENARIO:   $SCENARIO"

echo "EOSDIR:     $EOSDIR"
echo "LOCALDIR:   $LOCALDIR"


tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
scram b ProjectRename
source /cvmfs/cms.cern.ch/cmsset_default.sh
# cmsenv
eval `scramv1 runtime -sh`
cd -

echo "Copying file locally..."
echo "xrdcp root://cmsxrootd.fnal.gov/$SAMPLE $EOSDIR/GEN_SIM_RECO_$NPART.root"
xrdcp root://cmsxrootd.fnal.gov/${SAMPLE} ${EOSDIR}/GEN_SIM_RECO_${NPART}.root 2>&1
XRDEXIT=$?
if [[ $XRDEXIT -ne 0 ]]; then
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
fi
# run CMSSW
echo "cmsRun rereco_cfg.py outputFile=RERECO_${NPART}.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0. failRateME41=0."
cmsRun rereco_cfg.py outputFile=RERECO_${NPART}_ME31F0p0_ME41_F0p0.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0. failRateME41=0.>&1
echo "cmsRun rereco_cfg.py outputFile=RERECO_${NPART}.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0.25 failRateME41=0.25"
cmsRun rereco_cfg.py outputFile=RERECO_${NPART}_ME31F0p25_ME41_F0p25.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0.25 failRateME41=0.25>&1
echo "cmsRun rereco_cfg.py outputFile=RERECO_${NPART}.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0.5 failRateME41=0.5"
cmsRun rereco_cfg.py outputFile=RERECO_${NPART}_ME31F0p5_ME41_F0p5.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0.5 failRateME41=0.5>&1
echo "cmsRun rereco_cfg.py outputFile=RERECO_${NPART}.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0.75 failRateME41=0.75"
cmsRun rereco_cfg.py outputFile=RERECO_${NPART}_ME31F0p75_ME41_F0p75.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=0.75 failRateME41=0.75>&1
echo "cmsRun rereco_cfg.py outputFile=RERECO_${NPART}.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=1. failRateME41=1."
cmsRun rereco_cfg.py outputFile=RERECO_${NPART}_ME31F1p0_ME41_F1p1.root inputFiles=${EOSDIR}/GEN_SIM_RECO_${NPART}.root failRateME31=1. failRateME41=1.>&1

#CMSEXIT=$?

#if [[ $CMSEXIT -ne 0 ]]; then
#  rm *.root
#  echo "exit code $CMSEXIT, skipping xrdcp"
#  exit $CMSEXIT
#fi

# copy output to eos
echo "xrdcp output for condor"
for FILE in RERECO*.root
do
  echo "xrdcp -f ${FILE} ${EOSDIR}/RERECO/${FILE}"
  xrdcp -f ${FILE} ${EOSDIR}/RERECO/${FILE} 2>&1
  XRDEXIT=$?
  if [[ $XRDEXIT -ne 0 ]]; then
    #rm RERECO*.root
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
  fi
  rm ${FILE}
  rm ${LOCALDIR}/GEN_SIM_RECO_${NPART}.root
done
