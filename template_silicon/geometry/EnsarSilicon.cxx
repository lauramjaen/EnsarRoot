/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0 / 2014
*    -----------------------------------------------------------------------
*/

// EnsarRoot includes
#include "EnsarSilicon.h"

// FairRoot includes
#include "FairVolume.h"
#include "FairGeoVolume.h"
#include "FairGeoNode.h"
#include "FairRootManager.h"
#include "FairRuntimeDb.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairDetectorList.h"
#include "FairStack.h"

// ROOT includes 
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TVirtualMCStack.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoBBox.h"
#include "TGeoPgon.h"
#include "TGeoSphere.h"
#include "TGeoArb8.h"
#include "TGeoCone.h"
#include "TGeoTube.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TClonesArray.h"
#include "TGeoMCGeometry.h"
#include "TMath.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TObjArray.h"
#include "TMCProcess.h"

// C++ includes 
#include <iostream>
using std::cout;
using std::endl;


EnsarSilicon::EnsarSilicon() : EnsarDetector("EnsarSilicon", kTRUE, kTSILIDET)
{    
  ResetParameters();
  fEnsarSiliconPointCollection = new TClonesArray("EnsarSiliconPoint");
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  
}

EnsarSilicon::EnsarSilicon(const char* name, Bool_t active) : EnsarDetector(name, active, kTSILIDET)
{
  ResetParameters();
  fEnsarSiliconPointCollection = new TClonesArray("EnsarSiliconPoint");
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  
}

EnsarSilicon::~EnsarSilicon()
{
  if (fEnsarSiliconPointCollection) {
    fEnsarSiliconPointCollection->Delete();
    delete fEnsarSiliconPointCollection;
  }
   
}

void EnsarSilicon::Initialize()
{
 
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  EnsarSiliconGeoPar* gpar =(EnsarSiliconGeoPar*)(rtdb->getContainer("EnsarSiliconGeoPar"));
  
}

Bool_t  EnsarSilicon::ProcessHits(FairVolume* vol)
{
  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09; // in seconds
    fLength = gMC->TrackLength();
    fMass   = gMC->TrackMass();  
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fEnergy = gMC->Etot()-fMass;
    
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();


   // Set additional parameters at exit of active volume. Create R3BLandPoint.
  if ( gMC->IsTrackExiting()   ||
       gMC->IsTrackStop()        ||
       gMC->IsTrackDisappeared()  )
  {
    fTrackID        = gMC->GetStack()->GetCurrentTrackNumber();
    fParentTrackID  = gMC->GetStack()->GetCurrentParentTrackNumber();
    fVolumeID       = vol->getMCid();
    fMass           = gMC->TrackMass();
    fTrackPID       = gMC->TrackPid();
    fUniqueID       = gMC->GetStack()->GetCurrentTrack()->GetUniqueID();
    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);
//    fEnergy         = (fMomOut.Px()*fMomOut.Px() + fMomOut.Py()*fMomOut.Py() + fMomOut.Pz()*fMomOut.Pz())/(2*fMass);
    fEnergy = gMC->Etot()-fMass;

    if (fELoss == 0. ) return kFALSE;
 
  // Adding MonteCarlo Points in the PointColection stack
    
    AddHit(fTrackID, fVolumeID, fTrackPID, fParentTrackID, fUniqueID,
           TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
           fEnergy,fTime, fLength, fELoss);
    
    // Increment number of EnsarSilicon det points in TParticle
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kTSILIDET);
   
    ResetParameters();
  }

  return kTRUE;
}

void EnsarSilicon::EndOfEvent()
{

  if (fVerboseLevel) Print();
  fEnsarSiliconPointCollection->Clear();
  ResetParameters();
}

void EnsarSilicon::Register()
{

  /** This will create a branch in the output tree called
      EnsarSiliconPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("EnsarSiliconPoint", "SiliconPoint",
                                        fEnsarSiliconPointCollection, kTRUE);

}


TClonesArray* EnsarSilicon::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fEnsarSiliconPointCollection; }
  else { return NULL; }
}

void EnsarSilicon::Reset()
{
  fEnsarSiliconPointCollection->Clear();
  ResetParameters();
}

void EnsarSilicon::ConstructGeometry()
{
  Double_t dx, dy, dz;
    // Material definition
  Double_t aMat;
  Double_t a, z, density, w;
  Double_t radl, absl;
  Int_t nel, numed;
  Double_t par[8];
 
// Definition of the Material, Mixture and Medium
  
  // Vaccum
  TGeoMedium * VaccumMedium=NULL;
  if (gGeoManager->GetMedium("Vaccum") ) {
    VaccumMedium=gGeoManager->GetMedium("Vaccum");
  } else {
   aMat       = 1.010000;
   z       = 1.000000;
   density = 0.000000;
   radl    = 579553059467092293666734080.000000;
   absl    = 3511630811855104348084240384.000000;
   TGeoMaterial *VaccumMaterial = new TGeoMaterial("Vacuum", aMat,z,density,radl,absl);
   VaccumMaterial->SetIndex(0);    

// Vacuum parameters:
   numed   = 0;  // medium number
   par[0]  = 0.000000; // isvol
   par[1]  = 0.000000; // ifield
   par[2]  = 0.000000; // fieldm
   par[3]  = 0.000000; // tmaxfd
   par[4]  = 0.000000; // stemax
   par[5]  = 0.000000; // deemax
   par[6]  = 0.000000; // epsil
   par[7]  = 0.000000; // stmin
   VaccumMedium = new TGeoMedium("Vacuum", numed,VaccumMaterial, par);
  }
  // Air
  TGeoMedium * AirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    AirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel = 2;
    density = 0.001290;
    TGeoMixture*  AirMaterial = new TGeoMixture("Air", nel,density);
    aMat = 14.006740;   z = 7.000000;   w = 0.700000;  // N
    AirMaterial->DefineElement(0,aMat,z,w);
    aMat = 15.999400;   z = 8.000000;   w = 0.300000;  // O
    AirMaterial->DefineElement(1,aMat,z,w);
    AirMaterial->SetIndex(1);
    // Medium: Air
    numed   = 1;  // medium number
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd    EnsarSiliconDigiPar* dpar;
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    AirMedium = new TGeoMedium("Air", numed, AirMaterial, par);
  }
  
  // Silicon  
  
  TGeoMedium* SiliconMedium = NULL;
  if (gGeoManager->GetMedium("Silicon") ) {
    SiliconMedium = gGeoManager->GetMedium("Silicon");
  } else {
    // Material Silicon: for EnsarSiliconDetector
   a       = 28.0855;
   z       = 14.000000;
   density = 2.3300000;
   radl    = 9.351106;
   absl    = 456.628489;
   TGeoMaterial* SiliconMaterial = new TGeoMaterial("SiliconMat", a,z,density,radl,absl);
   SiliconMaterial->SetIndex(21);

   // Silicon parameters
   numed   = 21;  // medium number
   par[0]  = 0.000000; // isvol
   par[1]  = 0.000000; // ifield
   par[2]  = 0.000000; // fieldm
   par[3]  = 0.000000; // tmaxfd
   par[4]  = 0.000000; // stemax
   par[5]  = 0.000000; // deemax
   par[6]  = 0.000000; // epsil
   par[7]  = 0.000000; // stmin
   SiliconMedium = new TGeoMedium("Silicon", numed, SiliconMaterial, par);
  }
  
  // Geometry Construction
  // Shape: Virtual World Volume
  // Type: TGeoBBox
  // Material: Vaccum
   dx = 1000.000000;
   dy = 1000.000000;
   dz = 1000.000000;
   TGeoShape *WorldBox = new TGeoBBox("World", dx,dy,dz);
// Volume: World
   TGeoVolume *WorldVol = new TGeoVolume("World",WorldBox, VaccumMedium);
// or like this  
//  TGeoVolume *top=gGeoManager->MakeBox("World",AirMedium,1000,1000,1000);
  gGeoManager->SetTopVolume(WorldVol);
  gGeoManager->SetTopVisible(kFALSE);
  
  // Get world volume
  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();
  //pAWorld->SetVisLeaves(kTRUE);

// Shape: Ensar Mother Cave
// Type: TGeoBBox
// Material: Air
   dx = 500.000000;
   dy = 500.000000;
   dz = 500.000000;
   TGeoShape *motherShape = new TGeoBBox("SiliconDetectorWorld", dx,dy,dz);
 
// Volume: SiliconDetLogWorld
   TGeoVolume *motherCave = new TGeoVolume("SiliconDetLogWorld",motherShape, VaccumMedium);
//   motherCave->SetVisLeaves(kFALSE);
   TGeoCombiTrans *t0 = new TGeoCombiTrans();
   TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  
  // add the Mother Volume for all other geometries
   pAWorld->AddNodeOverlap(motherCave, 0, pGlobalc);

   // Shape: Ensar Silicon Detector Disk 
   // Type: TGeoTube
   // Material: Silicon

   Double_t rmin = 0.0;
   Double_t rmax = 1.0;
   dz = 0.1;
   TGeoTube *SiliconShape = new TGeoTube("SiliconDet", rmin,rmax,dz);
   // Volume: SiliconDetLog
   TGeoVolume *SiliconVol = new TGeoVolume("SiliconDetLog",SiliconShape, SiliconMedium);
   SiliconVol->SetVisLeaves(kTRUE);
   AddSensitiveVolume(SiliconVol);  
   motherCave->AddNode(SiliconVol,1,pos_rot);
   

    FairRuntimeDb *rtdb= FairRun::Instance()->GetRuntimeDb();
    EnsarSiliconDigiPar* dpar =(EnsarSiliconDigiPar*)(rtdb->getContainer("EnsarSiliconDigiPar"));
    dpar->SetSiliconLength(2*SiliconShape->GetRmax());
    dpar->SetSiliconHeight(2*SiliconShape->GetRmax());
    dpar->SetSiliconDepth(2*SiliconShape->GetDz());
    dpar->setChanged();   

  
}

EnsarSiliconPoint* EnsarSilicon::AddHit(Int_t trackID, Int_t volumeID,
                              Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                              TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
                              Double_t energy, Double_t time, Double_t length, Double_t eLoss)
{
  TClonesArray& clref = *fEnsarSiliconPointCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) EnsarSiliconPoint(trackID, volumeID, trackPID, parentTrackID, uniqueID, posIn, posOut,
                                       momIn, momOut, energy, time, length, eLoss);
}

int EnsarSilicon::SetPositionRotation(Double_t x, Double_t y,Double_t z, Double_t rx, Double_t ry, Double_t rz)
{
   TGeoRotation *Rotation= new TGeoRotation();
   Rotation->RotateX(rx);
   Rotation->RotateY(ry);
   Rotation->RotateZ(rz);
   pos_rot = new TGeoCombiTrans(x,y,z,Rotation);
}

ClassImp(EnsarSilicon)
