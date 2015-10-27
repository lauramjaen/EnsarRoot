#ifndef PNDGEOSLIT_H
#define PNDGEOSLIT_H

#include "FairGeoSet.h"

class  E105GeoSlit : public FairGeoSet
{
  protected:
    char modName[2];  // name of module
    char eleName[2];  // substring for elements in module
  public:
    E105GeoSlit();
    virtual ~E105GeoSlit();
    const char* getModuleName(Int_t) {return modName;}
    const char* getEleName(Int_t) {return eleName;}
    Bool_t create(FairGeoBuilder*);
    ClassDef(E105GeoSlit,0) // Class for geometry of beam pipe
};

#endif  /* !PNDGEOSLIT_H */
