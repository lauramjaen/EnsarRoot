// *********************************************************************
// *****   
// *****             EnsarThoriumChainGen header file         
// ***** 		     
// *****  	The generator creates the 232Th chain of natural background
// ***** 		It reads the energies and the probabilities from a text
// *****		file. This file could be modified with different num of 
// ***** 		gammas, energies and probabilities, p.ex. 208Tldecay.txt		
// ***** 	
// ***** created by E.Galiana		
// ***** elisabet.galiana@usc.es eligaliana@lip.pt                             
// *****   							
// *********************************************************************


//#ifndef ENSAR_232THORIUMCHAINGENERATOR_H
//#define ENSAR_232THORIUMCHAINGENERATOR_H

#ifndef ENSAR_THORIUMCHAINGENERATOR_H
#define ENSAR_THORIUMCHAINGENERATOR_H

#include "FairGenerator.h"

#include <iostream>
#include <fstream>


using namespace std;



class FairPrimaryGenerator;

class EnsarThoriumChainGen : public FairGenerator
{
  public:

    /** Default constructor. **/
    EnsarThoriumChainGen();

    /** Constructor.  **/
    EnsarThoriumChainGen(const char* inputFile);

    /** Destructor. **/
    virtual ~EnsarThoriumChainGen();
  
    /** Initializer **/
    Bool_t Init();
    
    /** Creates an event **/
    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

		/** Accessor functions **/
		
    void SetXYZ   (Double_t x=0, Double_t y=0, Double_t z=0) {
      fX=x;
      fY=y;
      fZ=z;
      fPointVtxIsSet=kTRUE;
    }

    void SetBoxXYZ (Double_t x1=0, Double_t y1=0, Double_t z1=0, Double_t x2=0, Double_t y2=0, Double_t z2=0) {
      fX1=x1;
      fY1=y1;
      fZ1=z1;
      fX2=x2;
      fY2=y2;
      fZ2=z2;
      fBoxVtxIsSet=kTRUE;
    }

		void SetThetaRange   (Double32_t thetamin=0, Double32_t thetamax=90)
    {fThetaMin=thetamin; fThetaMax=thetamax; fThetaRangeIsSet=kTRUE;};

		void SetPhiRange     (Double32_t phimin=0  , Double32_t phimax=360)
    {fPhiMin=phimin; fPhiMax=phimax; fPhiRangeIsSet=kTRUE;};

    
 private:

		ifstream*     fInputFile; //! Input file stream
    const Char_t* fFileName; 	//! Input file Name

		Int_t fnumGammas_228Th;		//total number of gammas in each decay
		Int_t fnumGammas_228Ac;
		Int_t fnumGammas_228Ra;
		Int_t fnumGammas_224Ra;
		Int_t fnumGammas_212Bi;
		Int_t fnumGammas_212Pb;
		Int_t fnumGammas_208Tl;

    Double_t* fenergy_228Th;	//Energy of gammas
    Double_t* fenergy_228Ac;
    Double_t* fenergy_228Ra;
		Double_t* fenergy_224Ra;
    Double_t* fenergy_212Bi;
    Double_t* fenergy_212Pb;
    Double_t* fenergy_208Tl;

    Double_t* fprob_228Th;	//Probability of gammas
    Double_t* fprob_228Ac;
    Double_t* fprob_228Ra;
    Double_t* fprob_224Ra;
    Double_t* fprob_212Bi;
    Double_t* fprob_212Pb;
    Double_t* fprob_208Tl;

		Bool_t     fPointVtxIsSet;       // True if point vertex is set
    Bool_t     fBoxVtxIsSet;         // True if box vertex is set
		Bool_t 		 fThetaRangeIsSet;			//True if Theta range is set
		Bool_t		 fPhiRangeIsSet;				// True if Phi range is set
		Bool_t		 ftrack_228Th;
		Bool_t		 ftrack_228Ac;
		Bool_t		 ftrack_228Ra;
		Bool_t		 ftrack_224Ra;
		Bool_t		 ftrack_212Bi;
		Bool_t		 ftrack_212Pb;
		Bool_t		 ftrack_208Tl;
		Bool_t		 fanytrack;
		Double_t fX, fY, fZ;           // Point vertex coordinates [cm]
    Double_t fX1, fY1, fZ1, fX2, fY2, fZ2;   // Box vertex coords (x1,y1,z1)->(x2,y2,z2)
		Double_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]
		Double_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
    
   
    ClassDef(EnsarThoriumChainGen,1);
    void CloseInput();
    void ReadParameters();
    
    
};


#endif
