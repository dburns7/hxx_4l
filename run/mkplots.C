
enum {MYCYAN1     = kCyan-4};
enum {MYCYAN2     = kCyan-3};
enum {MYGREEN1    = kGreen-4};
enum {MYGREEN2    = kGreen-3};
enum {MYORANGE1   = kOrange-4};
enum {MYORANGE2   = kOrange-3};
enum {MYMAGENTA1  = kMagenta};
enum {MYMAGENTA2  = kMagenta+2};
enum {MYBLUE1    = kBlue};
enum {MYBLUE2    = kBlue};

enum {MYRED1     = kRed};
enum {MYRED2     = kRed-2};

enum {FILL1 = 3004, FILL2 = 3013, FILL3 = 3005, FILL4=3014, FILL4 = 3007}; 
enum {FILLSIG1 = 3002, FILLSIG2 = 3003};

enum {LSOLID=1, LDASHED=2};


TH1F * AddHist(THStack *stack, TLegend * leg,  TFile *f, const char * name, const char * sample, const char * legsamp, int style, int color){
   char full_name[200];
   sprintf(full_name, "%s_%s", name, sample);
   TH1F * h = f->Get(full_name);
   if (h){
      h->SetFillStyle(style);
      h->SetLineColor(color);
      h->SetFillColor(color);
      if (leg) { leg->AddEntry(h, legsamp, "f"); }
      stack->Add(h);
   }
   return h;
}

THStack * GetBkgStack(TFile *f, const char * name, const char * xtitle, TLegend * leg, int mode==0){
   char full_name[200];

   if (leg) {
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
   }

   sprintf(full_name, "stack_%s", name);
   THStack * stack = new THStack(full_name, "");

   TH1F * h_base = f->Get(name);
   h_base->SetXTitle(xtitle);
   stack->SetHistogram(h_base);

   if (mode == 0){
      AddHist(stack, leg, f,name,"wh",    "WH",     FILL4, MYMAGENTA2);
      AddHist(stack, leg, f,name,"zh",    "ZH",     FILL3, MYGREEN2);
      AddHist(stack, leg, f,name,"hzz",   "H(ZZ)",  FILL1, MYORANGE1);
      AddHist(stack, leg, f,name,"zz_zw", "ZZ,ZW",  FILL4, MYBLUE1);
      AddHist(stack, leg, f,name,"ww",    "WWjj",   FILL2, MYCYAN2);
      AddHist(stack, leg, f,name,"wjjj",  "W+jets", FILL2, MYGREEN1);   
      AddHist(stack, leg, f,name,"tt",    "top",    FILL3, MYMAGENTA1);
      AddHist(stack, leg, f,name,"zjj",   "Zjj",    FILL1, MYCYAN1);   
   }
   return stack;
}

TH1F * GetSigHist(TFile *f, const char * name, const char * tag, int color, int lstyle, TLegend * leg, const char * legtag){
   char full_name[200];

   sprintf(full_name, "%s_%s", name, tag);
   TH1F * h = f->Get(full_name);

   h->SetFillStyle(0);
   h->SetFillColor(color);
   h->SetLineColor(color);   
   h->SetLineStyle(lstyle);   
   if (leg) { leg->AddEntry(h, legtag, "l"); }  

   return h;
}

mkplots(){
   gStyle->SetOptStat(0);

   double basemin = 0.001;

   TFile * f = new TFile("latest.root");
   f->ls();

   TCanvas * c1 = new TCanvas("c1", "");
   TLegend * leg1 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c1->SetLogy();
   c1->cd();
   THStack * hmll_bkg = GetBkgStack(f, "h0mll", "dilepton invariant mass [GeV]", leg1);
   TH1F    * hmll_sig1 = GetSigHist(f, "h0mll", "hxx1", MYRED1, LSOLID, leg1, "HXX (1 GeV)");
   TH1F    * hmll_sig2 = GetSigHist(f, "h0mll", "hxx1000", MYRED1, LDASHED, leg1, "HXX (1000 GeV)");
   hmll_bkg->SetMinimum(basemin);
   hmll_bkg->Draw();
   hmll_sig1->Draw("HSAME");
   hmll_sig2->Draw("HSAME");
   leg1->Draw();


   TCanvas * c2 = new TCanvas("c2", "");
   TLegend * leg2 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c2->SetLogy();
   c2->cd();
   THStack * hmjj_bkg = GetBkgStack(f, "h0mjj", "best dijet mass [GeV]", leg2);
   TH1F    * hmjj_sig1 = GetSigHist (f, "h0mjj", "hxx1", MYRED1, LSOLID, leg2, "HXX (1 GeV)");
   TH1F    * hmjj_sig2 = GetSigHist (f, "h0mjj", "hxx1000", MYRED1, LDASHED, leg2, "HXX (1000 GeV)");
   hmjj_bkg->SetMinimum(basemin);
   hmjj_bkg->Draw();
   hmjj_sig1->Draw("HSAME");
   hmjj_sig2->Draw("HSAME");
   leg2->Draw();

   TCanvas * c3 = new TCanvas("c3", "");
   TLegend * leg3 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c3->SetLogy();
   c3->cd();
   THStack * hmjjll_bkg = GetBkgStack(f, "h0mjjll", "M(jjll) [GeV]", leg3);
   TH1F    * hmjjll_sig1 = GetSigHist   (f, "h0mjjll", "hxx1", MYRED1, LSOLID, leg3, "HXX (1 GeV)");
   TH1F    * hmjjll_sig2 = GetSigHist (f, "h0mjjll", "hxx100", MYRED1, LDASHED, leg3, "HXX (100 GeV)");
   hmjjll_bkg->SetMinimum(basemin);
   hmjjll_bkg->Draw();
   hmjjll_sig1->Draw("HSAME");   
   hmjjll_sig2->Draw("HSAME");   
   leg3->Draw();

   TCanvas * c4 = new TCanvas("c4", "");
   TLegend * leg4 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c4->SetLogy();
   c4->cd();
   THStack * hmet_bkg = GetBkgStack(f, "h1met", "MET [GeV]", leg4);
   TH1F    * hmet_sig1 = GetSigHist(f, "h1met", "hxx1", MYRED1, LSOLID, leg4, "HXX (1 GeV)");
   TH1F    * hmet_sig2 = GetSigHist (f, "h1met", "hxx1000", MYRED1, LDASHED, leg4, "HXX (1000 GeV)");
   hmet_bkg->SetMinimum(basemin);
   hmet_bkg->Draw();
   hmet_sig1->Draw("HSAME");
   hmet_sig2->Draw("HSAME");
   leg4->Draw();

   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c5->SetLogy();
   c5->cd();
   THStack * hmjl_bkg = GetBkgStack(f, "h0mjl", "M(jl) [GeV]", leg5);
   TH1F    * hmjl_sig1 = GetSigHist (f, "h0mjl", "hxx1", MYRED1, LSOLID, leg5, "HXX (1 GeV)");
   TH1F    * hmjl_sig2 = GetSigHist (f, "h0mjl", "hxx1000", MYRED1, LDASHED, leg5, "HXX (1000 GeV)");
   hmjl_bkg->SetMinimum(basemin);
   hmjl_bkg->Draw();
   hmjl_sig1->Draw("HSAME");   
   hmjl_sig2->Draw("HSAME");   
   leg5->Draw();

   TCanvas * ctest = new TCanvas("ctest", "");
   TLegend * legtest = new TLegend(0.7, 0.65, 0.90, 0.90);
   ctest->SetLogy();
   ctest->cd();
   THStack * htest_bkg  = GetBkgStack(f, "h0jdphi", "test variable [GeV]", legtest);
   TH1F    * htest_sig1 = GetSigHist (f, "h0jdphi", "hxx1", MYRED1, LSOLID, legtest, "HXX (1 GeV)");
   TH1F    * htest_sig2 = GetSigHist (f, "h0jdphi", "hxx1000", MYRED1, LDASHED, legtest, "HXX (1000 GeV)");
   htest_bkg->SetMinimum(basemin);
   htest_bkg->Draw();
   htest_sig1->Draw("HSAME");
   htest_sig2->Draw("HSAME");
   legtest->Draw();

   return;

   TCanvas * ctest = new TCanvas("ctest", "");
   TLegend * legtest = new TLegend(0.7, 0.65, 0.90, 0.90);
   ctest->SetLogy();
   ctest->cd();
   THStack * htest_bkg  = GetBkgStack(f, "h0j2pt", "test variable [GeV]", legtest);
   TH1F    * htest_sig1 = GetSigHist (f, "h0j2pt", "hxx1", MYRED1, LSOLID, legtest, "HXX (1 GeV)");
   TH1F    * htest_sig2 = GetSigHist (f, "h0j2pt", "hxx1000", MYRED1, LDASHED, legtest, "HXX (1000 GeV)");
   htest_bkg->SetMinimum(basemin);
   htest_bkg->Draw();
   htest_sig1->Draw("HSAME");
   htest_sig2->Draw("HSAME");
   legtest->Draw();



   
   

   TCanvas * ctest = new TCanvas("ctest", "");
   TLegend * legtest = new TLegend(0.7, 0.65, 0.90, 0.90);
   ctest->SetLogy();
   ctest->cd();
   THStack * htest_bkg  = GetBkgStack(f, "htestx", "test variable [GeV]", legtest);
   TH1F    * htest_sig1 = GetSigHist (f, "htestx", "hxx1", MYRED1, LSOLID, legtest, "HXX (1 GeV)");
   TH1F    * htest_sig2 = GetSigHist (f, "htestx", "hxx1000", MYRED1, LDASHED, legtest, "HXX (1000 GeV)");
   htest_bkg->SetMinimum(basemin);
   htest_bkg->Draw();
   htest_sig1->Draw("HSAME");
   htest_sig2->Draw("HSAME");
   legtest->Draw();


   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c5->SetLogy();
   c5->cd();
   THStack * hnjet_bkg = GetBkgStack(f, "h1njet", "Num Jet", leg5);
   TH1F    * hnjet_sig = GetSigHist(f, "h1njet", "hxx1", MYRED1, LSOLID, leg5, "HXX (1 GeV)");
   hnjet_bkg->SetMinimum(basemin);
   hnjet_bkg->Draw();
   hnjet_sig->Draw("HSAME");
   leg5->Draw();

   TCanvas * c6 = new TCanvas("c6", "");
   TLegend * leg6 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c6->SetLogy();
   c6->cd();
   THStack * hnbjet_bkg = GetBkgStack(f, "h0nbjet", "Num B-Tags", leg6);
   TH1F    * hnbjet_sig = GetSigHist(f, "h0nbjet", "hxx1", MYRED1, LSOLID, leg6, "HXX (1 GeV)");
   hnbjet_bkg->SetMinimum(basemin);
   hnbjet_bkg->Draw();
   hnbjet_sig->Draw("HSAME");
   leg6->Draw();




   return;

/*

   TCanvas * c3 = new TCanvas("c3", "");
   TLegend * leg3 = new TLegend(0.65, 0.65, 0.90, 0.90);
   c3->Divide(2,2);

   c3->cd(1);
   c3->GetPad(1)->SetLogy();
   THStack * hj1pt_bkg = GetBkgStack(f, "hj1pt", "Jet 1 pT [GeV]", NULL);
   hj1pt_bkg->SetMinimum(basemin);
   hj1pt_bkg->Draw();

   c3->cd(2);
   c3->GetPad(2)->SetLogy();
   THStack * hj2pt_bkg = GetBkgStack(f, "hj2pt", "Jet 2 pT [GeV]", leg3);
   hj2pt_bkg->SetMinimum(basemin);
   hj2pt_bkg->Draw();
   leg3->Draw();

   c3->cd(3);
   c3->GetPad(3)->SetLogy();
   THStack * hj1eta_bkg = GetBkgStack(f, "hj1eta", "Jet 1 eta [GeV]", NULL);
   hj1eta_bkg->SetMinimum(basemin);
   hj1eta_bkg->Draw();

   c3->cd(4);
   c3->GetPad(4)->SetLogy();
   THStack * hj2eta_bkg = GetBkgStack(f, "hj2eta", "Jet 2 eta [GeV]", NULL);
   hj2eta_bkg->SetMinimum(basemin);
   hj2eta_bkg->Draw();



   TCanvas * c4 = new TCanvas("c4", "");
   TLegend * leg3 = new TLegend(0.65, 0.65, 0.90, 0.90);
   c4->Divide(2,2);

   c4->cd(1);
   c4->GetPad(1)->SetLogy();
   THStack * hl1pt_bkg = GetBkgStack(f, "hl1pt", "Lept 1 pT [GeV]", NULL);
   hl1pt_bkg->SetMinimum(basemin);
   hl1pt_bkg->Draw();

   c4->cd(2);
   c4->GetPad(2)->SetLogy();
   THStack * hl2pt_bkg = GetBkgStack(f, "hl2pt", "Lept 2 pT [GeV]", leg3);
   hl2pt_bkg->SetMinimum(basemin);
   hl2pt_bkg->Draw();
   leg3->Draw();

   c4->cd(3);
   c4->GetPad(3)->SetLogy();
   THStack * hl1eta_bkg = GetBkgStack(f, "hl1eta", "Lept 1 eta [GeV]", NULL);
   hl1eta_bkg->SetMinimum(basemin);
   hl1eta_bkg->Draw();

   c4->cd(4);
   c4->GetPad(4)->SetLogy();
   THStack * hl2eta_bkg = GetBkgStack(f, "hl2eta", "Lept 2 eta [GeV]", NULL);
   hl2eta_bkg->SetMinimum(basemin);
   hl2eta_bkg->Draw();

   TCanvas * c5 = new TCanvas("c5", "");
   TLegend * leg5 = new TLegend(0.7, 0.65, 0.90, 0.90);
   c5->SetLogy();
   c5->cd();
   THStack * hnjet_stage1_bkg = GetBkgStack(f, "hnjet_stage1", "Num Jets", leg5);
   hnjet_stage1_bkg->SetMinimum(1.0);
   hnjet_stage1_bkg->Draw("nostack");
   leg5->Draw();
   c5->SaveAs("plots/njet_stage1.png");

*/


   //c3->SaveAs("plots/jets.png");

   /*
   TCanvas * c = new TCanvas("c", "");
   c->SetLogy();
   c->cd();
   THStack * bkg_mll = new THStack("bkg_mll","");
   THStack * sig_mll = new THStack("bkg_mll","");

   TH1F * hmll            = (TH1F *) f->Get("hmll");   
   TH1F * hmll_h          = (TH1F *) f->Get("hmll_h");   
   TH1F * hmll_diboson    = (TH1F *) f->Get("hmll_diboson");   
   TH1F * hmll_z          = (TH1F *) f->Get("hmll_z");
   TH1F * hmll_hxx_llvvxx = (TH1F *) f->Get("hmll_hxx_llvvxx");

   hmll_h       ->SetFillStyle(3004);
   hmll_diboson ->SetFillStyle(3005);
   hmll_z       ->SetFillStyle(3006);
   hmll_hxx_llvvxx->SetFillStyle(3003);


   hmll->SetXTitle("dilepton invariant mass [GeV]");


   hmll_h       ->SetFillColor(1);
   hmll_diboson ->SetFillColor(3);
   hmll_z       ->SetFillColor(4);
   hmll_hxx_llvvxx->SetFillColor(2);

   bkg_mll->SetHistogram(hmll);
   bkg_mll->Add(hmll_h      );
   bkg_mll->Add(hmll_diboson);
   bkg_mll->Add(hmll_z    );  
   bkg_mll->SetMinimum(basemin);
   

   bkg_mll->Draw();
   hmll_hxx_llvvxx->Draw("HSAME");
   */
}
