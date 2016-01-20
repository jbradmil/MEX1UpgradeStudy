#ifndef __CSCDATAFORMATS_STANDALONEMUON_H__
#define __CSCDATAFORMATS_STANDALONEMUON_H__

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include <DataFormats/TrackReco/interface/Track.h> 

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
    StandAloneMuonDataFormat* getData() {return &sta_;}
    void Reset() {sta_.Reset();}

  private:
    int numberOSSF(edm::Handle<std::vector<reco::Track>> muons);

    CSCDataFormats::StandAloneMuonDataFormat sta_;

  };
}


#endif
