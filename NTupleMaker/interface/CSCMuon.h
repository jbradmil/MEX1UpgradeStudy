#ifndef __CSCDATAFORMATS_CSCMUON_H__
#define __CSCDATAFORMATS_CSCMUON_H__

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include <DataFormats/TrackReco/interface/Track.h> 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "CSCDataFormat.h"
#include "FWCore/Framework/interface/ConsumesCollector.h" 
#include <TMath.h>
#include "TVector3.h"

namespace CSCDataFormats
{
  class CSCMuon
  {

  public:
    CSCMuon();
    ~CSCMuon();

    void Set(const edm::Event& iEvent, bool isData, edm::Handle<edm::View<reco::Muon> > muons, edm::Handle<edm::TriggerResults> triggerBits, edm::Handle<pat::TriggerObjectStandAloneCollection> triggerObjects, edm::Handle<reco::VertexCollection> pvs);
    CSCDataFormat* getCollections() {return &coll_;}
    void Reset() {
      coll_.Reset();
    }

  private:
    //    int GetMotherID(const reco::Candidate *particle);
    bool isGoodGlobal(const reco::Muon *recoMu);
    bool isMedium(const reco::Muon *recoMu);
    bool isMediumPlus(const reco::Muon * recoMu, const reco::Vertex & pv);
    bool isTight(const reco::Muon *recoMu, const reco::Vertex & pv);
    double GetPUWeight(const int NPV);


    CSCDataFormats::CSCDataFormat coll_;
	
  };
}


#endif
