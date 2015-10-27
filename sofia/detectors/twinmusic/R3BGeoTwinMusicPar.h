#ifndef R3BGeoTwinMusicPAR_H
#define R3BGeoTwinMusicPAR_H
 
#include "FairParGenericSet.h"

#include "TH1F.h"

class R3BGeoTwinMusicPar : public FairParGenericSet {
public:
  TObjArray            *fGeoSensNodes; // List of FairGeoNodes for sensitive volumes
  TObjArray            *fGeoPassNodes; // List of FairGeoNodes for sensitive volumes

  R3BGeoTwinMusicPar(const char* name="R3BGeoTwinMusicPar",
             const char* title="Twin Music Geometry Parameters",
             const char* context="TestDefaultContext");
  ~R3BGeoTwinMusicPar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  TObjArray             *GetGeoSensitiveNodes(){return fGeoSensNodes;}
  TObjArray             *GetGeoPassiveNodes(){return fGeoPassNodes;}

  ClassDef(R3BGeoTwinMusicPar,1)
};

#endif /* !R3BGeoTwinMusicPAR_H */
