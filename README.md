# MEX1UpgradeStudy
Code for studying impact of various CSC upgrade scenarios on muon reconstruction. CFEB failure code adapted from studies by Devin Taylor: https://github.com/dntaylor/CSCPostLS2RateStudies. 

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

There are 3 main stages to this exercise: (1) re-running RECO on a
SLHC GEN-SIM-RECO sample to simulate CFEB failure and RecHit loss, (2)
making a flat tree out of the RE-RECO'd sample that (at-the-moment)
stores gen muons, which may or may not be matched to StandAlone muons,
and (3) making some simple efficiency plots and kinematic
distributions from this tree. Steps (1) and (2) can and probably
should be merged into one step soon.

### Quick start guide 
The top-level script for re-running RECO is [run_rereco_cfg.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/Production/test/run_rereco_cfg.py). It
should run out-of-the-box on a sample Z' file if you do

```
cd Production/test/
cmsRun run_rereco_cfg.py
```

To process a full dataset, you can create a crab task by running
crabSubmit.py. First, you will want to edit the file to select the
dataset and the desired fail rates for the ME2/1, 3/1, and 4/1
chambers. Then (assuming you've configured CRAB correctly), you can do

```
crab submit -c crabSubmit.py
```
