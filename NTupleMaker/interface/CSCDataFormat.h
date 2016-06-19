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

      mc_pu_weight=0;

      HLT_IsoMu27_v2 =false;
      HLT_IsoTkMu27_v2=false;
      HLT_IsoTkMu24_eta2p1_v2=false;
      HLT_IsoMu17_eta2p1_v2=false;

      hlt_pat_d3=-10.;

      mumuMass=0.;
   
      mu1TLV.SetXYZT(0,0,0,0);
      mu1Global=false;
      mu1Loose=false;
      mu1Medium=false;
      mu1Tight=false;
      mu1DZ=-1000.;
      mu1DXY=-1000.;

      mu1_validTrkHitFraction=0;
      mu1_normalizedGlbTrkChi2=-10;
      mu1_trkSAchi2LocalPosition=-10;
      mu1_trkKink=100;
      mu1_segmentCompatibility=0;
      mu1_ptRelError=0;
      mu1_nValidMuonHits=0;
      mu1_nMatchedStations=0;
      mu1_nPixelHits=0;
      mu1_nTrkLayersWithMeasurement=0;

      mu2TLV.SetXYZT(0,0,0,0);
      mu2Global=false;
      mu2Loose=false;
      mu2Medium=false;
      mu2Tight=false;
      mu2DZ=-1000.;
      mu2DXY=-1000.;

      mu2_validTrkHitFraction=0;
      mu2_normalizedGlbTrkChi2=-10;
      mu2_trkSAchi2LocalPosition=-10;
      mu2_trkKink=100;
      mu2_segmentCompatibility=0;
      mu2_ptRelError=0;
      mu2_nValidMuonHits=0;
      mu2_nMatchedStations=0;
      mu2_nPixelHits=0;
      mu2_nTrkLayersWithMeasurement=0;


       
    }

    int NPV;
    double mc_pu_weight;

    bool HLT_IsoMu27_v2;
    bool HLT_IsoTkMu27_v2;
    bool HLT_IsoTkMu24_eta2p1_v2;
    bool HLT_IsoMu17_eta2p1_v2;

    double hlt_pat_d3;
    
    double mumuMass;


    TLorentzVector mu1TLV;
    bool mu1Global;
    bool mu1Loose;
    bool mu1Medium;
    bool mu1Tight;
    double mu1DZ;
    double mu1DXY;

    double mu1_validTrkHitFraction; 
    double mu1_normalizedGlbTrkChi2; 
    double mu1_trkSAchi2LocalPosition; 
    double mu1_trkKink; 
    double mu1_segmentCompatibility; 
    double mu1_ptRelError; 
    int mu1_nValidMuonHits; 
    int mu1_nMatchedStations; 
    int mu1_nPixelHits; 
    int mu1_nTrkLayersWithMeasurement; 
   
    TLorentzVector mu2TLV;
    bool mu2Global;
    bool mu2Loose;
    bool mu2Medium;
    bool mu2Tight;
    double mu2DZ;
    double mu2DXY;
    

    double mu2_validTrkHitFraction; 
    double mu2_normalizedGlbTrkChi2; 
    double mu2_trkSAchi2LocalPosition; 
    double mu2_trkKink; 
    double mu2_segmentCompatibility; 
    double mu2_ptRelError; 
    int mu2_nValidMuonHits; 
    int mu2_nMatchedStations; 
    int mu2_nPixelHits; 
    int mu2_nTrkLayersWithMeasurement; 
 

  };
}


#endif

