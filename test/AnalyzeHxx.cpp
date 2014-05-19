#include <iostream>
#include <utility>
#include <vector>
#include <math.h>

#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TRandom.h"
#include "TLorentzVector.h"

#include "koptions.h"
#include "hxx_tree.h"
#include "histogram_manager.h"
#include "cutflow_tool.h"

using namespace std;
using namespace kutil;

void usage(){
   cout << "usage:  AnalyzeHxx  [OPTIONS] <input_file> <output_root> <output_dir>\n";
   cout << "\n";
   cout << "  --met_smear=<x>   : extra amount to smear MET.\n";
   cout << "  --num_smear=<n>   : number of times to smear MET.\n";
   cout << "  --fake_rate=<x>   : apply simple (whole event) fake rate <x> to W+jets.\n";
   cout << "  --8tev            : 8 tev settings (default 14 tev).\n";
   cout << "  --pub_plots       : generate publication plots.\n";
   exit(0);
}

double delta_phi(double phi0, double phi1){
  double dphi = fabs(fabs(fabs(phi1 - phi0) - M_PI) - M_PI);
  //cout << "dphi calc:  " << phi0 << ", " << phi1 << " : " << fabs(phi1 - phi0) << ", " << dphi << "\n";
  return dphi;
}

double min_delta_phi(double phi0, std::vector<double> & vphi, int & index, int skip=-1, int max=0){
  index = 0;
  if (max > vphi.size()) max = vphi.size();
  if (max == 0) max = vphi.size();
  double min_dphi = 1000.0;
  for (int i=0; i<max; i++){
    if (i==skip) continue;
    double phi1 = vphi[i];
    double dphi = fabs(fabs(fabs(phi1 - phi0) - M_PI) - M_PI);
    if (dphi < min_dphi) { index = i; min_dphi = dphi; }
  }
  return min_dphi;
}

double delta_r(double phi0, double eta0, double phi1, double eta1){
  double dphi = fabs(fabs(fabs(phi1 - phi0) - M_PI) - M_PI);
  double deta = fabs(eta1 - eta0);
  return sqrt(dphi*dphi + deta*deta);
}

double min_delta_r(double phi0, double eta0, std::vector<double> & vphi, std::vector<double> & veta, 
		   int & index, int skip=-1, int max=0){
  index = 0;
  if (max > vphi.size()) max = vphi.size();
  if (max == 0) max = vphi.size();
  double min_dr = 1000.0;
  for (int i=0; i<max; i++){
    if (i==skip) continue;
    double phi1 = vphi[i];
    double eta1 = veta[i];
    double dr = delta_r(phi0, eta0, phi1, eta1);
    
    if (dr < min_dr) { index = i; min_dr = dr; }
  }
  return min_dr;
}

void control_vars(hxx_tree & data, double & mjjon){
  mjjon = -1.0;
  while (1) {
    if(data.jet_phi->size() < 2) break;
    if(data.mll > 76) break;
    TLorentzVector vj1, vj2, zjj;
    vj1.SetPtEtaPhiM(data.jet_pt->at(0), data.jet_eta->at(0), data.jet_phi->at(0), 0.0);
    vj2.SetPtEtaPhiM(data.jet_pt->at(1), data.jet_eta->at(1), data.jet_phi->at(1), 0.0);
    zjj = vj1 + vj2;
    mjjon = zjj.M();
    break;
  }
}

void kinematic_vars(hxx_tree & data, double & pt_ja, double &dr_lla, double &dr_llb,
		    double & mjj, double & mjj_off, double & mllj, double & mlljj,
		    double &dphi_zz, double &zll_phi, double &x, double &y){
  TLorentzVector vl1, vl2, vj1, vj2, vja, vjb, zll, zjj, zab, hllj, hlljj;

  if (data.jet_pt->size() < 2) { cout << "ERROR:  jet size less than two!"; return; }

  vl1.SetPtEtaPhiM(data.l1_pt, data.l1_eta, data.l1_phi,0);
  vl2.SetPtEtaPhiM(data.l2_pt, data.l2_eta, data.l2_phi,0);
  vj1.SetPtEtaPhiM(data.jet_pt->at(0), data.jet_eta->at(0), data.jet_phi->at(0), 0.0);
  vj2.SetPtEtaPhiM(data.jet_pt->at(1), data.jet_eta->at(1), data.jet_phi->at(1), 0.0);

  // find jets "a" and "b", two nearest the direction of Z->ll
  zll   = vl1 + vl2;
  int ija, ijb;
  dr_lla = min_delta_r(zll.Phi(), zll.Eta(), *(data.jet_phi), *(data.jet_eta), ija);      
  dr_llb = min_delta_r(zll.Phi(), zll.Eta(), *(data.jet_phi), *(data.jet_eta), ijb, ija);      
  vja.SetPtEtaPhiM(data.jet_pt->at(ija), data.jet_eta->at(ija), data.jet_phi->at(ija), 0.0);
  vjb.SetPtEtaPhiM(data.jet_pt->at(ijb), data.jet_eta->at(ijb), data.jet_phi->at(ijb), 0.0);

  pt_ja = vja.Pt();

  zjj   = vj1 + vj2;
  zab   = vja + vjb;
  hlljj = zll + zab;
  hllj  = zll + vja;

  zll_phi = zll.Phi();
    
  mjj     = zab.M();
  mlljj   = hlljj.M();
  mllj    = hllj.M();
   
  mjj_off = -1.0;
  if (dr_llb < 1.0) mjj_off = mjj;

  dphi_zz = delta_phi(zll.Phi(), zjj.Phi());
  // or:
  //dphi_zz = delta_phi(zll.Phi(), zab.Phi());

   // for testing new variables...
   x = 0.0;
   y = 0.0;

}


void apply_fake_rate(TRandom & rng, hxx_tree & data, double rate){
  data.weight *= rate;

  //if (data.l1_pt <= 0.0) return;
  //if (data.l2_pt > 0.0) return;
  
  int n = data.jet_pt->size();

  if (n <= 0) return;

  int i = (int) (rng.Uniform() * n);
  //cout << i << " of " << n << "\n";

  double pt = data.jet_pt->at(i);

  if (pt > data.l1_pt){
    data.l2_pt  = data.l1_pt;
    data.l2_eta = data.l1_eta;
    data.l2_phi = data.l1_phi;
    data.l1_pt  = pt;
    data.l1_eta = data.jet_eta->at(i);
    data.l1_phi = data.jet_phi->at(i);
  } else {
    data.l2_pt  = pt;
    data.l2_eta = data.jet_eta->at(i);
    data.l2_phi = data.jet_phi->at(i);
  }

  data.erase_jet(i);

  if (data.l2_pt <= 0.0) {
    data.mll = 0.0;
    return;
  }

  TLorentzVector v, v1, v2;
  v1.SetPtEtaPhiM(data.l1_pt, data.l1_eta, data.l1_phi, 0.0);
  v2.SetPtEtaPhiM(data.l2_pt, data.l2_eta, data.l2_phi, 0.0);
  v = v1 + v2;
  data.mll = v.M();
  
  //cout << data.mll << "\n";
  //cout << "   " << data.l1_pt << "\n";
  //cout << "   " << data.l2_pt << "\n";

}


void best_mlj(hxx_tree & data, double & mlj){
      // Investigating new variables here:
      TLorentzVector v, vl1, vj1, vl2, vj2; 

      // could try j1/j2 here too:      
      vl1.SetPtEtaPhiM(data.l1_pt, data.l1_eta, data.l1_phi, 0.0);
      vj1.SetPtEtaPhiM(data.jet_pt->at(0), data.jet_eta->at(0), data.jet_phi->at(0), 0.0);
      vl2.SetPtEtaPhiM(data.l2_pt, data.l2_eta, data.l2_phi, 0.0);
      vj2.SetPtEtaPhiM(data.jet_pt->at(1), data.jet_eta->at(1), data.jet_phi->at(1), 0.0);

      v = vl1 + vj1;
      double m11 = v.M();
      v = vl2 + vj2;
      double m22 = v.M();

      double mina, maxa;
      if (m11 < m22){
         mina = m11;
         maxa = m22;
      } else {
         mina = m22;
         maxa = m11;
      }

      v = vl1 + vj2;
      double m12 = v.M();
      v = vl2 + vj1;
      double m21 = v.M();
      double minb, maxb;
      if (m12 < m21){
         minb = m12;
         maxb = m21;
      } else {
         minb = m21;
         maxb = m12;
      }

      double minmin, minmax, maxmin, maxmax;
      if (mina < minb) {
         minmin = mina;
         maxmin = minb;
      } else {            
         minmin = mina;
         maxmin = minb;
      }
      if (maxa < maxb) {
         minmax = maxa;
         maxmax - maxb;
      } else {            
         minmax = maxa;
         maxmax = maxb;
      }

      mlj = maxmin;
}


double sensitivity(TH1F * hsig, TH1F * hbkg, double sigtot){
  int n = hsig->GetNbinsX() + 2;
  double best_sens = 0.0;
  int ibest = 0;
  for (int i=0; i<n; i++){
    double s = 0;
    double b = 0;
    for (int j=i; j<n; j++){
      s += sigtot * hsig->GetBinContent(i);
      b += hbkg->GetBinContent(i);
    }    
    double sens = 0;
    if (b > 0.0) sens = s / sqrt(b);
    if (sens > best_sens) { 
       best_sens = sens;
       ibest = i;
    }
  }
  cout << "Best MET cut at:  " << hsig->GetBinLowEdge(ibest) << "\n";
  return best_sens;
}


int main(int argc, char *argv[])
{
   TRandom rng;
   rng.SetSeed(2014);
   hxx_tree data;
   
   std::string opt, infile, outroot, outdir;
   koptions options(argc, argv);
   
   //check for the --help option:
   if ( options.find("--help") ) { usage(); }
   //options.set("--seed=", seed);   
   //if (seed > 0) rng.SetSeed(seed);
   double met_smear = 30.0;
   options.set("--met_smear=", met_smear);   
   int    num_smear = 1;
   options.set("--num_smear=", num_smear);   
   double fake_rate = 0.0;
   options.set("--fake_rate=", fake_rate);   
   int mode_8tev = options.find("--8tev");
   int pub_plots = options.find("--pub_plots");



   double lumi = 300;
   double met_cut = 150.0;

   if (mode_8tev){
     lumi = 20.0;
     met_cut = 75;
     cout << "INFO:  settings for sqrt(s) = 8 TeV\n";
   } else {
     cout << "INFO:  settings for sqrt(s) = 14 TeV\n";
   }
   cout << "INFO:  lumi:     " << lumi << "\n";
   cout << "INFO:  MET cut:  " << met_cut << "\n";

   //check for unrecognized options (beginning with -- or -)
   while(options.unrecognized_option(opt)){
      cout << "WARNING: unrecognized option:" << opt << "\n";
      usage();
   } 

   if (options.size() != 3){
      usage();
   }

   options.shift_argument(infile);
   options.shift_argument(outroot);
   options.shift_argument(outdir);

   cout << "INFO: reading analysis tree file:    " << infile  << "\n";
   cout << "INFO: writing histogram root file:   " << outroot << "\n";
   cout << "INFO: writing results to directory:  " << outdir  << "\n";
   cout << "INFO: MET smearing amount is " << met_smear << "\n";
   cout << "INFO: MET smearing number is " << num_smear << "\n";
   cout << "INFO: W+jets fake rate is " << fake_rate << "\n";

   auto_write aw;

   cutflow_tool cutflow;
   histogram_manager h0mll(new TH1F("h0mll","",60,60.0,120.0));

   if (pub_plots) {
     h0mll.add_sample(1,  "_z");
     h0mll.add_sample(2,  "_h");
     h0mll.add_sample(3,  "_w");
     h0mll.add_sample(4,  "_top");

     cutflow.add_sample_name(1, "Z,ZZ,ZW");
     cutflow.add_sample_name(2, "Higgs");
     cutflow.add_sample_name(3, "W,WW");
     cutflow.add_sample_name(4, "tt");     
   } else {
     h0mll.add_sample(1,  "_zz");
     h0mll.add_sample(2,  "_hzz");
     h0mll.add_sample(3,  "_zh_zvv");
     h0mll.add_sample(4,  "_zh_zll");
     h0mll.add_sample(5,  "_wh");
     //h0mll.add_sample(6,  "_zh");
     //h0mll.add_sample(7,  "_wh");
     //h0mll.add_sample(8,  "_wjjj");

     cutflow.add_sample_name(1, "ZZ");
     cutflow.add_sample_name(2, "HZZ");
     cutflow.add_sample_name(3, "ZHvv");
     cutflow.add_sample_name(4, "ZHll");
     cutflow.add_sample_name(5, "WH");
     //cutflow.add_sample_name(6, "ZH");
     //cutflow.add_sample_name(7, "WH");
     //cutflow.add_sample_name(8, "W+jets");
   }




   h0mll.add_sample(20,  "_hxx1");
   //h0mll.add_sample(21,  "_hxx10");
   h0mll.add_sample(22,  "_hxx100");
   //h0mll.add_sample(23,  "_hxx500");
   //h0mll.add_sample(24,  "_hxx1000");
   
   cutflow.add_sample_name(20, "HXX1");
   //cutflow.add_sample_name(21, "HXX10");
   cutflow.add_sample_name(22, "HXX100");
   //cutflow.add_sample_name(23, "HXX500");
   //cutflow.add_sample_name(24, "HXX1000");
/*

   h0mll.add_sample(30,  "_hzp_100_1");
   h0mll.add_sample(31,  "_hzp_100_10");
   h0mll.add_sample(32,  "_hzp_100_100");
   h0mll.add_sample(33,  "_hzp_100_500");
   h0mll.add_sample(34,  "_hzp_100_1000");
   h0mll.add_sample(35,  "_hzp_1000_1");
   h0mll.add_sample(36,  "_hzp_1000_10");
   h0mll.add_sample(37,  "_hzp_1000_100");
   h0mll.add_sample(38,  "_hzp_1000_500");
   h0mll.add_sample(39,  "_hzp_1000_1000");
   
   cutflow.add_sample_name(30, "HZP_100_1");
   cutflow.add_sample_name(31, "HZP_100_10");
   cutflow.add_sample_name(32, "HZP_100_100");
   cutflow.add_sample_name(33, "HZP_100_500");
   cutflow.add_sample_name(34, "HZP_100_1000");
   cutflow.add_sample_name(35, "HZP_1000_1");
   cutflow.add_sample_name(36, "HZP_1000_10");
   cutflow.add_sample_name(37, "HZP_1000_100");
   cutflow.add_sample_name(38, "HZP_1000_500");
   cutflow.add_sample_name(39, "HZP_1000_1000");


   h0mll.add_sample(40,  "_hzpzp_100_1");
   h0mll.add_sample(41,  "_hzpzp_100_10");
   h0mll.add_sample(42,  "_hzpzp_100_100");
   h0mll.add_sample(43,  "_hzpzp_100_500");
   h0mll.add_sample(44,  "_hzpzp_100_1000");
   h0mll.add_sample(45,  "_hzpzp_1000_1");
   h0mll.add_sample(46,  "_hzpzp_1000_10");
   h0mll.add_sample(47,  "_hzpzp_1000_100");
   h0mll.add_sample(48,  "_hzpzp_1000_500");
   h0mll.add_sample(49,  "_hzpzp_1000_1000");
   
   cutflow.add_sample_name(40, "HZPZP_100_1");
   cutflow.add_sample_name(41, "HZPZP_100_10");
   cutflow.add_sample_name(42, "HZPZP_100_100");
   cutflow.add_sample_name(43, "HZPZP_100_500");
   cutflow.add_sample_name(44, "HZPZP_100_1000");
   cutflow.add_sample_name(45, "HZPZP_1000_1");
   cutflow.add_sample_name(46, "HZPZP_1000_10");
   cutflow.add_sample_name(47, "HZPZP_1000_100");
   cutflow.add_sample_name(48, "HZPZP_1000_500");
   cutflow.add_sample_name(49, "HZPZP_1000_1000");
*/   
   
   h0mll.add_auto_write(aw);

/*
   TH1F hfit_bkg("sample_1","",  100,0.0,300.0);
   TH1F hfit_sig20("signal20","",  100,0.0,300.0);
   TH1F hfit_sig21("signal21","",  100,0.0,300.0);
   TH1F hfit_sig22("signal22","",  100,0.0,300.0);
   TH1F hfit_sig23("signal23","",  100,0.0,300.0);
   TH1F hfit_sig24("signal24","",  100,0.0,300.0);

   int    nsig = 0;
   double wsig = 0.0;
   hfit_bkg.Sumw2();
   hfit_sig20.Sumw2();
   hfit_sig21.Sumw2();
   hfit_sig22.Sumw2();
   hfit_sig23.Sumw2();
   hfit_sig24.Sumw2();

   //control plots (no pre-selection)
   histogram_manager hcnjet  (new TH1F("hcnjet",  "", 10,  0.0, 10.0),     h0mll, aw);
   histogram_manager hcmjjon (new TH1F("hcmjjon", "", 75,  0.0, 150.0),    h0mll, aw);
   histogram_manager hcmjjoff(new TH1F("hcmjjoff","", 100, 0.0, 500.0),    h0mll, aw);

   //test new variables
   histogram_manager htestx  (new TH1F("htestx",  "", 25,  0.0, 3.2),      h0mll, aw);
   histogram_manager htesty  (new TH1F("htesty",  "", 25,  0.0, 200.0),    h0mll, aw);

   //histograms at stage 0
   histogram_manager h0mjj   (new TH1F("h0mjj",   "", 100, 0.0,  500.0),   h0mll, aw);
   histogram_manager h0mlj   (new TH1F("h0mlj",   "", 100, 0.0,  500.0),   h0mll, aw);
   histogram_manager h0mlljj (new TH1F("h0mlljj", "", 100, 0.0,  500.0),  h0mll, aw);
   histogram_manager h0mllj  (new TH1F("h0mllj",  "", 100, 0.0,  500.0),   h0mll, aw);
   histogram_manager h0met   (new TH1F("h0met",   "", 100, 0.0,  300.0),   h0mll, aw);
   histogram_manager h0njet  (new TH1F("h0njet",  "", 10,  0.0,  10.0),    h0mll, aw);
   histogram_manager h0nbjet (new TH1F("h0nbjet", "", 8,   0.0,  8.0),     h0mll, aw);
   histogram_manager h0jdphi (new TH1F("h0jdphi", "", 100, 0.0,  3.2),     h0mll, aw);   
   histogram_manager h0ptja  (new TH1F("h0ptja","", 32, 0.0,  300.0),   h0mll, aw);
   histogram_manager h0drlla (new TH1F("h0drlla","", 81,  0.0,  8.1),     h0mll, aw);
   histogram_manager h0drllb (new TH1F("h0drllb","", 81,  0.0,  8.1),     h0mll, aw);
   histogram_manager h0dphizz     (new TH1F("h0dphizz",     "", 32, 0.0, 3.2), h0mll, aw);
   histogram_manager h0dphizllmet (new TH1F("h0dphizllmet", "", 32, 0.0, 3.2), h0mll, aw);


   histogram_manager h0ht   (new TH1F("h0ht",    "", 100, 0.0, 500.0), h0mll, aw);
   histogram_manager h0l1pt (new TH1F("h0l1pt",  "", 100, 0.0, 300.0), h0mll, aw);
   histogram_manager h0l2pt (new TH1F("h0l2pt",  "", 100, 0.0, 300.0), h0mll, aw);
   histogram_manager h0j1pt (new TH1F("h0j1pt",  "", 100, 0.0, 300.0), h0mll, aw);
   histogram_manager h0j2pt (new TH1F("h0j2pt",  "", 100, 0.0, 300.0), h0mll, aw);

   histogram_manager h0l1eta(new TH1F("h0l1eta","", 60, -3.0, 3.0), h0mll, aw);
   histogram_manager h0l2eta(new TH1F("h0l2eta","", 60, -3.0, 3.0), h0mll, aw);
   histogram_manager h0j1eta(new TH1F("h0j1eta","", 60, -3.0, 3.0), h0mll, aw);
   histogram_manager h0j2eta(new TH1F("h0j2eta","", 60, -3.0, 3.0), h0mll, aw);

   //histograms at stage 1
   histogram_manager h1mll   (new TH1F("h1mll",   "", 60, 60.0,  120.0),   h0mll, aw);
   histogram_manager h1mjj   (new TH1F("h1mjj",   "", 100, 0.0,  500.0),   h0mll, aw);
   histogram_manager h1mlj   (new TH1F("h1mlj",   "", 100, 0.0,  500.0),   h0mll, aw);
   histogram_manager h1mlljj (new TH1F("h1mlljj", "", 100, 0.0,  500.0),  h0mll, aw);
   histogram_manager h1mllj  (new TH1F("h1mllj",  "", 100, 0.0,  500.0),   h0mll, aw);
   histogram_manager h1met   (new TH1F("h1met",   "", 100, 0.0,  300.0),   h0mll, aw);
   histogram_manager h1njet  (new TH1F("h1njet",  "", 10,  0.0,  10.0),    h0mll, aw);
   histogram_manager h1nbjet (new TH1F("h1nbjet", "", 8,   0.0,  8.0),     h0mll, aw);
   histogram_manager h1jdphi (new TH1F("h1jdphi", "", 100, 0.0,  3.2),     h0mll, aw);   
   histogram_manager h1ptja  (new TH1F("h1ptja","", 100, 0.0,  300.0),   h0mll, aw);
   histogram_manager h1drlla (new TH1F("h1drlla","", 81,  0.0,  8.1),     h0mll, aw);
   histogram_manager h1drllb (new TH1F("h1drllb","", 81,  0.0,  8.1),     h0mll, aw);
   histogram_manager h1dphizz     (new TH1F("h1dphizz",     "", 100, 0.0, 3.2), h0mll, aw);
   histogram_manager h1dphizllmet (new TH1F("h1dphizllmet", "", 100, 0.0, 3.2), h0mll, aw);
*/
   cout << "INFO: opening file: " << infile << "\n";

   TFile * file = new TFile(infile.c_str());
   TTree * tree = (TTree*) file->Get("hxxtree");

   if (! tree) {
      cout << "ERROR:  could not open tree.\n";
      exit(0);
   }

   data.ReadTree(tree);
   long int numberOfEntries = tree->GetEntries();

   //tree->Print();
  
   int count = 0;
   int nupdate = numberOfEntries / 20;
   if (nupdate < 1) nupdate=1;
   cout << "PROGRESS:  ";

   int prescale  = 0; 
   int iprescale = 0;
   // Loop over all events
   for(Int_t entry = 0; entry < numberOfEntries; ++entry)
   {
      count++;
      if (count % nupdate == 0) { cout << "."; cout.flush(); }

      if (iprescale < prescale){
         iprescale++;
         continue;
      } else {
         iprescale = 0;
      }
      
      tree->GetEntry(entry);

/*
      int num_smear_event = num_smear;
      if (data.sample == 1) {
         num_smear_event = num_smear * 10;
      }
      //if (data.sample == 4) {
      //num_smear_event = num_smear * 10;
      //}
      //if (data.sample == 3) {
      //num_smear_event = num_smear * 10;
      //}


      // First, apply W+jets fake rate if applicable:
      if (fake_rate > 0.0 && data.sample == 8){
	apply_fake_rate(rng, data, fake_rate);
      }

      // remap sample id for publication plots:
      if (pub_plots) {
	switch (data.sample){
	case 1: data.sample = 1; break;
	case 2: data.sample = 1; break;
	case 3: data.sample = 3; break;
	case 4: data.sample = 4; break;
	case 5: data.sample = 2; break;
	case 6: data.sample = 2; break;
	case 7: data.sample = 2; break;     
	case 8: data.sample = 3; break;     
	}
      }


      double mjjon;
      control_vars(data, mjjon);
      if (mjjon > 0.0) hcmjjon.Fill(data.sample, mjjon, data.weight);
      hcnjet.Fill(data.sample, data.jet_pt->size(), data.weight);


      //if (data.l1_pt < 12.0) continue;
      //if (data.l2_pt < 12.0) continue;

      //if (data.l1_pt < 20.0) continue;
      //if (data.l2_pt < 15.0) continue;

      // prune jet list according to analysis selection:
      for (int i=0; i<data.jet_pt->size(); i++){
         int veto = 0;
         if (data.jet_pt->at(i) < 15.0) veto = 1;
         if (fabs(data.jet_eta->at(i)) > 2.5) veto = 1;         
         if (veto) {
            data.erase_jet(i); 
            i--;
         }
      }
      if (data.jet_pt->size() != data.jet_btag->size()){
         cout << "ERROR:  Jet vectors out of sync.  Something is wrong.\n";
         exit(0);
      }

      int nbtag = 0;
      for (int i=0; i<data.jet_btag->size(); i++){
         if (data.jet_btag->at(i)) nbtag++;
      }

      //preselection cuts:
      //if (data.jet_pt->size() < 2) continue;
      //if (data.nmuon + data.nelec > 2) continue;
      

      // checking 2-jet bin:
      //if (data.jet_pt->size() > 2) continue;
*/
      cutflow.increment(0, data.sample, data.weight);      

/*
      double mlj, pt_ja, dr_lla, dr_llb, mjj, mjj_off, mllj, mlljj, dphi_zz, zll_phi, x, y;
      best_mlj(data, mlj);
      kinematic_vars(data, pt_ja, dr_lla, dr_llb, mjj, mjj_off, mllj, mlljj, dphi_zz, zll_phi, x, y);

      if (mjj_off > 0.0) { hcmjjoff.Fill(data.sample, mjj_off, data.weight); }
		   
      // Here we smear the MET to account for pile-up effects:
      vector<double> met;
      vector<double> met_phi;
      vector<double> jet_dphi;
      vector<double> zllmet_dphi;

      // use reduced weight when looping over entire MET vector:
      double met_weight = (data.weight / (double) num_smear_event);
      double nopu_metx = data.nopu_met * cos(data.nopu_met_phi);
      double nopu_mety = data.nopu_met * sin(data.nopu_met_phi);
      
      for (int i=0; i<num_smear_event; i++){
         double metx            = nopu_metx + rng.Gaus() * met_smear;
         double mety            = nopu_mety + rng.Gaus() * met_smear;
         double new_met         = sqrt(metx*metx + mety*mety);
         double new_met_phi     = atan2(mety, metx);
	 int min_i;
         double new_jet_dphi    = min_delta_phi(new_met_phi, *data.jet_phi, min_i,-1,1);
         double new_zllmet_dphi = delta_phi(new_met_phi, zll_phi);
         met.push_back(new_met);
         met_phi.push_back(new_met_phi);
         jet_dphi.push_back(new_jet_dphi);
         zllmet_dphi.push_back(new_zllmet_dphi);
      }

      // high MET test:
      //if (data.nopu_met < 100.0) continue;
*/      
      h0mll.Fill(data.sample, data.mll, data.weight);



/*
      if (data.jet_pt->size() > 0) {
         h0j1pt  .Fill(data.sample, data.jet_pt ->at(0), data.weight);
         h0j1eta .Fill(data.sample, data.jet_eta->at(0), data.weight);
      }
      if (data.jet_pt->size() > 1) {
         h0j2pt  .Fill(data.sample, data.jet_pt ->at(1), data.weight);
         h0j2eta .Fill(data.sample, data.jet_eta->at(1), data.weight);
      }
      h0l1pt  .Fill(data.sample, data.l1_pt,  data.weight);
      h0l1eta .Fill(data.sample, data.l1_eta, data.weight);
      h0l2pt  .Fill(data.sample, data.l2_pt,  data.weight);
      h0l2eta .Fill(data.sample, data.l2_eta, data.weight);

      h0njet.Fill(data.sample, data.jet_pt->size(), data.weight);
      h0nbjet.Fill(data.sample, nbtag, data.weight);

      h0mjj.Fill(data.sample, mjj, data.weight);      
      h0mlj.Fill(data.sample, mlj, data.weight);      
      h0mlljj.Fill(data.sample, mlljj, data.weight);      
      h0mllj.Fill(data.sample, mllj, data.weight);      
      h0ht.Fill(data.sample, data.ht, data.weight);      
      h0dphizz .Fill(data.sample, dphi_zz, data.weight);

      h0ptja   .Fill(data.sample, pt_ja,  data.weight);
      h0drlla  .Fill(data.sample, dr_lla, data.weight);
      h0drllb  .Fill(data.sample, dr_llb, data.weight);




      htestx .Fill(data.sample, x);
      htesty .Fill(data.sample, y);

      for (int i=0; i<met.size(); i++) {
         h0met        .Fill(data.sample, met[i],          met_weight);
         h0jdphi      .Fill(data.sample, jet_dphi[i],     met_weight);
         h0dphizllmet .Fill(data.sample, zllmet_dphi[i] , met_weight);      
      }
*/


      // Z-peak:
      //if (data.mll < 82)  continue;
      //if (data.mll > 98) continue;
      cutflow.increment(1, data.sample, data.weight);
      //if (mllj > 124.0)  continue;
      cutflow.increment(2, data.sample, data.weight);
      //if (dphi_zz > 2.25)  continue;
      cutflow.increment(3, data.sample, data.weight);
      //if (mlj > 60.0)  continue;
      // These two cuts are applied in MET loop... 0 = no cut
      double jet_dphi_cut = 0.0; 
      double zllmet_dphi_cut = 0.0;
      //if (mlj > 120.0)  continue;
      //





/*
      h1njet.  Fill(data.sample, data.jet_pt->size(), data.weight);
      h1mll.   Fill(data.sample, data.mll,            data.weight);
      h1mjj.   Fill(data.sample, mjj,                 data.weight);      
      h1mlj.   Fill(data.sample, mlj,                 data.weight);      
      h1mlljj. Fill(data.sample, mlljj,               data.weight);      
      h1mllj.  Fill(data.sample, mllj,                data.weight);      
      h1ptja.  Fill(data.sample, pt_ja,  data.weight);
      h1drlla. Fill(data.sample, dr_lla, data.weight);
      h1drllb. Fill(data.sample, dr_llb, data.weight);      


      for (int i=0; i<met.size(); i++) {
         double tmet =met[i];
         if (tmet > 299.0) tmet = 299.;

         h1jdphi .Fill(data.sample, jet_dphi[i], met_weight);         
         h1dphizllmet .Fill(data.sample, zllmet_dphi[i] , met_weight);
         if (jet_dphi[i]   < jet_dphi_cut) continue; // APPEARS IN TWO PLACES
         if (zllmet_dphi[i] < zllmet_dphi_cut) continue; // APPEARS IN TWO PLACES         
         h1met.Fill(data.sample, tmet, met_weight);
      }

      // fill limit setting histograms:
      for (int i=0; i<met.size(); i++){
         if (met_weight > 100){
            cout << "WARNING:  large weight at MET stage:  " << met_weight << "\n";  
         }
         double tmet =met[i];
         if (tmet > 299.0) tmet = 299.;

         if (jet_dphi[i]   < jet_dphi_cut) continue; // APPEARS IN TWO PLACES
         if (zllmet_dphi[i] < zllmet_dphi_cut) continue; // APPEARS IN TWO PLACES         
         if (data.sample < 20)  hfit_bkg.Fill(tmet, met_weight);
         if (data.sample == 20) hfit_sig20.Fill(tmet, met_weight);         
         if (data.sample == 21) hfit_sig21.Fill(tmet, met_weight);         
         if (data.sample == 22) hfit_sig22.Fill(tmet, met_weight);         
         if (data.sample == 23) hfit_sig23.Fill(tmet, met_weight);         
         if (data.sample == 24) hfit_sig24.Fill(tmet, met_weight);         

	 if (met[i] > met_cut)	 
	   cutflow.increment(4, data.sample, met_weight);
      }      
*/
   }
   cout << "\n";

   cout << "SUMMARY:  read " << count << " of " << tree->GetEntries() << " events from analysis tree.\n";
   
   TFile * foutroot = new TFile(outroot.c_str(), "RECREATE");
   foutroot->cd();
   aw.WriteAll();
   foutroot->Close();

   //cout << "SUMMARY:  done writing files.\n";

   cout << "Cutflow:  Stage 0 (lljj preselection)\n";
   cutflow.print(0);

   cout << "Cutflow:  Stage 1 (mll cut)\n";
   cutflow.print(1);

   cout << "Cutflow:  Stage 2 (after mjj cut) \n";
   cutflow.print(2);

   cout << "Cutflow:  Stage 3 (after mjjll cut) \n";
   cutflow.print(3);

   cout << "Cutflow:  Stage 4 (after MET cut) \n";
   cutflow.print(4);
/*
   cout << "Fit Histogram Summary:  \n";
   double SIGTOT = lumi * 149.8;
   hfit_sig20.Scale(1.0/SIGTOT);
   hfit_sig21.Scale(1.0/SIGTOT);
   hfit_sig22.Scale(1.0/SIGTOT);
   hfit_sig23.Scale(1.0/SIGTOT);
   hfit_sig24.Scale(1.0/SIGTOT);
   cout << " -> using total signal events of :    " << SIGTOT << "\n";
   cout << " -> background integral (evts):       " << hfit_bkg.GetSumOfWeights() << "\n";
   cout << " -> signal 1    integral (eff):       " << hfit_sig20.GetSumOfWeights() << "\n";
   cout << " -> signal 10   integral (eff):       " << hfit_sig21.GetSumOfWeights() << "\n";
   cout << " -> signal 100  integral (eff):       " << hfit_sig22.GetSumOfWeights() << "\n";
   cout << " -> signal 500  integral (eff):       " << hfit_sig23.GetSumOfWeights() << "\n";
   cout << " -> signal 1000 integral (eff):       " << hfit_sig24.GetSumOfWeights() << "\n";
   cout << " -> local count of signal events:     " << nsig << "\n";
   cout << " -> local integral of signal weight:  " << wsig << "\n";

   cout << " --> signal 1    sensitivity:  " << sensitivity(&hfit_sig20, &hfit_bkg, SIGTOT) << "\n";
   cout << " --> signal 10   sensitivity:  " << sensitivity(&hfit_sig21, &hfit_bkg, SIGTOT) << "\n";
   cout << " --> signal 100  sensitivity:  " << sensitivity(&hfit_sig22, &hfit_bkg, SIGTOT) << "\n";
   cout << " --> signal 500  sensitivity:  " << sensitivity(&hfit_sig23, &hfit_bkg, SIGTOT) << "\n";
   cout << " --> signal 1000 sensitivity:  " << sensitivity(&hfit_sig24, &hfit_bkg, SIGTOT) << "\n";

   char name[100];
   TFile * f = NULL;
   TH1F * h = NULL;

   sprintf(name, "%s/mchi1.root", outdir.c_str());
   f = new TFile(name, "RECREATE");
   f->cd();
   h = (TH1F *) hfit_sig20.Clone("signal");
   h->Write();
   hfit_bkg.Write();
   f->Close();

   sprintf(name, "%s/mchi10.root", outdir.c_str());
   f = new TFile(name, "RECREATE");
   f->cd();
   h = (TH1F *) hfit_sig21.Clone("signal");
   h->Write();
   hfit_bkg.Write();
   f->Close();

   sprintf(name, "%s/mchi100.root", outdir.c_str());
   f = new TFile(name, "RECREATE");
   f->cd();
   h = (TH1F *) hfit_sig22.Clone("signal");
   h->Write();
   hfit_bkg.Write();
   f->Close();

   sprintf(name, "%s/mchi500.root", outdir.c_str());
   f = new TFile(name, "RECREATE");
   f->cd();
   h = (TH1F *) hfit_sig23.Clone("signal");
   h->Write();
   hfit_bkg.Write();
   f->Close();

   sprintf(name, "%s/mchi1000.root", outdir.c_str());
   f = new TFile(name, "RECREATE");
   f->cd();
   h = (TH1F *) hfit_sig24.Clone("signal");
   h->Write();
   hfit_bkg.Write();
   f->Close();
*/


}


