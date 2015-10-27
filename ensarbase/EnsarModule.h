#ifndef ENSARMODULE_H
#define ENSARMODULE_H 1


#include "FairModule.h"

#include "TObject.h"
#include "TVector3.h"
#include "TGeoMatrix.h"


class EnsarModule : public FairModule
{
 public:
  /** Default constructor **/
  EnsarModule();
  
  /** Constructor with arguments
   **/
  EnsarModule(const char* Name, const char* title, Bool_t Active=kFALSE);
  
  /** Destructor **/
  virtual ~EnsarModule();
  
  /** Transformation **/
  void SetRotAnglesEuler(Double_t phi, Double_t theta, Double_t psi);
  void SetRotAnglesXYZ(Double_t xx, Double_t yy, Double_t zz);
  void SetTranslation(Double_t tx, Double_t ty, Double_t tz);
  
  TGeoRotation* GetGlobalRot(){ return fGlobalRot;}
  TGeoTranslation* GetGlobalTrans(){ return fGlobalTrans;}
  TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef=NULL);
  
 private:
  EnsarModule(const EnsarModule&);
  EnsarModule& operator=(const EnsarModule&) { return *this; }
  
 protected:
  TGeoRotation *fGlobalRot;
  TGeoTranslation *fGlobalTrans;
  TGeoCombiTrans *fRefRot;
  
  Double_t fThetaX;
  Double_t fThetaY;
  Double_t fThetaZ;
  
  Bool_t fLabTrans;
  Bool_t fLocalTrans;
  
  Double_t fX,fY,fZ;
  Double_t fPhi,fTheta,fPsi;
  
  ClassDef(EnsarModule,1)
};

#endif
