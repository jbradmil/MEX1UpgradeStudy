
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
#include "TGraphAsymmErrors.h"
#include "TPaveText.h"
#include "inc/jack_style.h"
// #ifdef __CINT__
// #pragma link C++ class std::vector<TLorentzVector>+;
// #endif

using namespace std;

TString plotdir = "Plotting/plots/613/data_vs_mc/";
TString datadir = "Plotting/hists/613/";
TString mcdir = "Plotting/hists/613/MC/";


void MakePlot(TString hname, TString tag, double ymax, double ymin)
{

  TString data_file = datadir+hname+".root";
  TString mc_file = mcdir+hname+".root";
  
  TFile* fData=TFile::Open(data_file.Data());
  TFile* fMC=TFile::Open(mc_file.Data());

  // Book histograms
  TH1D * hdata_0p5 = (TH1D*)fData->Get(Form("H%s_loss", tag.Data())); 
  // TH1D * hdata_1p0 = (TH1D*)fData->Get("H1p0_loss");
  TH1D * hmc_0p5 = (TH1D*)fMC->Get(Form("H%s_loss", tag.Data())); 
  //  TH1D * hmc_1p0 = (TH1D*)fMC->Get("H1p0_loss"); 

  // hdata_0p5->SetLineColor(2000);
  // hdata_0p5->SetLineWidth(1);
  // hdata_1p0->SetLineColor(2002);
  // hdata_1p0->SetLineWidth(1);
  //  hmc_0p5->SetLineWidth(0);


  //  cout << hmc_0p5->GetYaxis()->GetTitleOffset() << endl;
  hmc_0p5->GetYaxis()->SetTitleSize(0.04);
  hmc_0p5->GetYaxis()->SetTitleOffset(1.4);
  
  // hmc_1p0->SetLineColor(46);
  // hdata_0p5->SetMarkerStyle(20);
  // hdata_1p0->SetMarkerStyle(20);

  set_style(hdata_0p5, "data_obs");
  hdata_0p5->SetMarkerColor(1);
  hdata_0p5->SetLineWidth(0);
  hdata_0p5->SetLineColor(0);
  
  const int nbins = hmc_0p5->GetNbinsX();
  Double_t xvals[nbins];
  Double_t yvals_mc[nbins];
  Double_t yvals_data[nbins];
  Double_t exup_mc[nbins];
  Double_t exdown_mc[nbins];
  Double_t exup_data[nbins];
  Double_t exdown_data[nbins];
  Double_t eyup_mc[nbins];
  Double_t eydown_mc[nbins];
  Double_t eyup_data[nbins];
  Double_t eydown_data[nbins];
  
  for (int ibin(0); ibin<nbins; ibin++) {
    if (hmc_0p5->GetBinContent(ibin+1)>=0) hmc_0p5->SetBinContent(ibin+1,-0.0000001);
    if (hdata_0p5->GetBinContent(ibin+1)>=0) hdata_0p5->SetBinContent(ibin+1,-0.0000001);
    xvals[ibin] = hmc_0p5->GetBinCenter(ibin+1);
    yvals_mc[ibin] = hmc_0p5->GetBinContent(ibin+1);
    yvals_data[ibin] = hdata_0p5->GetBinContent(ibin+1);
    exup_mc[ibin] = hmc_0p5->GetBinWidth(ibin+1)/2;
    exdown_mc[ibin] = hmc_0p5->GetBinWidth(ibin+1)/2;
    exup_data[ibin] = 0.00001;
    exdown_data[ibin] = 0.00001;
    eyup_mc[ibin] = hmc_0p5->GetBinError(ibin+1);
    if (abs(hmc_0p5->GetBinError(ibin+1))>abs(hmc_0p5->GetBinContent(ibin+1))) eyup_mc[ibin]=abs(yvals_mc[ibin]);
    eydown_mc[ibin] = hmc_0p5->GetBinError(ibin+1);
    eyup_data[ibin] = hdata_0p5->GetBinError(ibin+1);
    if (abs(hdata_0p5->GetBinError(ibin+1))>abs(hdata_0p5->GetBinContent(ibin+1))) eyup_data[ibin]=abs(yvals_data[ibin]);
    eydown_data[ibin] = hdata_0p5->GetBinError(ibin+1);
    printf("Bin %d: %f + %f - %f\n", ibin+1, yvals_data[ibin], eyup_data[ibin], eydown_data[ibin]);
  }

  TGraphAsymmErrors* gmc = new TGraphAsymmErrors(nbins,xvals,yvals_mc,exdown_mc,exup_mc,eydown_mc,eyup_mc);
  TGraphAsymmErrors* gdata = new TGraphAsymmErrors(nbins,xvals,yvals_data,exdown_data,exup_data,eydown_data,eyup_data);
    if (tag=="0p5") {
    hmc_0p5->SetLineColor(38);
    //    hmc_0p5->SetFillColor(38);
    gmc->SetFillColor(38);
  }
  else {
    hmc_0p5->SetLineColor(46);
    //hmc_0p5->SetFillColor(46);
    gmc->SetFillColor(46);
  }
    gmc->SetFillStyle(3445);
    gdata->SetMarkerSize(1);
    gdata->SetLineWidth(1);
    gdata->SetMarkerStyle(20);
    gdata->SetLineColor(1);
    //hmc_0p5->SetFillStyle(3445);

    TH1D* hdummc = (TH1D*)hmc_0p5->Clone("hdummc");
    hdummc->SetFillColor(hdummc->GetLineColor());
    hdummc->SetFillStyle(3445);
      
  TH1D * ratio_0p5 = (TH1D *) hdata_0p5->Clone("ratio");
  ratio_0p5->Add(hmc_0p5,-1.);
  ratio_0p5->Divide(ratio_0p5,hdata_0p5,1,1);
  TGraphAsymmErrors* d_ratio = (TGraphAsymmErrors*)gdata->Clone();
  TGraphAsymmErrors* stat_ratio = (TGraphAsymmErrors*)gmc->Clone();
  for (int ibin(0); ibin<nbins; ibin++) {
    // if (abs(hdata_0p5->GetBinContent(ibin+1))>0) ratio_0p5->SetBinError(ibin+1, abs(hdata_0p5->GetBinError(ibin+1)/hdata_0p5->GetBinContent(ibin+1)));
    d_ratio->SetPoint(ibin, hdata_0p5->GetBinCenter(ibin+1), ratio_0p5->GetBinContent(ibin+1));
    stat_ratio->SetPoint(ibin, hmc_0p5->GetBinCenter(ibin+1), 0);
    if (hdata_0p5->GetBinContent(ibin+1)<=abs(hmc_0p5->GetBinContent(ibin+1))) {
      d_ratio->SetPointEYhigh(ibin, abs(gdata->GetErrorYlow(ibin)/hdata_0p5->GetBinContent(ibin+1)));
      d_ratio->SetPointEYlow(ibin, abs(gdata->GetErrorYhigh(ibin)/hdata_0p5->GetBinContent(ibin+1)));
      stat_ratio->SetPointEYhigh(ibin, abs(gmc->GetErrorYlow(ibin)/hdata_0p5->GetBinContent(ibin+1)));
      stat_ratio->SetPointEYlow(ibin, abs(gmc->GetErrorYhigh(ibin)/hdata_0p5->GetBinContent(ibin+1)));
    } else {
      d_ratio->SetPointEYhigh(ibin, abs(gdata->GetErrorYhigh(ibin)/hdata_0p5->GetBinContent(ibin+1)));
      d_ratio->SetPointEYlow(ibin, abs(gdata->GetErrorYlow(ibin)/hdata_0p5->GetBinContent(ibin+1)));
      stat_ratio->SetPointEYhigh(ibin, abs(gmc->GetErrorYhigh(ibin)/hdata_0p5->GetBinContent(ibin+1)));
      stat_ratio->SetPointEYlow(ibin, abs(gmc->GetErrorYlow(ibin)/hdata_0p5->GetBinContent(ibin+1)));
    }
    if (hmc_0p5->GetBinContent(ibin+1)==-0.0000001) {
       stat_ratio->SetPointEYlow(ibin, 0);
       // stat_ratio->SetPointEYhigh(ibin, 2);
    }
    if (hdata_0p5->GetBinContent(ibin+1)==-0.0000001) {
       d_ratio->SetPointEYlow(ibin, 0);
       // d_ratio->SetPointEYhigh(ibin, 2);
    }
     if (hmc_0p5->GetBinContent(ibin+1)==-1) {
          stat_ratio->SetPointEYhigh(ibin, 0);
     }
     if (hdata_0p5->GetBinContent(ibin+1)==-1) {
       d_ratio->SetPointEYhigh(ibin, 0);
     }
  }
  
  
  // TH1D * ratio_1p0 = (TH1D *) hdata_1p0->Clone("ratio_1p0");
  // ratio_1p0->Add(hmc_1p0,-1.);
  // ratio_1p0->Divide(ratio_1p0,hdata_1p0,1,1);


  //  ratio_0p5->Sumw2();
  ratio_0p5->SetStats(0);
  ratio_0p5->SetTitle(hmc_0p5->GetTitle());
  ratio_0p5->GetYaxis()->SetTitle("#splitline{(Data-MC)/Data}{Double ratio}");

  //ratio_0p5->SetFillColor(kRed);
  ratio_0p5->GetXaxis()->SetLabelSize(0.15);
  ratio_0p5->GetXaxis()->SetLabelOffset(0.03);
  ratio_0p5->GetXaxis()->SetTitleSize(0.14);
  ratio_0p5->GetXaxis()->SetTitleOffset(1.2);
  ratio_0p5->GetYaxis()->SetLabelSize(0.10);
  ratio_0p5->GetYaxis()->SetTitleSize(0.12);
  ratio_0p5->GetYaxis()->SetTitleOffset(0.6);
  ratio_0p5->GetYaxis()->SetNdivisions(505);

  TLine* ratiozero = new TLine(hmc_0p5->GetBinLowEdge(1),0,hmc_0p5->GetBinLowEdge(hmc_0p5->GetNbinsX()+1),0);
  TLine* ratiounity = new TLine(hmc_0p5->GetBinLowEdge(1),1,hmc_0p5->GetBinLowEdge(hmc_0p5->GetNbinsX()+1),1);


  
  // Setup legends
  TLegend * leg1 = new TLegend(0.23,0.76,0.48,0.9);
  set_style(leg1, 0.04);
  if (tag=="0p5") leg1->SetHeader("50% Fail Rate");
  else leg1->SetHeader("100% Fail Rate");
  leg1->SetNColumns(4);
  leg1->AddEntry(hdata_0p5,"Data");
  // leg1->AddEntry(hdata_1p0,"100 % (Data)");
  leg1->AddEntry(hdummc,"MC");
  // leg1->AddEntry(hmc_1p0,"100 % (MC)");


  hdata_0p5->Print("all");
  //hmc_0p5->Print("all");

  hmc_0p5->SetMaximum(ymax);
  hmc_0p5->SetMinimum(ymin);
  
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
  // Draw hists
  hmc_0p5->Draw("hist");
  gmc->Draw("2 same");
  ratiozero->SetLineStyle(2);
  ratiozero->Draw();
  //  hmc_1p0->Draw("hist, e1, same");
  hmc_0p5->GetXaxis()->SetLabelSize(0.03);
  hmc_0p5->GetXaxis()->SetTitleSize(0.025);
  hmc_0p5->GetYaxis()->SetLabelSize(0.04);

  hdata_0p5->Draw("p, same");
  gdata->Draw("1 same");
 //  hdata_1p0->Draw("e1, same");
  hmc_0p5->GetXaxis()->SetLabelSize(0);

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
  ratiounity->SetLineStyle(1);
  ratio_0p5->SetMaximum(1.1);
  ratio_0p5->SetMinimum(-1.1);
  ratio_0p5->Draw("p");
  stat_ratio->Draw("2 same");
  d_ratio->Draw("1p same");
  //  ratio_1p0->Draw("e1,same");
  ratiozero->Draw();
  ratio_0p5->GetXaxis()->SetTitleSize(0.12);
  ratio_0p5->GetXaxis()->SetLabelSize(0.12);
  hmc_0p5->GetXaxis()->SetTitleSize(0.12);
  ratio_0p5->GetXaxis()->SetLabelSize(0.12);


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
  gPad->Print(plotdir+hname+"_"+tag+".pdf");

 
  // Clean up

  fData->Close();
  fMC->Close();


 delete ratiounity;
 delete ratiozero;
 delete leg1;
 //  delete latex;
   delete pad1;
   delete pad2;
   delete c1;



  cout << "MakePlots(): DONE!" << endl;


  return;
}

void CompareEffsDataMC() {

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
  gStyle->SetErrorX(0);

  MakePlot("mu2_eta_zmass_TIGHT20_ME21", "0p5", 0.05, -0.14);
  MakePlot("mu2_eta_zmass_TIGHT20_ME3141", "0p5", 0.14, -0.31);
  MakePlot("mu2_eta_zmass_TIGHT20_ME213141", "0p5", 0.38, -1);

  
  MakePlot("mu2_abseta_zmass_TIGHT20_ME21", "0p5", 0.05, -0.14);
  MakePlot("mu2_abseta_zmass_TIGHT20_ME21", "1p0", 0.05, -0.14);
  MakePlot("mu2_abseta_zmass_TIGHT20_ME3141", "0p5", 0.14, -0.31);
  MakePlot("mu2_abseta_zmass_TIGHT20_ME3141", "1p0", 0.14, -0.31);
  MakePlot("mu2_abseta_zmass_TIGHT20_ME213141", "0p5", 0.38, -1);
  MakePlot("mu2_abseta_zmass_TIGHT20_ME213141", "1p0", 0.38, -1);

  MakePlot("mu2_pt_zmass_ecTIGHT_ME21", "0p5", 0.14, -0.24);
  MakePlot("mu2_pt_zmass_ecTIGHT_ME21", "1p0", 0.14, -0.24);
  MakePlot("mu2_pt_zmass_ecTIGHT_ME3141", "0p5", 0.14, -0.31);
  MakePlot("mu2_pt_zmass_ecTIGHT_ME3141", "1p0", 0.14, -0.31);
  MakePlot("mu2_pt_zmass_ecTIGHT_ME213141", "0p5", 0.38, -1);
  MakePlot("mu2_pt_zmass_ecTIGHT_ME213141", "1p0", 0.38, -1);



    
  return;
  
}

