/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/

// Ensar includes
#include "EnsarScintillator.h"
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


EnsarScintillator::EnsarScintillator() : EnsarDetector("EnsarScintillator", kTRUE, kTSCINTDET)
{    
  ResetParameters();
  fEnsarScintillatorPointCollection = new TClonesArray("EnsarScintillatorPoint");
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  
}

EnsarScintillator::EnsarScintillator(const char* name, Bool_t active) : EnsarDetector(name, active, kTSCINTDET)
{
  ResetParameters();
  fEnsarScintillatorPointCollection = new TClonesArray("EnsarScintillatorPoint");
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  
}

EnsarScintillator::~EnsarScintillator()
{
  if ( flGeoPar ) delete flGeoPar;
  if (fEnsarScintillatorPointCollection) {
    fEnsarScintillatorPointCollection->Delete();
    delete fEnsarScintillatorPointCollection;
  }
   
}

void EnsarScintillator::Initialize()
{
 
  FairDetector::Initialize();
  FairRuntimeDb* rtdb= FairRun::Instance()->GetRuntimeDb();
  EnsarScintillatorGeoPar* gpar =(EnsarScintillatorGeoPar*)(rtdb->getContainer("EnsarScintillatorGeoPar"));
  
   //Check this
  // Initialise variables from Birk law
  Double_t dP = 1.032 ;
  // Set constants for Birk's Law implentation
  fBirkC0 =  1.;
  fBirkC1 =  0.013/dP;
  fBirkC2 =  9.6e-6/(dP * dP);
}

Bool_t  EnsarScintillator::ProcessHits(FairVolume* vol)
{
  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fLightYield = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    fMass   = gMC->TrackMass();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fEnergy = gMC->Etot()-fMass;
    
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Apply Birk's law ( Adapted from G3BIRK/Geant3)
  Double_t lightYield =  gMC->Edep() ;

  // Correction for all charge states
  if (gMC->TrackCharge()!=0) {
    Double_t birkC1Mod = 0;
    
  // Apply correction for higher charge states
    if (fBirkC0==1){
      if (TMath::Abs(gMC->TrackCharge())>=2)
        birkC1Mod=fBirkC1*7.2/12.6;
      else
        birkC1Mod=fBirkC1;
    }

    Double_t dedxcm=0.;
    if (gMC->TrackStep()>0)
    {
      dedxcm=1000.*gMC->Edep()/gMC->TrackStep();
      lightYield=lightYield/(1.+birkC1Mod*dedxcm+fBirkC2*dedxcm*dedxcm);
      fLightYield=fLightYield+lightYield;
    }
  }

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
    fEnergy = gMC->Etot()-fMass;

    if (fELoss == 0. ) return kFALSE;
 
  // Adding MonteCarlo Points in the PointColection stack
    
    AddHit(fTrackID, fVolumeID, fTrackPID, fParentTrackID, fUniqueID,
           TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
           fEnergy,fTime, fLength, fELoss, fLightYield);

    // Increment number of EnsarScintillator det points in TParticle
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kTSCINTDET);
   
    ResetParameters();
  }

  return kTRUE;
}

void EnsarScintillator::EndOfEvent()
{

  if (fVerboseLevel) Print();
  fEnsarScintillatorPointCollection->Clear();
  ResetParameters();
}


void EnsarScintillator::Register()
{

  /** This will create a branch in the output tree called
      EnsarScintillatorPoint, setting the last parameter to kFALSE means:
      this collection will not be written to the file, it will exist
      only during the simulation.
  */

  FairRootManager::Instance()->Register("EnsarScintillatorPoint", "ScintillatorPoint",
                                        fEnsarScintillatorPointCollection, kTRUE);

}


TClonesArray* EnsarScintillator::GetCollection(Int_t iColl) const
{
  if (iColl == 0) { return fEnsarScintillatorPointCollection; }
  else { return NULL; }
}

void EnsarScintillator::Reset()
{
  fEnsarScintillatorPointCollection->Clear();
  ResetParameters();
}

void EnsarScintillator::ConstructGeometry()
{
  Double_t dx, dy, dz;
    // Material definition
  Double_t aMat;
  Double_t z, density, w;
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
    par[3]  = 0.000000; // tmaxfd    EnsarScintillatorDigiPar* dpar;
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    AirMedium = new TGeoMedium("Air", numed, AirMaterial, par);
  }
  
 // CsI  
  
  TGeoMedium * CsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    CsIMedium=gGeoManager->GetMedium("CsI");
  } else {
    nel     = 2; //number of mixted material
    density = 4.510000; // g/cm3
    TGeoMixture* CsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    CsIMaterial->DefineElement(0,aMat,z,w);
    aMat = 126.904470;   z = 53.000000;   w = 0.488451;  // I
    CsIMaterial->DefineElement(1,aMat,z,w);
    numed = 801;
    CsIMaterial->SetIndex(numed);
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    CsIMedium = new TGeoMedium("CsIn", numed,CsIMaterial, par);
  }
  
 // BC408
  
  TGeoMedium * BC408Medium=NULL;
  if (gGeoManager->GetMedium("BC408") ) {
    BC408Medium=gGeoManager->GetMedium("BC408");    
  } else {
     nel    = 2;
     density = 1.032000;
     TGeoMixture *mBC408 = new TGeoMixture("BC408", nel,density);
     aMat = 1.007940;  z = 1.000000;  w = 0.0764;  // H
     mBC408->DefineElement(0,aMat,z,w);
     aMat = 12.010700;  z = 6.000000;  w = 0.916;  // C
     mBC408->DefineElement(1,aMat,z,w);
     numed = 802;
     mBC408->SetIndex(numed);

   par[0]  = 0.000000; // isvol
   par[1]  = 0.000000; // ifield
   par[2]  = 0.000000; // fieldm
   par[3]  = 0.000000; // tmaxfd
   par[4]  = 0.000000; // stemax
   par[5]  = 0.000000; // deemax
   par[6]  = 0.000000; // epsil
   par[7]  = 0.000000; // stmin
   BC408Medium = new TGeoMedium("BC408", numed,mBC408,par);
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
   TGeoShape *motherShape = new TGeoBBox("ScintiDetWorld", dx,dy,dz);
 
// Volume: ScintiDetLogWorld
   TGeoVolume *motherCave = new TGeoVolume("ScintiDetLogWorld",motherShape, AirMedium);
//   motherCave->SetVisLeaves(kFALSE);
   TGeoCombiTrans *t0 = new TGeoCombiTrans();
   TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  
  // add the Mother Volume for all other geometries
   pAWorld->AddNodeOverlap(motherCave, 0, pGlobalc);

   // Shape: Ensar Scintillation Detector Box 
   // Type: TGeoBBox
   // Material: BC408

   Double_t rmin = 0.0;
   Double_t rmax = 5.0;
   dz = 10.000000;
   TGeoTube *ScintillatorShape = new TGeoTube("ScintiDet", rmin,rmax,dz);
   // Volume: ScintiDetLog
   TGeoVolume *ScintillatorVol = new TGeoVolume("ScintiDetLog",ScintillatorShape, BC408Medium);
   ScintillatorVol->SetVisLeaves(kTRUE);
   AddSensitiveVolume(ScintillatorVol);  
   motherCave->AddNode(ScintillatorVol,1,pos_rot);
   

    FairRuntimeDb *rtdb= FairRun::Instance()->GetRuntimeDb();
    EnsarScintillatorDigiPar* dpar =(EnsarScintillatorDigiPar*)(rtdb->getContainer("EnsarScintillatorDigiPar"));
    dpar->SetScintillatorLength(2*ScintillatorShape->GetRmax());
    dpar->SetScintillatorHeight(2*ScintillatorShape->GetRmax());
    dpar->SetScintillatorDepth(2*ScintillatorShape->GetDz());
    dpar->setChanged();   

  
}

EnsarScintillatorPoint* EnsarScintillator::AddHit(Int_t trackID, Int_t volumeID,
                              Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                              TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
                              Double_t energy, Double_t time, Double_t length, Double_t eLoss, Double_t lightYield)
{
  TClonesArray& clref = *fEnsarScintillatorPointCollection;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) EnsarScintillatorPoint(trackID, volumeID, trackPID, parentTrackID, uniqueID, posIn, posOut,
                                       momIn, momOut, energy, time, length, eLoss, lightYield);
}

int EnsarScintillator::SetPositionRotation(Double_t x, Double_t y,Double_t z, Double_t rx, Double_t ry, Double_t rz)
{
   TGeoRotation *Rotation= new TGeoRotation();
   Rotation->RotateX(rx);
   Rotation->RotateY(ry);
   Rotation->RotateZ(rz);
   pos_rot = new TGeoCombiTrans(x,y,z,Rotation);
}



ClassImp(EnsarScintillator)
