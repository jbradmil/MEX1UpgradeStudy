// -*- C++ -*-
//
// Package:    CFEBBufferOverloadProducer
// Class:      CFEBBufferOverloadProducer
// 
/**\class CFEBBufferOverloadProducer CFEBBufferOverloadProducer.cc CSCPostLS2RateStudies/CFEBBufferOverloadProducer/plugins/CFEBBufferOverloadProducer.cc

   Description: A producer to randomly remove CFEB digis in the inner muon rings.

   Implementation:
   [Notes on implementation]
*/
//
// Original Author:  Devin Taylor
//         Created:  Sun, 13 Jul 2014 10:55:48 GMT
// $Id$
//
//


// system include files
#include <memory>
#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <math.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandomEngine.h"

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/MuonDetId/interface/CSCDetId.h>

//
// class declaration
//

class CSCGeometry;
class CSCDetId;

class CFEBBufferOverloadProducer : public edm::EDProducer {
public:
  explicit CFEBBufferOverloadProducer(const edm::ParameterSet&);
  ~CFEBBufferOverloadProducer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  virtual void beginJob() override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
      
  bool checkOverload(int station, std::string type, CLHEP::HepRandomEngine& engine);

  // ----------member data ---------------------------
  edm::EDGetTokenT<CSCRecHit2DCollection> rh_token;
  bool doCFEBFailure_;
  bool doDDUFailure_;
  bool doUniformFailure_;
  //      double failureRate_;
  double failRateME21_;
  double failRateME31_;
  double failRateME41_;
  double latency_;
  double l1aRate_;
  bool failME11_;
  bool failME12_;
  bool failME13_;
  bool failME1a_;
  bool failME21_;
  bool failME22_;
  bool failME31_;
  bool failME32_;
  bool failME41_;
  bool failME42_;

  const CSCGeometry* geom_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//
CFEBBufferOverloadProducer::CFEBBufferOverloadProducer(const edm::ParameterSet& iConfig)
{
  doCFEBFailure_ = iConfig.getUntrackedParameter<bool>("doCFEBFailure",true);
  //doDDUFailure_ = iConfig.getUntrackedParameter<bool>("doDDUFailure",true);
  doUniformFailure_ = iConfig.getUntrackedParameter<bool>("doUniformFailure",true);
  //  failureRate_ = iConfig.getUntrackedParameter<double>("failureRate",0.1);
  failRateME21_ = iConfig.getUntrackedParameter<double>("failRateME21",0.0);
  failRateME31_ = iConfig.getUntrackedParameter<double>("failRateME31",0.0);
  failRateME41_ = iConfig.getUntrackedParameter<double>("failRateME41",0.0);
  //latency_ = iConfig.getUntrackedParameter<double>("latency",20.); // in microseconds
  //l1aRate_ = iConfig.getUntrackedParameter<double>("l1aRate",500.); // in kHz
  failME11_ = iConfig.getUntrackedParameter<bool>("failME11",true);
  failME12_ = iConfig.getUntrackedParameter<bool>("failME12",true);
  failME13_ = iConfig.getUntrackedParameter<bool>("failME13",true);
  failME1a_ = iConfig.getUntrackedParameter<bool>("failME1a",true);
  failME21_ = iConfig.getUntrackedParameter<bool>("failME21",true);
  failME22_ = iConfig.getUntrackedParameter<bool>("failME22",true);
  failME31_ = iConfig.getUntrackedParameter<bool>("failME31",true);
  failME32_ = iConfig.getUntrackedParameter<bool>("failME32",true);
  failME41_ = iConfig.getUntrackedParameter<bool>("failME41",true);
  failME42_ = iConfig.getUntrackedParameter<bool>("failME42",true);
  // consumes
  rh_token = consumes<CSCRecHit2DCollection>( iConfig.getParameter<edm::InputTag>("cscRecHitTag") );

  //register your products
  produces<CSCRecHit2DCollection>();

}


CFEBBufferOverloadProducer::~CFEBBufferOverloadProducer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
CFEBBufferOverloadProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  // Get the CSC Geometry :
  edm::ESHandle<CSCGeometry> cscGeom;
  iSetup.get<MuonGeometryRecord>().get(cscGeom);

  // get random number generator
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine& engine = rng->getEngine();

  // Get the RecHits collection :
  edm::Handle<CSCRecHit2DCollection> recHits;
  iEvent.getByToken( rh_token, recHits );

  // Create empty collection of rechits  
  std::auto_ptr<CSCRecHit2DCollection> oc( new CSCRecHit2DCollection );

  // Vector to store rechit within layer
  std::vector<CSCRecHit2D> hitsInLayer;
 
  //std::cout << "Selecting Overloaded Buffers" << std::endl;
  bool bufferOverloaded[2][4][4][36][5] = {{{{{0}}}}}; //endcap, station, ring, chamber, cfeb
  //bool currChamberOverload = false;
  //std::cout << "First 10 overloaded cfebs" << std::endl;
  //int num = 0;
  // iterate over each cfeb and decide if it had a buffer overload
  for (int endcap=1; endcap<3; endcap++) {                                                            // +/-
    for (int station=1; station<5; station++) {                                                       // ME(1,2,3,4)
      for (int ring=1; ring<5; ring++) {                                                              // ME1: ring 4 = a (1,2,3,a)
	//if (station>1 && ring>2) continue;                                                          // 2 rings in all but me1
	for (int chamber=1; chamber<37; chamber++) {
	  //if (station>1 && ring==1 && chamber>18) continue;                                        // inner rings ME2,3,4 18 chambers only
	  //if (doDDUFailure_) currChamberOverload = checkOverload(station,"DDU",engine);
	  for (int cfeb=1; cfeb<6; cfeb++) { 
	    //if ((station==1 && ring==1 && cfeb>4) || (station==1 && ring==4 && cfeb>3)) continue; // 4(3) dcfebs on me11(/a), 5 cfebs elsewhere
	    if (station>1 && ring==1) if (doCFEBFailure_) bufferOverloaded[endcap-1][station-1][ring-1][chamber-1][cfeb-1] = checkOverload(station,"CFEB",engine);
	    //if (currChamberOverload) bufferOverloaded[endcap-1][station-1][ring-1][chamber-1][cfeb-1] = true;
	    //if (bufferOverloaded[endcap-1][station-1][ring-1][chamber-1][cfeb-1] && num<10) {
	    //   std::cout << " E: " << std::to_string(endcap) 
	    //             << " S: " << std::to_string(station) 
	    //             << " R: " << std::to_string(ring)
	    //             << " C: " << std::to_string(chamber) 
	    //             << " B: " << std::to_string(cfeb) << std::endl;
	    //   num++;
	    //}
	  }
	}
      }
    }
  }

  //iterate over each rechit, get detector id, check if in overloaded cfeb, store in new collection
  CSCDetId prevId;
  CSCDetId currId;
  //std::cout << "Iterating over CSCRecHit2D" << std::endl;
  for (CSCRecHit2DCollection::const_iterator it=recHits->begin(); it!=recHits->end(); it++) {
    CSCRecHit2D hit = (*it);
    currId = (CSCDetId)hit.cscDetId();
    if (currId!=prevId) { //if we are moving on to the next detector
      if (!hitsInLayer.empty()) {
	//std::cout << "   " << prevId << " (" << std::to_string(hitsInLayer.size()) << " CSCRecHits added)" << std::endl;
	oc->put(prevId, hitsInLayer.begin(), hitsInLayer.end());
	hitsInLayer.clear();
      }
      hitsInLayer.clear();
    }
    prevId = currId;
    int numStrips = hit.nStrips();
    int strip = ((numStrips==3) ? hit.channels(1) : hit.channels(0));
    int cfeb = ceil(strip/16.0);
    //std::cout << "CSCDetID: " << currId << " B: " << std::to_string(cfeb);
    if (!bufferOverloaded[currId.endcap()-1][currId.station()-1][currId.ring()-1][currId.chamber()-1][cfeb-1]) {
      //std::cout << " added" << std::endl;
      hitsInLayer.push_back(hit); // if not overloaded, save the normal rechits to our new collection
    }
    //else {
    //   std::cout << " not added" << std::endl;
    //}
  }

  //std::cout << "Done processing overloaded buffers" << std::endl;
  iEvent.put(oc);
}

bool CFEBBufferOverloadProducer::checkOverload(int station, std::string type, CLHEP::HepRandomEngine& engine)
{
  double randomNumber = engine.flat();
  double failRateME21 = failRateME21_;
  double failRateME31 = failRateME31_;
  double failRateME41 = failRateME41_;
  // printf("Station %d: RandomNumber=%3.2f, FailRate=%3.2f\n", station, randomNumber, (station==3) ? failRateME31 : failRateME41 );
  if (doUniformFailure_) {
    if (station==2) return (randomNumber<failRateME21);
    else if (station==3) return (randomNumber<failRateME31);
    else if (station==4) return (randomNumber<failRateME41);
    else return false;
  }
  else { // have an error, give no failure
    return false;
  }
}

// ------------ method called once each job just before starting event loop  ------------
void 
CFEBBufferOverloadProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CFEBBufferOverloadProducer::endJob() {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CFEBBufferOverloadProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CFEBBufferOverloadProducer);
