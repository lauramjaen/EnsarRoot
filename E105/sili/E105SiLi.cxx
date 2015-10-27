#include "E105SiLi.h"

#include "E105SiLiPoint.h"
#include "E105SiLiGeo.h"
#include "E105SiLiGeoPar.h"

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

E105SiLi::E105SiLi()
  : FairDetector("E105SiLi", kTRUE,kSiLi ),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fE105SiLiPointCollection(new TClonesArray("E105SiLiPoint"))
{
}

E105SiLi::E105SiLi(const char* name, Bool_t active)
  : FairDetector(name, active, kSiLi),
    fTrackID(-1),
    fVolumeID(-1),
    fPos(),
    fMom(),
    fTime(-1.),
    fLength(-1.),
    fELoss(-1),
    fE105SiLiPointCollection(new TClonesArray("E105SiLiPoint"))
{
}

E105SiLi::~E105SiLi()
{
  if (fE105SiLiPointCollection) {
    fE105SiLiPointCollection->Delete();
    delete fE105SiLiPointCollection;
  }
}

void E105SiLi::Initialize()
{
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  E105SiLiGeoPar* par=(E105SiLiGeoPar*)(rtdb->getContainer("E105SiLiGeoPar"));
}

Bool_t  E105SiLi::ProcessHits(FairVolume* vol)
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

  // Create E105SiLiPoint at exit of active volume
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

    // Increment number of E105SiLi det points in TParticle
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kSiLi);
  }

  return kTRUE;
}

void E105SiLi::EndOfEvent()
{

  fE105SiLiPointCollection->Clear();

}



void E105SiLi::Register()
{

  /** This will create a branch in the output tree called
      E105SiLiPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("E105SiLiPoint", "E105SiLi",
                                        fE105SiLiPointCollection, kTRUE);

}


TClonesArray* E105SiLi::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fE105SiLiPointCollection; }
  else { return NULL; }
}

void E105SiLi::Reset()
{
  fE105SiLiPointCollection->Clear();
}

void E105SiLi::ConstructGeometry()
{
  // medium 
  ////////////

  Double_t density =        0.;
  Double_t a       =        0.;
  Double_t z       =        0.;
  Double_t radl    =        0.;
  Double_t absl    =        0.;
  Int_t numed      =        0 ;
  Double_t par[8];
  // Material: Silicon
  TGeoMedium * pMedSi=NULL;
  if (gGeoManager->GetMedium("Silicon") ){
    pMedSi=gGeoManager->GetMedium("Silicon");
  }else{
    a       = 28.090000;
    z       = 14.000000;
    density = 2.330000;
    radl    = 9.351106;
    absl    = 456.628489;
    TGeoMaterial* pMatSi = new TGeoMaterial("Silicon", a,z,density);

    // Medium: Silicon
    numed   = 3;  // medium number
    
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
  // Specific Material definition
  // --  Material: Vacuum
  TGeoMedium * pMedVac=NULL;
  if (gGeoManager->GetMedium("Vacuum") ){
    pMedVac=gGeoManager->GetMedium("Vacuum");
   }else{
    a = 0.;
    z = 0.;
    density = 0.;
    TGeoMaterial *mat = new TGeoMaterial("Vacuum", a, z, density);
    numed   = 1;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 1.000000; // ifield
    par[2]  = 1.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pMedVac = new TGeoMedium("Vacuum", numed,mat, par);
   }

  // Geometry description
  // SiLi Virtual container
  /////////////////// 
  Double_t dx =  5.4;     // [cm]
  Double_t dy = 10.4;     // [cm]
  Double_t dz =  0.65;    // [cm]   
  TGeoShape *SiLiVirBox = new TGeoBBox("SiLiVirBox" ,dx/2, dy/2, dz/2);
  TGeoVolume* SiLi = new TGeoVolume("SiLi", SiLiVirBox, pMedVac);
  SiLi->SetVisibility(1);

  // SiLi Detector description
  // Booleans 
  ///////////////////////////
  Double_t rmin=dy/2;
  Double_t rmax=dy;
  TGeoShape *sBox  = new TGeoBBox("sBox" ,dx/2, dy/2, dz/2);
  TGeoShape *B1    = new TGeoTube("B1", rmin,rmax,dz*2);  
  TGeoTranslation *tr1 = new TGeoTranslation();
  TGeoSubtraction *sb1 = new TGeoSubtraction(sBox,B1,0,tr1);
  TGeoShape *sb1_shape = new TGeoCompositeShape("sb1_shape", sb1);
  // make the composit volume
  TGeoVolume *comp = new TGeoVolume("COMP",sb1_shape,pMedSi);

  // Add the sensitive part
  AddSensitiveVolume(comp);
  fNbOfSensitiveVol+=1;

  // put it in the virtual geometry
  SiLi->AddNode(comp, 0);

  // global positioning
  //////////////////////
  TGeoVolume* pWorld = gGeoManager->GetTopVolume();
  pWorld->SetVisLeaves(kTRUE);

  // global positioning SiLi#1
  Double_t t2ED1=31.15; // to the surface
  Double_t halfXActiveArea = 4.8/2.;
  Double_t phi=86.0;       // starting angle
  Double_t phi0=80.5;        
  TVector3 baseVecD1 = TVector3(0,0,t2ED1);
  TVector3 corner2center = TVector3(-halfXActiveArea,0,dz/2);
  TVector3 EDet1Pos = baseVecD1+corner2center;
  EDet1Pos.RotateY(phi*TMath::Pi()/180.);
  TGeoRotation *rot1 = new TGeoRotation();
  rot1->RotateX(0.);
  rot1->RotateY(phi0);
  rot1->RotateZ(0.);
  TGeoCombiTrans *t1 = new TGeoCombiTrans(EDet1Pos.X(),EDet1Pos.Y(),
					  EDet1Pos.Z(),rot1);

  pWorld->AddNode(SiLi, 1, t1);

  // global positioning SiLi#2
  Double_t t2ED2 = 33.55;     // to the surface
  phi=82.0;                // starting angle
  TVector3 baseVecD2 = TVector3(0,0,t2ED2);
  TVector3 EDet2Pos = baseVecD2+corner2center;
  EDet2Pos.RotateY(phi*TMath::Pi()/180.);
  TGeoCombiTrans *t2 = new TGeoCombiTrans(EDet2Pos.X(),EDet2Pos.Y(),
					  EDet2Pos.Z(),rot1);

  pWorld->AddNode(SiLi, 2, t2);
}

E105SiLiPoint* E105SiLi::AddHit(Int_t trackID, Int_t detID,
    TVector3 pos, TVector3 mom, TVector3 posOut,
    TVector3 momOut, Double_t time, Double_t length,
    Double_t eLoss)
{
  TClonesArray& clref = *fE105SiLiPointCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) E105SiLiPoint(trackID, detID, pos, mom,
         posOut, momOut, time, length, eLoss);
}

ClassImp(E105SiLi)
