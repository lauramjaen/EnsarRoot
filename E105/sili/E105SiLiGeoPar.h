#ifndef PNDSILIDETECTORGEOPAR_H
#define PNDSILIDETECTORGEOPAR_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class E105SiLiGeoPar       : public FairParGenericSet
{
  public:

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    E105SiLiGeoPar(const char* name="E105SiLiGeoPar",
                           const char* title="E105SiLi Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~E105SiLiGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    E105SiLiGeoPar(const E105SiLiGeoPar&);
    E105SiLiGeoPar& operator=(const E105SiLiGeoPar&);

    ClassDef(E105SiLiGeoPar,1)
};

#endif
