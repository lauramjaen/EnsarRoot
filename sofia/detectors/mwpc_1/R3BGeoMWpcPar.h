#ifndef R3BGEOMWPCPAR_H
#define R3BGEOMWPCPAR_H
 
#include "FairParGenericSet.h"

#include "TH1F.h"

class R3BGeoMWpcPar : public FairParGenericSet {
public:
  TObjArray            *fGeoSensNodes; // List of FairGeoNodes for sensitive volumes
  TObjArray            *fGeoPassNodes; // List of FairGeoNodes for sensitive volumes

  R3BGeoMWpcPar(const char* name="R3BGeoMWpcPar",
             const char* title="MWPC Geometry Parameters",
             const char* context="TestDefaultContext");
  ~R3BGeoMWpcPar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  TObjArray             *GetGeoSensitiveNodes(){return fGeoSensNodes;}
  TObjArray             *GetGeoPassiveNodes(){return fGeoPassNodes;}

  ClassDef(R3BGeoMWpcPar,1)
};

#endif /* !R3BGEOMWPCPAR_H */
