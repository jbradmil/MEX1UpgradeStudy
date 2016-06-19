#include "MEX1UpgradeStudy/NTupleMaker/interface/CSCMuon.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include <DataFormats/TrackReco/interface/Track.h>
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <cmath>
#include "TLorentzVector.h"

using std::cout;
using std::endl;

CSCDataFormats::CSCMuon::CSCMuon() 
{

  
}

CSCDataFormats::CSCMuon::~CSCMuon() 
{
}

void CSCDataFormats::CSCMuon::Set(const edm::Event& iEvent, bool isData, edm::Handle<edm::View<reco::Muon> > muons, edm::Handle<edm::TriggerResults> triggerBits, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, edm::Handle<reco::VertexCollection> pvs) 
{
 

  if (pvs->empty()) return; // skip the event if no PV found
  const reco::Vertex &PV = pvs->front();
  coll_.NPV = pvs->size();

  coll_.mc_pu_weight = GetPUWeight(coll_.NPV);

  TVector3 hlt_muon(0,0,0);
  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
  //  std::cout << "\n === TRIGGER PATHS === " << std::endl;
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
    if (!triggerBits->accept(i)) continue;
    if ( names.triggerName(i) == "HLT_IsoMu27_v2") {
      coll_.HLT_IsoMu27_v2=true;
      //      std::cout << "Trigger " << names.triggerName(i) << 
      //	//      ", prescale " << triggerPrescales->getPrescaleForIndex(i) <<
      //	": " << (triggerBits->accept(i) ? "PASS" : "fail (or not run)") 
      //		<< std::endl;
    }
    if ( names.triggerName(i) == "HLT_IsoTkMu27_v2") coll_.HLT_IsoTkMu27_v2=true;
    if ( names.triggerName(i) == "HLT_IsoTkMu24_eta2p1_v2") coll_.HLT_IsoTkMu24_eta2p1_v2=true;
    if ( names.triggerName(i) == "HLT_IsoMu17_eta2p1_v2") coll_.HLT_IsoMu17_eta2p1_v2=true;
  }

  if (!coll_.HLT_IsoMu27_v2) return; // JACK

  
  for (pat::TriggerObjectStandAlone obj : *triggerObjects) { // note: not "const &" since we want to call unpackPathNames
    obj.unpackPathNames(names);
    TString name(obj.collection());
    if(name!="hltL3MuonCandidates::HLT") continue;
    //    std::cout << "\tTrigger object:  px " << obj.px() << ", py " << obj.py() << ", pz " << obj.pz() << std::endl;
    hlt_muon.SetXYZ(obj.px(), obj.py(), obj.pz());
    break;
  }



 TVector3 pat_muon(0,0,0);
 int hlt_match(-1);
 double minD3(999.);
 for (int ipat=0; ipat<(int)muons->size();ipat++) {
   if ( muons->at(ipat).pt() < 27 ) continue;
   pat_muon.SetXYZ(muons->at(ipat).px(), muons->at(ipat).py(), muons->at(ipat).pz());
   double D3 = (hlt_muon-pat_muon).Mag();
   if (D3<minD3) {
     minD3 = D3;
     hlt_match = ipat;
   }
 }
 if (hlt_match<0) {
   // cout << "Could not match pat muon to hlt muon" << endl;
   return;
 }
 // cout << "Matched pat muon " << hlt_match << ", (" << pat_muon.X() << ", " << pat_muon.Y() << ", " << pat_muon.Z() << ") to hlt muon, D3 = " << minD3 << endl;
 coll_.hlt_pat_d3 = minD3;
 TLorentzVector TLV1;
 TLV1.SetVectM(pat_muon, 0.);
 coll_.mu1TLV = TLV1;
 coll_.mu1Global=muons->at(hlt_match).isGlobalMuon();
 // coll_.mu1Loose=muons->at(hlt_match).isLooseMuon();
 coll_.mu1Medium=isMediumPlus(&muons->at(hlt_match), PV);
 coll_.mu1Tight=isTight(&muons->at(hlt_match), PV);
 coll_.mu1DZ=muons->at(hlt_match).muonBestTrack()->dz(PV.position());
 coll_.mu1DXY=muons->at(hlt_match).muonBestTrack()->dxy(PV.position());
 if (muons->at(hlt_match).isGlobalMuon() || muons->at(hlt_match).isTrackerMuon()) { 
   coll_.mu1_validTrkHitFraction = muons->at(hlt_match).innerTrack()->validFraction();
   if (muons->at(hlt_match).isGlobalMuon()) coll_.mu1_normalizedGlbTrkChi2 = muons->at(hlt_match).globalTrack()->normalizedChi2();
   else coll_.mu1_normalizedGlbTrkChi2 = -10;
   coll_.mu1_trkSAchi2LocalPosition = muons->at(hlt_match).combinedQuality().chi2LocalPosition;
   coll_.mu1_trkKink = muons->at(hlt_match).combinedQuality().trkKink;
   coll_.mu1_ptRelError = muons->at(hlt_match).muonBestTrack()->ptError()/muons->at(hlt_match).muonBestTrack()->pt();
   coll_.mu1_segmentCompatibility = muon::segmentCompatibility(muons->at(hlt_match));
   if (muons->at(hlt_match).isGlobalMuon()) coll_.mu1_nValidMuonHits = muons->at(hlt_match).globalTrack()->hitPattern().numberOfValidMuonHits();
   else coll_.mu1_nValidMuonHits = 0;
   coll_.mu1_nMatchedStations = muons->at(hlt_match).numberOfMatchedStations();
   coll_.mu1_nPixelHits = muons->at(hlt_match).innerTrack()->hitPattern().numberOfValidPixelHits();
   coll_.mu1_nTrkLayersWithMeasurement = muons->at(hlt_match).innerTrack()->hitPattern().trackerLayersWithMeasurement() ;
 }
  
 // now find another muon
 TLorentzVector TLV2;
 for (int ipat=0; ipat<(int)muons->size();ipat++) {
   if (ipat==hlt_match) continue;
   if (muons->at(ipat).muonBestTrack()->dz(PV.position())>20 || muons->at(ipat).muonBestTrack()->dxy(PV.position())>0.3) continue;
   if (muons->at(ipat).charge() == muons->at(hlt_match).charge()) continue; // must be OS
   // printf("2nd muon with pt %4.1f, dz(PV) %+5.3f, POG loose id %d, tight id %d\n",
   // 	   muons->at(ipat).pt(), muons->at(ipat).muonBestTrack()->dz(PV.position()), muons->at(ipat).isLooseMuon(), muons->at(ipat).isTightMuon(PV));
   TLV2.SetPtEtaPhiM(muons->at(ipat).pt(), muons->at(ipat).eta(), muons->at(ipat).phi(), 0.);
   coll_.mu2Global=muons->at(ipat).isGlobalMuon();
   //coll_.mu2Loose=muons->at(ipat).isLooseMuon();
   coll_.mu2Medium=isMediumPlus(&muons->at(ipat), PV);
   coll_.mu2Tight=isTight(&muons->at(ipat), PV);
   coll_.mu2DZ=muons->at(ipat).muonBestTrack()->dz(PV.position());
   coll_.mu2DXY=muons->at(ipat).muonBestTrack()->dxy(PV.position());
   if (muons->at(ipat).isGlobalMuon() || muons->at(ipat).isTrackerMuon()) { 
     coll_.mu2_validTrkHitFraction = muons->at(ipat).innerTrack()->validFraction();
     if (muons->at(ipat).isGlobalMuon()) coll_.mu2_normalizedGlbTrkChi2 = muons->at(ipat).globalTrack()->normalizedChi2();
     else coll_.mu2_normalizedGlbTrkChi2 = -10;
     coll_.mu2_trkSAchi2LocalPosition = muons->at(ipat).combinedQuality().chi2LocalPosition;
     coll_.mu2_trkKink = muons->at(ipat).combinedQuality().trkKink;
     coll_.mu2_ptRelError = muons->at(ipat).muonBestTrack()->ptError()/muons->at(ipat).muonBestTrack()->pt();
     coll_.mu2_segmentCompatibility = muon::segmentCompatibility(muons->at(ipat));
     if (muons->at(ipat).isGlobalMuon()) coll_.mu2_nValidMuonHits = muons->at(ipat).globalTrack()->hitPattern().numberOfValidMuonHits();
     else coll_.mu2_nValidMuonHits = 0;
     coll_.mu2_nMatchedStations = muons->at(ipat).numberOfMatchedStations();
     coll_.mu2_nPixelHits = muons->at(ipat).innerTrack()->hitPattern().numberOfValidPixelHits();
     coll_.mu2_nTrkLayersWithMeasurement = muons->at(ipat).innerTrack()->hitPattern().trackerLayersWithMeasurement() ;
   }
   break;
 }

 coll_.mu2TLV = TLV2;
 coll_.mumuMass = (coll_.mu1TLV + coll_.mu2TLV).M();

}

// int CSCDataFormats::CSCMuon::GetMotherID( const reco::Candidate * p ){
//   const reco::Candidate* mother = p->mother();
//   if( mother ){
//     if( mother->pdgId() == p->pdgId() ){
//       return GetMotherID(mother);
//     }else{
//       return mother->pdgId();
//     }
//   }else{
//     return 0;
//   }
// }

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

double CSCDataFormats::CSCMuon::GetPUWeight( const int NPV ){
  
  if (NPV<10) return 17.3305;
  else if (NPV<20) return 2.52133;
  else if (NPV<30) return 0.0604228;
  else return 0.000279073;

}
