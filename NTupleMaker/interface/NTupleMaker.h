#ifndef __CSCPOSTLS2RATESTUDIES_NTUPLEMAKER_H__
#define __CSCPOSTLS2RATESTUDIES_NTUPLEMAKER_H__

#include <memory>
#include <string>

#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "MEX1UpgradeStudy/NTupleMaker/interface/CSCMuon.h"
#include "MEX1UpgradeStudy/NTupleMaker/interface/CSCDataFormat.h"

class TFile;
class TTree;


class NTupleMaker : public edm::EDAnalyzer {

  public:

   // constructor
      explicit NTupleMaker(const edm::ParameterSet&);
      virtual ~NTupleMaker();

   // fill tree
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void beginJob(void);
      virtual void endJob();

  private:

      edm::Service<TFileService> tfs_;
      TTree* tree_;

   // pat muons
      edm::InputTag muons_;
      edm::InputTag pvs_;
      edm::InputTag genParticles_;
      CSCDataFormats::CSCMuon* cscMuon;
      CSCDataFormats::CSCDataFormat* csc_data; 

};

#endif
