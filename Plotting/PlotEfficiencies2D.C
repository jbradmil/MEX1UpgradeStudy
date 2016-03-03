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
#include "TH2.h"

#include "Plotting/inc/jack_style.h"

// #include "macros/include/cms_style.h"

#include <iostream>

using namespace std;

void MakePlot(const TString sample, const TCut cuts, const TString plotTitle, const TString input_directory = "NTupleMaker/test/") 
{ 

  TH2::StatOverflows(true);
  TH2::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


 

  //  gROOT->SetStyle("CMS");
  gStyle->SetOptStat(0);

  // TCut baseline ="ht50>=400&&met>150&&weightppb<1.";
  // TCut lep_veto("num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  // baseline = baseline+lep_veto+xtraCuts;


  
  TChain * ch[5][5];
  for (unsigned int ix(0); ix<5; ix++) {
    for (unsigned int iy(0); iy<5; iy++) {
      ch[ix][iy] = new TChain("skim/RecoTree");
      double failrate_21 = 0.25*ix;
      double failrate_31 = 0.25*iy;
      TString filename = Form("%s%s_me21_%3.2f_me31_me41_%3.2f.root", input_directory.Data(), sample.Data(), failrate_21, failrate_31);
      filename.ReplaceAll(".00", ".0");
      filename.ReplaceAll(".50", ".5");
      filename.ReplaceAll("_0.", "_0p");
      filename.ReplaceAll("_1.", "_1p");
      ch[ix][iy]->Add(filename);
      // printf("ch[%d][%d]->Add(%s);\n", ix, iy, filename.Data());
    }
  }

  TH2D * H_pass = new TH2D("H_pass","StandAlone Reconstruction Efficiency;Fail Rate ME+/-2/1;Fail Rate ME+/-3/1 & ME+/-4/1", 5, -0.125, 1.125, 5, -0.125, 1.125);
  TH2D * H_all = new TH2D("H_all","StandAlone Reconstruction Efficiency;Fail Rate ME+/-2/1;Fail Rate ME+/-3/1 & ME+/-4/1", 5, -0.125, 1.125, 5, -0.125, 1.125);
  TH2D * H_eff = new TH2D("H_eff","StandAlone Reconstruction Efficiency;Fail Rate ME+/-2/1;Fail Rate ME+/-3/1 & ME+/-4/1", 5, -0.125, 1.125, 5, -0.125, 1.125);

  TH1D* htemp = new TH1D("htemp","",2,0,2);
  
  for (unsigned int ix(0); ix<5; ix++) {
    for (unsigned int iy(0); iy<5; iy++) {
    ch[ix][iy]->Project("htemp", "1", cuts);
    H_all->SetBinContent(ix+1,iy+1, htemp->Integral());
    ch[ix][iy]->Project("htemp", "1", cuts+"GenRecoMatch");
    H_pass->SetBinContent(ix+1,iy+1, htemp->Integral());
    }
  }
 

  H_eff->Divide(H_pass, H_all, 1., 1., "B");


  int W = 800;
  int H = 800;

  TCanvas* thecanvas = new TCanvas("canvName","canvName",50,50,W,H);
  gStyle->SetPalette(56);
  
  H_eff->SetMaximum(1.);
  H_eff->SetMinimum(0.5);

  
  H_eff->Draw("colz, text, e");
  H_eff->GetYaxis()->SetTitleOffset(1.25);
  H_eff->GetZaxis()->SetLabelSize(0.025);
  for (unsigned int i(0); i<5; i++) {
    H_eff->GetXaxis()->SetBinLabel(i+1, Form("%3.2f",0.25*i));
    H_eff->GetYaxis()->SetBinLabel(i+1, Form("%3.2f",0.25*i));
  }

  thecanvas->Print("Plotting/plots/2D/"+plotTitle+".pdf");

  thecanvas->SetLeftMargin(0.15);
  thecanvas->SetRightMargin(0.15);
  
  H_eff->Print("all");

  delete H_pass;
  delete H_all;
  delete H_eff;
  delete thecanvas;
  delete htemp;
}


void PlotEfficiencies2D() {

  gStyle->SetPaintTextFormat("3.2f");
  
  MakePlot("HZZ", "GenMotherID==25||GenMotherID==23", "HZZ_all_pt_eta");
  // MakePlot("Zprime", "GenMotherID==32", "Zprime_all_pt_eta");
  // MakePlot("VBF", "GenMotherID==25", "VBF_all_pt_eta");


}
