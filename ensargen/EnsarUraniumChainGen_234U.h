// *********************************************************************
// *****   
// *****             EnsarUraniumChainGen_234U header file         
// ***** 		     
// *****  	The generator creates the 234U->alpha + 230Th* and 
// ***** 		230Th*->230Th+gamma reactions
// ***** 		from the 238U chain of natural background	
// ***** 	
// ***** created by E.Galiana		
// ***** elisabet.galiana@usc.es eligaliana@lip.pt                             
// *****   							
// *********************************************************************



#ifndef ENSAR_URANIUMCHAIN_234U_H
#define ENSAR_URANIUMCHAIN_234U_H

#include "FairGenerator.h"
#include "TArrayF.h"

#include <iostream>
#include <fstream>


using namespace std;



class FairPrimaryGenerator;

class EnsarUraniumChainGen_234U : public FairGenerator
{
  public:

    /** Default constructor. **/
    EnsarUraniumChainGen_234U();

    /** Constructor.  **/
    EnsarUraniumChainGen_234U(const char* inputFile);

    /** Destructor. **/
    virtual ~EnsarUraniumChainGen_234U();
  
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

		Bool_t     fPointVtxIsSet;       // True if point vertex is set
    Bool_t     fBoxVtxIsSet;         // True if box vertex is set
		Bool_t 		 fThetaRangeIsSet;			//True if Theta range is set
		Bool_t		 fPhiRangeIsSet;				// True if Phi range is set
		Double_t fX, fY, fZ;           // Point vertex coordinates [cm]
    Double_t fX1, fY1, fZ1, fX2, fY2, fZ2;   // Box vertex coords (x1,y1,z1)->(x2,y2,z2)
		Double_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]
		Double_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
    
   
    ClassDef(EnsarUraniumChainGen_234U,1);
    void CloseInput();
    void ReadParameters();
		void TrackMomentum(Double_t Energy, Double_t & Px, Double_t & Py, Double_t & Pz);
		TArrayF* Deexcitation(Int_t InicialState, Int_t & FinalState);
    

};


#endif
