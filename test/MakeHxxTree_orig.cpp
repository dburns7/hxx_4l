#include <iostream>
#include <utility>
#include <vector>

#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"

#include "TString.h"

#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TRandom.h"

#include "classes/DelphesClasses.h"

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "ExRootAnalysis/ExRootResult.h"
#include "ExRootAnalysis/ExRootUtilities.h"


#include "koptions.h"
#include "hxx_tree.h"

using namespace std;
using namespace kutil;

void usage(){
   cout << "usage:  MakeHxxTree  [OPTIONS] <input_file> <output_file>\n";
   cout << "\n";
   cout << " --seed=<n>          (0)     set rng seed if non-zero\n";
   cout << " --sample=<n>        (0)     set sample id to <n>\n";
   cout << " --lumi=<l>          (14)    set weight for lumi <l> [fb^-1]\n";
   cout << " --xsec=<x>          (0)     set weight for xsec <x> [fb]\n";
   cout << " --maxevent=<n>      (0)     if greater than zero, write only <n> events.\n";
   cout << " --mllcut                    require non-zero mll (saves space).\n";
   exit(0);
}

int compare_part(GenParticle * g1, GenParticle * g2){ return (g1->PT > g2->PT); }

int main(int argc, char *argv[])
{
   TRandom rng;
   hxx_tree data;
   int    seed = 0;
   int    sample = 0;
   double lumi = 14.0;
   double xsec = 0.0;
   double weight = 1.0; 
   int maxevent = 0;

   std::string opt, infile, outfile;
   koptions options(argc, argv);
   
   //check for the --help option:
   int mllcut = options.find("--mllcut"); 

   if ( options.find("--help") ) { usage(); }
   options.set("--seed=", seed);   
   if (seed > 0) rng.SetSeed(seed);

   options.set("--sample=", sample);   
   options.set("--lumi=", lumi);
   options.set("--xsec=", xsec);
   options.set("--maxevent=", maxevent);

   //check for unrecognized options (beginning with -- or -)
   while(options.unrecognized_option(opt)){
      cout << "WARNING: unrecognized option:" << opt << "\n";
      usage();
   } 

   if (options.size() < 2){
      usage();
   }

   options.shift_argument(outfile);

   cout << "INFO: sample id is " << sample << "\n";
   cout << "INFO: writing analysis tree file:  " << outfile << "\n";

   TChain chain("Delphes");
   while(options.shift_argument(infile)){
      cout << "INFO: adding input file:  " << infile << "\n";   
      chain.Add(infile.c_str());
   }

   ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
   Long64_t numberOfEntries = treeReader->GetEntries();
   if (numberOfEntries == 0) { cout << "Zero entries...\n"; return 0; }
   if ((maxevent > 0) && (maxevent < numberOfEntries)) numberOfEntries = maxevent;
   double Ngen = numberOfEntries;

   // Get pointers to branches used in this analysis
   TClonesArray *branchJet      = treeReader->UseBranch("Jet");
   TClonesArray *branchElec     = treeReader->UseBranch("Electron");
   TClonesArray *branchMuon     = treeReader->UseBranch("Muon");
   TClonesArray *branchMET      = treeReader->UseBranch("MissingET");  
   TClonesArray *branchHt       = treeReader->UseBranch("ScalarHT");  
   TClonesArray *branchGen      = treeReader->UseBranch("Particle");  

   // calculate appropriate weight:
   if (xsec > 0.0){
      double Lgen = Ngen / xsec;
      weight = lumi * xsec / Ngen;
      cout << "INFO:  calculating event weights:\n";
      cout << "INFO:  Ngen = " << Ngen << "\n";
      cout << "INFO:  lumi = " << lumi << "\n";
      cout << "INFO:  xsec = " << xsec << "\n";
      cout << "INFO:  Lgen = " << Lgen << "\n";
      cout << "INFO:  weight = " << weight << "\n";
      cout << "INFO:  Ntot = " << weight * Ngen << "\n";
   } else {
      cout << "INFO:  no xsec provided, using weight=1";
      weight = 1;
   }

   TFile * file = new TFile(outfile.c_str(), "RECREATE");
   TTree * tree = new TTree("hxxtree", "");
   data.WriteTree(tree);
   
   double tot_wgt = 0.0;
   int count = 0;
   int nupdate = numberOfEntries / 20;
   if (nupdate < 1) nupdate=1;
   cout << "PROGRESS:  ";

   // Loop over all events
   for(Int_t entry = 0; entry < numberOfEntries; ++entry)
   {
      count++;
      if (count % nupdate == 0) { cout << "."; cout.flush(); }
      treeReader->ReadEntry(entry);
      
      data.Clear();
      data.sample = sample;
      data.weight = weight;
      data.weight_met = 0;

      data.nelec = branchElec->GetEntries();
      data.nmuon = branchMuon->GetEntries();
      data.njet  = branchJet->GetEntries();
      
      if (data.nmuon >= data.nelec) {
         data.lepton_flavor = LFMUMU;
         if (branchMuon->GetEntries() > 0) {
            Muon * mu1 = (Muon*) branchMuon->At(0);
            data.l1_pt = mu1->PT;
            data.l1_eta = mu1->Eta;
            data.l1_phi = mu1->Phi;
         }
         if (branchMuon->GetEntries() > 1) {
            Muon * mu1 = (Muon*) branchMuon->At(0);
            Muon * mu2 = (Muon*) branchMuon->At(1);
            data.l2_pt = mu2->PT;
            data.l2_eta = mu2->Eta;
            data.l2_phi = mu2->Phi;
            data.mll = ((mu1->P4()) + (mu2->P4())).M(); 
         }
      } else {
         data.lepton_flavor = LFEMEM;
         if (branchElec->GetEntries() > 0) {
            Electron * elec1 = (Electron*) branchElec->At(0);
            data.l1_pt = elec1->PT;
            data.l1_eta = elec1->Eta;
            data.l1_phi = elec1->Phi;
         }
         if (branchElec->GetEntries() > 1) {
            Electron * elec1 = (Electron*) branchElec->At(0);
            Electron * elec2 = (Electron*) branchElec->At(1);
            data.l2_pt = elec2->PT;
            data.l2_eta = elec2->Eta;
            data.l2_phi = elec2->Phi;
            data.mll = ((elec1->P4()) + (elec2->P4())).M(); 
         }
      }


      if (mllcut && (data.mll <= 0.0)) continue;

      int nJet = branchJet->GetEntries();
      for (int i=0; i<nJet; i++){
         Jet * jet = (Jet*) branchJet->At(i);
         data.jet_pt     ->push_back(jet->PT);
         data.jet_eta    ->push_back(jet->Eta);
         data.jet_phi    ->push_back(jet->Phi);
         data.jet_btag   ->push_back((int) jet->BTag);
         data.jet_tautag ->push_back((int) jet->TauTag);
      }      


      if (branchMET->GetEntries() > 0) {
         MissingET * met = (MissingET *) branchMET->At(0);
         data.nopu_met = met->MET;
         data.nopu_met_phi = met->Phi;
      }

      if (branchHt->GetEntries() > 0) {
         ScalarHT * ht = (ScalarHT *) branchHt->At(0);
         data.ht = ht->HT;
      }

      // Sort Gen Muons and Electrons:
      std::vector<GenParticle *> genLept;
      int npart = branchGen->GetEntries();
      for (int i=0; i<npart; i++){
	GenParticle * part = (GenParticle*) branchGen->At(i);
	int aPID = abs(part->PID);
	if (part->Status!=1) continue;
	if (part->PT < 1.0) continue;
	if (part->D1 != -1) { cout << "ERROR DETECTED!\n"; }

	if (aPID == 11){ genLept.push_back(part); }     
	if (aPID == 13){ genLept.push_back(part); }
      }
      sort(genLept.begin(), genLept.end(), compare_part);


      if (genLept.size() > 0) {
	GenParticle * lept1 = genLept.at(0);
	data.gl1_pt  = lept1->PT;
	data.gl1_eta = lept1->Eta;
	data.gl1_phi = lept1->Phi;
      }
      if (genLept.size() > 1) {
	GenParticle * lept1 = genLept.at(0);
	GenParticle * lept2 = genLept.at(1);
	data.gl2_pt  = lept2->PT;
	data.gl2_eta = lept2->Eta;
	data.gl2_phi = lept2->Phi;
	data.gmll = ((lept1->P4()) + (lept2->P4())).M(); 
      }

      if (0) {
	int n = genLept.size();
	if (n > 2) n=2;
	cout << "\n\nGenerated Leptons:\n";
	for (int i=0; i<n; i++) { cout << "Lept:  " << genLept[i]->PT << "\n"; }
	int nmuon = branchMuon->GetEntries();
	cout << "Reco Muon:\n";
	for (int i=0; i<nmuon; i++){
	  Muon * mu = (Muon*) branchMuon->At(i);
	  cout << "PT:  " << mu->PT << "\n";
	}
	
	int nelec = branchElec->GetEntries();
	cout << "Reco Electron:\n";
	for (int i=0; i<nelec; i++){
	  Electron * mu = (Electron*) branchElec->At(i);
	  cout << "PT:  " << mu->PT << "\n";
	}
      }

      tot_wgt += data.weight;
      tree->Fill();
   }
   cout << "\n";

   cout << "SUMMARY:  wrote " << tree->GetEntries() << " to analysis tree from " << count << " events considered.\n";
   cout << "SUMMARY:  total weight: " << tot_wgt << "\n";
   file->cd();
   tree->Write();
   file->Close();   

   cout << "SUMMARY:  done writing files.\n";
}


