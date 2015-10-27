// -----------------------------------------------------------------------
// -----                   EnsarMyDet source file                    -----
// -----               Created 11/09/12  by P.Cabanelas              -----
// -----------------------------------------------------------------------
#include "EnsarMyDet.h"

#include "EnsarMyDetPoint.h"

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
EnsarMyDet::EnsarMyDet() : EnsarDetector("EnsarMyDet", kTRUE, kMYDET)
{
  ResetParameters();
  fMyDetCollection = new TClonesArray("EnsarMyDetPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
EnsarMyDet::EnsarMyDet(const char* name, Bool_t active)
    : EnsarDetector(name, active, kCALIFA)
{
  ResetParameters();
  fMyDetCollection = new TClonesArray("EnsarMyDetPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
EnsarMyDet::~EnsarMyDet()
{

  if ( flGeoPar ) delete flGeoPar;
  if (fMyDetCollection) {
    fMyDetCollection->Delete();
    delete fMyDetCollection;
  }
}
// -------------------------------------------------------------------------
void EnsarMyDet::Initialize()
{
  FairDetector::Initialize();

  cout << endl;
  cout << "-I- EnsarMyDet: initialisation" << endl;
  cout << "-I- EnsarMyDet: Vol (McId) def." << endl;

/* ==============================================
  Char_t buffer[126];
  for (Int_t i=0; i<30; i++ ) {
    sprintf(buffer,"crystalLog%i",i+1);
    cout << "-I- EnsarMyDet: Crystal Nb   : " << i << " connected to (McId) ---> " <<  gMC->VolId(buffer)<< endl;
    fCrystalType[i] = gMC->VolId(buffer);
  }
============================================== */

  TGeoVolume *vol = gGeoManager->GetVolume("World");
  vol->SetVisibility(kFALSE);


}


void EnsarMyDet::SetSpecialPhysicsCuts()
{

  cout << endl;

  cout << "-I- EnsarMyDet: Adding customized Physics cut ... " << endl;
  cout << "-I- EnsarMyDet: Yet not implemented !... " << endl;

  cout << endl;

}




// -----   Public method ProcessHits  --------------------------------------
Bool_t EnsarMyDet::ProcessHits(FairVolume* vol)
{

  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Set additional parameters at exit of active volume. Create EnsarMyDetPoint.
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

      if ( fPosIn.Z() < 30. && newpos[2] > 30.02 ) {
//        cerr << "2nd direction: " << olddirection[0] << "," << olddirection[1] << "," << olddirection[2]
//             << " with safety = " << safety << endl;
//        cerr << "oldpos = " << oldpos[0] << "," << oldpos[1] << "," << oldpos[2] << endl;
//        cerr << "newpos = " << newpos[0] << "," << newpos[1] << "," << newpos[2] << endl;
//*bp
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

    // Increment number of MyDetPoints for this track
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kMYDET);
    //stack->AddPoint(0);

    ResetParameters();
  }

  return kTRUE;
}
// ----------------------------------------------------------------------------
//void EnsarMyDet::SaveGeoParams(){
//
//  cout << " -I Save STS geo params " << endl;
//
//  TFolder *mf = (TFolder*) gDirectory->FindObjectAny("cbmroot");
//  cout << " mf: " << mf << endl;
//  TFolder *stsf = NULL;
//  if (mf ) stsf = (TFolder*) mf->FindObjectAny(GetName());
//  cout << " stsf: " << stsf << endl;
//  if (stsf) stsf->Add( flGeoPar0 ) ;
//  FairRootManager::Instance()->WriteFolder();
//  mf->Write("cbmroot",TObject::kWriteDelete);
//}


// -----   Public method EndOfEvent   -----------------------------------------
void EnsarMyDet::BeginEvent()
{

//  if (! kGeoSaved ) {
//      SaveGeoParams();
//  cout << "-I STS geometry parameters saved " << endl;
//  kGeoSaved = kTRUE;
//  }

}
// -----   Public method EndOfEvent   -----------------------------------------
void EnsarMyDet::EndOfEvent()
{

  if (fVerboseLevel) Print();

  fMyDetCollection->Clear();

  ResetParameters();

}
// ----------------------------------------------------------------------------



// -----   Public method Register   -------------------------------------------
void EnsarMyDet::Register()
{

  FairRootManager::Instance()->Register("MyDetPoint", GetName(), fMyDetCollection, kTRUE);

}
// ----------------------------------------------------------------------------



// -----   Public method GetCollection   --------------------------------------
TClonesArray* EnsarMyDet::GetCollection(Int_t iColl) const
{

  if (iColl == 0) return fMyDetCollection;
  else return NULL;

}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void EnsarMyDet::Print() const
{

  Int_t nHits = fMyDetCollection->GetEntriesFast();
  cout << "-I- EnsarMyDet: " << nHits << " points registered in this event." << endl;

}
// ----------------------------------------------------------------------------



// -----   Public method Reset   ----------------------------------------------
void EnsarMyDet::Reset()
{

  fMyDetCollection->Clear();
  ResetParameters();

}
// ----------------------------------------------------------------------------



// -----   Public method CopyClones   -----------------------------------------
void EnsarMyDet::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{

  Int_t nEntries = cl1->GetEntriesFast();
  cout << "-I- EnsarMyDet: " << nEntries << " entries to add." << endl;
  TClonesArray& clref = *cl2;
  EnsarMyDetPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (EnsarMyDetPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) EnsarMyDetPoint(*oldpoint);
    fPosIndex++;
  }
  cout << " -I- EnsarMyDet: " << cl2->GetEntriesFast() << " merged entries." << endl;

}

// -----   Private method AddHit   --------------------------------------------
EnsarMyDetPoint* EnsarMyDet::AddHit(Int_t trackID, Int_t volumeID,
                              Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                              TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
                              Double_t time, Double_t length, Double_t eLoss)
{
  TClonesArray& clref = *fMyDetCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1)
    cout << "-I- EnsarMyDet: Adding Point at (" << posIn.X() << ", " << posIn.Y()
         << ", " << posIn.Z() << ") cm,  active volume " << volumeID << ", track "
         << trackID << ", energy loss " << eLoss*1e06 << " keV" << endl;
  return new(clref[size]) EnsarMyDetPoint(trackID, volumeID, trackPID, parentTrackID, uniqueID, posIn, posOut,
                                       momIn, momOut, time, length, eLoss);
}



// -----   Private method createMatrix --------------------------------------------
TGeoRotation* EnsarMyDet::createMatrix( Double_t phi, Double_t theta, Double_t psi)
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

// -----   Public method ConstructGeometry   ----------------------------------
void EnsarMyDet::ConstructGeometry()
{
  // Material definition
  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel = 2;
    density = 0.001290;
    TGeoMixture*  pAirMaterial = new TGeoMixture("Air", nel,density);
    aMat = 14.006740;   z = 7.000000;   w = 0.700000;  // N
    pAirMaterial->DefineElement(0,aMat,z,w);
    aMat = 15.999400;   z = 8.000000;   w = 0.300000;  // O
    pAirMaterial->DefineElement(1,aMat,z,w);
    pAirMaterial->SetIndex(1);
    // Medium: Air
    numed   = 1;  // medium number
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pAirMedium = new TGeoMedium("Air", numed, pAirMaterial, par);
  }
  
  // CsI  
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2; //number of mixted material
    density = 4.510000; // g/cm3
    TGeoMixture* pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCsIMaterial->DefineElement(0,aMat,z,w);
    aMat = 126.904470;   z = 53.000000;   w = 0.488451;  // I
    pCsIMaterial->DefineElement(1,aMat,z,w);
    numed = 801;
    pCsIMaterial->SetIndex(numed);
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pCsIMedium = new TGeoMedium("CsIn", numed,pCsIMaterial, par);
  }

  //Wrapping - mol weight: 62.046440
  nel = 3;
  density = 1.200000; //medio inventada en base a datos de plásticos
  TGeoMixture *pWrappingMaterial = new TGeoMixture("Wrapping", nel, density);
  
  aMat = 12.000000; z = 6.000000; w = 0.386807; // C
  pWrappingMaterial->DefineElement(0,aMat,z,w);    
  
  aMat = 1.007940; z = 1.000000; w = 0.097470; //H
  pWrappingMaterial->DefineElement(1,aMat,z,w);
  
  aMat = 15.999400; z = 8.000000; w = 0.515723; //O
  pWrappingMaterial->DefineElement(2,aMat,z,w);
  
  numed = 802;    
  pWrappingMaterial->SetIndex(numed);
  Double_t par[8];
  par[0]  = 0.000000; // isvol
  par[1]  = 0.000000; // ifield
  par[2]  = 0.000000; // fieldm
  par[3]  = 0.000000; // tmaxfd
  par[4]  = 0.000000; // stemax
  par[5]  = 0.000000; // deemax
  par[6]  = 0.000100; // epsil
  par[7]  = 0.000000; // stmin
  
  TGeoMedium *pWrappingMedium = new TGeoMedium("Wrapping", numed, pWrappingMaterial, par);
  
  
  
  //WORLD
  TGeoVolume *top=gGeoManager->MakeBox("World",pAirMedium,400,400,400);
  gGeoManager->SetTopVolume(top);
  gGeoManager->SetTopVisible(kFALSE);
  
  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();
  //pAWorld->SetVisLeaves(kTRUE);
  
  // Defintion of the Mother Volume
  Double_t length = 300.;
  
  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);
  
  TGeoVolume* pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);
  pWorld->SetVisibility(0); 
  pWorld->SetVisLeaves(kTRUE);
  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  
  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);
  //gGeoManager->SetTopVisible(0);
  
  //Crystals with and without wrapping
  Double_t Wthickness = 0.01; //100 micras promedio
  
  Double_t* vertices=new Double_t[16];
  Double_t* vertices2=new Double_t[16];
  vertices[0] = 0; vertices[1] = 0;         vertices2[0] = 0 + Wthickness; vertices2[1] = 0 + Wthickness;
  vertices[2] = 2.36; vertices[3] = 0;      vertices2[2] = 2.36 - Wthickness; vertices2[3] = 0 + Wthickness;
  vertices[4] = 2.36; vertices[5] = 1.27;   vertices2[4] = 2.36 - Wthickness; vertices2[5] = 1.27 - Wthickness;
  vertices[6] = 0; vertices[7] = 1.27;      vertices2[6] = 0 + Wthickness; vertices2[7] = 1.27 - Wthickness;
  vertices[8] = 0; vertices[9] = 0;         vertices2[8] = 0 + Wthickness; vertices2[9] = 0 + Wthickness;
  vertices[10] = 3.25; vertices[11] = 0;    vertices2[10] = 3.25 - Wthickness; vertices2[11] = 0 + Wthickness;
  vertices[12] = 3.25; vertices[13] = 1.74; vertices2[12] = 3.25 - Wthickness; vertices2[13] = 1.74 - Wthickness;
  vertices[14] = 0; vertices[15] = 1.74;    vertices2[14] = 0 + Wthickness; vertices2[15] = 1.74 - Wthickness;
  
  TGeoVolume *CrystalWithWrapping_1_1 = gGeoManager->MakeArb8("CrystalWithWrapping_1_1", pWrappingMedium,9 + Wthickness, vertices);  
  CrystalWithWrapping_1_1->SetVisibility(1); 
  CrystalWithWrapping_1_1->SetVisLeaves(kTRUE);
  CrystalWithWrapping_1_1->SetLineColor(kGreen);
  TGeoVolume *Crystal_1_1 = gGeoManager->MakeArb8("Crystal_1_1", pCsIMedium,9, vertices2);
  Crystal_1_1->SetLineColor(kRed);
  
  //Some common geometrical operations
  TGeoRotation *rotUni = new TGeoRotation();          //unitary rotation 
  TGeoTranslation* noTrans=new TGeoTranslation("noTrans",0.,0.,0.);
  TGeoRotation *rotSymmetric = new TGeoRotation(); //Symmetric crystal
  rotSymmetric->RotateZ(180);
  
  CrystalWithWrapping_1_1->AddNode(Crystal_1_1,1,new TGeoCombiTrans(0,0,0,rotUni));
  AddSensitiveVolume(Crystal_1_1); 
  
  pWorld->AddNode(CrystalWithWrapping_1_1,1,new TGeoCombiTrans(0,0,0,rotUni)); 
}


ClassImp(EnsarMyDet)
