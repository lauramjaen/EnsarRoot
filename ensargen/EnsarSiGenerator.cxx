// ************************************************************************
// *****             EnsarSiGenerator source file                     *****
// ***** 												              *****
// *****      Gamma Generator of different Si* cascades 	          *****
// *****      with Angular Correlations and Branching Rations		  *****
// ***** 	  from primary reaction  Al+p-> Si*-> Si+gammas           *****
// ***** 												              *****
// ***** 	  Works with the Si_12643_cascade.dat data file	    	  *****
// ***** 	  or any file with the same data structure		    	  *****
// ***** 												              *****
// ***** elisabet.galiana@usc.es				                      *****
// ***** Universidad de Santiago de Compostela                        *****
// ***** Dpto. Física de Partículas                                   *****
// ***** 												              *****
// ************************************************************************

#include "EnsarSiGenerator.h"

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

// -----   Default constructor   ------------------------------------------
EnsarSiGenerator::EnsarSiGenerator()  :
  FairGenerator() 
  {
  }
// ------------------------------------------------------------------------


// -----   Standard constructor   -----------------------------------------
EnsarSiGenerator::EnsarSiGenerator(const char* inputFile, Int_t state) :
  FairGenerator() 
{


 // fInputFile(NULL), fFileName(finputFile)
  cout << "-I- EnsarSiGenerator: Opening input file " << inputFile << endl;
  fFileName  = inputFile;

  fInputFile = new ifstream(fFileName);
  //fInputFile = new TFile(fFileName,"READ");
  if ( ! fInputFile->is_open() ) Fatal("EnsarSiGenerator","Cannot open input file.");

    
  //Read Si* Parameters File
  ReadParameters();
}
// ------------------------------------------------------------------------


// -----   Inizialize generator   -----------------------------------------
Bool_t  EnsarSiGenerator::Init()
{
  // not finished!
  // if (state>2 || state<0) {
  //  Fatal("Init()","EnsarSiGenerator: Invalid number of Si* state");
  //}

  // Condition energy of proton (E_p) can't be higher than a limit (Elimit=10GeV) and have to be positive
  //if (Ep>Elimit || Ep<0)
    //Fatal("Init()","EnsarBoxGenerator: Proton energy range not valid");
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarSiGenerator::~EnsarSiGenerator() {
  CloseInput();
}
// ------------------------------------------------------------------------

// -----   Read events   --------------------------------------------------
Bool_t EnsarSiGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{


	 //----- Check for input file -----
	 if ( ! fInputFile->is_open() ) {
	    cout << "-E- EnsarSiGenerator: Input file not open!" << endl;
	    return kFALSE;
	  }


	//Cascade Generation of 12643 Si* state 	
	Double_t Egamma1=0.;
	Double_t Egamma2=0.;
	Double_t Egamma3=0.;
	 
	Double_t A2=0.;
	Double_t A4=0.;
	Double_t A6=0.;
	Double_t A8=0.;
	 
	Int_t i=0;


	Float_t ran =gRandom->Rndm();
	//cout<<"ran= "<<ran<<endl;
	
	Double_t limit1=0.;
	Double_t limit2=0.;
	Double_t l2=0.;
	Double_t limit3=0.;
	Double_t l3=0.;
	Double_t limit4=0.;
	Double_t l4=0.;
	
	limit1=fBR[0];
	l2=fBR[1];
	limit2=limit1 + l2;  
	l3=fBR[2];
	limit3=limit2 + l3;
	l4=fBR[3];
	limit4=limit3 + l4;
	//cout<< "limit 1,2,3,4= "<<limit1<<", "<<limit2<<", "<<limit3<<", "<<limit4<<endl;
	
	
		if(ran<=limit1){ //Cascade A
			  i=0;
			  Egamma1=fener_gamma[i];
			  Egamma2=fener_f[i];
			  A2=fA[i][i];
			  A4=fA[i][i+1];
			  //cout<<"i= "<<i<<endl;
			  //cout<<"Egamma1= "<<Egamma1<<endl;
			  //cout<<"Egamma2= "<<Egamma2<<endl;
			  //cout<<"A2= "<<A2<<" A4= "<<A4<<endl;
		}	  
		else if (ran>limit1 && ran<=limit2){ //Cascade B
			  i=1;
			  Egamma1=fener_gamma[i];
			  Egamma2=fener_f[i];
			  A2=fA[i][0];
			  A4=fA[i][1];
			  //cout<<"i= "<<i<<endl;
			  //cout<<"Egamma1= "<<Egamma1<<endl;
			  //cout<<"Egamma2= "<<Egamma2<<endl;
			  //cout<<"A2= "<<A2<<" A4= "<<A4<<endl;
		}
		else if (ran>limit2 && ran<=limit3){ //Cascade C
			  i=2;
			  Egamma1=fener_gamma[i];
			  Egamma2=fener_f[i];
			  A2=fA[i][0];
			  A4=fA[i][1];
			  //cout<<"i= "<<i<<endl;
			  //cout<<"Egamma1= "<<Egamma1<<endl;
			  //cout<<"Egamma2= "<<Egamma2<<endl;
			  //cout<<"A2= "<<A2<<" A4= "<<A4<<endl;
		}
		else if (ran>limit3 && ran<=limit4){ //Cascade D and E
	
			  i=3;
			  //cout<<"i= "<<i<<endl;
			  Egamma1=fener_gamma[i];
			  Egamma2=fener_gamma[i+1];
			  Egamma3=fener_f[i+1];
			  A2=fA[i][0];
			  A4=fA[i][1];
			  A6=fA[i+1][0];
			  A8=fA[i+1][1];
			  //cout<<"Egamma1= "<<Egamma1<<endl;
			  //cout<<"Egamma2= "<<Egamma2<<endl;
			  //cout<<"Egamma3= "<<Egamma3<<endl;
			  //cout<<"A2= "<<A2<<" A4= "<<A4<<endl;
			  //cout<<"A6= "<<A6<<" A8= "<<A8<<endl;
		}
		  

		//--------------------------------------------------------------
	  
		 

		//Casacade with 2 gammas and the first 2 gammas of 3-gammas cascade--------------------------
	   
	  	//Gamma angles
		Double_t phi1=0.;
		Double_t phi2_p=0.;
		Double_t phi1_ch=0.;
		Double_t theta1=0.;
		Double_t theta2_p=0.;
		Double_t R=0;
		R=gRandom->Rndm();
	
	
		//if (R>0){  
		 
		//Angles in the SR of Lab(1) and SR of first gamma emmited(2) 
		/*RS Lab*/ 
		phi1 = 6.283185307  *gRandom->Rndm();//0-2*pi 
		theta1    = TMath::ACos(1-2*gRandom->Rndm());//0-pi
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
	
		  
	  	/*if(i==3){//the third gamma of 3-gammas Cascade--------------------------
	  
	  		//Gamma angles
			Double_t phi3=0.;
			Double_t theta3=0.;
			Double_t theta_add3=0.;
			Double_t theta_Rdm3=0.;
		  
		 	phi3 = 6.283185307  *gRandom->Rndm();
		 	  
		 	theta_Rdm3 = TMath::ACos(1-2*gRandom->Rndm());
			theta_add3 = 1 + A6*pow((TMath::Cos(theta_Rdm3)),2) +A8*pow((TMath::Cos(theta_Rdm3)),4);
			theta3    = theta2_p + theta_add3;
			  
			//Direction of gammas
			TVector3 direction3;
		  	direction3 = TVector3(TMath::Sin(theta3)*TMath::Cos(phi3),
				       TMath::Sin(theta3)*TMath::Sin(phi3),
				       TMath::Cos(theta3));
				       
			//Momentum of gamma
			Double_t px3=0.;
			Double_t py3=0.;
			Double_t pz3=0.;

			px3= Egamma3*direction3.X();
			py3= Egamma3*direction3.Y();
			pz3= Egamma3*direction3.Z(); 
			  
			//adding the third gamma
			primGen->AddTrack(fPDGType, px3, py3, pz3, X, Y, Z);
	  
	  	  }*/
		//}
	
	

return kTRUE;
	
}
// ------------------------------------------------------------------------

// -----   Private method CloseInput   ------------------------------------
void EnsarSiGenerator::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
       cout << "-I- EnsarSiGenerator: Closing input file " 
	    << fFileName << endl;
       fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------

// ---Read Si* Parameters File --------------------------------------------
void EnsarSiGenerator::ReadParameters() {

	cout << "\n\n\tREAD PARAMETERS\n" << endl;

	//---- Si* state variabes definition -----
	Int_t Nstate=0; 							//Total number of Si* states
	Nstate=2;
	fstate		= new Int_t [Nstate];			//Number of each states
	fener		= new Double_t [Nstate];		//Energy of the states
	fprob 		= new Double_t [Nstate];		//Probability of the states
	fnLevel_tot = new Double_t [Nstate];		//Number of Levels
	string dummy;
	 
	 
	// ----- Reading the number, energy, probability of population and number of levels of each state
	for(int i = 0; i < 8; i++) {fInputFile->ignore(256,'\n');}
	for(int i = 0; i < Nstate; i++){         
         *fInputFile >> fstate[i]>> fener[i] >> fprob[i] >> fnLevel_tot[i];
         cout<<"State= "<<fstate[i]<<endl;
         cout<<"Energy of state= "<<fener[i]<<endl;
         cout<<"Probability of state= "<<fprob[i]<<endl;
         cout<<"Num of levels= "<<fnLevel_tot[i]<<endl;
    }
         
    Int_t nLevel;
    nLevel=fnLevel_tot[0];						//nLevel=5 for 1st state
    Int_t a;		
    a=2;	
    //def dynamic vectors	
    fener_i 	= new Double_t [nLevel];		//Initial energy of each level
	fener_f 	= new Double_t [nLevel];		//Final energy of each level
	fener_gamma = new Double_t [nLevel];		//Gamma energy
	fBR 		= new Double_t [nLevel];		//Branching Ratio
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
		for(int j = 0; j < 2; j++){ 
		    *fInputFile >> fA[i][j]; 
		    cout<<"A[i][j]= "<<fA[i][j]<<endl;
		}
    } 

}
//-------------------------------------------------------------------------

ClassImp(EnsarSiGenerator)
