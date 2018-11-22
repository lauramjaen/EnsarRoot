// *********************************************************************
// *****   
// *****             EnsarUraniumChainGen_234Th source file                                   
// *****   	
// *****  Due to the 234Pa and 234mPa have an internal transition of
// *****  IT=0.16%, we are not going to consider the gamma of the 
// *****  73.9(3+)->0(4*) transition that has an energy of 73.9keV,	
// *****  so, insted of 2 as a final state we are going to put 0.
// *****  					
// *********************************************************************

#include "EnsarUraniumChainGen_234Th.h"

#include "FairPrimaryGenerator.h"

#include "TRandom.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"
#include "TArrayF.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <string>

#define fPDGBetaType 11
#define fPDGType 22

using namespace std;


// -----   Default constructor   ------------------------------------------
EnsarUraniumChainGen_234Th::EnsarUraniumChainGen_234Th()  :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
EnsarUraniumChainGen_234Th::EnsarUraniumChainGen_234Th(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarUraniumChainGen_234Th: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarUraniumChainGen_234Th","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarUraniumChainGen_234Th::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarUraniumChainGen_234Th: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarUraniumChainGen_234Th::~EnsarUraniumChainGen_234Th() {

  CloseInput();

}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarUraniumChainGen_234Th::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarUraniumChainGen_234Th: Input file not open!" << endl;
    return kFALSE;
  }
 
	
	
	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

	Int_t numInicialCases=4;

	Double_t Probability_emmittedParticles [numInicialCases];
	Probability_emmittedParticles [0]=0.703;
	Probability_emmittedParticles [1]=0.192;
	Probability_emmittedParticles [2]=0.076;
	Probability_emmittedParticles [3]=0.029;

	Double_t FinalState_daughter [numInicialCases];//final state of 234Pa*
	FinalState_daughter [0]=0;//it would be 2
	FinalState_daughter [1]=4;
	FinalState_daughter [2]=5;
	FinalState_daughter [3]=6;
			
	Double_t limits[numInicialCases+1];
	limits[0]=0;

	for (Int_t i=1;i<numInicialCases+1;i++){
		Double_t lim1, prob1;
		lim1=limits[i-1];
		prob1=Probability_emmittedParticles[i-1];
		limits[i]= lim1+ prob1;
		//cout<<"limits["<<i<<"]"<<limits[i]<<endl;//ok!
	}

	Float_t ran_ini =gRandom->Rndm();
	//cout<<"ran_ini="<<ran_ini<<endl;
	Int_t j=0;
	Int_t Case;
	int finalstate;

	if (ran_ini>limits[j] && ran_ini<limits[j+1]){
			Case=0;	//2
			//cout<<"Limits "<<limits[j]<<" - "<<limits[j+1]<<endl;//ok
	}
	else if (ran_ini>limits[j+1] && ran_ini<limits[j+2]){
			Case=1;//1			
			//cout<<"Limits "<<limits[j+1]<<" - "<<limits[j+2]<<endl;//ok
	}
	else if (ran_ini>limits[j+2] && ran_ini<limits[j+3]){
			Case=2;//0		
			//cout<<"Limits "<<limits[j+2]<<" - "<<limits[j+3]<<endl;//ok
	}
	else if (ran_ini>limits[j+3] && ran_ini<limits[j+4]){
			Case=3;	
			//cout<<"Limits "<<limits[j+3]<<" - "<<limits[j+4]<<endl;//ok
	}

	finalstate=FinalState_daughter[Case];
	
	Bool_t kINFO;
	kINFO=false;
	if (kINFO==true){cout<<"Case="<<Case<<"  the nucleus is in the state="<<finalstate<<endl;}
	

	Int_t Contador=1;

	while(finalstate>0){
		
		int finalstate2;
		Int_t size=0;
		TArrayF* Energy_gammas_array;
		Energy_gammas_array=new TArrayF();
		Double_t px,py,pz;
		px=0; py=0; pz=0;

		Energy_gammas_array=Deexcitation(finalstate,finalstate2);
		TrackMomentum(Energy_gammas_array->GetAt(0), px, py, pz);
		primGen->AddTrack(fPDGType, px, py, pz, fX, fY, fZ);		
				
		if (kINFO==true){
			cout<<endl<<"Counter of gammas="<<Contador<<endl;
			cout<<"		Energy_array size="<<Energy_gammas_array->GetSize()<<endl;
			cout<<"		Energy_gamma="<<Energy_gammas_array->GetAt(0)<<endl;
			cout<<"		Final state="<<finalstate2<<endl;
			cout<<"		Ading gamma of energy="<<Energy_gammas_array->GetAt(0)<<" p="<<px<<","<<py<<","<<pz<<endl;
		}

		finalstate=finalstate2;
		Contador++;
	}

	primGen->AddTrack(fPDGType, 0.0000001, 0.0000001, 0.0000001, 0., 0., 0.);
	if (kINFO==true){cout<<"////////////////////////////////////////////////"<<endl<<endl<<endl;}
	
  return kTRUE;
  
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void EnsarUraniumChainGen_234Th::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarUraniumChainGen_234Th: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarUraniumChainGen_234Th::ReadParameters() {

}

// --- De-excitation Process --------------------------------------------
TArrayF* EnsarUraniumChainGen_234Th::Deexcitation(Int_t InicialState, Int_t & FinalState) {
	
	// --- NUCLEAR DATA, deexcitation of 234Pa --- //
	Double_t Probability	[50][50];//[Nucleus inicial state][number of gammas]
	Double_t Energygammas	[50][50];
	Int_t 	 NucleusFinalState		[50][50];

	//2st excited state with 1 gamma
	Probability					[2][1]=1.;
	Energygammas				[2][1]=0.00007392;//MeV
	NucleusFinalState		[2][1]=0;


	//3rd excited state with 1 gamma
	Probability					[3][1]=1.;
	Energygammas				[3][1]=0.00002949;
	NucleusFinalState		[3][1]=0;//it would be 2

	//4th excited state with 2 gammas
	Probability					[4][1]=0.9926;
	Energygammas				[4][1]=0.00009238;
	NucleusFinalState		[4][1]=0;//it would be 2

	Probability					[4][2]=0.0074;
	Energygammas				[4][2]=0.00006286;
	NucleusFinalState		[4][2]=3;

	//5th excited state with 2 gammas
	Probability					[5][1]=0.6369;
	Energygammas				[5][1]=0.00006329;
	NucleusFinalState		[5][1]=3;

	Probability					[5][2]=0.3631;
	Energygammas				[5][2]=0.00009280;
	NucleusFinalState		[5][2]=0;//it would be 2

	//6th excited state with 3 gammas
	Probability					[6][1]=0.7564;
	Energygammas				[6][1]=0.00011281;
	NucleusFinalState		[6][1]=0;//it would be 2

	Probability					[6][2]=0.2163;
	Energygammas				[6][2]=0.00008330;
	NucleusFinalState		[6][2]=3;

	Probability					[6][3]=0.0273;//0.0272 to Prob=1
	Energygammas				[6][3]=0.00002002;
	NucleusFinalState		[6][3]=5;

	Int_t numGammas;
	Float_t random =gRandom->Rndm();

	if (InicialState==2 || InicialState==3){numGammas=1;}
	else if (InicialState==4 || InicialState==5){numGammas=2;}
	else if (InicialState==6){numGammas=3;}
	else{
		cout << "-E- EnsarUraniumChainGen_234Th: Incorrect final state!" << endl;
	}
	
	//cout<<"		Entering into the Deexcitation function with an Inicial state="<<InicialState<<" which has a numGamma="<<numGammas<<endl;	

	TArrayF* EnergyGamma_array;
	EnergyGamma_array= new TArrayF();
	EnergyGamma_array->Set(1);

	//Limits calculus
	Double_t limits2[numGammas+1];
	limits2[0]=0;

	for (Int_t i=1;i<numGammas+1;i++){
		limits2[i]= limits2[i-1] + Probability[InicialState][i];
		//cout<<"		limits2["<<i<<"]"<<limits2[i]<<endl;
	}
	
	for (Int_t i=0;i<numGammas;i++){
		if (random>limits2[i] && random<limits2[i+1]){
				//cout<<"		Inside loop: "<<random<<" exists between "<<limits2[i]<<" and "<<limits2[i+1]<<endl;
				//cout<<"		Inside loop: the Gamma emmited is the "<<i+1<<endl;
				EnergyGamma_array->SetAt(Energygammas[InicialState][i+1],0);
				FinalState=NucleusFinalState[InicialState][i+1];
				//cout<<"		Inside loop: FinalState="<<NucleusFinalState[InicialState][i+1]<<endl;
		}
	}
	
	return EnergyGamma_array;
}

// --- Track Momentum Calculus --------------------------------------------
void EnsarUraniumChainGen_234Th::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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

ClassImp(EnsarUraniumChainGen_234Th)
