#include "MEX1UpgradeStudy/NTupleMaker/interface/StandAloneMuon.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include <DataFormats/TrackReco/interface/Track.h>
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <cmath>
#include "TLorentzVector.h"

CSCDataFormats::StandAloneMuon::StandAloneMuon() 
{
}

CSCDataFormats::StandAloneMuon::~StandAloneMuon() 
{
}

void CSCDataFormats::StandAloneMuon::Set(const edm::Event& e, const edm::InputTag& muons_, const edm::InputTag& genParticles_) 
{
  edm::Handle<std::vector<reco::Track>> muons;
  e.getByLabel(muons_,muons);

  edm::Handle<std::vector<reco::GenParticle>> genParticles;
  e.getByLabel(genParticles_,genParticles);

  gen_.muonSize = 0;
  for(std::vector<reco::GenParticle>::const_iterator gen1=genParticles->begin(); gen1!=genParticles->end(); ++gen1){
    if (std::abs(gen1->pdgId())==13 && gen1->status()==1) { // found a gen muon
      gen_.muonSize++;
      gen_.GenPt.push_back(gen1->pt());
      gen_.GenP.push_back(gen1->p());
      gen_.GenEta.push_back(gen1->eta());
      gen_.GenPhi.push_back(gen1->phi());
      gen_.GenID.push_back(gen1->pdgId());
      gen_.GenMotherID.push_back(GetMotherID(&(*gen1)));
      // gen_.GenStatus.push_back(gen1->status());
      //      printf("\nGen muon %d (pt, eta, phi): (%3.2f, %3.2f, %3.2f)\n", gen_.muonSize, gen1->pt(), gen1->eta(), gen1->phi());
      bool foundRECO=false;
      for(std::vector<reco::Track>::const_iterator mu1=muons->begin(); mu1!=muons->end(); ++mu1){ // was it reconstructed?
	//	printf("Reco muon (pt, eta, phi): (%3.2f, %3.2f, %3.2f)\n", mu1->pt(), mu1->eta(), mu1->phi());	
	if (std::abs(deltaR<reco::Track,reco::GenParticle>(*mu1,*gen1))<0.2) { // found a match
	  gen_.GenRecoMatch.push_back(true);
	  gen_.muStandAlonePt.push_back(mu1->pt());
	  gen_.muStandAloneP.push_back(mu1->p());
	  gen_.muStandAloneEta.push_back(mu1->eta());
	  gen_.muStandAlonePhi.push_back(mu1->phi());
	  //gen_.muStandAloneNStations.push_back(mu1->numberOfMatchedStations());
	  gen_.muDeltaPt.push_back(mu1->pt()-gen1->pt());
	  gen_.muResolution.push_back((mu1->pt()-gen1->pt())/gen1->pt());
	  //	  printf("This is a MATCH!\n");
	  foundRECO=true;
	  break;
	}
      } // loop over standalone muons
      if (!foundRECO) {
	gen_.GenRecoMatch.push_back(false);
	gen_.muStandAlonePt.push_back(-999.);
	gen_.muStandAloneP.push_back(-999.);
	gen_.muStandAloneEta.push_back(-999.);
	gen_.muStandAlonePhi.push_back(-999.);
	gen_.muDeltaPt.push_back(-999.);
	gen_.muResolution.push_back(-999.);
	//gen_.muStandAloneNStations.push_back(-9);
	//	printf("Could not find a match\n");
      }
    } // gen muons
  } // gen particles loop
   
  // standalone loop -- under development
  // sta_.muonSize = 0;
  // for(std::vector<reco::Track>::const_iterator mu1=muons->begin(); mu1!=muons->end(); ++mu1){ // was it reconstructed?
  //   sta_.muonSize++;
  //   sta_.muStandAlonePt.push_back(mu1->pt());
  //   sta_.muStandAloneP.push_back(mu1->p());
  //   sta_.muStandAloneEta.push_back(mu1->eta());
  //   sta_.muStandAlonePhi.push_back(mu1->phi());
  //   double minDR=999.;
  //   reco::GenParticle* match(NULL);
  //   for(std::vector<reco::GenParticle>::const_iterator gen1=genParticles->begin(); gen1!=genParticles->end(); ++gen1){ // now match this to gen particle
  //     if (gen1->status()!=1) continue; // final state only
  //     double DR = std::abs(deltaR<reco::Track,reco::GenParticle>(*mu1,*gen1));
  //     if (DR<minDR) {
  // 	minDR=DR;
  // 	match = (reco::GenParticle*)&(*gen1);
  // 	if (minDR<0.2&&abs(gen1->pdgId())==13) break;
  //     }
  //   } // gen loop
  //   if (minDR<0.2) {
  //     sta_.GenRecoMatch.push_back(true);
  //     sta_.GenPt.push_back(match->pt());
  //     sta_.GenP.push_back(match->p());
  //     sta_.GenEta.push_back(match->eta());
  //     sta_.GenPhi.push_back(match->phi());
  //     sta_.GenID.push_back(match->pdgId());
  //     sta_.GenMotherID.push_back(GetMotherID(match));
  //     sta_.muDeltaPt.push_back(mu1->pt()-match->pt());
  //     sta_.muResolution.push_back((mu1->pt()-match->pt())/match->pt());
  //   } else{
  //     sta_.GenRecoMatch.push_back(false);
  //     sta_.GenPt.push_back(-999.);
  //     sta_.GenP.push_back(-999.);
  //     sta_.GenEta.push_back(-999.);
  //     sta_.GenPhi.push_back(-999.);
  //     sta_.GenID.push_back(0);
  //     sta_.GenMotherID.push_back(0);
  //     sta_.muDeltaPt.push_back(-999.);
  //     sta_.muResolution.push_back(-999.);
  //   }
  // } // standalone loop

}

int CSCDataFormats::StandAloneMuon::numberOSSF(edm::Handle<std::vector<reco::Track>> muons)
{
  int numPlus = 0;
  int numMinus = 0;
  for(std::vector<reco::Track>::const_iterator mu1=muons->begin(); mu1!=muons->end(); ++mu1){
    if (mu1->charge()>0) {
      numPlus++;
    }
    else {
      numMinus++;
    }
  }
  return std::min(numPlus,numMinus);
}

int CSCDataFormats::StandAloneMuon::GetMotherID( const reco::Candidate * p ){
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
