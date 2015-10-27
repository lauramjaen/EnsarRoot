// -------------------------------------------------------------------------
// -----                      R3BTPCRaw source file              -----
// -------------------------------------------------------------------------

#include "R3BTPCRaw.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BTPCRaw::R3BTPCRaw() : FairMultiLinkedData() {
 for(Int_t i=0;i<TPC_NbDelayLines;i++){
 fTime_DelayLineRight[i]=-1;
 fTime_DelayLineLeft[i]=-1;
 fCharge_DelayLineRight[i]=-1;
 fCharge_DelayLineLeft[i]=-1;
 }
 for(Int_t i=0;i<TPC_NbAnodes;i++){
 fStatusAnode[i]=-1;
 fTimeAnode[i]=-1;
 fChargeAnode[i]=-1;
 }
}
// -------------------------------------------------------------------------


// -----   Standard constructor   ------------------------------------------
R3BTPCRaw::R3BTPCRaw(Int_t StatusAnode[TPC_NbAnodes], Int_t TimeAnode[TPC_NbAnodes], Int_t ChargeAnode[TPC_NbAnodes],
            Int_t Time_DelayLineRight[TPC_NbDelayLines], Int_t Charge_DelayLineRight[TPC_NbDelayLines], 
            Int_t Time_DelayLineLeft[TPC_NbDelayLines], Int_t Charge_DelayLineLeft[TPC_NbDelayLines]) 
  : FairMultiLinkedData() {
 for(Int_t i=0;i<TPC_NbDelayLines;i++){
 fTime_DelayLineRight[i]=Time_DelayLineRight[i];
 fTime_DelayLineLeft[i]=Time_DelayLineLeft[i];
 fCharge_DelayLineRight[i]=Charge_DelayLineRight[i];
 fCharge_DelayLineLeft[i]=Charge_DelayLineLeft[i];
 }
 for(Int_t i=0;i<TPC_NbAnodes;i++){
 fStatusAnode[i]=StatusAnode[i];
 fTimeAnode[i]=TimeAnode[i];
 fChargeAnode[i]=ChargeAnode[i];
 }
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
R3BTPCRaw::~R3BTPCRaw() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void R3BTPCRaw::Print(const Option_t* opt) const {
  cout << "-I- R3BTPCRaw:" << endl;
  for(Int_t i=0;i<TPC_NbAnodes;i++){
  cout << "Anode " << i << " Status " << fStatusAnode[i] << endl;
  cout << "Time " << fTimeAnode[i] << endl;
  cout << "Charge " << fChargeAnode[i] << endl;
  }
  for(Int_t i=0;i<TPC_NbDelayLines;i++){
  cout << "Delay line " << i << endl;
  cout << "Time delay right " << fTime_DelayLineRight[i] << endl;
  cout << "Charge delay right " << fCharge_DelayLineRight[i] << endl;
  cout << "Time delay left " << fTime_DelayLineLeft[i] << endl;
  cout << "Charge delay left " << fCharge_DelayLineLeft[i] << endl;
}
}
// -------------------------------------------------------------------------
