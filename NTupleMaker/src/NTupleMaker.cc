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
NTupleMaker::NTupleMaker(const edm::ParameterSet& iConfig) : tree_(0) {

  isData_ = iConfig.getParameter<bool>("isData");
  muons_ = iConfig.getParameter<edm::InputTag>("muons");
  muonTok_ = consumes<edm::View<reco::Muon>>(muons_);
  bits_ = iConfig.getParameter<edm::InputTag>("bits");
  bitsTok_ = consumes<edm::TriggerResults>(bits_);
  trigObjects_ = iConfig.getParameter<edm::InputTag>("objects");
  trigObjectsTok_ = consumes<pat::TriggerObjectStandAloneCollection>(trigObjects_);
  pvs_ = iConfig.getParameter<edm::InputTag>("pvs");
  pvTok_ = consumes<reco::VertexCollection>(pvs_);
  // genParticles_ = iConfig.getParameter<edm::InputTag>("genParticles");

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

void NTupleMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& es) {

  cscMuon->Reset();

  edm::Handle<edm::View<reco::Muon> > muons;
  iEvent.getByToken(muonTok_,muons);

  edm::Handle<edm::TriggerResults> triggerBits;
  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects;
  iEvent.getByToken(bitsTok_, triggerBits);
  iEvent.getByToken(trigObjectsTok_, triggerObjects);

  
  edm::Handle<reco::VertexCollection> pvs;
  iEvent.getByToken(pvTok_, pvs);

   
  cscMuon->Set(iEvent, isData_, muons, triggerBits, triggerObjects, pvs); 
  tree_->Fill();

}

//define this as a plug-in
DEFINE_FWK_MODULE(NTupleMaker);
