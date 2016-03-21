#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TMath.h"
#include "TSystem.h"
#include "TF1.h"
#include "TF2.h"
#include "TF3.h"
#include "TVector3.h"
#include "snprintf.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <math.h>

using namespace std;

void Print_Usage();
void Parse_Args(int *argc, char **argv);

extern TSystem *gSystem;


//////////////////////////////////////////////////////////////
// Options Globals
//////////////////////////////////////////////////////////////
char  outfilename[128];
char  infilename[100];

int main (int argc, char **argv) {

  if(argc < 2){
    Print_Usage();
    exit(1);
  }


  Parse_Args( &argc, argv );
 
  if( outfilename[0] == 0 ) {
    cout << "Specify Output File Name" << endl;
    Print_Usage();
    exit(1);
  }
  
  if( infilename[0] == 0 ) {
    cout << "Specify Input File Name" << endl;
    Print_Usage();
    exit(1);
  }  


  Int_t n_part;
  Double_t px[20];
  Double_t py[20];
  Double_t pz[20];
  Int_t particle_id[20];
  Double_t Ein_beam;

  Double_t weight;

  TBranch        *b_n_part;   //!
  TBranch        *b_px;   //!
  TBranch        *b_py;   //!
  TBranch        *b_pz;   //!
  TBranch        *b_Ein_beam;   //!
  TBranch        *b_weight;   //!
  TBranch        *b_particle_id;   //!


  TChain *input_chain = new TChain("T");

  input_chain->Add(infilename);

  input_chain->SetBranchAddress("n_part2", &n_part, &b_n_part);
  input_chain->SetBranchAddress("px2", px, &b_px);
  input_chain->SetBranchAddress("py2", py, &b_py);
  input_chain->SetBranchAddress("pz2", pz, &b_pz);
  input_chain->SetBranchAddress("weight2", &weight, &b_weight);
  input_chain->SetBranchAddress("particle_id2", particle_id, &b_particle_id);
  input_chain->SetBranchAddress("Ein_beam2", &Ein_beam, &b_Ein_beam);


  Int_t nentries = (Int_t)input_chain->GetEntries();
  

  TFile *fout = new TFile(outfilename,"RECREATE");

  TTree *fTree = new TTree("T", "HG Monte Carlo");
  fTree->Branch("n_part", &n_part, "n_part/I");
  fTree->Branch("px", px, "px[n_part]/D");
  fTree->Branch("py", py, "py[n_part]/D");
  fTree->Branch("pz", pz, "pz[n_part]/D");
  fTree->Branch("particle_id", particle_id, "particle_id[n_part]/I");
  fTree->Branch("Ein_beam", &Ein_beam, "Ein_beam/D");
  fTree->Branch("weight", &weight, "weight/D");
  for (int i=0; i<nentries ; i++) {
    input_chain->GetEntry(i);
    if(i % 100000 == 0 ){
      printf("Analyzed %09d events of total %09d \n",i,nentries);
    }
      
    // Here you can do some operation for changing format between the two TTree.

    fTree->Fill();
  
  }
  fout->cd();
  fTree->Write();
  fout->Close();

}




//***********************************************************************************
//  Parse arguments 
//***********************************************************************************
void Parse_Args(int *argc, char **argv){
  // Read and parse all the options, leaving only input files in the
  // argv array.
  // This version, all options flags are globals.
#define REMOVE_ONE {(*argc)--;for(j=i;j<(*argc);j++)argv[j]=argv[j+1];i--;}
#define I_PLUS_PLUS if((i+1)<(*argc)){i++;}else{break;}
  int  i,j;


  for( i=1; i<(*argc); i++ )  
    {
      if( argv[i][0] == '-' ) {
	if(strcmp(argv[i],"-o")==0 || strcmp(argv[i],"-outfile")==0)
	  {
	    I_PLUS_PLUS;
	    strcpy(outfilename,argv[i]);
	    REMOVE_ONE;
	  }
	else if(strcmp(argv[i],"-i")==0 || strcmp(argv[i],"-infile")==0)
	  {
	    I_PLUS_PLUS;
	    strcpy(infilename,argv[i]);
	    REMOVE_ONE;
	  }
	else if(strcmp(argv[i],"-help")==0||strcmp(argv[i],"-h")==0)  
	  {
	    Print_Usage();
	    exit(1);
	  }
	else
	  {
	    fprintf(stderr,"\nI did not understand the option : %s\n",argv[i]);
	    Print_Usage();
	    exit(1);
	  }
	/* KILL the option from list */
	REMOVE_ONE;
      }
    }
}

//***********************************************************************************
//  Print Usage 
//***********************************************************************************

void Print_Usage() {
  cout << " transformTTree : This Program translate the output from one TTree to another\n";  
  cout << " Usage: transformTTree  -o outputfile -i inputfile\n";  
  cout << "     -o outputfile     : output file name (example plutoout.root)  \n";  
  cout << "     -i inputfile      : input file name (example plutoin.root)  \n";  
  cout << "     -h help, print this message \n";
  cout << " --- \n\n";
}
