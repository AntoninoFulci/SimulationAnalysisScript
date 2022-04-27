//******************************************************************
//Questa macro permette di leggere i file _tab.lis creati da fluka
//e plottare i risultati con ROOT
//******************************************************************

#include <fstream>
#include <sstream>
#include <string>
#include "util.h"

void neutron_fluka(){
  gErrorIgnoreLevel = kWarning;

  GraphFolder();

  std::ifstream infile("Neutrons_0-20MeV_tab.lis");                           //Nome del file da leggere
  string line;

  string det_line = "# Detector n:";                                            //questa è la stringa che cercherà per iniziare a leggere i dati
  string end_line = "# double differential distributions";                      //questa è la stringa che cercherà per finirre di leggere i dati

  bool read;
  int counter = 0;
  vector<string> det_names;                                                     //vettore di stringhe che conserva i nomi dei detector

  double edge_sx, edge_dx, content, error;                                      //buffer
  vector<vector<double>> bin_edges;                                             //matrice che conterrà i bordi sx dei bin, a root servono solo quelli sx per costrutire l'histo
  vector<vector<double>> bin_content;                                           //matrice che conterrà i contenuti dei bin
  int k = 0;

  vector<double> vec_integral1;
  vector<double> vec_integral2;

  while (std::getline(infile, line)){                                           //"leggi fino alla fine"

    std::istringstream iss(line);                                               //ogni linea viene messo in un oggetto stringstream che è più controllabile

    if(line.find(det_line) != std::string::npos) {                              //cerca nella linea corrente "det_line"
      read = true;
    }

    if(read){                                                                   //se la trova si attiva l'if
      counter++;                                                                //serve a fargli skippare certe linee
      if(counter == 1){
        det_names.push_back(line.substr(30,8));                                 //in questo modo si trova la parte della linea che contiene il nome del Detector
        // cout<<line.substr(30,8)<<endl;

        bin_edges.push_back(vector<double>());                                  //ogni volta che trova il nome di un detector aggiunge una "colonna" nella matrice
        bin_content.push_back(vector<double>());
      }
      if(counter > 2){                                                          //arrivato alla linea in cui iniziano i numeri
        if(!(iss >> edge_sx >> edge_dx >> content >> error)){                   //inizia a "strippare" la lineea nei vari numeri e a metterli nei buffer
          // cout<<line<<endl; //End
        }
        else{
          // cout<<content<<endl;
          bin_edges[k].push_back(edge_sx);                                      //in tutti gli altri casi inserisce le variabili nella matrice nella colonna corrispondente al detector
          bin_content[k].push_back(content);
        }
      }
    }


    if(line.find(end_line) != std::string::npos) {                              //qui cerca la stringa "end line" all'interno della linea corrente
      counter = 0;
      k++;
      read = false;                                                             //se la trova setta read su false in maniera tale da stoppare la lettura dei numeri
    }


  }

  int det_number = bin_edges.size();                                            //il numero di detector trovati corrisponde a quante "colonne" ci sono nella matrice "bin_edges"
  double *a_bin_edges;                                                          //array buffer che serve a convertire il vector in array in modo da passarlo a root

  TH1D *energy;
  TCanvas *canvas;

  int up_det_id(0), fw_det_id(0), ot_det_id(0);

  for(int i = 1; i <= bin_edges.size(); i++){
    canvas = new TCanvas(Form("Canvas_%d",i), Form("Canvas_%d",i), 1200, 1000);
    a_bin_edges = &bin_edges[i-1][0];                                           //artificio per convertire il vector in array

    if(det_names[i-1].find("Up") != std::string::npos){
      up_det_id++;
      energy = new TH1D(
        Form("Detector %s", det_names[i-1].c_str()),                                                    //nome histo
        Form("Detector %d;E(GeV);Part/GeV/cm^{2} #upoint y", up_det_id),                      //titolo histo + assi
        bin_edges[i-1].size()-1,                                                  //il numero di bin deve essere inferiore di 1 al numero di edges
        a_bin_edges                                                               //si passa tutto l'array di bin_edges all'histogramma
      );
    }
    else if(det_names[i-1].find("Fw") != std::string::npos){
      fw_det_id++;
      energy = new TH1D(
        Form("Detector %s", det_names[i-1].c_str()),                                                    //nome histo
        Form("Detector %d;E(GeV);Part/GeV/cm^{2} #upoint y",fw_det_id+100),                      //titolo histo + assi
        bin_edges[i-1].size()-1,                                                  //il numero di bin deve essere inferiore di 1 al numero di edges
        a_bin_edges                                                               //si passa tutto l'array di bin_edges all'histogramma
      );
    }
    else if(det_names[i-1].find("Ot_Det4") != std::string::npos){
      ot_det_id++;
      energy = new TH1D("Detector 136",                                                //nome histo
        "Detector 136;E(GeV);Part/GeV/cm^{2} #upoint y",                      //titolo histo + assi
        bin_edges[i-1].size()-1,                                                  //il numero di bin deve essere inferiore di 1 al numero di edges
        a_bin_edges                                                               //si passa tutto l'array di bin_edges all'histogramma
      );
    }
    else if(det_names[i-1].find("Ot") != std::string::npos){
      ot_det_id++;
      energy = new TH1D(
        Form("Detector %s", det_names[i-1].c_str()),                                                    //nome histo
        Form("Detector %d;E(GeV);Part/GeV/cm^{2} #upoint y",ot_det_id+200),                      //titolo histo + assi
        bin_edges[i-1].size()-1,                                                  //il numero di bin deve essere inferiore di 1 al numero di edges
        a_bin_edges                                                               //si passa tutto l'array di bin_edges all'histogramma
      );
    }



    for(int j = 0; j < bin_edges[i-1].size(); j++){                             //ciclo per settare il contenuto dei bin
      // cout<<bin_content[i][j]<<endl;
      energy->SetBinContent(j, bin_content[i-1][j]);
    }

    energy->GetXaxis()->SetRangeUser(1e-12, 0.02);
    energy->SetStats(0);
    energy->Scale(1e22);                                                            //draw degli histogrammi
    energy->Draw("hist");                                                             //draw degli histogrammi
    canvas->SetLogx();
    canvas->SetLogy();
    canvas->SetGrid();

    canvas->Update();
    canvas->Modified();

    TPaveText *pt = new TPaveText();
    pt->SetX1NDC(0.60);
    pt->SetY1NDC(0.82);
    pt->SetX2NDC(0.95);
    pt->SetY2NDC(0.92);
    pt->SetTextSize(0.033);
    pt->SetTextFont(42);
    pt->SetBorderSize(1);
    pt->SetFillColor(0);
    pt->AddText(Form("No. of n #upoint m^{2} #upoint y = %1.2E", energy->Integral("width")*10000));
    pt->Draw();

    canvas->SaveAs(Form("Grafici/neutron_Energy_%s_0-20MeV.png",
      det_names[i-1].c_str()));
    // canvas->Clear();



    cout<<"Detector "<<det_names[i-1]<<" integral (over energy and surface):"<<endl;
    cout<<"Neutroni: "<<energy->Integral("width")*10000<<endl;                  //essendo Part/GeV/cmq/pr si passa l'opzione "width" in maniera tale che quando fa l'integral
    // cout<<"Max: "<<energy->GetXaxis()->GetXmax()<<" Min :"<< energy->GetXaxis()->GetXmin()<<endl;
    cout<<"********************"<<endl;                                         //moltiplichi anche per la larghezza del bin e poi si moltiplica per la sup del detector

    if(det_names[i-1].find("Up") != std::string::npos){
      vec_integral1.push_back(energy->Integral("width")*10000);
    }
    else{
      vec_integral2.push_back(energy->Integral("width")*10000);
    }

  }

  vector<double> z1 = {1.0668, 1.37, 1.64, 2.04, 2.47, 2.97, 3.47, 3.97, 4.32, 4.65,
                  5.0, 5.5, 6.0, 6.5, 7.0, 7.4, 7.8};
  vector<double> z2 = {6.8313, 7.01459, 7.51459, 8.01459, 8.51459, 9.01459, 9.51459,
                  10.0146, 10.5146, 11.0146, 11.5146, 12.0146, 12.5146, 13.0146,
                  13.5146, 14.0146, 14.5146, 14.7546, 14.9546, 15.4546, 15.9546,
                  16.4546, 16.9546, 17.4546, 17.9546, 18.4546, 18.9546, 19.4546,
                  19.9546, 20.4546, 20.9546, 21.4546, 21.9546, 22.1546, 22.8346,
                  25.2146};

  // cout<<z1.size()<<endl;
  // cout<<vec_integral1.size()<<endl;
  cout<<z2.size()<<endl;
  cout<<vec_integral2.size()<<endl;

  TCanvas *c1 = new TCanvas("prova1", "prova1", 1200, 1000);
  c1->SetGrid();
  c1->SetLogy();
  double *arr_integral1 = &vec_integral1[0];
  double *arr_z1 = &z1[0];
  TGraph *g1 = new TGraph(vec_integral1.size(), arr_z1, arr_integral1);
  g1->SetTitle("P #upoint m^{2} #upoint y vs Y - 0-20 MeV; y(cm); P #upoint m^{2} #upoint y");
  // g1->GetXaxis()->SetRangeUser(0.5, 5.5);
  g1->GetYaxis()->SetRangeUser(1e12, 1e20);
  g1->SetMarkerStyle(8);
  g1->SetMarkerSize(1);
  g1->Draw("ap");
  c1->SaveAs("P_EOT vs Y - 0-20 MeV.png");

  TCanvas *c2 = new TCanvas("prova2", "prova2", 1200, 1000);
  c2->SetGrid();
  c2->SetLogy();
  double *arr_integral2 = &vec_integral2[0];
  double *arr_z2 = &z2[0];
  TGraph *g2 = new TGraph(vec_integral2.size(), arr_z2, arr_integral2);
  g2->SetTitle("P #upoint m^{2} #upoint y vs Z - 0-20 MeV; z(cm); P #upoint m^{2} #upoint y");
  // g2->GetXaxis()->SetRangeUser(6.5, 15.5);
  // g2->GetYaxis()->SetRangeUser(9*1e-9*1e22, 2*1e-4*1e22);
  g2->SetMarkerStyle(8);
  g2->SetMarkerSize(1);
  g2->Draw("ap");
  c2->SaveAs("P_EOT vs Z - 0-20 MeV.png");



}
