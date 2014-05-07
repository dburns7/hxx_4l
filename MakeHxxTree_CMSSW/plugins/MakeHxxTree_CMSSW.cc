// -*- C++ -*-
//
// Package:    MakeHxxTree_CMSSW
// Class:      MakeHxxTree_CMSSW
// 
/**\class MakeHxxTree_CMSSW MakeHxxTree_CMSSW.cc hxx_4l/MakeHxxTree_CMSSW/src/MakeHxxTree_CMSSW.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  dburns@ucdavis.edu
//         Created:  Wed May  7 12:49:25 PDT 2014
// $Id$
//
//


// system include files
#include <memory>
#include <map>
#include <string>
#include <iostream>
using namespace std;

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

//#include "DataFormats/PatCandidates/interface/Electron.h"
//#include "DataFormats/PatCandidates/interface/Muon.h"
//#include "DataFormats/PatCandidates/interface/MET.h"

#include "TTree.h"
#include "hxx_tree.h"

//
// class declaration
//

class MakeHxxTree_CMSSW : public edm::EDAnalyzer {
   public:
      explicit MakeHxxTree_CMSSW(const edm::ParameterSet&);
      ~MakeHxxTree_CMSSW();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      edm::InputTag elecSrc_;
      edm::InputTag muonSrc_;
      edm::InputTag metSrc_;
      
      TTree * evt_tree;
      hxx_tree data;

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
MakeHxxTree_CMSSW::MakeHxxTree_CMSSW(const edm::ParameterSet& iConfig):
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc"))
{
   //now do what ever initialization is needed
  Service<TFileService> fs;
  evt_tree = fs->make<TTree>("Ntuple",    "Ntuple");
  data.AddBranches(evt_tree);
}


MakeHxxTree_CMSSW::~MakeHxxTree_CMSSW()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
MakeHxxTree_CMSSW::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
  // get electron collection
  edm::Handle<edm::View<pat::Electron> > electrons;
  iEvent.getByLabel(elecSrc_,electrons);

  // get muon collection
  edm::Handle<edm::View<pat::Muon> > muons;
  iEvent.getByLabel(muonSrc_,muons);

  // get met collection
  edm::Handle<edm::View<pat::MET> > mets;
  iEvent.getByLabel(metSrc_,mets);

  int muonCnt = muons->size();
  int elecCnt = electrons->size();

  cout << muonCnt << endl;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
MakeHxxTree_CMSSW::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
MakeHxxTree_CMSSW::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
MakeHxxTree_CMSSW::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
MakeHxxTree_CMSSW::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
MakeHxxTree_CMSSW::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
MakeHxxTree_CMSSW::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
MakeHxxTree_CMSSW::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MakeHxxTree_CMSSW);
