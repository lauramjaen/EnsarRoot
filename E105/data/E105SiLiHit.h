#ifndef PNDSILIDETECTORHIT_H
#define PNDSILIDETECTORHIT_H 1


//#include "TVector3.h"
#include "FairHit.h"

class TVector3;

class E105SiLiHit : public FairHit
{

  public:

    /** Default constructor **/
    E105SiLiHit();
    E105SiLiHit(Int_t detID, Int_t mcindex, TVector3& pos, TVector3& dpos);

    /** Destructor **/
    virtual ~E105SiLiHit();

  private:

    E105SiLiHit(const E105SiLiHit&);
    E105SiLiHit operator=(const E105SiLiHit&);

    ClassDef(E105SiLiHit,1);

};
#endif
