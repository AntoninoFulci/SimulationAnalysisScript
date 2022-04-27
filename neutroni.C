#define neutroni_cxx
#define _USE_MATH_DEFINES
#include "neutroni.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <vector>
#include "util.h"

void neutroni::Loop(){
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;


  //Using the function defined above
  //pass 1 to go in debug mode and to see how many seconds the function takes
  vector<int> surf = GetNSup();     //vector with the surface number
	int ne = GetNEOT();               //find the number simulated electrons

  cout<<"Il numero di elettroni per EOT è: "<<ne<<endl;

  double Emax = 10.0;
  double Emin = 0.01;
  int LogBinNumber = 200;

  vector<double> LogBinningEdges = LogBinning(Emax, Emin, LogBinNumber);
	double* LogBinningEdges_array = &LogBinningEdges[0];

  //DEFINIZIONE ISTOGRAMMI E VARIABILI
  vector<vector<TH2D*>> XvsY;
  vector<vector<TH2D*>> EvsMID;

  vector<vector<TH2D*>> th_pVSr;
  vector<vector<TH2D*>> m_EVSr;

  vector<vector<TH1D*>> energy;
	vector<vector<TH1D*>> energyLog;
	vector<vector<TH1D*>> energy20_100;
	vector<vector<TH1D*>> energy100_11;

  vector<vector<TH1D*>> x;
  vector<vector<TH1D*>> y;
  vector<vector<TH1D*>> z;

  vector<vector<TH1D*>> p;
  vector<vector<TH1D*>> px;
  vector<vector<TH1D*>> py;
  vector<vector<TH1D*>> pz;

  vector<vector<TH1D*>> r;
  vector<vector<TH1D*>> theta_p;


	vector <string> particella;
	particella.push_back("neutron");			//[0]

	TH2D *h_XvsY;

  for(int j=0; j<particella.size(); j++){

    XvsY.push_back(vector<TH2D*>());
    EvsMID.push_back(vector<TH2D*>());

    th_pVSr.push_back(vector<TH2D*>());
    m_EVSr.push_back(vector<TH2D*>());

    energy.push_back(vector<TH1D*>());
		energyLog.push_back(vector<TH1D*>());
    energy20_100.push_back(vector<TH1D*>());
    energy100_11.push_back(vector<TH1D*>());

    x.push_back(vector<TH1D*>());
    y.push_back(vector<TH1D*>());
    z.push_back(vector<TH1D*>());

    p.push_back(vector<TH1D*>());
    px.push_back(vector<TH1D*>());
    py.push_back(vector<TH1D*>());
    pz.push_back(vector<TH1D*>());

    r.push_back(vector<TH1D*>());
    theta_p.push_back(vector<TH1D*>());

    for(int i=0; i<surf.size(); i++){

      //put here the number of the detector that should be XvsZ
			if(i==60000){
				h_XvsY  = new TH2D(Form("%s_XvsZ_Det%d", particella[j].c_str(), surf[i]), Form("%s X vs Z Det %d; x(cm); z(cm)", particella[j].c_str(), surf[i]), 100, 0, 0, 100, 0, 0);
			}
			else{
      	h_XvsY  = new TH2D(Form("%s_XvsY_Det%d", particella[j].c_str(), surf[i]), Form("%s X vs Y Det %d; x(cm); y(cm)", particella[j].c_str(), surf[i]), 100, 0, 0, 100, 0, 0);
			}

			TH2D *h_EvsMID  = new TH2D(Form("%s_h_EvsMID_Det%d", particella[j].c_str(), surf[i]), Form("%s Energy VS MotherID %d; E(GeV); M_ID", particella[j].c_str(), surf[i]), 100, 0, 0, 100, 0, 0);

      TH2D *h_ThetapvsR  = new TH2D(Form("%s_th_pVSr_Det%d", particella[j].c_str(), surf[i]), Form("%s Theta_p VS R Det %d; #theta_{p}(#circ); r(cm)", particella[j].c_str(), surf[i]), 100, 0, 0, 100, 0, 0);
      TH2D *h_EvsR = new TH2D(Form("%s_m_EVSr_Det%d", particella[j].c_str(), surf[i]), Form("%s m_E VS r Det %d; E(GeV); r(cm)", particella[j].c_str(), i), 100, 0, 0, 100, 0, 0);

      TH1D *h_x = new TH1D(Form("%s_x_Det%d", particella[j].c_str(), surf[i]), Form("%s x Det %d; x(cm); Particles/EOT", particella[j].c_str(), surf[i]), 100, 0, 0);
      TH1D *h_y = new TH1D(Form("%s_y_Det%d", particella[j].c_str(), surf[i]), Form("%s y Det %d; y(cm); Particles/EOT", particella[j].c_str(), surf[i]), 100, 0, 0);
      TH1D *h_z = new TH1D(Form("%s_z_Det%d", particella[j].c_str(), surf[i]), Form("%s z Det %d; z(cm); Particles/EOT", particella[j].c_str(), surf[i]), 100, 0, 0);

      TH1D *h_p  = new TH1D(Form("%s_p_Det%d", particella[j].c_str(), surf[i]), Form("%s p Det %d; p(GeV); Particles/EOT", particella[j].c_str(), i), 100, 0, 0);
      TH1D *h_px  = new TH1D(Form("%s_px_Det%d", particella[j].c_str(), surf[i]), Form("%s p_x Det %d; p_{x}(GeV); Particles/EOT", particella[j].c_str(), surf[i]), 100, 0, 0);
      TH1D *h_py  = new TH1D(Form("%s_py_Det%d", particella[j].c_str(), surf[i]), Form("%s p_y Det %d; p_{y}(GeV); Particles/EOT", particella[j].c_str(), surf[i]), 100, 0, 0);
      TH1D *h_pz  = new TH1D(Form("%s_pz_Det%d", particella[j].c_str(), surf[i]), Form("%s p_z Det %d; p_{z}(GeV); Particles/EOT", particella[j].c_str(), surf[i]), 100, 0, 0);

      TH1D *h_r  = new TH1D(Form("%s_r_Det%d", particella[j].c_str(), surf[i]), Form("%s r Det %d; r(cm); Particles/EOT", particella[j].c_str(), i), 100, 0, 0);
      TH1D *h_Thetap  = new TH1D(Form("%s_theta_p_Det%d", particella[j].c_str(), surf[i]), Form("%s theta_p Det %d; #theta_{p}(#circ); Particles/EOT", particella[j].c_str(), surf[i]), 100, 0, 0);

			TH1D *h_Energy  = new TH1D(Form("%s_Energy_Det%d", particella[j].c_str(), surf[i]), Form("%s energy Det %d; E (GeV); Particles/EOT", particella[j].c_str(), surf[i]), 200, 0, 0);
			TH1D *h_Energy_Log  = new TH1D(Form("%s_EnergyLog_Det%d", particella[j].c_str(), surf[i]), Form("%s energy Det %d; E (GeV); Particles/EOT", particella[j].c_str(), surf[i]), LogBinNumber, LogBinningEdges_array);
			TH1D *h_Energy_20_100  = new TH1D(Form("%s_Energy_20_100_Det%d", particella[j].c_str(), surf[i]), Form("%s energy 20-100 Det %d; E (GeV); Particles/EOT", particella[j].c_str(), surf[i]), 100, 20*1e-3,100*1e-3);
			TH1D *h_Energy_100_11  = new TH1D(Form("%s_Energy_100_11_Det%d", particella[j].c_str(), surf[i]), Form("%s energy 100-11 Det %d; E (GeV); Particles/EOT", particella[j].c_str(), surf[i]), 100, 100*1e-3,11);

			EvsMID[j].push_back(h_EvsMID);
      XvsY[j]     		.push_back(h_XvsY);

      x[j]        		.push_back(h_x);
      y[j]        		.push_back(h_y);
      z[j]        		.push_back(h_z);

      p[j]        		.push_back(h_p);
      py[j]       		.push_back(h_py);
      px[j]       		.push_back(h_px);
      pz[j]       		.push_back(h_pz);

      r[j]        		.push_back(h_r);
      theta_p[j]  		.push_back(h_Thetap);

      energy[j]   		.push_back(h_Energy);
      energyLog[j]   	.push_back(h_Energy_Log);
      energy20_100[j]   		.push_back(h_Energy_20_100);
      energy100_11[j]   		.push_back(h_Energy_100_11);

      th_pVSr[j]  		.push_back(h_ThetapvsR);
      m_EVSr[j]   		.push_back(h_EvsR);
    }
  }



	double m_m  = 0.1056583755;			//muon mass in GeV
	double n_m	= 0.939565378;			//neutron mass in GeV
	double deltaE = Emax - Emin;

  double n_px(0), n_py(0), n_pz(0), n_p(0), n_r_p(0), n_th_p(0);

  double m_r(0);
  double m_Ek(0), n_Ek(0);
  double peso_eot(0), peso_eot_neutroni(0);

  //*************************************************************************//

	//Usato per stimare il tempo di analisi
  auto start_t = std::chrono::steady_clock::now();
  cout<<"Inizio a costruire gli istogrammi..."<<endl;
	//*************************************************************************//

  //CICLO PRINCIPALE
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    //***************************************************
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);
    nbytes += nb;
    //***************************************************

		//Current particle r from center
		m_r =     sqrt(m_vx*m_vx + m_vy*m_vy);

    //Neutron kin energy & momentum
    n_Ek = m_E - n_m;
    n_p = sqrt(pow(m_E,2.0)-pow(n_m, 2.0));
    //Neutron momentum projections
    n_px = n_p * m_cx;
    n_py = n_p * m_cy;
    n_py = n_p * m_cz;
    //Neutron momentum radius & theta
    n_r_p =   sqrt(n_px*n_px + n_py*n_py + n_pz*n_pz);
    n_th_p =  atan2(sqrt(n_px*n_px+n_py*n_py),n_pz) * 180 / M_PI;


		//Definizione peso per eot
    peso_eot = m_weight2/ne;

		//Filling
    for(int i = 0; i<surf.size(); i++){

			//neutrino e antinuetrino elettronico [0]
      //for the old simulation swhere flux_ID start from 0 purt surf[i]-1
			if(m_fluxID == surf[i]-1 && (m_pid == 8)){

        //put here the number of the detector that should be XvsZ
				if(m_fluxID == 60000){
					XvsY[0][i]     -> Fill(m_vx, m_vz, peso_eot);
				}
				else{
					XvsY[0][i]     -> Fill(m_vx, m_vy, peso_eot);
				}

        EvsMID[0][i]->Fill(n_Ek, m_motherID, peso_eot);

				th_pVSr[0][i]  -> Fill(n_th_p, m_r, peso_eot);
				m_EVSr[0][i]   ->Fill(n_Ek,m_r,peso_eot);


				x[0][i]      -> Fill(m_vx,peso_eot);
				y[0][i]      -> Fill(m_vy,peso_eot);
				z[0][i]      -> Fill(m_vz,peso_eot);

				p[0][i]      -> Fill(n_p,peso_eot);
				px[0][i]     -> Fill(n_px,peso_eot);
				py[0][i]     -> Fill(n_py,peso_eot);
				pz[0][i]     -> Fill(n_pz,peso_eot);

				r[0][i]        -> Fill(m_r,peso_eot);
				theta_p[0][i]  -> Fill(n_th_p,peso_eot);

				energy[0][i]   -> Fill(n_Ek,peso_eot);
	      energyLog[0][i]   -> Fill(n_Ek,peso_eot);
				energy20_100[0][i]   -> Fill(n_Ek,peso_eot);
				energy100_11[0][i]   -> Fill(n_Ek,peso_eot);


			}




		}



	}

  //*************************************************************************//
  auto end_t = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end_t-start_t;
  cout<<"Ho finito di analizzare. ("<<elapsed_seconds.count()<<" s)" <<endl;
  cout<<"Inizio a salvare gli istogrammi..."<<endl;
	//*************************************************************************//


	//Write the filled histograms to a root file
  for(int j=0; j<1; j++){

    TFile *contenitore = new TFile(Form("Analisi_%s.root",particella[j].c_str()), "recreate");
    TTree *events = new TTree(Form("Histo_%d_11GeV", j), "prova");

    for(int i=0; i<surf.size(); i++){
      XvsY[j][i]     -> Write();

			// EvsMID[j][i]->Write();

      // r[j][i]        -> Write();
      // theta_p[j][i]  -> Write();

      energy[j][i]   -> Write();
      energy20_100[j][i]   -> Write();
      energy100_11[j][i]   -> Write();
      energyLog[j][i]   -> Write();

      // th_pVSr[j][i]  -> Write();
      // m_EVSr[j][i]   -> Write();

      // x[j][i]   ->Write();
      y[j][i]     ->Write();
      z[j][i]     ->Write();
      // p[j][i]    -> Write();
      // px[j][i]   -> Write();
      // py[j][i]   -> Write();
      // pz[j][i]   -> Write();

			}

		contenitore->Close();

		}

  cout<<"Ho finito di salvare gli istogrammi"<<endl;



}
