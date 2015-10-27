// -------------------------------------------------------------------------
// -----                      R3BMusicRaw source file              -----
// -------------------------------------------------------------------------

#include "R3BMusicRaw.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BMusicRaw::R3BMusicRaw() : FairMultiLinkedData() {
 for(Int_t i=0;i<Music_NbAnodes;i++){
 fCharge[i]=-1;
 }
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BMusicRaw::R3BMusicRaw(Int_t Charge[Music_NbAnodes]) 
  : FairMultiLinkedData() {
  for(Int_t i=0;i<Music_NbAnodes;i++){
 fCharge[i]=Charge[i];
 }
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
R3BMusicRaw::~R3BMusicRaw() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void R3BMusicRaw::Print(const Option_t* opt) const {
  cout << "-I- R3BMusicRaw:" << endl;
  for(Int_t i=0;i<Music_NbAnodes;i++){
  cout << "Anode " << i << " Charge " << fCharge[i] << endl;
}
}
// -------------------------------------------------------------------------
