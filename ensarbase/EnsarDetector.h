#ifndef ENSARDETECTOR_H
#define ENSARDETECTOR_H 1


#include "FairDetector.h"

#include "TObject.h"
#include "TVector3.h"
#include "TGeoMatrix.h"


class EnsarDetector : public FairDetector
{
 public:

  /** Default constructor **/
  EnsarDetector();


  /** Constructor with arguments
   **/
  EnsarDetector(const char* Name,  Bool_t Active , Int_t detId=0);

  /** Destructor **/
  virtual ~EnsarDetector();

  /** Transformation **/
  void SetRotAnglesEuler(Double_t phi, Double_t theta, Double_t psi);
  void SetRotAnglesXYZ(Double_t xx, Double_t yy, Double_t zz);
  void SetTranslation(Double_t tx, Double_t ty, Double_t tz);

  TGeoRotation* GetGlobalRot(){ return fGlobalRot;}
  TGeoTranslation* GetGlobalTrans(){ return fGlobalTrans;}
  TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef=NULL);

protected:
    TGeoRotation *fGlobalRot;
    TGeoCombiTrans *fRefRot;
    TGeoTranslation *fGlobalTrans;

    Double_t fThetaX;
    Double_t fThetaY;
    Double_t fThetaZ;

    Bool_t fLabTrans;
    Bool_t fLocalTrans;

    Double_t fX,fY,fZ;
    Double_t fPhi,fTheta,fPsi;


  ClassDef(EnsarDetector,1)

};

#endif
