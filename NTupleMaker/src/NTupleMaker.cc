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

   samuons_ = ps.getParameter<edm::InputTag>("samuons");
   gmuons_ = ps.getParameter<edm::InputTag>("gmuons");
   pvs_ = ps.getParameter<edm::InputTag>("pvs");
   genParticles_ = ps.getParameter<edm::InputTag>("genParticles");

   cscMuon = new CSCDataFormats::CSCMuon();
   //cscSTA_data = cscSTA->getStandAloneCollection();
   csc_data = cscMuon->getCollections();

   tree_ = tfs_->make<TTree>("Tree", "Tree");
   tree_->Branch("CSCMuon", "CSCDataFormats::CSCDataFormat", &csc_data, 32000, 3);

}

NTupleMaker::~NTupleMaker() {
   
   delete cscMuon;

}

void NTupleMaker::beginJob(void) {
}

void NTupleMaker::endJob() {
}

void NTupleMaker::analyze(const edm::Event& e, const edm::EventSetup& es) {

   cscMuon->Reset();
   cscMuon->Set(e, samuons_, gmuons_, pvs_, genParticles_); 
   tree_->Fill();

}

//define this as a plug-in
DEFINE_FWK_MODULE(NTupleMaker);
