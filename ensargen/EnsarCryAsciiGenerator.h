// -------------------------------------------------------------------------
// -----              EnsarCryAsciiGenerator header file               -----
// -----           Created 06.03.17 <josejavier.cuenca@usc.es>         -----
// -------------------------------------------------------------------------

#ifndef ENSARCRYASCIIGENERATOR_H
#define ENSARCRYASCIIGENERATOR_H 1

#include "FairGenerator.h"

#include <map>
#include <fstream>

using namespace std;
using std::ifstream;

class TDatabasePDG;
class FairPrimaryGenerator;

class EnsarCryAsciiGenerator : public FairGenerator
{
 public:

  /** Default constructor without arguments should not be used. **/
  EnsarCryAsciiGenerator();

  /** Standard constructor. 
   ** @param fileName The input file name
   **/
  EnsarCryAsciiGenerator(const char* fileName);


  /** Destructor. **/
  virtual ~EnsarCryAsciiGenerator();


  /** Reads on event from the input file and pushes the tracks onto
   ** the stack. Abstract method in base class.
   ** @param primGen  pointer to the FairPrimaryGenerator
   **/
  virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);

  void SetXYZ   (Double32_t x=0, Double32_t y=0, Double32_t z=0) 
     {
      fX=x;
      fY=y;
      fZ=z;
      fPointVtxIsSet=kTRUE;
     }

  void SetDxDyDz(Double32_t sx=0, Double32_t sy=0, Double32_t sz=0) 
     {
      fDX=sx;
      fDY=sy;
      fDZ=sz;
      fBoxVtxIsSet=kTRUE;
     }

 private:

  ifstream*      fInputFile;          //! Input file stream
  const Char_t*  fFileName;           //! Input file Name
  TDatabasePDG*  fPDG;                //!  PDG database

  /** Private method CloseInput. Closes the input file properly. 
   ** Called from destructor and from ReadEvent. **/
  void CloseInput();

  Double32_t fX, fY, fZ;           // Point vertex coordinates [cm]     
  Bool_t     fPointVtxIsSet;       // True if point vertex is set
  Double32_t fDX, fDY, fDZ;        // Point vertex coordinates [cm]     
  Bool_t     fBoxVtxIsSet;         // True if point vertex is set


ClassDef(EnsarCryAsciiGenerator,1);

};

#endif
