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

void MakePlot(const TString xvar, TH1D* htemplate, const TCut cuts, const TString plotTitle, int config=3, double ymin=0., double ymax=1.1, bool logx = false, bool logy=false, const TString input_directory = /*"/hadoop/cms/store/user/jbradmil/CSCUpgrade/Trees/"*/ "NTupleMaker/test/") 
{ 

  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


 

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);

  // TCut baseline ="ht50>=400&&met>150&&weightppb<1.";
  // TCut lep_veto("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // baseline = baseline+lep_veto+xtraCuts;


  
  TChain * ch0p0 = new TChain("skim/RecoTree");
  ch0p0->Add(input_directory+"DY_me21_me31_me41_0p0.root");
  TChain * ch0p25 = new TChain("skim/RecoTree");
  TChain * ch0p5 = new TChain("skim/RecoTree");
  TChain * ch0p75 = new TChain("skim/RecoTree");
  TChain * ch1p0 = new TChain("skim/RecoTree");
  if (config==4) { // 2/1, 3/1, 4/1
    ch0p25->Add(input_directory+"DY_me21_0p25.root");
    ch0p5->Add(input_directory+"DY_me21_0p5.root");
    ch0p75->Add(input_directory+"DY_me21_0p75.root");
    ch1p0->Add(input_directory+"DY_me21_1p0.root");
  } else if (config==3) { // 2/1, 3/1, 4/1
    ch0p25->Add(input_directory+"ME21_ME31_ME410p25.root");
    ch0p5->Add(input_directory+"ME21_ME31_ME410p5.root");
    ch0p75->Add(input_directory+"ME21_ME31_ME410p75.root");
    ch1p0->Add(input_directory+"ME21_ME31_ME411p0.root");
  } else if (config==2) { // 3/1, 4/1
    ch0p25->Add(input_directory+"ME31_ME410p25.root");
    ch0p5->Add(input_directory+"ME31_ME410p5.root");
    ch0p75->Add(input_directory+"ME31_ME410p75.root");
    ch1p0->Add(input_directory+"ME31_ME411p0.root");
  } else { //  4/1
    ch0p25->Add(input_directory+"ME410p25.root");
    ch0p5->Add(input_directory+"ME410p5.root");
    ch0p75->Add(input_directory+"ME410p75.root");
    ch1p0->Add(input_directory+"ME411p0.root");
  }
  
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

  H0p0_eff->SetLineColor(1);
  H0p25_eff->SetLineColor(2000);
  H0p5_eff->SetLineColor(2004);
  H0p75_eff->SetLineColor(2005);
  H1p0_eff->SetLineColor(2002);
  H0p0_eff->SetMarkerColor(1);
  H0p25_eff->SetMarkerColor(2000);
  H0p5_eff->SetMarkerColor(2004);
  H0p75_eff->SetMarkerColor(2005);
  H1p0_eff->SetMarkerColor(2002);
  H0p25_eff->SetLineStyle(9);
  H0p5_eff->SetLineStyle(7);
  H0p75_eff->SetLineStyle(2);
  H1p0_eff->SetLineStyle(3);
 

  Width_t width=2;
  H0p0_eff->SetLineWidth(width);
  H0p25_eff->SetLineWidth(width);
  H0p5_eff->SetLineWidth(width);
  H0p75_eff->SetLineWidth(3);
  H1p0_eff->SetLineWidth(3);


  
  ch0p0->Project("H0p0_all", xvar, cuts);
  ch0p25->Project("H0p25_all", xvar, cuts);
  ch0p5->Project("H0p5_all", xvar, cuts);
  ch0p75->Project("H0p75_all", xvar, cuts);
  ch1p0->Project("H1p0_all", xvar, cuts);

  ch0p0->Project("H0p0_pass", xvar, cuts+"GenRecoMatch");
  ch0p25->Project("H0p25_pass", xvar, cuts+"GenRecoMatch");
  ch0p5->Project("H0p5_pass", xvar, cuts+"GenRecoMatch");
  ch0p75->Project("H0p75_pass", xvar, cuts+"GenRecoMatch");
  ch1p0->Project("H1p0_pass", xvar, cuts+"GenRecoMatch");

  H0p0_eff->Divide(H0p0_pass, H0p0_all, 1., 1., "B");
  H0p25_eff->Divide(H0p25_pass, H0p25_all, 1., 1., "B");
  H0p5_eff->Divide(H0p5_pass, H0p5_all, 1., 1., "B");
  H0p75_eff->Divide(H0p75_pass, H0p75_all, 1., 1., "B");
  H1p0_eff->Divide(H1p0_pass, H1p0_all, 1., 1., "B");

  //  H0p0_eff->SetBit(TH1::kIsAverage);
  // H0p25_eff->SetBit(TH1::kIsAverage);
  // H0p5_eff->SetBit(TH1::kIsAverage);
  // H0p75_eff->SetBit(TH1::kIsAverage);
  // H1p0_eff->SetBit(TH1::kIsAverage);
  

  // Setup canvas and pads

  int W = 800;
  int H = 800;

  // 
  // Simple example of macro: plot with CMS name and lumi text
  //  (this script does not pretend to work in all configurations)
  // iPeriod = 1*(0/1 7 TeV) + 2*(0/1 8 TeV)  + 4*(0/1 13 TeV) 
  // For instance: 
  //               iPeriod = 3 means: 7 TeV + 8 TeV
  //               iPeriod = 7 means: 7 TeV + 8 TeV + 13 TeV 
  // Initiated by: Gautier Hamel de Monchenault (Saclay)
  // Updated by:   Dinko Ferencek (Rutgers)
  //
  int H_ref = 800; 
  int W_ref = 800; 

  // references for T, B, L, R
  float T = 0.08*H_ref;
  float B = 0.1*H_ref; 
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;

  TCanvas* thecanvas = new TCanvas("canvName","canvName",50,50,W,H);
  thecanvas->SetFillColor(0);
  thecanvas->SetBorderMode(0);
  thecanvas->SetFrameFillStyle(0);
  thecanvas->SetFrameBorderMode(0);
  thecanvas->SetLeftMargin( L/W );
  thecanvas->SetRightMargin( R/W );
  thecanvas->SetTopMargin( T/H );
  thecanvas->SetBottomMargin( B/H );
  thecanvas->SetTickx(0);
  thecanvas->SetTicky(0);

   double up_height     = 0.8;  // please tune so that the upper figures size will meet your requirement
  double dw_correction = 1.30; // please tune so that the smaller canvas size will work in your environment
  double font_size_dw  = 0.14;  // please tune the font size parameter for bottom figure
  double dw_height     = (1. - up_height) * dw_correction;
  double dw_height_offset = 0.02; // KH, added to put the bottom one closer to the top panel

  
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.4, 1.0, 1.0);
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.2);

  pad1->SetTickx(0);
  pad1->SetTicky(0);
  pad1->SetPad(0., 1 - up_height,    1., 1.00);
  //
  pad1->SetFrameFillColor(0);
  pad1->SetFillColor(0);
  pad1->SetTopMargin(0.12);
  pad1->SetLeftMargin(0.1);
  // pad1->SetBottomMargin(0.0);
  pad1->Draw();

  pad2->SetPad(0., 0., 1., dw_height+dw_height_offset);
  pad2->SetFillColor(0);
  pad2->SetFrameFillColor(0);
  pad2->SetBottomMargin(0.38);
  pad2->SetTopMargin(0);
  pad2->SetLeftMargin(0.1);
  pad2->Draw();
  pad1->cd();
  pad1->SetLogx(logx);
  pad1->SetLogy(logy);
  pad2->SetLogx(logx);

  // TCanvas * thecanvas= new TCanvas("thecanvas","the canvas", 800, 600);

  // thecanvas->SetLogx(logx);
  // thecanvas->SetLogy(logy);



  // H0p0_all->Print("all");
  // H0p5_all->Print("all");
  // H0p0_pass->Print("all");
  // H0p5_pass->Print("all");
  // H0p0_eff->Print("all");
  // H1p0_eff->Print("all");

  


  TLegend * leg = new TLegend(0.13,0.71,0.43,0.86);
  set_style(leg, 0.035);
  if (config==4) leg->SetHeader("CFEB Fail Rate (ME+/-2/1 only)");
  else if (config==3) leg->SetHeader("CFEB Fail Rate (ME+/-2/1, ME+/-3/1 & ME+/-4/1)");
  else if (config==2) leg->SetHeader("CFEB Fail Rate (ME+/-3/1 & ME+/-4/1 only)");
  else leg->SetHeader("CFEB Fail Rate (ME+/-4/1 only)");
  leg->SetNColumns(3);
  leg->AddEntry(H0p0_eff,"0 %");
  leg->AddEntry(H0p25_eff,"25 %");
  leg->AddEntry(H0p5_eff,"50 %");
  leg->AddEntry(H0p75_eff,"75 %");
  leg->AddEntry(H1p0_eff,"100 %");
 
  H0p0_eff->GetYaxis()->SetTitleOffset(1.3);
  H0p0_eff->GetYaxis()->SetTitle("Offline StandAlone Reconstruction #epsilon");
  H0p0_eff->SetMaximum(ymax);
  H0p0_eff->SetMinimum(ymin);

  
  H0p0_eff->Draw("hist e"); 
  H0p25_eff->Draw("hist e SAME");
  H0p5_eff->Draw("hist e SAME"); 
  H0p75_eff->Draw("hist e SAME"); 
  H1p0_eff->Draw("hist e SAME"); 
  H0p0_eff->Draw("hist e SAME"); 

  leg->Draw();

  pad2->cd();
  pad2->SetGridy(0);
  // calculate efficiency loss
  TH1D * H0p25_loss = (TH1D*)H0p25_eff->Clone("H0p25_loss");
  TH1D * H0p5_loss = (TH1D*)H0p5_eff->Clone("H0p5_loss");
  TH1D * H0p75_loss = (TH1D*)H0p75_eff->Clone("H0p75_loss");
  TH1D * H1p0_loss = (TH1D*)H1p0_eff->Clone("H1p0_loss");

  H0p25_loss->Add(H0p25_eff, H0p0_eff, 1., -1.);
  H0p5_loss->Add(H0p5_eff, H0p0_eff, 1., -1.);
  H0p75_loss->Add(H0p75_eff, H0p0_eff, 1., -1.);
  H1p0_loss->Add(H1p0_eff, H0p0_eff, 1., -1.);


  H1p0_loss->GetYaxis()->SetTitle("#epsilon (fail) - #epsilon (no fail)");
  H1p0_loss->GetXaxis()->SetLabelSize(font_size_dw);
  H1p0_loss->GetXaxis()->SetTitleSize(font_size_dw);
  H1p0_loss->GetYaxis()->SetLabelSize(0.1);
  H1p0_loss->GetYaxis()->SetTitleSize(0.1);

  H1p0_loss->GetXaxis()->SetTitleOffset(1.);
  H1p0_loss->GetXaxis()->SetTitleFont(42);
  H1p0_loss->GetYaxis()->SetTitleOffset(0.5);
  H1p0_loss->GetYaxis()->SetTitleFont(42);
  H1p0_loss->GetYaxis()->SetNdivisions(505);
  H1p0_loss->GetYaxis()->SetTickLength(0.015);
  H1p0_loss->GetXaxis()->SetTickLength(0.08);
  //hbg_pred->GetXaxis()->SetTitleSize(0.2);

  //  printf("%3.2f %3.2f %3.2f %3.2f\n", H1p0_loss->GetXaxis()->GetTitleSize(), H1p0_loss->GetXaxis()->GetTitleOffset(), H1p0_loss->GetYaxis()->GetTitleSize(), H1p0_loss->GetYaxis()->GetTitleOffset());
  H1p0_loss->SetMinimum(H1p0_loss->GetMinimum()-0.05);
  H1p0_loss->SetMaximum(0-(H1p0_loss->GetMinimum()/10.));
  H1p0_loss->Draw("hist e");
  H0p75_loss->Draw("hist e same");
  H0p5_loss->Draw("hist e same");
  H0p25_loss->Draw("hist e same");
  H1p0_loss->Draw("hist e same");

  pad1->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();
  pad2->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();

  
  thecanvas->cd();
  thecanvas->Print("Plotting/plots/DY/"+plotTitle+".pdf");


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
  delete H0p25_loss;
  delete H0p5_loss;
  delete H0p75_loss;
  delete H1p0_loss;
  delete pad1;
  delete pad2;
  delete thecanvas;
  delete leg;
}


void PlotEfficiencies() {

  Double_t ptBins[8] = {20, 50, 100, 200, 400, 600, 800, 1250};
  TH1D* hPt = new TH1D("hPt", ";p_{T}^{#mu} [GeV];", 7, ptBins);
  hPt->Sumw2();
  // MakePlot("GenPt", hPt, "abs(GenEta)>1.6&&abs(GenEta)<2.4", "muon_pt_eta_g1p6_ME41", 1, 0.35, 1.15, true);
  MakePlot("GenPt", hPt, "abs(GenEta)>1.6&&abs(GenEta)<2.4", "muon_pt_eta_g1p6_ME31_ME41", 2, 0.73, 1.15, false);
  //MakePlot("GenPt", hPt, "abs(GenEta)>1.6&&abs(GenEta)<2.4", "muon_pt_eta_g1p6_ME21_ME31_ME41", 3, 0.35, 1.15, true);
  MakePlot("GenPt", hPt, "abs(GenEta)>1.6&&abs(GenEta)<2.4", "muon_pt_eta_g1p6_ME21", 4, 0.73, 1.15, false);


  Double_t etaBins[13] = {-2.4, -2.1, -1.6, -1.2, -1.04, -0.8, 0, 0.8, 1.04, 1.2, 1.6, 2.1, 2.4};
  TH1D* hEta = new TH1D("hEta", ";#eta^{#mu};", 12, etaBins);
  hEta->Sumw2();
  // MakePlot("GenEta", hEta, "GenPt>5", "muon_eta_pt_g5_ME41", 1, 0.01, 1.3);
  MakePlot("GenEta", hEta, "GenPt>5", "muon_eta_pt_g5_ME31_ME41", 2, 0.73, 1.08);
  //MakePlot("GenEta", hEta, "GenPt>5", "muon_eta_pt_g5_ME21_ME31_ME41", 3, 0.001, 1.3);
  MakePlot("GenEta", hEta, "GenPt>5", "muon_eta_pt_g5_ME21", 4, 0.73, 1.08);

}
