#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "util.h"

vector<int> surf = GetNSup();

void DoGraph(int particle_type, int f_lowE, int f_highE, bool f_scale, bool f_logBin = 0){

  gStyle->SetOptStat(0);
  gErrorIgnoreLevel = kWarning;

  int choice  = particle_type;
  int lowE    = f_lowE;
  int highE   = f_highE;
  cout<<lowE<<" "<<highE<<endl;

  bool scale  = f_scale;
  bool logBin = f_logBin;

  string afile = analys_file(choice, 1);                                           //this fucntion scan the folder to find .root file
  TFile *t_nu_mu = TFile::Open(afile.c_str());

  //Create the folders
  GraphFolder("Grafici");
  GraphFolder("Tab");

  //Create vector with the name of the particle, the first is left blank
  //in order to have the vector start at the right place
  vector <string> part;
  part.push_back("null");
  part.push_back("ane&ne");
  part.push_back("neutron");

  //Creating map for detectors
  std::map<int, std::string> detectors;


  for(int i = 0; i < surf.size(); i++){
    string de;
    de = "Detector " + std::to_string(surf[i]);
    detectors[surf[i]-1] = de;
  }

  //Print the map for debugging
  for(auto itr = detectors.begin(); itr != detectors.end(); ++itr){
    cout<<(*itr).first<<"\t"<<(*itr).second<<endl;
  }


  //Beginning of the script to plot graphs
  TH1F *h_nu_mu;
  TCanvas *c1;

  int nsup;
  string detname;
  int i = 0;
  int k = 0;

  for(auto itr = detectors.begin(); itr != detectors.end(); ++itr){             //use iterator to cycle through the map

    i++;
    nsup = itr->first;
    detname = itr->second;

    //Get the histograms from the root file
    if(highE != 0){
      h_nu_mu = (TH1F*)t_nu_mu->Get(Form("%s_Energy_%d_%d_Det%d",part[choice].c_str(), lowE, highE, nsup+1));
    }
    else if(lowE == 0 && highE == 0 && logBin == 0){
      h_nu_mu = (TH1F*)t_nu_mu->Get(Form("%s_Energy_Det%d",part[choice].c_str(), nsup+1));
    }
    else if(logBin){
      h_nu_mu = (TH1F*)t_nu_mu->Get(Form("%s_EnergyLog_Det%d",part[choice].c_str(), nsup+1));
    }
    else{
      return;
    }

    h_nu_mu->SetTitle(detname.c_str());

    if(scale){
      h_nu_mu->Scale(1e22);
      h_nu_mu->GetYaxis()->SetTitle("Particles #upoint m^{2} #upoint y");
    }
    else{
      h_nu_mu->GetYaxis()->SetTitle("Particles #upoint m^{2}/EOT");
    }



    int nBin = h_nu_mu->GetNbinsX();
    double maxE     = h_nu_mu->GetXaxis()->GetXmax();
    double minE     = h_nu_mu->GetXaxis()->GetXmin();
    double deltaE   = maxE - minE;
    double binWidth = deltaE/nBin;

    cout<<detname<<endl;
    // cout<<"Energia Min (GeV): "<<minE<<"\t"<<"Energia Max (GeV): "<<maxE<<endl;
    // cout<<"Numero di bin: "<<nBin<<endl;
    // cout<<"Larghezza dei bin: "<<binWidth<<endl;
    cout<<"Numero neutrini: "<<h_nu_mu->Integral()<<endl;
    cout<<"****************"<<endl;

    //Setting the canvas
    c1 = new TCanvas(Form("%s_%d",part[choice].c_str(), i), Form("%s_%d",part[choice].c_str(),i), 1200, 1000);
    c1->SetLogy();
    c1->SetGrid();

    if(choice==2){
      c1->SetLogx();
    }

    h_nu_mu->Draw("hist");

    TPaveText *pt = new TPaveText();
    pt->SetX1NDC(0.60);
    pt->SetY1NDC(0.75);
    pt->SetX2NDC(0.97);
    pt->SetY2NDC(0.92);
    pt->SetTextSize(0.033);
    pt->SetTextFont(42);
    pt->SetBorderSize(1);
    pt->SetFillColor(0);
    pt->AddText(Form("Entries %g", h_nu_mu->GetEntries()));
    if(choice == 1){
      pt->AddText(Form("No. of #nu #upoint m^{2} #upoint y = %1.2E", h_nu_mu->Integral()));
    }
    else if(choice == 2){
      pt->AddText(Form("No. of n #upoint m^{2} #upoint y = %1.2E", h_nu_mu->Integral()));
    }
    pt->Draw();

    if(highE != 0){
      string f = "Grafici/" + part[choice];
      GraphFolder(f);
      c1->Print(Form("Grafici/%s/%s_%s_%g-%gGeV.png",part[choice].c_str(), part[choice].c_str(), detname.c_str(),minE, maxE));
    }
    else if((lowE == 0 && highE == 0) || logBin){
      string f = "Grafici/" + part[choice];
      GraphFolder(f);
      c1->SetLogx();
      c1->Print(Form("Grafici/%s/%s_%s.png",part[choice].c_str(), part[choice].c_str(), detname.c_str()));
    }

    //Writing the tables
    ofstream tab;

    if(highE != 0){
      string f = "Tab/" + part[choice];
      GraphFolder(f);
      tab.open(Form("Tab/%s_%s_%g-%gGeV_%dbin.dat", part[choice].c_str(), detname.c_str(), minE, maxE, nBin));
    }
    else if((lowE == 0 && highE == 0) || logBin){
      string f = "Tab/" + part[choice];
      GraphFolder(f);
      tab.open(Form("Tab/%s_%s_%dbin.dat", part[choice].c_str(), detname.c_str(),nBin));
    }

    tab<<detname.c_str()<<endl;
    tab<<"Energy Min (GeV): "<<minE<<"\t"<<"Energy Max (GeV): "<<maxE<<endl;
    tab<<"Bin number: "<<nBin<<endl;
    tab<<"Bin width: "<<binWidth<<endl<<endl;
    tab<<"Bin number \t \t Bin center \t \t Bin Content"<<endl;

    for(int i = 0; i < nBin; i++){
      // cout<<h_nu_mu->GetBinCenter(i)<<"\t"<<h_nu_mu->GetBinContent(i)<<endl;
      tab<<i<<"\t \t"<<h_nu_mu->GetBinCenter(i)<<"\t \t"<<h_nu_mu->GetBinContent(i)<<endl;
    }

    c1->Destructor();

  }

}

void GraphPlot(){

  //particle_type: 1: neutrino; 2: neutron;

  DoGraph(1, 0, 500, 1);
  DoGraph(2, 20, 100, 1);
  DoGraph(2, 100, 11, 1);

}
