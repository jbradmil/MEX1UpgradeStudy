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
#include "TFile.h"
#include "TROOT.h"
#include "TCanvas.h"

#include "Plotting/inc/jack_style.h"

// #include "macros/include/cms_style.h"

#include <iostream>

using namespace std;

void MakePlot(const TString xvar, TH1D* htemplate, TCut cuts, const TString plotTitle, int config=1, bool logx = false, bool logy=false, const TString input_directory = "~/muon_trees/trees/DYMC/skims/") 
{ 

  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


 

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);


  
  TChain * ch0p0 = new TChain("Tree");
  ch0p0->Add(input_directory+"DY-FR-0.00-0.00-0.00.root");
  TChain * ch0p5 = new TChain("Tree");
  TChain * ch1p0 = new TChain("Tree");
  if (config==1) { // 2/1
    ch0p5->Add(input_directory+"DY-FR-0.50-0.00-0.00.root");
    ch1p0->Add(input_directory+"DY-FR-1.00-0.00-0.00.root");
  } else if (config==2) { // 3/1, 4/1
    ch0p5->Add(input_directory+"DY-FR-0.00-0.50-0.50.root");
    ch1p0->Add(input_directory+"DY-FR-0.00-1.00-1.00.root");
  }
  else if (config==3) { //2/1,  3/1, 4/1
    ch0p5->Add(input_directory+"DY-FR-0.50-0.50-0.50.root");
    ch1p0->Add(input_directory+"DY-FR-1.00-1.00-1.00.root");
  }

  double nTag0p0 = (double)ch0p0->GetEntries("HLT_IsoMu27_v2&&abs(mu1TLV.Eta())<1.6&&mu1TLV.Pt()>27&&mu1Tight&&abs(mu2TLV.Eta())<1.6&&mu2TLV.Pt()>20&&mu2Tight&&abs(mumuMass-91)<10");
  double nTag0p5 = (double)ch0p5->GetEntries("HLT_IsoMu27_v2&&abs(mu1TLV.Eta())<1.6&&mu1TLV.Pt()>27&&mu1Tight&&abs(mu2TLV.Eta())<1.6&&mu2TLV.Pt()>20&&mu2Tight&&abs(mumuMass-91)<10");
  double nTag1p0 = (double)ch1p0->GetEntries("HLT_IsoMu27_v2&&abs(mu1TLV.Eta())<1.6&&mu1TLV.Pt()>27&&mu1Tight&&abs(mu2TLV.Eta())<1.6&&mu2TLV.Pt()>20&&mu2Tight&&abs(mumuMass-91)<10");
  
  printf("Events in chains: %3.0f/%3.0f/%3.0f\n", (double)ch0p0->GetEntries(), (double)ch0p5->GetEntries(), (double)ch1p0->GetEntries());
  printf("Events in chains with tag: %3.0f/%3.0f/%3.0f\n", nTag0p0, nTag0p5, nTag1p0);
  
  TH1D * H0p0_pass = (TH1D*)htemplate->Clone("H0p0_pass");
  TH1D * H0p5_pass = (TH1D*)htemplate->Clone("H0p5_pass");
  TH1D * H1p0_pass = (TH1D*)htemplate->Clone("H1p0_pass");
  H0p0_pass->SetLineColor(1);
  H0p5_pass->SetLineColor(38);
  H1p0_pass->SetLineColor(46);
  H0p0_pass->SetMarkerColor(1);
  H0p5_pass->SetMarkerColor(38);
  H1p0_pass->SetMarkerColor(46);
  // H0p0_pass->SetLineStyle(3);
  // H0p5_pass->SetLineStyle(3);
  // H1p0_pass->SetLineStyle(3);
  

 

  H0p0_pass->SetLineWidth(3);
  H0p5_pass->SetLineWidth(3);
  H1p0_pass->SetLineWidth(3);


  cuts+="abs(mu1TLV.Eta())<1.6&&mu1TLV.Pt()>27&&mu1Tight";

  printf("Filling H0p0_pass: %s\n", cuts.GetTitle());

  ch0p0->Project("H0p0_pass", xvar, cuts);

  double ch0p5_weight=nTag0p0/nTag0p5;
  double ch1p0_weight=nTag0p0/nTag1p0;
  
  TCut c_ch0p5_weight = Form("%f", ch0p5_weight);
  TCut c_ch1p0_weight = Form("%f", ch1p0_weight);
  
  printf("Filling H0p5_pass: %s\n", (cuts*c_ch0p5_weight).GetTitle());
  ch0p5->Project("H0p5_pass", xvar, cuts);
  printf("Filling H1p0_pass: %s\n", (cuts*c_ch1p0_weight).GetTitle());
  ch1p0->Project("H1p0_pass", xvar, cuts);

  // H0p0_pass->Print("all");
  // H0p5_pass->Print("all");
  // H1p0_pass->Print("all");
  double nProbe0p0 = H0p0_pass->Integral();
  double nProbe0p5 = H0p5_pass->Integral();
  double nProbe1p0 = H1p0_pass->Integral();
  printf("Events in chains with probe: %3.0f/%3.0f/%3.0f\n", nProbe0p0, nProbe0p5, nProbe1p0);
  
  // Setup canvas and pads

  int W = 800;
  int H = 800;
  int H_ref = 800; 
  int W_ref = 800; 

  // references for T, B, L, R
  float T = 0.08*H_ref;
  float B = 0.1*H_ref; 
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;

  TCanvas* thecanvas = new TCanvas("canvName","canvName",50,50,W,H);
  thecanvas->SetFillColor(0);
  thecanvas->SetFillStyle(4000);
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

  
  TPad * pad1 = new TPad("pad1", "top pad" , 0.1, 0.4, 1.0, 1.0);
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.1, 0.0, 1.0, 0.2);

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

 
  TLegend * leg1 = new TLegend(0.13,0.74,0.28,0.86);
  set_style(leg1, 0.035);
  if (config==1) leg1->SetHeader("Fail Rate (2/1 affected)");
  else if (config==2) leg1->SetHeader("Fail Rate (3/1 & 4/1 affected)");
  else if (config==3) leg1->SetHeader("Fail Rate (2/1, 3/1, & 4/1 affected)");
  leg1->SetNColumns(3);
  leg1->AddEntry(H0p0_pass,"0 %");
  leg1->AddEntry(H0p5_pass,"50 %");
  leg1->AddEntry(H1p0_pass,"100 %");

 
  H0p0_pass->GetYaxis()->SetTitleOffset(1.3);
  H0p0_pass->GetYaxis()->SetTitle("Muons (a.u.)");
  if (xvar=="mumuMass") H0p0_pass->GetYaxis()->SetTitle("Events (a.u.)");
  //  H0p0_pass->SetMaximum(ymax);
  if (logy) {
    H0p0_pass->SetMaximum(30*H0p0_pass->GetMaximum());
    H0p0_pass->SetMinimum(0.09);
  }
  else{
    H0p0_pass->SetMaximum(1.3*H0p0_pass->GetMaximum());
    H0p0_pass->SetMinimum(0);
  }
  
  // if (config==1) H0p0_pass->SetTitle("StandAlone efficiency vs. CFEB fail rate (ME+/-2/1 affected)");
  // else if (config==2) H0p0_pass->SetTitle("StandAlone efficiency vs. CFEB fail rate (ME+/-3/1 & ME+/-4/1 affected)");
  // //else if (config==3) H0p0_pass->SetTitle("RECO efficiency vs. CFEB fail rate (ME+/-2/1, ME+/-3/1, & ME+/-4/1 affected)");
  H0p0_pass->Draw("hist e"); 
  H0p5_pass->Draw("hist e SAME"); 
  H1p0_pass->Draw("hist e SAME"); 
  H0p0_pass->Draw("hist e SAME");

  leg1->Draw();

  pad2->cd();
  pad2->SetGridy(0);
  // calculate efficiency loss
  TH1D * H0p5_loss = (TH1D*)H0p5_pass->Clone("H0p5_loss");
  TH1D * H1p0_loss = (TH1D*)H1p0_pass->Clone("H1p0_loss");

  H0p5_loss->Add(H0p5_pass, H0p0_pass, 1., -1.);
  H1p0_loss->Add(H1p0_pass, H0p0_pass, 1., -1.);

  H0p5_loss->Divide(H0p5_loss, H0p0_pass, 1., 1., "B");
  H1p0_loss->Divide(H1p0_loss, H0p0_pass, 1., 1., "B");

  H0p5_loss->Print("all");  
  H1p0_loss->Print("all");  

  H0p5_loss->GetYaxis()->SetTitle("#frac{N (fail) - N (no fail)}{N (no fail)}");
  H1p0_loss->GetYaxis()->SetTitle("#frac{N (fail) - N (no fail)}{N (no fail)}");
  H1p0_loss->GetXaxis()->SetLabelSize(font_size_dw);
  H1p0_loss->GetXaxis()->SetTitleSize(font_size_dw);
  H1p0_loss->GetYaxis()->SetLabelSize(0.08);
  H1p0_loss->GetYaxis()->SetTitleSize(0.08);

  H1p0_loss->GetXaxis()->SetTitleOffset(1.);
  H1p0_loss->GetXaxis()->SetTitleFont(42);
  H1p0_loss->GetYaxis()->SetTitleOffset(0.5);
  H1p0_loss->GetYaxis()->SetTitleFont(42);
  H1p0_loss->GetYaxis()->SetNdivisions(505);
  H1p0_loss->GetYaxis()->SetTickLength(0.015);
  H1p0_loss->GetXaxis()->SetTickLength(0.08);
  //hbg_pred->GetXaxis()->SetTitleSize(0.2);

  //  printf("%3.2f %3.2f %3.2f %3.2f\n", H1p0_loss->GetXaxis()->GetTitleSize(), H1p0_loss->GetXaxis()->GetTitleOffset(), H1p0_loss->GetYaxis()->GetTitleSize(), H1p0_loss->GetYaxis()->GetTitleOffset());
  H1p0_loss->SetMinimum(-0.99);
  H1p0_loss->SetMaximum(-0.007);
  H1p0_loss->Draw("hist e");
  H0p5_loss->Draw("hist e same");

  pad1->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();
  pad2->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();

  
  thecanvas->cd();
  thecanvas->Print("Plotting/plots/613/MC/"+plotTitle+".pdf");

  TFile* outfile = new TFile("Plotting/hists/613/MC/"+plotTitle+".root", "recreate");
  H0p0_pass->Write();
  H0p5_pass->Write();
  H1p0_pass->Write();
  H0p5_loss->Write();
  H1p0_loss->Write();
  outfile->Close();

  delete ch0p0;
  delete ch0p5;
  delete ch1p0;
  delete H0p0_pass;
  delete H0p5_pass;
  delete H1p0_pass;
  delete H0p5_loss;
  delete H1p0_loss;
  delete pad1;
  delete pad2;
  delete thecanvas;
  delete leg1;
}


void PlotEfficienciesMC() {


  TCut mumuMass = "mumuMass<101&&mumuMass>81";
  TCut mu2EC = "abs(mu2TLV.Eta())>1.6&&abs(mu2TLV.Eta())<2.4";
  
  Double_t ptBins[14] = {10,15,20,25,30,35,40,50, 75, 100, 150, 200, 300, 400};
  //TH1D* hPt = new TH1D("hPt", ";p_{T}^{#mu} [GeV];", 13, ptBins);
  TH1D* hPt = new TH1D("hPt", ";p_{T}^{#mu} [GeV];", 5, 10, 210);
  hPt->Sumw2();
  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10"+mu2EC, "mu2_pt_zmass_ecSA_ME21", 1, false, true);
  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Global"+mu2EC, "mu2_pt_zmass_ecGLB_ME21", 1, false, true);
  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Medium"+mu2EC, "mu2_pt_zmass_ecMED_ME21", 1, false, true);
  MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Tight"+mu2EC, "mu2_pt_zmass_ecTIGHT_ME21", 1, false, true);

  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10"+mu2EC, "mu2_pt_zmass_ecSA_ME3141", 2, false, true);
  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Global"+mu2EC, "mu2_pt_zmass_ecGLB_ME3141", 2, false, true);
  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Medium"+mu2EC, "mu2_pt_zmass_ecMED_ME3141", 2, false, true);
  MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Tight"+mu2EC, "mu2_pt_zmass_ecTIGHT_ME3141", 2, false, true);

  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10"+mu2EC, "mu2_pt_zmass_ecSA_ME213141", 3, false, true);
  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Global"+mu2EC, "mu2_pt_zmass_ecGLB_ME213141", 3, false, true);
  // MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Medium"+mu2EC, "mu2_pt_zmass_ecMED_ME213141", 3, false, true);
  MakePlot("mu2TLV.Pt()", hPt, mumuMass+"mu2TLV.Pt()>10&&mu2Tight"+mu2EC, "mu2_pt_zmass_ecTIGHT_ME213141", 3, false, true);

  
  TH1D* hAbsEta = new TH1D("hAbsEta", ";|#eta^{#mu}|;", 8, 1.6, 2.4);
  hAbsEta->Sumw2();

  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20", "mu2_abseta_zmass_SA20_ME21", 1);
  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Global", "mu2_abseta_zmass_GLB20_ME21", 1);
  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Medium", "mu2_abseta_zmass_MED20_ME21", 1);
  MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Tight", "mu2_abseta_zmass_TIGHT20_ME21", 1);

  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20", "mu2_abseta_zmass_SA20_ME3141", 2);
  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Global", "mu2_abseta_zmass_GLB20_ME3141", 2);
  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Medium", "mu2_abseta_zmass_MED20_ME3141", 2);
  MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Tight", "mu2_abseta_zmass_TIGHT20_ME3141", 2);

  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20", "mu2_abseta_zmass_SA20_ME213141", 3);
  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Global", "mu2_abseta_zmass_GLB20_ME213141", 3);
  // MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Medium", "mu2_abseta_zmass_MED20_ME213141", 3);
  MakePlot("abs(mu2TLV.Eta())", hAbsEta, mumuMass+"mu2TLV.Pt()>20&&mu2Tight", "mu2_abseta_zmass_TIGHT20_ME213141", 3);

  Double_t etaBins[13] = {-2.4, -2.1, -1.6, -1.2, -1.04, -0.8, 0, 0.8, 1.04, 1.2, 1.6, 2.1, 2.4};
  TH1D* hEta = new TH1D("hEta", ";#eta^{#mu};", 12, etaBins);
  hEta->Sumw2();

  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20", "mu2_eta_zmass_SA20_ME21", 1);
  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Global", "mu2_eta_zmass_GLB20_ME21", 1);
  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Medium", "mu2_eta_zmass_MED20_ME21", 1);
  MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Tight", "mu2_eta_zmass_TIGHT20_ME21", 1);

  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20", "mu2_eta_zmass_SA20_ME3141", 2);
  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Global", "mu2_eta_zmass_GLB20_ME3141", 2);
  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Medium", "mu2_eta_zmass_MED20_ME3141", 2);
  MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Tight", "mu2_eta_zmass_TIGHT20_ME3141", 2);

  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20", "mu2_eta_zmass_SA20_ME213141", 3);
  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Global", "mu2_eta_zmass_GLB20_ME213141", 3);
  // MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Medium", "mu2_eta_zmass_MED20_ME213141", 3);
  MakePlot("mu2TLV.Eta()", hEta, mumuMass+"mu2TLV.Pt()>20&&mu2Tight", "mu2_eta_zmass_TIGHT20_ME213141", 3);
  
   



}
