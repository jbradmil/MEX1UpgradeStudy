#ifndef __CSCDATAFORMATS_CSCDATAFORMAT_H__
#define __CSCDATAFORMATS_CSCDATAFORMAT_H__

#include <TROOT.h>
#include <vector>
#include <iostream>
#include "TLorentzVector.h"

namespace CSCDataFormats
{
  struct CSCDataFormat
  {
    CSCDataFormat() {Reset();}
    ~CSCDataFormat() {}
 
    void Reset()
    {

      NPV=0;

      GenTLV.clear();
      GenID.clear();
      GenMotherID.clear();
      
      GenRECOMatch.clear();
      GRMTLV.clear();
      GRMDZ.clear();
      GRMDXY.clear();
      GRMNStations.clear();
      GRDeltaPt.clear();
      GRResolution.clear();

      GRMisGlobal.clear();
      GRMisGoodGlobal.clear();
      GRMisTracker.clear();
      GRMisMedium.clear();
      GRMisMediumPlus.clear();
      GRMisTight.clear();

      GRMvalidTrkHitFraction.clear();
      GRMnormalizedGlbTrkChi2.clear();
      GRMtrkSAchi2LocalPosition.clear();
      GRMtrkKink.clear();
      GRMsegmentCompatibility.clear();
      GRMptRelError.clear();
      GRMnValidMuonHits.clear();
      GRMnMatchedStations.clear();
      GRMnPixelHits.clear();
      GRMnTrkLayersWithMeasurement.clear();

      TLV.clear();
      dZ.clear();
      dXY.clear();
      
      isGlobal.clear();
      isGoodGlobal.clear();
      isTracker.clear();
      isMedium.clear();
      isMediumPlus.clear();
      isTight.clear();

      validTrkHitFraction.clear();
      normalizedGlbTrkChi2.clear();
      trkSAchi2LocalPosition.clear();
      trkKink.clear();
      segmentCompatibility.clear();
      ptRelError.clear();
      nValidMuonHits.clear();
      nMatchedStations.clear();
      nPixelHits.clear();
      nTrkLayersWithMeasurement.clear();


      
      RECOGenMatch.clear();
      RECOGenMatchDR.clear();
      RECOGenMatchID.clear();
      RECOGenMatchMotherID.clear();
       
    }

    int NPV;

    std::vector<TLorentzVector> GenTLV;
    std::vector<int> GenID;
    std::vector<int> GenMotherID;

    // properties of the matched reconstructed muon, for convenience
    std::vector<bool> GenRECOMatch;
    std::vector<TLorentzVector> GRMTLV;
    std::vector<float> GRMDZ;
    std::vector<float> GRMDXY;
    std::vector<int> GRMNStations;
    std::vector<float> GRDeltaPt;
    std::vector<float> GRResolution;

    std::vector<bool> GRMisGlobal;
    std::vector<bool> GRMisGoodGlobal;
    std::vector<bool> GRMisTracker;
    std::vector<bool> GRMisMedium;
    std::vector<bool> GRMisMediumPlus;
    std::vector<bool> GRMisTight;

    std::vector<float> GRMvalidTrkHitFraction;
    std::vector<float> GRMnormalizedGlbTrkChi2;
    std::vector<float> GRMtrkSAchi2LocalPosition;
    std::vector<float> GRMtrkKink;
    std::vector<float> GRMsegmentCompatibility;
    std::vector<float> GRMptRelError;
    std::vector<int> GRMnValidMuonHits;
    std::vector<int> GRMnMatchedStations;
    std::vector<int> GRMnPixelHits;
    std::vector<int> GRMnTrkLayersWithMeasurement;
 

    std::vector<TLorentzVector> TLV;
    std::vector<float> dZ;
    std::vector<float> dXY;

    std::vector<bool> isGlobal;
    std::vector<bool> isGoodGlobal;
    std::vector<bool> isTracker;
    std::vector<bool> isMedium;
    std::vector<bool> isMediumPlus;
    std::vector<bool> isTight;

    std::vector<float> validTrkHitFraction;
    std::vector<float> normalizedGlbTrkChi2;
    std::vector<float> trkSAchi2LocalPosition;
    std::vector<float> trkKink;
    std::vector<float> segmentCompatibility;
    std::vector<float> ptRelError;
    std::vector<int> nValidMuonHits;
    std::vector<int> nMatchedStations;
    std::vector<int> nPixelHits;
    std::vector<int> nTrkLayersWithMeasurement;
 

    
    std::vector<bool> RECOGenMatch;
    std::vector<float> RECOGenMatchDR;
    std::vector<int> RECOGenMatchID;
    std::vector<int> RECOGenMatchMotherID;


  };
}


#endif

