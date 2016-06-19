
#include <iostream>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "TFileCollection.h"
#include "TLorentzVector.h"
#include "TPaveText.h"
#include "inc/jack_style.h"
// #ifdef __CINT__
// #pragma link C++ class std::vector<TLorentzVector>+;
// #endif

using namespace std;

TString plotdir = "plots/data_mc_dists/";



void MakePlot(TString var, TCut cut, TTree* data_ch, TTree* mc_ch, TH1D* htemp, TString plot_title, bool logy=false)
{

  

  // Book histograms
  TH1D * hdata_obs = (TH1D*)htemp->Clone("data_obs"); 
  TH1D * hmc_exp = (TH1D*)htemp->Clone("mc_exp");


  data_ch->Project("data_obs",var,cut);
  mc_ch->Project("mc_exp",var,cut);

  double SF = hdata_obs->Integral()/hmc_exp->Integral();
  hmc_exp->Scale(SF);

  // double xup = htemp->GetBinLowEdge(
  // double binwidth = (xup - xlow) / nbinsx;
  TString ytitle = Form("Events");
  hmc_exp->GetXaxis()->SetTitle(hmc_exp->GetXaxis()->GetTitle());
  hmc_exp->GetYaxis()->SetTitle(ytitle);
   
  // Setup histogram styles
  set_style_pastel(hmc_exp, "wlnug");
  set_style(hdata_obs, "data_obs");

  
  // Setup auxiliary histograms (ratios, errors, etc)
  TH1D * staterr = (TH1D *) hmc_exp->Clone("staterr");
  //  staterr->Sumw2();
  //staterr->SetFillColor(kRed);
  staterr->SetFillColor(1);
  staterr->SetLineColor(0);
  staterr->SetMarkerSize(0);
  staterr->SetLineWidth(0);
  staterr->SetFillStyle(3445);

  //cout << "Compute ratio hist..." << endl;
  TH1D * ratio = (TH1D *) hdata_obs->Clone("ratio");
  ratio->Divide(hdata_obs,hmc_exp);


  //  ratio->Sumw2();
  ratio->SetMarkerSize(0);
  ratio->SetLineWidth(3);
  ratio->SetStats(0);
  ratio->SetTitle(hmc_exp->GetTitle());
  ratio->GetYaxis()->SetTitle("Data/MC");
  ratio->SetMaximum(1.2);
  ratio->SetMinimum(0.075);
  ratio->SetMarkerSize(0);
  //ratio->SetFillColor(kRed);
  ratio->GetXaxis()->SetLabelSize(0.15);
  ratio->GetXaxis()->SetLabelOffset(0.03);
  ratio->GetXaxis()->SetTitleSize(0.14);
  ratio->GetXaxis()->SetTitleOffset(1.2);
  ratio->GetYaxis()->SetLabelSize(0.10);
  ratio->GetYaxis()->SetTitleSize(0.12);
  ratio->GetYaxis()->SetTitleOffset(0.6);
  ratio->GetYaxis()->SetNdivisions(505);

  TLine* ratiounity = new TLine(hmc_exp->GetBinLowEdge(1),1,hmc_exp->GetBinLowEdge(hmc_exp->GetNbinsX()+1),1);


  
  // Setup legends
  TLegend * leg1 = new TLegend(0.73, 0.7, 0.95, 0.92);
  set_style(leg1,0.035);
  leg1->AddEntry(hdata_obs, "Data", "pel");
  leg1->AddEntry(hmc_exp, "DY MC", "f");

  double ymax = hmc_exp->GetMaximum();
  if (hdata_obs->GetMaximum()>ymax) ymax=hdata_obs->GetMaximum();


  // Setup canvas and pads  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  pad1->SetBottomMargin(0.0);
  pad1->Draw();
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  pad2->SetTopMargin(0.0);
  pad2->SetBottomMargin(0.35);
  pad2->Draw();
  pad1->cd();
  pad1->SetLogy(logy);
  // Draw hists
  hmc_exp->Draw("hist");
  hmc_exp->GetXaxis()->SetLabelSize(0.03);
  hmc_exp->GetXaxis()->SetTitleSize(0.025);
  hmc_exp->GetYaxis()->SetLabelSize(0.04);
  hmc_exp->SetMarkerSize(0);
  hmc_exp->SetLineWidth(0);
  staterr->Draw("e2, same");
  hdata_obs->Draw("e1, same");
  hmc_exp->GetXaxis()->SetLabelSize(0);

  // Draw legends
  leg1->Draw();
  // TLatex * latex = new TLatex();
  // latex->SetNDC();
  // latex->SetTextAlign(12);
  // latex->SetTextFont(62);
  // latex->SetTextSize(0.052);
  // latex->DrawLatex(0.19, 0.89, "RA2/b Simulation");
  // latex->SetTextSize(0.04);
  // latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 20 fb^{-1}");

  pad2->cd();
  pad2->SetGridy(0);
  // ratio->GetXaxis()->SetRangeUser(0,6);
  //  pad2->SetLogy();
  ratiounity->SetLineStyle(2);
  set_style(ratio, "data_obs");
  ratio->SetMaximum(1.5);
  ratio->SetMinimum(0.5);
  ratio->Draw("e1");
  ratiounity->Draw();
  ratio->GetXaxis()->SetTitleSize(0.12);
  ratio->GetXaxis()->SetLabelSize(0.12);
  hmc_exp->GetXaxis()->SetTitleSize(0.12);
  ratio->GetXaxis()->SetLabelSize(0.12);

  TPaveText * pave = new TPaveText(0.18, 0.86, 0.4, 0.96, "brNDC");
  pave->SetLineColor(0);
  pave->SetFillColor(kWhite);
  pave->SetShadowColor(0);
  pave->SetBorderSize(1);
  // double nchisq = hdata_obs->Chi2Test(hmc_exp, "UWCHI2/NDF"); // MC uncert. (stat)
  // double p_value = hdata_obs->Chi2Test(hmc_exp, "UW"); // MC uncert. (stat)
  // //double kolprob = hdata_obs->KolmogorovTest(hmc_pred); // MC uncert. (stat)
  // TText * text = pave->AddText(Form("#chi_{#nu}^{2}/ndf = %.3f, p = %.3f", nchisq, p_value));
  // // //TText * text = pave->AddText(Form("#chi_{#nu}^{2} = %.3f, K_{s} = %.3f", nchisq, kolprob));
  // text->SetTextFont(62);
  // text->SetTextSize(0.07);
  // // text->SetTextSize(0.06);
  // //
  // pave->Draw();
    

  // if (plotData) pave->Draw();

  pad1->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();
  pad2->cd();
  gPad->RedrawAxis();
  gPad->Modified();
  gPad->Update();

  c1->cd();

  c1->cd();
  gPad->Print(plotdir+plot_title+".pdf");

 
  // Clean up

  
  delete hdata_obs;
  delete hmc_exp;


  delete staterr;
  delete ratio;
  delete ratiounity;
  delete leg1;
  //  delete latex;
  delete pave;
  delete pad1;
  delete pad2;
  delete c1;


  cout << "MakePlots(): DONE!" << endl;


  return;
}

void CompareDataMCDists() {

  TH1::SetDefaultSumw2(1);
  //gROOT->SetBatch(1);


  if (gSystem->AccessPathName(plotdir))
    gSystem->mkdir(plotdir);

  // Setup style
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();
  gStyle->SetHatchesLineWidth(1);
  gStyle->SetHatchesSpacing(1);
  
  TChain * data_ch0p0_ch0p0 = new TChain("Tree");
  data_ch0p0_ch0p0->Add("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-0.00-0.00-0.00.root");
  TChain * mc_ch0p0_ch0p0 = new TChain("Tree");
  mc_ch0p0_ch0p0->Add("~/muon_trees/trees/DYMC/skims/DY-FR-0.00-0.00-0.00.root");
  TChain * data_ch0p5_ch0p0 = new TChain("Tree");
  TChain * data_ch0p0_ch0p5 = new TChain("Tree");
  TChain * data_ch0p5_ch0p5 = new TChain("Tree");
  TChain * data_ch1p0_ch0p0 = new TChain("Tree");
  TChain * data_ch0p0_ch1p0 = new TChain("Tree");
  TChain * data_ch1p0_ch1p0 = new TChain("Tree");
  TChain * mc_ch0p5_ch0p0 = new TChain("Tree");
  TChain * mc_ch0p0_ch0p5 = new TChain("Tree");
  TChain * mc_ch0p5_ch0p5 = new TChain("Tree");
  TChain * mc_ch1p0_ch0p0 = new TChain("Tree");
  TChain * mc_ch0p0_ch1p0 = new TChain("Tree");
  TChain * mc_ch1p0_ch1p0 = new TChain("Tree");
  data_ch0p5_ch0p0->Add("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-0.50-0.00-0.00.root");
  data_ch1p0_ch0p0->Add("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-1.00-0.00-0.00.root");
  mc_ch0p5_ch0p0->Add("~/muon_trees/trees/DYMC/skims/DY-FR-0.50-0.00-0.00.root");
  mc_ch1p0_ch0p0->Add("~/muon_trees/trees/DYMC/skims/DY-FR-1.00-0.00-0.00.root");
  data_ch0p0_ch0p5->Add("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-0.00-0.50-0.50.root");
  data_ch0p0_ch0p0->Add("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-0.00-1.00-1.00.root");
  mc_ch0p0_ch0p5->Add("~/muon_trees/trees/DYMC/skims/DY-FR-0.00-0.50-0.50.root");
  mc_ch0p0_ch0p0->Add("~/muon_trees/trees/DYMC/skims/DY-FR-0.00-1.00-1.00.root");
  data_ch0p5_ch0p5->Add("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-0.50-0.50-0.50.root");
  data_ch1p0_ch1p0->Add("~/muon_trees/trees/SMuData/skims/SingleMuon-FR-1.00-1.00-1.00.root");
  mc_ch0p5_ch0p5->Add("~/muon_trees/trees/DYMC/skims/DY-FR-0.50-0.50-0.50.root");
  mc_ch1p0_ch1p0->Add("~/muon_trees/trees/DYMC/skims/DY-FR-1.00-1.00-1.00.root");
  

  TCut mumuMass = "mumuMass<101&&mumuMass>81";
  TCut mu2EC = "abs(mu2TLV.Eta())>1.6&&abs(mu2TLV.Eta())<2.4";
  TCut TT = "mu1Tight && mu2Tight";
  TCut TT20 = "mu1Tight && mu2Tight && mu1TLV.Pt()>20 && mu2TLV.Pt() > 20";


  TH1D* hMass = new TH1D("hMass", ";m_{#mu#mu} [GeV];", 20, 71, 111);
  MakePlot("mumuMass", TT, data_ch0p0_ch0p0, mc_ch0p0_ch0p0, hMass, "mass_TT_nominal_data_mc");
  MakePlot("mumuMass", TT20, data_ch0p0_ch0p0, mc_ch0p0_ch0p0, hMass, "mass_TT20_nominal_data_mc");

  Double_t etaBins[13] = {-2.4, -2.1, -1.6, -1.2, -1.04, -0.8, 0, 0.8, 1.04, 1.2, 1.6, 2.1, 2.4};
  TH1D* hEta = new TH1D("hEta", ";#eta^{#mu};", 12, etaBins);
  MakePlot("mu2TLV.Eta()", TT, data_ch0p0_ch0p0, mc_ch0p0_ch0p0, hEta, "mu2eta_TT_nominal_data_mc");
  MakePlot("mu2TLV.Eta()", TT20, data_ch0p0_ch0p0, mc_ch0p0_ch0p0, hEta, "mu2eta_TT20_nominal_data_mc");

  TH1D* hAbsEta = new TH1D("hAbsEta", ";|#eta^{#mu}|;", 8, 1.6, 2.4);
  MakePlot("abs(mu2TLV.Eta())", TT, data_ch0p0_ch0p0, mc_ch0p0_ch0p0, hAbsEta, "mu2abseta_TT_nominal_data_mc");
  MakePlot("abs(mu2TLV.Eta())", TT20, data_ch0p0_ch0p0, mc_ch0p0_ch0p0, hAbsEta, "mu2abseta_TT20_nominal_data_mc");

  TH1D* hPt = new TH1D("hPt", ";p_{T}^{#mu} [GeV];", 20, 10, 220);
  MakePlot("mu2TLV.Pt()", TT+mumuMass, data_ch0p0_ch0p0, mc_ch0p0_ch0p0, hPt, "mu2pt_TTmass_nominal_data_mc");

    
  return;
  
}

