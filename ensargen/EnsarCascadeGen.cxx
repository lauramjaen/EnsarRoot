// *********************************************************************
// *****             EnsarCascadeGen source file                   *****
// *****   												           *****
// *****      Simply Gamma Cascade Generator  	                   *****
// *****      with Angular Correlations                       	   *****
// ***** 												           *****
// *****      Works with the data files:				    	   *****
// ***** 		 -Co_cascade.dat								   *****
// *****		 -Si_12900_cascade.dat		    				   *****
// ***** 		 -Gamma_AngularC_200k.dat						   *****
// ***** 	  or any file with the same data structure		       *****
// *****												           *****
// ***** elisabet.galiana@usc.es				                   *****
// ***** Universidad de Santiago de Compostela                     *****
// ***** Dpto. Física de Partículas                                *****
// *****   												           *****
// *********************************************************************

#include "EnsarCascadeGen.h"

#include "FairPrimaryGenerator.h"

#include "TRandom.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"
#include "TF1.h"
#include "Math/WrappedTF1.h"
#include "Math/BrentRootFinder.h"

#include <iostream>
#include <fstream>
#include <istream>
#include <string>

#define fPDGType 22

using namespace std;
using std::cout;
using std::endl;
using std::map;
using std::ifstream;

// -----   Default constructor   ------------------------------------------
EnsarCascadeGen::EnsarCascadeGen()  :
  FairGenerator() 
  {
  }
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
EnsarCascadeGen::EnsarCascadeGen(const char* inputFile, Int_t state) :
  FairGenerator() 
{


 // fInputFile(NULL), fFileName(finputFile)
  cout << "-I- EnsarCascadeGen: Opening input file " << inputFile << endl;
  fFileName  = inputFile;

  fInputFile = new ifstream(fFileName);
  //fInputFile = new TFile(fFileName,"READ");
  if ( ! fInputFile->is_open() ) Fatal("EnsarCascadeGen","Cannot open input file.");

    
  //Read Si* Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarCascadeGen::Init()
{

}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarCascadeGen::~EnsarCascadeGen() {
  CloseInput();
}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarCascadeGen::ReadEvent(FairPrimaryGenerator* primGen)
{


	 //----- Check for input file -----
	 if ( ! fInputFile->is_open() ) {
	    cout << "-E- EnsarCascadeGen: Input file not open!" << endl;
	    return kFALSE;
	  }


	//Cascade Generation of 12643 Si* state 	
	Double_t Egamma1=0.;
	Double_t Egamma2=0.;
	Double_t BR=0.;
	 
	Double_t A2=0.;
	Double_t A4=0.;
	 
	Int_t i=0;

	
	//Branching Ratio
	BR=fBR[i];
	
	// Energies of gammas 
	Egamma1=fener_gamma[i];
	Egamma2=fener_f[i];
	
	//Angular Coeficients A2 & A4
	A2=fA[i][i];
	A4=fA[i][i+1];
	
	//cout<<"Egamma1= "<<Egamma1<<endl;
	//cout<<"Egamma2= "<<Egamma2<<endl;
	//cout<<"A2= "<<A2<<" A4= "<<A4<<endl;

		  
	//--------------------------------------------------------------
	  

		//Casacade with 2 gammas --------------------------
	   
	  	//Gamma angles
		Double_t phi1=0.;
		Double_t phi2_p=0.;
		Double_t phi1_ch=0.;
		Double_t theta1=0.;
		Double_t theta2_p=0.;
		Double_t R=0;
		R=gRandom->Rndm();  
		 
		//Angles in the RS of Lab(1) and RS of first gamma emmited(2) 
		/*RS Lab*/ 
		phi1 = 6.283185307  *gRandom->Rndm();//0-2*pi 
		//theta1    = TMath::ACos(1-2*gRandom->Rndm());//0-pi
		phi1_ch = phi1 + TMath::Pi()/2.; //add pi/2 for the change of RS gamma to LabRS
	
		/*RS gamma*/
		phi2_p = 6.283185307  *gRandom->Rndm();//0-2*pi	
	
			//theta2_p created following Angular Correlation -->  W(theta) = 1 + A2*cos^2 (theta) + A4*cos^4 (theta)
			//													  p = W(theta)*sin(theta) impose theta isotropic
			//                                                    P = integrate (p) d(theta)
			//													  P' = P/normalization  --> P'(theta=0)=0 & P'(theta=pi)=1
			//													  random = P'(theta)     
			//													  [0 = P'(theta)-random]   find zeros of this final equation using a Numerical Method
			Double_t a;
		 	Double_t b;
		 	Double_t c;
		 	Double_t d;
		 	
		 	a = A2/3.;
		 	b = A4/5.;
		 	c = 1.+ A2/3.+A4/5.;
		 	d = 2.*c;
		 	
		 	Double_t Rd=0;
			Rd=gRandom->Rndm();
		
			// Create the function and wrap it
		 	TF1 *fa = new TF1("fa"," (-TMath::Cos(x) -[0]*pow((TMath::Cos(x)),3) -[1]*pow((TMath::Cos(x)),5) + [2])/[3] -[4]", 0., TMath::Pi() );// [0 = P'(theta)-random]
		 	fa->SetParameter(0,a);
			fa->SetParameter(1,b);
			fa->SetParameter(2,c);
			fa->SetParameter(3,d);
			fa->SetParameter(4,Rd);
		
			ROOT::Math::WrappedTF1 wf1(*fa);
		 
			// Create the Integrator
			ROOT::Math::BrentRootFinder brf;
		 
			// Set parameters of the method
			brf.SetFunction( wf1, 0., TMath::Pi() );
			brf.Solve();
			theta2_p=brf.Root();//0-pi
			//-------------------------------------------------------------------------------
		  
		//Direction of gammas
		TVector3 direction1;  /*RS Lab*/ 
		direction1 = TVector3(TMath::Sin(theta1)*TMath::Cos(phi1),
				       TMath::Sin(theta1)*TMath::Sin(phi1),
				       TMath::Cos(theta1));	         
		TVector3 direction2;  /*RS gamma*/  
		direction2 = TVector3(TMath::Sin(theta2_p)*TMath::Cos(phi2_p),
				       TMath::Sin(theta2_p)*TMath::Sin(phi2_p),
				       TMath::Cos(theta2_p));
		 
		//Momentum of gamma     LAB frame = CM frame
		//momentum module:  P^2 = E^2 - M^2 = (T + M)^2 - M^2 ===> P=E (M=0)
		Double_t px1=0.;
		Double_t py1=0.;
		Double_t pz1=0.;
		Double_t px2_p=0.;
		Double_t py2_p=0.;
		Double_t pz2_p=0.;
		Double_t px2=0.;
		Double_t py2=0.;
		Double_t pz2=0.;

		px1= Egamma1*direction1.X();/*RS Lab*/
		py1= Egamma1*direction1.Y();
		pz1= Egamma1*direction1.Z();  
		//cout << "Direction of gammas: " << px1 << " -- " << py1 << " -- " << pz1 << " for energy: " << Egamma1 << endl;
		   
		px2_p= Egamma2*direction2.X();/*RS gamma*/
		py2_p= Egamma2*direction2.Y();
		pz2_p= Egamma2*direction2.Z(); 
	
		/*RS Lab*/	
		px2 = TMath::Cos(phi1_ch)*px2_p - TMath::Cos(theta1)*TMath::Sin(phi1_ch)*py2_p + TMath::Sin(theta1)*TMath::Sin(phi1_ch)*pz2_p;//conversion from RSgamma to Lab using Euler Rotation matrix
		py2 = TMath::Sin(phi1_ch)*px2_p + TMath::Cos(theta1)*TMath::Cos(phi1_ch)*py2_p - TMath::Sin(theta1)*TMath::Cos(phi1_ch)*pz2_p;// imposing theta1, phi1_ch and psi=0
		pz2 = TMath::Sin(theta1)*py2_p + TMath::Cos(theta1)*pz2_p;
		 
		//cout<< "theta1= "<<theta1*180/TMath::Pi()<<" and phi1_ch= "<<phi1_ch*180/TMath::Pi()<<endl;
		//cout<< "theta2_p= "<<theta2_p*180/TMath::Pi()<<" and phi2_p= "<<phi2_p*180/TMath::Pi()<<endl;
		 
		//Gamma vertex in the origen 
		Double_t X=0.;
		Double_t Y=0.;
		Double_t Z=0.;
	  	  
	  	//adding the gammas of Si* cascade
		primGen->AddTrack(fPDGType, px1, py1, pz1, X, Y, Z);
		primGen->AddTrack(fPDGType, px2, py2, pz2, X, Y, Z);
				
		//TVector3 mom;
		//mom.SetXYZ(px1,py1,pz1);
		//cout << " Mom angles: " << mom.Theta() << " -- " << mom.Phi() << endl;
		
		delete fa;

return kTRUE;
	
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void EnsarCascadeGen::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
       cout << "-I- EnsarCascadeGen: Closing input file " 
	    << fFileName << endl;
       fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------

// ---Read Si* Parameters File --------------------------------------------
void EnsarCascadeGen::ReadParameters() {

	cout << "\n\n\tREAD PARAMETERS\n" << endl;
	
	//---- State variabes definition -----
	Int_t Nstate=0; 							//Total number of States
	Nstate=1;
	fener		= new Double_t [Nstate];		//Energy of the states
	fnLevel_tot = new Double_t [Nstate];		//Number of Levels
	
	// ----- Reading the energy, probability of population and number of levels of each state
	fInputFile->ignore(256,'\n');
	fInputFile->ignore(256,'\n');
	fInputFile->ignore(256,'\n');
	fInputFile->ignore(256,'\n');
	fInputFile->ignore(256,'\n');
	fInputFile->ignore(256,'\n');
	fInputFile->ignore(256,'\n');
	for(int i = 0; i < Nstate; i++){ 
		*fInputFile >> fener[i] >> fnLevel_tot[i];
		cout<<"Energy of state= "<<fener[i]<<endl;
   		cout<<"Num of levels= "<<fnLevel_tot[i]<<endl;
   	}
	
    Int_t nLevel;
    nLevel=fnLevel_tot[0];		//nLevel=1 for one cascade
    Int_t a;		
    a=2;	
    //def dynamic vectors	
    fener_i 	= new Double_t [nLevel];		//Initial energy of each level
	fener_f 	= new Double_t [nLevel];		//Final energy of each level
	fener_gamma = new Double_t [nLevel];		//Gamma energy
	fBR 		= new Double_t [nLevel];		//Branching Ratios
	//def dynamic matrix
	fA 	= new Double_t* [nLevel];				//Angular Coeficients A2 and A4
	for(Int_t i=0; i<nLevel; i++){
		fA[i]=new Double_t [a];
	}
	
	// ---- Reading the initial and final energies of each Level
	fInputFile->ignore(256,'\n');
	fInputFile->ignore(256,'\n');
	for(int i = 0; i < nLevel; i++){ 
	 	*fInputFile >> fener_i[i] >> fener_f[i];
	 	cout<<"E_inicial= "<<fener_i[i]<<endl;
	 	cout<<"E_final= "<<fener_f[i]<<endl;
    }
    
    // ---- Reading the energy of gamma emmited
    fInputFile->ignore(256,'\n');
    fInputFile->ignore(256,'\n');
    for(int i = 0; i < nLevel; i++){ 
	 	*fInputFile >> fener_gamma[i];
	 	cout<<"E_gamma= "<<fener_gamma[i]<<endl;
    }
    
    // ---- Reading Branching Ratios
    fInputFile->ignore(256,'\n');
    fInputFile->ignore(256,'\n');
    for(int i = 0; i < nLevel; i++){ 
	 	*fInputFile >> fBR[i];
	 	cout<<"BR[i]= "<<fBR[i]<<endl;
    }	
    
    // ---- Reading Angular Coeficients A2 and A4
    fInputFile->ignore(256,'\n');
    fInputFile->ignore(256,'\n');
    for(int i = 0; i < nLevel; i++){ 
		for(int j = 0; j < a; j++){ 
		    *fInputFile >> fA[i][j]; 
		    cout<<"A[i][j]= "<<fA[i][j]<<endl;
		}
    } 

}
//-------------------------------------------------------------------------

ClassImp(EnsarCascadeGen)
