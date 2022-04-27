#define _USE_MATH_DEFINES
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <vector>
#include <chrono>
#include "util.h"
#include <map>

vector<int> surf = GetNSup();

void DoDistanze(int f_choice, int f_lowE, int f_highE){
  gErrorIgnoreLevel = kWarning;

  //Setting the energy range to search the histograms
  // lowE = 0;
  // highE = 500;

  // cout<<"Do you want to plot [1] neutrinos or [2] neutrons?"<<endl;
  // cin>>choice;
  //
  // cout<<"Insert the range:"<<endl;
  // cin>>lowE;
  // cin>>highE;

  int choice = f_choice;
  int lowE = f_lowE;
  int highE = f_highE;

  string afile = analys_file(choice, 1);                                           //this fucntion scan the folder to find .root file
  TFile *t_file= TFile::Open(afile.c_str());


  string dir = "Grafici_Distanze";
  GraphFolder(dir);

  //Create vector with the name of the particle, the first is left blank
  //in order to have the vector start at the right place
  vector <string> part;
  part.push_back("null");
  part.push_back("ane&ne");
  part.push_back("neutron");

  std::map<int, std::string> detectors;
  for(int i = 0; i < surf.size(); i++){
    string de;
    de = "Detector " + std::to_string(surf[i]);
    detectors[surf[i]-1] = de;
  }

  TH1D *h_posz;
  TH1D *h_posy;
  TH1D *h_en;

  int nsup(0), i(0);
  double z(0), y(0), integ(0);
  vector<double> vec_z;
  vector<double> vec_y;
  vector<double> vec_eny;
  vector<double> vec_enz;
  string detname;

  bool others = false;

  for(auto itr = detectors.begin(); itr != detectors.end(); ++itr){             //use iterator to cycle through the map

    i++;
    nsup = itr->first;
    detname = itr->second;

    // cout<<detname<<endl;
    if(nsup < 100){
      h_posy = (TH1D*)t_file->Get(Form("%s_y_Det%d",part[choice].c_str(), nsup+1));
      h_en = (TH1D*)t_file->Get(Form("%s_Energy_%d_%d_Det%d",part[choice].c_str(), lowE, highE, nsup+1));
      h_en->Scale(1e22);

      y = h_posy->GetMean();
      vec_y.push_back(y/100);
      cout<<"Y: "<<y/100<<endl;

      integ = h_en->Integral();
      vec_eny.push_back(integ);
      // cout<<"Integrale: "<<integ<<endl;

    }
    else if(nsup >= 100 && nsup <200){
      h_posz = (TH1D*)t_file->Get(Form("%s_z_Det%d",part[choice].c_str(), nsup+1));
      h_en = (TH1D*)t_file->Get(Form("%s_Energy_%d_%d_Det%d",part[choice].c_str(), lowE, highE, nsup+1));
      h_en->Scale(1e22);

      z = h_posz->GetMean();
      vec_z.push_back(z/100 - 4728.54/100);
      cout<<"Z: "<<z/100 - 4728.54/100<<endl;

      integ = h_en->Integral();
      vec_enz.push_back(integ);
      // cout<<"Integrale: "<<integ<<endl;

    }
    else if(nsup >= 200 && others){
      h_posy = (TH1D*)t_file->Get(Form("%s_y_Det%d",part[choice].c_str(), nsup+1));
      y = h_posy->GetMean();
      vec_y.push_back(y/100);
    }

  }

  double *arr_z = &vec_z[0];
  double *arr_y = &vec_y[0];
  double *arr_eny = &vec_eny[0];
  double *arr_enz = &vec_enz[0];

  // double arrei[3] = {12.1, 121.2, 124.4};

  TCanvas *c1 = new TCanvas("Front detectors", "P #upoint y vs Z", 1200, 1000);
  c1->SetGrid();
  c1->SetLogy();

  TGraph *int_z_front = new TGraph(vec_z.size(), arr_z, arr_enz);
  if(lowE < highE){
    int_z_front->SetTitle(Form("P #upoint m^{2} #upoint y vs Z - %d-%d MeV; z(m); P #upoint m^{2} #upoint y", lowE, highE));
  }
  else if(lowE > highE){
    int_z_front->SetTitle(Form("P #upoint m^{2} #upoint y vs Z - %g-%d GeV; z(m); P #upoint m^{2} #upoint y", (double)lowE/1000, highE));
  }
  int_z_front->SetMarkerStyle(8);
  int_z_front->SetMarkerSize(1);
  int_z_front->Draw("ap");
  int_z_front->GetXaxis()->SetRangeUser(5, 20);
  c1->SaveAs(Form("%s/%s_P_EOT_vs_Z - %d-%d MeV.png", dir.c_str(), part[choice].c_str(), lowE, highE));

  TCanvas *c2 = new TCanvas("Front detectors", "P #upoint y vs Y", 1200, 1000);
  c2->SetGrid();
  c2->SetLogy();

  TGraph *int_y_front = new TGraph(vec_y.size(), arr_y, arr_eny);

  if(lowE < highE){
    int_y_front->SetTitle(Form("P #upoint m^{2} #upoint y vs Y - %d-%d MeV; y(m); P #upoint m^{2} #upoint y", lowE, highE));
  }
  else if(lowE > highE){
    int_y_front->SetTitle(Form("P #upoint m^{2} #upoint y vs Y - %g-%d GeV; y(m); P #upoint m^{2} #upoint y", (double)lowE/1000, highE));
  }
  int_y_front->SetMarkerStyle(8);
  int_y_front->SetMarkerSize(1);
  int_y_front->Draw("ap");
  c2->SaveAs(Form("%s/%s_P_EOT_vs_Y - %d-%d MeV.png", dir.c_str(), part[choice].c_str(), lowE, highE));

  // c1->Destructor();
  // c2->Destructor();

}

void distanze(){

  DoDistanze(1, 0, 500);
  DoDistanze(2, 20, 100);
  DoDistanze(2, 100, 11);

}
