#ifndef PNDTORINODETECTORGEOPAR_H
#define PNDTORINODETECTORGEOPAR_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class E105DSSDGeoPar       : public FairParGenericSet
{
  public:

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    E105DSSDGeoPar(const char* name="E105DSSDGeoPar",
                           const char* title="E105DSSD Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~E105DSSDGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    E105DSSDGeoPar(const E105DSSDGeoPar&);
    E105DSSDGeoPar& operator=(const E105DSSDGeoPar&);

    ClassDef(E105DSSDGeoPar,1)
};

#endif
