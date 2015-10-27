// -------------------------------------------------------------------------
// -----                  TraRPCHit source file                -----
// -------------------------------------------------------------------------

#include "TraRPCHit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
TraRPCHit::TraRPCHit()
  : FairMultiLinkedData(),
    fCharge(-1.), fTime(-1.), fRPCId(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
TraRPCHit::TraRPCHit(Int_t id, Double_t charge, Double_t time) 
  : FairMultiLinkedData(),
    fCharge(charge), fTime(time), fRPCId(id)
{
}
// -------------------------------------------------------------------------

TraRPCHit::TraRPCHit(const TraRPCHit& right)
  : FairMultiLinkedData(right),
    fCharge(right.fCharge), fTime(right.fTime), fRPCId(right.fRPCId)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
TraRPCHit::~TraRPCHit()
{
}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void TraRPCHit::Print(const Option_t* opt) const {
  cout << "-I- TraRPCHit: a calo RPCHit in RPC identifier " << fRPCId << endl;
  cout << "    Charge = " << fCharge << " (arbitrary units)" << endl;
  cout << "    Time =" << fTime << " ns  " << endl;
}
// -------------------------------------------------------------------------
