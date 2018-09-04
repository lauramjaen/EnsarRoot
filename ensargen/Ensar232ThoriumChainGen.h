// *********************************************************************
// *****             EnsarThoriumBackgroundGen header file              
// *****   						
// *****      Gamma Thorium Background Generator                     	 
// ***** 							
// ***** 	  This Generator reproduce different gammas which 
// *****		have come from 232Th-chain	
// ***** 			
// ***** elisabet.galiana@usc.es
// ***** Universidad de Santiago de Compostela                    
// ***** Dpto. Física de Partículas                               
// *****   							
// *********************************************************************


#ifndef ENSAR_232THORIUMCHAINGENERATOR_H
#define ENSAR_232THORIUMCHAINGENERATOR_H

#include "FairGenerator.h"

#include <iostream>
#include <fstream>


using namespace std;



class FairPrimaryGenerator;

class Ensar232ThoriumChainGen : public FairGenerator
{
  public:

    /** Default constructor. **/
    Ensar232ThoriumChainGen();

    /** Constructor.  **/
    Ensar232ThoriumChainGen(const char* inputFile);

    /** Destructor. **/
    virtual ~Ensar232ThoriumChainGen();
  
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

    void SetBoxXYZ (Double_t x1=0, Double_t y1=0, Double_t x2=0, Double_t y2=0, Double_t z=0) {
      fX1=x1;
      fY1=y1;
      fX2=x2;
      fY2=y2;
      fZ=z;
      fBoxVtxIsSet=kTRUE;
    }

		void SetThetaRange   (Double32_t thetamin=0, Double32_t thetamax=90)
    {fThetaMin=thetamin; fThetaMax=thetamax; fThetaRangeIsSet=kTRUE;};

		void SetPhiRange     (Double32_t phimin=0  , Double32_t phimax=360)
    {fPhiMin=phimin; fPhiMax=phimax; fPhiRangeIsSet=kTRUE;};

    
 private:

		ifstream*     fInputFile; //! Input file stream
    const Char_t* fFileName; 	//! Input file Name
		Int_t fnumGammas;				//total number of gammas in the chain
    Double_t* fenergy;			//Energy of gammas
    Double_t* fprobability;	//Probability of each gamma

		Bool_t     fPointVtxIsSet;       // True if point vertex is set
    Bool_t     fBoxVtxIsSet;         // True if box vertex is set
		Bool_t 		 fThetaRangeIsSet;			//True if Theta range is set
		Bool_t		 fPhiRangeIsSet;				// True if Phi range is set
		Double_t fX, fY, fZ;           // Point vertex coordinates [cm]
    Double_t fX1, fY1, fX2, fY2;   // Box vertex coords (x1,y1)->(x2,y2)
		Double_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]
		Double_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
    
   
    ClassDef(Ensar232ThoriumChainGen,2);
    void CloseInput();
    void ReadParameters();
    
    
};


#endif
