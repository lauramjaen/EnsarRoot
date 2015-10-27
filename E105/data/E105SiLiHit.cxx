#include "E105SiLiHit.h"

E105SiLiHit::E105SiLiHit()
  : FairHit()
{
}

E105SiLiHit::E105SiLiHit(Int_t detID, Int_t mcindex, TVector3& pos, TVector3& dpos)
  : FairHit(detID, pos, dpos, mcindex)
{
}

E105SiLiHit::~E105SiLiHit()
{
}

ClassImp(E105SiLiHit)
