#ifndef __CSCDATAFORMATS_STANDALONEMUON_H__
#define __CSCDATAFORMATS_STANDALONEMUON_H__

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include <DataFormats/TrackReco/interface/Track.h> 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "StandAloneMuonDataFormat.h"
#include <TMath.h>

namespace CSCDataFormats
{
  class StandAloneMuon
  {

  public:
    StandAloneMuon();
    ~StandAloneMuon();

    void Set(const edm::Event& e, const edm::InputTag& muons_, const edm::InputTag& genParticles_);
    // StandAloneMuonDataFormat* getStandAloneCollection() {return &sta_;}
    StandAloneMuonDataFormat* getGenCollection() {return &gen_;}
    void Reset() {
      // sta_.Reset();
      gen_.Reset();
    }

  private:
    int numberOSSF(edm::Handle<std::vector<reco::Track>> muons);
    int GetMotherID(const reco::Candidate *particle);

    // CSCDataFormats::StandAloneMuonDataFormat sta_;
    CSCDataFormats::StandAloneMuonDataFormat gen_;

  };
}


#endif
