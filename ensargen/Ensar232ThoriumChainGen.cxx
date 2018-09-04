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

#include <iostream>
#include <fstream>
#include <istream>
#include <string>

#define fPDGType 22

using namespace std;

/*
Double_t* fenergy;			//Energy of gammas
    Double_t* fprobability;	//Probability of each gamma

		Bool_t     fPointVtxIsSet;       // True if point vertex is set
    Bool_t     fBoxVtxIsSet;         // True if box vertex is set
		Bool_t 		 fThetaRangeIsSet;			//True if Theta range is set
		Bool_t		 fPhiRangeIsSet;				// True if Phi range is set
		Double_t fX, fY, fZ;           // Point vertex coordinates [cm]
    Double_t fX1, fY1, fX2, fY2;   // Box vertex coords (x1,y1)->(x2,y2)
		Double_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]
		Double_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
*/

// -----   Default constructor   ------------------------------------------
Ensar232ThoriumChainGen::Ensar232ThoriumChainGen()  :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fX2(0), fY2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
Ensar232ThoriumChainGen::Ensar232ThoriumChainGen(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fX2(0), fY2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
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
	//Checking out the probabilities
	Double_t sumProb;
	sumProb=0;
	for (Int_t i=0; i<fnumGammas; i++){
		sumProb=sumProb+fprobability[i];
	}
	if (sumProb>1.001){
		Fatal("Init()","Ensar232ThoriumChainGen: The sum of all probabilities is higher than 1! Modify it, please.");		
	}
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","Ensar232ThoriumChainGen: Cannot set point and box vertices simultaneously");
  }
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
  
  //cout<<"We are Here!"<<endl;

	for(Int_t i = 0; i < fnumGammas; i++){
		//cout<<"Gamma i="<<i<<endl;
    //cout<<"Energy= "<<fenergy[i]<<endl;
    //cout<<"Probability= "<<fprobability[i]<<endl;
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
	
	for(Int_t i=0; i<fnumGammas+1; i++){
		//cout<<"i="<<i<<"  up_limit ="<<up_limit[i]<<endl;
	}

	Int_t whichGamma;
 	Float_t ran =gRandom->Rndm();
  //cout<<"ran= "<<ran<<endl;

	for (Int_t i=0; i<fnumGammas; i++){
		if (ran>up_limit[i] && ran<up_limit[i+1]){
			whichGamma=i;
			//cout<<"whichGamma="<<whichGamma<<"   up_lim[i]="<<up_limit[i]<<"   up_lim[i+1]="<<up_limit[i+1]<<endl;
		}
	}

	//Angles
	Double_t phi;		//0-2pi
	Double_t theta;	//0-pi
									//Remember: we always consider the CosTheta
									//if not it will be: theta = gRandom->Uniform(fThetaMin,fThetaMax) * TMath::DegToRad();

	if (fThetaRangeIsSet==kTRUE){
		theta = acos(gRandom->Uniform(cos(fThetaMin* TMath::DegToRad()),
                                      cos(fThetaMax* TMath::DegToRad())));
	}else { theta = TMath::ACos(1-2*gRandom->Rndm());}

	if (fPhiRangeIsSet==kTRUE){
		phi = gRandom->Uniform(fPhiMin,fPhiMax) * TMath::DegToRad();
	}else{ phi = 6.283185307  *gRandom->Rndm();}
	
	//cout<<endl<<endl<<endl;
	
	//cout<<"theta2="<<theta*180/TMath::Pi()<<"  phi2="<<phi*180/TMath::Pi()<<endl;
	//cout<<"theta3="<<TMath::ACos(1-2*gRandom->Rndm())<<"  phi3="<<6.283185307  *gRandom->Rndm()<<endl;

  //Direction of gamma RS Lab
  TVector3 direction;  
  direction = TVector3(TMath::Sin(theta)*TMath::Cos(phi),
			TMath::Sin(theta)*TMath::Sin(phi),
			TMath::Cos(theta));	

	//Momentum
	Double_t px, py, pz;
	px= fenergy[whichGamma]*direction.X();
  py= fenergy[whichGamma]*direction.Y();
  pz= fenergy[whichGamma]*direction.Z();  

	//cout<<"fenergy[whichGamma]="<<fenergy[whichGamma]<<endl;
	//cout<<"px="<<px<<"  py="<<py<<"  pz="<<pz<<endl;

	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
	}

	//cout<<"fX="<<fX<<" fY="<<fY<<" fZ="<<fZ<<endl;

  //adding the gammas 232Th chain
  primGen->AddTrack(fPDGType, px, py, pz, fX, fY, fZ);
 
  
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
void Ensar232ThoriumChainGen::ReadParameters() {

  cout << "\n\n\t------ READING PARAMETERS ------\n" << endl;
    
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
		//cout<<"Gamma i="<<i+1<<endl;
    //cout<<"Energy= "<<fenergy[i]<<endl;
    //cout<<"Probability= "<<fprobability[i]<<endl;
  }  


	cout << "-------------------------------------------------" << endl;  
}
//-------------------------------------------------------------------------

ClassImp(Ensar232ThoriumChainGen)
