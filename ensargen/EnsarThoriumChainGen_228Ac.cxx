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



	Energygammas				[4][1]=0.000191349;
	Probability					[4][1]=1.;
	NucleusFinalState		[4][1]=2;



	Energygammas				[5][1]=0.00033832;
	Probability					[5][1]=0.74488;
	NucleusFinalState		[5][1]=1;

	Energygammas				[5][2]=0.000209253;
	Probability					[5][2]=0.25475;
	NucleusFinalState		[5][2]=2;

	Energygammas				[5][3]=0.00006808;
	Probability					[5][3]=0.00037;
	NucleusFinalState		[5][3]=3;




	Energygammas				[6][1]=0.00014101;
	Probability					[6][1]=0.09008;
	NucleusFinalState		[6][1]=4;

	Energygammas				[6][2]=0.00033237;
	Probability					[6][2]=0.90992;
	NucleusFinalState		[6][2]=2;




	Energygammas				[9][1]=0.000503823;
	Probability					[9][1]=0.75472;
	NucleusFinalState		[9][1]=3;

	Energygammas				[9][2]=0.00077405;
	Probability					[9][2]=0.24151;
	NucleusFinalState		[9][2]=1;

	Energygammas				[9][3]=0.000831;
	Probability					[9][3]=0.00377;
	NucleusFinalState		[9][3]=0;




	Energygammas				[10][1]=0.0004784;
	Probability					[10][1]=0.39952;
	NucleusFinalState		[10][1]=5;

	Energygammas				[10][2]=0.00054645;
	Probability					[10][2]=0.35957;
	NucleusFinalState		[10][2]=3;

	Energygammas				[10][3]=0.0006881;
	Probability					[10][3]=0.09988;
	NucleusFinalState		[10][3]=2;

	Energygammas				[10][4]=0.00081662;
	Probability					[10][4]=0.04914;
	NucleusFinalState		[10][4]=1;

	Energygammas				[10][5]=0.00087445;
	Probability					[10][5]=0.09189;
	NucleusFinalState		[10][5]=0;




	Energygammas				[14][1]=0.0005478;
	Probability					[14][1]=0.06859;
	NucleusFinalState		[14][1]=5;

	Energygammas				[14][2]=0.00061621;
	Probability					[14][2]=0.45725;
	NucleusFinalState		[14][2]=3;

	Energygammas				[14][3]=0.00088644;
	Probability					[14][3]=0.01692;
	NucleusFinalState		[14][3]=1;

	Energygammas				[14][4]=0.000944196;
	Probability					[14][4]=0.45725;
	NucleusFinalState		[14][4]=0;





	Energygammas				[15][1]=0.00057223;
	Probability					[15][1]=0.42194;
	NucleusFinalState		[15][1]=5;

	Energygammas				[15][2]=0.00064034;
	Probability					[15][2]=0.16456;
	NucleusFinalState		[15][2]=3;

	Energygammas				[15][3]=0.0009106;
	Probability					[15][3]=0.4135;
	NucleusFinalState		[15][3]=1;




	Energygammas				[16][1]=0.00044923;
	Probability					[16][1]=0.26545;
	NucleusFinalState		[16][1]=6;

	Energygammas				[16][2]=0.0005723;
	Probability					[16][2]=0.45767;
	NucleusFinalState		[16][2]=5;

	Energygammas				[16][3]=0.0005901;
	Probability					[16][3]=0.00686;
	NucleusFinalState		[16][3]=4;

	Energygammas				[16][4]=0.0007819;
	Probability					[16][4]=0.0595;
	NucleusFinalState		[16][4]=2;

	Energygammas				[16][5]=0.0009107;
	Probability					[16][5]=0.21053;
	NucleusFinalState		[16][5]=1;





	Energygammas				[17][1]=0.0007821405;
	Probability					[17][1]=0.01128;
	NucleusFinalState		[17][1]=2;

	Energygammas				[17][2]=0.0009112044;
	Probability					[17][2]=0.61297;
	NucleusFinalState		[17][2]=1;

	Energygammas				[17][3]=0.00096897417;
	Probability					[17][3]=0.37575;
	NucleusFinalState		[17][3]=0;





	Energygammas				[18][1]=0.00058341;
	Probability					[18][1]=0.34459;
	NucleusFinalState		[18][1]=5;

	Energygammas				[18][2]=0.00065148;
	Probability					[18][2]=0.27912;
	NucleusFinalState		[18][2]=3;

	Energygammas				[18][3]=0.00079274;
	Probability					[18][3]=0.25155;
	NucleusFinalState		[18][3]=2;

	Energygammas				[18][4]=0.00092194;
	Probability					[18][4]=0.04549;
	NucleusFinalState		[18][4]=1;

	Energygammas				[18][5]=0.00097946;
	Probability					[18][5]=0.07926;
	NucleusFinalState		[18][5]=0;





	Energygammas				[19][1]=0.000497;
	Probability					[19][1]=0.16129;
	NucleusFinalState		[19][1]=6;

	Energygammas				[19][2]=0.00062033;
	Probability					[19][2]=0.12724;
	NucleusFinalState		[19][2]=5;

	Energygammas				[19][3]=0.00068811;
	Probability					[19][3]=0.10753;
	NucleusFinalState		[19][3]=3;

	Energygammas				[19][4]=0.00082955;
	Probability					[19][4]=0.12545;
	NucleusFinalState		[19][4]=2;

	Energygammas				[19][5]=0.00095862;
	Probability					[19][5]=0.44803;
	NucleusFinalState		[19][5]=1;

	Energygammas				[19][6]=0.00101644;
	Probability					[19][6]=0.03047;
	NucleusFinalState		[19][6]=0;




	Energygammas				[20][1]=0.000835701;
	Probability					[20][1]=0.24869;
	NucleusFinalState		[20][1]=2;

	Energygammas				[20][2]=0.000964777;
	Probability					[20][2]=0.75131;
	NucleusFinalState		[20][2]=1;



	Energygammas				[21][1]=0.00054068;
	Probability					[21][1]=0.23673;
	NucleusFinalState		[21][1]=6;

	Energygammas				[21][2]=0.00066388;
	Probability					[21][2]=0.3551;
	NucleusFinalState		[21][2]=5;

	Energygammas				[21][3]=0.00087311;
	Probability					[21][3]=0.40816;
	NucleusFinalState		[21][3]=2;




	Energygammas				[22][1]=0.0005555;
	Probability					[22][1]=0.37175;
	NucleusFinalState		[22][1]=6;

	Energygammas				[22][2]=0.0006786;
	Probability					[22][2]=0.33457;
	NucleusFinalState		[22][2]=5;

	Energygammas				[22][3]=0.0006971;
	Probability					[22][3]=0.05948;
	NucleusFinalState		[22][3]=4;

	Energygammas				[22][4]=0.0008879;
	Probability					[22][4]=0.09665;
	NucleusFinalState		[22][4]=2;

	Energygammas				[22][5]=0.001017;
	Probability					[22][5]=0.13755;
	NucleusFinalState		[22][5]=1;





	Energygammas				[23][1]=0.0005718;
	Probability					[23][1]=0.01718;
	NucleusFinalState		[23][1]=6;

	Energygammas				[23][2]=0.0006948;
	Probability					[23][2]=0.02166;
	NucleusFinalState		[23][2]=5;

	Energygammas				[23][3]=0.0007131;
	Probability					[23][3]=0.01344;
	NucleusFinalState		[23][3]=4;

	Energygammas				[23][4]=0.00090419;
	Probability					[23][4]=0.74683;
	NucleusFinalState		[23][4]=2;

	Energygammas				[23][5]=0.00103325;
	Probability					[23][5]=0.2009;
	NucleusFinalState		[23][5]=1;




	Energygammas				[26][1]=0.00010041;
	Probability					[26][1]=0.01598;
	NucleusFinalState		[26][1]=20;

	Energygammas				[26][2]=0.000153962;
	Probability					[26][2]=0.12206;
	NucleusFinalState		[26][2]=17;

	Energygammas				[26][3]=0.0001787;
	Probability					[26][3]=0.01453;
	NucleusFinalState		[26][3]=14;

	Energygammas				[26][4]=0.000726864;
	Probability					[26][4]=0.09881;
	NucleusFinalState		[26][4]=5;

	Energygammas				[26][5]=0.000794948;
	Probability					[26][5]=0.72653;
	NucleusFinalState		[26][5]=3;

	Energygammas				[26][6]=0.00106519;
	Probability					[26][6]=0.02209;
	NucleusFinalState		[26][6]=1;

	

	Energygammas				[28][1]=0.000173964;
	Probability					[28][1]=0.04648;
	NucleusFinalState		[28][1]=18;

	Energygammas				[28][2]=0.00018454;
	Probability					[28][2]=0.09295;
	NucleusFinalState		[28][2]=17;

	Energygammas				[28][3]=0.0002787;
	Probability					[28][3]=0.21289;
	NucleusFinalState		[28][3]=10;

	Energygammas				[28][4]=0.000321646;
	Probability					[28][4]=0.29985;
	NucleusFinalState		[28][4]=9;

	Energygammas				[28][5]=0.001095679;
	Probability					[28][5]=0.16492;
	NucleusFinalState		[28][5]=1;

	Energygammas				[28][6]=0.00115352;
	Probability					[28][6]=0.18291;
	NucleusFinalState		[28][6]=0;



	Energygammas				[30][1]=0.00007734;
	Probability					[30][1]=0.0075;
	NucleusFinalState		[30][1]=23;

	Energygammas				[30][2]=0.00014584;
	Probability					[30][2]=0.04588;
	NucleusFinalState		[30][2]=20;

	Energygammas				[30][3]=0.00019941;
	Probability					[30][3]=0.09189;
	NucleusFinalState		[30][3]=17;

	Energygammas				[30][4]=0.0001998;
	Probability					[30][4]=0.00309;
	NucleusFinalState		[30][4]=16;

	Energygammas				[30][5]=0.000224;
	Probability					[30][5]=0.03;
	NucleusFinalState		[30][5]=14;

	Energygammas				[30][6]=0.00064903;
	Probability					[30][6]=0.01037;
	NucleusFinalState		[30][6]=6;

	Energygammas				[30][7]=0.00077229;
	Probability					[30][7]=0.44115;
	NucleusFinalState		[30][7]=5;

	Energygammas				[30][8]=0.00084038;
	Probability					[30][8]=0.27351;
	NucleusFinalState		[30][8]=3;

	Energygammas				[30][9]=0.0009815;
	Probability					[30][9]=0.01323;
	NucleusFinalState		[30][9]=2;

	Energygammas				[30][10]=0.00111061;
	Probability					[30][10]=0.08338;
	NucleusFinalState		[30][10]=1;




	Energygammas				[31][1]=0.0007962;
	Probability					[31][1]=0.32432;
	NucleusFinalState		[31][1]=4;

	Energygammas				[31][2]=0.0009877;
	Probability					[31][2]=0.67568;
	NucleusFinalState		[31][2]=2;




	Energygammas				[32][1]=0.00023142;
	Probability					[32][1]=0.01111;
	NucleusFinalState		[32][1]=14;

	Energygammas				[32][2]=0.0007795;
	Probability					[32][2]=0.03333;
	NucleusFinalState		[32][2]=5;

	Energygammas				[32][3]=0.0008471;
	Probability					[32][3]=0.02778;
	NucleusFinalState		[32][3]=3;

	Energygammas				[32][4]=0.00098845;
	Probability					[32][4]=0.55556;
	NucleusFinalState		[32][4]=2;

	Energygammas				[32][5]=0.00111756;
	Probability					[32][5]=0.24444;
	NucleusFinalState		[32][5]=1;

	Energygammas				[32][6]=0.00117533;
	Probability					[32][6]=0.12778;
	NucleusFinalState		[32][6]=0;




	Energygammas				[34][1]=0.00017814;
	Probability					[34][1]=0.05031;
	NucleusFinalState		[34][1]=20;

	Energygammas				[34][2]=0.0002315;
	Probability					[34][2]=0.22642;
	NucleusFinalState		[34][2]=17;

	Energygammas				[34][3]=0.0003261;
	Probability					[34][3]=0.62893;
	NucleusFinalState		[34][3]=10;

	Energygammas				[34][4]=0.00101354;
	Probability					[34][4]=0.06918;
	NucleusFinalState		[34][4]=2;

	Energygammas				[34][5]=0.00114278;
	Probability					[34][5]=0.02516;
	NucleusFinalState		[34][5]=1;

	


	Energygammas				[35][1]=0.00013551;
	Probability					[35][1]=0.01999;
	NucleusFinalState		[35][1]=23;

	Energygammas				[35][2]=0.000204031;
	Probability					[35][2]=0.12346;
	NucleusFinalState		[35][2]=20;

	Energygammas				[35][3]=0.0002581;
	Probability					[35][3]=0.0241;
	NucleusFinalState		[35][3]=16;

	Energygammas				[35][4]=0.0002581;
	Probability					[35][4]=0.0241;
	NucleusFinalState		[35][4]=15;

	Energygammas				[35][5]=0.0007074;
	Probability					[35][5]=0.16167;
	NucleusFinalState		[35][5]=6;

	Energygammas				[35][6]=0.000830486;
	Probability					[35][6]=0.58789;
	NucleusFinalState		[35][6]=5;

	Energygammas				[35][7]=0.00103984;
	Probability					[35][7]=0.05879;
	NucleusFinalState		[35][7]=2;




	Energygammas				[38][1]=0.0008918;
	Probability					[38][1]=1.;
	NucleusFinalState		[38][1]=4;




	Energygammas				[41][1]=0.0002063;
	Probability					[41][1]=0.34843;
	NucleusFinalState		[41][1]=23;

	Energygammas				[41][2]=0.000602;
	Probability					[41][2]=0.13937;
	NucleusFinalState		[41][2]=8;

	Energygammas				[41][3]=0.0007781;
	Probability					[41][3]=0.18815;
	NucleusFinalState		[41][3]=6;

	Energygammas				[41][4]=0.00090126;
	Probability					[41][4]=0.15679;
	NucleusFinalState		[41][4]=5;

	Energygammas				[41][5]=0.00111061;
	Probability					[41][5]=0.16725;
	NucleusFinalState		[41][5]=2;



	Energygammas				[43][1]=0.00016862;
	Probability					[43][1]=0.04355;
	NucleusFinalState		[43][1]=32;

	Energygammas				[43][2]=0.00082494;
	Probability					[43][2]=0.20906;
	NucleusFinalState		[43][2]=6;

	Energygammas				[46][3]=0.00094798;
	Probability					[46][3]=0.43554;
	NucleusFinalState		[46][3]=5;

	Energygammas				[43][4]=0.00101644;
	Probability					[43][4]=0.0784;
	NucleusFinalState		[43][4]=3;

	Energygammas				[43][5]=0.00115714;
	Probability					[43][5]=0.02875;
	NucleusFinalState		[43][5]=2;

	Energygammas				[43][6]=0.00128627;
	Probability					[43][6]=0.2047;
	NucleusFinalState		[43][6]=1;





	Energygammas				[46][1]=0.0003998;
	Probability					[46][1]=0.18672;
	NucleusFinalState		[46][1]=19;

	Energygammas				[46][2]=0.0004478;
	Probability					[46][2]=0.06846;
	NucleusFinalState		[46][2]=16;

	Energygammas				[46][3]=0.00047177;
	Probability					[46][3]=0.20747;
	NucleusFinalState		[46][3]=14;

	Energygammas				[46][4]=0.00101986;
	Probability					[46][4]=0.13278;
	NucleusFinalState		[46][4]=5;

	Energygammas				[46][5]=0.00108818;
	Probability					[46][5]=0.03734;
	NucleusFinalState		[46][5]=3;

	Energygammas				[46][6]=0.0012294;
	Probability					[46][6]=0.18672;
	NucleusFinalState		[46][6]=2;

	Energygammas				[46][7]=0.0013583;
	Probability					[46][7]=0.1805;
	NucleusFinalState		[46][7]=1;



	Energygammas				[49][1]=0.0001349;
	Probability					[49][1]=0.00503;
	NucleusFinalState		[49][1]=41;

	Energygammas				[49][2]=0.0001616;
	Probability					[49][2]=0.00204;
	NucleusFinalState		[49][2]=38;

	Energygammas				[49][3]=0.0002314;
	Probability					[49][3]=0.02092;
	NucleusFinalState		[49][3]=34;

	Energygammas				[49][4]=0.00025749;
	Probability					[49][4]=0.00362;
	NucleusFinalState		[49][4]=31;

	Energygammas				[49][5]=0.00026362;
	Probability					[49][5]=0.00543;
	NucleusFinalState		[49][5]=30;

	Energygammas				[49][6]=0.0002787;
	Probability					[49][6]=0.00402;
	NucleusFinalState		[49][6]=28;

	Energygammas				[49][7]=0.00034098;
	Probability					[49][7]=0.04976;
	NucleusFinalState		[49][7]=23;

	Energygammas				[49][8]=0.0003571;
	Probability					[49][8]=0.00933;
	NucleusFinalState		[49][8]=22;

	Energygammas				[49][9]=0.0003722;
	Probability					[49][9]=0.00226;
	NucleusFinalState		[49][9]=21;

	Energygammas				[49][10]=0.000409461;
	Probability					[49][10]=0.24713;
	NucleusFinalState		[49][10]=20;

	Energygammas				[49][11]=0.0004156;
	Probability					[49][11]=0.01188;
	NucleusFinalState		[49][11]=19;

	Energygammas				[49][12]=0.00045251;
	Probability					[49][12]=0.00254;
	NucleusFinalState		[49][12]=18;

	Energygammas				[49][13]=0.000463005;
	Probability					[49][13]=0.56551;
	NucleusFinalState		[49][13]=17;

	Energygammas				[49][14]=0.0004633;
	Probability					[49][14]=0.04015;
	NucleusFinalState		[49][14]=16;

	Energygammas				[49][15]=0.0005574;
	Probability					[49][15]=0.01414;
	NucleusFinalState		[49][15]=10;

	Energygammas				[49][16]=0.0010538;
	Probability					[49][16]=0.0000113;
	NucleusFinalState		[49][16]=4;

	Energygammas				[49][17]=0.00110341;
	Probability					[49][17]=0.00192;
	NucleusFinalState		[49][17]=3;

	Energygammas				[49][18]=0.00124516;
	Probability					[49][18]=0.0125;
	NucleusFinalState		[49][18]=2;

	Energygammas				[49][19]=0.00137424;
	Probability					[49][19]=0.00181;
	NucleusFinalState		[49][19]=1;





	Energygammas				[60][1]=0.000099509;
	Probability					[60][1]=0.44356;
	NucleusFinalState		[60][1]=49;

	Energygammas				[60][2]=0.00035694;
	Probability					[60][2]=0.00599;
	NucleusFinalState		[60][2]=31;

	Energygammas				[60][3]=0.00037799;
	Probability					[60][3]=0.00887;
	NucleusFinalState		[60][3]=28;

	Energygammas				[60][4]=0.00044044;
	Probability					[60][4]=0.04258;
	NucleusFinalState		[60][4]=23;

	Energygammas				[60][5]=0.00050897;
	Probability					[60][5]=0.17742;
	NucleusFinalState		[60][5]=20;

	Energygammas				[60][6]=0.0005625;
	Probability					[60][6]=0.31493;
	NucleusFinalState		[60][6]=17;

	Energygammas				[60][7]=0.00113524;
	Probability					[60][7]=0.00355;
	NucleusFinalState		[60][7]=5;

	Energygammas				[60][8]=0.00134459;
	Probability					[60][8]=0.0031;
	NucleusFinalState		[60][8]=2;



	Energygammas				[70][1]=0.0004702;
	Probability					[70][1]=0.01137;
	NucleusFinalState		[70][1]=30;

	Energygammas				[70][2]=0.00051512;
	Probability					[70][2]=0.04943;
	NucleusFinalState		[70][2]=26;

	Energygammas				[70][3]=0.00130971;
	Probability					[70][3]=0.01582;
	NucleusFinalState		[70][3]=3;

	Energygammas				[70][4]=0.0014514;
	Probability					[70][4]=0.0089;
	NucleusFinalState		[70][4]=2;

	Energygammas				[70][5]=0.00158053;
	Probability					[70][5]=0.49432;
	NucleusFinalState		[70][5]=1;

	Energygammas				[70][6]=0.00163828;
	Probability					[70][6]=0.42017;
	NucleusFinalState		[70][6]=0;



	Energygammas				[71][1]=0.000299;
	Probability					[71][1]=0.07176;
	NucleusFinalState		[71][1]=43;

	Energygammas				[71][2]=0.0004165;
	Probability					[71][2]=0.14801;
	NucleusFinalState		[71][2]=35;

	Energygammas				[71][3]=0.00047475;
	Probability					[71][3]=0.02063;
	NucleusFinalState		[71][3]=30;

	Energygammas				[71][4]=0.000520152;
	Probability					[71][4]=0.05606;
	NucleusFinalState		[71][4]=26;

	Energygammas				[71][5]=0.0005832;
	Probability					[71][5]=0.06907;
	NucleusFinalState		[71][5]=21;

	Energygammas				[71][6]=0.00062681;
	Probability					[71][6]=0.01256;
	NucleusFinalState		[71][6]=19;

	Energygammas				[71][7]=0.00067416;
	Probability					[71][7]=0.09419;
	NucleusFinalState		[71][7]=17;

	Energygammas				[71][8]=0.0006747;
	Probability					[71][8]=0.03454;
	NucleusFinalState		[71][8]=15;

	Energygammas				[71][9]=0.00069896;
	Probability					[71][9]=0.03364;
	NucleusFinalState		[71][9]=14;

	Energygammas				[71][10]=0.00124708;
	Probability					[71][10]=0.44851;
	NucleusFinalState		[71][10]=5;

	Energygammas				[71][11]=0.00131531;
	Probability					[71][11]=0.01103;
	NucleusFinalState		[71][11]=3;




	Energygammas				[73][1]=0.00011454;
	Probability					[73][1]=0.0016;
	NucleusFinalState		[73][1]=60;

	Energygammas				[73][2]=0.0002299;
	Probability					[73][2]=0.02455;
	NucleusFinalState		[73][2]=46;

	Energygammas				[73][3]=0.0004194;
	Probability					[73][3]=0.00371;
	NucleusFinalState		[73][3]=35;

	Energygammas				[73][4]=0.0004449;
	Probability					[73][4]=0.01542;
	NucleusFinalState		[73][4]=34;

	Energygammas				[73][5]=0.0004706;
	Probability					[73][5]=0.01827;
	NucleusFinalState		[73][5]=32;

	Energygammas				[73][6]=0.0004775;
	Probability					[73][6]=0.05196;
	NucleusFinalState		[73][6]=30;

	Energygammas				[73][7]=0.0004923;
	Probability					[73][7]=0.00422;
	NucleusFinalState		[73][7]=28;

	Energygammas				[73][8]=0.000523132;
	Probability					[73][8]=0.01998;
	NucleusFinalState		[73][8]=26;

	Energygammas				[73][9]=0.0005551;
	Probability					[73][9]=0.00799;
	NucleusFinalState		[73][9]=23;

	Energygammas				[73][10]=0.0005711;
	Probability					[73][10]=0.08564;
	NucleusFinalState		[73][10]=22;

	Energygammas				[73][11]=0.0005862;
	Probability					[73][11]=0.01827;
	NucleusFinalState		[73][11]=21;

	Energygammas				[73][12]=0.00062348;
	Probability					[73][12]=0.00257;
	NucleusFinalState		[73][12]=20;

	Energygammas				[73][13]=0.0006294;
	Probability					[73][13]=0.00708;
	NucleusFinalState		[73][13]=19;

	Energygammas				[73][14]=0.00066647;
	Probability					[73][14]=0.01011;
	NucleusFinalState		[73][14]=18;

	Energygammas				[73][15]=0.00067707;
	Probability					[73][15]=0.01136;
	NucleusFinalState		[73][15]=17;

	Energygammas				[73][16]=0.00124997;
	Probability					[73][16]=0.01102;
	NucleusFinalState		[73][16]=5;

	Energygammas				[73][17]=0.00145914;
	Probability					[73][17]=0.13531;
	NucleusFinalState		[73][17]=2;

	Energygammas				[73][18]=0.00158819;
	Probability					[73][18]=0.57094;
	NucleusFinalState		[73][18]=1;





	Energygammas				[78][1]=0.0006601;
	Probability					[78][1]=0.00263;
	NucleusFinalState		[78][1]=20;

	Energygammas				[78][2]=0.0012863;
	Probability					[78][2]=0.41236;
	NucleusFinalState		[78][2]=5;

	Energygammas				[78][3]=0.00149603;
	Probability					[78][3]=0.45314;
	NucleusFinalState		[78][3]=2;

	Energygammas				[78][4]=0.00162506;
	Probability					[78][4]=0.13187;
	NucleusFinalState		[78][4]=1;





	Energygammas				[80][1]=0.00004246;
	Probability					[80][1]=0.00462;
	NucleusFinalState		[80][1]=73;

	Energygammas				[80][2]=0.000672;
	Probability					[80][2]=0.01286;
	NucleusFinalState		[80][2]=19;

	Energygammas				[80][3]=0.00081377;
	Probability					[80][3]=0.00348;
	NucleusFinalState		[80][3]=10;

	Energygammas				[80][4]=0.00150157;
	Probability					[80][4]=0.22244;
	NucleusFinalState		[80][4]=2;

	Energygammas				[80][5]=0.001630627;
	Probability					[80][5]=0.7566;
	NucleusFinalState		[80][5]=1;





	Energygammas				[84][1]=0.0003082;
	Probability					[84][1]=0.1041;
	NucleusFinalState		[84][1]=46;

	Energygammas				[84][2]=0.0005235;
	Probability					[84][2]=0.11767;
	NucleusFinalState		[84][2]=34;

	Energygammas				[84][3]=0.00054874;
	Probability					[84][3]=0.00996;
	NucleusFinalState		[84][3]=32;

	Energygammas				[84][4]=0.00057088;
	Probability					[84][4]=0.07785;
	NucleusFinalState		[84][4]=28;

	Energygammas				[84][5]=0.000701744;
	Probability					[84][5]=0.08192;
	NucleusFinalState		[84][5]=20;

	Energygammas				[84][6]=0.000755315;
	Probability					[84][6]=0.45259;
	NucleusFinalState		[84][6]=15;

	Energygammas				[84][7]=0.0007802;
	Probability					[84][7]=0.02308;
	NucleusFinalState		[84][7]=14;

	Energygammas				[84][8]=0.0008495;
	Probability					[84][8]=0.02308;
	NucleusFinalState		[84][8]=10;

	Energygammas				[84][9]=0.00153787;
	Probability					[84][9]=0.01901;
	NucleusFinalState		[84][9]=2;

	Energygammas				[84][10]=0.001666522;
	Probability					[84][10]=0.0783;
	NucleusFinalState		[84][10]=1;

	Energygammas				[84][11]=0.0017242;
	Probability					[84][11]=0.01245;
	NucleusFinalState		[84][11]=0;



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
