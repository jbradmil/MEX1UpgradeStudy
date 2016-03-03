#ifndef __CSCDATAFORMATS_STANDALONEMUONDATAFORMAT_H__
#define __CSCDATAFORMATS_STANDALONEMUONDATAFORMAT_H__

#include <TROOT.h>
#include <vector>
#include <iostream>

namespace CSCDataFormats
{
  struct StandAloneMuonDataFormat
  {
    StandAloneMuonDataFormat() {Reset();}
    ~StandAloneMuonDataFormat() {}
 
    void Reset()
    {
       muonSize = 0;

       muStandAlonePt.clear();
       muStandAloneP.clear();
       muStandAloneEta.clear();
       muStandAlonePhi.clear();

       muStandAloneNStations.clear();
       /* muStandAloneZ.clear(); */
       /* muStandAloneZmumu.clear(); */

       GenPt.clear();
       GenP.clear();
       GenEta.clear();
       GenPhi.clear();

       GenRecoMatch.clear();
       GenID.clear();
       GenMotherID.clear();
       //       GenStatus.clear();

       
       muDeltaPt.clear();
       muResolution.clear();
    }

    int muonSize;
    
    // standalone muon properties
    std::vector<float> muStandAlonePt;
    std::vector<float> muStandAloneP;
    std::vector<float> muStandAloneEta;
    std::vector<float> muStandAlonePhi;

    std::vector<int> muStandAloneNStations;

    std::vector<float> GenPt;
    std::vector<float> GenP;
    std::vector<float> GenEta;
    std::vector<float> GenPhi;

    std::vector<bool> GenRecoMatch;
    std::vector<int> GenID;
    std::vector<int> GenMotherID;
    //    std::vector<int> GenStatus;

    std::vector<float> muDeltaPt;
    std::vector<float> muResolution;


  };
}


#endif

