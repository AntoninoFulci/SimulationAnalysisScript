{
  // gStyle->SetOptStat(0);
  #include <map>

  double E_low  [2] = {0.02, 0.1};
  double E_high [2] = {0.1, 11};
  cout<<"[1] 0.02 < Ek < 0.1 (Gev) \n[2] 0.1 < Ek < 11 (GeV)"<<endl;
  int scelta1;
  cin>>scelta1;

  TFile *t_neutron_up = TFile::Open(Form("Analisi_neutron_20GeV_up_%g-%gGeV.root", E_low[scelta1-1], E_high[scelta1-1]));
  TFile *t_neutron_front = TFile::Open(Form("Analisi_neutron_20GeV_front_%g-%gGeV.root", E_low[scelta1-1], E_high[scelta1-1]));

  std::map<int, std::string> detectors_up;
  std::map<int, std::string> detectors_front;

  string value = "Detector ";

  for(int i=1; i<= 23; i++){
    detectors_up[i] = value + i;
    // cout<<detectors_up[i]<<endl;
  }

  for(int i=101; i<= 134; i++){
    detectors_front[i] = value + i;
    // cout<<detectors_front[i]<<endl;
  }

  // for(auto itr = detectors.begin(); itr != detectors.end(); ++itr){
  //   cout<<(*itr).first<<"\t"<<(*itr).second<<endl;
  // }


  TH1F *h_n_up;
  TH1F *h_n_front;
  TCanvas *c1;

  TPaveStats *st;
  TPaveText *pt;
  TLatex *Integ;
  TList *listOfLines;
  TText *tconst;

  int nsup;
  string detname;
  int i = 0;

  int k = 0;

  int scelta;
  cout<<"[1] Front"<<endl<<"[2] Up"<<endl;
  cin>>scelta;

  if(scelta == 1){
    for(auto itr = detectors_front.begin(); itr != detectors_front.end(); ++itr){
      // for(auto i: sup){

      i++;
      nsup = itr->first;
      detname = itr->second;

      //Setting the canvas
      c1 = new TCanvas(Form("Neutron_%d",i), Form("Neutron_%d",i), 1200, 1000);
      c1->SetGrid();
      c1->SetLogx();
      c1->SetLogy();

      //Setting the histogram
      h_n_front = (TH1F*)t_neutron_front->Get(Form("neutron_Energy_Sup%d",nsup));
      // h_n_front->Scale(1e22);

      h_n_front->SetTitle(detname.c_str());
      h_n_front->SetName(detname.c_str());
      h_n_front->GetYaxis()->SetTitle("Particles/EOT");
      h_n_front->GetXaxis()->SetTitle("E (GeV)");

      //Setting the variables
      double maxE = h_n_front->GetXaxis()->GetXmax();
      double minE = h_n_front->GetXaxis()->GetXmin();
      int nBin = h_n_front->GetNbinsX();
      double deltaE = maxE-minE;
      double binWidth = deltaE/nBin;


      //Draw the histogram in the canvas
      h_n_front->Draw("hist");
      c1->Update();
      c1->Modified();

      Integ = new TLatex(0.5,0.5,Form("No. of n/EOT = %1.2E", h_n_front->Integral()));
      Integ->SetTextSize(0.033);
      Integ->SetTextFont(42);

      st = (TPaveStats*)c1->GetPrimitive("stats");

      if(st != NULL){
        listOfLines = st->GetListOfLines();

        tconst = st->GetLineWith("Mean");
        listOfLines->Remove(tconst);
        tconst = st->GetLineWith("Std Dev");
        listOfLines->Remove(tconst);

        listOfLines->Add(Integ);


        st->SetBorderSize(1);
        st->SetFillColor(0);
        st->SetX1NDC(0.60);
        st->SetY1NDC(0.77);
        st->SetX2NDC(0.95);
        st->SetY2NDC(0.94);
        st->SetTextSize(0.033);
        st->SetTextFont(42);
        st->DrawClone();
      }

      //Save the canvas
      c1->SaveAs(Form("Grafici/neutron_Energy_Sup%d_%g-%gGeV.png",nsup, E_low[scelta1-1], E_high[scelta1-1]));
      c1->Clear();

      cout<<detname<<endl;
      cout<<"Neutroni: "<<h_n_front->Integral()<<endl;
      cout<<endl;

    }
  }
  else if(scelta == 2){
    for(auto itr = detectors_up.begin(); itr != detectors_up.end(); ++itr){
      // for(auto i: sup){

      i++;
      nsup = itr->first;
      detname = itr->second;

      //Setting the canvas
      c1 = new TCanvas(Form("Neutron_%d",i), Form("Neutron_%d",i), 1200, 1000);
      c1->SetGrid();
      c1->SetLogx();
      c1->SetLogy();

      //Setting the histogram
      h_n_up = (TH1F*)t_neutron_up->Get(Form("neutron_Energy_Sup%d",nsup));
      // h_n_up->Scale(1e22);

      h_n_up->SetTitle(detname.c_str());
      h_n_up->SetName(detname.c_str());
      h_n_up->GetYaxis()->SetTitle("Particles/EOT");
      h_n_up->GetXaxis()->SetTitle("E (GeV)");

      //Setting the variables
      double maxE = h_n_up->GetXaxis()->GetXmax();
      double minE = h_n_up->GetXaxis()->GetXmin();
      int nBin = h_n_up->GetNbinsX();
      double deltaE = maxE-minE;
      double binWidth = deltaE/nBin;


      //Draw the histogram in the canvas
      h_n_up->Draw("hist");
      c1->Update();
      c1->Modified();

      Integ = new TLatex(0.5,0.5,Form("No. of n/EOT = %1.2E", h_n_up->Integral()));
      Integ->SetTextSize(0.033);
      Integ->SetTextFont(42);

      st = (TPaveStats*)c1->GetPrimitive("stats");

      if(st != NULL){
        listOfLines = st->GetListOfLines();

        tconst = st->GetLineWith("Mean");
        listOfLines->Remove(tconst);
        tconst = st->GetLineWith("Std Dev");
        listOfLines->Remove(tconst);

        listOfLines->Add(Integ);


        st->SetBorderSize(1);
        st->SetFillColor(0);
        st->SetX1NDC(0.60);
        st->SetY1NDC(0.77);
        st->SetX2NDC(0.95);
        st->SetY2NDC(0.94);
        st->SetTextSize(0.033);
        st->SetTextFont(42);
        st->DrawClone();
      }

      //Save the canvas
      c1->SaveAs(Form("Grafici/neutron_Energy_Sup%d_%g-%gGeV.png",nsup, E_low[scelta1-1], E_high[scelta1-1]));
      c1->Clear();

      cout<<detname<<endl;
      cout<<"Neutroni: "<<h_n_up->Integral()<<endl;
      cout<<endl;
    }

  }


}
