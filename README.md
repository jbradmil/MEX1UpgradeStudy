# MEX1UpgradeStudy
Code for studying impact of various CSC upgrade scenarios on muon reconstruction. CFEB failure code adapted from studies by Devin Taylor: https://github.com/dntaylor/CSCPostLS2RateStudies. 

## Instructions

Currently, the code is validated for running in SLHC releases of CMSSW 6 on the UCSD T2. To get started, you'll need to do:

```
export SCRAM_ARCH=slc6_amd64_gcc472
cmsrel CMSSW_6_2_0_SLHC25_patch1
cd CMSSW_6_2_0_SLHC25_patch1/src/
cmsenv
git clone git@github.com:jbradmil/MEX1UpgradeStudy.git
scram b -j 8
```

There are 3 main stages to this exercise: (1) re-running RECO on a
SLHC GEN-SIM-RECO sample to simulate CFEB failure and RecHit loss, (2)
making a flat tree out of the RE-RECO'd sample that (at-the-moment)
stores gen muons, which may or may not be matched to StandAlone muons,
and (3) making some simple efficiency plots and kinematic
distributions from this tree. Steps (1) and (2) have been merged into one step.

* Steps 1-2: The top-level python config script for re-running RECO is [run_rereco_and_nutpler_cfg.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/Production/test/run_rereco_and_ntupler_cfg.py). It takes as input a GEN-SIM-RECO file and a 3 desired fail rates, one for all CFEBs in all ME2/1 chambers, one for all CFEBs in all ME3/1 chambers, and one for all CFEBs in all ME4/1 chambers. This calls the [CFEBBufferOverloadProducer] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/CFEBBufferOverloadProducer/src/CFEBBufferOverloadProducer.cc) module, which uses random number generators to remove CFEB data at the specified fail rates. The output muon collection from this re-reconstruction step is then passed to the [NTupleMaker] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/NTupleMaker/src/NTupleMaker.cc), which makes a flat tree of the reconstructed muons and the gen muons.  In principle, you can still run Steps 1 and 2 separately, using the scripts [Zprime_RECO_PU.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/Production/test/Zprime_RECO_PU.py) and [makeStandAloneNtuple_cfg.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/NTupleMaker/test/makeStandAloneNtuple_cfg.py).
*  Step 3: Once you have a flat tree, you can make plots. The scripts I use to maek most of my efficiency plots are [PlotEfficiencies.C] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/Plotting/PlotEfficiencies.C) and [PlotEfficiencies2D.C] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/Plotting/PlotEfficiencies2D.C). Note that to run these you will need to specify multiple input files for various CFEB failure scenarios.

### Quick start guide 
The top-level script for re-running RECO is [run_rereco_and_ntupler_cfg.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/Production/test/run_rereco_and_nutpler cfg.py). It
should run out-of-the-box on a sample Z' file if you do

```
cd Production/test/
cmsRun run_rereco_and_ntupler_cfg.py
```

To process a full dataset, you can create a crab task by running
crabSubmit.py. First, you will want to edit the file to select the
dataset and the desired fail rates for the ME2/1, 3/1, and 4/1
chambers. Then (assuming you've configured CRAB correctly), you can do

```
crab submit -c crabSubmit.py
```

It should be pretty easy to make some basic plots from these simple trees. You can find some of my plotting scripts in [Plotting](https://github.com/jbradmil/MEX1UpgradeStudy/tree/master/Plotting)
