// -------------------------------------------------------------------------
// -----                      R3BTwinMusicRaw source file              -----
// -------------------------------------------------------------------------

#include "R3BTwinMusicRaw.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
R3BTwinMusicRaw::R3BTwinMusicRaw() : FairMultiLinkedData() {

 for(Int_t i=0;i<TwinMusic_NbAnodesPerSide;i++){
 fStatusAnodeRight[i]=-1;
 fStatusAnodeLeft[i]=-1;
 fDtimeRight[i]=0.;
 fDtimeLeft[i]=0.;
 fChargeRight[i]=0.;
 fChargeLeft[i]=0.;
 }
 
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BTwinMusicRaw::R3BTwinMusicRaw(Int_t StatusAnodeRight[TwinMusic_NbAnodesPerSide], Int_t StatusAnodeLeft[TwinMusic_NbAnodesPerSide], 
                                 Double_t DtimeRight[TwinMusic_NbAnodesPerSide], Double_t DtimeLeft[TwinMusic_NbAnodesPerSide], 
                                 Double_t ChargeRight[TwinMusic_NbAnodesPerSide], Double_t ChargeLeft[TwinMusic_NbAnodesPerSide]) 
  : FairMultiLinkedData() {
  
  for(Int_t i=0;i<TwinMusic_NbAnodesPerSide;i++){
 fStatusAnodeRight[i]=StatusAnodeRight[i];
 fStatusAnodeLeft[i]=StatusAnodeLeft[i];
 fDtimeRight[i]=DtimeRight[i];
 fDtimeLeft[i]=DtimeLeft[i];
 fChargeRight[i]=ChargeRight[i];
 fChargeLeft[i]=ChargeLeft[i];
 }

}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BTwinMusicRaw::~R3BTwinMusicRaw() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void R3BTwinMusicRaw::Print(const Option_t* opt) const {
  cout << "-I- R3BTwinMusicRaw:" << endl;
  for(Int_t i=0;i<TwinMusic_NbAnodesPerSide;i++){
  cout << "anode right " << i << " status " << fStatusAnodeRight[i] << endl;
  cout << "Drift time right= " << fDtimeRight[i] << endl;
  cout << "Charge right = " << fChargeRight[i] << endl;
  cout << "anode left " << i << " status " << fStatusAnodeLeft[i] << endl;
  cout << "Drift time left= " << fDtimeLeft[i] << endl;
  cout << "Charge left = " << fChargeLeft[i] << endl;
}
}
// -------------------------------------------------------------------------
