#include "E105DSSD.h"

#include "E105DSSDPoint.h"
#include "E105DSSDGeo.h"
#include "E105DSSDGeoPar.h"

#include "FairVolume.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"
#include "FairRootManager.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairDetectorList.h"
#include "FairStack.h"

#include "TClonesArray.h"
#include "TVirtualMC.h"

#include "TGeoMCGeometry.h"
#include "TGeoManager.h"
#include "TGeoBBox.h"

#include <iostream>
using std::cout;
using std::endl;

E105DSSD::E105DSSD()
  : FairDetector("E105DSSD", kTRUE,kDSSD ),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fE105DSSDPointCollection(new TClonesArray("E105DSSDPoint"))
{
}

E105DSSD::E105DSSD(const char* name, Bool_t active)
  : FairDetector(name, active, kDSSD),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fE105DSSDPointCollection(new TClonesArray("E105DSSDPoint"))
{
}

E105DSSD::~E105DSSD()
{
  if (fE105DSSDPointCollection) {
    fE105DSSDPointCollection->Delete();
    delete fE105DSSDPointCollection;
  }
}

void E105DSSD::Initialize()
{
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  E105DSSDGeoPar* par=(E105DSSDGeoPar*)(rtdb->getContainer("E105DSSDGeoPar"));
}

Bool_t  E105DSSD::ProcessHits(FairVolume* vol)
{
  /** This method is called from the MC stepping */

  //Set parameters at entrance of volume. Reset ELoss.
  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPos);
    gMC->TrackMomentum(fMom);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Create E105DSSDPoint at exit of active volume
  if ( gMC->IsTrackExiting()    ||
       gMC->IsTrackStop()       ||
       gMC->IsTrackDisappeared()   ) {
    fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fVolumeID = vol->getMCid();
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
    if (fELoss == 0. ) { return kFALSE; }
    AddHit(fTrackID, fVolumeID, TVector3(fPos.X(),  fPos.Y(),  fPos.Z()),
           TVector3(fMom.Px(), fMom.Py(), fMom.Pz()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()), fTime, fLength,
           fELoss);

    // Increment number of E105DSSD det points in TParticle
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kDSSD);
  }

  return kTRUE;
}

void E105DSSD::EndOfEvent()
{

  fE105DSSDPointCollection->Clear();

}



void E105DSSD::Register()
{

  /** This will create a branch in the output tree called
      E105DSSDPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("E105DSSDPoint", "E105DSSD",
                                        fE105DSSDPointCollection, kTRUE);

}


TClonesArray* E105DSSD::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fE105DSSDPointCollection; }
  else { return NULL; }
}

void E105DSSD::Reset()
{
  fE105DSSDPointCollection->Clear();
}

void E105DSSD::ConstructGeometry()
{
  // medium 
  ////////////
  // Material: Silicon
  TGeoMedium * pMedSi=NULL;
  if (0) {//gGeoManager->GetMedium("Silicon") ){
    1;//    pMedSi=gGeoManager->GetMedium("Silicon");
  }else{
    Double_t a       = 28.090000;
    Double_t z       = 14.000000;
    Double_t density = 2.330000;
    Double_t radl    = 9.351106;
    Double_t absl    = 456.628489;
    TGeoMaterial* pMatSi = new TGeoMaterial("Silicon", a,z,density);

    // Medium: Silicon
    Int_t numed   = 3;  // medium number
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pMedSi = new TGeoMedium("Silicon", numed,pMatSi,par);
   }

  // Geometry description
  // DSSD
  /////////////////// 
  Double_t dx = 6.4;     // [cm]
  Double_t dy = 6.4;     // [cm]
  Double_t dz = 0.0285;  // [cm]
  TGeoShape *DSSDShape   = new TGeoBBox("DSSDShape" ,dx/2, dy/2, dz/2);
  TGeoVolume *DSSDVolume = new TGeoVolume("DSSDVolume",DSSDShape,pMedSi);

  AddSensitiveVolume(DSSDVolume);
  fNbOfSensitiveVol+=1;

  // global positioning
  /////////////////
  TGeoVolume* pWorld = gGeoManager->GetTopVolume();
  pWorld->SetVisLeaves(kTRUE);

  // global Positioning DSSD#1
  Double_t det1toTarget = 25.1;   // cm  distancd to target
  Double_t det1Rotation = 80.5;   // deg
  TVector3 vec1 = TVector3(0,0,det1toTarget);
  vec1.RotateY(det1Rotation*TMath::Pi()/180.);
   
  TGeoRotation *rot1 = new TGeoRotation();
  rot1->RotateX(0.);
  rot1->RotateY(det1Rotation);
  rot1->RotateZ(0.);
  TGeoCombiTrans *t1 = new TGeoCombiTrans(vec1.X(),vec1.Y(),vec1.Z(),rot1);
  
  pWorld->AddNode(DSSDVolume, 1, t1);
  
  // global Positioning DSSD#2
  Double_t det2toTarget = 35.4;   // cm  distancd to target
  Double_t det2Rotation = 32.5;   // deg
  TVector3 vec2 = TVector3(0,0,det2toTarget);
  vec2.RotateY(det2Rotation*TMath::Pi()/180.);
  
  TGeoRotation *rot2 = new TGeoRotation();
  rot2->RotateX(0.);
  rot2->RotateY(det2Rotation);
  rot2->RotateZ(0.);
  TGeoCombiTrans *t2 = new TGeoCombiTrans(vec2.X(),vec2.Y(),vec2.Z(),rot2);

  pWorld->AddNode(DSSDVolume, 2 , t2);
}

E105DSSDPoint* E105DSSD::AddHit(Int_t trackID, Int_t detID,
    TVector3 pos, TVector3 mom, TVector3 posOut,
    TVector3 momOut, Double_t time, Double_t length,
    Double_t eLoss)
{
  TClonesArray& clref = *fE105DSSDPointCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) E105DSSDPoint(trackID, detID, pos, mom,
         posOut, momOut, time, length, eLoss);
}



ClassImp(E105DSSD)
