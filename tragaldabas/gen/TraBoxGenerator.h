// -------------------------------------------------------------------------
// -----          TraBoxGenerator source file                   -----
// -----          Created 19/06/15  by Javier Diaz				   -----
// -------------------------------------------------------------------------

/**  TraBoxGenerator.h 
 *
 * The TraBoxGenerator generates particle for the Tragaldabas study. 
 * Copies from FairBoxGenerator.
 * Derived from FairGenerator.
**/

#ifndef FAIR_TRABOXGENERATOR_H
#define FAIR_TRABOXGENERATOR_H

#include "FairGenerator.h"

#include <iostream>
#include "Rtypes.h"                     // for Double32_t, Bool_t, kTRUE, etc



class FairPrimaryGenerator;

class TraBoxGenerator : public FairGenerator
{
public:

  /** Default constructor. **/
  TraBoxGenerator();

  /** Constructor with PDG-ID, multiplicity
   **@param pdgid Particle type (PDG encoding)
   **@param mult  Multiplicity (default is 1)
   **/
  TraBoxGenerator(Int_t pdgid, Int_t mult=1);

  /** Destructor **/
  virtual ~TraBoxGenerator() {};

  /** Modifiers **/
  void SetPDGType      (Int_t pdg)  {fPDGType = pdg;  };
	
  void SetMultiplicity (Int_t mult) {fMult    = mult; };
	
  void SetPRange(Double32_t pmin   , Double32_t pmax)
    {fPMin=pmin; fPMax=pmax; fPRangeIsSet=kTRUE;}
	
  void SetPtRange      (Double32_t ptmin   , Double32_t ptmax)
    {fPtMin=ptmin; fPtMax=ptmax; fPtRangeIsSet=kTRUE;};
	
  void SetPhiRange     (Double32_t phimin=0  , Double32_t phimax=360)
    {fPhiMin=phimin; fPhiMax=phimax;};
	
  void SetEtaRange     (Double32_t etamin=-5 , Double32_t etamax=7)
    {fEtaMin=etamin; fEtaMax=etamax; fEtaRangeIsSet=kTRUE;};
	
  void SetYRange       (Double32_t ymin=-5   , Double32_t ymax=7)
    {fYMin=ymin; fYMax=ymax; fYRangeIsSet=kTRUE;};
	
  void SetThetaRange   (Double32_t thetamin=0, Double32_t thetamax=90)
    {fThetaMin=thetamin; fThetaMax=thetamax; fThetaRangeIsSet=kTRUE;};
	
  void SetCosTheta   ()
    {fCosThetaIsSet=kTRUE;};
	
  void SetXYZ   () {
		fX=xgen; fY=ygen; fZ=zgen; fPointVtxIsSet=kTRUE;}
	
  void SetBoxXYZ (Double32_t x1=0, Double32_t y1=0, Double32_t z1=0, Double32_t x2=0, Double32_t y2=0, Double32_t z2=0) {
		fX1=x1; fY1=y1; fZ1=z1; fX2=x2; fY2=y2; fZ2=z2; fBoxVtxIsSet=kTRUE;}
	
  void SetDebug(Bool_t debug=0) {fDebug = debug;}



  //Tragaldabas detector

  void SetXhRange       (Double32_t xmin=-140   , Double32_t xmax=140)
    {fxMin=xmin; fxMax=xmax; fXhRangeIsSet=kTRUE;};

  void SetYhRange       (Double32_t yhmin=-140 , Double32_t yhmax=140)
    {fyMin=yhmin; fyMax=yhmax; fYhRangeIsSet=kTRUE;};


	
  /** Initializer **/
  virtual Bool_t Init();

  /** Creates an event with given type and multiplicity.
   **@param primGen  pointer to the FairPrimaryGenerator
   **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

private:
  Int_t      fPDGType;             // Particle type (PDG encoding)
  Int_t      fMult;                // Multiplicity
	
  Double32_t fPDGMass;             // Particle mass [GeV]
  Double32_t fPtMin,  fPtMax;      // Transverse momentum range [GeV]
  Double32_t fPhiMin, fPhiMax;     // Azimuth angle range [degree]
  Double32_t fEtaMin, fEtaMax;     // Pseudorapidity range in lab system
  Double32_t fYMin,   fYMax;       // Rapidity range in lab system
  Double32_t fPMin,   fPMax;       // Momentum range in lab system  
  Double32_t fThetaMin, fThetaMax; // Polar angle range in lab system [degree]
  Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]
  Double32_t fX1, fY1, fZ1, fX2, fY2, fZ2;   // Box vertex coords (x1,y1,z1)->(x2,y2,z2)
	
  Bool_t     fEtaRangeIsSet;       // True if eta range is set
  Bool_t     fYRangeIsSet;         // True if rapidity range is set
  Bool_t     fThetaRangeIsSet;     // True if theta range is set
  Bool_t     fCosThetaIsSet;       // True if uniform distribution in 
								   //cos(theta) is set (default -> not set)
  Bool_t     fPtRangeIsSet;        // True if transverse momentum range is set
  Bool_t     fPRangeIsSet;         // True if abs.momentum range is set
  Bool_t     fPointVtxIsSet;       // True if point vertex is set
  Bool_t     fBoxVtxIsSet;         // True if box vertex is set
  Bool_t     fDebug;               // Debug switch

  //SPECIFIC FOR TRAGALDABAS FIRST ANALYSIS

  Double32_t fxMax, fxMin, fyMax, fyMin;            //x,y max and min from an RPC plane detector
  Bool_t     fXhRangeIsSet;        // True if X range is set
  Bool_t     fYhRangeIsSet;        // True if Y range is set
  Double32_t xgen, ygen, zgen;     //generation coordinates
  Double32_t xdown, ydown, zup, zdown, xup, yup;  //Random coordinates generated in planes


	
 
 

 ClassDef(TraBoxGenerator,1);

};


#endif
