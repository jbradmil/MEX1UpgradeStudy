#!/bin/bash

if [ "$1" == 1 ]; then
  echo "Need to specify output directory in argument 1"
  exit
fi

OUTPUTDIR=$1
KEEPTAR=$2

./FScheck.sh "$KEEPTAR"

SAMPLES=(
    #2023SHCalNoTaper_PU140.DYToMuMu_M-20_TuneZ2star_14TeV-pythia6-tauola
    Summer13-UpgradePhase1Age3H_DR61SLHCx_PU140Bx25.DYToMuMu_M-20_TuneZ2star_14TeV-pythia6
)

for SAMPLE in ${SAMPLES[@]}; do
  python generateSubmission.py -n 1 -o ${OUTPUTDIR} -f ${SAMPLE}
done
