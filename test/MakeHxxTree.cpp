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
      data.testvar = 2;
      data.sample = sample;
  //hs->Draw();
  //hs->Draw();
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
//-------------------------------------------ADDED-----------------------------
       
      /*
      //single electron variables
      for (int i=0; i<data.nelec; i++){
         Electron * elec = (Electron*) branchElec->At(i);
         data.elec_pt     ->push_back(elec->PT);
         data.elec_eta    ->push_back(elec->Eta);
         data.elec_phi    ->push_back(elec->Phi);
         //data.elec_P4     ->push_back(elec->P4());
         data.elec_ch     ->push_back(elec->Charge);
      }   
      
      //single muon variables
      for (int i=0; i<data.nmuon; i++){
         Muon * muon = (Muon*) branchMuon->At(i);
         data.muon_pt     ->push_back(muon->PT);
         data.muon_eta    ->push_back(muon->Eta);
         data.muon_phi    ->push_back(muon->Phi);
         data.muon_ch     ->push_back(muon->Charge);
      }

      //dielectron variables
      if(data.nelec > 1){
         Electron * elec1 = (Electron*) branchElec->At(0);
         Electron * elec2 = (Electron*) branchElec->At(1);
         TLorentzVector elecP4 = (elec1->P4())+(elec2->P4());
         data.dielec_minv ->push_back(elecP4.M());
         if(data.nelec > 3){
           Electron * elec3 = (Electron*) branchElec->At(2);
           Electron * elec4 = (Electron*) branchElec->At(3);
           TLorentzVector elecP42 = (elec3->P4())+(elec4->P4());
           data.dielec_minv ->push_back(elecP42.M());
         }
      }
      
      //dimuon variables
      if(data.nmuon > 1){
         Muon * muon1 = (Muon*) branchMuon->At(0);
         Muon * muon2 = (Muon*) branchMuon->At(1);
         TLorentzVector muonP4 = (muon1->P4())+(muon2->P4());
         data.dimuon_minv ->push_back(muonP4.M());
         if(data.nmuon > 3){
           Muon * muon3 = (Muon*) branchMuon->At(2);
           Muon * muon4 = (Muon*) branchMuon->At(3);
           TLorentzVector muonP42 = (muon3->P4())+(muon4->P4());
           data.dimuon_minv ->push_back(muonP42.M());
         }
      }

      //dilepton and 4lepton variables
      Electron *elec1, *elec2, *elec3, *elec4;
      Muon     *muon1, *muon2, *muon3, *muon4;
      TLorentzVector dimuonP4, dimuonP4_2, dielecP4, dielecP4_2, totalP4;
      double dimuonPT, dielecPT, totalPT; 
      if(data.nelec == 2 && data.nmuon == 2){
        weight *= 0.1*weight;
        elec1 = (Electron *) branchElec->At(0);
        elec2 = (Electron *) branchElec->At(1);
        muon1   = (Muon *) branchMuon->At(0);
        muon2   = (Muon *) branchMuon->At(1);
        dimuonP4 = (muon1->P4())+(muon2->P4());
        dielecP4 = (elec1->P4())+(elec2->P4());
        totalP4 = dimuonP4 + dielecP4;
        if(abs(dielecP4.M()-91.0) < abs(dimuonP4.M()-91.0)){
          data.leadingm = dielecP4.M();
          data.subleadingm = dimuonP4.M();
        }
        else{
          data.leadingm = dimuonP4.M();
          data.subleadingm = dielecP4.M();
        }
        data.total_minv = totalP4.M(); 
      }
      if(data.nelec == 4 && data.nmuon == 0){
        weight *= 0.1*weight;
        elec1 = (Electron *) branchElec->At(0);
        elec2 = (Electron *) branchElec->At(1);
        elec3 = (Electron *) branchElec->At(2);
        elec4 = (Electron *) branchElec->At(3);
        dielecP4 = (elec1->P4())+(elec2->P4());
        dielecP4_2 = (elec3->P4())+(elec4->P4());
        totalP4 = dielecP4 + dielecP4_2;
        if(abs(dielecP4.M()-91.0) < abs(dielecP4_2.M()-91.0)){
          data.leadingm = dielecP4.M();
          data.subleadingm = dielecP4_2.M();
        }
        else{
          data.leadingm = dielecP4_2.M();
          data.subleadingm = dielecP4.M();
        }
        data.total_minv = totalP4.M();
      }
      if(data.nelec == 0 && data.nmuon == 4){
        weight *= 0.1*weight;
        muon1   = (Muon *) branchMuon->At(0);
        muon2   = (Muon *) branchMuon->At(1);
        muon3   = (Muon *) branchMuon->At(2);
        muon4   = (Muon *) branchMuon->At(3);
        dimuonP4 = (muon1->P4())+(muon2->P4());
        dimuonP4_2 = (muon3->P4())+(muon4->P4());
        totalP4 = dimuonP4 + dimuonP4_2;
        if(abs(dimuonP4.M()-91.0) < abs(dimuonP4_2.M()-91.0)){
          data.leadingm = dimuonP4.M();
          data.subleadingm = dimuonP4_2.M();
        }
        else{
          data.leadingm = dimuonP4_2.M();
          data.subleadingm = dimuonP4.M();
        }
        data.total_minv = totalP4.M();
      }
      */
      
      Electron *elec1, *elec2, *elec3, *elec4;
      Muon     *muon1, *muon2, *muon3, *muon4;
      TLorentzVector dimuonP4, dimuonP4_2, dielecP4, dielecP4_2, totalP4;
      if(data.nelec == 2 && data.nmuon == 2){
        weight *= 0.1*weight;
        elec1 = (Electron *) branchElec->At(0);
        elec2 = (Electron *) branchElec->At(1);
        muon1   = (Muon *) branchMuon->At(0);
        muon2   = (Muon *) branchMuon->At(1);
        dimuonP4 = (muon1->P4())+(muon2->P4());
        dielecP4 = (elec1->P4())+(elec2->P4());
        totalP4 = dimuonP4 + dielecP4;
        data.elec_pt     ->push_back(elec1->PT);
        data.elec_eta    ->push_back(elec1->Eta);
        data.elec_phi    ->push_back(elec1->Phi);
        data.elec_ch     ->push_back(elec1->Charge);
        data.elec_pt     ->push_back(elec2->PT);
        data.elec_eta    ->push_back(elec2->Eta);
        data.elec_phi    ->push_back(elec2->Phi);
        data.elec_ch     ->push_back(elec2->Charge);
        data.muon_pt     ->push_back(muon1->PT);
        data.muon_eta    ->push_back(muon1->Eta);
        data.muon_phi    ->push_back(muon1->Phi);
        data.muon_ch     ->push_back(muon1->Charge);
        data.muon_pt     ->push_back(muon2->PT);
        data.muon_eta    ->push_back(muon2->Eta);
        data.muon_phi    ->push_back(muon2->Phi);
        data.muon_ch     ->push_back(muon2->Charge);
        data.dielec_minv ->push_back(dielecP4.M());
        data.dimuon_minv ->push_back(dimuonP4.M());
        if(abs(dielecP4.M()-91.0) < abs(dimuonP4.M()-91.0)){
          data.leadingm = dielecP4.M();
          data.subleadingm = dimuonP4.M();
        }
        else{
          data.leadingm = dimuonP4.M();
          data.subleadingm = dielecP4.M();
        }
        data.total_minv = totalP4.M(); 
      }
      if(data.nelec == 4 && data.nmuon == 0){
        weight *= 0.1*weight;
        elec1 = (Electron *) branchElec->At(0);
        elec2 = (Electron *) branchElec->At(1);
        elec3 = (Electron *) branchElec->At(2);
        elec4 = (Electron *) branchElec->At(3);
        dielecP4 = (elec1->P4())+(elec2->P4());
        dielecP4_2 = (elec3->P4())+(elec4->P4());
        totalP4 = dielecP4 + dielecP4_2;
        data.elec_pt     ->push_back(elec1->PT);
        data.elec_eta    ->push_back(elec1->Eta);
        data.elec_phi    ->push_back(elec1->Phi);
        data.elec_ch     ->push_back(elec1->Charge);
        data.elec_pt     ->push_back(elec2->PT);
        data.elec_eta    ->push_back(elec2->Eta);
        data.elec_phi    ->push_back(elec2->Phi);
        data.elec_ch     ->push_back(elec2->Charge);
        data.elec_pt     ->push_back(elec3->PT);
        data.elec_eta    ->push_back(elec3->Eta);
        data.elec_phi    ->push_back(elec3->Phi);
        data.elec_ch     ->push_back(elec3->Charge);
        data.elec_pt     ->push_back(elec4->PT);
        data.elec_eta    ->push_back(elec4->Eta);
        data.elec_phi    ->push_back(elec4->Phi);
        data.elec_ch     ->push_back(elec4->Charge);
        data.dielec_minv ->push_back(dielecP4.M());
        data.dielec_minv ->push_back(dielecP4_2.M());
        if(abs(dielecP4.M()-91.0) < abs(dielecP4_2.M()-91.0)){
          data.leadingm = dielecP4.M();
          data.subleadingm = dielecP4_2.M();
        }
        else{
          data.leadingm = dielecP4_2.M();
          data.subleadingm = dielecP4.M();
        }
        data.total_minv = totalP4.M();
      }
      if(data.nelec == 0 && data.nmuon == 4){
        weight *= 0.1*weight;
        muon1   = (Muon *) branchMuon->At(0);
        muon2   = (Muon *) branchMuon->At(1);
        muon3   = (Muon *) branchMuon->At(2);
        muon4   = (Muon *) branchMuon->At(3);
        dimuonP4 = (muon1->P4())+(muon2->P4());
        dimuonP4_2 = (muon3->P4())+(muon4->P4());
        totalP4 = dimuonP4 + dimuonP4_2;
        data.muon_pt     ->push_back(muon1->PT);
        data.muon_eta    ->push_back(muon1->Eta);
        data.muon_phi    ->push_back(muon1->Phi);
        data.muon_ch     ->push_back(muon1->Charge);
        data.muon_pt     ->push_back(muon2->PT);
        data.muon_eta    ->push_back(muon2->Eta);
        data.muon_phi    ->push_back(muon2->Phi);
        data.muon_ch     ->push_back(muon2->Charge);
        data.muon_pt     ->push_back(muon3->PT);
        data.muon_eta    ->push_back(muon3->Eta);
        data.muon_phi    ->push_back(muon3->Phi);
        data.muon_ch     ->push_back(muon3->Charge);
        data.muon_pt     ->push_back(muon4->PT);
        data.muon_eta    ->push_back(muon4->Eta);
        data.muon_phi    ->push_back(muon4->Phi);
        data.muon_ch     ->push_back(muon4->Charge);
        data.dimuon_minv ->push_back(dimuonP4.M());
        data.dimuon_minv ->push_back(dimuonP4_2.M());
        if(abs(dimuonP4.M()-91.0) < abs(dimuonP4_2.M()-91.0)){
          data.leadingm = dimuonP4.M();
          data.subleadingm = dimuonP4_2.M();
        }
        else{
          data.leadingm = dimuonP4_2.M();
          data.subleadingm = dimuonP4.M();
        }
        data.total_minv = totalP4.M();
      }
 
//--------------------------------------------------------------------

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


