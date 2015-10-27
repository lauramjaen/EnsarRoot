// -------------------------------------------------------------------------
// -----              EnsarAsciiIonGenerator header file               -----
// -----           Created 16.07.13 <pablo.cabanelas@usc.es>           -----
// -------------------------------------------------------------------------

/** EnsarAsciiIonGenerator
**/


#ifndef ENSARASCIIIONGENERATOR_H
#define ENSARASCIIIONGENERATOR_H 1


#include "FairGenerator.h"

#include <fstream>
#include <map>
#include <fstream>

using namespace std;


class TDatabasePDG;
class FairPrimaryGenerator;
class FairIon;

class EnsarAsciiIonGenerator : public FairGenerator  
{

 public: 

  /** Default constructor without arguments should not be used. **/
  EnsarAsciiIonGenerator();


  /** Standard constructor. 
   ** @param fileName The input file name
   **/
  EnsarAsciiIonGenerator(const char* fileName);


  /** Destructor. **/
  virtual ~EnsarAsciiIonGenerator();

	
  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
   ** @param primGen  pointer to the FairPrimaryGenerator
   **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

 void SetXYZ   (Double32_t x=0, Double32_t y=0, Double32_t z=0) {
      fX=x;
      fY=y;
      fZ=z;
      fPointVtxIsSet=kTRUE;
   }

 void SetDxDyDz(Double32_t sx=0, Double32_t sy=0, Double32_t sz=0) {
      fDX=sx;
      fDY=sy;
      fDZ=sz;
      fBoxVtxIsSet=kTRUE;

 } 

 private:

  ifstream*      fInputFile;          //! Input file stream
  const Char_t*  fFileName;           //! Input file Name
  TDatabasePDG*  fPDG;                //!  PDG database

	
  /** Private method CloseInput. Just for convenience. Closes the 
   ** input file properly. Called from destructor and from ReadEvent. **/
  void CloseInput();


  /** Private method RegisterIons. Goes through the input file and registers
   ** any ion needed. **/
  Int_t RegisterIons();



  /** STL map from ion name to FairIon **/
  std::map<TString, FairIon*> fIonMap;       //!
	
  Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]	
  Bool_t     fPointVtxIsSet;       // True if point vertex is set
  Double32_t fDX, fDY, fDZ;        // Point vertex coordinates [cm]	
  Bool_t     fBoxVtxIsSet;         // True if point vertex is set



  ClassDef(EnsarAsciiIonGenerator,1);

};

#endif
