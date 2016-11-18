// -------------------------------------------------------------------------
// -----                  NaIHit source file                -----
// -------------------------------------------------------------------------

#include "NaIHit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
NaIHit::NaIHit()
  : FairMultiLinkedData(),
    fEnergy(-1.), fTime(-1.)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
NaIHit::NaIHit(Double_t energy, Double_t time) 
  : FairMultiLinkedData(),
    fEnergy(energy), fTime(time)
{
}
// -------------------------------------------------------------------------

NaIHit::NaIHit(const NaIHit& right)
  : FairMultiLinkedData(right),
    fEnergy(right.fEnergy), fTime(right.fTime)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
NaIHit::~NaIHit()
{
}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void NaIHit::Print(const Option_t* opt) const {
  cout << "-I- NaIHit: a Hit in the NaI detector with:" << endl;
  cout << "    Energy = " << fEnergy << " GeV " << endl;
  cout << "    Time =" << fTime << " ns  " << endl;
}
// -------------------------------------------------------------------------
