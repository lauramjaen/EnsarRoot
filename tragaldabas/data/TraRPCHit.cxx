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
    fCharge(-1.), fTime(-1.), fRPCId(-1), fPosXIn(0), fPosYIn(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
TraRPCHit::TraRPCHit(Int_t id, Double_t charge, Double_t time, Double_t posxin, Double_t posyin) 
  : FairMultiLinkedData(),
    fCharge(charge), fTime(time), fRPCId(id), fPosXIn(posxin), fPosYIn(posyin)
{
}
// -------------------------------------------------------------------------

TraRPCHit::TraRPCHit(const TraRPCHit& right)
  : FairMultiLinkedData(right),
    fCharge(right.fCharge), fTime(right.fTime), fRPCId(right.fRPCId), fPosXIn(right.fPosXIn), fPosYIn(right.fPosYIn)
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
