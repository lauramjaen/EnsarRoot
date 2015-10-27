#ifndef SLIT_H
#define SLIT_H

#include "TNamed.h"
#include "TArrayI.h"
#include "TClonesArray.h"
#include "FairDetector.h"
#include "FairModule.h"


class E105Slit : public FairModule
{
  public:
    E105Slit(const char* name, const char* Title="E105 Slit");
    E105Slit();

    virtual ~E105Slit();
    virtual void ConstructGeometry();
    void ConstructDetails(TGeoVolume* slit);

    ClassDef(E105Slit,1) //PNDSLIT

};

#endif //SLIT_H

