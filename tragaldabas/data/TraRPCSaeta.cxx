// -------------------------------------------------------------------------
// -----                  TraRPCSaeeta source file                -----
// -------------------------------------------------------------------------

#include "TraRPCSaeta.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
TraRPCSaeta::TraRPCSaeta()
  : FairMultiLinkedData(),
  fX(-999.), fSlopeX(-999.), fY(-999.) , fSlopeY(-999.),
   fTime(-1.), fSlowness(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
TraRPCSaeta::TraRPCSaeta(Double_t x, Double_t slopex, Double_t y, Double_t slopey , Double_t time, Double_t slowness) 
  : FairMultiLinkedData(),
     fX(x), fSlopeX(slopex), fY(y), fSlopeY(slopey),
     fTime(time), fSlowness(slowness)
{
}
// -------------------------------------------------------------------------

TraRPCSaeta::TraRPCSaeta(const TraRPCSaeta& saeta)
  : FairMultiLinkedData(saeta),
    fX(saeta.fX), fSlopeX(saeta.fSlopeX), fY(saeta.fY), fSlopeY(saeta.fSlopeY),
    fTime(saeta.fTime), fSlowness(saeta.fSlowness)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
TraRPCSaeta::~TraRPCSaeta()
{
}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void TraRPCSaeta::Print(const Option_t* opt) const {
  cout << "-I- TraRPCSaeta: a RPC Saeta in RPC detector with " << endl;
  cout << "    Time =" << fTime << " ns  " << endl;
  cout << "    Slowness = " << fSlowness << " (arbitrary units)" << endl;
  cout << "    X = " << fX << " and X' = " << fSlopeX << endl;
  cout << "    Y = " << fY << " and Y' = " << fSlopeY << endl;
}
// -------------------------------------------------------------------------
