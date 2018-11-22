// *********************************************************************
// *****   
// *****             EnsarThoriumChainGen_228Ac source file                                   
// *****   							
// *********************************************************************

#include "EnsarThoriumChainGen_228Ac.h"

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
EnsarThoriumChainGen_228Ac::EnsarThoriumChainGen_228Ac()  :
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
EnsarThoriumChainGen_228Ac::EnsarThoriumChainGen_228Ac(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarThoriumChainGen_228Ac: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarThoriumChainGen_228Ac","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarThoriumChainGen_228Ac::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarThoriumChainGen_228Ac: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarThoriumChainGen_228Ac::~EnsarThoriumChainGen_228Ac() {

  CloseInput();

}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarThoriumChainGen_228Ac::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarThoriumChainGen_228Ac: Input file not open!" << endl;
    return kFALSE;
  }
 
	
	
	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

	Int_t numInicialCases=15;

	Double_t Probability_emmittedParticles [numInicialCases];
	Probability_emmittedParticles [0]=0.0176;
	Probability_emmittedParticles [1]=0.0243;
	Probability_emmittedParticles [2]=0.0112;
	Probability_emmittedParticles [3]=0.0419;
	Probability_emmittedParticles [4]=0.03;
	Probability_emmittedParticles [5]=0.0115;
	Probability_emmittedParticles [6]=0.076;
	Probability_emmittedParticles [7]=0.012;
	Probability_emmittedParticles [8]=0.0311;
	Probability_emmittedParticles [9]=0.058;
	Probability_emmittedParticles [10]=0.059;
	Probability_emmittedParticles [11]=0.0311;
	Probability_emmittedParticles [12]=0.299;
	Probability_emmittedParticles [13]=0.1165;
	Probability_emmittedParticles [14]=0.07;

	Double_t FinalState_daughter [numInicialCases];
	FinalState_daughter [0]=84;
	FinalState_daughter [1]=80;
	FinalState_daughter [2]=78;
	FinalState_daughter [3]=73;
	FinalState_daughter [4]=71;
	FinalState_daughter [5]=70;
	FinalState_daughter [6]=60;
	FinalState_daughter [7]=49;
	FinalState_daughter [8]=30;
	FinalState_daughter [9]=28;
	FinalState_daughter [10]=26;
	FinalState_daughter [11]=20;
	FinalState_daughter [12]=17;
	FinalState_daughter [13]=5;
	FinalState_daughter [14]=1;

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
			//cout<<"HEREEEEE  CASE 3 Limits "<<limits[j+3]<<" - "<<limits[j+4]<<endl;
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
	else if (ran_ini>limits[j+11] && ran_ini<limits[j+12]){
			Case=11;			
			//cout<<"Limits "<<limits[j+11]<<" - "<<limits[j+12]<<endl;
	}
	else if (ran_ini>limits[j+12] && ran_ini<limits[j+13]){
			Case=12;			
			//cout<<"Limits "<<limits[j+12]<<" - "<<limits[j+13]<<endl;
	}
	else if (ran_ini>limits[j+13] && ran_ini<limits[j+14]){
			Case=13;			
			//cout<<"Limits "<<limits[j+13]<<" - "<<limits[j+14]<<endl;
	}
	else if (ran_ini>limits[j+14] && ran_ini<limits[j+15]){
			Case=14;			
			//cout<<"Limits "<<limits[j+14]<<" - "<<limits[j+15]<<endl;
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
void EnsarThoriumChainGen_228Ac::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarThoriumChainGen_228Ac: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarThoriumChainGen_228Ac::ReadParameters() {

}

// --- De-excitation Process --------------------------------------------
TArrayF* EnsarThoriumChainGen_228Ac::Deexcitation(Int_t InicialState, Int_t & FinalState) {
	
	// --- NUCLEAR DATA, deexcitation --- //
	Double_t Probability	[100][100];//[Nucleus inicial state][number of gammas]
	Double_t Energygammas	[100][100];
	Int_t 	 NucleusFinalState	[100][100];

// At this point we only consider the gammas 
// with Intensity higer than 50 (%) for each level
// coloured in blue on the excel

	Energygammas				[1][1]=0.000057766;
	Probability					[1][1]=1.;
	NucleusFinalState		[1][1]=0;


	Energygammas				[2][1]=0.000129065;
	Probability					[2][1]=1.;
	NucleusFinalState		[2][1]=1;


	Energygammas				[3][1]=0.000270245;
	Probability					[3][1]=0.53107;
	NucleusFinalState		[3][1]=1;

	Energygammas				[3][2]=0.000328022;
	Probability					[3][2]=0.46893;
	NucleusFinalState		[3][2]=0;



	Energygammas				[5][1]=0.00033832;
	Probability					[5][1]=1.;//0.74488 to Prob=1
	NucleusFinalState		[5][1]=1;


	Energygammas				[9][1]=0.000503823;
	Probability					[9][1]=1.;//0.75472 to Prob=1
	NucleusFinalState		[9][1]=3;


	Energygammas				[10][1]=0.0004784;
	Probability					[10][1]=0.39952;
	NucleusFinalState		[10][1]=5;

	Energygammas				[10][2]=0.00054645;
	Probability					[10][2]=0.60048;//0.35957 to Prob=1
	NucleusFinalState		[10][2]=3;


	Energygammas				[15][1]=0.00057223;
	Probability					[15][1]=0.42194;
	NucleusFinalState		[15][1]=5;

	Energygammas				[15][2]=0.0009106;
	Probability					[15][2]=0.57806;//0.4135 to Prob=1
	NucleusFinalState		[15][2]=1;


	Energygammas				[17][1]=0.000911204;
	Probability					[17][1]=0.61297;
	NucleusFinalState		[17][1]=1;

	Energygammas				[17][2]=0.000968974;
	Probability					[17][2]=0.38703;//0.37575 to Prob=1
	NucleusFinalState		[17][2]=0;


	Energygammas				[20][1]=0.000964777;
	Probability					[20][1]=1.;//0.75131 to Prob=1
	NucleusFinalState		[20][1]=1;


	Energygammas				[26][1]=0.000794948;
	Probability					[26][1]=1.;//0.72653 to Prob=1
	NucleusFinalState		[26][1]=3;

	
	Energygammas				[28][1]=0.0002787;
	Probability					[28][1]=0.21289;
	NucleusFinalState		[28][1]=10;

	Energygammas				[28][2]=0.000321646;
	Probability					[28][2]=0.29985;
	NucleusFinalState		[28][2]=9;

	Energygammas				[28][3]=0.001095679;
	Probability					[28][3]=0.16492;
	NucleusFinalState		[28][3]=1;

	Energygammas				[28][4]=0.00115352;
	Probability					[28][4]=0.32234;//0.18291 to Prob=1
	NucleusFinalState		[28][4]=0;


	
	Energygammas				[30][1]=0.00077229;
	Probability					[30][1]=0.44115;
	NucleusFinalState		[30][1]=5;

	Energygammas				[30][2]=0.00084038;
	Probability					[30][2]=0.55885;//0.27351 to Prob=1
	NucleusFinalState		[30][2]=3;

	
	Energygammas				[49][1]=0.000463005;
	Probability					[49][1]=1.;//0.56551 to Prob=1
	NucleusFinalState		[49][1]=17;

	

	Energygammas				[60][1]=0.000099509;
	Probability					[60][1]=0.44356;
	NucleusFinalState		[60][1]=49;

	Energygammas				[60][2]=0.0005625;
	Probability					[60][2]=0.55644;//0.31493 to Prob=1
	NucleusFinalState		[60][2]=17;


	

	Energygammas				[70][1]=0.00158053;
	Probability					[70][1]=0.49432;
	NucleusFinalState		[70][1]=1;

	Energygammas				[70][2]=0.00163828;
	Probability					[70][2]=0.50568;//0.42017 to Prob=1
	NucleusFinalState		[70][2]=0;


	Energygammas				[71][1]=0.00124708;
	Probability					[71][1]=1.;//0.44851 to Prob=1
	NucleusFinalState		[71][1]=5;


	Energygammas				[73][1]=0.00158819;//=0.00158819 Problem¿?
	Probability					[73][1]=1.;//0.57094 to Prob=1
	NucleusFinalState		[73][1]=1;


	Energygammas				[78][1]=0.0012863;
	Probability					[78][1]=0.41236;
	NucleusFinalState		[78][1]=5;

	Energygammas				[78][2]=0.00149603;
	Probability					[78][2]=0.58764;//0.45314 to Prob=1
	NucleusFinalState		[78][2]=2;


	Energygammas				[80][1]=0.001630627;
	Probability					[80][1]=1.;//0.7566 to Prob=1
	NucleusFinalState		[80][1]=1;


	Energygammas				[84][1]=0.000755315;
	Probability					[84][1]=1.;//0.45259 to Prob=1
	NucleusFinalState		[84][1]=15;

	Int_t numGammas;
	Float_t random =gRandom->Rndm();

	if (InicialState==1 ||InicialState==2 ||InicialState==5 ||InicialState==9 ||InicialState==20 ||InicialState==26 ||InicialState==49 ||InicialState==71 ||InicialState==73 ||InicialState==80 ||InicialState==84){numGammas=1;}
	else if (InicialState==3 ||InicialState==10 ||InicialState==15 ||InicialState==17 ||InicialState==30 ||InicialState==60 ||InicialState==70 ||InicialState==78){numGammas=2;}
	else if (InicialState==28){numGammas=4;}
	else{
		cout << "-E- EnsarThoriumChainGen_228Ac: Incorrect final state!" << endl;
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
void EnsarThoriumChainGen_228Ac::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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

ClassImp(EnsarThoriumChainGen_228Ac)
