#ifndef __CSCDATAFORMATS_CSCMUON_H__
#define __CSCDATAFORMATS_CSCMUON_H__

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include <DataFormats/TrackReco/interface/Track.h> 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "CSCDataFormat.h"
#include <TMath.h>

namespace CSCDataFormats
{
  class CSCMuon
  {

  public:
    CSCMuon();
    ~CSCMuon();

    void Set(const edm::Event& e, const edm::InputTag& muons_, const edm::InputTag& pvs_, const edm::InputTag& genParticles_);
    CSCDataFormat* getCollections() {return &coll_;}
    void Reset() {
      coll_.Reset();
    }

  private:
    int GetMotherID(const reco::Candidate *particle);
    bool isGoodGlobal(const reco::Muon *recoMu);
    bool isMedium(const reco::Muon *recoMu);
    bool isMediumPlus(const reco::Muon * recoMu, const reco::Vertex & pv);
    bool isTight(const reco::Muon *recoMu, const reco::Vertex & pv);

    CSCDataFormats::CSCDataFormat coll_;

  };
}


#endif
