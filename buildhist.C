#define Run2011

int histcounter = 0;

TH1F *getH(TString name,int bins, float xmin, float xmax, int color = kBlue)
{
  //  histcounter++;
  //  TString name = "hist"+TString::Itoa(histcounter,10);
  auto h = new TH1F(name,name,bins,xmin,xmax);
  h->SetLineColor(color); h->SetMarkerColor(color); h->Sumw2();
  return h;
}

void checktracks(TString inputfile, TString outputfile)
{

  //"HiForest_Centrality_Unpacker_Hydjet_Quenched_MinBias_5020GeV_750_RECODEBUG_v0_merged_forest_0.root";

  TFile *_file0 = new TFile(inputfile);

  /*  TTreeReader reader("ak4PFJetAnalyzer/t", _file0);
  TTreeReaderValue<int> evt(reader, "evt");
  TTreeReaderValue<int> nref(reader, "nref");
  TTreeReaderArray<float> discr_csvSimple(reader, "discr_csvSimple");

  TTreeReader readerhlt("hltanalysis/HltTree", _file0);
  TTreeReaderValue<int> HLT_AK4CaloJet60_Eta5p1_v1(readerhlt, "HLT_AK4CaloJet60_Eta5p1_v1");
  */
  TTreeReader readerTrack("anaTrack/trackTree",_file0);
  TTreeReaderValue<int> ntrk(readerTrack, "nTrk");
  TTreeReaderArray<float> trkDxyError1(readerTrack, "trkDxyError1");
  TTreeReaderArray<float> trkDxy1(readerTrack, "trkDxy1");
  TTreeReaderArray<float> trkPt(readerTrack, "trkPt");
  TTreeReaderArray<float> trkPhi(readerTrack, "trkPhi");
  TTreeReaderArray<float> trkEta(readerTrack, "trkEta");
  TTreeReaderArray<float> trkMVA(readerTrack, "trkMVA");

  TTreeReaderArray<float> trkChi2(readerTrack, "trkChi2");
  TTreeReaderArray<float> trkDz1(readerTrack, "trkDz1");
  TTreeReaderArray<float> trkDzError1(readerTrack, "trkDzError1");
  

  
  TTreeReaderArray<bool> highPurity(readerTrack, "highPurity");
  

#ifdef Run2011
  TTreeReaderArray<int> trkNhit(readerTrack, "trkNHit");
  TTreeReaderArray<int> trkAlgo(readerTrack, "trkAlgo");
  TTreeReaderArray<int> trkOriginalAlgo(readerTrack, "trkOriginalAlgo");
  TTreeReaderArray<float> trkNdof(readerTrack, "trkNdof");
#else
  TTreeReaderArray<unsigned char> trkNhit(readerTrack, "trkNHit");
  TTreeReaderArray<unsigned char> trkAlgo(readerTrack, "trkAlgo");
  TTreeReaderArray<unsigned char> trkOriginalAlgo(readerTrack, "trkOriginalAlgo");
  TTreeReaderArray<unsigned char>   trkNdof(readerTrack, "trkNdof");
#endif

  float ptmax = 20;

  auto hpt = getH("hpt",50,0,ptmax);
  auto hpt4 = getH("hpt4",50,0,ptmax);
  auto hpt5 = getH("hpt5",50,0,2);
  auto hpt6 = getH("hpt6",50,0,ptmax);
  auto hpt7 = getH("hpt7",50,0,ptmax);

  auto hphi = getH("hphi",30,-3.2,3.2);
  auto heta = getH("heta",30,2.5,2.5);
  auto hnhit = getH("hnhit",26,5,30);
  auto hnTrk = getH("hnTrk",50,0,7000);

  auto hchi2n = getH("hchi2n",50,0,6);



  auto hIPsign = getH("hIPsign",50,-50,50);
  auto hZsign = getH("hZsign",50,-50,50);

  auto hmva4 = getH("hmva4",50,-1,1);
  auto hmva5 = getH("hmva5",50,-1,1);
  auto hmva6 = getH("hmva6",50,-1,1);
  auto hmva7 = getH("hmva7",50,-1,1);

  auto halgos = new TH2F("halgos","halgos",8,4,12,8,4,12);

  int onep = readerTrack.GetEntries(true)/100;
  int counter = 0;
  //  while(readerTrack.Next()) {
  while(readerTrack.Next() && counter<1000) {
    counter++;
    if (counter%onep == 0) std::cout<<counter/onep<<"\%"<<std::endl;
    for (int i=0;i<*ntrk;i++) {
      if (highPurity[i])
	hIPsign->Fill(trkDxy1[i]/trkDxyError1[i]);
	hZsign->Fill(trkDz1[i]/trkDzError1[i]);

	hnTrk->Fill(*ntrk);
	hchi2n->Fill(trkChi2[i]/trkNdof[i]);

	
	if (trkAlgo[i]==4) {hmva4->Fill(trkMVA[i]);hpt4->Fill(trkPt[i]); }
	if (trkAlgo[i]==5) {hmva5->Fill(trkMVA[i]);hpt5->Fill(trkPt[i]); }
	if (trkAlgo[i]==6) {hmva6->Fill(trkMVA[i]);hpt6->Fill(trkPt[i]); }
	if (trkAlgo[i]==7) {hmva7->Fill(trkMVA[i]);hpt7->Fill(trkPt[i]); }
	
	hphi->Fill(trkPhi[i]);
	heta->Fill(trkEta[i]);
	hnhit->Fill(trkNhit[i]);
	hpt->Fill(trkPt[i]);
	
	halgos->Fill(trkAlgo[i],trkOriginalAlgo[i]);

	
    }
    
  }

  std::cout<<counter<<std::endl;


  TFile *fout = new TFile(outputfile,"recreate");
  hpt->Write();
  hpt4->Write();
  hpt5->Write();
  hpt6->Write();
  hpt7->Write();
  hIPsign->Write();
  hmva4->Write();
  hmva5->Write();
  hmva6->Write();
  hmva7->Write();
  halgos->Write();
  hphi->Write();
  heta->Write();
  hchi2n->Write();
  hZsign->Write();
  hnTrk->Write();
  hnhit->Write();

}



void buildhist()
{

#ifdef Run2011
  checktracks("HydjetMB_2076GeV_FOREST_753p1_v0_merged.root","MCMB2076.root");
  checktracks("HiForest_HIMinBiasUPC_HIRun2011-v1_merged.root","DATAMB2076.root");
#else
  checktracks("HiForest_Centrality_Unpacker_Hydjet_Quenched_MinBias_5020GeV_750_RECODEBUG_v0_merged_forest_0.root","MCMB5020.root");
#endif
}


