#include "MEX1UpgradeStudy/NTupleMaker/interface/NTupleMaker.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// Constructor
NTupleMaker::NTupleMaker(const edm::ParameterSet& ps) : tree_(0) {

   muons_ = ps.getParameter<edm::InputTag>("muons");
   genParticles_ = ps.getParameter<edm::InputTag>("genParticles");

   cscSTA = new CSCDataFormats::StandAloneMuon();
   cscSTA_data = cscSTA->getData();

   tree_ = tfs_->make<TTree>("RecoTree", "RecoTree");
   tree_->Branch("StandAloneMuon", "CSCDataFormats::StandAloneMuonDataFormat", &cscSTA_data, 32000, 3);

}

NTupleMaker::~NTupleMaker() {
   
   delete cscSTA;

}

void NTupleMaker::beginJob(void) {
}

void NTupleMaker::endJob() {
}

void NTupleMaker::analyze(const edm::Event& e, const edm::EventSetup& es) {

   cscSTA->Reset();
   cscSTA->Set(e,muons_,genParticles_); 
   tree_->Fill();

}

//define this as a plug-in
DEFINE_FWK_MODULE(NTupleMaker);
