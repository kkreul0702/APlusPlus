#include "TGraph.h"
#include "TString.h"
#include "AtlasUtils.C"
#include "AtlasStyle.C"
#include "AtlasLabels.C"
#include <vector>
#include <string>
#include <cmath>

/////////////////////////////////////////////////////////////////////
// CREATED : 01/02/2016
// AUTHOR : Frederic Derue (LPNHE Paris)
// PURPOSE : ROOT macro to plot fake efficiencies vs different variables
//           It reads as input the default efficiencies of TopFakes package
// MODIFICATION : 
//
/////////////////////////////////////////////////////////////////////

void FakeEffElec()
{
  // choice the option you want - in which control region
  //int choice = 0; // CR1
  //int choice = 1; // CR1 vs CR2 0 b-tag
  //int choice = 2; // CR1 vs CR2 b-tag
  //int choice = 3; // CR1 MCup/down 0 b-tag
  //int choice = 4; // CR1 MCup/down b-tag
  //int choice = 5; // CR1 MC WZ 0 b-tag
  //int choice = 6; // CR1 MC WZ b-tag

  // set the variable one want to compare
  //int var = 0;  // pt
  //int var = 1;  // eta
  //int var = 2;  // dR
  //int var = 3;  // jetpt
  //int var = 4;  // dphi
  //int var = 5;  // met
  //int var = 6;  // mtw
  //int var = 7;  // njet
  //int var = 8;  // nbjet

  SetAtlasStyle();
  gROOT->Reset();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptTitle(0);
  
  // define the class 
  MyEff my(1);
  // uncomment if you want to do all plots in one go
  for (int choice==0;choice<=6;choice++)
    for (int var==0;var<=8;var++)
      my.execute(var,choice);
  
  return;
}

class MyEff {
  
public:
  
  MyEff(int choice) {}
  
  // ========================================
  void execute(int var, int choice) 
  {
    TGraph* plot0;
    TH1F* plot[10][10];
    TString myname="";
    TString name[100];
    TString mynamehist = "";
    TFile *f[10];

    // read input files
    // CR1
    myname = "..//data/F_el_Default.root";    
    f[0] = TFile::Open(myname,"old");
    // CR2
    myname = "../data/F_el_Default_CRfake.root";    
    f[1] = TFile::Open(myname,"old");
    // CR1 MCup
    myname = "../data/F_el_Default_MCup.root";    
    f[2] = TFile::Open(myname,"old");
    // CR1 MCdown
    myname = "../data/F_el_Default_MCdown.root";    
    f[3] = TFile::Open(myname,"old");
    // CR1 Powheg
    myname = "../data/F_el_Default_WZPowheg.root";    
    f[4] = TFile::Open(myname,"old");
    // CR1 MadGraph
    myname = "../data/F_el_Default_WZMG.root";    
    f[5] = TFile::Open(myname,"old");

    // loop on each files
    for (int ifile=0;ifile<=5;ifile++) {
      // loop on jet bins
      for (int ijet=0;ijet<=3;ijet++) {
	// check which variable to be used
	if (var==0) mynamehist = "eff_pt";
	if (var==1) mynamehist = "eff_eta";
	if (var==2) mynamehist = "eff_dR";
	if (var==3) mynamehist = "eff_jetpt";
	if (var==4) mynamehist = "eff_dPhi";
	if (var==5) mynamehist = "eff_met";
	if (var==6) mynamehist = "eff_mtw";
	if (var==7) mynamehist = "eff_njet";
	if (var==8) mynamehist = "eff_nbtag";
	
	if (var!=7) {
	  if (ijet==0) mynamehist  += "_0b_1j";
	  if (ijet==1) mynamehist  += "_0b_ge2j";
	  if (ijet==2) mynamehist  += "_ge1b_1j";
	  if (ijet==3) mynamehist  += "_ge1b_ge2j";
	} else {
	  if (ijet==0) mynamehist  += "_0b";
	  if (ijet==1) mynamehist  += "_0b_ge2j";
	  if (ijet==2) mynamehist  += "_ge1b";
	  if (ijet==3) mynamehist  += "_ge1b_ge2j";
	}
 
	std::cout << " ijet = " << ijet 
		  << " namehist = " << mynamehist << std::endl;
	plot[ifile][ijet] = (TH1F*)f[ifile]->Get(mynamehist);
	//std::cout << " plot = " << plot[ifile][ijet] << std::endl;
      }
    }

    // ===============================================
    // set the TAxis ranges
    m_nlines = 2;
    m_eff[0] = 0.;
    m_eff[1] = 0.8;
    if (var==0) {m_var[0] = 25.;  m_var[1] = 150.;}  // pt
    if (var==1) {m_var[0] = 0.;   m_var[1] = 2.3;}   // eta
    if (var==2) {m_var[0] = 0.;   m_var[1] = 6.;}    // dR
    if (var==3) {m_var[0] = 25.;  m_var[1] = 500.;}  // jetpt
    if (var==4) {m_var[0] = 0.;   m_var[1] = 3.2;}   // dphi
    if (var==5) {m_var[0] = 0.;   m_var[1] = 1000.;} // met
    if (var==6) {m_var[0] = 0.;   m_var[1] = 1000.;} // mtw
    if (var==7) {m_var[0] = 0.;   m_var[1] = 6.;}    // njet
    if (var==8) {m_var[0] = 0.;   m_var[1] = 3.;}    // nbjet
    plot0 = new TGraph(m_nlines,m_var,m_eff);

    // plot the TGraph
    plotGraph(var,choice,plot0,plot);
  }
  
  // ============================================== 
  void plotGraph(int var,int choice,TGraph* plot0,TH1F* plot[10][10])
  {
    // set the TCanvas
    c1 = new TCanvas("c1"," ",0.,0.,800,600);
    c1->SetFillColor(42);
    c1->SetFillColor(10);
    
    // plot the TAxis
    plot0->SetFillColor(19);
    plot0->SetLineColor(0);
    plot0->SetLineWidth(4);
    plot0->SetMarkerColor(0);
    plot0->SetMarkerStyle(20);
    //plot0->GetXaxis()->CenterLabels(true);
    if (var==0) plot0->GetXaxis()->SetTitle("E_{T}(elec) [GeV]");
    if (var==1) plot0->GetXaxis()->SetTitle("|#eta(elec)|");
    if (var==2) plot0->GetXaxis()->SetTitle("#DeltaR (elec, nearest jet)");
    if (var==3) plot0->GetXaxis()->SetTitle("p_{T}(leading jet) [GeV]");
    if (var==4) plot0->GetXaxis()->SetTitle("#Delta#phi (elec,E_{T}^{miss})");
    if (var==5) plot0->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
    if (var==6) plot0->GetXaxis()->SetTitle("m_{T}(elec,E_{T}^{miss}) [GeV]");
    if (var==7) plot0->GetXaxis()->SetTitle("n_{jet}");
    if (var==8) plot0->GetXaxis()->SetTitle("n_{b-jet}");
    plot0->GetYaxis()->SetTitle("Efficiency");
    plot0->Draw("AP");
    
    // ================================================================
    // plot the TGraph
    if (choice==0) { // CR1
      myplot(plot[0][0],1,20); 
      if (var!=7) myplot(plot[0][1],2,21);  // case for njet
      myplot(plot[0][2],3,24); 
      if (var!=7) myplot(plot[0][3],4,25);  // case for njet
      
      if (var==8) { // case for nbjet (use same colors)
	myplot(plot[0][0],1,20); 
	myplot(plot[0][1],3,24); 
	myplot(plot[0][2],1,20); 
	myplot(plot[0][3],3,24); 
      }
    } 
    if (choice==1) { // CR1 vs CR2 0 btag
      myplot(plot[0][0],1,20);  // 1j 0b
      myplot(plot[1][0],1,24); 
      myplot(plot[0][1],2,21);  // ge2j 0b
      myplot(plot[1][1],2,25); 
    }
    if (choice==2) { //CR1 vs CR2  btag
      myplot(plot[0][2],1,20);  // 1j ge1b
      myplot(plot[1][2],1,24); 
      myplot(plot[0][3],2,21);  // ge2j ge1b
      myplot(plot[1][3],2,25); 
    }
    if (choice==3) { // CR1 MCup/down 0 btag
      myplot(plot[0][0],1,20);  // 1j 0b
      myplot(plot[2][0],1,21); 
      myplot(plot[3][0],1,22); 

      myplot(plot[0][1],2,24);  // ge2j 0b
      myplot(plot[2][1],2,25); 
      myplot(plot[3][1],2,26); 
    }
    if (choice==4) { // CR1 MCup/down btag
      myplot(plot[0][2],1,20);  // 1j b
      myplot(plot[2][2],1,21); 
      myplot(plot[3][2],1,22); 

      myplot(plot[0][3],2,24);  // ge2j b
      myplot(plot[2][3],2,25); 
      myplot(plot[3][3],2,26); 
    }
    if (choice==5) { // CR1 MC WZ 0 btag
      myplot(plot[0][0],1,20);  // 1j 0b
      myplot(plot[4][0],1,21); 
      myplot(plot[5][0],1,22); 

      myplot(plot[0][1],2,24);  // ge2j 0b
      myplot(plot[4][1],2,25); 
      myplot(plot[5][1],2,26); 
    }
    if (choice==6) { // CR1 MC WZ btag
      myplot(plot[0][2],1,20);  // 1j b
      myplot(plot[4][2],1,21); 
      myplot(plot[5][2],1,22); 

      myplot(plot[0][3],2,24);  // ge2j b
      myplot(plot[4][3],2,25); 
      myplot(plot[5][3],2,26); 
    }

    c1->Update();
    c1->GetFrame()->SetFillColor(0);
    c1->GetFrame()->SetBorderSize(0);
    c1->Modified();
    // plot the TLegend
    plotLegend(var,choice,plot);   
    c1->RedrawAxis();

    // plot the output names
    TString suffix = "";
    if (var==0) suffix = "_vsEt";
    if (var==1) suffix = "_vsEta";
    if (var==2) suffix = "_vsdR";
    if (var==3) suffix = "_vsPtLeadingJet";
    if (var==4) suffix = "_vsdPhi";
    if (var==5) suffix = "_vsMET";
    if (var==6) suffix = "_vsMTW";
    if (var==7) suffix = "_vsnjet";
    if (var==8) suffix = "_vsnbjet";

    TString format = "";
    for (int i=0;i<3;i++) {
      if (i==0) format = ".eps";
      if (i==1) format = ".pdf";
      if (i==2) format = ".png";

      if (choice==0) // CR1
	c1->SaveAs("fig/efffake_elec"+suffix+format);
      if (choice==1) // CR1 vs CR2 0 btag
	c1->SaveAs("fig/efffake_elec_CR12_0btag"+suffix+format);
      if (choice==2) // CR1 vs CR2 ge1 btag
	c1->SaveAs("fig/efffake_elec_CR12_btag"+suffix+format);
      if (choice==3) // CR1 MCup/down 0 btag
	c1->SaveAs("fig/efffake_elec_CR1_MCUpDown_0btag"+suffix+format);
      if (choice==4) // CR1 MCup/down ge1 btag
	c1->SaveAs("fig/efffake_elec_CR1_MCUpDown_btag"+suffix+format);
      if (choice==5) // CR1 MC WZ 0 btag
	c1->SaveAs("fig/efffake_elec_CR1_MCWZ_0btag"+suffix+format);
      if (choice==6) // CR1 MC WZ ge1 btag
	c1->SaveAs("fig/efffake_elec_CR1_MCWZ_btag"+suffix+format);
    }
  }
  
  // ============================================== 
  void myplot(TH1F* plot,int mkcolor, int mktype, float size=1.5)
  {
    plot->SetFillColor(19);
    plot->SetLineColor(1);
    plot->SetLineWidth(1.5);
    plot->SetMarkerColor(mkcolor);
    plot->SetMarkerStyle(mktype);
    //plot->SetTitle("rel 16");
    plot->SetMarkerSize(size);
    plot->Draw("P sames");
  }

  // ============================================== 
  void plotLegend(int var,int choice,TH1F* plot[10][10])
  {
    float xlabel = 0.2; 
    float ylabel = 0.88;
    // set the ATLAS label
    ATLASLabel(xlabel,ylabel,"Work in Progress");
    // set energy and luminosity (adapt to your own case)
    myText(xlabel+0.1,ylabel-0.06,1,"e+jets, #sqrt{s}=13 TeV, 3.2 fb^{-1}",0.045);

    // ============================================================      
    // set the TLegend (adapt to your own team)
    if (choice==0) { 
      float ymin = 0.6;
      float ymax = 0.8;
      leg = new TLegend(0.2,ymin,0.3,ymax);
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);  
      if (var!=7 && var!=8) {
	leg->AddEntry(plot[0][0],"#epsilon_{f} CR_{1}, 1 jet, 0 btag","ep");
	leg->AddEntry(plot[0][1],"#epsilon_{f} CR_{1}, #geq2 jets, 0 btag","ep");
	leg->AddEntry(plot[0][2],"#epsilon_{f} CR_{1}, 1 jet, #geq1 btag","ep");
	leg->AddEntry(plot[0][3],"#epsilon_{f} CR_{1}, #geq2 jets, #geq1 btag","ep");
      } else if (var==7) { // case for njet
	leg->AddEntry(plot[0][0],"#epsilon_{f} CR_{1} 0 btag","ep");
	leg->AddEntry(plot[0][2],"#epsilon_{f} CR_{1} #geq1 btag","ep");
      } else if (var==8) { // case for nbjet
	leg->AddEntry(plot[0][0],"#epsilon_{f} CR_{1}, 1 jet","ep");
	leg->AddEntry(plot[0][1],"#epsilon_{f} CR_{1}, #geq2 jets","ep");
      }
      leg->Draw();
    } 

    // =============================
    if (choice==1) { // for comparison CR1 CR2
      float ymin = 0.65;
      float ymax = 0.8;
      leg = new TLegend(0.2,ymin,0.3,ymax);
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);  
      leg->AddEntry(plot[0][0],"#epsilon_{f} CR_{1}, 1 jet, 0 btag","ep");
      leg->AddEntry(plot[1][0],"#epsilon_{f} CR_{2}, 1 jet, 0 btag","ep");
      leg->Draw();

      float ymin2 = 0.65;
      float ymax2 = 0.8;
      leg2 = new TLegend(0.5,ymin2,0.6,ymax2);
      leg2->SetFillColor(0);
      leg2->SetFillStyle(0);
      leg2->SetBorderSize(0);
      leg2->SetTextSize(0.04);  
      leg2->AddEntry(plot[0][1],"#epsilon_{f} CR_{1}, #geq2 jets, 0 btag","ep");
      leg2->AddEntry(plot[1][1],"#epsilon_{f} CR_{2}, #geq2 jets, 0 btag","ep");
      leg2->Draw();
    }

    // =============================
    if (choice==2) { // for comparison CR1 CR2
      float ymin = 0.65;
      float ymax = 0.8;
      leg = new TLegend(0.2,ymin,0.3,ymax);
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);  
      leg->AddEntry(plot[0][2],"#epsilon_{f} CR_{1}, 1 jet, #geq1 btag","ep");
      leg->AddEntry(plot[1][2],"#epsilon_{f} CR_{2}, 1 jet, #geq1 btag","ep");
      leg->Draw();

      float ymin2 = 0.65;
      float ymax2 = 0.8;
      leg2 = new TLegend(0.5,ymin2,0.6,ymax2);
      leg2->SetFillColor(0);
      leg2->SetFillStyle(0);
      leg2->SetBorderSize(0);
      leg2->SetTextSize(0.04);  
      leg2->AddEntry(plot[0][3],"#epsilon_{f} CR_{1}, #geq2 jets, #geq1 btag","ep");
      leg2->AddEntry(plot[1][3],"#epsilon_{f} CR_{2}, #geq2 jets, #geq1 btag","ep");
      leg2->Draw();
    }
    // =============================
    if (choice==3) { // for comparison CR1 MCup/down 0 btag
      float ymin = 0.65;
      float ymax = 0.8;
      leg = new TLegend(0.2,ymin,0.3,ymax);
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.035);  
      leg->AddEntry(plot[0][0],"#epsilon_{f} CR_{1}, 1 jet, 0 btag","ep");
      leg->AddEntry(plot[2][0],"#epsilon_{f} CR_{1} MCup, 1 jet, 0 btag","ep");
      leg->AddEntry(plot[3][0],"#epsilon_{f} CR_{1} MCdown, 1 jet, 0 btag","ep");
      leg->Draw();

      float ymin2 = 0.65;
      float ymax2 = 0.8;
      leg2 = new TLegend(0.58,ymin2,0.68,ymax2);
      leg2->SetFillColor(0);
      leg2->SetFillStyle(0);
      leg2->SetBorderSize(0);
      leg2->SetTextSize(0.035);  
      leg2->AddEntry(plot[0][1],"#epsilon_{f} CR_{1}, #geq2 jets, 0 btag","ep");
      leg2->AddEntry(plot[2][1],"#epsilon_{f} CR_{1} MCup, #geq2 jets, 0 btag","ep");
      leg2->AddEntry(plot[3][1],"#epsilon_{f} CR_{1} down, #geq2 jets, 0 btag","ep");
      leg2->Draw();
    }

    // =============================
    if (choice==4) { // for comparison CR1 MCup/down >=1 btag
      float ymin = 0.65;
      float ymax = 0.8;
      leg = new TLegend(0.2,ymin,0.3,ymax);
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.035);  
      leg->AddEntry(plot[0][2],"#epsilon_{f} CR_{1}, 1 jet, #geq1 btag","ep");
      leg->AddEntry(plot[2][2],"#epsilon_{f} CR_{1} MCup, 1 jet, #geq1 btag","ep");
      leg->AddEntry(plot[3][2],"#epsilon_{f} CR_{1} MCdown, 1 jet, #geq1 btag","ep");
      leg->Draw();

      float ymin2 = 0.65;
      float ymax2 = 0.8;
      leg2 = new TLegend(0.58,ymin2,0.68,ymax2);
      leg2->SetFillColor(0);
      leg2->SetFillStyle(0);
      leg2->SetBorderSize(0);
      leg2->SetTextSize(0.035);  
      leg2->AddEntry(plot[0][3],"#epsilon_{f} CR_{1}, #geq2 jets, #geq1 btag","ep");
      leg2->AddEntry(plot[2][3],"#epsilon_{f} CR_{1} MCup, #geq2 jets, #geq1 btag","ep");
      leg2->AddEntry(plot[3][3],"#epsilon_{f} CR_{1} down, #geq2 jets, #geq1 btag","ep");
      leg2->Draw();
    }
    // =============================
    if (choice==5) { // for comparison CR1 MC WZ 0 btag
      float ymin = 0.65;
      float ymax = 0.8;
      leg = new TLegend(0.2,ymin,0.3,ymax);
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.03);  
      leg->AddEntry(plot[0][0],"#epsilon_{f} CR_{1} Sherpa, 1 jet, 0 btag","ep");
      leg->AddEntry(plot[4][0],"#epsilon_{f} CR_{1} Powheg, 1 jet, 0 btag","ep");
      leg->AddEntry(plot[5][0],"#epsilon_{f} CR_{1} MadGraph, 1 jet, 0 btag","ep");
      leg->Draw();

      float ymin2 = 0.65;
      float ymax2 = 0.8;
      leg2 = new TLegend(0.58,ymin2,0.68,ymax2);
      leg2->SetFillColor(0);
      leg2->SetFillStyle(0);
      leg2->SetBorderSize(0);
      leg2->SetTextSize(0.03);  
      leg2->AddEntry(plot[0][1],"#epsilon_{f} CR_{1} Sherpa, #geq2 jets, 0 btag","ep");
      leg2->AddEntry(plot[4][1],"#epsilon_{f} CR_{1} Powheg, #geq2 jets, 0 btag","ep");
      leg2->AddEntry(plot[5][1],"#epsilon_{f} CR_{1} MadGraph, #geq2 jets, 0 btag","ep");
      leg2->Draw();
    }

    // =============================
    if (choice==6) { // for comparison CR1 MC WZ >=1 btag
      float ymin = 0.65;
      float ymax = 0.8;
      leg = new TLegend(0.2,ymin,0.3,ymax);
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.03);  
      leg->AddEntry(plot[0][2],"#epsilon_{f} CR_{1} Sherpa, 1 jet, #geq1 btag","ep");
      leg->AddEntry(plot[4][2],"#epsilon_{f} CR_{1} Powheg, 1 jet, #geq1 btag","ep");
      leg->AddEntry(plot[5][2],"#epsilon_{f} CR_{1} MadGraph, 1 jet, #geq1 btag","ep");
      leg->Draw();

      float ymin2 = 0.65;
      float ymax2 = 0.8;
      leg2 = new TLegend(0.58,ymin2,0.68,ymax2);
      leg2->SetFillColor(0);
      leg2->SetFillStyle(0);
      leg2->SetBorderSize(0);
      leg2->SetTextSize(0.03);  
      leg2->AddEntry(plot[0][3],"#epsilon_{f} CR_{1} Sherpa, #geq2 jets, #geq1 btag","ep");
      leg2->AddEntry(plot[4][3],"#epsilon_{f} CR_{1} Powheg, #geq2 jets, #geq1 btag","ep");
      leg2->AddEntry(plot[5][3],"#epsilon_{f} CR_{1} MadGraph, #geq2 jets, #geq1 btag","ep");
      leg2->Draw();
    }

  }

private:
  
  Float_t m_var[300];
  Float_t m_errvar[300];
  Float_t m_eff[300];
  Float_t m_erreff[300];
  
  Int_t m_nlines;
};
