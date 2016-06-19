#include <string>
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TDirectory.h"


void MakeFile(std::string inDir, double fr21, double fr31, double fr41)
{
  TFile* out = new TFile(Form("~/muon_trees/trees/DYMC/skims/DY-FR-%3.2f-%3.2f-%3.2f.root", fr21, fr31, fr41),"recreate");
  TChain* ch = new TChain("tree/Tree");
  ch->Add(Form("/hadoop/cms/store/user/jbradmil/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/%s/0000/*root",inDir.c_str()));
  printf("DY-FR-%3.2f-%3.2f-%3.2f.root: found %d input files\n", fr21, fr31, fr41, ch->GetNtrees());
  out->cd();
  TTree* tree = (TTree*)ch->CopyTree("HLT_IsoMu27_v2");
  tree->Write();
  out->Close();
  return;
}

void MakeMCSkims() {

  MakeFile("crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_fr21_0p00_fr31_0p00_fr41_0p00_reco_to_rereco/160601_152209", 0.0, 0.0, 0.0);

  // MakeFile("crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_fr21_0p50_fr31_0p00_fr41_0p00_reco_to_rereco/160601_152510", 0.5, 0.0, 0.0);
  // MakeFile("crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_fr21_1p00_fr31_0p00_fr41_0p00_reco_to_rereco/160601_152425", 1.0, 0.0, 0.0);


  // MakeFile("crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_fr21_0p00_fr31_0p50_fr41_0p50_reco_to_rereco/160601_152550", 0.0, 0.5, 0.5);
  // MakeFile("crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_fr21_0p00_fr31_1p00_fr41_1p00_reco_to_rereco/160601_153316", 0.0, 1.0, 1.0);
  
  // MakeFile("crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_fr21_0p50_fr31_0p50_fr41_0p50_reco_to_rereco/160601_152253", 0.5, 0.5, 0.5);
  // MakeFile("crab_DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_fr21_1p00_fr31_1p00_fr41_1p00_reco_to_rereco/160601_152338", 1.0, 1.0, 1.0);
  
}
