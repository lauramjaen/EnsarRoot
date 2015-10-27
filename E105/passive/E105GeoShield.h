#ifndef PNDGEOSHIELD_H
#define PNDGEOSHIELD_H

#include "FairGeoSet.h"

class  E105GeoShield : public FairGeoSet
{
  protected:
    char modName[2];  // name of module
    char eleName[2];  // substring for elements in module
  public:
    E105GeoShield();
    virtual ~E105GeoShield();
    const char* getModuleName(Int_t) {return modName;}
    const char* getEleName(Int_t) {return eleName;}
    Bool_t create(FairGeoBuilder*);
    ClassDef(E105GeoShield,0) // Class for geometry of beam pipe
};

#endif  /* !PNDGEOSHIELD_H */
