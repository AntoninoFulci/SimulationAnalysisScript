#define neutrini_cxx
#define _USE_MATH_DEFINES
#include "neutrini.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <vector>
#include <chrono>


char* StringToChar(string StringaIn){
	char* CharOut = new char[StringaIn.length()+1];
	strcpy(CharOut, StringaIn.c_str());
	return CharOut;
}

void neutrini::Loop(){
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;

  //DEFINIZIONE ISTOGRAMMI E VARIABILI

	vector <string> particella;
	particella.push_back("ane&ne_e");			//[0]
	particella.push_back("ane&ne_mu"); 		//[1]

  double peso_eot(0);

	//Numero di elettroni nella simulazione
	auto h_nEOT = new TH1F("h1","ntuple",100,0,0);
	auto myFile = TFile::Open("simulazione_100Mln_11GeV_V6.root");
	if (!myFile || myFile->IsZombie()) {
		 return;
	}

	TTreeReader myReader("CycleSummary", myFile);
	TTreeReaderValue<Int_t> nEOT(myReader, "nEOT");
	while (myReader.Next()) {
			h_nEOT->Fill(*nEOT);
	}

	int ne = h_nEOT->GetMean()*h_nEOT->GetEntries();	//assumendo che il numero di elettroni simulati sia uguale in ogni run;
	cout<<"Il numero di elettroni per EOT è: "<<ne<<endl;

  //*************************************************************************//

  // TH3D *ane_ne_e = new TH3D("ane_ne_e", "ane_ne_e", 500, 0, 0, 500, 0, 0, 500, 0, 0);
  // TH3D *ane_ne_mu = new TH3D("ane_ne_mu", "ane_ne_mu", 500, 0, 0, 500, 0, 0, 500, 0, 0);

	TH2D *ane_ne_e_XY = new TH2D("ane_ne_e_XY", "ane_ne_e_XY", 300, -150, 150, 300, -150, 150);
	TH2D *ane_ne_e_ZX = new TH2D("ane_ne_e_ZX", "ane_ne_e_ZX", 1200, 0, 1200, 300, -150, 150);
	TH2D *ane_ne_e_ZY = new TH2D("ane_ne_e_ZY", "ane_ne_e_ZY", 1200, 0, 1200, 300, -150, 150);

	TH2D *ane_ne_e_ZX_zoom = new TH2D("ane_ne_e_ZX_zoom", "ane_ne_e_ZX_zoom", 500, 0, 250, 300, -150, 150);
	TH2D *ane_ne_e_ZY_zoom = new TH2D("ane_ne_e_ZY_zoom", "ane_ne_e_ZY_zoom", 500, 0, 250, 300, -150, 150);

	TH2D *ane_ne_mu_XY = new TH2D("ane_ne_mu_XY", "ane_ne_mu_XY", 300, -150, 150, 300, -150, 150);
	TH2D *ane_ne_mu_ZX = new TH2D("ane_ne_mu_ZX", "ane_ne_mu_ZX", 1200, 0, 1200, 300, -150, 150);
	TH2D *ane_ne_mu_ZY = new TH2D("ane_ne_mu_ZY", "ane_ne_mu_ZY", 1200, 0, 1200, 300, -150, 150);

	TH2D *ane_ne_mu_ZX_zoom = new TH2D("ane_ne_mu_ZX_zoom", "ane_ne_mu_ZX_zoom", 500, 0, 250, 300, -150, 150);
	TH2D *ane_ne_mu_ZY_zoom = new TH2D("ane_ne_mu_ZY_zoom", "ane_ne_mu_ZY_zoom", 500, 0, 250, 300, -150, 150);


  //CICLO PRINCIPALE
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    //***************************************************
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
    //***************************************************

    peso_eot = m_weight/ne;


		//neutrino e antinuetrino elettronico [0]
		if( m_E < 0.5 && (m_pid == 5 || m_pid == 6)){
			ane_ne_e_XY->Fill(m_vertexX, m_vertexY, peso_eot);
			ane_ne_e_ZX->Fill(m_vertexZ, m_vertexX, peso_eot);
			ane_ne_e_ZY->Fill(m_vertexZ, m_vertexY, peso_eot);

			ane_ne_e_ZX_zoom->Fill(m_vertexZ, m_vertexX, peso_eot);
			ane_ne_e_ZY_zoom->Fill(m_vertexZ, m_vertexY, peso_eot);
		}

			//anti neutrino elettronico [1]
		else if(m_E < 0.5 &&  (m_pid == 27 || m_pid == 28)){
			ane_ne_mu_XY->Fill(m_vertexX, m_vertexY, peso_eot);
			ane_ne_mu_ZX->Fill(m_vertexZ, m_vertexX, peso_eot);
			ane_ne_mu_ZY->Fill(m_vertexZ, m_vertexY, peso_eot);

			ane_ne_mu_ZX_zoom->Fill(m_vertexZ, m_vertexX, peso_eot);
			ane_ne_mu_ZY_zoom->Fill(m_vertexZ, m_vertexY, peso_eot);

		}

	}


  TFile *contenitore = new TFile("vertex.root", "recreate");
  TTree *events = new TTree("Vertici", "Vertici");

	ane_ne_e_XY->Write();
	ane_ne_e_ZX->Write();
	ane_ne_e_ZY->Write();
	ane_ne_e_ZX_zoom->Write();
	ane_ne_e_ZY_zoom->Write();

	ane_ne_mu_XY->Write();
	ane_ne_mu_ZX->Write();
	ane_ne_mu_ZY->Write();
	ane_ne_mu_ZX_zoom->Write();
	ane_ne_mu_ZY_zoom->Write();

}
