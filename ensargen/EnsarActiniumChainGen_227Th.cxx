// *********************************************************************
// *****   
// *****             EnsarActiniumChainGen_227Th source file                                   
// *****   							
// *********************************************************************

#include "EnsarActiniumChainGen_227Th.h"

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
EnsarActiniumChainGen_227Th::EnsarActiniumChainGen_227Th()  :
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
EnsarActiniumChainGen_227Th::EnsarActiniumChainGen_227Th(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarActiniumChainGen_227Th: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarActiniumChainGen_227Th","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarActiniumChainGen_227Th::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarActiniumChainGen_227Th: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarActiniumChainGen_227Th::~EnsarActiniumChainGen_227Th() {

  CloseInput();

}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarActiniumChainGen_227Th::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarActiniumChainGen_227Th: Input file not open!" << endl;
    return kFALSE;
  }
 
	
	
	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

	Int_t numInicialCases=12;

	Double_t Probability_emmittedParticles [numInicialCases];
	Probability_emmittedParticles [0]=0.242;
	Probability_emmittedParticles [1]=0.029;
	Probability_emmittedParticles [2]=0.235;
	Probability_emmittedParticles [3]=0.03;
	Probability_emmittedParticles [4]=0.0242;
	Probability_emmittedParticles [5]=0.0127;
	Probability_emmittedParticles [6]=0.204;
	Probability_emmittedParticles [7]=0.0489;
	Probability_emmittedParticles [8]=0.083;
	Probability_emmittedParticles [9]=0.0336;
	Probability_emmittedParticles [10]=0.015;
	Probability_emmittedParticles [11]=0.0206;

	Double_t FinalState_daughter [numInicialCases];
	FinalState_daughter [0]=0;
	FinalState_daughter [1]=1;
	FinalState_daughter [2]=3;
	FinalState_daughter [3]=4;
	FinalState_daughter [4]=8;
	FinalState_daughter [5]=9;
	FinalState_daughter [6]=11;
	FinalState_daughter [7]=12;
	FinalState_daughter [8]=13;
	FinalState_daughter [9]=14;
	FinalState_daughter [10]=16;
	FinalState_daughter [11]=17;
			
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
	else if (ran_ini>limits[j+11] && ran_ini<limits[j+12]){
			Case=11;			
			//cout<<"Limits "<<limits[j+11]<<" - "<<limits[j+12]<<endl;
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
void EnsarActiniumChainGen_227Th::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarActiniumChainGen_227Th: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarActiniumChainGen_227Th::ReadParameters() {

}

// --- De-excitation Process --------------------------------------------
TArrayF* EnsarActiniumChainGen_227Th::Deexcitation(Int_t InicialState, Int_t & FinalState) {
	
	// --- NUCLEAR DATA, deexcitation --- //
	Double_t Probability	[50][50];//[Nucleus inicial state][number of gammas]
	Double_t Energygammas	[50][50];
	Int_t 	 NucleusFinalState		[50][50];

	//Excited states
	Energygammas				[1][1]=0.00002986;
	Probability					[1][1]=1.;
	NucleusFinalState		[1][1]=0;


	Energygammas				[2][1]=0.00002025;
	Probability					[2][1]=0.0272;
	NucleusFinalState		[2][1]=1;

	Energygammas				[2][2]=0.00005013;
	Probability					[2][2]=0.9728;
	NucleusFinalState		[2][2]=0;



	Energygammas				[3][1]=0.00003158;
	Probability					[3][1]=0.4318;
	NucleusFinalState		[3][1]=1;

	Energygammas				[3][2]=0.000061441;
	Probability					[3][2]=0.5682;
	NucleusFinalState		[3][2]=0;




	Energygammas				[4][1]=0.0000296;
	Probability					[4][1]=0.0024;
	NucleusFinalState		[4][1]=2;

	Energygammas				[4][2]=0.00004982;
	Probability					[4][2]=0.1832;
	NucleusFinalState		[4][2]=1;

	Energygammas				[4][3]=0.00007969;
	Probability					[4][3]=0.8144;//0.8143 to Prob=1
	NucleusFinalState		[4][3]=0;



	Energygammas				[5][1]=0.0000438;
	Probability					[5][1]=0.6711;
	NucleusFinalState		[5][1]=3;

	Energygammas				[5][2]=0.00007501;
	Probability					[5][2]=0.3289;
	NucleusFinalState		[5][2]=1;



	Energygammas				[6][1]=0.00004422;
	Probability					[6][1]=0.0297;
	NucleusFinalState		[6][1]=4;

	Energygammas				[6][2]=0.00006245;
	Probability					[6][2]=0.1138;
	NucleusFinalState		[6][2]=3;

	Energygammas				[6][3]=0.00007363;
	Probability					[6][3]=0.0076;
	NucleusFinalState		[6][3]=2;

	Energygammas				[6][4]=0.00009388;
	Probability					[6][4]=0.8489;
	NucleusFinalState		[6][4]=1;



	Energygammas				[7][1]=0.0000065;
	Probability					[7][1]=0.0063;
	NucleusFinalState		[7][1]=6;

	Energygammas				[7][2]=0.00006874;
	Probability					[7][2]=0.4051;
	NucleusFinalState		[7][2]=3;

	Energygammas				[7][3]=0.00010027;
	Probability					[7][3]=0.5886;
	NucleusFinalState		[7][3]=1;



	Energygammas				[8][1]=0.0000444;
	Probability					[8][1]=0.0741;
	NucleusFinalState		[8][1]=7;

	Energygammas				[8][2]=0.00011311;
	Probability					[8][2]=0.9259;
	NucleusFinalState		[8][2]=3;



	Energygammas				[9][1]=0.00011105;
	Probability					[9][1]=0.0012;
	NucleusFinalState		[9][1]=6;

	Energygammas				[9][2]=0.0001555;
	Probability					[9][2]=0.0007;
	NucleusFinalState		[9][2]=4;

	Energygammas				[9][3]=0.00017345;
	Probability					[9][3]=0.0263;
	NucleusFinalState		[9][3]=3;

	Energygammas				[9][4]=0.00018465;
	Probability					[9][4]=0.054;
	NucleusFinalState		[9][4]=2;

	Energygammas				[9][5]=0.00020498;
	Probability					[9][5]=0.2429;
	NucleusFinalState		[9][5]=1;

	Energygammas				[9][6]=0.00023476;
	Probability					[9][6]=0.6749;
	NucleusFinalState		[9][6]=0;


	

	Energygammas				[10][1]=0.00015014;
	Probability					[10][1]=0.0644;
	NucleusFinalState		[10][1]=7;

	Energygammas				[10][2]=0.0001758;
	Probability					[10][2]=0.1223;
	NucleusFinalState		[10][2]=5;

	Energygammas				[10][3]=0.0002005;
	Probability					[10][3]=0.0772;
	NucleusFinalState		[10][3]=4;

	Energygammas				[10][4]=0.0002189;
	Probability					[10][4]=0.6435;
	NucleusFinalState		[10][4]=3;

	Energygammas				[10][5]=0.00025015;
	Probability					[10][5]=0.0521;
	NucleusFinalState		[10][5]=1;

	Energygammas				[10][6]=0.0002807;
	Probability					[10][6]=0.0405;
	NucleusFinalState		[10][6]=0;



	Energygammas				[11][1]=0.00023596;
	Probability					[11][1]=0.5959;
	NucleusFinalState		[11][1]=2;

	Energygammas				[11][2]=0.00025623;
	Probability					[11][2]=0.3236;
	NucleusFinalState		[11][2]=1;

	Energygammas				[11][3]=0.00028609;
	Probability					[11][3]=0.0805;
	NucleusFinalState		[11][3]=0;

	

	Energygammas				[12][1]=0.00004377;
	Probability					[12][1]=0.0345;
	NucleusFinalState		[12][1]=11;

	Energygammas				[12][2]=0.00009497;
	Probability					[12][2]=0.0038;
	NucleusFinalState		[12][2]=9;

	Energygammas				[12][3]=0.00020608;
	Probability					[12][3]=0.0412;
	NucleusFinalState		[12][3]=6;

	Energygammas				[12][4]=0.00025027;
	Probability					[12][4]=0.0738;
	NucleusFinalState		[12][4]=4;

	Energygammas				[12][5]=0.0002798;
	Probability					[12][5]=0.0086;
	NucleusFinalState		[12][5]=2;

	Energygammas				[12][6]=0.00029998;
	Probability					[12][6]=0.3592;
	NucleusFinalState		[12][6]=1;

	Energygammas				[12][7]=0.00032985;
	Probability					[12][7]=0.4789;
	NucleusFinalState		[12][7]=0;




	Energygammas				[13][1]=0.0000483;
	Probability					[13][1]=0.0027;
	NucleusFinalState		[13][1]=11;

	Energygammas				[13][2]=0.00005419;
	Probability					[13][2]=0.0013;
	NucleusFinalState		[13][2]=10;

	Energygammas				[13][3]=0.00009958;
	Probability					[13][3]=0.0052;
	NucleusFinalState		[13][3]=9;

	Energygammas				[13][4]=0.00020414;
	Probability					[13][4]=0.0446;
	NucleusFinalState		[13][4]=7;

	Energygammas				[13][5]=0.00021062;
	Probability					[13][5]=0.2465;
	NucleusFinalState		[13][5]=6;

	Energygammas				[13][6]=0.00025463;
	Probability					[13][6]=0.1405;
	NucleusFinalState		[13][6]=4;

	Energygammas				[13][7]=0.00027291;
	Probability					[13][7]=0.1001;
	NucleusFinalState		[13][7]=3;

	Energygammas				[13][8]=0.00028424;
	Probability					[13][8]=0.0079;
	NucleusFinalState		[13][8]=2;

	Energygammas				[13][9]=0.0003045;
	Probability					[13][9]=0.2268;
	NucleusFinalState		[13][9]=1;

	Energygammas				[13][10]=0.00033437;
	Probability					[13][10]=0.2244;//0.2243 to Prob=1
	NucleusFinalState		[13][10]=0;



	Energygammas				[14][1]=0.00005642;
	Probability					[14][1]=0.0075;
	NucleusFinalState		[14][1]=11;

	Energygammas				[14][2]=0.00010776;
	Probability					[14][2]=0.0063;
	NucleusFinalState		[14][2]=9;

	Energygammas				[14][3]=0.00026287;
	Probability					[14][3]=0.0869;
	NucleusFinalState		[14][3]=4;

	Energygammas				[14][4]=0.00028142;
	Probability					[14][4]=0.1441;
	NucleusFinalState		[14][4]=3;

	Energygammas				[14][5]=0.00029241;
	Probability					[14][5]=0.0535;
	NucleusFinalState		[14][5]=2;

	Energygammas				[14][6]=0.00031269;
	Probability					[14][6]=0.4177;
	NucleusFinalState		[14][6]=1;

	Energygammas				[14][7]=0.00034255;
	Probability					[14][7]=0.284;
	NucleusFinalState		[14][7]=0;



	Energygammas				[15][1]=0.00006245;
	Probability					[15][1]=0.3418;
	NucleusFinalState		[15][1]=10;

	Energygammas				[15][2]=0.00010776;
	Probability					[15][2]=0.0134;
	NucleusFinalState		[15][2]=9;

	Energygammas				[15][3]=0.00016836;
	Probability					[15][3]=0.0254;
	NucleusFinalState		[15][3]=8;

	Energygammas				[15][4]=0.0002127;
	Probability					[15][4]=0.1335;
	NucleusFinalState		[15][4]=7;

	Energygammas				[15][5]=0.0002189;
	Probability					[15][5]=0.1842;
	NucleusFinalState		[15][5]=6;

	Energygammas				[15][6]=0.00028142;
	Probability					[15][6]=0.3017;
	NucleusFinalState		[15][6]=3;




	Energygammas				[16][1]=0.00000815;
	Probability					[16][1]=0.0492;
	NucleusFinalState		[16][1]=14;

	Energygammas				[16][2]=0.00006435;
	Probability					[16][2]=0.169;
	NucleusFinalState		[16][2]=11;

	Energygammas				[16][3]=0.0003005;
	Probability					[16][3]=0.0893;
	NucleusFinalState		[16][3]=2;

	Energygammas				[16][4]=0.00035054;
	Probability					[16][4]=0.6925;
	NucleusFinalState		[16][4]=0;




	Energygammas				[17][1]=0.00003339;
	Probability					[17][1]=0.0059;
	NucleusFinalState		[17][1]=15;

	Energygammas				[17][2]=0.00004193;
	Probability					[17][2]=0.0222;
	NucleusFinalState		[17][2]=13;

	Energygammas				[17][3]=0.00009603;
	Probability					[17][3]=0.0519;
	NucleusFinalState		[17][3]=10;

	Energygammas				[17][4]=0.00014142;
	Probability					[17][4]=0.089;
	NucleusFinalState		[17][4]=9;

	Energygammas				[17][5]=0.00020164;
	Probability					[17][5]=0.0178;
	NucleusFinalState		[17][5]=8;

	Energygammas				[17][6]=0.00024612;
	Probability					[17][6]=0.0093;
	NucleusFinalState		[17][6]=7;

	Energygammas				[17][7]=0.0002525;
	Probability					[17][7]=0.0853;
	NucleusFinalState		[17][7]=6;

	Energygammas				[17][8]=0.0002965;
	Probability					[17][8]=0.33;
	NucleusFinalState		[17][8]=4;

	Energygammas				[17][9]=0.00031485;
	Probability					[17][9]=0.3708;
	NucleusFinalState		[17][9]=3;

	Energygammas				[17][10]=0.00032599;
	Probability					[17][10]=0.0048;
	NucleusFinalState		[17][10]=2;

	Energygammas				[17][11]=0.00034645;
	Probability					[17][11]=0.0089;
	NucleusFinalState		[17][11]=1;

	Energygammas				[17][12]=0.00037627;
	Probability					[17][12]=0.0041;
	NucleusFinalState		[17][12]=0;


	Int_t numGammas;
	Float_t random =gRandom->Rndm();
	//cout<<"Inside loop: Random="<<random<<endl;

	if (InicialState==1){numGammas=1;}
	else if (InicialState==2 || InicialState==3 || InicialState==5 || InicialState==8){numGammas=2;}
	else if (InicialState==4 || InicialState==7 || InicialState==11){numGammas=3;}
	else if (InicialState==6 || InicialState==16){numGammas=4;}
	else if (InicialState==9 || InicialState==10 || InicialState==15){numGammas=6;}
	else if (InicialState==12 || InicialState==14){numGammas=7;}
	else if (InicialState==13){numGammas=10;}
	else if (InicialState==17){numGammas=12;}
	else{
		cout << "-E- EnsarActiniumChainGen_227Th: Incorrect final state!" << endl;
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
void EnsarActiniumChainGen_227Th::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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

ClassImp(EnsarActiniumChainGen_227Th)
