#ifndef ESRGenerator_h
#define ESRGenerator_h 1

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TGenPhaseSpace.h"
#include "TVector3.h"
#include "TRandom.h"
#include <iostream>
#include <fstream>

typedef struct {
  TString  name;
  Int_t  pdg;   // pdg code (for ions -1)
  UInt_t z;     // z number
  UInt_t a;     // atomic number 
  Int_t q;      // charge
  Double_t m;   // mass 
  Double_t e;   // excitation energy
}Part_t;

class ESRGenerator
{
public:
  ESRGenerator();
  ~ESRGenerator();

  // define particles
  Part_t *PDefinition(TString name, Int_t pdg, 
		     UInt_t  z, UInt_t  a, Int_t q, Double_t m, Double_t e); 
  void incomingChannel(Part_t *beam, Part_t *target, Double_t beamKinEnergy);
  void outgoingChannel(Part_t *ePart,  Part_t *pPart,
		       Part_t *hfPart, Part_t *lfPart);
  void outgoingChannel(Part_t *ePart, Part_t *pPart);
  void decayChannel(TLorentzVector *pm);
  void printToASCIFile(Int_t eId, Int_t nTracks);
  Bool_t correctAcceptReject();
  void genbod();

  // set methods
  void setVertexPosition(Double_t vx=0, Double_t vy=0, Double_t vz=0);
  void setInteractionProfileWidth(Double_t sx=0, Double_t sy=0, Double_t sz=0);
  void setDecayFlag(TString flag="off") {fDecayFlag=flag;}
  void setNEvents(Int_t n) {nEvents=n;}
  void setASCIFileName(TString FName) {fFileName=FName;}
  void setLimitsFlag(TString flag="off") {limitsFlag=flag;}
  void setEjectileLimits(Double_t minThe, Double_t maxThe, 
			 Double_t minPhi, Double_t maxPhi);

  // get methods
  TString getDecayFlag() {return fDecayFlag;}
  void getVertexPosition();
  TString getASCIFileName() {return fFileName;}

private:            
  Int_t nEvents;
  TString fFileName;

  // Interaction profile characteristics
  Double_t fVX;
  Double_t fVY;
  Double_t fVZ;
  Double_t fSX;
  Double_t fSY;
  Double_t fSZ;

  TString fDecayFlag;

  TGenPhaseSpace *event1;
  TGenPhaseSpace *event2;
  TLorentzVector W1;
  TLorentzVector beam;
  Double_t mass2[2];

  TRandom* posGen;
 
  TLorentzVector *pPar[4]; 
  Part_t *part[4];

  ofstream outFile;

  TVector3  vertexPos;

  Double_t minTheta;
  Double_t maxTheta;
  Double_t minPhi;
  Double_t maxPhi;
  TString limitsFlag;

  ClassDef(ESRGenerator,1) //ROOT CINT
};
#endif
