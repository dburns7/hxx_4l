#include "TTree.h"

#include "hxx_tree.h"

#include <iostream>

hxx_tree::hxx_tree()
{
   jet_pt     = new std::vector<double>;
   jet_eta    = new std::vector<double>;
   jet_phi    = new std::vector<double>;
   jet_btag   = new std::vector<int>;
   jet_tautag = new std::vector<int>;
}

void hxx_tree::WriteTree(TTree * tree){   
   tree->Branch("sample", &sample);
   tree->Branch("weight", &weight);
   tree->Branch("lepton_flavor", &lepton_flavor);

   // multiplicities
   tree->Branch("nelec", &nelec);
   tree->Branch("nmuon", &nmuon);
   tree->Branch("njet",  &njet);
   
   // dilepton variables:
   tree->Branch("l1_pt",  &l1_pt  );
   tree->Branch("l1_eta", &l1_eta );
   tree->Branch("l1_phi", &l1_phi );
   tree->Branch("l2_pt",  &l2_pt  );
   tree->Branch("l2_eta", &l2_eta );
   tree->Branch("l2_phi", &l2_phi );
   tree->Branch("mll",    &mll    );

   // generator-level variables:
   tree->Branch("gl1_pt",  &gl1_pt  );
   tree->Branch("gl1_eta", &gl1_eta );
   tree->Branch("gl1_phi", &gl1_phi );
   tree->Branch("gl2_pt",  &gl2_pt  );
   tree->Branch("gl2_eta", &gl2_eta );
   tree->Branch("gl2_phi", &gl2_phi );
   tree->Branch("gmll",    &gmll    );


   // met variables:
   tree->Branch("nopu_met",     &nopu_met);
   tree->Branch("nopu_met_phi", &nopu_met_phi);
   
   tree->Branch("ht",     &ht);

   // jets:
   tree->Branch("jet_pt",     jet_pt);
   tree->Branch("jet_eta",    jet_eta);
   tree->Branch("jet_phi",    jet_phi);
   tree->Branch("jet_btag",   jet_btag);
   tree->Branch("jet_tautag", jet_tautag);
}
      
void hxx_tree::ReadTree(TTree * tree){
   tree->SetBranchAddress("l1_pt", &l1_pt     );

   tree->SetBranchAddress("sample", &sample   );
   tree->SetBranchAddress("weight", &weight   );
   tree->SetBranchAddress("lepton_flavor", &lepton_flavor );

   // multiplicities
   tree->SetBranchAddress("nelec", &nelec     );
   tree->SetBranchAddress("nmuon", &nmuon     );
   tree->SetBranchAddress("njet",  &njet      );
   
   // dilepton variables:
   tree->SetBranchAddress("l1_pt",  &l1_pt    );
   tree->SetBranchAddress("l1_eta", &l1_eta   );
   tree->SetBranchAddress("l1_phi", &l1_phi   );
   tree->SetBranchAddress("l2_pt",  &l2_pt    );
   tree->SetBranchAddress("l2_eta", &l2_eta   );
   tree->SetBranchAddress("l2_phi", &l2_phi   );
   tree->SetBranchAddress("mll",    &mll      );

   // generator-level variables:
   tree->SetBranchAddress("gl1_pt",  &gl1_pt    );
   tree->SetBranchAddress("gl1_eta", &gl1_eta   );
   tree->SetBranchAddress("gl1_phi", &gl1_phi   );
   tree->SetBranchAddress("gl2_pt",  &gl2_pt    );
   tree->SetBranchAddress("gl2_eta", &gl2_eta   );
   tree->SetBranchAddress("gl2_phi", &gl2_phi   );
   tree->SetBranchAddress("gmll",    &gmll      );

   // met variables:
   tree->SetBranchAddress("nopu_met",     &nopu_met     );
   tree->SetBranchAddress("nopu_met_phi", &nopu_met_phi );

   tree->SetBranchAddress("ht",           &ht);

   // ROOT makes this complicated...
   delete jet_pt;     jet_pt     = NULL;
   delete jet_phi;    jet_phi    = NULL;
   delete jet_eta;    jet_eta    = NULL;
   delete jet_btag;   jet_btag   = NULL;
   delete jet_tautag; jet_tautag = NULL;

   tree->SetBranchAddress("jet_pt",     &jet_pt     );
   tree->SetBranchAddress("jet_phi",    &jet_phi    );
   tree->SetBranchAddress("jet_eta",    &jet_eta    );
   tree->SetBranchAddress("jet_btag",   &jet_btag   );
   tree->SetBranchAddress("jet_tautag", &jet_tautag );
}

void hxx_tree::erase_jet(int i){
   jet_pt     ->erase(jet_pt->begin() + i);
   jet_eta    ->erase(jet_eta->begin() + i);
   jet_phi    ->erase(jet_phi->begin() + i);
   jet_btag   ->erase(jet_btag->begin() + i);
   jet_tautag ->erase(jet_tautag->begin() + i);
}

void hxx_tree::Clear(){
   sample   = 0;
   weight   = 1;
   weight_met = 0;
   lepton_flavor = 0;
   nelec    = 0;
   nmuon    = 0;
   njet     = 0;
   l1_pt    = 0;
   l1_eta   = 0;
   l1_phi   = 0;
   l2_pt    = 0;
   l2_eta   = 0;
   l2_phi   = 0;
   mll      = 0;
   gl1_pt   = 0;
   gl1_eta  = 0;
   gl1_phi  = 0;
   gl2_pt   = 0;
   gl2_eta  = 0;
   gl2_phi  = 0;
   gmll     = 0;
   nopu_met = 0;
   nopu_met_phi = 0;
   ht = 0;
   jet_pt ->clear();
   jet_eta->clear();
   jet_phi->clear();
   jet_btag->clear();
   jet_tautag->clear();
}
