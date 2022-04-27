#ifndef neutroni_h
#define neutroni_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

//Header file for the classes stored in the TTree if any.

class neutroni {
public :
  //Nome del file da aprire
  string FileName = "simulazione.root";

  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain

  // Declaration of leaf types
  //Results         *flux;
  Int_t           m_eventn;
  Int_t           m_pid;
  Int_t           m_fluxID;
  Double_t        m_E;
  Double_t        m_cx;
  Double_t        m_cy;
  Double_t        m_cz;
  Double_t        m_vx;
  Double_t        m_vy;
  Double_t        m_vz;
  Double_t        m_weight;
  Double_t        m_weight2;
  Int_t           m_motherID;
  Int_t           m_procID;
  Double_t        m_motherE;
  Double_t        m_vertexX;
  Double_t        m_vertexY;
  Double_t        m_vertexZ;

  //added varibables
  // int debug;

  // List of branches
  TBranch        *b_flux_m_eventn;   //!
  TBranch        *b_flux_m_pid;   //!
  TBranch        *b_flux_m_fluxID;   //!
  TBranch        *b_flux_m_E;   //!
  TBranch        *b_flux_m_cx;   //!
  TBranch        *b_flux_m_cy;   //!
  TBranch        *b_flux_m_cz;   //!
  TBranch        *b_flux_m_vx;   //!
  TBranch        *b_flux_m_vy;   //!
  TBranch        *b_flux_m_vz;   //!
  TBranch        *b_flux_m_weight;   //!
  TBranch        *b_flux_m_weight2;   //!
  TBranch        *b_flux_m_motherID;   //!
  TBranch        *b_flux_m_procID;   //!
  TBranch        *b_flux_m_motherE;   //!
  TBranch        *b_flux_m_vertexX;   //!
  TBranch        *b_flux_m_vertexY;   //!
  TBranch        *b_flux_m_vertexZ;   //!

  neutroni(TTree *tree=0);

  virtual ~neutroni();

  //A virtual function is a member function which is declared within a base class and is re-defined (overridden) by a derived class.

  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

};

#endif

#ifdef neutroni_cxx
neutroni::neutroni(TTree *tree) : fChain(0){
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(FileName.c_str());
      if (!f || !f->IsOpen()) {
         f = new TFile(FileName.c_str());
      }
      f->GetObject("out",tree);

   }
   Init(tree);
}

neutroni::~neutroni(){
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t neutroni::GetEntry(Long64_t entry){
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t neutroni::LoadTree(Long64_t entry){
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void neutroni::Init(TTree *tree){
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  fChain->SetBranchAddress("m_eventn", &m_eventn, &b_flux_m_eventn);
  fChain->SetBranchAddress("m_pid", &m_pid, &b_flux_m_pid);
  fChain->SetBranchAddress("m_fluxID", &m_fluxID, &b_flux_m_fluxID);
  fChain->SetBranchAddress("m_E", &m_E, &b_flux_m_E);
  fChain->SetBranchAddress("m_cx", &m_cx, &b_flux_m_cx);
  fChain->SetBranchAddress("m_cy", &m_cy, &b_flux_m_cy);
  fChain->SetBranchAddress("m_cz", &m_cz, &b_flux_m_cz);
  fChain->SetBranchAddress("m_vx", &m_vx, &b_flux_m_vx);
  fChain->SetBranchAddress("m_vy", &m_vy, &b_flux_m_vy);
  fChain->SetBranchAddress("m_vz", &m_vz, &b_flux_m_vz);
  fChain->SetBranchAddress("m_weight", &m_weight, &b_flux_m_weight);
  fChain->SetBranchAddress("m_weight2", &m_weight2, &b_flux_m_weight2);
  fChain->SetBranchAddress("m_motherID", &m_motherID, &b_flux_m_motherID);
  fChain->SetBranchAddress("m_procID", &m_procID, &b_flux_m_procID);
  fChain->SetBranchAddress("m_motherE", &m_motherE, &b_flux_m_motherE);
  fChain->SetBranchAddress("m_vertexX", &m_vertexX, &b_flux_m_vertexX);
  fChain->SetBranchAddress("m_vertexY", &m_vertexY, &b_flux_m_vertexY);
  fChain->SetBranchAddress("m_vertexZ", &m_vertexZ, &b_flux_m_vertexZ);
  Notify();
}

Bool_t neutroni::Notify(){
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void neutroni::Show(Long64_t entry){
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t neutroni::Cut(Long64_t entry){
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
#endif // #ifdef neutroni_cxx
