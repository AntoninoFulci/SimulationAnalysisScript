#include <dirent.h>
#include <chrono>
#include <sys/stat.h>

void GraphFolder(string f_path = "Grafici", bool debug = 0){
  struct stat st;
	string path = f_path;
	if(stat(path.c_str(),&st) == 0){
  	if(debug) cout<<"The directory "<<path<<" already exists skipping creating it"<<endl;
	}
	else{
		cout<<path<<" does not exits, creating it"<<endl;
		mkdir(path.c_str(), ACCESSPERMS);
	}
}


//get the name of the analyzed file
string analys_file(int which_file, bool debug = 0){
  DIR *dir;
  struct dirent *ent;
  vector<string> files;
  if ((dir = opendir (".")) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      // printf ("%s\n", ent->d_name);
      files.push_back(ent->d_name);
    }
    closedir (dir);
  }

  string analys_file;

  if(which_file == 1){
    for(auto it: files){
      if (it.find("Analisi_ane&ne") != std::string::npos) analys_file = it;
    }
  }
  else if(which_file == 0){
    for(auto it: files){
      if (it.find("simulazione") != std::string::npos) analys_file = it;
    }
  }
  else if(which_file == 2){
    for(auto it: files){
      if (it.find("Analisi_neutron") != std::string::npos) analys_file = it;
    }
  }

  if(debug){
    cout << analys_file <<endl;
  }
  return analys_file;
}

//Get the number of surfaces used on the mgdraw.f file
vector<int> GetNSup(int debug = 0){

  string FileName = analys_file(0);

  //Use to know how much time passes from when this function is called and when
  //it return the number of surfaces
  auto start_t = std::chrono::steady_clock::now();

  TH1F *h_fluxID = new TH1F("h_fluxID","h_fluxID",1000,0,1000);
  auto myFile = TFile::Open(FileName.c_str());

  TTreeReader myReader("out", myFile);
  TTreeReaderValue<Int_t> i_fluxID(myReader, "m_fluxID");
  while (myReader.Next()) {
      h_fluxID->Fill(*i_fluxID);
  }

  int buffer;
  vector <int> surfaces;
  for(int i = 0; i<1000; i++){
    buffer = h_fluxID->GetBinContent(i);
    if(buffer != 0){
      surfaces.push_back(i);
    }
  }

  // cout<<"Debug GetNSup:"<<endl;
  // cout<<surfaces.size()<<endl;

  return surfaces;

}

//Find the number of simulates electrons
//This function assumes that the number of simulated electrons is equal in every running
//Check later if this number is correct
int GetNEOT(int debug = 0){
  string FileName = analys_file(0);

	auto h_nEOT = new TH1F("h1","ntuple",100,0,0);
	auto myFile = TFile::Open(FileName.c_str());

	TTreeReader myReader("CycleSummary", myFile);
	TTreeReaderValue<Int_t> nEOT(myReader, "nEOT");
	while (myReader.Next()) {
			h_nEOT->Fill(*nEOT);
	}

  //Here the calculus!!!
	int ne = h_nEOT->GetMean()*h_nEOT->GetEntries();

  return ne;
}


vector<double> LogBinning(double EMax, double EMin, int NumberoBin){

  double Emax = EMax;
	double Emin = EMin;

	int LogBinNumber = NumberoBin;

	double lMaxE = log10(Emax);
	double lMinE = log10(Emin);
	double lWidth = (lMaxE - lMinE)/LogBinNumber;

	vector <double> LogBinningEdges (LogBinNumber+1);

	for(int i = 0; i<=  LogBinNumber; i++){
		LogBinningEdges[i] = TMath::Power(10,lMinE+i * lWidth);
	}

  return LogBinningEdges;
}
