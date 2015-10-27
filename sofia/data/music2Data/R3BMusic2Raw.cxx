// -------------------------------------------------------------------------
// -----                      R3BMusic2Raw source file              -----
// -------------------------------------------------------------------------

#include "R3BMusic2Raw.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BMusic2Raw::R3BMusic2Raw() : FairMultiLinkedData() {
 for(Int_t i=0;i<Music2_NbAnodes;i++){
 fCharge[i]=-1.0;
 }
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BMusic2Raw::R3BMusic2Raw(Double_t Charge[Music2_NbAnodes]) 
  : FairMultiLinkedData() {
  for(Int_t i=0;i<Music2_NbAnodes;i++){
 fCharge[i]=Charge[i];
 }
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
R3BMusic2Raw::~R3BMusic2Raw() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void R3BMusic2Raw::Print(const Option_t* opt) const {
  cout << "-I- R3BMusic2Raw:" << endl;
  for(Int_t i=0;i<Music2_NbAnodes;i++){
  cout << "Anode " << i << " Charge " << fCharge[i] << endl;
}
}
// -------------------------------------------------------------------------
