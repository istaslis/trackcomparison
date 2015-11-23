#include "TROOT.h"
#include "TStyle.h"
#include "TColor.h"
#include "TLatex.h"
#include "TSystem.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"
#include "TTreeReader.h"
#include "TCanvas.h"
#include "TFile.h"

void drawText(const char *text, float xp, float yp, int color = kBlack, int size=30){
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetTextFont(43);
  tex->SetTextSize(size);
  tex->SetTextColor(color);
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw("same");
}


void DrawCompare(TH1F *h1, TH1F *h2, int color1 = kBlack, int color2 = kBlue, TString legend1 = "Data", TString legend2 = "MC", TString title = "compare", TString caption = "A_{J}", bool divide = true, bool logy = false)
{
  TCanvas *c1 = new TCanvas(title,title,600,700);
  TPad *pad1 = new TPad("pad1","pad1",0,0.4,1,1);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  pad1->cd();

  float legendx1 = 0.65, legendy1 = 0.65, legendx2 = 0.84, legendy2 = 0.84;
  TLegend *l = new TLegend(legendx1, legendy1, legendx2, legendy2);

  l->AddEntry(h1, legend1.Data(), "P");
  l->AddEntry(h2, legend2.Data(), "P");

  h1->GetYaxis()->SetTitle(title);

  h1->SetMarkerColor(color1);
  h1->SetLineColor(color1);
  h2->SetMarkerColor(color2);
  h2->SetLineColor(color2);
  h1->Draw();
  h2->Draw("same");
 l->Draw();
  //TLegend* myleg=pad1->BuildLegend();
  //myleg->Draw();
  pad1->Draw();

  if (logy)
    pad1->SetLogy();

//  pad1->Modified();
//  pad1->Update();

//  l->Draw();



  //drawText(legend1.Data(),0.7,0.6,color1);
  //drawText(legend2.Data(),0.7,0.7,color2);
  //drawText(sel,0.18,0.8,kBlack,20);
  c1->cd();

  TPad *pad2 = new TPad("pad2","pad2",0,0.1,1,0.4);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.25);
  pad2->Draw();
  pad2->cd();

  TH1F *h3=(TH1F *)h1->Clone();//(TH1F *)h1->DrawCopy();
  //  h3->Sumw2();
  //  h3->SetStats(0);

  if (divide) {
    h3->Divide(h1,h2);
    h3->SetMinimum(-0.2);
    h3->SetMaximum(2.2);
  } else 
    h3->Add(h1,h2,1,-1);

  h3->GetXaxis()->SetTitle(divide ? "ratio" : "difference");
  h3->GetXaxis()->SetTitle(caption);
  h3->GetXaxis()->SetTitleOffset(3.5);
  //  drawText(var,0.18,0.8,kBlack,20);

  h3->SetMarkerStyle(21);
  h3->Draw("ep");

  gPad->Modified(); gPad->Update(); // make sure gPad is updated

  float y = divide ? 1 : 0;

  TLine *line = new TLine(gPad->GetUxmin(), y, gPad->GetUxmax(), y);
  line->SetLineStyle(2);
  line->Draw();

  c1->cd();

  c1->SaveAs(Form("Compare_%s.pdf",h1->GetTitle()));//Form("Compare_%s_%s_%s.pdf",title.Data(), legend1.Data(), legend2.Data()));

}

int canvcounter = 0;

void DrawCompare(TH1F *&h1, TH1F *&h2,TH1F *&h3, TH1F *&h4, int color1 = kBlack, int color2 = kBlue,int color3 = kBlack, int color4 = kBlue, TString legend1 = "Data", TString legend2 = "MC", TString legend3 = "Data2", TString legend4 = "MC2", TString title = "compare", TString caption = "A_{J}", bool divide = true, bool logy = false)
{
  h1->Scale(1/h1->Integral());
  h2->Scale(1/h2->Integral());
  h3->Scale(1/h3->Integral());
  h4->Scale(1/h4->Integral());

  canvcounter++;

  //  TCanvas *c1 = new TCanvas("canv"+TString::Itoa(canvcounter,10),title,600,700);
  TCanvas *c1 = new TCanvas(title,title,600,700);
  TPad *pad1 = new TPad("pad1","pad1",0,0.4,1,1);
  pad1->SetBottomMargin(0);
  pad1->Draw();
  pad1->cd();

  float legendx1 = 0.65, legendy1 = 0.65, legendx2 = 0.84, legendy2 = 0.84;
  TLegend *l = new TLegend(legendx1, legendy1, legendx2, legendy2);

  l->AddEntry(h1, legend1.Data(), "P");
  l->AddEntry(h2, legend2.Data(), "P");
  l->AddEntry(h3, legend3.Data(), "P");
  l->AddEntry(h4, legend4.Data(), "P");

  h1->GetYaxis()->SetTitle(title);

  h1->SetMarkerColor(color1);
  h1->SetLineColor(color1);
  h2->SetMarkerColor(color2);  h2->SetLineColor(color2);
  h3->SetMarkerColor(color3);  h3->SetLineColor(color3);
  h4->SetMarkerColor(color4);  h4->SetLineColor(color4);
  h1->SetMarkerStyle(kFullCircle);  h2->SetMarkerStyle(kFullCircle);
  h3->SetMarkerStyle(kFullSquare);  h4->SetMarkerStyle(kFullSquare);

  if (!logy) {
    h1->SetMinimum(0.);
    h2->SetMinimum(0.);
    h3->SetMinimum(0.);
    h4->SetMinimum(0.);
  }

  h1->Draw();
  h2->Draw("same");
  h3->Draw("same");
  h4->Draw("same");

 l->Draw();
  //TLegend* myleg=pad1->BuildLegend();
  //myleg->Draw();
  pad1->Draw();

  if (logy)
    pad1->SetLogy();

//  pad1->Modified();
//  pad1->Update();

//  l->Draw();



  //drawText(legend1.Data(),0.7,0.6,color1);
  //drawText(legend2.Data(),0.7,0.7,color2);
  //drawText(sel,0.18,0.8,kBlack,20);
  c1->cd();

  TPad *pad2 = new TPad("pad2","pad2",0,0.1,1,0.4);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.25);
  pad2->Draw();
  pad2->cd();

  TH1F *hd1=(TH1F *)h1->Clone();  hd1->SetStats(0);
  TH1F *hd2=(TH1F *)h3->Clone();  hd2->SetStats(0);

  if (divide) {
    hd1->Divide(h1,h2);
    hd1->SetMinimum(-0.2);
    hd1->SetMaximum(4.2);
    hd2->Divide(h3,h4);
    hd2->SetMinimum(-0.2);
    hd2->SetMaximum(4.2);
  } else {
    hd1->Add(h1,h2,1,-1);
    hd2->Add(h3,h4,1,-1);
  }

  hd1->GetYaxis()->SetTitle(divide ? "Data/MC" : "difference");
  hd1->GetXaxis()->SetTitle(caption);
  hd1->GetXaxis()->SetTitleOffset(3.5);
  //  drawText(var,0.18,0.8,kBlack,20);

  hd1->SetMarkerStyle(kFullCircle);
  hd2->SetMarkerStyle(kFullSquare);
  hd1->Draw("ep");
  hd2->Draw("ep,same");

  gPad->Modified(); gPad->Update(); // make sure gPad is updated

  float y = divide ? 1 : 0;

  TLine *line = new TLine(gPad->GetUxmin(), y, gPad->GetUxmax(), y);
  line->SetLineStyle(2);
  line->Draw();

  c1->cd();

  c1->SaveAs(Form("Compare_%s.pdf",h1->GetTitle()));//Form("Compare_%s_%s_%s.pdf",title.Data(), legend1.Data(), legend2.Data()));

}

