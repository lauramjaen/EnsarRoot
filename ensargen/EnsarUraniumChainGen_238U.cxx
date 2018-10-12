// *********************************************************************
// *****   
// *****             EnsarUraniumChainGen_238U source file                                   
// *****   							
// *********************************************************************

#include "EnsarUraniumChainGen_238U.h"

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


// -----   Default constructor   ------------------------------------------
EnsarUraniumChainGen_238U::EnsarUraniumChainGen_238U()  :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0), 
	f_numAlphas_238U(0),f_EnergyAlphas_238U(0), 
	f_ProbabilityAlphas_238U(0), f_FinalState_238U(0)
{
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
EnsarUraniumChainGen_238U::EnsarUraniumChainGen_238U(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0),
	f_numAlphas_238U(0),f_EnergyAlphas_238U(0), 
	f_ProbabilityAlphas_238U(0), f_FinalState_238U(0)
{
  
  cout << "-I- EnsarUraniumChainGen_238U: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarUraniumChainGen_238U","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarUraniumChainGen_238U::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarUraniumChainGen_238U: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarUraniumChainGen_238U::~EnsarUraniumChainGen_238U() {

  CloseInput();
}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarUraniumChainGen_238U::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarUraniumChainGen_238U: Input file not open!" << endl;
    return kFALSE;
  }
 
	
	
	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

/*
Probability	[238][92][0][1]=0.209;
Energy			[238][92][0][1]=4151.5;
Final_A			[238][92][0][1]=234;
Final_Z			[238][92][0][1]=90;
Final_State	[238][92][0][1]=1;

Probability	[238][92][0][2]=0.79;
Energy			[238][92][0][2]=4198;
Final_A			[238][92][0][2]=234;
Final_Z			[238][92][0][2]=90;
Final_State	[238][92][0][2]=0;
*/
//Float_t ran1 =gRandom->Rndm();


	Int_t numAlphas_238U=2;

	Double_t EnergyAlphas_238U [f_numAlphas_238U];
	EnergyAlphas_238U [0]=4151.5;
	EnergyAlphas_238U [1]=4198;

	Double_t ProbabilityAlphas_238U [f_numAlphas_238U];
	ProbabilityAlphas_238U [0]=0.209;
	ProbabilityAlphas_238U [1]=0.791;

	Double_t FinalState_238U [f_numAlphas_238U];
	FinalState_238U [0]=1;
	FinalState_238U [1]=2;

			
	Double_t limits[numAlphas_238U+1];
	limits[0]=0;

	for (Int_t i=1;i<numAlphas_238U+1;i++){
		Double_t lim1, prob1;
		lim1=limits[i-1];
		prob1=ProbabilityAlphas_238U[i-1];
		limits[i]= lim1+ prob1;
		cout<<"limits["<<i<<"]"<<limits[i]<<endl;
	}

	Float_t ran_ini =gRandom->Rndm();
cout<<"ran_ini="<<ran_ini<<endl;
	Int_t j=0;
	Int_t Case;
	int finalstate;
	if (ran_ini>limits[j] && ran_ini<limits[j+1]){
			Case=1;
			//finalstate=1;
	}
	else if (ran_ini>limits[j+1] && ran_ini<limits[j+2]){
			Case=2;
			//finalstate=0;
	}

	finalstate=FinalState_238U[Case-1];
	cout<<"Case="<<Case<<"  finalstate="<<finalstate<<endl;

/* if we want to add alphas(PID=?¿) to the code:

		Double_t 1st_px, 1st_py, 1st_pz;
		Double_t 1st_energy;
		1st_energy=EnergyAlphas_238U[Case-1];

		TrackMomentum(1st_energy, 1st_px, 1st_py, 1st_pz);
		primGen->AddTrack(fPDGType, 1st_px, 1st_py, 1st_pz, fX, fY, fZ);

*/

	Int_t Chain_states[50];
	Chain_states[0]=finalstate;
	Int_t i;
	i=1;
	Double_t energy;
	energy=0;

	while(finalstate>0){
		int finalstate2;
		finalstate2=WhichState(finalstate,energy);
		Chain_states[i]=finalstate2;	
		finalstate=finalstate2;
		//cout<<"i="<<i<<"  Chain_states="<<Chain_states[i]<<endl;
		i++;
	}

Int_t size;
if(Case==1)size=2;
else size=1;

for (Int_t k=0; k<size; k++){
	cout<<"  Chain of states["<<k<<"]="<<Chain_states[k]<<endl;
}


/*
Double_t px_186state, py_186state, pz_186state;	
Egamma1=0.0000833;
Egamma2=0.00012281;
Float_t ran =gRandom->Rndm();
	if (ran1>0 && ran1<0.2163){
			TrackMomentum(Egamma1, px_186state, py_186state, pz_186state);
			//primGen->AddTrack(fPDGType, px_186state, py_186state, pz_186state, fx, fy, fz);
			FinalState=103.42;
	}
*/
	if (energy>0){

		Double_t px,py,pz;
		TrackMomentum(energy, px, py, pz);
		primGen->AddTrack(fPDGType, px, py, pz, fX, fY, fZ);

		cout<<"Ading gamma of energy="<<energy<<" p="<<px<<","<<py<<","<<pz<<endl;

	}else primGen->AddTrack(fPDGType, 0.0000001, 0.0000001, 0.0000001, 0., 0., 0.);

	cout<<"////////////////////////////////////////////////"<<endl<<endl<<endl;
  return kTRUE;
  
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void EnsarUraniumChainGen_238U::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarUraniumChainGen_238U: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarUraniumChainGen_238U::ReadParameters() {

}


// --- Track Momentum Calculus --------------------------------------------
int EnsarUraniumChainGen_238U::WhichState(Int_t FinalState, Double_t & Egamma) {
	
	int FinalState2;
	//FinalState2=0;
	if (FinalState==1){
			Egamma=49.55;//keV
			FinalState2=0;
	}
	return FinalState2;

/*	
Double_t px_186state, py_186state, pz_186state;	
Double_t Egamma1, Egamma2;
Egamma1=0.0000833;
Egamma2=0.00012281;
Float_t ran =gRandom->Rndm();
	if (ran1>0 && ran1<0.2163){
			TrackMomentum(Egamma1, px_186state, py_186state, pz_186state);
			//primGen->AddTrack(fPDGType, px_186state, py_186state, pz_186state, fx, fy, fz);
			FinalState=103.42;
	}
	if (ran1>0.2163 && ran1<0.9727){
			TrackMomentum(Egamma2, px_186state, py_186state, pz_186state);
			//primGen->AddTrack(fPDGType, px_186state, py_186state, pz_186state, fx, fy, fz);
			FinalState=73.92;
	}*/

}


// --- Track Momentum Calculus --------------------------------------------
void EnsarUraniumChainGen_238U::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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
		
	//cout<<"theta2="<<theta*180/TMath::Pi()<<"  phi2="<<phi*180/TMath::Pi()<<endl;

  //Direction of gamma RS Lab
  TVector3 direction;  
  direction = TVector3(TMath::Sin(theta)*TMath::Cos(phi),
			TMath::Sin(theta)*TMath::Sin(phi),
			TMath::Cos(theta));	

	//Momentum
	Px= Energy*direction.X();
  Py= Energy*direction.Y();
  Pz= Energy*direction.Z(); 
	
	//cout<<"TrackMomentum function  Px="<<Px<<"  Py="<<Py<<"  Pz="<<Pz<<endl; 
}
//-------------------------------------------------------------------------

ClassImp(EnsarUraniumChainGen_238U)
