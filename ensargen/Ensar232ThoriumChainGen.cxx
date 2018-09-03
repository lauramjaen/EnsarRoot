// *********************************************************************
// *****             Ensar232ThoriumChainGen source file              
// *****   						
// ***** 			
// ***** elisabet.galiana@usc.es
// ***** Universidad de Santiago de Compostela                    
// ***** Dpto. Física de Partículas                               
// *****   							
// *********************************************************************

#include "Ensar232ThoriumChainGen.h"

#include "FairPrimaryGenerator.h"

#include "TRandom.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"
#include "TF1.h"
//#include "Math/WrappedTF1.h"
//#include "Math/BrentRootFinder.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <string>

#define fPDGType 22

using namespace std;

// -----   Default constructor   ------------------------------------------
Ensar232ThoriumChainGen::Ensar232ThoriumChainGen()  :
  FairGenerator() 
{
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
Ensar232ThoriumChainGen::Ensar232ThoriumChainGen(const char* inputFile) :
  FairGenerator() 
{
  
  cout << "-I- Ensar232ThoriumChainGen: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  //fInputFile = new TFile(fFileName,"READ");
  if ( ! fInputFile->is_open() ) Fatal("Ensar232ThoriumChainGen","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  Ensar232ThoriumChainGen::Init()
{

}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
Ensar232ThoriumChainGen::~Ensar232ThoriumChainGen() {
  CloseInput();
}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t Ensar232ThoriumChainGen::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- Ensar232ThoriumChainGen: Input file not open!" << endl;
    return kFALSE;
  }
  
  cout<<"We are Here!"<<endl;

	for(Int_t i = 0; i < fnumGammas; i++){
		cout<<"Gamma i="<<i+1<<endl;
		//Double_t e=0.;
		//Double_t p=0.;
		//e=fenergy;
		//p=fprobability;
    cout<<"Energy= "<<fenergy[i]<<endl;
    cout<<"Probability= "<<fprobability[i]<<endl;
  }  

	//Limits calculation
	Double_t up_limit[fnumGammas+1];

	for(Int_t i=0; i<fnumGammas+1; i++){

		if(i==0){
			up_limit[i]=0.;
		}else{
			for(Int_t j=0;j<i;j++){up_limit[i]=fprobability[j]+up_limit[i];}
		}
	}
	
	for(Int_t i=0; i<fnumGammas; i++){
		cout<<"i="<<i<<"  up_limit ="<<up_limit[i]<<endl;
	}

	//Gamma vertex in the origen 
  Double_t X=0.;
  Double_t Y=0.;
  Double_t Z=0.;

	Double_t px1=0.5;
  Double_t py1=0.5;
  Double_t pz1=0.5;
  
	Double_t px2=0.5;
  Double_t py2=0.5;
  Double_t pz2=0.5;
  
  //adding the gammas 232Th chain
  primGen->AddTrack(fPDGType, px1, py1, pz1, X, Y, Z);
  primGen->AddTrack(fPDGType, px2, py2, pz2, X, Y, Z);


//

  
  return kTRUE;
  
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void Ensar232ThoriumChainGen::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- Ensar232ThoriumChainGenr: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------

// ---Read Si* Parameters File --------------------------------------------
bool Ensar232ThoriumChainGen::ReadParameters() {

  cout << "\n\n\t------ READING 232Th-chain PARAMETERS ------\n" << endl;
    
	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');

  //---- Total number of gammas -----
  //Int_t numGammas;
	*fInputFile >> fnumGammas;

  //---- Reading the energy and the probability of population for each gamma

  //def dynamic vectors	
  fenergy		= new Double_t [fnumGammas];	//energy
  fprobability = new Double_t [fnumGammas];	//probability

 	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  
	for(int i = 0; i < fnumGammas; i++){
    *fInputFile >> fenergy[i] >> fprobability[i];
		cout<<"Gamma i="<<i+1<<endl;
		//Double_t e=0.;
		//Double_t p=0.;
		//e=fenergy;
		//p=fprobability;
    cout<<"Energy= "<<fenergy[i]<<endl;
    cout<<"Probability= "<<fprobability[i]<<endl;
  }  

	//Checking out the probabilities
	Double_t sumProb;
	sumProb=0;
	for (Int_t i=0; i<fnumGammas; i++){
		sumProb=sumProb+fprobability[i];
	}
	if(sumProb>1){
		cout<<"-E- Ensar232ThoriumChainGen: The total probability is higher than 1!! Modify it, please."<<endl;//REvisar no va be
		return kFALSE;
	}else{
		cout<<"-E- Ensar232ThoriumChainGen: The total probability is "<<sumProb<<endl;
		return kTRUE;
	}
	cout << "-------------------------------------------------" << endl;  
}
//-------------------------------------------------------------------------

ClassImp(Ensar232ThoriumChainGen)
