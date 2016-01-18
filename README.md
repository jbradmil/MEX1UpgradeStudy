# MEX1UpgradeStudy
Code for studying impact of various CSC upgrade scenarios on muon reconstruction. CFEB failure code adapted from studies by Devin Taylor: https://github.com/dntaylor/CSCPostLS2RateStudies. Batch submission code adapted from RA2/b TreeMaker: https://github.com/TreeMaker/TreeMaker.

## Instructions

Currently, the code runs in SLHC releases of CMSSW 6. To get started, you'll need to do:

```
export SCRAM_ARCH=slc6_amd64_gcc481
cmsrel CMSSW_6_2_0_SLHC26_patch2
cd CMSSW_6_2_0_SLHC26_patch2/src/
cmsenv
git clone git@github.com:jbradmil/MEX1UpgradeStudy.git
scram b -j 8
```
