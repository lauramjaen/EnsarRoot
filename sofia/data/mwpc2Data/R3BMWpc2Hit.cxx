// -------------------------------------------------------------------------
// -----                      R3BMWpc2Hit source file                  -----
// -------------------------------------------------------------------------

#include "R3BMWpc2Hit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
R3BMWpc2Hit::R3BMWpc2Hit() : FairMultiLinkedData() {
  fmwpc2Type = fmwpc2Copy = fmwpc2Id = -1;
  fEnergy = fTime = -1; 
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BMWpc2Hit::R3BMWpc2Hit(Int_t type, Int_t copy, Int_t ident,Double_t energy, Double_t time) 
  : FairMultiLinkedData() {
  
  fmwpc2Id = ident;
  fmwpc2Type = type;
  fmwpc2Copy = copy;
  fEnergy = energy;
  fTime = time;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BMWpc2Hit::~R3BMWpc2Hit() { }
// -------------------------------------------------------------------------




// -----   Public method Print   -------------------------------------------
void R3BMWpc2Hit::Print(const Option_t* opt) const {
  cout << "-I- R3BMWpc2Hit: a MWpc mwpc2Hit in mwpc2 type " << fmwpc2Type 
       << " and copy  " << fmwpc2Copy << ". Uniqued mwpc2 identifier: " << fmwpc2Id << endl;
  cout << "    Energy = " << fEnergy << " GeV" << endl;
  cout << "    Time " << fTime << " ns  " << endl;
}
// -------------------------------------------------------------------------
