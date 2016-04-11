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

void CSCDataFormats::CSCMuon::Set(const edm::Event& e, const edm::InputTag& muons_, const edm::InputTag& pvs_, const edm::InputTag& genParticles_) 
{
  edm::Handle<std::vector<reco::Muon>> muons;
  e.getByLabel(muons_,muons);

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
    // bool foundRECO=false;
    for(std::vector<reco::Muon>::const_iterator ireco=muons->begin(); ireco!=muons->end(); ++ireco){ // was it reconstructed as SA?
      if (std::abs(deltaR<reco::Muon,reco::GenParticle>(*ireco,*igen))<0.2) { // found a match -- note: currently not filling unless there's a match
	coll_.GenRECOMatch.push_back(true);
	// note GRM = GenRecoMatch
	coll_.GRMTLV.push_back( TLorentzVector(ireco->px(), ireco->py(), ireco->pz(), sqrt(ireco->px()*ireco->px() + ireco->py()*ireco->py() + ireco->pz()*ireco->pz())) );
	coll_.GRMDZ.push_back(ireco->muonBestTrack()->dz(pvs->at(0).position()));
	coll_.GRMDXY.push_back(ireco->muonBestTrack()->dxy(pvs->at(0).position()));
	coll_.GRDeltaPt.push_back(ireco->pt()-igen->pt());
	coll_.GRResolution.push_back(1/ireco->pt()-1/igen->pt());
	// id and muon type
	coll_.GRMisGlobal.push_back(ireco->isGlobalMuon());
	coll_.GRMisGoodGlobal.push_back(isGoodGlobal(&(*ireco)));
	coll_.GRMisTracker.push_back(ireco->isTrackerMuon());
	coll_.GRMisMedium.push_back(isMedium(&(*ireco)));
	coll_.GRMisMediumPlus.push_back(isMediumPlus(&(*ireco), pvs->at(0)));
	coll_.GRMisTight.push_back(isTight(&(*ireco), pvs->at(0)));
	// ID variables
	const reco::Muon * recoMu = &(*ireco);
	if (recoMu->isGlobalMuon() || recoMu->isTrackerMuon()) { 
	  coll_.GRMvalidTrkHitFraction.push_back(recoMu->innerTrack()->validFraction());
	  if (recoMu->isGlobalMuon()) coll_.GRMnormalizedGlbTrkChi2.push_back(recoMu->globalTrack()->normalizedChi2());
	  else coll_.GRMnormalizedGlbTrkChi2.push_back(-10);
	  coll_.GRMtrkSAchi2LocalPosition.push_back(recoMu->combinedQuality().chi2LocalPosition);
	  coll_.GRMtrkKink.push_back(recoMu->combinedQuality().trkKink);
	  coll_.GRMptRelError.push_back(recoMu->muonBestTrack()->ptError()/recoMu->muonBestTrack()->pt());
	  coll_.GRMsegmentCompatibility.push_back(muon::segmentCompatibility(*recoMu));
	  if (recoMu->isGlobalMuon())coll_.GRMnValidMuonHits.push_back(recoMu->globalTrack()->hitPattern().numberOfValidMuonHits());
	  else coll_.GRMnValidMuonHits.push_back(0);
	  coll_.GRMnMatchedStations.push_back(recoMu->numberOfMatchedStations());
	  coll_.GRMnPixelHits.push_back(recoMu->innerTrack()->hitPattern().numberOfValidPixelHits());
	  coll_.GRMnTrkLayersWithMeasurement.push_back(recoMu->innerTrack()->hitPattern().trackerLayersWithMeasurement() );
	} else {
	  coll_.GRMvalidTrkHitFraction.push_back(0);
	  coll_.GRMnormalizedGlbTrkChi2.push_back(-10);
	  coll_.GRMtrkSAchi2LocalPosition.push_back(-10);
	  coll_.GRMtrkKink.push_back(-10);
	  coll_.GRMsegmentCompatibility.push_back(-10);
	  coll_.GRMptRelError.push_back(-10);
	  coll_.GRMnValidMuonHits.push_back(0);
	  coll_.GRMnMatchedStations.push_back(0);
	  coll_.GRMnPixelHits.push_back(0);
	  coll_.GRMnTrkLayersWithMeasurement.push_back(0);
	}
	//foundRECO=true;
	break;
      } // standalone match
    }// standalone muon loop
    // if (!foundRECO) {
    //   coll_.GenRECOMatch.push_back(false);
    //   coll_.GenRECOMatchTLV.push_back( TLorentzVector(0,0,0,0) );
    //   coll_.GenRECOMatchDZ.push_back(-9.);
    //   coll_.GenRECOMatchDXY.push_back(-9.);
    //   coll_.GenRECODeltaPt.push_back(-999.);
    //   coll_.GenRECOResolution.push_back(-999.);
    // }
  } // gen particles loop
   
    // standalone loop -- under development
  int nRECO(0);
  for(std::vector<reco::Muon>::const_iterator ireco=muons->begin(); ireco!=muons->end(); ++ireco){
    // if (fabs(ireco->dz()) > 20 || fabs(ireco->dXY()) > 0.3) continue; // most basic IP cuts
    nRECO++;
    //    printf("RECO muon %d\n", nRECO);

    coll_.TLV.push_back( TLorentzVector(ireco->px(), ireco->py(), ireco->pz(), sqrt(ireco->px()*ireco->px() + ireco->py()*ireco->py() + ireco->pz()*ireco->pz())) );
    // vtx cuts
    coll_.dZ.push_back(ireco->muonBestTrack()->dz(pvs->at(0).position()));
    coll_.dXY.push_back(ireco->muonBestTrack()->dxy(pvs->at(0).position()));
    // id and muon type
    coll_.isGlobal.push_back(ireco->isGlobalMuon());
    coll_.isGoodGlobal.push_back(isGoodGlobal(&(*ireco)));
    coll_.isTracker.push_back(ireco->isTrackerMuon());
    coll_.isMedium.push_back(isMedium(&(*ireco)));
    coll_.isMediumPlus.push_back(isMediumPlus(&(*ireco), pvs->at(0)));
    coll_.isTight.push_back(isTight(&(*ireco), pvs->at(0)));
    // ID variables
    const reco::Muon * recoMu = &(*ireco);
    if (recoMu->isGlobalMuon() || recoMu->isTrackerMuon()) { 
      coll_.validTrkHitFraction.push_back(recoMu->innerTrack()->validFraction());
      if (recoMu->isGlobalMuon()) coll_.normalizedGlbTrkChi2.push_back(recoMu->globalTrack()->normalizedChi2());
      else coll_.normalizedGlbTrkChi2.push_back(-10);
      coll_.trkSAchi2LocalPosition.push_back(recoMu->combinedQuality().chi2LocalPosition);
      coll_.trkKink.push_back(recoMu->combinedQuality().trkKink);
      coll_.ptRelError.push_back(recoMu->muonBestTrack()->ptError()/recoMu->muonBestTrack()->pt());
      coll_.segmentCompatibility.push_back(muon::segmentCompatibility(*recoMu));
      if (recoMu->isGlobalMuon()) coll_.nValidMuonHits.push_back(recoMu->globalTrack()->hitPattern().numberOfValidMuonHits());
      else coll_.nValidMuonHits.push_back(0);
      coll_.nMatchedStations.push_back(recoMu->numberOfMatchedStations());
      coll_.nPixelHits.push_back(recoMu->innerTrack()->hitPattern().numberOfValidPixelHits());
      coll_.nTrkLayersWithMeasurement.push_back(recoMu->innerTrack()->hitPattern().trackerLayersWithMeasurement() );
    } else {
      coll_.validTrkHitFraction.push_back(0);
      coll_.normalizedGlbTrkChi2.push_back(-10);
      coll_.trkSAchi2LocalPosition.push_back(-10);
      coll_.trkKink.push_back(-10);
      coll_.segmentCompatibility.push_back(-10);
      coll_.ptRelError.push_back(-10);
      coll_.nValidMuonHits.push_back(0);
      coll_.nMatchedStations.push_back(0);
      coll_.nPixelHits.push_back(0);
      coll_.nTrkLayersWithMeasurement.push_back(0);
  }
    
    double minDR=999.;
    reco::GenParticle* match(NULL);
    for(std::vector<reco::GenParticle>::const_iterator igen=genParticles->begin(); igen!=genParticles->end(); ++igen){ // now match this to gen particle
      if (igen->status()!=1) continue; // final state only
      double DR = std::abs(deltaR<reco::Muon,reco::GenParticle>(*ireco,*igen));
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
    coll_.RECOGenMatchDR.push_back(minDR);
    if (minDR<0.2) {
      coll_.RECOGenMatch.push_back(true);
      coll_.RECOGenMatchID.push_back(match->pdgId());
      coll_.RECOGenMatchMotherID.push_back(GetMotherID(match));
    } else{
      coll_.RECOGenMatch.push_back(false);
      coll_.RECOGenMatchID.push_back(0);
      coll_.RECOGenMatchMotherID.push_back(0);
    }
  } // standalone loop

  
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

bool CSCDataFormats::CSCMuon::isGoodGlobal( const reco::Muon * recoMu ){

  if (recoMu->isGlobalMuon() && recoMu->globalTrack()->normalizedChi2() < 3 && recoMu->combinedQuality().chi2LocalPosition < 12 && recoMu->combinedQuality().trkKink < 20) return true;
  else return false;
  
}

bool CSCDataFormats::CSCMuon::isMedium( const reco::Muon * recoMu ){

  bool goodGlob = isGoodGlobal(recoMu);
  
  if ( (recoMu->isGlobalMuon() || recoMu->isTrackerMuon()) && recoMu->innerTrack()->validFraction() > 0.8 && muon::segmentCompatibility(*recoMu) > (goodGlob ? 0.303 : 0.451) ) return true;
  else return false;
  
}

bool CSCDataFormats::CSCMuon::isMediumPlus( const reco::Muon * recoMu, const reco::Vertex & pv ){
  
  if ( isMedium(recoMu) && fabs(recoMu->muonBestTrack()->dxy(pv.position())) < 0.2 && fabs(recoMu->muonBestTrack()->dz(pv.position()))< 0.5 ) return true;
  else return false;
  
}

bool CSCDataFormats::CSCMuon::isTight( const reco::Muon * recoMu, const reco::Vertex & pv ){
  
  if ( recoMu->isGlobalMuon()
       && fabs(recoMu->muonBestTrack()->dxy(pv.position())) < 0.2 && fabs(recoMu->muonBestTrack()->dz(pv.position()))< 0.5
       && recoMu->globalTrack()->normalizedChi2() < 10 && recoMu->globalTrack()->hitPattern().numberOfValidMuonHits() > 0
       && recoMu->numberOfMatchedStations() > 1 && recoMu->innerTrack()->hitPattern().numberOfValidPixelHits() > 0
       && recoMu->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5) return true;
  else return false;
  
}
