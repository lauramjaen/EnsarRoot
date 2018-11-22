// *********************************************************************
// *****   
// *****             EnsarUraniumChainGen_214Bi source file                                   
// *****   							
// *********************************************************************

#include "EnsarUraniumChainGen_214Bi.h"

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
EnsarUraniumChainGen_214Bi::EnsarUraniumChainGen_214Bi()  :
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
EnsarUraniumChainGen_214Bi::EnsarUraniumChainGen_214Bi(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarUraniumChainGen_214Bi: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarUraniumChainGen_214Bi","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarUraniumChainGen_214Bi::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarUraniumChainGen_214Bi: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarUraniumChainGen_214Bi::~EnsarUraniumChainGen_214Bi() {

  CloseInput();

}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarUraniumChainGen_214Bi::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarUraniumChainGen_214Bi: Input file not open!" << endl;
    return kFALSE;
  }
 
	
	
	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

	Int_t numInicialCases=14;

	Double_t Probability_emmittedParticles [numInicialCases];
	Probability_emmittedParticles [0]=0.191;
	Probability_emmittedParticles [1]=0.0735;
	Probability_emmittedParticles [2]=0.0312;
	Probability_emmittedParticles [3]=0.1757;
	Probability_emmittedParticles [4]=0.1696;
	Probability_emmittedParticles [5]=0.0814;
	Probability_emmittedParticles [6]=0.01588;
	Probability_emmittedParticles [7]=0.01177;
	Probability_emmittedParticles [8]=0.01431;
	Probability_emmittedParticles [9]=0.0245;
	Probability_emmittedParticles [10]=0.04345;
	Probability_emmittedParticles [11]=0.056;
	Probability_emmittedParticles [12]=0.0278;
	Probability_emmittedParticles [13]=0.01244;


	Double_t FinalState_daughter [numInicialCases];//final state of 214Bi*
	FinalState_daughter [0]=0;
	FinalState_daughter [1]=4;
	FinalState_daughter [2]=6;
	FinalState_daughter [3]=9;
	FinalState_daughter [4]=11;
	FinalState_daughter [5]=12;
	FinalState_daughter [6]=13;
	FinalState_daughter [7]=14;
	FinalState_daughter [8]=15;
	FinalState_daughter [9]=16;
	FinalState_daughter [10]=18;
	FinalState_daughter [11]=21;
	FinalState_daughter [12]=28;
	FinalState_daughter [13]=29;
	
			
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
	}
	else if (ran_ini>limits[j+1] && ran_ini<limits[j+2]){
			Case=1;		
	}
	else if (ran_ini>limits[j+2] && ran_ini<limits[j+3]){
			Case=2;		
	}
	else if (ran_ini>limits[j+3] && ran_ini<limits[j+4]){
			Case=3;			
	}
	else if (ran_ini>limits[j+4] && ran_ini<limits[j+5]){
			Case=4;			
	}
	else if (ran_ini>limits[j+5] && ran_ini<limits[j+6]){
			Case=5;		
	}
	else if (ran_ini>limits[j+6] && ran_ini<limits[j+7]){
			Case=6;		
	}
	else if (ran_ini>limits[j+7] && ran_ini<limits[j+8]){
			Case=7;			
	}
	else if (ran_ini>limits[j+8] && ran_ini<limits[j+9]){
			Case=8;			
	}
	else if (ran_ini>limits[j+9] && ran_ini<limits[j+10]){
			Case=9;		
	}
	else if (ran_ini>limits[j+10] && ran_ini<limits[j+11]){
			Case=10;		
	}
	else if (ran_ini>limits[j+11] && ran_ini<limits[j+12]){
			Case=11;			
	}
	else if (ran_ini>limits[j+12] && ran_ini<limits[j+13]){
			Case=12;			
	}
	else if (ran_ini>limits[j+13] && ran_ini<limits[j+14]){
			Case=13;			
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
void EnsarUraniumChainGen_214Bi::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarUraniumChainGen_214Bi: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarUraniumChainGen_214Bi::ReadParameters() {

}

// --- De-excitation Process --------------------------------------------
TArrayF* EnsarUraniumChainGen_214Bi::Deexcitation(Int_t InicialState, Int_t & FinalState) {
	
	// --- NUCLEAR DATA, deexcitation of 214Bi --- //
	Double_t Probability	[150][150];//[Nucleus inicial state][number of gammas]
	Double_t Energygammas	[150][150];
	Int_t 	 NucleusFinalState		[150][150];


	//1 excited state
	Probability					[1][1]=1.;
	Energygammas				[1][1]=0.00060932;
	NucleusFinalState		[1][1]=0;

	//2 excited state
	Probability					[2][1]=1.;
	Energygammas				[2][1]=0.00040572;
	NucleusFinalState		[2][1]=1;

	//3 excited state
	Probability					[3][1]=1.;
	Energygammas				[3][1]=0.000665447;
	NucleusFinalState		[3][1]=1;

	//4 excited state
	Probability					[4][1]=0.551;
	Energygammas				[4][1]=0.00076836;
	NucleusFinalState		[4][1]=1;

	Probability					[4][2]=0.449;
	Energygammas				[4][2]=0.001377669;
	NucleusFinalState		[4][2]=0;

	//5 excited state
	Probability					[5][1]=1.;
	Energygammas				[5][1]=0.00080618;
	NucleusFinalState		[5][1]=1;

	//6 excited state
	Probability					[6][1]=0.0048;
	Energygammas				[6][1]=0.0002688;
	NucleusFinalState		[6][1]=3;

	Probability					[6][2]=0.0014;
	Energygammas				[6][2]=0.000528;
	NucleusFinalState		[6][2]=2;

	Probability					[6][3]=0.9051;
	Energygammas				[6][3]=0.000934056;
	NucleusFinalState		[6][3]=1;

	Probability					[6][4]=0.0887;
	Energygammas				[6][4]=0.00154334;
	NucleusFinalState		[6][4]=0;

	//7 excited state
	Probability					[7][1]=0.2302;
	Energygammas				[7][1]=0.00105196;
	NucleusFinalState		[7][1]=1;

	Probability					[7][2]=0.7698;
	Energygammas				[7][2]=0.001661274;
	NucleusFinalState		[7][2]=0;

	//8 excited state
	Probability					[8][1]=0.4083;
	Energygammas				[8][1]=0.00069793;
	NucleusFinalState		[8][1]=2;

	Probability					[8][2]=0.5917;
	Energygammas				[8][2]=0.0011037;
	NucleusFinalState		[8][2]=1;

	//9 excited state
	Probability					[9][1]=0.0041;
	Energygammas				[9][1]=0.0003519;
	NucleusFinalState		[9][1]=4;

	Probability					[9][2]=0.0161;
	Energygammas				[9][2]=0.00045479;
	NucleusFinalState		[9][2]=3;

	Probability					[9][3]=0.8214;
	Energygammas				[9][3]=0.001120294;
	NucleusFinalState		[9][3]=1;

	Probability					[9][4]=0.1584;
	Energygammas				[9][4]=0.001729595;
	NucleusFinalState		[9][4]=0;

	//10 excited state
	Probability					[10][1]=1.;
	Energygammas				[10][1]=0.00113366;
	NucleusFinalState		[10][1]=1;


	//11 excited state
	Probability					[11][1]=0.0002;
	Energygammas				[11][1]=0.000221;
	NucleusFinalState		[11][1]=6;

	Probability					[11][2]=0.006;
	Energygammas				[11][2]=0.00034892;
	NucleusFinalState		[11][2]=5;

	Probability					[11][3]=0.017;
	Energygammas				[11][3]=0.00038678;
	NucleusFinalState		[11][3]=4;

	Probability					[11][4]=0.0943;
	Energygammas				[11][4]=0.00115521;
	NucleusFinalState		[11][4]=1;

	Probability					[11][5]=0.8825;
	Energygammas				[11][5]=0.001764491;
	NucleusFinalState		[11][5]=0;

	//12 excited state
	Probability					[12][1]=0.0047;
	Energygammas				[12][1]=0.0003042;
	NucleusFinalState		[12][1]=6;

	Probability					[12][2]=0.0162;
	Energygammas				[12][2]=0.00046977;
	NucleusFinalState		[12][2]=4;

	Probability					[12][3]=0.0096;
	Energygammas				[12][3]=0.00057278;
	NucleusFinalState		[12][3]=3;

	Probability					[12][4]=0.0034;
	Energygammas				[12][4]=0.00083236;
	NucleusFinalState		[12][4]=2;

	Probability					[12][5]=0.7172;
	Energygammas				[12][5]=0.001238122;
	NucleusFinalState		[12][5]=1;

	Probability					[12][6]=0.2489;
	Energygammas				[12][6]=0.00189032;
	NucleusFinalState		[12][6]=0;

	//13 excited state
	Probability					[13][1]=0.0347;
	Energygammas				[13][1]=0.00061577;
	NucleusFinalState		[13][1]=3;

	Probability					[13][2]=0.9124;
	Energygammas				[13][2]=0.001280976;
	NucleusFinalState		[13][2]=1;

	Probability					[13][3]=0.0529;
	Energygammas				[13][3]=0.00189032;
	NucleusFinalState		[13][3]=0;

	//14 excited state
	Probability					[14][1]=0.0031;
	Energygammas				[14][1]=0.000230;
	NucleusFinalState		[14][1]=11;

	Probability					[14][2]=0.0505;
	Energygammas				[14][2]=0.00033337;
	NucleusFinalState		[14][2]=7;

	Probability					[14][3]=0.0265;
	Energygammas				[14][3]=0.0006171;
	NucleusFinalState		[14][3]=4;

	Probability					[14][4]=0.3042;
	Energygammas				[14][4]=0.00071987;
	NucleusFinalState		[14][4]=3;

	Probability					[14][5]=0.6157;//to Prob=0.6158
	Energygammas				[14][5]=0.00138531;
	NucleusFinalState		[14][5]=1;

	//15 excited state
	Probability					[15][1]=0.012;
	Energygammas				[15][1]=0.00059524;
	NucleusFinalState		[15][1]=5;

	Probability					[15][2]=0.0385;
	Energygammas				[15][2]=0.0006331;
	NucleusFinalState		[15][2]=4;

	Probability					[15][3]=0.9188;
	Energygammas				[15][3]=0.001401515;
	NucleusFinalState		[15][3]=1;

	Probability					[15][4]=0.0307;
	Energygammas				[15][4]=0.00201082;
	NucleusFinalState		[15][4]=0;

	//16 excited state
	Probability					[16][1]=0.0013;
	Energygammas				[16][1]=0.0002528;
	NucleusFinalState		[16][1]=11;

	Probability					[16][2]=0.0028;
	Energygammas				[16][2]=0.000356;
	NucleusFinalState		[16][2]=7;

	Probability					[16][3]=0.0137;
	Energygammas				[16][3]=0.00063962;
	NucleusFinalState		[16][3]=4;

	Probability					[16][4]=0.9822;//to Prob=0.9823
	Energygammas				[16][4]=0.001407988;
	NucleusFinalState		[16][4]=1;

	//17 excited state
	Probability					[17][1]=0.5747;
	Energygammas				[17][1]=0.00071071;
	NucleusFinalState		[17][1]=4;

	Probability					[17][2]=0.4253;
	Energygammas				[17][2]=0.00147917;
	NucleusFinalState		[17][2]=1;

	//18 excited state
	Probability					[18][1]=0.0128;
	Energygammas				[18][1]=0.0004867;
	NucleusFinalState		[18][1]=9;

	Probability					[18][2]=0.0625;
	Energygammas				[18][2]=0.0007697;
	NucleusFinalState		[18][2]=5;

	Probability					[18][3]=0.0394;
	Energygammas				[18][3]=0.00087306;
	NucleusFinalState		[18][3]=4;

	Probability					[18][4]=0.8558;
	Energygammas				[18][4]=0.00153853;
	NucleusFinalState		[18][4]=1;

	Probability					[18][5]=0.0295;
	Energygammas				[18][5]=0.00214799;
	NucleusFinalState		[18][5]=0;

	//19 excited state
	Probability					[19][1]=0.0128;
	Energygammas				[19][1]=0.0004867;
	NucleusFinalState		[19][1]=7;

	Probability					[19][2]=0.0625;
	Energygammas				[19][2]=0.0007697;
	NucleusFinalState		[19][2]=4;

	Probability					[19][3]=0.0394;
	Energygammas				[19][3]=0.00087306;
	NucleusFinalState		[19][3]=3;

	Probability					[19][4]=0.8558;
	Energygammas				[19][4]=0.00153853;
	NucleusFinalState		[19][4]=1;

	Probability					[19][5]=0.0295;
	Energygammas				[19][5]=0.00214799;
	NucleusFinalState		[19][5]=0;

	//20 excited state
	Probability					[20][1]=0.0131;
	Energygammas				[20][1]=0.00042807;
	NucleusFinalState		[20][1]=11;

	Probability					[20][2]=0.0665;
	Energygammas				[20][2]=0.00064922;
	NucleusFinalState		[20][2]=6;

	Probability					[20][3]=0.0452;
	Energygammas				[20][3]=0.00081496;
	NucleusFinalState		[20][3]=4;

	Probability					[20][4]=0.0049;
	Energygammas				[20][4]=0.0009177;
	NucleusFinalState		[20][4]=3;

	Probability					[20][5]=0.821;
	Energygammas				[20][5]=0.001583204;
	NucleusFinalState		[20][5]=1;

	Probability					[20][6]=0.0493;
	Energygammas				[20][6]=0.00219259;
	NucleusFinalState		[20][6]=0;

	//21 excited state
	Probability					[21][1]=0.0091;
	Energygammas				[21][1]=0.00046108;
	NucleusFinalState		[21][1]=10;

	Probability					[21][2]=0.0176;
	Energygammas				[21][2]=0.00047444;
	NucleusFinalState		[21][2]=9;

	Probability					[21][3]=0.0138;
	Energygammas				[21][3]=0.00054283;
	NucleusFinalState		[21][3]=7;

	Probability					[21][4]=0.0095;
	Energygammas				[21][4]=0.00066094;
	NucleusFinalState		[21][4]=6;

	Probability					[21][5]=0.0024;
	Energygammas				[21][5]=0.0007886;
	NucleusFinalState		[21][5]=5;

	Probability					[21][6]=0.0211;
	Energygammas				[21][6]=0.0082645;
	NucleusFinalState		[21][6]=4;

	Probability					[21][7]=0.0475;
	Energygammas				[21][7]=0.00159475;
	NucleusFinalState		[21][7]=1;

	Probability					[21][8]=0.879;//to Prob=0.8791
	Energygammas				[21][8]=0.002204059;
	NucleusFinalState		[21][8]=0;

	//22 excited state
	Probability					[22][1]=0.0095;
	Energygammas				[22][1]=0.0005476;
	NucleusFinalState		[22][1]=7;

	Probability					[22][2]=0.1292;
	Energygammas				[22][2]=0.0009341;
	NucleusFinalState		[22][2]=3;

	Probability					[22][3]=0.8613;
	Energygammas				[22][3]=0.00159937;
	NucleusFinalState		[22][3]=1;


	//28 excited state
	Probability					[28][1]=0.0109;
	Energygammas				[28][1]=0.00045292;
	NucleusFinalState		[28][1]=14;

	Probability					[28][2]=0.029;
	Energygammas				[28][2]=0.00068323;
	NucleusFinalState		[28][2]=11;

	Probability					[28][3]=0.0164;
	Energygammas				[28][3]=0.00070496;
	NucleusFinalState		[28][3]=10;

	Probability					[28][4]=0.1093;
	Energygammas				[28][4]=0.00078635;
	NucleusFinalState		[28][4]=7;

	Probability					[28][5]=0.0268;
	Energygammas				[28][5]=0.00090431;
	NucleusFinalState		[28][5]=6;

	Probability					[28][6]=0.0224;
	Energygammas				[28][6]=0.00103238;
	NucleusFinalState		[28][6]=5;

	Probability					[28][7]=0.0962;
	Energygammas				[28][7]=0.00106996;
	NucleusFinalState		[28][7]=4;

	Probability					[28][8]=0.0191;
	Energygammas				[28][8]=0.001173;
	NucleusFinalState		[28][8]=3;

	Probability					[28][9]=0.1235;
	Energygammas				[28][9]=0.00183836;
	NucleusFinalState		[28][9]=1;

	Probability					[28][10]=0.5464;
	Energygammas				[28][10]=0.0024477;
	NucleusFinalState		[28][10]=0;


	//29 excited state
	Probability					[29][1]=0.1013;
	Energygammas				[29][1]=0.0002738;
	NucleusFinalState		[29][1]=22;

	Probability					[29][2]=0.0293;
	Energygammas				[29][2]=0.00033478;
	NucleusFinalState		[29][2]=19;

	Probability					[29][3]=0.01;
	Energygammas				[29][3]=0.00039405;
	NucleusFinalState		[29][3]=17;

	Probability					[29][4]=0.0221;
	Energygammas				[29][4]=0.00048795;
	NucleusFinalState		[29][4]=14;

	Probability					[29][5]=0.005;
	Energygammas				[29][5]=0.00063472;
	NucleusFinalState		[29][5]=12;

	Probability					[29][6]=0.1017;
	Energygammas				[29][6]=0.00075285;
	NucleusFinalState		[29][6]=9;

	Probability					[29][7]=0.1274;
	Energygammas				[29][7]=0.00082118;
	NucleusFinalState		[29][7]=7;

	Probability					[29][8]=0.0139;
	Energygammas				[29][8]=0.0009396;
	NucleusFinalState		[29][8]=6;

	Probability					[29][9]=0.0614;
	Energygammas				[29][9]=0.00110471;
	NucleusFinalState		[29][9]=4;

	Probability					[29][10]=0.3568;
	Energygammas				[29][10]=0.00120768;
	NucleusFinalState		[29][10]=3;

	Probability					[29][11]=0.1695;
	Energygammas				[29][11]=0.00187316;
	NucleusFinalState		[29][11]=1;

	Probability					[29][12]=0.0016;//0.0018 to Prob=1
	Energygammas				[29][12]=0.0024828;
	NucleusFinalState		[29][12]=0;

	Int_t numGammas;
	Float_t random =gRandom->Rndm();

	if (InicialState==1 || InicialState==3 || InicialState==5 || InicialState==10){numGammas=1;}
	else if(InicialState==4 || InicialState==7 || InicialState==8 || InicialState==17){numGammas=2;}
	else if(InicialState==13 || InicialState==22){numGammas=3;}
	else if(InicialState==6 || InicialState==9 || InicialState==15 || InicialState==16){numGammas=4;}
	else if(InicialState==11 || InicialState==14 || InicialState==18 || InicialState==19){numGammas=5;}
	else if(InicialState==12 || InicialState==20){numGammas=6;}
	else if(InicialState==21){numGammas=8;}
	else if(InicialState==28){numGammas=10;}
	else if(InicialState==29){numGammas=12;}
	else{
		cout << "-E- EnsarUraniumChainGen_226Ra: Incorrect final state!" << endl;
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
void EnsarUraniumChainGen_214Bi::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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

ClassImp(EnsarUraniumChainGen_214Bi)
