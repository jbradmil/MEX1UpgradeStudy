# MEX1UpgradeStudy
Code for studying impact of various CSC upgrade scenarios on muon reconstruction. CFEB failure code adapted from studies by Devin Taylor: https://github.com/dntaylor/CSCPostLS2RateStudies. 

## Instructions for running in CMSSW_7_6_X


```
export SCRAM_ARCH=slc6_amd64_gcc493
cmsrel CMSSW_7_6_3
cd CMSSW_7_6_3/src/
cmsenv
git clone git@github.com:jbradmil/MEX1UpgradeStudy.git -b CMSSW_7_6
scram b -j 8
```
This version of the code is structured similarly to the version written for CMSSW_6, but is now compliant with the 7_X standard (e.g. getByToken...). The trees now store slightly different variables, but their content should be fairly transparent. At the moment, the NtupleMaker does not save any gen information. In principle, this should be easy to add back in, following the example from the CMSSW_6 version, and taking advantage of the `isData` parameter in NTupleMaker (see below).


* Steps 1-2: The top-level python config script for re-running RECO is now [run_reco_on_reco_cfg.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/master/Production/test/run_reco_on_reco_cfg.py) for data and [run_reco_on_reco_mc_cfg.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/mast\
er/Production/test/run_reco_on_reco_mc_cfg.py) for MC. The only difference between the two is that the data version takes a JSON file to filter out the bad runs. One of the inputs to NTupleMaker is now a bool called `isData.` This should be useful in adding gen information to the MC trees without introducing any problems for data.

### Quick start guide 
The top-level script for re-running RECO on MC is [run_reco_on_reco_mc_cfg.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/mast\
er/Production/test/run_reco_on_reco_mc_cfg.py). It
should run out-of-the-box on a sample DY file if you do

```
cd Production/test/
cmsRun run_reco_on_reco_mc_cfg.py
```

To process a full dataset, you can create a crab task by running
[crabSubmit.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/CMSSW_7_6/Production/test/crabSubmit.py) for data or [crabSubmitMC.py] (https://github.com/jbradmil/MEX1UpgradeStudy/blob/CMSSW_7_6/Production/test/crabSubmitMC.py) for MC. First, you will want to edit the file to select the
dataset and the desired fail rates for the ME2/1, 3/1, and 4/1
chambers. Then (assuming you've configured CRAB correctly), you can do

```
crab submit -c crabSubmit.py
```

It should be pretty easy to make some basic plots from these simple trees. You can find some of my plotting scripts in [Plotting](https://github.com/jbradmil/MEX1UpgradeStudy/tree/master/Plotting)
