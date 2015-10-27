// -------------------------------------------------------------------------
// -----                      R3BStartFFRaw source file              -----
// -------------------------------------------------------------------------

#include "R3BStartFFRaw.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BStartFFRaw::R3BStartFFRaw() : FairMultiLinkedData() {
 for(Int_t i=0;i<StaFF_VFTX_N;i++){
 fTraw_right[i]=-1.;
 fTraw_left[i]=-1.;
 }
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BStartFFRaw::R3BStartFFRaw(Double_t Traw_right[StaFF_VFTX_N], Double_t Traw_left[StaFF_VFTX_N]) 
  : FairMultiLinkedData() {
 for(Int_t i=0;i<StaFF_VFTX_N;i++){
 fTraw_right[i]=Traw_right[i];
 fTraw_left[i]=Traw_left[i];
 }
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
R3BStartFFRaw::~R3BStartFFRaw() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void R3BStartFFRaw::Print(const Option_t* opt) const {
  cout << "-I- R3BStartFFRaw:" << endl;
  for(Int_t i=0;i<StaFF_VFTX_N;i++){
  cout << "Module " << i << endl;
  cout << "Time right " << fTraw_right[i] << endl;
  cout << "Time left " << fTraw_left[i] << endl;
}
}
// -------------------------------------------------------------------------
