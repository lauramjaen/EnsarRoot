#ifndef PNDDSSDDETECTORHIT_H
#define PNDDSSDDETECTORHIT_H 1


//#include "TVector3.h"
#include "FairHit.h"

class TVector3;

class E105DSSDHit : public FairHit
{

  public:

    /** Default constructor **/
    E105DSSDHit();
    E105DSSDHit(Int_t detID, Int_t mcindex, TVector3& pos, TVector3& dpos);

    /** Destructor **/
    virtual ~E105DSSDHit();

  private:

    E105DSSDHit(const E105DSSDHit&);
    E105DSSDHit operator=(const E105DSSDHit&);

    ClassDef(E105DSSDHit,1);

};
#endif
