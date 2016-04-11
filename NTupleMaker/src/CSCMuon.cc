#include "MEX1UpgradeStudy/NTupleMaker/interface/CSCMuon.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include <DataFormats/TrackReco/interface/Track.h>
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include <cmath>
#include "TLorentzVector.h"

CSCDataFormats::CSCMuon::CSCMuon() 
{
}

CSCDataFormats::CSCMuon::~CSCMuon() 
{
}

void CSCDataFormats::CSCMuon::Set(const edm::Event& e, const edm::InputTag& samuons_, const edm::InputTag& gmuons_, const edm::InputTag& pvs_, const edm::InputTag& genParticles_) 
{
  edm::Handle<std::vector<reco::Track>> samuons;
  e.getByLabel(samuons_,samuons);

  edm::Handle<std::vector<reco::Track>> gmuons;
  e.getByLabel(gmuons_,gmuons);

  edm::Handle<std::vector<reco::GenParticle>> genParticles;
  e.getByLabel(genParticles_,genParticles);

  edm::Handle<std::vector<reco::Vertex>> pvs;
  e.getByLabel(pvs_,pvs);
  coll_.NPV = pvs->size();


  for(std::vector<reco::GenParticle>::const_iterator igen=genParticles->begin(); igen!=genParticles->end(); ++igen){
    if (!(std::abs(igen->pdgId())==13 && igen->status()==1)) continue; // only gen final state muons
    coll_.GenTLV.push_back( TLorentzVector(igen->px(), igen->py(), igen->pz(), igen->energy()) );
    coll_.GenID.push_back(igen->pdgId());
    coll_.GenMotherID.push_back(GetMotherID(&(*igen)));
    bool foundSA=false;
    bool foundG=false;
    for(std::vector<reco::Track>::const_iterator isa=samuons->begin(); isa!=samuons->end(); ++isa){ // was it reconstructed as SA?
      if (std::abs(deltaR<reco::Track,reco::GenParticle>(*isa,*igen))<0.2) { // found a match
	coll_.GenSAMatch.push_back(true);
	coll_.GenSAMatchTLV.push_back( TLorentzVector(isa->px(), isa->py(), isa->pz(), sqrt(isa->px()*isa->px() + isa->py()*isa->py() + isa->pz()*isa->pz())) );
	coll_.GenSAMatchDZ.push_back(fabs(isa->dz()));
	coll_.GenSAMatchDXY.push_back(fabs(isa->d0()));
	coll_.GenSADeltaPt.push_back(isa->pt()-igen->pt());
	coll_.GenSAResolution.push_back(1/isa->pt()-1/igen->pt());
	foundSA=true;
	// now try to match to global muon
	for(std::vector<reco::Track>::const_iterator iglobal=gmuons->begin(); iglobal!=gmuons->end(); ++iglobal){ // was it reconstructeas global?
	  if (std::abs(deltaR<reco::Track,reco::GenParticle>(*iglobal,*igen))<0.2) { // found a match
	    coll_.GenGMatch.push_back(true);
	    coll_.GenGMatchTLV.push_back( TLorentzVector(iglobal->px(), iglobal->py(), iglobal->pz(), sqrt(iglobal->px()*iglobal->px() + iglobal->py()*iglobal->py() + iglobal->pz()*iglobal->pz())) );
	    coll_.GenGMatchDZ.push_back(fabs(iglobal->dz()));
	    coll_.GenGMatchDXY.push_back(fabs(iglobal->d0()));
	    coll_.GenGDeltaPt.push_back(iglobal->pt()-igen->pt());
	    coll_.GenGResolution.push_back(1/iglobal->pt()-1/igen->pt());
	    foundG=true;
	    break;
	  } // global match
	} // global muon loop
      } // standalone match
    }// standalone muon loop
    if (!foundSA) {
      coll_.GenSAMatch.push_back(false);
      coll_.GenSAMatchTLV.push_back( TLorentzVector(0,0,0,0) );
      coll_.GenSAMatchDZ.push_back(-9.);
      coll_.GenSAMatchDXY.push_back(-9.);
      coll_.GenSADeltaPt.push_back(-999.);
      coll_.GenSAResolution.push_back(-999.);
      if (!foundG) {
	coll_.GenGMatch.push_back(false);
	coll_.GenGMatchTLV.push_back( TLorentzVector(0,0,0,0) );
	coll_.GenGMatchDZ.push_back(-9.);
	coll_.GenGMatchDXY.push_back(-9.);
	coll_.GenGDeltaPt.push_back(-999.);
	coll_.GenGResolution.push_back(-999.);
      }
    }
  } // gen particles loop
   
    // standalone loop -- under development
  int nSA(0);
  for(std::vector<reco::Track>::const_iterator isa=samuons->begin(); isa!=samuons->end(); ++isa){
    // if (fabs(isa->dz()) > 20 || fabs(isa->d0()) > 0.3) continue; // most basic IP cuts
    nSA++;
    //    printf("SA muon %d\n", nSA);

    coll_.SATLV.push_back( TLorentzVector(isa->px(), isa->py(), isa->pz(), sqrt(isa->px()*isa->px() + isa->py()*isa->py() + isa->pz()*isa->pz())) );
    coll_.SAdZ.push_back(isa->dz());
    coll_.SAd0.push_back(isa->d0());
    double minDR=999.;
    reco::GenParticle* match(NULL);
    for(std::vector<reco::GenParticle>::const_iterator igen=genParticles->begin(); igen!=genParticles->end(); ++igen){ // now match this to gen particle
      if (igen->status()!=1) continue; // final state only
      double DR = std::abs(deltaR<reco::Track,reco::GenParticle>(*isa,*igen));
      if (DR<0.2&&abs(igen->pdgId())==13) {
  	match = (reco::GenParticle*)&(*igen);
	break;
      }
      //    printf("\tFinal state particle %d -- DR = %3.4f\n", igen->pdgId(), DR);
      if (DR<minDR) {
  	minDR=DR;
  	match = (reco::GenParticle*)&(*igen);
	//	printf("\tThis is the new match!\n");
	//  	if (DR<0.2&&abs(igen->pdgId())==13) break;
      }
    } // gen loop
    coll_.SAGenMatchDR.push_back(minDR);
    if (minDR<0.2) {
      coll_.SAGenMatch.push_back(true);
      coll_.SAGenMatchID.push_back(match->pdgId());
      coll_.SAGenMatchMotherID.push_back(GetMotherID(match));
    } else{
      coll_.SAGenMatch.push_back(false);
      coll_.SAGenMatchID.push_back(0);
      coll_.SAGenMatchMotherID.push_back(0);
    }
  } // standalone loop

      // global muon loop -- under development
  int nG(0);
  for(std::vector<reco::Track>::const_iterator iglobal=gmuons->begin(); iglobal!=gmuons->end(); ++iglobal){
    //if (fabs(iglobal->dz()) > 20 || fabs(iglobal->d0()) > 0.3) continue; // most basic IP cuts
    nG++;
    //    printf("G muon %d\n", nG);

    coll_.GTLV.push_back( TLorentzVector(iglobal->px(), iglobal->py(), iglobal->pz(), sqrt(iglobal->px()*iglobal->px() + iglobal->py()*iglobal->py() + iglobal->pz()*iglobal->pz())) );
    coll_.GdZ.push_back(iglobal->dz());
    coll_.Gd0.push_back(iglobal->d0());
    double minDR=999.;
    reco::GenParticle* match(NULL);
    for(std::vector<reco::GenParticle>::const_iterator igen=genParticles->begin(); igen!=genParticles->end(); ++igen){ // now match this to gen particle
      if (igen->status()!=1) continue; // final state only
      double DR = std::abs(deltaR<reco::Track,reco::GenParticle>(*iglobal,*igen));
      if (DR<0.2&&abs(igen->pdgId())==13) {
  	match = (reco::GenParticle*)&(*igen);
	break;
      }
      //    printf("\tFinal state particle %d -- DR = %3.4f\n", igen->pdgId(), DR);
      if (DR<minDR) {
  	minDR=DR;
  	match = (reco::GenParticle*)&(*igen);
	//	printf("\tThis is the new match!\n");
	//  	if (DR<0.2&&abs(igen->pdgId())==13) break;
      }
    } // gen loop
    coll_.GGenMatchDR.push_back(minDR);
    if (minDR<0.2) {
      coll_.GGenMatch.push_back(true);
      coll_.GGenMatchID.push_back(match->pdgId());
      coll_.GGenMatchMotherID.push_back(GetMotherID(match));
    } else{
      coll_.GGenMatch.push_back(false);
      coll_.GGenMatchID.push_back(0);
      coll_.GGenMatchMotherID.push_back(0);
    }
  } // global muon loop

}

int CSCDataFormats::CSCMuon::GetMotherID( const reco::Candidate * p ){
  const reco::Candidate* mother = p->mother();
  if( mother ){
    if( mother->pdgId() == p->pdgId() ){
      return GetMotherID(mother);
    }else{
      return mother->pdgId();
    }
  }else{
    return 0;
  }
}
