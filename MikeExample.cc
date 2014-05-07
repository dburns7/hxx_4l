/*
 * =====================================================================================
 *
 *       Filename:  FastCalExporter.cc
 *
 *    Description:  Export data for fast calorimeter trigger simulation
 *                  
 *         Author:  Michael Mulhearn
 *
 * =====================================================================================
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/L1RCTParametersRcd.h"
#include "CondFormats/DataRecord/interface/L1GctJetFinderParamsRcd.h"
#include "CondFormats/DataRecord/interface/L1JetEtScaleRcd.h"
#include "CondFormats/DataRecord/interface/L1EmEtScaleRcd.h"
#include "CondFormats/DataRecord/interface/L1CaloEcalScaleRcd.h"
#include "CondFormats/DataRecord/interface/L1CaloHcalScaleRcd.h"

#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"
#include "CondFormats/L1TObjects/interface/L1GctJetFinderParams.h"
#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/L1TObjects/interface/L1CaloEcalScale.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"

#include "CondFormats/L1TObjects/interface/L1RCTChannelMask.h"
#include "CondFormats/DataRecord/interface/L1RCTChannelMaskRcd.h"
#include "CondFormats/L1TObjects/interface/L1RCTNoisyChannelMask.h"
#include "CondFormats/DataRecord/interface/L1RCTNoisyChannelMaskRcd.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/L1Trigger/interface/L1JetParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1JetParticle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h" 
#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloEmCand.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegionDetId.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Scalers/interface/LumiScalers.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
//#include "CLHEP/Random/RandPoissonQ.h"
#include "FWCore/Utilities/interface/Exception.h"


// RunInfo stuff
#include "CondFormats/RunInfo/interface/RunInfo.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "TTree.h"

#include "L1Trigger/FastCal/interface/FastCalTree.h"

#include <iostream>
#include <fstream>
#include <stdlib.h> 

using namespace fastcal;
using namespace edm;
using namespace std;



typedef vector<InputTag> VInputTag;

// Turn a set of InputTags into a colleciton of candidate pointers.
static void fillPhysicsObjects(const edm::Event& evt, const VInputTag& collections, const vector<int> & quality_flags, double pt_cut, vector<PhysicsObj> * objs) {
  PhysicsObj obj;
  // Loop over collections
  for (size_t i = 0; i < collections.size(); ++i) {
    edm::Handle<edm::View<reco::Candidate> > handle;
    evt.getByLabel(collections[i], handle);
    // Loop over objects in current collection
    for (size_t j = 0; j < handle->size(); ++j) {
      const reco::Candidate& object = handle->at(j);      
      double pt = object.pt();
      if (pt < pt_cut) continue;
      obj.quality = quality_flags[i];
      obj.pt  = pt;
      obj.phi = object.phi();
      obj.eta = object.eta();
      objs->push_back(obj);
    }
  }
}

class FastCalExporter : public EDAnalyzer {
public:
  FastCalExporter(const ParameterSet& pset);
  virtual ~FastCalExporter(){}
  virtual void analyze(const Event& evt, const EventSetup& es);
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
private:
  InputTag   lumiscalers;
  InputTag   ecal_tpgs;
  InputTag   hcal_tpgs;
  InputTag   jet_regions;
  InputTag   em_cands;
  VInputTag  jets_reco;
  VInputTag  em_reco;
  VInputTag  jets_trig;
  VInputTag  em_trig;
  unsigned   seed;
  double     jet_pt_reco_cut;
  double     em_pt_reco_cut;
  double     jet_pt_prescale;
  double     em_pt_prescale;  
  double     jet_lambda_prescale;
  double     em_lambda_prescale;
  double     max_prescale;
  
  TTree * evt_tree;
  FastCalTree data;
};

FastCalExporter::FastCalExporter(const ParameterSet& pset) :
  lumiscalers         (pset.getParameter<InputTag>     ("lumiscalers")),
  ecal_tpgs           (pset.getParameter<InputTag>     ("ecal_tpgs")),
  hcal_tpgs           (pset.getParameter<InputTag>     ("hcal_tpgs")),
  jet_regions         (pset.getParameter<InputTag>     ("jet_regions")),
  em_cands            (pset.getParameter<InputTag>     ("em_cands")),
  jets_reco           (pset.getParameter<VInputTag>    ("jets_reco")),
  em_reco             (pset.getParameter<VInputTag>    ("em_reco")),
  jets_trig           (pset.getParameter<VInputTag>    ("jets_trig")),
  em_trig             (pset.getParameter<VInputTag>    ("em_trig")),
  seed                (pset.getParameter<unsigned int> ("seed")),
  jet_pt_reco_cut     (pset.getParameter<double>       ("jet_pt_reco_cut")),
  em_pt_reco_cut      (pset.getParameter<double>       ("em_pt_reco_cut")),  
  jet_pt_prescale     (pset.getParameter<double>       ("jet_pt_prescale")),
  em_pt_prescale      (pset.getParameter<double>       ("em_pt_prescale")),
  jet_lambda_prescale (pset.getParameter<double>       ("jet_lambda_prescale")),
  em_lambda_prescale  (pset.getParameter<double>       ("em_lambda_prescale")),
  max_prescale        (pset.getParameter<double>       ("max_prescale"))

{
  Service<TFileService> fs;
  evt_tree = fs->make<TTree>("Ntuple",    "Ntuple");
  data.AddBranches(evt_tree);
  srand(seed);
}

void FastCalExporter::beginRun(const edm::Run& run, const edm::EventSetup& eventSetup)
{
  unsigned runN = run.run();
  cout << "FastCalExporter:  Starting Run:  " << runN << "\n";

  cout << "L1RCTParameters:\n";
  edm::ESHandle<L1RCTParameters> rctParameters;
  eventSetup.get<L1RCTParametersRcd>().get(rctParameters);
  const L1RCTParameters* r = rctParameters.product();
  r->print(cout);

  const std::vector<double>& vem = r->eGammaECalScaleFactors();
  int n = vem.size();

  ofstream output("emscale.dat");
  for (int i=0; i<n; i++){
    cout << vem[i] << "\n";
    double buf = vem[i];
    output.write((char *) &buf, sizeof(double)); 
  }
  output.close();

  cout << "L1GctJetFinderParams:\n";
  edm::ESHandle<L1GctJetFinderParams> gctjfParameters;
  eventSetup.get<L1GctJetFinderParamsRcd>().get(gctjfParameters);
  cout << (*gctjfParameters) << "\n";

  cout << "L1JetEtScale::\n";
  edm::ESHandle<L1CaloEtScale> jetScale ;
  eventSetup.get<L1JetEtScaleRcd>().get(jetScale); 
  jetScale->print(cout);

  cout << "L1EmEtScale::\n";
  edm::ESHandle<L1CaloEtScale> emScale;
  eventSetup.get<L1EmEtScaleRcd>().get(emScale);
  emScale->print(cout);

  //cout << "L1CaloEcalScale::\n";
  edm::ESHandle<L1CaloEcalScale> ecalScale;
  eventSetup.get<L1CaloEcalScaleRcd>().get(ecalScale);
  //ecalScale->print(cout);
  //ecal_scale = ecalScale.product();

  //cout << "L1CaloHcalScale::\n";
  edm::ESHandle<L1CaloHcalScale> hcalScale;
  eventSetup.get<L1CaloHcalScaleRcd>().get(hcalScale);
  //hcalScale->print(cout);
  //hcal_scale = hcalScale.product();

  // list of RCT channels to mask
  edm::ESHandle<L1RCTChannelMask> channelMask;
  eventSetup.get<L1RCTChannelMaskRcd>().get(channelMask);
  const L1RCTChannelMask* cEs = channelMask.product();
  cEs->print(cout);

  // list of Noisy RCT channels to mask
  edm::ESHandle<L1RCTNoisyChannelMask> hotChannelMask;
  eventSetup.get<L1RCTNoisyChannelMaskRcd>().get(hotChannelMask);
  const L1RCTNoisyChannelMask* cEsNoise = hotChannelMask.product();
  cEsNoise->print(cout);
  
}


void FastCalExporter::analyze(const Event& evt, const EventSetup& es) {
  static int index=0;
  cout << "FastCalExporter:  analyzing event with index "  << index << "\n";
  index++;
  
  //cout << "FastCalExporter: analyze function called.\n";
  data.Clear();

  //
  // TODO:  change event to long (needed?) in tree... then add run,lumi,evt, plus ilumi to tree...
  //
  // Setup meta info

  data.run       = evt.id().run();
  data.lumiblock = evt.id().luminosityBlock();
  data.event     = evt.id().event();
  data.wgt       = 1.0;

  // Instantaneous lumi from the scalers
  edm::Handle<LumiScalersCollection> lumi;
  evt.getByLabel(lumiscalers, lumi);
  data.lumi = -1;
  if (lumi->size())
    data.lumi = lumi->begin()->instantLumi();

  Handle<EcalTrigPrimDigiCollection> ecal;
  evt.getByLabel(ecal_tpgs, ecal);
  EcalTrigPrimDigiCollection ecalCollection;
  if (ecal.isValid()) { ecalCollection = *ecal;}
  else {return;}
  
  unsigned nEcalDigi = ecalCollection.size();
  if (nEcalDigi>4032) {nEcalDigi=4032;}
  for (unsigned i = 0; i < nEcalDigi; i++){
    
    //TPG: eta [-28,-1],[1,28] phi [1,72]
    int teta = ecalCollection[i].id().ieta();
    int tphi = ecalCollection[i].id().iphi();

    // Get ECAL transparency correction
    unsigned ieta;
    // Map eta to [0,55]
    if (teta < 0)
      ieta = teta + 28;
    else if (teta > 0)
      ieta = teta + 27;
    else {
      cout << "FastCalExporter: unexpected eta value:  zero.\n";
      return;
    }
    unsigned iphi   = tphi - 1;    
    int      soi = ecalCollection[i].sampleOfInterest();
    unsigned raw = ecalCollection[i].sample(soi).raw();

    if (raw > 0){
      //cout << "ecal:  " << ieta << " " << iphi << ":  0x" << hex << raw << dec << " et:  "  << et << "\n";
      data.ecal_tpgs.raw(ieta, iphi)     = raw;
    }
  }
  

  Handle<HcalTrigPrimDigiCollection> hcal;
  evt.getByLabel(hcal_tpgs, hcal);

  HcalTrigPrimDigiCollection hcalCollection;
  if (hcal.isValid()) { hcalCollection = *hcal;}
  else {return;}

  unsigned nHcalDigi = hcalCollection.size();
  //incl HF 4032 + 144 = 4176
  //if (nHcalDigi>4032) {nHcalDigi=4032;}
  for (unsigned i = 0; i < nHcalDigi; i++){
    //TPG: eta [-28,-1],[1,28] phi [1,72]
    //(HF) eta[-32,-29],[29,32]
    int teta = hcalCollection[i].id().ieta(); 
    int tphi = hcalCollection[i].id().iphi();    

    // In L1RCT there is this offset of 18?
    //unsigned iphi = (72 + 18 - cal_iphi) % 72;
    unsigned iphi = tphi - 1;
    // Map eta to [0,63]
    unsigned ieta;
    if (teta < 0)
      ieta = teta + 28;
    else if (teta > 0)
      ieta = teta + 27;
    else {
      cout << "FastCalExporter: unexpected eta value:  zero.\n";
      return;
    }
    ieta += 4; // HF

    unsigned  raw = hcalCollection[i].t0().raw();
    //unsigned  ret = hcalCollection[i].SOI_compressedEt();
    
    if (raw) {
      //cout << "hcaltpgs:  " << ieta << " " << iphi << ":  0x" << hex << raw << dec << "\n";      
      data.hcal_tpgs.raw(ieta, iphi) = raw;
      
    }
  }
  //data.hcaltpgs.Print(0xff);

  Handle<L1CaloRegionCollection> jetreg; 
  evt.getByLabel(jet_regions, jetreg);
  //evt.getByLabel("gctDigis", jetreg);
  for(L1CaloRegionCollection::const_iterator region = jetreg->begin();
      region != jetreg->end(); region++) {
    unsigned eta     = region->id().ieta();
    unsigned phi     = region->id().iphi();
    //unsigned gct_eta = region->gctEta();
    //unsigned gct_phi = region->gctPhi();
    unsigned raw     = region->raw();
    //cout << "eta:  " << eta << " phi:  " << phi << " raw:  " << raw << "\n";
    //cout << "gct eta:  " << gct_eta << " gct phi:  " << gct_phi << "\n";
    data.jet_regions.raw(eta, phi)     = raw;
    //data.jetregions.phys_et(eta, phi) = 0.0;
  }
  //data.jetregions.Print(0x1ff);
  
  Handle<L1CaloEmCollection> emcand;
  evt.getByLabel(em_cands, emcand);
  //evt.getByLabel("gctDigis", emcand);
  for(L1CaloEmCollection::const_iterator cand = emcand->begin();
      cand != emcand->end(); cand++) {
    unsigned eta     = cand->regionId().ieta();
    unsigned phi     = cand->regionId().iphi();
    unsigned raw     = cand->raw();
    unsigned iso     = (unsigned) cand->isolated();

    if (iso) {
      data.em_cands_iso.raw(eta, phi) = raw;
    } else {
      data.em_cands_noniso.raw(eta, phi) = raw;
    }    
  }
  //data.emcandions.Print(0x1ff);
  
  //
  // Reco Jets Access:
  //
  PhysicsObj jet;

  // should be config...
  std::vector<int> q;
  q.push_back(0);
  q.push_back(1);
  q.push_back(2);

  fillPhysicsObjects(evt, jets_reco, q, jet_pt_reco_cut, &data.jets_reco);
  fillPhysicsObjects(evt, jets_trig, q, 0.0, &data.jets_trig);
  fillPhysicsObjects(evt, em_reco,   q, em_pt_reco_cut, &data.em_reco);
  fillPhysicsObjects(evt, em_trig,   q, 0.0, &data.em_trig);

  double prescale = max_prescale;

  
  if (data.jets_reco.size() > 0){
    double jet_pt = data.jets_reco[0].pt;
    if (jet_pt < jet_pt_prescale){
      if (jet_lambda_prescale > 0.0){
	double jet_prescale = exp((jet_pt_prescale - jet_pt) / jet_lambda_prescale);
	if (prescale > jet_prescale) prescale = jet_prescale;
      } 
    } else {
      prescale = 1;
    }
  }

  if (data.em_reco.size() > 0){
    double em_pt = data.em_reco[0].pt;
    if (em_pt < em_pt_prescale){
      if (em_lambda_prescale > 0.0){
	double em_prescale = exp((em_pt_prescale - em_pt) / em_lambda_prescale);
	if (prescale > em_prescale) prescale = em_prescale;
      } 
    } else {
      prescale = 1;
    }
  }

  data.wgt = 1.0;
  if (prescale > 1.0){
    double x = rand() / double(RAND_MAX);
    x = x * prescale;
    if (x < 1.0) data.wgt = prescale;
    else         data.wgt = 0;
  }
  //cout << "prescale:  " << prescale << " weight:  " << data.wgt << " njets:  " << data.jets_reco.size() << "\n";

  if (data.wgt > 0.0){
    data.Pack();
    evt_tree->Fill();
  } 
}
    
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(FastCalExporter);
