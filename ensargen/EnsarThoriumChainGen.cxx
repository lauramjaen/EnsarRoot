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
	fnumGammas_228Th(0),fenergy_228Th(NULL),fprob_228Th(NULL),
	fnumGammas_228Ac(0),fenergy_228Ac(NULL),fprob_228Ac(NULL),
	fnumGammas_228Ra(0),fenergy_228Ra(NULL),fprob_228Ra(NULL),
	fnumGammas_224Ra(0),fenergy_224Ra(NULL),fprob_224Ra(NULL),
	fnumGammas_212Bi(0),fenergy_212Bi(NULL),fprob_212Bi(NULL),
	fnumGammas_212Pb(0),fenergy_212Pb(NULL),fprob_212Pb(NULL),
	fnumGammas_208Tl(0),fenergy_208Tl(NULL),fprob_208Tl(NULL),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0),
	ftrack_228Th(0), ftrack_228Ac(0), ftrack_228Ra(0), ftrack_224Ra(0),
	ftrack_212Bi(0), ftrack_212Pb(0), ftrack_208Tl(0), fanytrack(0)
{
}
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
EnsarThoriumChainGen::EnsarThoriumChainGen(const char* inputFile) :
  FairGenerator(),
	fPointVtxIsSet(0), fBoxVtxIsSet(0),
  fThetaRangeIsSet(0), fPhiRangeIsSet(0),
	fnumGammas_228Th(0),fenergy_228Th(NULL),fprob_228Th(NULL),
	fnumGammas_228Ac(0),fenergy_228Ac(NULL),fprob_228Ac(NULL),
	fnumGammas_228Ra(0),fenergy_228Ra(NULL),fprob_228Ra(NULL),
	fnumGammas_224Ra(0),fenergy_224Ra(NULL),fprob_224Ra(NULL),
	fnumGammas_212Bi(0),fenergy_212Bi(NULL),fprob_212Bi(NULL),
	fnumGammas_212Pb(0),fenergy_212Pb(NULL),fprob_212Pb(NULL),
	fnumGammas_208Tl(0),fenergy_208Tl(NULL),fprob_208Tl(NULL),
	fX(0), fY(0), fZ(0),
	fX1(0), fY1(0), fZ1(0), fX2(0), fY2(0), fZ2(0),
	fThetaMin(0), fThetaMax(0),
	fPhiMin(0), fPhiMax(0),
	ftrack_228Th(0), ftrack_228Ac(0), ftrack_228Ra(0), ftrack_224Ra(0),
	ftrack_212Bi(0), ftrack_212Pb(0), ftrack_208Tl(0), fanytrack(0)
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
		
	//Checking out the probabilities
	/*Double_t sumProb;
	sumProb=0;
	for (Int_t i=0; i<fnumGammas; i++){
		sumProb=sumProb+fprobability[i];
	}
	if (sumProb>1.001){
		Fatal("Init()","EnsarThoriumChainGen: The sum of all probabilities is higher than 1! Modify it, please.");		
	}*/
	if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","EnsarThoriumChainGen: Cannot set point and box vertices simultaneously");
  }
	
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarThoriumChainGen::~EnsarThoriumChainGen() {

	delete fenergy_228Th;
	delete fenergy_228Ac;
	delete fenergy_228Ra;
	delete fenergy_224Ra;
	delete fenergy_212Bi;
	delete fenergy_212Pb;
	delete fenergy_208Tl;

  delete fprob_228Th;
	delete fprob_228Ac;
	delete fprob_228Ra;
	delete fprob_224Ra;
	delete fprob_212Bi;
	delete fprob_212Pb;
	delete fprob_208Tl	;
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
 
	fanytrack=false;
	ftrack_228Th=false;
  ftrack_228Ac=false;
	ftrack_228Ra=false;
  ftrack_224Ra=false;
	ftrack_212Bi=false;
  ftrack_212Pb=false;
	ftrack_208Tl=false;

	//Limits calculation 
	Double_t limit_228Th[fnumGammas_228Th+1];
	Double_t limit_228Ac[fnumGammas_228Ac+1];
	Double_t limit_228Ra[fnumGammas_228Ra+1];
	Double_t limit_224Ra[fnumGammas_224Ra+1];
	Double_t limit_212Bi[fnumGammas_212Bi+1];
	Double_t limit_212Pb[fnumGammas_212Pb+1];
	Double_t limit_208Tl[fnumGammas_208Tl+1];


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


//cout<<"  --- Checking out the Limits  -------------------"<<endl<<endl;	
	for(Int_t i=0; i<fnumGammas_228Th+1; i++){
		//cout<<"Th: i="<<i<<"  limit ="<<limit_228Th[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_228Ac+1; i++){
		//cout<<"Ac: i="<<i<<"  limit ="<<limit_228Ac[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_228Ra+1; i++){
		//cout<<"i="<<i<<"228Ra  limit ="<<limit_228Ra[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_224Ra+1; i++){
		//cout<<"i="<<i<<"224Ra  limit ="<<limit_224Ra[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_212Bi+1; i++){
		//cout<<"i="<<i<<"Bi  limit ="<<limit_212Bi[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_212Pb+1; i++){
		//cout<<"i="<<i<<"Pb  limit ="<<limit_212Pb[i]<<endl;
	}
	for(Int_t i=0; i<fnumGammas_208Tl+1; i++){
		//cout<<"i="<<i<<"  limit ="<<limit_208Tl[i]<<endl;
	}

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
  
	/*for (Int_t i=0; i<fnumGammas_228Th; i++){
		if (ran_228Th>limit_228Th[i] && ran_228Th<limit_228Th[i+1]){
			Gamma_228Th=i;
			ftrack_228Th=true;
			fanytrack=true;
			//cout<<"whichGamma="<<whichGamma<<"   up_lim[i]="<<up_limit[i]<<"   up_lim[i+1]="<<up_limit[i+1]<<endl;
		}
	}

	for (Int_t i=0; i<fnumGammas_228Ac; i++){
		if (ran_228Ac>limit_228Ac[i] && ran_228Ac<limit_228Ac[i+1]){
			Gamma_228Ac=i;
			ftrack_228Ac=true;
			fanytrack=true;
			//cout<<"whichGamma="<<whichGamma<<"   up_lim[i]="<<up_limit[i]<<"   up_lim[i+1]="<<up_limit[i+1]<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_228Ra; i++){
		if (ran_228Ra>limit_228Ra[i] && ran_228Ra<limit_228Ra[i+1]){
			Gamma_228Ra=i;
			ftrack_228Ra=true;
			fanytrack=true;
			//cout<<"whichGamma="<<whichGamma<<"   up_lim[i]="<<up_limit[i]<<"   up_lim[i+1]="<<up_limit[i+1]<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_224Ra; i++){
		if (ran_224Ra>limit_224Ra[i] && ran_224Ra<limit_224Ra[i+1]){
			Gamma_224Ra=i;
			ftrack_224Ra=true;
			fanytrack=true;
			//cout<<"whichGamma="<<whichGamma<<"   up_lim[i]="<<up_limit[i]<<"   up_lim[i+1]="<<up_limit[i+1]<<endl;
		}
	}
	for (Int_t i=0; i<fnumGammas_212Bi; i++){
		if (ran_212Bi>limit_212Bi[i] && ran_212Bi<limit_212Bi[i+1]){
			Gamma_212Bi=i;
			ftrack_212Bi=true;
			fanytrack=true;
			//cout<<"whichGamma="<<whichGamma<<"   up_lim[i]="<<up_limit[i]<<"   up_lim[i+1]="<<up_limit[i+1]<<endl;
		}
	}

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
	
	//cout<<endl<<endl<<endl;
	
	//cout<<"theta2="<<theta*180/TMath::Pi()<<"  phi2="<<phi*180/TMath::Pi()<<endl;
	//cout<<"theta3="<<TMath::ACos(1-2*gRandom->Rndm())<<"  phi3="<<6.283185307  *gRandom->Rndm()<<endl;

  //Direction of gamma RS Lab
  TVector3 direction;  
  direction = TVector3(TMath::Sin(theta)*TMath::Cos(phi),
			TMath::Sin(theta)*TMath::Sin(phi),
			TMath::Cos(theta));	

	//Momentum
	Double_t px, py, pz;
	px= fenergy[whichGamma]*direction.X();
  py= fenergy[whichGamma]*direction.Y();
  pz= fenergy[whichGamma]*direction.Z();  

	//cout<<"fenergy[whichGamma]="<<fenergy[whichGamma]<<endl;
	//cout<<"px="<<px<<"  py="<<py<<"  pz="<<pz<<endl;

	if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
			fZ = gRandom->Uniform(fZ1,fZ2);
	}

	//cout<<"fX="<<fX<<" fY="<<fY<<" fZ="<<fZ<<endl;

  //adding the gammas Th chain
  primGen->AddTrack(fPDGType, px, py, pz, fX, fY, fZ);
 
 */ 
	primGen->AddTrack(fPDGType, 0.2, 0.2, 0.2, 0., 0., 0.);
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
  fenergy_228Th		= new Double_t [fnumGammas_228Th];
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
	cout << "---------------------------------------------------------------" << endl;  
}


// --- Track Momentum Calculus --------------------------------------------
/*void EnsarThoriumChainGen::TrackMomentum( Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz ) {

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
}*/
//-------------------------------------------------------------------------

ClassImp(EnsarThoriumChainGen)
