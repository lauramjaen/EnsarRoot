#include "E105DSSDHit.h"

E105DSSDHit::E105DSSDHit()
  : FairHit()
{
}

E105DSSDHit::E105DSSDHit(Int_t detID, Int_t mcindex, TVector3& pos, TVector3& dpos)
  : FairHit(detID, pos, dpos, mcindex)
{
}

E105DSSDHit::~E105DSSDHit()
{
}

ClassImp(E105DSSDHit)
