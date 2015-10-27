#ifndef PNDGEOPASSIVEPAR_H
#define PNDGEOPASSIVEPAR_H

#include "FairParGenericSet.h"
#include "TH1F.h"

class E105GeoPassivePar : public FairParGenericSet
{
 public:
  TObjArray* fGeoSensNodes; // List of FairGeoNodes for sensitive volumes
  TObjArray* fGeoPassNodes; // List of FairGeoNodes for sensitive volumes

  E105GeoPassivePar(const char* name="E105GeoPassivePar",
                    const char* title="Passive Geometry Parameters",
                    const char* context="TestDefaultContext");
  ~E105GeoPassivePar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  TObjArray*             GetGeoSensitiveNodes() {return fGeoSensNodes;}
  TObjArray*             GetGeoPassiveNodes() {return fGeoPassNodes;}

 private:
  E105GeoPassivePar(const E105GeoPassivePar&);
  E105GeoPassivePar& operator=(const E105GeoPassivePar&);

  ClassDef(E105GeoPassivePar,1)
};

#endif /* !PNDGEOPASSIVEPAR_H */
