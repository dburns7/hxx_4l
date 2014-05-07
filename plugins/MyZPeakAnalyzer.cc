#include <map>
#include <string>
#include <iostream>
using namespace std;

#include "TH1.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/MET.h"

class MyZPeakAnalyzer : public edm::EDAnalyzer {

public:
  explicit MyZPeakAnalyzer(const edm::ParameterSet&);
  ~MyZPeakAnalyzer();
  
private:

  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  // simple map to contain all histograms; 
  // histograms are booked in the beginJob() 
  // method
  std::map<std::string,TH1F*> histContainer_; 
  //TH1F* met1;  
  //TFile* f;
  
  // input tags  
  edm::InputTag elecSrc_;
  edm::InputTag muonSrc_;
  edm::InputTag metSrc_;
};


MyZPeakAnalyzer::MyZPeakAnalyzer(const edm::ParameterSet& iConfig):

  histContainer_(),
  elecSrc_(iConfig.getUntrackedParameter<edm::InputTag>("electronSrc")),
  muonSrc_(iConfig.getUntrackedParameter<edm::InputTag>("muonSrc")),
  metSrc_(iConfig.getUntrackedParameter<edm::InputTag>("metSrc"))
{
}

MyZPeakAnalyzer::~MyZPeakAnalyzer()
{
}

void
MyZPeakAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
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
  
  histContainer_["met"]->Fill(mets->begin()->pt()); 
  
  // If Event contains 2 electrons and 2 muons, select first two of each, oppositely charged
  if(muonCnt == 2 && elecCnt == 2)
  { 
    int es = 0;
    int ms = 0;
    // loop over muons
    for(edm::View<pat::Muon>::const_iterator muon1=muons->begin(); muon1!=muons->end(); ++muon1)
    {
      if(muon1->pt()<5) continue;
      //if(fabs(muon1->eta())>2.1) continue;
      ms+=1;
      
      histContainer_["muonPt"]->Fill(muon1->pt());
      histContainer_["muonEta"]->Fill(muon1->eta());
      histContainer_["muonPhi"]->Fill(muon1->phi());

      // loop over muons
      for(edm::View<pat::Muon>::const_iterator muon2=muons->begin(); muon2!=muons->end(); ++muon2)
      {
        double diMuMass = (muon1->p4()+muon2->p4()).mass();
        
        if(muon2<muon1) continue;
        if(muon1->charge()*muon2->charge()>0) continue;
        if(muon2->pt()<5) continue;
        //if(fabs(muon2->eta())>2.1) continue;
        if(diMuMass < 3) continue;
        ms+=1;
        
        histContainer_["mumuMass"]->Fill(diMuMass);       
       
        // loop over electrons
        for(edm::View<pat::Electron>::const_iterator electron1=electrons->begin(); electron1!=electrons->end(); ++electron1)
        {
          if(electron1->pt()<5) continue;
          //if(fabs(electron1->eta())>2.1) continue;
          es+=1;
          
          // loop over electrons
          for(edm::View<pat::Electron>::const_iterator electron2=electrons->begin(); electron2!=electrons->end(); ++electron2)
          {  
            double diElecMass = (electron1->p4()+electron2->p4()).mass();
            double diElecdiMuMass = (muon1->p4()+muon2->p4()+electron1->p4()+electron2->p4()).mass();
            double leadingMass, subleadingMass;         
            if(abs(diElecMass-91) < abs(diMuMass-91))
            {
              leadingMass = diElecMass;
              subleadingMass = diMuMass;
            }
            else if(abs(diMuMass-91) < abs(diElecMass-91))
            {
              leadingMass = diMuMass;
              subleadingMass = diElecMass;
            }
            
            if(electron2<electron1) continue;
            if(electron1->charge()*electron2->charge()>0) continue;
            if(electron2->pt()<5) continue;
            //if(fabs(electron2->eta())>2.1) continue;
            if(leadingMass < 3 | subleadingMass < 3) continue;        
            if(diElecMass < 3 | diMuMass < 3) continue;        
            es+=1;
            cout << ms << "  " << es << endl;
            if(ms > 2 && es > 2) break;

            histContainer_["leadingMass"]->Fill(leadingMass);
            histContainer_["subleadingMass"]->Fill(subleadingMass);
            //histContainer_["eeMass"]->Fill(diElecMass);
            histContainer_["mumueeMass"]->Fill(diElecdiMuMass);
            histContainer_["test"]->Fill(20);
            histContainer_["totalMass"]->Fill(diElecdiMuMass);
            //histContainer_["met"]->Fill(mets->begin()->pt());
          
          
          }
        }     
      }
    }
  }
 
  // If there are 4 muons and 0 electrons
  if(muonCnt == 4 && elecCnt == 0)
  {
    int ms = 0;
    
    // loop over muons
    for(edm::View<pat::Muon>::const_iterator muon1=muons->begin(); muon1!=muons->end(); ++muon1)
    {
      if(muon1->pt()<5) continue;
      //if(fabs(muon1->eta())>2.1) continue;
      ms+=1;
       
      histContainer_["muonPt"]->Fill(muon1->pt());
      histContainer_["muonEta"]->Fill(muon1->eta());
      histContainer_["muonPhi"]->Fill(muon1->phi());
      
      // loop over muons
      for(edm::View<pat::Muon>::const_iterator muon2=muons->begin(); muon2!=muons->end(); ++muon2)
      {
        double diMuMass = (muon1->p4()+muon2->p4()).mass();
        
        if(muon2<muon1) continue;
        if(muon1->charge()*muon2->charge()>0) continue;
        if(muon2->pt()<5) continue;
        //if(fabs(muon2->eta())>2.1) continue;
        if(diMuMass < 3) continue;
        ms+=1;
        
        histContainer_["mumuMass"]->Fill(diMuMass);
        
        // loop over muons
        for(edm::View<pat::Muon>::const_iterator muon3=muons->begin(); muon3!=muons->end(); ++muon3)
        {
          if(muon3->pt()<5) continue;
          if(muon3 < muon1 | muon3 < muon1) continue;
          ms+=1;
          
          for(edm::View<pat::Muon>::const_iterator muon4=muons->begin(); muon4!=muons->end(); ++muon4)
          {
            double fourMuMass = (muon1->p4()+muon2->p4()+muon3->p4()+muon4->p4()).mass();
            double diMuMass2 = (muon3->p4()+muon4->p4()).mass();
            double leadingMass, subleadingMass;
            if(abs(diMuMass-91) < abs(diMuMass2-91)){
              leadingMass = diMuMass;
              subleadingMass = diMuMass2;
            }
            else if(abs(diMuMass2-91) < abs(diMuMass-91)){
              leadingMass = diMuMass2;
              subleadingMass = diMuMass;
            }
            
            if(muon4->pt()<5) continue;
            if(muon4 < muon1 | muon4 < muon2 | muon4 < muon3) continue;
            if(diMuMass < 3 | diMuMass2 < 3) continue;
            if(leadingMass < 3 | subleadingMass < 3) continue;
            ms+=1;
            if(ms > 4) break;

            histContainer_["totalMass"]->Fill(fourMuMass);
            histContainer_["leadingMass"]->Fill(leadingMass);
            histContainer_["subleadingMass"]->Fill(subleadingMass);
          }
        }
      }  
    }
  }  
  
  // If there are 4 electrons and 0 muons
  if(elecCnt == 4 && muonCnt ==0)
  {
    int es = 0;
    
    // loop over electrons
    for(edm::View<pat::Electron>::const_iterator electron1=electrons->begin(); electron1!=electrons->end(); ++electron1)
    {
      if(electron1->pt() < 5) continue;
      //if(electron1->eta() > 2.1) continue;
      es+=1;
      
      histContainer_["elePt"]->Fill(electron1->pt());
      histContainer_["eleEta"]->Fill(electron1->eta());
      histContainer_["elePhi"]->Fill(electron1->phi());
    
      // loop over electrons
      for(edm::View<pat::Electron>::const_iterator electron2=electrons->begin(); electron2!=electrons->end(); ++electron2)
      {
        double diElecMass1 = (electron1->p4()+electron2->p4()).mass();
        
        if(electron2->pt() < 5) continue;
        //if(electron2->eta >2.1) continue;
        if(electron2 < electron1) continue;
        if(electron1->charge()*electron2->charge() > 0) continue;
        es+=1;
        
        histContainer_["eeMass"]->Fill(diElecMass1);
        
        // loop over electrons
        for(edm::View<pat::Electron>::const_iterator electron3=electrons->begin(); electron3!=electrons->end(); ++electron3)
        {
          if(electron3->pt() < 5) continue;
          if(electron3 < electron1 | electron3 < electron2) continue;
          es+=1; 
          
          // loop over electrons
          for(edm::View<pat::Electron>::const_iterator electron4=electrons->begin(); electron4!=electrons->end(); ++electron4)
          {
            double fourEMass = (electron1->p4()+electron2->p4()+electron3->p4()+electron4->p4()).mass();
            double leadingMass, subleadingMass;
            double diElecMass2 = (electron3->p4()+electron4->p4()).mass();
            
            if(electron4->pt() < 5) continue;
            if(electron4 < electron1 | electron4 < electron2 | electron4 < electron3) continue;
            if(abs(diElecMass1-91) < abs(diElecMass2-91)){
              leadingMass = diElecMass1;
              subleadingMass = diElecMass2;
            }
            else if(abs(diElecMass2-91) < abs(diElecMass1-91)){
              leadingMass = diElecMass2;
              subleadingMass = diElecMass1;
            }
            if(diElecMass1 < 3 | diElecMass2 < 3) continue;
            if(leadingMass < 3 | subleadingMass < 3) continue;
            es+=1;
            if(es > 4) break;
            
            histContainer_["totalMass"]->Fill(fourEMass);
            histContainer_["leadingMass"]->Fill(leadingMass);
            histContainer_["subleadingMass"]->Fill(subleadingMass);
          }
        }
      }
    }
  }

  // Multiplicity
  histContainer_["eleMult" ]->Fill(electrons->size());
  histContainer_["muonMult"]->Fill(muons->size() );
}

void 
MyZPeakAnalyzer::beginJob()
{
  // register to the TFileService
  edm::Service<TFileService> fs;
  //TFile *f = new TFile("monoh_hists.root", "UPDATE");
  // book histograms for MET
  histContainer_["met"]=fs->make<TH1F>("met", "met",    100,   0, 200);
  //TH1F *met1 = new TH1F("met1","met1", 100, 0, 200);
  
  // book histograms for Invariant mass
  histContainer_["mumuMass"]=fs->make<TH1F>("mumuMass", "mumu Mass",    100,   0, 120);
  histContainer_["eeMass"]=fs->make<TH1F>("eeMass", "ee Mass",    100,   0, 120);
  histContainer_["mumueeMass"]=fs->make<TH1F>("mumueeMass", "mumuee Mass",    100,   30, 150);
  histContainer_["leadingMass"]=fs->make<TH1F>("leadingMass", "leading Mass",    100,   0, 120);
  histContainer_["subleadingMass"]=fs->make<TH1F>("subleadingMass", "subleading Mass",    100,   0, 120);
  histContainer_["test"]=fs->make<TH1F>("test", "test",    100,   0, 120);
  histContainer_["totalMass"]=fs->make<TH1F>("totalMass", "total mass", 100, 0, 150);
  // book histograms for Multiplicity:

  histContainer_["eleMult"]=fs->make<TH1F>("eleMult",   "electron multiplicity", 100, 0,  50);
  histContainer_["muonMult"]=fs->make<TH1F>("muonMult",   "muon multiplicity",     100, 0,  50);

  // book histograms for Pt:

  histContainer_["elePt"]=fs->make<TH1F>("elePt",   "electron Pt", 100, 0,  200);
  histContainer_["muonPt"]=fs->make<TH1F>("muonPt",   "muon Pt", 100, 0, 200);

 // book histograms for Eta:
  histContainer_["eleEta"]=fs->make<TH1F>("eleEta",   "electron Eta",100, -5,  5);
  histContainer_["muonEta"]=fs->make<TH1F>("muonEta",   "muon Eta",  100, -5,  5);


 // book histograms for Phi:
  histContainer_["elePhi"]=fs->make<TH1F>("elePhi",   "electron Phi", 100, -3.5, 3.5);
  histContainer_["muonPhi"]=fs->make<TH1F>("muonPhi",   "muon Phi",     100, -3.5, 3.5);
    
}

void 
MyZPeakAnalyzer::endJob() 
{
  //cout << "JOB END" << endl;
  //met1->Write();
  //f->Close();
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(MyZPeakAnalyzer);

