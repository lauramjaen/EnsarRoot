// *********************************************************************
// *****   
// *****             EnsarThoriumChainGen source file                                   
// *****   							
// *********************************************************************

#include "EnsarThoriumChainGen.h"

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
EnsarThoriumChainGen::EnsarThoriumChainGen()  :
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
EnsarThoriumChainGen::EnsarThoriumChainGen(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0)
{
  
  cout << "-I- EnsarThoriumChainGen: Opening input file " << inputFile << endl;
  fFileName  = inputFile;
  
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarThoriumChainGen","Cannot open input file.");
   
  //Read Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarThoriumChainGen::Init()
{
		
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarThoriumChainGen: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarThoriumChainGen::~EnsarThoriumChainGen() {

  CloseInput();
}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarThoriumChainGen::ReadEvent(FairPrimaryGenerator* primGen)
{
  
  
  //----- Check for input file -----
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarThoriumChainGen: Input file not open!" << endl;
    return kFALSE;
  }
 
	Bool_t		 ftrack_228Th;
	Bool_t		 ftrack_228Ac;
	Bool_t		 ftrack_228Ra;
	Bool_t		 ftrack_224Ra;
	Bool_t		 ftrack_212Bi;
	Bool_t		 ftrack_212Pb;
	Bool_t		 ftrack_208Tl;
	Bool_t		 fanytrack;
	fanytrack=false;
	ftrack_228Th=false;
  ftrack_228Ac=false;
	ftrack_228Ra=false;
	ftrack_224Ra=false;
  ftrack_212Bi=false;
	ftrack_212Pb=false;
	ftrack_208Tl=false;

	Int_t fnumGammas_228Th=1;		//total number of gammas in each decay
	Int_t fnumGammas_228Ac=16;
	Int_t fnumGammas_228Ra=1;
	Int_t fnumGammas_224Ra=1;
	Int_t fnumGammas_212Bi=4;
	Int_t fnumGammas_212Pb=2;
	Int_t fnumGammas_208Tl=5;

  Double_t fenergy_228Th [fnumGammas_228Th];	//Energy of gammas
  Double_t fenergy_228Ac [fnumGammas_228Ac];
  Double_t fenergy_228Ra [fnumGammas_228Ra];
  Double_t fenergy_224Ra [fnumGammas_224Ra];
  Double_t fenergy_212Bi [fnumGammas_212Bi];
  Double_t fenergy_212Pb [fnumGammas_212Pb];
  Double_t fenergy_208Tl [fnumGammas_208Tl];

	fenergy_228Th[0]=0.0000844;//GeV

	fenergy_228Ac[0]=0.001631;
 	fenergy_228Ac[1]=0.001588;
	fenergy_228Ac[2]=0.0000995;
	fenergy_228Ac[3]=0.000463;
	fenergy_228Ac[4]=0.000409;
	fenergy_228Ac[5]=0.000772;
	fenergy_228Ac[6]=0.000795;
	fenergy_228Ac[7]=0.000965;
	fenergy_228Ac[8]=0.000836;
	fenergy_228Ac[9]=0.000969;
	fenergy_228Ac[10]=0.000911;
	fenergy_228Ac[11]=0.000338;
	fenergy_228Ac[12]=0.000209;
	fenergy_228Ac[14]=0.000328;
	fenergy_228Ac[15]=0.000270;
	fenergy_228Ac[16]=0.000129;
	
	fenergy_228Ra[0]=0.0000135;

	fenergy_224Ra[0]=0.000241;
	
	fenergy_212Bi[0]=0.001621;
	fenergy_212Bi[1]=0.000785;
	fenergy_212Bi[2]=0.000727;
	fenergy_212Bi[3]=0.0000399;

	fenergy_212Pb[0]=0.000300;
	fenergy_212Pb[1]=0.000239;

	fenergy_208Tl[0]=0.000511;
	fenergy_208Tl[1]=0.000277;
	fenergy_208Tl[2]=0.000861;
	fenergy_208Tl[3]=0.000583;
	fenergy_208Tl[4]=0.002615;



  Double_t fprob_228Th [fnumGammas_228Th];	//Energy of gammas
  Double_t fprob_228Ac [fnumGammas_228Ac];
  Double_t fprob_228Ra [fnumGammas_228Ra];
  Double_t fprob_224Ra [fnumGammas_224Ra];
  Double_t fprob_212Bi [fnumGammas_212Bi];
  Double_t fprob_212Pb [fnumGammas_212Pb];
  Double_t fprob_208Tl [fnumGammas_208Tl];

	fprob_228Th[0]=0.013;

	fprob_228Ac[0]=0.016; 
	fprob_228Ac[1]=0.033;
 	fprob_228Ac[2]=0.013;
	fprob_228Ac[3]=0.044;
	fprob_228Ac[4]=0.019;
	fprob_228Ac[5]=0.015;
	fprob_228Ac[6]=0.043;
	fprob_228Ac[7]=0.051;
	fprob_228Ac[8]=0.017;
	fprob_228Ac[9]=0.162;
	fprob_228Ac[10]=0.266;
	fprob_228Ac[11]=0.113;
	fprob_228Ac[12]=0.039;
	fprob_228Ac[13]=0.03;
	fprob_228Ac[14]=0.034;
	fprob_228Ac[15]=0.025;
	
	fprob_228Ra[0]=0.016;

	fprob_224Ra[0]=0.04;
	
	fprob_212Bi[0]=0.015;
	fprob_212Bi[1]=0.011;
	fprob_212Bi[2]=0.067;
	fprob_212Bi[3]=0.011;

	fprob_212Pb[0]=0.033;
	fprob_212Pb[1]=0.436;

	fprob_208Tl[0]=0.081;
	fprob_208Tl[1]=0.023;
	fprob_208Tl[2]=0.045;
	fprob_208Tl[3]=0.304;
	fprob_208Tl[4]=0.356;


	//Limits calculation 
	Double_t limit_228Th[fnumGammas_228Th+1];
	Double_t limit_228Ac[fnumGammas_228Ac+1];
	Double_t limit_228Ra[fnumGammas_228Ra+1];
	Double_t limit_224Ra[fnumGammas_224Ra+1];
	Double_t limit_212Bi[fnumGammas_212Bi+1];
	Double_t limit_212Pb[fnumGammas_212Pb+1];
	Double_t limit_208Tl[fnumGammas_208Tl+1];

/*228Th 228Ac 228Ra 224Ra 212Bi 212Pb 208Tl*/
	limit_228Th[0]=0.;
	for(Int_t i=1; i<fnumGammas_228Th+1; i++){
			for(Int_t j=0;j<i;j++){limit_228Th[i]=fprob_228Th[j]+limit_228Th[i];}		
	}

	limit_228Ac[0]=0.;
	for(Int_t i=1; i<fnumGammas_228Ac+1; i++){
			for(Int_t j=0;j<i;j++){limit_228Ac[i]=fprob_228Ac[j]+limit_228Ac[i];}		
	}

	limit_228Ra[0]=0.;
	for(Int_t i=1; i<fnumGammas_228Ra+1; i++){
			for(Int_t j=0;j<i;j++){limit_228Ra[i]=fprob_228Ra[j]+limit_228Ra[i];}		
	}

	limit_224Ra[0]=0.;
	for(Int_t i=1; i<fnumGammas_224Ra+1; i++){
			for(Int_t j=0;j<i;j++){limit_224Ra[i]=fprob_224Ra[j]+limit_224Ra[i];}		
	}

	limit_212Bi[0]=0.;
	for(Int_t i=1; i<fnumGammas_212Bi+1; i++){
			for(Int_t j=0;j<i;j++){limit_212Bi[i]=fprob_212Bi[j]+limit_212Bi[i];}		
	}

	limit_212Pb[0]=0.;
	for(Int_t i=1; i<fnumGammas_212Pb+1; i++){
			for(Int_t j=0;j<i;j++){limit_212Pb[i]=fprob_212Pb[j]+limit_212Pb[i];}		
	}

	limit_208Tl[0]=0.;
	for(Int_t i=1; i<fnumGammas_208Tl+1; i++){
			for(Int_t j=0;j<i;j++){limit_208Tl[i]=fprob_208Tl[j]+limit_208Tl[i];}		
	}


	/*cout<<"  --- Checking out the Limits  -------------------"<<endl<<endl;	
	for(Int_t i=0; i<fnumGammas_228Th+1; i++){
		cout<<"228Th: i="<<i<<"  limit ="<<limit_228Th[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_228Ac+1; i++){
		cout<<"228Ac: i="<<i<<"  limit ="<<limit_228Ac[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_228Ra+1; i++){
		cout<<"228Ra: i="<<i<<"  limit ="<<limit_228Ra[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_224Ra+1; i++){
		cout<<"224Ra: i="<<i<<"  limit ="<<limit_224Ra[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_212Bi+1; i++){
		cout<<"212Bi: i="<<i<<"  limit ="<<limit_212Bi[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_212Pb+1; i++){
		cout<<"212Pb: i="<<i<<"  limit ="<<limit_212Pb[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_208Tl+1; i++){
		cout<<"208Tl: i="<<i<<"  limit ="<<limit_208Tl[i]<<endl;
	}*/

	Int_t Gamma_228Th;
	Int_t Gamma_228Ac;
	Int_t Gamma_228Ra;
	Int_t Gamma_224Ra;
	Int_t Gamma_212Bi;
	Int_t Gamma_212Pb;
	Int_t Gamma_208Tl;

 	Float_t ran_228Th =gRandom->Rndm();
	Float_t ran_228Ac =gRandom->Rndm();
 	Float_t ran_228Ra =gRandom->Rndm();
 	Float_t ran_224Ra =gRandom->Rndm();
 	Float_t ran_212Bi =gRandom->Rndm();
	Float_t ran_212Pb =gRandom->Rndm();
 	Float_t ran_208Tl =gRandom->Rndm();

/*228Th 228Ac 228Ra 224Ra 212Bi 212Pb 208Tl*/
  
	for (Int_t i=0; i<fnumGammas_228Th; i++){
		if (ran_228Th>limit_228Th[i] && ran_228Th<limit_228Th[i+1]){
			Gamma_228Th=i;
			ftrack_228Th=true;
			fanytrack=true;
			//cout<<"Gamma_228Th="<<Gamma_228Th<<endl;
			//cout<<"ran_228Th="<<ran_228Th<<"  between ["<<limit_228Th[i]<<" and "<<limit_228Th[i+1]<<"]"<<endl<<endl;
		}
	}

	for (Int_t i=0; i<fnumGammas_228Ac; i++){
		if (ran_228Ac>limit_228Ac[i] && ran_228Ac<limit_228Ac[i+1]){
			Gamma_228Ac=i;
			ftrack_228Ac=true;
			fanytrack=true;
			//cout<<"Gamma_228Ac="<<Gamma_228Ac<<endl;
			//cout<<"ran_228Ac="<<ran_228Ac<<"  between ["<<limit_228Ac[i]<<" and "<<limit_228Ac[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_228Ra; i++){
		if (ran_228Ra>limit_228Ra[i] && ran_228Ra<limit_228Ra[i+1]){
			Gamma_228Ra=i;
			ftrack_228Ra=true;
			fanytrack=true;
			//cout<<"Gamma_228Ra="<<Gamma_228Ra<<endl;
			//cout<<"ran_228Ra="<<ran_228Ra<<"  between ["<<limit_228Ra[i]<<" and "<<limit_228Ra[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_224Ra; i++){
		if (ran_224Ra>limit_224Ra[i] && ran_224Ra<limit_224Ra[i+1]){
			Gamma_224Ra=i;
			ftrack_224Ra=true;
			fanytrack=true;
			//cout<<"Gamma_224Ra="<<Gamma_224Ra<<endl;
			//cout<<"ran_224Ra="<<ran_224Ra<<"  between ["<<limit_224Ra[i]<<" and "<<limit_224Ra[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_212Bi; i++){
		if (ran_212Bi>limit_212Bi[i] && ran_212Bi<limit_212Bi[i+1]){
			Gamma_212Bi=i;
			ftrack_212Bi=true;
			fanytrack=true;
			//cout<<"Gamma_212Bi="<<Gamma_212Bi<<endl;
			//cout<<"ran_212Bi="<<ran_212Bi<<"  between ["<<limit_212Bi[i]<<" and "<<limit_212Bi[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_212Pb; i++){
		if (ran_212Pb>limit_212Pb[i] && ran_212Pb<limit_212Pb[i+1]){
			Gamma_212Pb=i;
			ftrack_212Pb=true;
			fanytrack=true;
			//cout<<"Gamma_212Pb="<<Gamma_212Pb<<endl;
			//cout<<"ran_212Pb="<<ran_212Pb<<"  between ["<<limit_212Pb[i]<<" and "<<limit_212Pb[i+1]<<"]"<<endl<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_208Tl; i++){
		if (ran_208Tl>limit_208Tl[i] && ran_208Tl<limit_208Tl[i+1]){
			Gamma_208Tl=i;
			ftrack_208Tl=true;
			fanytrack=true;
			//cout<<"Gamma_208Tl="<<Gamma_208Tl<<endl;
			//cout<<"ran_208Tl="<<ran_208Tl<<"  between ["<<limit_208Tl[i]<<" and "<<limit_208Tl[i+1]<<"]"<<endl<<endl;
		}
	}

	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

//cout<<endl<<endl;

/*228Th 228Ac 228Ra 224Ra 212Bi 212Pb 208Tl*/

	if (ftrack_228Th==true){
		Double_t px_228Th, py_228Th, pz_228Th;
		TrackMomentum(fenergy_228Th[Gamma_228Th], px_228Th, py_228Th, pz_228Th);
		//cout<<"fenergy_228Th["<<Gamma_228Th<<"]="<<fenergy_228Th[Gamma_228Th]<<"  px="<<px_228Th<<"  py="<<py_228Th<<"  pz="<<pz_228Th<<endl<<endl;
		primGen->AddTrack(fPDGType, px_228Th, py_228Th, pz_228Th, fX, fY, fZ);
	}

	if (ftrack_228Ac==true){
		Double_t px_228Ac, py_228Ac, pz_228Ac;
		TrackMomentum(fenergy_228Ac[Gamma_228Ac], px_228Ac, py_228Ac, pz_228Ac);
		//cout<<"fenergy_228Ac["<<Gamma_228Ac<<"]="<<fenergy_228Ac[Gamma_228Ac]<<"  px="<<px_228Ac<<"  py="<<py_228Ac<<"  pz="<<pz_228Ac<<endl<<endl;
		primGen->AddTrack(fPDGType, px_228Ac, py_228Ac, pz_228Ac, fX, fY, fZ);
	}
	if (ftrack_228Ra==true){
		Double_t px_228Ra, py_228Ra, pz_228Ra;
		TrackMomentum(fenergy_228Ra[Gamma_228Ra], px_228Ra, py_228Ra, pz_228Ra);
		//cout<<"fenergy_228Ra["<<Gamma_228Ra<<"]="<<fenergy_228Ra[Gamma_228Ra]<<"  px="<<px_228Ra<<"  py="<<py_228Ra<<"  pz="<<pz_228Ra<<endl<<endl;
		primGen->AddTrack(fPDGType, px_228Ra, py_228Ra, pz_228Ra, fX, fY, fZ);
	}
	if (ftrack_224Ra==true){
		Double_t px_224Ra, py_224Ra, pz_224Ra;
		TrackMomentum(fenergy_224Ra[Gamma_224Ra], px_224Ra, py_224Ra, pz_224Ra);
		//cout<<"fenergy_224Ra["<<Gamma_224Ra<<"]="<<fenergy_224Ra[Gamma_224Ra]<<"  px="<<px_224Ra<<"  py="<<py_224Ra<<"  pz="<<pz_224Ra<<endl<<endl;
		primGen->AddTrack(fPDGType, px_224Ra, py_224Ra, pz_224Ra, fX, fY, fZ);
	}
	if (ftrack_212Bi==true){
		Double_t px_212Bi, py_212Bi, pz_212Bi;
		TrackMomentum(fenergy_212Bi[Gamma_212Bi], px_212Bi, py_212Bi, pz_212Bi);
		//cout<<"fenergy_212Bi["<<Gamma_212Bi<<"]="<<fenergy_212Bi[Gamma_212Bi]<<"  px="<<px_212Bi<<"  py="<<py_212Bi<<"  pz="<<pz_212Bi<<endl<<endl;
		primGen->AddTrack(fPDGType, px_212Bi, py_212Bi, pz_212Bi, fX, fY, fZ);
	}
	if (ftrack_212Pb==true){
		Double_t px_212Pb, py_212Pb, pz_212Pb;
		TrackMomentum(fenergy_212Pb[Gamma_212Pb], px_212Pb, py_212Pb, pz_212Pb);
		//cout<<"fenergy_212Pb["<<Gamma_212Pb<<"]="<<fenergy_212Pb[Gamma_212Pb]<<"  px="<<px_212Pb<<"  py="<<py_212Pb<<"  pz="<<pz_212Pb<<endl<<endl;
		primGen->AddTrack(fPDGType, px_212Pb, py_212Pb, pz_212Pb, fX, fY, fZ);
	}
	if (ftrack_208Tl==true){
		Double_t px_208Tl, py_208Tl, pz_208Tl;
		TrackMomentum(fenergy_208Tl[Gamma_208Tl], px_208Tl, py_208Tl, pz_208Tl);
		//cout<<"fenergy_208Tl["<<Gamma_208Tl<<"]="<<fenergy_208Tl[Gamma_208Tl]<<"  px="<<px_208Tl<<"  py="<<py_208Tl<<"  pz="<<pz_208Tl<<endl<<endl;
		primGen->AddTrack(fPDGType, px_208Tl, py_208Tl, pz_208Tl, fX, fY, fZ);
	}

	if (fanytrack==false){
	cout<<"-I- EnsarThoriumChainGen: <<<<<<    Gamma of 1keV is added, no gamma from the Uranium chain.     <<<<<<"<<endl;
	primGen->AddTrack(fPDGType, 0.0000001, 0.0000001, 0.0000001, 0., 0., 0.);
	}

//cout<<"////////////////////////////////////////////////"<<endl<<endl<<endl;
  return kTRUE;
  
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void EnsarThoriumChainGen::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarThoriumChainGenr: Closing input file " 
	   << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


// ---Read Parameters File --------------------------------------------
void EnsarThoriumChainGen::ReadParameters() {

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
void EnsarThoriumChainGen::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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

ClassImp(EnsarThoriumChainGen)
