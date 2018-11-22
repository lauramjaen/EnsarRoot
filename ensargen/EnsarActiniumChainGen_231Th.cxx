// *********************************************************************
// *****   
// *****             EnsarActiniumChainGen_231Th source file                                   
// *****   							
// *********************************************************************

#include "EnsarActiniumChainGen_231Th.h"

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

#define fPDGType 22

using namespace std;


// -----   Default constructor   ------------------------------------------
EnsarActiniumChainGen_231Th::EnsarActiniumChainGen_231Th()  :
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
EnsarActiniumChainGen_231Th::EnsarActiniumChainGen_231Th(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarActiniumChainGen_231Th: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarActiniumChainGen_231Th","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarActiniumChainGen_231Th::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarActiniumChainGen_231Th: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarActiniumChainGen_231Th::~EnsarActiniumChainGen_231Th() {

  CloseInput();

}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarActiniumChainGen_231Th::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarActiniumChainGen_231Th: Input file not open!" << endl;
    return kFALSE;
  }
 
	
	
	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

	Int_t numInicialCases=6;

	Double_t Probability_emmittedParticles [numInicialCases];
	Probability_emmittedParticles [0]=0.32;
	Probability_emmittedParticles [1]=0.4;
	Probability_emmittedParticles [2]=0.12;
	Probability_emmittedParticles [3]=0.013;
	Probability_emmittedParticles [4]=0.121;
	Probability_emmittedParticles [5]=0.026;


	Double_t FinalState_daughter [numInicialCases];
	FinalState_daughter [0]=4;	
	FinalState_daughter [1]=5;
	FinalState_daughter [2]=6;
	FinalState_daughter [3]=8;
	FinalState_daughter [4]=9;
	FinalState_daughter [5]=10;

			
	Double_t limits[numInicialCases+1];
	limits[0]=0;

	for (Int_t i=1;i<numInicialCases+1;i++){
		Double_t lim1, prob1;
		lim1=limits[i-1];
		prob1=Probability_emmittedParticles[i-1];
		limits[i]= lim1+ prob1;
		//cout<<"limits["<<i<<"]"<<limits[i]<<endl;
	}

	Float_t ran_ini =gRandom->Rndm();
	//cout<<"ran_ini="<<ran_ini<<endl;
	Int_t j=0;
	Int_t Case;
	int finalstate;

	if (ran_ini>limits[j] && ran_ini<limits[j+1]){
			Case=0;	
			//cout<<"Limits "<<limits[j]<<" - "<<limits[j+1]<<endl;
	}
	else if (ran_ini>limits[j+1] && ran_ini<limits[j+2]){
			Case=1;			
			//cout<<"Limits "<<limits[j+1]<<" - "<<limits[j+2]<<endl;
	}
	else if (ran_ini>limits[j+2] && ran_ini<limits[j+3]){
			Case=2;			
			//cout<<"Limits "<<limits[j+2]<<" - "<<limits[j+3]<<endl;
	}
	else if (ran_ini>limits[j+3] && ran_ini<limits[j+4]){
			Case=3;			
			//cout<<"Limits "<<limits[j+3]<<" - "<<limits[j+4]<<endl;
	}
	else if (ran_ini>limits[j+4] && ran_ini<limits[j+5]){
			Case=4;			
			//cout<<"Limits "<<limits[j+4]<<" - "<<limits[j+5]<<endl;
	}
	else if (ran_ini>limits[j+5] && ran_ini<limits[j+6]){
			Case=5;			
			//cout<<"Limits "<<limits[j+5]<<" - "<<limits[j+6]<<endl;
	}

	finalstate=FinalState_daughter[Case];
	
	Bool_t kINFO;
	kINFO=false;
	if (kINFO==true){cout<<"Case="<<Case<<"  the nucleus is in the state="<<finalstate<<endl;}
	

	Int_t Counter=1;

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
			cout<<endl<<"Counter of gammas="<<Counter<<endl;
			cout<<"		Energy_array size="<<Energy_gammas_array->GetSize()<<endl;
			cout<<"		Energy_gamma="<<Energy_gammas_array->GetAt(0)<<endl;
			cout<<"		Final state="<<finalstate2<<endl;
			cout<<"		Ading gamma of energy="<<Energy_gammas_array->GetAt(0)<<" p="<<px<<","<<py<<","<<pz<<endl;
		}

		finalstate=finalstate2;
		Counter++;
	}

	primGen->AddTrack(fPDGType, 0.0000001, 0.0000001, 0.0000001, 0., 0., 0.);
	if (kINFO==true){cout<<"////////////////////////////////////////////////"<<endl<<endl<<endl;}
	
  return kTRUE;
  
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void EnsarActiniumChainGen_231Th::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarActiniumChainGen_231Th: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarActiniumChainGen_231Th::ReadParameters() {

}

// --- De-excitation Process --------------------------------------------
TArrayF* EnsarActiniumChainGen_231Th::Deexcitation(Int_t InicialState, Int_t & FinalState) {
	
	// --- NUCLEAR DATA, deexcitation --- //
	Double_t Probability	[50][50];//[Nucleus inicial state][number of gammas]
	Double_t Energygammas	[50][50];
	Int_t 	 NucleusFinalState		[50][50];

	//1st excited state with 1 gamma
	Energygammas				[1][1]=0.0000092;
	Probability					[1][1]=1.;
	NucleusFinalState		[1][1]=0;


	Energygammas				[2][1]=0.00005857;
	Probability					[2][1]=1.;
	NucleusFinalState		[2][1]=0;


	Energygammas				[3][1]=0.0000685;
	Probability					[3][1]=0.5814;
	NucleusFinalState		[3][1]=1;

	Energygammas				[3][2]=0.00007769;
	Probability					[3][2]=0.4186;
	NucleusFinalState		[3][2]=0;


	Energygammas				[4][1]=0.00002564;
	Probability					[4][1]=0.68166;
	NucleusFinalState		[4][1]=2;

	Energygammas				[4][2]=0.000084214;
	Probability					[4][2]=0.31834;
	NucleusFinalState		[4][2]=0;



	Energygammas				[5][1]=0.00004286;
	Probability					[5][1]=1.;
	NucleusFinalState		[5][1]=2;


	Energygammas				[6][1]=0.00001807;
	Probability					[6][1]=0.04322;
	NucleusFinalState		[6][1]=4;

	Energygammas				[6][2]=0.00009302;
	Probability					[6][2]=0.09248;
	NucleusFinalState		[6][2]=1;

	Energygammas				[6][3]=0.00010227;
	Probability					[6][3]=0.8643;
	NucleusFinalState		[6][3]=0;


	Energygammas				[7][1]=0.00001025;
	Probability					[7][1]=1.;
	NucleusFinalState		[7][1]=5;


	Energygammas				[8][1]=0.000072751;
	Probability					[8][1]=0.1984;
	NucleusFinalState		[8][1]=5;

	Energygammas				[8][2]=0.00008995;
	Probability					[8][2]=0.7841;
	NucleusFinalState		[8][2]=4;

	Energygammas				[8][3]=0.00011563;
	Probability					[8][3]=0.0008;
	NucleusFinalState		[8][3]=2;

	Energygammas				[8][4]=0.000165;
	Probability					[8][4]=0.0027;
	NucleusFinalState		[8][4]=1;

	Energygammas				[8][5]=0.00017415;
	Probability					[8][5]=0.0140;
	NucleusFinalState		[8][5]=0;

	

	Energygammas				[9][1]=0.000081228;
	Probability					[9][1]=0.5817;
	NucleusFinalState		[9][1]=6;

	Energygammas				[9][2]=0.000082087;
	Probability					[9][2]=0.2693;
	NucleusFinalState		[9][2]=5;

	Energygammas				[9][3]=0.000099278;
	Probability					[9][3]=0.0849;
	NucleusFinalState		[9][3]=4;

	Energygammas				[9][4]=0.00010581;
	Probability					[9][4]=0.0051;
	NucleusFinalState		[9][4]=3;

	Energygammas				[9][5]=0.000124914;
	Probability					[9][5]=0.0376;
	NucleusFinalState		[9][5]=2;

	Energygammas				[9][6]=0.0001835;
	Probability					[9][6]=0.0214;
	NucleusFinalState		[9][6]=0;



	Energygammas				[10][1]=0.00006386;
	Probability					[10][1]=0.0778;
	NucleusFinalState		[10][1]=9;

	Energygammas				[10][2]=0.000135664;
	Probability					[10][2]=0.265;
	NucleusFinalState		[10][2]=7;

	Energygammas				[10][3]=0.00014506;
	Probability					[10][3]=0.0192;
	NucleusFinalState		[10][3]=6;

	Energygammas				[10][4]=0.00014594;
	Probability					[10][4]=0.1037;
	NucleusFinalState		[10][4]=5;

	Energygammas				[10][5]=0.000163101;
	Probability					[10][5]=0.5186;
	NucleusFinalState		[10][5]=4;

	Energygammas				[10][6]=0.00016966;
	Probability					[10][6]=0.0045;
	NucleusFinalState		[10][6]=3;

	Energygammas				[10][7]=0.00018876;
	Probability					[10][7]=0.0112;
	NucleusFinalState		[10][7]=2;


	Int_t numGammas;
	Float_t random =gRandom->Rndm();

	if (InicialState==1 || InicialState==2 || InicialState==5 || InicialState==7){numGammas=1;}
	else if (InicialState==3 || InicialState==4){numGammas=2;}
	else if (InicialState==6){numGammas=3;}
	else if (InicialState==8){numGammas=5;}
	else if (InicialState==9){numGammas=6;}
	else if (InicialState==10){numGammas=7;}
	else{
		cout << "-E- EnsarActiniumChainGen_231Th: Incorrect final state!" << endl;
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
void EnsarActiniumChainGen_231Th::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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

ClassImp(EnsarActiniumChainGen_231Th)
