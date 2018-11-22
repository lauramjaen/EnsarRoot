// *********************************************************************
// *****   
// *****             EnsarActiniumChainGen_231Pa source file                                   
// *****   							
// *********************************************************************

#include "EnsarActiniumChainGen_231Pa.h"

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
EnsarActiniumChainGen_231Pa::EnsarActiniumChainGen_231Pa()  :
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
EnsarActiniumChainGen_231Pa::EnsarActiniumChainGen_231Pa(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarActiniumChainGen_231Pa: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarActiniumChainGen_231Pa","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarActiniumChainGen_231Pa::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarActiniumChainGen_231Pa: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarActiniumChainGen_231Pa::~EnsarActiniumChainGen_231Pa() {

  CloseInput();

}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarActiniumChainGen_231Pa::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarActiniumChainGen_231Pa: Input file not open!" << endl;
    return kFALSE;
  }
 
	
	
	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

	Int_t numInicialCases=11;

	Double_t Probability_emmittedParticles [numInicialCases];
	Probability_emmittedParticles [0]=0.11;
	Probability_emmittedParticles [1]=0.025;
	Probability_emmittedParticles [2]=0.2;
	Probability_emmittedParticles [3]=0.254;
	Probability_emmittedParticles [4]=0.014;
	Probability_emmittedParticles [5]=0.228;
	Probability_emmittedParticles [6]=0.03;
	Probability_emmittedParticles [7]=0.014;
	Probability_emmittedParticles [8]=0.084;
	Probability_emmittedParticles [9]=0.01268;
	Probability_emmittedParticles [10]=0.015;

	Double_t FinalState_daughter [numInicialCases];
	FinalState_daughter [0]=0;
	FinalState_daughter [1]=1;
	FinalState_daughter [2]=2;
	FinalState_daughter [3]=3;
	FinalState_daughter [4]=4;
	FinalState_daughter [5]=6;
	FinalState_daughter [6]=8;
	FinalState_daughter [7]=7;
	FinalState_daughter [8]=11;
	FinalState_daughter [9]=12;
	FinalState_daughter [10]=13;
			
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
	else if (ran_ini>limits[j+6] && ran_ini<limits[j+7]){
			Case=6;			
			//cout<<"Limits "<<limits[j+6]<<" - "<<limits[j+7]<<endl;
	}
	else if (ran_ini>limits[j+7] && ran_ini<limits[j+8]){
			Case=7;			
			//cout<<"Limits "<<limits[j+7]<<" - "<<limits[j+8]<<endl;
	}
	else if (ran_ini>limits[j+8] && ran_ini<limits[j+9]){
			Case=8;			
			//cout<<"Limits "<<limits[j+8]<<" - "<<limits[j+9]<<endl;
	}
	else if (ran_ini>limits[j+9] && ran_ini<limits[j+10]){
			Case=9;			
			//cout<<"Limits "<<limits[j+9]<<" - "<<limits[j+10]<<endl;
	}
	else if (ran_ini>limits[j+10] && ran_ini<limits[j+11]){
			Case=10;			
			//cout<<"Limits "<<limits[j+10]<<" - "<<limits[j+11]<<endl;
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
void EnsarActiniumChainGen_231Pa::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarActiniumChainGen_231Pa: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarActiniumChainGen_231Pa::ReadParameters() {

}

// --- De-excitation Process --------------------------------------------
TArrayF* EnsarActiniumChainGen_231Pa::Deexcitation(Int_t InicialState, Int_t & FinalState) {
	
	// --- NUCLEAR DATA, deexcitation --- //
	Double_t Probability	[50][50];//[Nucleus inicial state][number of gammas]
	Double_t Energygammas	[50][50];
	Int_t 	 NucleusFinalState		[50][50];

	//Excited states
	Energygammas				[1][1]=0.00002736;
	Probability					[1][1]=1.;
	NucleusFinalState		[1][1]=0;


	Energygammas				[2][1]=0.00002997;
	Probability					[2][1]=1.;
	NucleusFinalState		[2][1]=0;



	Energygammas				[3][1]=0.0000165;
	Probability					[3][1]=0.2938;
	NucleusFinalState		[3][1]=2;

	Energygammas				[3][2]=0.0000196;
	Probability					[3][2]=0.4593;
	NucleusFinalState		[3][2]=1;

	Energygammas				[3][3]=0.00004635;
	Probability					[3][3]=0.2469;
	NucleusFinalState		[3][3]=0;



	Energygammas				[4][1]=0.00004414;
	Probability					[4][1]=0.7133;
	NucleusFinalState		[4][1]=2;

	Energygammas				[4][2]=0.00007415;
	Probability					[4][2]=0.2867;
	NucleusFinalState		[4][2]=0;



	Energygammas				[5][1]=0.0000382;
	Probability					[5][1]=0.5862;
	NucleusFinalState		[5][1]=3;

	Energygammas				[5][2]=0.0000546;
	Probability					[5][2]=0.2814;
	NucleusFinalState		[5][2]=2;

	Energygammas				[5][3]=0.00005719;
	Probability					[5][3]=0.1325;
	NucleusFinalState		[5][3]=1;



	Energygammas				[6][1]=0.00002548;
	Probability					[6][1]=0.6618;
	NucleusFinalState		[6][1]=5;

	Energygammas				[6][2]=0.00003583;
	Probability					[6][2]=0.09;
	NucleusFinalState		[6][2]=4;

	Energygammas				[6][3]=0.00006364;
	Probability					[6][3]=0.2482;
	NucleusFinalState		[6][3]=3;



	Energygammas				[7][1]=0.0000236;
	Probability					[7][1]=0.1667;
	NucleusFinalState		[7][1]=9;

	Energygammas				[7][2]=0.00010085;
	Probability					[7][2]=0.8333;
	NucleusFinalState		[7][2]=6;



	Energygammas				[8][1]=0.00005271;
	Probability					[8][1]=0.4792;
	NucleusFinalState		[8][1]=4;

	Energygammas				[8][2]=0.00009684;
	Probability					[8][2]=0.5208;
	NucleusFinalState		[8][2]=2;



	Energygammas				[9][1]=0.0000605;
	Probability					[9][1]=0.0696;
	NucleusFinalState		[9][1]=8;

	Energygammas				[9][2]=0.00007734;
	Probability					[9][2]=0.7564;
	NucleusFinalState		[9][2]=6;

	Energygammas				[9][3]=0.0001026;
	Probability					[9][3]=0.174;
	NucleusFinalState		[9][3]=5;



	

	Energygammas				[10][1]=0.00019889;
	Probability					[10][1]=0.0387;
	NucleusFinalState		[10][1]=4;

	Energygammas				[10][2]=0.0002266;
	Probability					[10][2]=0.0165;
	NucleusFinalState		[10][2]=3;

	Energygammas				[10][3]=0.00024311;
	Probability					[10][3]=0.3079;
	NucleusFinalState		[10][3]=2;

	Energygammas				[10][4]=0.00024604;
	Probability					[10][4]=0.1062;
	NucleusFinalState		[10][4]=1;

	Energygammas				[10][5]=0.00027315;
	Probability					[10][5]=0.5308;
	NucleusFinalState		[10][5]=0;



	Energygammas				[11][1]=0.00005719;
	Probability					[11][1]=0.0015;
	NucleusFinalState		[11][1]=10;

	Energygammas				[11][2]=0.0002456;
	Probability					[11][2]=0.0009;
	NucleusFinalState		[11][2]=5;

	Energygammas				[11][3]=0.00025578;
	Probability					[11][3]=0.0135;
	NucleusFinalState		[11][3]=4;

	Energygammas				[11][4]=0.000283682;
	Probability					[11][4]=0.211;
	NucleusFinalState		[11][4]=3;

	Energygammas				[11][5]=0.000300066;
	Probability					[11][5]=0.3083;
	NucleusFinalState		[11][5]=2;

	Energygammas				[11][6]=0.000302667;
	Probability					[11][6]=0.2912;
	NucleusFinalState		[11][6]=1;

	Energygammas				[11][7]=0.000330055;
	Probability					[11][7]=0.1737;
	NucleusFinalState		[11][7]=0;



	Energygammas				[12][1]=0.0000245;
	Probability					[12][1]=0.0358;
	NucleusFinalState		[12][1]=11;

	Energygammas				[12][2]=0.00032714;
	Probability					[12][2]=0.2625;
	NucleusFinalState		[12][2]=1;

	Energygammas				[12][3]=0.00035448;
	Probability					[12][3]=0.7018;
	NucleusFinalState		[12][3]=0;



	Energygammas				[13][1]=0.00005719;
	Probability					[13][1]=0.0032;
	NucleusFinalState		[13][1]=11;

	Energygammas				[13][2]=0.00026019;
	Probability					[13][2]=0.2083;
	NucleusFinalState		[13][2]=8;

	Energygammas				[13][3]=0.00027722;
	Probability					[13][3]=0.0779;
	NucleusFinalState		[13][3]=6;

	Energygammas				[13][4]=0.000302667;
	Probability					[13][4]=0.2;
	NucleusFinalState		[13][4]=5;

	Energygammas				[13][5]=0.00031292;
	Probability					[13][5]=0.1152;
	NucleusFinalState		[13][5]=4;

	Energygammas				[13][6]=0.00034071;
	Probability					[13][6]=0.2029;
	NucleusFinalState		[13][6]=3;

	Energygammas				[13][7]=0.00035711;
	Probability					[13][7]=0.1923;
	NucleusFinalState		[13][7]=2;

	Energygammas				[13][8]=0.000387;
	Probability					[13][8]=0.0003;
	NucleusFinalState		[13][8]=0;



	Int_t numGammas;
	Float_t random =gRandom->Rndm();

	if (InicialState==1 || InicialState==2){numGammas=1;}
	else if (InicialState==4 || InicialState==7 || InicialState==8){numGammas=2;}
	else if (InicialState==3 || InicialState==5 || InicialState==6 || InicialState==9 || InicialState==12){numGammas=3;}
	else if (InicialState==10){numGammas=5;}
	else if (InicialState==11){numGammas=7;}
	else if (InicialState==13){numGammas=8;}
	else{
		cout << "-E- EnsarActiniumChainGen_231Pa: Incorrect final state!" << endl;
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
void EnsarActiniumChainGen_231Pa::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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

ClassImp(EnsarActiniumChainGen_231Pa)
