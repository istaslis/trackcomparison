#include "TH1F.h"
#include "TH2F.h"
#include "TLine.h"
#include "TFile.h"
#include "TCanvas.h"

#include "auxdraw.C"

int canvascount = 0;
TCanvas *getC()
{
  canvascount++;
  TString name = "canvas"+TString::Itoa(canvascount,10);
  return new TCanvas(name,name,600,600);
}

TFile *_file1,*_file2,*_file3,*_file4;

TH1F *getRatio(TString hname)
{
  auto h1     = (TH1F*)_file1->Get(hname);
  auto h2     = (TH1F*)_file2->Get(hname);

  h1->Scale(1/h1->Integral());
  h2->Scale(1/h2->Integral());
  TH1F *h3 = (TH1F *)h1->Clone();
  h3->Reset();
  h3->Divide(h1,h2,1.,1.);

  std::cout<<hname<<" "<<h1->Integral()<<std::endl;
  return h3;
}

void Draw(TString var, TString caption, bool logy=false, float ymin = 999, float ymax = 999)
{
  auto h1 = (TH1F*)_file1->Get(var);
  auto h2 = (TH1F*)_file2->Get(var);
  auto h3 = (TH1F*)_file3->Get(var);
  auto h4 = (TH1F*)_file4->Get(var);

  if (ymin!=999) {
    h1->SetMinimum(ymin);
    h2->SetMinimum(ymin);
    h3->SetMinimum(ymin);
    h4->SetMinimum(ymin);
    cout<<caption<<" "<<ymax<<endl;
  }
  if (ymax!=999) {
    h1->SetMaximum(ymax);
    h2->SetMaximum(ymax);
    h3->SetMaximum(ymax);
    h4->SetMaximum(ymax);
    //    h1->GetYaxis()->SetRangeUser(ymin,ymax);
    getC(); h1->Draw(); h2->Draw("same");
  }


  DrawCompare(h1,h2,h3,h4,
	      //(TH1F*)_file1->Get(var),(TH1F*)_file2->Get(var), (TH1F*)_file3->Get(var),(TH1F*)_file4->Get(var), 
	      kBlue+3, kBlue-5,kRed+3,kRed-5,"Data2TeV","MC2TeV","Data2TeV","MC5TeV","",caption, true, logy);

}


void drawtracks()
{
  TString file1 = "DATAMB2076.root";
  TString file2 = "MCMB2076.root";
  TString file3 = "DATAMB2076.root";
  TString file4 = "MCMB5020.root";

  _file1 = new TFile(file1);
  _file2 = new TFile(file2);
  _file3 = new TFile(file3);
  _file4 = new TFile(file4);

  Draw("hpt","p_{T} [GeV/c]",true);
  Draw("hpt4","p_{T} [GeV/c]",true);
  Draw("hpt5","p_{T} [GeV/c]",true);
  Draw("hpt6","p_{T} [GeV/c]",true);
  Draw("hpt7","p_{T} [GeV/c]",true);

  Draw("hIPsign","IP significance",true);
  Draw("hZsign","dZ significance",true);
  
  Draw("hmva4","MVA discriminator algo4");
  Draw("hmva5","MVA discriminator algo5");
  Draw("hmva6","MVA discriminator algo6");
  Draw("hmva7","MVA discriminator algo7");

  Draw("hphi","#phi", false, 0, 0.04);

  Draw("heta","#eta");
  Draw("hnhit","# of hits");
  Draw("hnTrk","# of tracks", true);
  Draw("hchi2n","#chi^2 / N_{dof}",true);
  

  auto halgos1  = (TH2F*)_file1->Get("halgos");
  auto halgos2  = (TH2F*)_file2->Get("halgos");
  getC(); halgos1->Draw("colz");
  getC(); halgos2->Draw("colz");



  //  void DrawCompare(TH1F *h1, TH1F *h2, int color1 = kBlack, int color2 = kBlue, TString legend1 = "Data", TString legend2 = "MC", TString title = "compare", TString caption = "A_{J}", bool divide = true)


}



void drawtracks2()
{
  TString file1 = "DATAMB2076.root";
  TString file2 = "MCMB2076.root";

  _file1 = new TFile(file1);
  _file2 = new TFile(file2);

  auto hpt     = getRatio("hpt");
  auto hpt4    = getRatio("hpt4");
  auto hpt5    = getRatio("hpt5");
  auto hpt6    = getRatio("hpt6");
  auto hpt7    = getRatio("hpt7");
  auto hIPsign = getRatio("hIPsign");
  auto hmva4   = getRatio("hmva4");
  auto hmva5   = getRatio("hmva5");
  auto hmva6   = getRatio("hmva6");
  auto hmva7   = getRatio("hmva7");

  auto halgos1  = (TH2F*)_file1->Get("halgos");
  auto halgos2  = (TH2F*)_file2->Get("halgos");

  auto hphi   = getRatio("hphi");
  auto hnhit   = getRatio("hnhit");


  hpt->SetXTitle("p_{T} [GeV/c]");

  hpt4->SetXTitle("p_{T} [GeV/c]"); //hpt4->SetMarkerColor(kred);
  hpt5->SetXTitle("p_{T} [GeV/c]"); //hpt5->SetMarkerColor(kblue);
  hpt6->SetXTitle("p_{T} [GeV/c]"); //hpt6->SetMarkerColor(kgreen);
  hpt7->SetXTitle("p_{T} [GeV/c]"); //hpt7->SetMarkerColor(kGray);

  hIPsign->SetXTitle("IP significance");

  hmva4->SetXTitle("MVA discriminator");
  halgos1->SetXTitle("final algo"); halgos1->SetYTitle("original algo");
  halgos2->SetXTitle("final algo"); halgos2->SetYTitle("original algo");


  TLegend *l = new TLegend(0.6,0.6,0.8,0.8);
  l->AddEntry(hpt4,"algo 4","P");
  l->AddEntry(hpt5,"algo 5","P");
  l->AddEntry(hpt6,"algo 6","P");
  l->AddEntry(hpt7,"algo 7","P");

  auto c=getC();
  hpt->Draw();
  auto c4=getC();
  hpt4->Draw();
  auto c5=getC();
  hpt5->Draw();
  auto c6=getC();
  hpt6->Draw();
  auto c7=getC();
  hpt7->Draw();

  auto cm4=getC();
  hmva4->Draw();
  auto cm5=getC();
  hmva5->Draw();
  auto cm6=getC();
  hmva6->Draw();
  auto cm7=getC();
  hmva7->Draw();
  
  auto c2 = getC();
  halgos1->Draw("colz");

  auto c3 = getC();
  halgos2->Draw("colz");

  auto sig = getC();
  hIPsign->Draw();

  getC();
  hphi->Draw();

  auto ccc = getC();
  hnhit->Draw();

}

