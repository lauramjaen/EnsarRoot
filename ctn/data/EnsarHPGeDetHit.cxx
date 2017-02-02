// -------------------------------------------------------------------------
// -----                      EnsarHPGeDetHit source file                  -----
// -------------------------------------------------------------------------

#include "EnsarHPGeDetHit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
EnsarHPGeDetHit::EnsarHPGeDetHit() 
  : FairMultiLinkedData(),
  fEnergy(-1.), fTime(-1.) 
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
EnsarHPGeDetHit::EnsarHPGeDetHit(Double_t energy, Double_t time) 
  : FairMultiLinkedData(),
    fEnergy(energy), fTime(time)
{
}
// -------------------------------------------------------------------------

EnsarHPGeDetHit::EnsarHPGeDetHit(const EnsarHPGeDetHit& right)
  : FairMultiLinkedData(right),
    fEnergy(right.fEnergy), fTime(right.fTime)
{
}
//--------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
EnsarHPGeDetHit::~EnsarHPGeDetHit() 
{
}
// -------------------------------------------------------------------------




// -----   Public method Print   -------------------------------------------
void EnsarHPGeDetHit::Print(const Option_t* opt) const {
  cout << "-I- : EnsarHPGeDetHit a Hit in the HPGe detector with:" << endl;
  cout << "    Energy = " << fEnergy << " GeV " << endl;
  cout << "    Time =" << fTime << " ns  " << endl;
}
// -------------------------------------------------------------------------


ClassImp(EnsarHPGeDetHit)

