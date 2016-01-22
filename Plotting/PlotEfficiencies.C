// make plots of efficiencies -- run from MEX1UpgradeStudies

#include "TMath.h"
#include "TH1D.h"
#include "TChain.h"
#include "TLine.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "TCut.h"
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"

#include "Plotting/inc/jack_style.h"

// #include "macros/include/cms_style.h"

#include <iostream>

using namespace std;

void MakePlot(const TString xvar, TH1D* htemplate, const TCut cuts, const TString plotTitle, const TString input_directory = "NTupleMaker/test/",  bool logy = false) 
{ 

  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


 

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);

  // TCut baseline ="ht50>=400&&met>150&&weightppb<1.";
  // TCut lep_veto("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // baseline = baseline+lep_veto+xtraCuts;


  
  TChain * ch0p0 = new TChain("skim/RecoTree");
  ch0p0->Add(input_directory+"ME31F0p0_ME41_F0p0.root");
  TChain * ch0p25 = new TChain("skim/RecoTree");
  ch0p25->Add(input_directory+"ME31F0p25_ME41_F0p25.root");
  TChain * ch0p5 = new TChain("skim/RecoTree");
  ch0p5->Add(input_directory+"ME31F0p5_ME41_F0p5.root");
  TChain * ch0p75 = new TChain("skim/RecoTree");
  ch0p75->Add(input_directory+"ME31F0p75_ME41_F0p75.root");
  TChain * ch1p0 = new TChain("skim/RecoTree");
  ch1p0->Add(input_directory+"ME31F1p0_ME41_F1p1.root");

  TH1D * H0p0_pass = (TH1D*)htemplate->Clone("H0p0_pass");
  TH1D * H0p25_pass = (TH1D*)htemplate->Clone("H0p25_pass");
  TH1D * H0p5_pass = (TH1D*)htemplate->Clone("H0p5_pass");
  TH1D * H0p75_pass = (TH1D*)htemplate->Clone("H0p75_pass");
  TH1D * H1p0_pass = (TH1D*)htemplate->Clone("H1p0_pass");
  TH1D * H0p0_all = (TH1D*)htemplate->Clone("H0p0_all");
  TH1D * H0p25_all = (TH1D*)htemplate->Clone("H0p25_all");
  TH1D * H0p5_all = (TH1D*)htemplate->Clone("H0p5_all");
  TH1D * H0p75_all = (TH1D*)htemplate->Clone("H0p75_all");
  TH1D * H1p0_all = (TH1D*)htemplate->Clone("H1p0_all");
  TH1D * H0p0_eff = (TH1D*)htemplate->Clone("H0p0_eff");
  TH1D * H0p25_eff = (TH1D*)htemplate->Clone("H0p25_eff");
  TH1D * H0p5_eff = (TH1D*)htemplate->Clone("H0p5_eff");
  TH1D * H0p75_eff = (TH1D*)htemplate->Clone("H0p75_eff");
  TH1D * H1p0_eff = (TH1D*)htemplate->Clone("H1p0_eff");
 
  
  ch0p0->Project("H0p0_all", xvar, cuts);
  ch0p25->Project("H0p25_all", xvar, cuts);
  ch0p5->Project("H0p5_all", xvar, cuts);
  ch0p75->Project("H0p75_all", xvar, cuts);
  ch1p0->Project("H1p0_all", xvar, cuts);

  ch0p0->Project("H0p0_pass", xvar, cuts+"muGenRecoMatch");
  ch0p25->Project("H0p25_pass", xvar, cuts+"muGenRecoMatch");
  ch0p5->Project("H0p5_pass", xvar, cuts+"muGenRecoMatch");
  ch0p75->Project("H0p75_pass", xvar, cuts+"muGenRecoMatch");
  ch1p0->Project("H1p0_pass", xvar, cuts+"muGenRecoMatch");

  H0p0_eff->Divide(H0p0_pass, H0p0_all, 1., 1., "B");
  H0p25_eff->Divide(H0p25_pass, H0p25_all, 1., 1., "B");
  H0p5_eff->Divide(H0p5_pass, H0p5_all, 1., 1., "B");
  H0p75_eff->Divide(H0p75_pass, H0p75_all, 1., 1., "B");
  H1p0_eff->Divide(H1p0_pass, H1p0_all, 1., 1., "B");
  

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas", 800, 600);

  thecanvas->SetLogy(logy);

  H0p0_eff->SetLineColor(1);
  H0p25_eff->SetLineColor(5000);
  H0p5_eff->SetLineColor(5002);
  H0p75_eff->SetLineColor(2005);
  H1p0_eff->SetLineColor(5004);
 

  Width_t width=2;
  H0p0_eff->SetLineWidth(width);
  H0p25_eff->SetLineWidth(width);
  H0p5_eff->SetLineWidth(width);
  H0p75_eff->SetLineWidth(width);
  H1p0_eff->SetLineWidth(width);

  H0p0_eff->GetYaxis()->SetTitleOffset(1.3);
  H0p0_eff->GetYaxis()->SetTitle("Efficiency");
  H0p0_eff->SetMaximum(H0p0_eff->GetMaximum()*1.5);
  H0p0_eff->SetMinimum(0.);

  H0p0_all->Print("all");
  H0p5_all->Print("all");
  H0p0_pass->Print("all");
  H0p5_pass->Print("all");
  H0p0_eff->Print("all");
  H0p5_eff->Print("all");


  TLegend * leg = new TLegend(0.13,0.73,0.43,0.88);
  set_style(leg, 0.035);
  leg->SetHeader("#splitline{CFEB Fail Rate}{(ME+/-3/1 & ME+/-4/1)}");
  leg->SetNColumns(3);
  leg->AddEntry(H0p0_eff,"0 %");
  leg->AddEntry(H0p25_eff,"25 %");
  leg->AddEntry(H0p5_eff,"50 %");
  leg->AddEntry(H0p75_eff,"75 %");
  leg->AddEntry(H1p0_eff,"100 %");
 
 
  H0p0_eff->Draw("hist e"); 
  H0p25_eff->Draw("hist e SAME");
  H0p5_eff->Draw("hist e SAME"); 
  H0p75_eff->Draw("hist e SAME"); 
  H1p0_eff->Draw("hist e SAME"); 



  leg->Draw();

  thecanvas->Print("Plotting/plots/"+plotTitle+".pdf");


  delete ch0p0;
  delete ch0p25;
  delete ch0p5;
  delete ch0p75;
  delete ch1p0;
  delete H0p0_pass;
  delete H0p25_pass;
  delete H0p5_pass;
  delete H0p75_pass;
  delete H1p0_pass;
  delete H0p0_all;
  delete H0p25_all;
  delete H0p5_all;
  delete H0p75_all;
  delete H1p0_all;
  delete H0p0_eff;
  delete H0p25_eff;
  delete H0p5_eff;
  delete H0p75_eff;
  delete H1p0_eff;
  delete thecanvas;
  delete leg;
}


void PlotEfficiencies() {

  Double_t ptBins[9] = {0, 3, 6, 10, 20, 50, 100, 200, 400};
  TH1D* hPt = new TH1D("hPt", ";p_{T}^{#mu} [GeV];", 8, ptBins);
  hPt->Sumw2();
  MakePlot("muGenPt", hPt, "muGenPt>0", "muon_pt_all_eta");
  MakePlot("muGenPt", hPt, "abs(muGenEta)<2.4", "muon_pt_eta_l2p4");
  MakePlot("muGenPt", hPt, "abs(muGenEta)<1.6", "muon_pt_eta_l1p6");
  MakePlot("muGenPt", hPt, "abs(muGenEta)>2.4", "muon_pt_eta_g2p4");
  MakePlot("muGenPt", hPt, "abs(muGenEta)>1.6", "muon_pt_eta_g1p6");

  Double_t etaBins[15] = {-5., -4., -3., -2.4, -1.6, -1., -0.5, 0, 0.5, 1., 1.6, 2.4, 3., 4., 5.};
  TH1D* hEta = new TH1D("hEta", ";#eta^{#mu} [GeV];", 14, etaBins);
  hEta->Sumw2();
  MakePlot("muGenEta", hEta, "muGenPt>0", "muon_eta_all_pt");

}
