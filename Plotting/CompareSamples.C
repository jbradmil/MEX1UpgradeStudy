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

void MakePlot(const TString xvar, TH1D* htemplate, const TCut cuts, const TString plotTitle, bool logx = false, bool logy=false, bool precise=false, const TString input_directory = "NTupleMaker/test/") 
{ 

  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


 

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);

  // TCut baseline ="ht50>=400&&met>150&&weightppb<1.";
  // TCut lep_veto("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // baseline = baseline+lep_veto+xtraCuts;


  
  TChain * chDY = new TChain("skim/RecoTree");
  TChain * chZprime = new TChain("skim/RecoTree");
  TChain * chVBF = new TChain("skim/RecoTree");
  chDY->Add(input_directory+"DY_me21_me31_me41_0p0.root");
  chZprime->Add(input_directory+"Zprime_me21_me31_me41_0p0.root");
  chVBF->Add(input_directory+"VBF_me21_me31_me41_0p0.root");
 
  
  TH1D * HDY = (TH1D*)htemplate->Clone("HDY");
  TH1D * HZprime = (TH1D*)htemplate->Clone("HZprime");
  TH1D * HVBF = (TH1D*)htemplate->Clone("HVBF");

  HDY->SetLineColor(1);
  HZprime->SetLineColor(2000);
  HVBF->SetLineColor(2004);
  HDY->SetMarkerColor(1);
  HZprime->SetMarkerColor(2000);
  HVBF->SetMarkerColor(2004);
  HZprime->SetLineStyle(9);
  HVBF->SetLineStyle(7);
 

  Width_t width=2;
  HDY->SetLineWidth(width);
  HZprime->SetLineWidth(3);
  HVBF->SetLineWidth(3);


  chDY->Project("HDY", xvar, cuts);
  chZprime->Project("HZprime", xvar, cuts);
  chVBF->Project("HVBF", xvar, cuts);

  double mu_DY = HDY->GetMean();
  double rms_DY = HDY->GetRMS();
  double mu_Zprime = HZprime->GetMean();
  double rms_Zprime = HZprime->GetRMS();
  double mu_VBF = HVBF->GetMean();
  double rms_VBF = HVBF->GetRMS();
  double mu_err_DY = HDY->GetMeanError();
  double rms_err_DY = HDY->GetRMSError();
  double mu_err_Zprime = HZprime->GetMeanError();
  double rms_err_Zprime = HZprime->GetRMSError();
  double mu_err_VBF = HVBF->GetMeanError();
  double rms_err_VBF = HVBF->GetRMSError();
  
  HDY->Scale(1/HDY->Integral());
  HZprime->Scale(1/HZprime->Integral());
  HVBF->Scale(1/HVBF->Integral());


  HDY->GetYaxis()->SetTitleOffset(1.3);
  HDY->GetYaxis()->SetTitle("Fraction of muons");
  double ymax = HDY->GetMaximum();
  if (HZprime->GetMaximum()>ymax)ymax=HZprime->GetMaximum();
  if (logy)  HDY->SetMaximum(ymax*17);
  else HDY->SetMaximum(ymax*2);
  if (logy) HDY->SetMinimum(0.005);
  else HDY->SetMinimum(0);

  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetLogx(logx);
  c1->SetLogy(logy);


  TLegend * leg = new TLegend(0.13,0.66,0.43,0.88);
  set_style(leg, 0.035);
  leg->SetHeader("Process");
  if (precise) {
    leg->AddEntry(HDY, Form("DY (#mu = %3.2f#pm%3.2f)", mu_DY, mu_err_DY, rms_DY, rms_err_DY), "l");
    leg->AddEntry(HZprime, Form("Z' [M = 2500 GeV] (#mu = %3.2f#pm%3.2f)", mu_Zprime, mu_err_Zprime, rms_Zprime, rms_err_Zprime), "l");
    leg->AddEntry(HVBF, Form("VBF h (#mu = %3.2f#pm%3.2f)", mu_VBF, mu_err_VBF, rms_VBF, rms_err_VBF), "l");
  } else {
    leg->AddEntry(HDY, Form("DY (#mu = %3.1f#pm%3.1f)", mu_DY, mu_err_DY, rms_DY, rms_err_DY), "l");
    leg->AddEntry(HZprime, Form("Z' [M = 2500 GeV] (#mu = %3.1f#pm%3.1f)", mu_Zprime, mu_err_Zprime, rms_Zprime, rms_err_Zprime), "l");
    leg->AddEntry(HVBF, Form("VBF h (#mu = %3.1f#pm%3.1f)", mu_VBF, mu_err_VBF, rms_VBF, rms_err_VBF), "l");
  }
  
 


  HDY->GetXaxis()->SetLabelSize(0.03);
  HDY->GetXaxis()->SetTitleSize(0.03);
  HDY->GetXaxis()->SetTitleOffset(1.3);
  HDY->GetYaxis()->SetLabelSize(0.03);
  
  HDY->Draw("hist e"); 
  HZprime->Draw("hist e SAME");
  HVBF->Draw("hist e SAME"); 
  HDY->Draw("hist e SAME"); 

  leg->Draw();

  
  c1->Print("Plotting/plots/sample_comparison/"+plotTitle+"_samples.pdf");


  delete chDY;
  delete chZprime;
  delete chVBF;
  delete HDY;
  delete HZprime;
  delete HVBF;
  delete c1;
  delete leg;
}


void CompareSamples() {

  TH1D* hResolution = new TH1D("hResolution", ";[p_{T}^{RECO}-p_{T}^{GEN}]/p_{T}^{GEN};", 50, -1, 1);
  hResolution->Sumw2();
  MakePlot("muResolution", hResolution, "GenPt>0&&GenRecoMatch&&(GenMotherID==23||GenMotherID==25||GenMotherID==32)", "muon_Resolution", false, false, true);

  Double_t etaBins[19] = {-5, -4, -3, -2.4, -2.1, -1.6, -1.2, -1.04, -0.8, 0, 0.8, 1.04, 1.2, 1.6, 2.1, 2.4, 3, 4, 5};
  TH1D* hEta = new TH1D("hEta", ";#eta^{#mu};", 18, etaBins);
  hEta->Sumw2();
  MakePlot("GenEta", hEta, "GenPt>5&&(GenMotherID==23||GenMotherID==25||GenMotherID==32)", "muon_eta", false, false, true);
 
  
  Double_t ptBins[8] = {3, 20, 50, 100, 200, 500, 1000, 2000};
  TH1D* hPt = new TH1D("hPt", ";p_{T}^{#mu} [GeV];", 7, ptBins);
  hPt->Sumw2();
  MakePlot("GenPt", hPt, "abs(GenEta)<2.4&&(GenMotherID==23||GenMotherID==25||GenMotherID==32)", "muon_sa_pt_ME31_ME41", false, false);

  
}
