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

       /* muStandAloneZ.clear(); */
       /* muStandAloneZmumu.clear(); */

       muGenPt.clear();
       muGenP.clear();
       muGenEta.clear();
       muGenPhi.clear();

       muGenRecoMatch.clear();

       
       muDeltaPt.clear();
       muResolution.clear();
    }

    int muonSize;
    
    // standalone muon properties
    std::vector<float> muStandAlonePt;
    std::vector<float> muStandAloneP;
    std::vector<float> muStandAloneEta;
    std::vector<float> muStandAlonePhi;

    std::vector<float> muGenPt;
    std::vector<float> muGenP;
    std::vector<float> muGenEta;
    std::vector<float> muGenPhi;

    std::vector<bool> muGenRecoMatch;

    std::vector<float> muDeltaPt;
    std::vector<float> muResolution;


  };
}


#endif

