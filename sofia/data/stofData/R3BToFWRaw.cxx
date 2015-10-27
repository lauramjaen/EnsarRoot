// -------------------------------------------------------------------------
// -----                      R3BToFWRaw source file              -----
// -------------------------------------------------------------------------

#include "R3BToFWRaw.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
R3BToFWRaw::R3BToFWRaw() : FairMultiLinkedData() {

 for(Int_t i=0;i<ToFW_Nbplastics;i++){
 fToFW_up_Eraw[i]=0;
 fToFW_down_Eraw[i]=0;
 fToFW_up_Traw[i]=-1.;
 fToFW_down_Traw[i]=-1.;
 }
 
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BToFWRaw::R3BToFWRaw(UShort_t ToFW_up_Eraw[ToFW_Nbplastics], UShort_t ToFW_down_Eraw[ToFW_Nbplastics], Double_t ToFW_up_Traw[ToFW_Nbplastics], 
                   Double_t ToFW_down_Traw[ToFW_Nbplastics]) 
  : FairMultiLinkedData() {
  
 for(Int_t i=0;i<ToFW_Nbplastics;i++){
 fToFW_up_Eraw[i]=ToFW_up_Eraw[i];
 fToFW_down_Eraw[i]=ToFW_down_Eraw[i];
 fToFW_up_Traw[i]=ToFW_up_Traw[i];
 fToFW_down_Traw[i]=ToFW_down_Traw[i];
 }

}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
R3BToFWRaw::~R3BToFWRaw() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void R3BToFWRaw::Print(const Option_t* opt) const {
  cout << "-I- R3BToFWRaw:" << endl;
  for(Int_t i=0;i<ToFW_Nbplastics;i++){
  cout << "Charge up " << fToFW_up_Eraw[i] << endl;
  cout << "Charge down " << fToFW_down_Eraw[i] << endl;
  cout << "Time up = " << fToFW_up_Traw << endl;
  cout << "Time down " << fToFW_down_Traw[i] << endl;
}
}
// -------------------------------------------------------------------------
