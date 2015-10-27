#ifndef SHIELD_H
#define SHIELD_H

#include "TNamed.h"
#include "TArrayI.h"
#include "TClonesArray.h"
#include "FairDetector.h"
#include "FairModule.h"


class E105Shield : public FairModule
{
  public:
    E105Shield(const char* name, const char* Title="E105 Shield");
    E105Shield();

    virtual ~E105Shield();
    virtual void ConstructGeometry();

    ClassDef(E105Shield,1) //PNDSHIELD

};

#endif //SHIELD_H

