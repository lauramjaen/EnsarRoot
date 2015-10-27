// -------------------------------------------------------------------------
// -----                      R3BMWpcHit source file                   -----
// -------------------------------------------------------------------------

#include "R3BMWpcHit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
R3BMWpcHit::R3BMWpcHit() : FairMultiLinkedData() {
  fmwpc1Type = fmwpc1Copy = fmwpc1Id = -1;
  fEnergy = fTime = -1; 
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BMWpcHit::R3BMWpcHit(Int_t type, Int_t copy, Int_t ident,Double_t energy, Double_t time) 
  : FairMultiLinkedData() {
  
  fmwpc1Id = ident;
  fmwpc1Type = type;
  fmwpc1Copy = copy;
  fEnergy = energy;
  fTime = time;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BMWpcHit::~R3BMWpcHit() { }
// -------------------------------------------------------------------------




// -----   Public method Print   -------------------------------------------
void R3BMWpcHit::Print(const Option_t* opt) const {
  cout << "-I- R3BMWpcHit: a MWPC mwpc1Hit in mwpc1 type " << fmwpc1Type 
       << " and copy  " << fmwpc1Copy << ". Uniqued mwpc1 identifier: " << fmwpc1Id << endl;
  cout << "    Energy = " << fEnergy << " GeV" << endl;
  cout << "    Time " << fTime << " ns  " << endl;
}
// -------------------------------------------------------------------------
