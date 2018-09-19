// *********************************************************************
// *****   
// *****             EnsarUraniumChainGen source file                                   
// *****   							
// *********************************************************************

#include "EnsarUraniumChainGen.h"

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
EnsarUraniumChainGen::EnsarUraniumChainGen()  :
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
EnsarUraniumChainGen::EnsarUraniumChainGen(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarUraniumChainGen: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarUraniumChainGen","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarUraniumChainGen::Init()
{
		
	//Checking out the probabilities
	/*Double_t sumProb;
	sumProb=0;
	for (Int_t i=0; i<fnumGammas; i++){
		sumProb=sumProb+fprobability[i];
	}
	if (sumProb>1.001){
		Fatal("Init()","EnsarUraniumChainGen: The sum of all probabilities is higher than 1! Modify it, please.");		
	}*/
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarUraniumChainGen: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarUraniumChainGen::~EnsarUraniumChainGen() {

  CloseInput();
}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarUraniumChainGen::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarUraniumChainGen: Input file not open!" << endl;
    return kFALSE;
  }
 
	Bool_t		 ftrack_234Th;
	Bool_t		 ftrack_226Ra;
	Bool_t		 ftrack_214Bi;
	Bool_t		 ftrack_214Pb;
	Bool_t		 ftrack_210Pb;
	Bool_t		 fanytrack;	
	fanytrack=false;
	ftrack_234Th=false;
  ftrack_226Ra=false;
	ftrack_214Bi=false;
  ftrack_214Pb=false;
	ftrack_210Pb=false;


	Int_t fnumGammas_234Th=3;	//total number of gammas in each decay
	Int_t fnumGammas_226Ra=1;
	Int_t fnumGammas_214Bi=14;
	Int_t fnumGammas_214Pb=2;
	Int_t fnumGammas_210Pb=1;

  Double_t fenergy_234Th [fnumGammas_234Th];	//Energy of gammas
  Double_t fenergy_226Ra [fnumGammas_226Ra];
  Double_t fenergy_214Bi [fnumGammas_214Bi];
  Double_t fenergy_214Pb [fnumGammas_214Pb];
  Double_t fenergy_210Pb [fnumGammas_210Pb];

	fenergy_234Th[0]=0.0000928;//GeV
	fenergy_234Th[1]=0.0000633;
	fenergy_234Th[2]=0.0000924;

	fenergy_226Ra[0]=0.000186;

	fenergy_214Bi[0]=0.002448; 
	fenergy_214Bi[1]=0.002204;
 	fenergy_214Bi[2]=0.002119;
	fenergy_214Bi[3]=0.001509;
	fenergy_214Bi[4]=0.001408;
	fenergy_214Bi[5]=0.001847;
	fenergy_214Bi[6]=0.001238;
	fenergy_214Bi[7]=0.001765;
	fenergy_214Bi[8]=0.001155;
	fenergy_214Bi[9]=0.001730;
	fenergy_214Bi[10]=0.001120;
	fenergy_214Bi[11]=0.000934;
	fenergy_214Bi[12]=0.001378;
	fenergy_214Bi[13]=0.000768;
	
	fenergy_214Pb[0]=0.000295;
	fenergy_214Pb[1]=0.000242;

	fenergy_210Pb[0]=0.0000465;

  Double_t fprob_234Th [fnumGammas_234Th];	//Probability of gammas
  Double_t fprob_226Ra [fnumGammas_226Ra];
  Double_t fprob_214Bi [fnumGammas_214Bi];
  Double_t fprob_214Pb [fnumGammas_214Pb];
  Double_t fprob_210Pb [fnumGammas_210Pb];

	fprob_234Th[0]=0.024;
	fprob_234Th[1]=0.043;
	fprob_234Th[2]=0.025;

	fprob_226Ra[0]=0.035;

	fprob_214Bi[0]=0.014;
	fprob_214Bi[1]=0.046;
	fprob_214Bi[2]=0.011;
	fprob_214Bi[3]=0.02;
	fprob_214Bi[4]=0.027;
	fprob_214Bi[5]=0.02;
	fprob_214Bi[6]=0.056;
	fprob_214Bi[7]=0.147;
	fprob_214Bi[8]=0.016;
	fprob_214Bi[9]=0.028;
	fprob_214Bi[10]=0.141;
	fprob_214Bi[11]=0.029;
	fprob_214Bi[12]=0.037;
	fprob_214Bi[13]=0.046;

	fprob_214Pb[0]=0.177;
	fprob_214Pb[1]=0.072;

	fprob_210Pb[0]=0.039;


	//Limits calculation 
	Double_t limit_234Th[fnumGammas_234Th+1];
	Double_t limit_226Ra[fnumGammas_226Ra+1];
	Double_t limit_214Bi[fnumGammas_214Bi+1];
	Double_t limit_214Pb[fnumGammas_214Pb+1];
	Double_t limit_210Pb[fnumGammas_210Pb+1];

	limit_234Th[0]=0.;
	for(Int_t i=1; i<fnumGammas_234Th+1; i++){
			for(Int_t j=0;j<i;j++){limit_234Th[i]=fprob_234Th[j]+limit_234Th[i];}		
	}

	limit_226Ra[0]=0.;
	for(Int_t i=1; i<fnumGammas_226Ra+1; i++){
			for(Int_t j=0;j<i;j++){limit_226Ra[i]=fprob_226Ra[j]+limit_226Ra[i];}		
	}

	limit_214Bi[0]=0.;
	for(Int_t i=1; i<fnumGammas_214Bi+1; i++){
			for(Int_t j=0;j<i;j++){limit_214Bi[i]=fprob_214Bi[j]+limit_214Bi[i];}		
	}

	limit_214Pb[0]=0.;
	for(Int_t i=1; i<fnumGammas_214Pb+1; i++){
			for(Int_t j=0;j<i;j++){limit_214Pb[i]=fprob_214Pb[j]+limit_214Pb[i];}		
	}

	limit_210Pb[0]=0.;
	for(Int_t i=1; i<fnumGammas_210Pb+1; i++){
			for(Int_t j=0;j<i;j++){limit_210Pb[i]=fprob_210Pb[j]+limit_210Pb[i];}		
	}


/*cout<<"  --- Checking out the Limits  -------------------"<<endl<<endl;	
	for(Int_t i=0; i<fnumGammas_234Th+1; i++){
		cout<<"Th: i="<<i<<"  limit ="<<limit_234Th[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_226Ra+1; i++){
		cout<<"226Ra: i="<<i<<"  limit ="<<limit_226Ra[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_214Bi+1; i++){
		cout<<"214Bi: i="<<i<<"  limit ="<<limit_214Bi[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_214Pb+1; i++){
		cout<<"214Pb: i="<<i<<"  limit ="<<limit_214Pb[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_210Pb+1; i++){
		cout<<"210Pb: i="<<i<<"  limit ="<<limit_210Pb[i]<<endl;
	}*/


  
	Int_t Gamma_234Th;
	Int_t Gamma_226Ra;
	Int_t Gamma_214Bi;
	Int_t Gamma_214Pb;
	Int_t Gamma_210Pb;
 	Float_t ran_234Th =gRandom->Rndm();
	Float_t ran_226Ra =gRandom->Rndm();
 	Float_t ran_214Bi =gRandom->Rndm();
 	Float_t ran_214Pb =gRandom->Rndm();
 	Float_t ran_210Pb =gRandom->Rndm();
  
	for (Int_t i=0; i<fnumGammas_234Th; i++){
		if (ran_234Th>limit_234Th[i] && ran_234Th<limit_234Th[i+1]){
			Gamma_234Th=i;
			ftrack_234Th=true;
			fanytrack=true;
			//cout<<"Gamma_234Th="<<Gamma_234Th<<endl;
			//cout<<"ran_234Th="<<ran_234Th<<"  between ["<<limit_234Th[i]<<" and "<<limit_234Th[i+1]<<"]"<<endl<<endl;
		}
	}

	for (Int_t i=0; i<fnumGammas_226Ra; i++){
		if (ran_226Ra>limit_226Ra[i] && ran_226Ra<limit_226Ra[i+1]){
			Gamma_226Ra=i;
			ftrack_226Ra=true;
			fanytrack=true;
			//cout<<"Gamma_226Ra="<<Gamma_226Ra<<endl;
			//cout<<"ran_226Ra="<<ran_226Ra<<"  between ["<<limit_226Ra[i]<<" and "<<limit_226Ra[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_214Bi; i++){
		if (ran_214Bi>limit_214Bi[i] && ran_214Bi<limit_214Bi[i+1]){
			Gamma_214Bi=i;
			ftrack_214Bi=true;
			fanytrack=true;
			//cout<<"Gamma_214Bi="<<Gamma_214Bi<<endl;
			//cout<<"ran_214Bi="<<ran_214Bi<<"  between ["<<limit_214Bi[i]<<" and "<<limit_214Bi[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_214Pb; i++){
		if (ran_214Pb>limit_214Pb[i] && ran_214Pb<limit_214Pb[i+1]){
			Gamma_214Pb=i;
			ftrack_214Pb=true;
			fanytrack=true;
			//cout<<"Gamma_214Pb="<<Gamma_214Pb<<endl;
			//cout<<"ran_214Pb="<<ran_214Pb<<"  between ["<<limit_214Pb[i]<<" and "<<limit_214Pb[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_210Pb; i++){
		if (ran_210Pb>limit_210Pb[i] && ran_210Pb<limit_210Pb[i+1]){
			Gamma_210Pb=i;
			ftrack_210Pb=true;
			fanytrack=true;
			//cout<<"Gamma_210Pb="<<Gamma_210Pb<<endl;
			//cout<<"ran_210Pb="<<ran_210Pb<<"  between ["<<limit_210Pb[i]<<" and "<<limit_210Pb[i+1]<<"]"<<endl<<endl;
		}
	}

	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

//cout<<endl<<endl;

	if (ftrack_234Th==true){
		Double_t px_234Th, py_234Th, pz_234Th;
		TrackMomentum(fenergy_234Th[Gamma_234Th], px_234Th, py_234Th, pz_234Th);
		//cout<<"fenergy_234Th["<<Gamma_234Th<<"]="<<fenergy_234Th[Gamma_234Th]<<"  px="<<px_234Th<<"  py="<<py_234Th<<"  pz="<<pz_234Th<<endl<<endl;
		primGen->AddTrack(fPDGType, px_234Th, py_234Th, pz_234Th, fX, fY, fZ);
	}

	if (ftrack_226Ra==true){
		Double_t px_226Ra, py_226Ra, pz_226Ra;
		TrackMomentum(fenergy_226Ra[Gamma_226Ra], px_226Ra, py_226Ra, pz_226Ra);
		//cout<<"fenergy_226Ra["<<Gamma_226Ra<<"]="<<fenergy_226Ra[Gamma_226Ra]<<"  px="<<px_226Ra<<"  py="<<py_226Ra<<"  pz="<<pz_226Ra<<endl<<endl;
		primGen->AddTrack(fPDGType, px_226Ra, py_226Ra, pz_226Ra, fX, fY, fZ);
	}

	if (ftrack_214Bi==true){
		Double_t px_214Bi, py_214Bi, pz_214Bi;
		TrackMomentum(fenergy_214Bi[Gamma_214Bi], px_214Bi, py_214Bi, pz_214Bi);
		//cout<<"fenergy_214Bi["<<Gamma_214Bi<<"]="<<fenergy_214Bi[Gamma_214Bi]<<"  px="<<px_214Bi<<"  py="<<py_214Bi<<"  pz="<<pz_214Bi<<endl<<endl;
		primGen->AddTrack(fPDGType, px_214Bi, py_214Bi, pz_214Bi, fX, fY, fZ);
	}

	if (ftrack_214Pb==true){
		Double_t px_214Pb, py_214Pb, pz_214Pb;
		TrackMomentum(fenergy_214Pb[Gamma_214Pb], px_214Pb, py_214Pb, pz_214Pb);
		//cout<<"fenergy_214Pb["<<Gamma_214Pb<<"]="<<fenergy_214Pb[Gamma_214Pb]<<"  px="<<px_214Pb<<"  py="<<py_214Pb<<"  pz="<<pz_214Pb<<endl<<endl;
		primGen->AddTrack(fPDGType, px_214Pb, py_214Pb, pz_214Pb, fX, fY, fZ);
	}
	if (ftrack_210Pb==true){
		Double_t px_210Pb, py_210Pb, pz_210Pb;
		TrackMomentum(fenergy_210Pb[Gamma_210Pb], px_210Pb, py_210Pb, pz_210Pb);
		//cout<<"fenergy_210Pb["<<Gamma_210Pb<<"]="<<fenergy_210Pb[Gamma_210Pb]<<"  px="<<px_210Pb<<"  py="<<py_210Pb<<"  pz="<<pz_210Pb<<endl<<endl;
		primGen->AddTrack(fPDGType, px_210Pb, py_210Pb, pz_210Pb, fX, fY, fZ);
	}

	if (fanytrack==false){
	cout<<"-I- EnsarUraniumChainGen: <<<<<<    Gamma of 1keV is added, no gamma from the Uranium chain.     <<<<<<"<<endl;
	primGen->AddTrack(fPDGType, 0.0000001, 0.0000001, 0.0000001, 0., 0., 0.);
	}

//cout<<"////////////////////////////////////////////////"<<endl<<endl<<endl;
  return kTRUE;
  
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void EnsarUraniumChainGen::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarUraniumChainGenr: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarUraniumChainGen::ReadParameters() {

 	//def dynamic vectors	
  /*fenergy_228Th		= new Double_t [fnumGammas_228Th];
	fenergy_228Ac		= new Double_t [fnumGammas_228Ac];
	fenergy_228Ra		= new Double_t [fnumGammas_228Ra];
	fenergy_224Ra		= new Double_t [fnumGammas_224Ra];
	fenergy_212Bi		= new Double_t [fnumGammas_212Bi];
	fenergy_212Pb		= new Double_t [fnumGammas_212Pb];
	fenergy_208Tl		= new Double_t [fnumGammas_208Tl];

  fprob_228Th	  = new Double_t [fnumGammas_228Th];
	fprob_228Ac		= new Double_t [fnumGammas_228Ac];
	fprob_228Ra		= new Double_t [fnumGammas_228Ra];
	fprob_224Ra		= new Double_t [fnumGammas_224Ra];
	fprob_212Bi		= new Double_t [fnumGammas_212Bi];
	fprob_212Pb		= new Double_t [fnumGammas_212Pb];
	fprob_208Tl		= new Double_t [fnumGammas_208Tl];


  cout << "\n\n\t------ READING PARAMETERS ------\n" << endl;
    
	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');

  //228Th
	*fInputFile >> fnumGammas_228Th;
	cout<<"Gammas 228Th= "<<fnumGammas_228Th<<endl;
	for(int i = 0; i < fnumGammas_228Th; i++){
		  *fInputFile >> fenergy_228Th[i] >> fprob_228Th[i];
		  cout<<"    "<<fenergy_228Th[i]<<"  		"<<fprob_228Th[i]<<endl;
		  
	}

	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
	
	//228Ac
	*fInputFile >> fnumGammas_228Ac;
	cout<<"Gammas 228Ac= "<<fnumGammas_228Ac<<endl;
	for(int i = 0; i < fnumGammas_228Ac; i++){
		  *fInputFile >> fenergy_228Ac[i] >> fprob_228Ac[i];
			//cout<<"Gamma i="<<i+1<<endl;
		  cout<<"    "<<fenergy_228Ac[i]<<"  		"<<fprob_228Ac[i]<<endl;
	}

	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');
	
	//228Ra
	*fInputFile >> fnumGammas_228Ra;
	cout<<"Gammas 228Ra= "<<fnumGammas_228Ra<<endl;
	for(int i = 0; i < fnumGammas_228Ra; i++){
		  *fInputFile >> fenergy_228Ra[i] >> fprob_228Ra[i];
		  cout<<"    "<<fenergy_228Ra[i]<<"  		"<<fprob_228Ra[i]<<endl;
	}

	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');

	//224Ra
	*fInputFile >> fnumGammas_224Ra;
	cout<<"Gammas 224Ra= "<<fnumGammas_224Ra<<endl;
	for(int i = 0; i < fnumGammas_224Ra; i++){
		  *fInputFile >> fenergy_224Ra[i] >> fprob_224Ra[i];
		  cout<<"    "<<fenergy_224Ra[i]<<"  		"<<fprob_224Ra[i]<<endl;
	}

	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');

	//212Bi
	*fInputFile >> fnumGammas_212Bi;
	cout<<"Gammas 212Bi= "<<fnumGammas_212Bi<<endl;
	for(int i = 0; i < fnumGammas_212Bi; i++){
		  *fInputFile >> fenergy_212Bi[i] >> fprob_212Bi[i];
		  cout<<"    "<<fenergy_212Bi[i]<<"  		"<<fprob_212Bi[i]<<endl;
	}

	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');

	//212Pb
	*fInputFile >> fnumGammas_212Pb;
	cout<<"Gammas 212Pb= "<<fnumGammas_212Pb<<endl;
	for(int i = 0; i < fnumGammas_212Pb; i++){
		  *fInputFile >> fenergy_212Pb[i] >> fprob_212Pb[i];
		  cout<<"    "<<fenergy_212Pb[i]<<"  		"<<fprob_212Pb[i]<<endl;
	}

	fInputFile->ignore(256,'\n');
  fInputFile->ignore(256,'\n');

	//208Tl
	*fInputFile >> fnumGammas_208Tl;
	cout<<"Gammas 208Tl= "<<fnumGammas_208Tl<<endl;
	for(int i = 0; i < fnumGammas_208Tl; i++){
		  *fInputFile >> fenergy_208Tl[i] >> fprob_208Tl[i];
		  cout<<"    "<<fenergy_208Tl[i]<<"  		"<<fprob_208Tl[i]<<endl;
	}
	cout << "---------------------------------------------------------------" << endl;  */
}


// --- Track Momentum Calculus --------------------------------------------
void EnsarUraniumChainGen::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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
	
if (Px>0.0028 || Py>0.0028 || Pz>0.0028){
	cout << "Heyyy, this is the error energy="<<Energy<<"  Px="<<Px<<"  Py="<<Py<<"  Pz="<<Pz<<endl;
	}
	//cout<<"TrackMomentum function  Px="<<Px<<"  Py="<<Py<<"  Pz="<<Pz<<endl; 
}
//-------------------------------------------------------------------------

ClassImp(EnsarUraniumChainGen)
