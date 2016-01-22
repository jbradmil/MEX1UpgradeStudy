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

void MakePlot(TH1D* htemplate, const TString cuts, const TString plotTitle, const TString input_directory = "NTupleMaker/test/",  bool logy = false) 
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

  TH1D * Hgen = (TH1D*)htemplate->Clone("Hgen");
  TH1D * H0p0_pass = (TH1D*)htemplate->Clone("H0p0_pass");
  TH1D * H0p25_pass = (TH1D*)htemplate->Clone("H0p25_pass");
  TH1D * H0p5_pass = (TH1D*)htemplate->Clone("H0p5_pass");
  TH1D * H0p75_pass = (TH1D*)htemplate->Clone("H0p75_pass");
  TH1D * H1p0_pass = (TH1D*)htemplate->Clone("H1p0_pass");
  
  ch0p0->Project("Hgen", Form("Sum$(%s)",cuts.Data()));
  ch0p0->Project("H0p0_pass", Form("Sum$(%s&&muGenRecoMatch)",cuts.Data()));
  ch0p25->Project("H0p25_pass", Form("Sum$(%s&&muGenRecoMatch)",cuts.Data()));
  ch0p5->Project("H0p5_pass", Form("Sum$(%s&&muGenRecoMatch)",cuts.Data()));
  ch0p75->Project("H0p75_pass", Form("Sum$(%s&&muGenRecoMatch)",cuts.Data()));
  ch1p0->Project("H1p0_pass", Form("Sum$(%s&&muGenRecoMatch)",cuts.Data()));


  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas", 800, 600);

  thecanvas->SetLogy(logy);

  Hgen->SetLineColor(12);
  Hgen->SetLineStyle(3);
  H0p0_pass->SetLineColor(1);
  H0p25_pass->SetLineColor(5000);
  H0p5_pass->SetLineColor(5002);
  H0p75_pass->SetLineColor(2005);
  H1p0_pass->SetLineColor(5004);
 

  Width_t width=2;
  Hgen->SetLineWidth(width);
  H0p0_pass->SetLineWidth(width);
  H0p25_pass->SetLineWidth(width);
  H0p5_pass->SetLineWidth(width);
  H0p75_pass->SetLineWidth(width);
  H1p0_pass->SetLineWidth(width);

  Hgen->GetYaxis()->SetTitleOffset(1.3);
  Hgen->GetYaxis()->SetTitle("Fraction of events");

  cout << Hgen->Integral() << endl;
  cout << H0p0_pass->Integral() << endl;
  cout << H1p0_pass->Integral() << endl;
  // H0p5_all->Print("all");
  // H0p0_pass->Print("all");
  // H0p5_pass->Print("all");
  // H0p0_pass->Print("all");
  // H0p5_pass->Print("all");

  Hgen->Scale(1/Hgen->Integral());
  H0p0_pass->Scale(1/H0p0_pass->Integral());
  H0p25_pass->Scale(1/H0p25_pass->Integral());
  H0p5_pass->Scale(1/H0p5_pass->Integral());
  H0p75_pass->Scale(1/H0p75_pass->Integral());
  H1p0_pass->Scale(1/H1p0_pass->Integral());

  Hgen->SetMaximum(1.1);
  Hgen->SetMinimum(0.);



  TLegend * leg = new TLegend(0.13,0.73,0.83,0.88);
  set_style(leg, 0.035);
  leg->SetHeader("CFEB Fail Rate (ME+/-3/1 & ME+/-4/1)");
  leg->SetNColumns(3);
  TString label;
  label = Form("0%% (#mu = %3.2f)", H0p0_pass->GetMean()); 
  leg->AddEntry(H0p0_pass,label);
  label = Form("25%% (#mu = %3.2f)", H0p25_pass->GetMean()); 
  leg->AddEntry(H0p25_pass,label);
  label = Form("50%% (#mu = %3.2f)", H0p5_pass->GetMean()); 
  leg->AddEntry(H0p5_pass,label);
  label = Form("75%% (#mu = %3.2f)", H0p75_pass->GetMean()); 
  leg->AddEntry(H0p75_pass, label);
  label = Form("100%% (#mu = %3.2f)", H1p0_pass->GetMean()); 
  leg->AddEntry(H1p0_pass,label);
  label = Form("Gen (#mu = %3.2f)", Hgen->GetMean()); 
  leg->AddEntry(Hgen,label);
 
 
  Hgen->Draw("hist e"); 
  H0p0_pass->Draw("hist e SAME"); 
  H0p25_pass->Draw("hist e SAME");
  H0p5_pass->Draw("hist e SAME"); 
  H0p75_pass->Draw("hist e SAME"); 
  H1p0_pass->Draw("hist e SAME"); 



  leg->Draw();

  thecanvas->Print("Plotting/plots/"+plotTitle+".pdf");


  delete ch0p0;
  delete ch0p25;
  delete ch0p5;
  delete ch0p75;
  delete ch1p0;
  delete Hgen;
  delete H0p0_pass;
  delete H0p25_pass;
  delete H0p5_pass;
  delete H0p75_pass;
  delete H1p0_pass;
  delete thecanvas;
  delete leg;
}


void PlotNMuons() {

  TH1D* hNMuons = new TH1D("hNMuons", ";N^{#mu}", 13, -0.5, 12.5);
  hNMuons->Sumw2();
  MakePlot(hNMuons, "muGenPt>0", "Nmuons_all_pt_all_eta");
  hNMuons->GetXaxis()->SetTitle("N^{#mu} (p_{T} > 20 GeV)");
  MakePlot(hNMuons, "muGenPt>20", "Nmuons_ptg20_all_eta");
  hNMuons->GetXaxis()->SetTitle("N^{#mu} (p_{T} > 20 GeV, #eta < 1.6)");
  MakePlot(hNMuons, "muGenPt>20&&abs(muGenEta)<1.6", "Nmuons_ptg20_etal1p6");
  hNMuons->GetXaxis()->SetTitle("N^{#mu} (#eta > 3)");
  MakePlot(hNMuons, "abs(muGenEta)>3", "Nmuons_all_pt_etag3");

}
