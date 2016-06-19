#include <string>
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TDirectory.h"


void MakeFile(std::string inDirC, std::string inDirD, double fr21, double fr31, double fr41)
{
  TFile* out = new TFile(Form("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-%3.2f-%3.2f-%3.2f.root",fr21, fr31, fr41),"recreate");
  TChain* ch = new TChain("tree/Tree");
  ch->Add(Form("/hadoop/cms/store/user/jbradmil/SingleMuon/%s/0000/*root",inDirC.c_str()));
  //  ch->Add(Form("/hadoop/cms/store/user/jbradmil/SingleMuon/%s/0000/failed/*root",inDirC.c_str()));
  ch->Add(Form("/hadoop/cms/store/user/jbradmil/SingleMuon/%s/0000/*root",inDirD.c_str()));
  //ch->Add(Form("/hadoop/cms/store/user/jbradmil/SingleMuon/%s/0000/failed/*root",inDirD.c_str()));
  out->cd();
  TTree* tree = (TTree*)ch->CopyTree("HLT_IsoMu27_v2");
  printf("Writing tree to %s with %lld entries.\n", out->GetName(), tree->GetEntries());
  tree->Write();
  out->Close();
  return;
}

void MakeDataSkims() {

  MakeFile("crab_-SingleMuon-Run2015C_25ns-16Dec2015-v1-RECO_fr21_0p00_fr31_0p00_fr41_0p00/160531_105831", "crab_-SingleMuon-Run2015D-16Dec2015-v1-RECO_fr21_0p00_fr31_0p00_fr41_0p00/160610_205355", 0.0, 0.0, 0.0);
  MakeFile("crab_-SingleMuon-Run2015C_25ns-16Dec2015-v1-RECO_fr21_0p00_fr31_0p50_fr41_0p50/160531_120159", "crab_-SingleMuon-Run2015D-16Dec2015-v1-RECO_fr21_0p00_fr31_0p50_fr41_0p50/160610_160535", 0.0, 0.5, 0.5);
  MakeFile("crab_-SingleMuon-Run2015C_25ns-16Dec2015-v1-RECO_fr21_0p00_fr31_1p00_fr41_1p00/160531_120353", "crab_-SingleMuon-Run2015D-16Dec2015-v1-RECO_fr21_0p00_fr31_1p00_fr41_1p00/160610_205511", 0.0, 1.0, 1.0);
  
  MakeFile("crab_-SingleMuon-Run2015C_25ns-16Dec2015-v1-RECO_fr21_0p50_fr31_0p00_fr41_0p00/160531_120820", "crab_-SingleMuon-Run2015D-16Dec2015-v1-RECO_fr21_0p50_fr31_0p00_fr41_0p00/160610_205715", 0.5, 0.0, 0.0);
  MakeFile("crab_-SingleMuon-Run2015C_25ns-16Dec2015-v1-RECO_fr21_1p00_fr31_0p00_fr41_0p00/160531_121045", "crab_-SingleMuon-Run2015D-16Dec2015-v1-RECO_fr21_1p00_fr31_0p00_fr41_0p00/160610_205629", 1.0, 0.0, 0.0);

  MakeFile("crab_-SingleMuon-Run2015C_25ns-16Dec2015-v1-RECO_fr21_0p50_fr31_0p50_fr41_0p50/160531_120546", "crab_-SingleMuon-Run2015D-16Dec2015-v1-RECO_fr21_0p50_fr31_0p50_fr41_0p50/160610_155111", 0.5, 0.5, 0.5);
  MakeFile("crab_-SingleMuon-Run2015C_25ns-16Dec2015-v1-RECO_fr21_1p00_fr31_1p00_fr41_1p00/160531_105931", "crab_-SingleMuon-Run2015D-16Dec2015-v1-RECO_fr21_1p00_fr31_1p00_fr41_1p00/160610_161455", 1.0, 1.0, 1.0);


      

  
}
