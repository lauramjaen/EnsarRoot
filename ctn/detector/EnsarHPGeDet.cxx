// -----------------------------------------------------------------------
// -----                   EnsarHPGeDet source file                    -----
// -----               Created 11/09/12  by P.Cabanelas              -----
// -----------------------------------------------------------------------
#include "EnsarHPGeDet.h"

#include "EnsarHPGeDetPoint.h"
#include "EnsarHPGeDetHit.h"

#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
#include "FairStack.h"
#include "FairRuntimeDb.h"
#include "FairRun.h"
#include "FairVolume.h"

#include "TClonesArray.h"
#include "TGeoMCGeometry.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TObjArray.h"
#include "TMCProcess.h"

// includes for modeling
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
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;



// -----   Default constructor   -------------------------------------------
EnsarHPGeDet::EnsarHPGeDet() : EnsarDetector("EnsarHPGeDet", kTRUE, kHPGEDET)
{
  ResetParameters();
  fHPGeDetCollection = new TClonesArray("EnsarHPGeDetPoint");
  fHPGeDetHitCollection = new TClonesArray("EnsarHPGeDetHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fNonUniformity = 0.01;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
EnsarHPGeDet::EnsarHPGeDet(const char* name, Bool_t active)
    : EnsarDetector(name, active, kCALIFA)
{
  ResetParameters();
  fHPGeDetCollection = new TClonesArray("EnsarHPGeDetPoint");
  fHPGeDetHitCollection = new TClonesArray("EnsarHPGeDetHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fNonUniformity = 0.01;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
EnsarHPGeDet::~EnsarHPGeDet()
{

  if ( flGeoPar ) delete flGeoPar;
  if (fHPGeDetCollection) {
    fHPGeDetCollection->Delete();
    delete fHPGeDetCollection;
  }
  if (fHPGeDetHitCollection) {
    fHPGeDetHitCollection->Delete();
    delete fHPGeDetHitCollection;
  }
}
// -------------------------------------------------------------------------
void EnsarHPGeDet::Initialize()
{
  FairDetector::Initialize();

  cout << endl;
  cout << "-I- EnsarHPGeDet: initialisation" << endl;
  cout << "-I- EnsarHPGeDet: Vol (McId) def." << endl;

  TGeoVolume *vol = gGeoManager->GetVolume("World");
  vol->SetVisibility(kFALSE);


}


void EnsarHPGeDet::SetSpecialPhysicsCuts()
{

  cout << endl;

  cout << "-I- EnsarHPGeDet: Adding customized Physics cut ... " << endl;
  cout << "-I- EnsarHPGeDet: Yet not implemented !... " << endl;

  cout << endl;

}




// -----   Public method ProcessHits  --------------------------------------
Bool_t EnsarHPGeDet::ProcessHits(FairVolume* vol)
{

  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fEinc   = gMC->Etot();
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();
  fNSteps++;

  // Set additional parameters at exit of active volume. Create EnsarHPGeDetPoint.
  if ( gMC->IsTrackExiting()    ||
       gMC->IsTrackStop()       ||
       gMC->IsTrackDisappeared()   ) {

    fTrackID        = gMC->GetStack()->GetCurrentTrackNumber();
    fParentTrackID  = gMC->GetStack()->GetCurrentParentTrackNumber();
    fVolumeID       = vol->getMCid();
    fTrackPID       = gMC->TrackPid();
    fUniqueID       = gMC->GetStack()->GetCurrentTrack()->GetUniqueID();

    gMC->TrackPosition(fPosOut);
    gMC->TrackMomentum(fMomOut);

    if (fELoss == 0. ) return kFALSE;

    if (gMC->IsTrackExiting()) {
      const Double_t* oldpos;
      const Double_t* olddirection;
      Double_t newpos[3];
      Double_t newdirection[3];
      Double_t safety;

      gGeoManager->FindNode(fPosOut.X(),fPosOut.Y(),fPosOut.Z());
      oldpos = gGeoManager->GetCurrentPoint();
      olddirection = gGeoManager->GetCurrentDirection();

      for (Int_t i=0; i<3; i++) {
        newdirection[i] = -1*olddirection[i];
      }

      gGeoManager->SetCurrentDirection(newdirection);
      safety = gGeoManager->GetSafeDistance();

      gGeoManager->SetCurrentDirection(-newdirection[0],-newdirection[1],-newdirection[2]);

      for (Int_t i=0; i<3; i++) {
        newpos[i] = oldpos[i] - (3*safety*olddirection[i]);
      }

      fPosOut.SetX(newpos[0]);
      fPosOut.SetY(newpos[1]);
      fPosOut.SetZ(newpos[2]);
    }

    AddHit(fTrackID, fVolumeID, fTrackPID, fParentTrackID, fUniqueID,
           TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
           fTime, fLength, fELoss);

    // Increment number of HPGeDetPoints for this track
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kHPGEDET);


    //Adding Hit support
    Int_t nHits = fHPGeDetHitCollection->GetEntriesFast();
    Bool_t existHit = 0;
    if (nHits==0) AddFullHit(fTrackID, fVolumeID, fTrackPID, fParentTrackID, fUniqueID,
        NUSmearing(fELoss), fTime, fNSteps, fEinc);
    else {
      for (Int_t i=0; i<nHits; i++) {
        ((EnsarHPGeDetHit *)(fHPGeDetHitCollection->At(i)))->AddMoreEnergy(NUSmearing(fELoss));
        if ( ((EnsarHPGeDetHit *)(fHPGeDetHitCollection->At(i)))->GetTime() > fTime ) {
          ((EnsarHPGeDetHit *)(fHPGeDetHitCollection->At(i)))->SetTime(fTime);
        }
        existHit=1; //to avoid the creation of a new Hit
        break;
      }
      if (!existHit) AddFullHit(fTrackID, fVolumeID, fTrackPID, fParentTrackID, fUniqueID,
        NUSmearing(fELoss), fTime, fNSteps, fEinc);
    }

    existHit=0;

    ResetParameters();
  }

  return kTRUE;
}


// -----   Public method EndOfEvent   -----------------------------------------
void EnsarHPGeDet::BeginEvent()
{

}
// -----   Public method EndOfEvent   -----------------------------------------
void EnsarHPGeDet::EndOfEvent()
{

  if (fVerboseLevel) Print();

  fHPGeDetCollection->Clear();
  fHPGeDetHitCollection->Clear();

  ResetParameters();

}
// ----------------------------------------------------------------------------



// -----   Public method Register   -------------------------------------------
void EnsarHPGeDet::Register()
{

  FairRootManager::Instance()->Register("HPGeDetPoint", GetName(), fHPGeDetCollection, kTRUE);
  FairRootManager::Instance()->Register("HPGeDetHit", GetName(), fHPGeDetHitCollection, kTRUE);

}
// ----------------------------------------------------------------------------



// -----   Public method GetCollection   --------------------------------------
TClonesArray* EnsarHPGeDet::GetCollection(Int_t iColl) const
{

  if (iColl == 0) return fHPGeDetCollection;
  if (iColl == 1) return fHPGeDetHitCollection;
  else return NULL;

}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void EnsarHPGeDet::Print() const
{

  Int_t nHits = fHPGeDetCollection->GetEntriesFast();
  cout << "-I- EnsarHPGeDet: " << nHits << " points registered in this event." << endl;

}
// ----------------------------------------------------------------------------



// -----   Public method Reset   ----------------------------------------------
void EnsarHPGeDet::Reset()
{

  fHPGeDetCollection->Clear();
  fHPGeDetHitCollection->Clear();
  ResetParameters();

}
// ----------------------------------------------------------------------------



// -----   Public method CopyClones   -----------------------------------------
void EnsarHPGeDet::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{

  Int_t nEntries = cl1->GetEntriesFast();
  cout << "-I- EnsarHPGeDet: " << nEntries << " entries to add." << endl;
  TClonesArray& clref = *cl2;
  EnsarHPGeDetPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (EnsarHPGeDetPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) EnsarHPGeDetPoint(*oldpoint);
    fPosIndex++;
  }
  cout << " -I- EnsarHPGeDet: " << cl2->GetEntriesFast() << " merged entries." << endl;

}

// -----   Private method AddHit   --------------------------------------------
EnsarHPGeDetPoint* EnsarHPGeDet::AddHit(Int_t trackID, Int_t volumeID,
                              Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                              TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
                              Double_t time, Double_t length, Double_t eLoss)
{
  TClonesArray& clref = *fHPGeDetCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1)
    cout << "-I- EnsarHPGeDet: Adding Point at (" << posIn.X() << ", " << posIn.Y()
         << ", " << posIn.Z() << ") cm,  active volume " << volumeID << ", track "
         << trackID << ", energy loss " << eLoss*1e06 << " keV" << endl;
  return new(clref[size]) EnsarHPGeDetPoint(trackID, volumeID, trackPID, parentTrackID, uniqueID, posIn, posOut,
                                       momIn, momOut, time, length, eLoss);
}

// -----   Private method AddFullHit   --------------------------------------------
EnsarHPGeDetHit* EnsarHPGeDet::AddFullHit(Int_t trackID, Int_t volumeID,
                              Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                              Double_t energy, Double_t time, Int_t steps, Double_t einc)

{
  TClonesArray& clref = *fHPGeDetHitCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1)
    cout << "-I- EnsarHPGeDet: Adding Hit in active volume " << volumeID << ", track "
         << trackID << ", energy loss " << energy*1e06 << " keV" << endl;
  return new(clref[size]) EnsarHPGeDetHit(trackID, volumeID, trackPID, parentTrackID, uniqueID,
                                       energy, time, steps, einc);
}



// -----   Private method createMatrix --------------------------------------------
TGeoRotation* EnsarHPGeDet::createMatrix( Double_t phi, Double_t theta, Double_t psi)
{

  // Rotation
  TGeoRotation * matrix = new TGeoRotation("");

  Double_t rm[9];

  Double_t degrad = TMath::Pi()/180.;

  // define trigonometry
  Double_t  sinPhi   = sin( degrad*phi )  ;
  Double_t  cosPhi   = cos( degrad*phi )  ;
  Double_t  sinTheta = sin( degrad*theta );
  Double_t  cosTheta = cos( degrad*theta );
  Double_t  sinPsi   = sin( degrad*psi )  ;
  Double_t  cosPsi   = cos( degrad*psi )  ;


  // filling from Euler definition a la G4 !

  rm[0] =   cosPsi * cosPhi - cosTheta * sinPhi * sinPsi;
  rm[1] =   cosPsi * sinPhi + cosTheta * cosPhi * sinPsi;
  rm[2] =   sinPsi * sinTheta;

  rm[3] = - sinPsi * cosPhi - cosTheta * sinPhi * cosPsi;
  rm[4] = - sinPsi * sinPhi + cosTheta * cosPhi * cosPsi;
  rm[5] =   cosPsi * sinTheta;

  rm[6] =   sinTheta * sinPhi;
  rm[7] = - sinTheta * cosPhi;
  rm[8] =   cosTheta;


  matrix->SetMatrix( (const Double_t*) &rm[0] );

  return matrix;

}

// -----   Private method NUSmearing  --------------------------------------------
Double_t EnsarHPGeDet::NUSmearing(Double_t inputEnergy)
{
  // Very simple preliminary scheme where the NU is introduced as a flat random
  // distribution with limits fNonUniformity (%) of the energy value.
  //
  return gRandom->Uniform(inputEnergy-inputEnergy*fNonUniformity/100,inputEnergy+inputEnergy*fNonUniformity/100);
}

// -----  Public method SetNonUniformity  ----------------------------------
void EnsarHPGeDet::SetNonUniformity(Double_t nonU)
{
  fNonUniformity = nonU;
  LOG(INFO) << "EnsarHPGeDet::SetNonUniformity to " << fNonUniformity << " %" << FairLogger::endl;
}



// -----   Public method ConstructGeometry   ----------------------------------
void EnsarHPGeDet::ConstructGeometry()
{

 
 // ----------------------------------------------
 // Creating materials and mediums
 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 TGeoMaterial *matGe  = new TGeoMaterial("Ge",72.64,32,5.32);
 TGeoMaterial *matAl  = new TGeoMaterial("Al",26.98,13,2.70);
 TGeoMaterial *matLi  = new TGeoMaterial("Li",6.94,3,0.54);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 TGeoMedium *Ge     = new TGeoMedium("Ge Medium",1, matGe);
 TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 TGeoMedium *Li     = new TGeoMedium("Li Medium",3, matLi);

 //Inox Steel: 0.15%C, 13%Cr, 86.85%Fe 
 Int_t nel = 3;
 Float_t density = 7.9;
 TGeoMixture *matSteel = new TGeoMixture("Steel",nel,density);
 Float_t aMat = 12.000000; Float_t z = 6.000000; Float_t w = 0.0015; // C
 matSteel->DefineElement(0,aMat,z,w);
 aMat = 51.996; z = 24.000000; w = 0.13; //Cr
 matSteel->DefineElement(1,aMat,z,w);
 aMat = 55.845; z = 26.000000; w = 0.8685; //Fe
 matSteel->DefineElement(2,aMat,z,w);
 Int_t numed = 4;
 matSteel->SetIndex(numed);
 Double_t par[8];
 par[0]  = 0.000000; // isvol
 par[1]  = 0.000000; // ifield
 par[2]  = 0.000000; // fieldm
 par[3]  = 0.000000; // tmaxfd
 par[4]  = 0.000000; // stemax
 par[5]  = 0.000000; // deemax
 par[6]  = 0.000100; // epsil
 par[7]  = 0.000000; // stmin
 TGeoMedium *Steel = new TGeoMedium("Steel Medium",numed,matSteel,par);
 // ----------------------------------------------

 // ----------------------------------------------
 // Top Volume
 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,1000,1000,1000);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 // ----------------------------------------------
 
 // ----------------------------------------------
 // Auxiliar vacuum volume holding all important volumes as daugthers
 // The easiest way to rotate all volumes to the final position
 TGeoVolume *main_tube = gGeoManager->MakeTube("MAIN_TUBE",Vacuum,0.0,4.035,3.505);
 main_tube->SetFillColor(5);
 main_tube->SetLineColor(5);
 main_tube->SetTransparency(70);
 // ----------------------------------------------

 // ----------------------------------------------
 // Steel holder of the chamber
 TGeoVolume *steel_tub1 = gGeoManager->MakeTube("STEEL_TUB1", Steel, 0.0, 3.95, 0.1);
 steel_tub1->SetFillColor(5);
 steel_tub1->SetLineColor(5);
 main_tube->AddNode(steel_tub1,1,new TGeoTranslation(0,0,-3.405));
 TGeoVolume *steel_tub2 = gGeoManager->MakeTube("STEEL_TUB2", Steel, 3.75, 3.95, 3.405);
 steel_tub2->SetFillColor(5);
 steel_tub2->SetLineColor(5);
 main_tube->AddNode(steel_tub2,1,new TGeoTranslation(0,0,0.1));
 // ----------------------------------------------

 // ----------------------------------------------
 // First Aluminum wrapping
 TGeoVolume *al_tub1 = gGeoManager->MakeTube("AL_TUB1", Al, 0.0, 3.75, 0.05);
 al_tub1->SetFillColor(18);
 al_tub1->SetLineColor(18);
 main_tube->AddNode(al_tub1,1,new TGeoTranslation(0,0,-3.255));
 TGeoVolume *al_tub2 = gGeoManager->MakeTube("AL_TUB2", Al, 3.65, 3.75, 3.355);
 al_tub2->SetFillColor(18);
 al_tub2->SetLineColor(18);
 main_tube->AddNode(al_tub2,1,new TGeoTranslation(0,0,0.15));
 // ----------------------------------------------

 // ----------------------------------------------
 // Second Aluminum wrapping (4mm vacuum betwenn both wrappings)
 TGeoVolume *al_tub3 = gGeoManager->MakeTube("AL_TUB3", Al, 0.0, 3.25, 0.025);
 al_tub3->SetFillColor(14);
 al_tub3->SetLineColor(14);
 main_tube->AddNode(al_tub3,1,new TGeoTranslation(0,0,-2.78));
 TGeoVolume *al_tub4 = gGeoManager->MakeTube("AL_TUB4", Al, 3.20, 3.25, 3.13);
 al_tub4->SetFillColor(14);
 al_tub4->SetLineColor(14);
 main_tube->AddNode(al_tub4,1,new TGeoTranslation(0,0,0.375));
 // ----------------------------------------------

 // ----------------------------------------------
 // Lithium side-contacts
 TGeoVolume *li_tub1 = gGeoManager->MakeTube("LI_TUB1", Li, 3.13, 3.20, 3.13);
 li_tub1->SetFillColor(4);
 li_tub1->SetLineColor(4);
 main_tube->AddNode(li_tub1,1,new TGeoTranslation(0,0,0.375));
 // ----------------------------------------------

 // ----------------------------------------------
 //Ge inactive layer: 2mm
 TGeoVolume *ge_tub1 = gGeoManager->MakeTube("GE_TUB1", Ge, 0.0, 3.2, 0.1);
 ge_tub1->SetFillColor(50);
 ge_tub1->SetLineColor(50);
 main_tube->AddNode(ge_tub1,1,new TGeoTranslation(0,0,-2.68));
 TGeoVolume *ge_tub2 = gGeoManager->MakeTube("GE_TUB2", Ge, 3.0, 3.2, 3.03);
 ge_tub2->SetFillColor(50);
 ge_tub2->SetLineColor(50);
 main_tube->AddNode(ge_tub2,1,new TGeoTranslation(0,0,0.475));
 // ----------------------------------------------

 // ----------------------------------------------
 //Ge active volume
 TGeoVolume *ge_tub3 = gGeoManager->MakeTube("GE_TUB3", Ge, 0.0, 3.0, 0.575);
 ge_tub3->SetFillColor(30);
 ge_tub3->SetLineColor(30);
 main_tube->AddNode(ge_tub3,1,new TGeoTranslation(0,0,-2.005));
 TGeoVolume *ge_tub4 = gGeoManager->MakeTube("GE_TUB4", Ge, 0.45, 3.0, 2.455);
 ge_tub4->SetFillColor(30);
 ge_tub4->SetLineColor(30);
 main_tube->AddNode(ge_tub4,1,new TGeoTranslation(0,0,1.05));
 // ----------------------------------------------


 // ----------------------------------------------
 // only linear translation
 //top->AddNode(main_tube,1,new TGeoTranslation(0,0,8.855));
 // ----------------------------------------------

 // ----------------------------------------------
 // detector at 130 degrees
 TGeoRotation *rot     = new TGeoRotation("rot",0,220,0);
 TGeoCombiTrans *comb = new TGeoCombiTrans("comb",0,6.7833,-5.6919,rot);

 top->AddNode(main_tube,1,comb);
 // ----------------------------------------------
 

 // ----------------------------------------------
 // Adding sensitive volumes only de active region
 AddSensitiveVolume(ge_tub3);
 AddSensitiveVolume(ge_tub4);
 // ----------------------------------------------

}


ClassImp(EnsarHPGeDet)
