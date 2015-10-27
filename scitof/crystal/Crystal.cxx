// -----------------------------------------------------------------------
// -----                   Crystal source file                    -----
// -----               Created 11/09/12  by P.Cabanelas              -----
// -----------------------------------------------------------------------
#include "Crystal.h"

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
Crystal::Crystal() : EnsarDetector("Crystal", kTRUE, kMYDET)
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
Crystal::Crystal(const char* name, Bool_t active)
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
Crystal::~Crystal()
{

  if ( flGeoPar ) delete flGeoPar;
  if (fMyDetCollection) {
    fMyDetCollection->Delete();
    delete fMyDetCollection;
  }
}
// -------------------------------------------------------------------------
void Crystal::Initialize()
{
  FairDetector::Initialize();

  cout << endl;
  cout << "-I- Crystal: initialisation" << endl;
  cout << "-I- Crystal: Vol (McId) def." << endl;

/* ==============================================
  Char_t buffer[126];
  for (Int_t i=0; i<30; i++ ) {
    sprintf(buffer,"crystalLog%i",i+1);
    cout << "-I- Crystal: Crystal Nb   : " << i << " connected to (McId) ---> " <<  gMC->VolId(buffer)<< endl;
    fCrystalType[i] = gMC->VolId(buffer);
  }
============================================== */

  TGeoVolume *vol = gGeoManager->GetVolume("World");
  vol->SetVisibility(kFALSE);


}


void Crystal::SetSpecialPhysicsCuts()
{

  cout << endl;

  cout << "-I- Crystal: Adding customized Physics cut ... " << endl;
  cout << "-I- Crystal: Yet not implemented !... " << endl;

  cout << endl;

}




// -----   Public method ProcessHits  --------------------------------------
Bool_t Crystal::ProcessHits(FairVolume* vol)
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
//void Crystal::SaveGeoParams(){
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
void Crystal::BeginEvent()
{

//  if (! kGeoSaved ) {
//      SaveGeoParams();
//  cout << "-I STS geometry parameters saved " << endl;
//  kGeoSaved = kTRUE;
//  }

}
// -----   Public method EndOfEvent   -----------------------------------------
void Crystal::EndOfEvent()
{

  if (fVerboseLevel) Print();

  fMyDetCollection->Clear();

  ResetParameters();

}
// ----------------------------------------------------------------------------



// -----   Public method Register   -------------------------------------------
void Crystal::Register()
{

  FairRootManager::Instance()->Register("MyDetPoint", GetName(), fMyDetCollection, kTRUE);

}
// ----------------------------------------------------------------------------



// -----   Public method GetCollection   --------------------------------------
TClonesArray* Crystal::GetCollection(Int_t iColl) const
{

  if (iColl == 0) return fMyDetCollection;
  else return NULL;

}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void Crystal::Print() const
{

  Int_t nHits = fMyDetCollection->GetEntriesFast();
  cout << "-I- Crystal: " << nHits << " points registered in this event." << endl;

}
// ----------------------------------------------------------------------------



// -----   Public method Reset   ----------------------------------------------
void Crystal::Reset()
{

  fMyDetCollection->Clear();
  ResetParameters();

}
// ----------------------------------------------------------------------------



// -----   Public method CopyClones   -----------------------------------------
void Crystal::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{

  Int_t nEntries = cl1->GetEntriesFast();
  cout << "-I- Crystal: " << nEntries << " entries to add." << endl;
  TClonesArray& clref = *cl2;
  EnsarMyDetPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (EnsarMyDetPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) EnsarMyDetPoint(*oldpoint);
    fPosIndex++;
  }
  cout << " -I- Crystal: " << cl2->GetEntriesFast() << " merged entries." << endl;

}

// -----   Private method AddHit   --------------------------------------------
EnsarMyDetPoint* Crystal::AddHit(Int_t trackID, Int_t volumeID,
                              Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                              TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
                              Double_t time, Double_t length, Double_t eLoss)
{
  TClonesArray& clref = *fMyDetCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1)
    cout << "-I- Crystal: Adding Point at (" << posIn.X() << ", " << posIn.Y()
         << ", " << posIn.Z() << ") cm,  active volume " << volumeID << ", track "
         << trackID << ", energy loss " << eLoss*1e06 << " keV" << endl;
  return new(clref[size]) EnsarMyDetPoint(trackID, volumeID, trackPID, parentTrackID, uniqueID, posIn, posOut,
                                       momIn, momOut, time, length, eLoss);
}



// -----   Private method createMatrix --------------------------------------------
TGeoRotation* Crystal::createMatrix( Double_t phi, Double_t theta, Double_t psi)
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
void Crystal::ConstructGeometry(int type)
{
 if (type == 0) { //CeBr3_EnsarMyDet
  /* ===============================================================================

  // **************************************************************************** //
  // Material definition

  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Mixture: CsI
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2;
    density = 4.510000;
    TGeoMixture*
    pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS
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

  // Mixture: Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel     = 2;
    density = 0.001290;
    TGeoMixture*
    pAirMaterial = new TGeoMixture("Air", nel,density);
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
    pAirMedium = new TGeoMedium("Air", numed,pAirMaterial, par);
  }


  //WORLD

  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

  // Defintion of the Mother Volume

  Double_t length = 300.;

  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);

  TGeoVolume*
  pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

#include "myDetector.geo"

======================================================================== */

 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 //TGeoMaterial *matAl  = new TGeoMaterial("Al", 26.98,13,2.7);
 //TGeoMaterial *matCu  = new TGeoMaterial("Cu",63.549,29,8.92);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 //TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 //TGeoMedium *Cu     = new TGeoMedium("Cu Medium",2, matCu);

 //TGeoVolume *al_tub = gGeoManager->MakeTube("AL_TUB", Al, 0.5, 20, 20);
 //TGeoVolume *cu_tub = gGeoManager->MakeTube("CU_TUB", Cu, 0.1, 0.5, 25);



// Mixture: CsI
//TGeoMaterial *matCsI  = new TGeoMaterial("CsI");
//TGeoMedium *CsI     = new TGeoMedium("CsI Medium",2, matCsI);
    int nel     = 2; //number of mixted material
    double density = 5.10000;
    TGeoMixture* pCeBr3Material = new TGeoMixture("CeBr3n", nel,density);
    double aMat = 140.116,   z = 58.000000,   w = 0.368893;  // Ce, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCeBr3Material->DefineElement(0,aMat,z,w);//http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=141
    aMat = 79.904000;   z = 35.000000;   w = 0.631107;  // Br
    pCeBr3Material->DefineElement(1,aMat,z,w);
    int numed = 801;
    pCeBr3Material->SetIndex(numed);
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    TGeoMedium *pCeBr3Medium = new TGeoMedium("CeBr3n", numed,pCeBr3Material, par);

TGeoVolume *crystal = gGeoManager->MakePara("CRYSTAL", pCeBr3Medium, 0.9, 0.9, 1.25,0,0,0);
crystal->SetFillColor(2);
crystal->SetLineColor(2);










 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,100,100,100);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 
 /*al_tub->SetFillColor(16);
 al_tub->SetLineColor(16);
 cu_tub->SetFillColor(48);
 cu_tub->SetLineColor(48);*/

// top->AddNodeOverlap(al_tub,1,new TGeoTranslation(0,0,0));
// top->AddNodeOverlap(cu_tub,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(crystal,1,new TGeoTranslation(0,0,0));

// AddSensitiveVolume(al_tub);
// AddSensitiveVolume(cu_tub);
AddSensitiveVolume(crystal);
 } else if (type == 1) { //CeBr3_Al1mm5_EnsarMyDet
/* ===============================================================================

  // **************************************************************************** //
  // Material definition

  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Mixture: CsI
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2;
    density = 4.510000;
    TGeoMixture*
    pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS
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

  // Mixture: Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel     = 2;
    density = 0.001290;
    TGeoMixture*
    pAirMaterial = new TGeoMixture("Air", nel,density);
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
    pAirMedium = new TGeoMedium("Air", numed,pAirMaterial, par);
  }


  //WORLD

  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

  // Defintion of the Mother Volume

  Double_t length = 300.;

  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);

  TGeoVolume*
  pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

#include "myDetector.geo"

======================================================================== */

 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 TGeoMaterial *matAl  = new TGeoMaterial("Al", 26.98,13,2.7);
 //TGeoMaterial *matCu  = new TGeoMaterial("Cu",63.549,29,8.92);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 //TGeoMedium *Cu     = new TGeoMedium("Cu Medium",2, matCu);

 //TGeoVolume *al_tub = gGeoManager->MakeTube("AL_TUB", Al, 0.5, 20, 20);
 //TGeoVolume *cu_tub = gGeoManager->MakeTube("CU_TUB", Cu, 0.1, 0.5, 25);



// Mixture: CsI
//TGeoMaterial *matCsI  = new TGeoMaterial("CsI");
//TGeoMedium *CsI     = new TGeoMedium("CsI Medium",2, matCsI);
    int nel     = 2; //number of mixted material
    double density = 5.10000;
    TGeoMixture* pCeBr3Material = new TGeoMixture("CeBr3n", nel,density);
    double aMat = 140.116,   z = 58.000000,   w = 0.368893;  // Ce, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCeBr3Material->DefineElement(0,aMat,z,w);//http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=141
    aMat = 79.904000;   z = 35.000000;   w = 0.631107;  // Br
    pCeBr3Material->DefineElement(1,aMat,z,w);
    int numed = 801;
    pCeBr3Material->SetIndex(numed);
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    TGeoMedium *pCeBr3Medium = new TGeoMedium("CeBr3n", numed,pCeBr3Material, par);

TGeoVolume *crystal = gGeoManager->MakePara("CRYSTAL", pCeBr3Medium, 0.9, 0.9, 1.25,0,0,0);
TGeoVolume *al_foil = gGeoManager->MakePara("Al_foil",Al, 0.9,0.9,0.075,0,0,0);

al_foil->SetFillColor(16);
al_foil->SetLineColor(16);
crystal->SetFillColor(5);
crystal->SetLineColor(5);



 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,100,100,100);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 
 /*al_tub->SetFillColor(16);
 al_tub->SetLineColor(16);
 cu_tub->SetFillColor(48);
 cu_tub->SetLineColor(48);*/

// top->AddNodeOverlap(al_tub,1,new TGeoTranslation(0,0,0));
// top->AddNodeOverlap(cu_tub,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(crystal,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(al_foil,1,new TGeoTranslation(0,0,-1.325));

// AddSensitiveVolume(al_tub);
// AddSensitiveVolume(cu_tub);

//AddSensitiveVolume(al_foil);
AddSensitiveVolume(crystal);
 } else if (type == 2) { //CeBr3_Al5mm_EnsarMyDet
/* ===============================================================================

  // **************************************************************************** //
  // Material definition

  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Mixture: CsI
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2;
    density = 4.510000;
    TGeoMixture*
    pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS
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

  // Mixture: Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel     = 2;
    density = 0.001290;
    TGeoMixture*
    pAirMaterial = new TGeoMixture("Air", nel,density);
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
    pAirMedium = new TGeoMedium("Air", numed,pAirMaterial, par);
  }


  //WORLD

  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

  // Defintion of the Mother Volume

  Double_t length = 300.;

  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);

  TGeoVolume*
  pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

#include "myDetector.geo"

======================================================================== */

 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 TGeoMaterial *matAl  = new TGeoMaterial("Al", 26.98,13,2.7);
 //TGeoMaterial *matCu  = new TGeoMaterial("Cu",63.549,29,8.92);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 //TGeoMedium *Cu     = new TGeoMedium("Cu Medium",2, matCu);

 //TGeoVolume *al_tub = gGeoManager->MakeTube("AL_TUB", Al, 0.5, 20, 20);
 //TGeoVolume *cu_tub = gGeoManager->MakeTube("CU_TUB", Cu, 0.1, 0.5, 25);



// Mixture: CsI
//TGeoMaterial *matCsI  = new TGeoMaterial("CsI");
//TGeoMedium *CsI     = new TGeoMedium("CsI Medium",2, matCsI);
    int nel     = 2; //number of mixted material
    double density = 5.10000;
    TGeoMixture* pCeBr3Material = new TGeoMixture("CeBr3n", nel,density);
    double aMat = 140.116,   z = 58.000000,   w = 0.368893;  // Ce, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCeBr3Material->DefineElement(0,aMat,z,w);//http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=141
    aMat = 79.904000;   z = 35.000000;   w = 0.631107;  // Br
    pCeBr3Material->DefineElement(1,aMat,z,w);
    int numed = 801;
    pCeBr3Material->SetIndex(numed);
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    TGeoMedium *pCeBr3Medium = new TGeoMedium("CeBr3n", numed,pCeBr3Material, par);

TGeoVolume *crystal = gGeoManager->MakePara("CRYSTAL", pCeBr3Medium, 0.9, 0.9, 1.25,0,0,0);
TGeoVolume *al_foil = gGeoManager->MakePara("Al_foil",Al, 0.9,0.9,0.25,0,0,0);

al_foil->SetFillColor(16);
al_foil->SetLineColor(16);
crystal->SetFillColor(5);
crystal->SetLineColor(5);



 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,100,100,100);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 
 /*al_tub->SetFillColor(16);
 al_tub->SetLineColor(16);
 cu_tub->SetFillColor(48);
 cu_tub->SetLineColor(48);*/

// top->AddNodeOverlap(al_tub,1,new TGeoTranslation(0,0,0));
// top->AddNodeOverlap(cu_tub,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(crystal,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(al_foil,1,new TGeoTranslation(0,0,-1.50));

// AddSensitiveVolume(al_tub);
// AddSensitiveVolume(cu_tub);

//AddSensitiveVolume(al_foil);
AddSensitiveVolume(crystal);
 } else if (type == 3) { //CsI_EnsarMyDet
/* ===============================================================================

  // **************************************************************************** //
  // Material definition

  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Mixture: CsI
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2;
    density = 4.510000;
    TGeoMixture*
    pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS
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

  // Mixture: Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel     = 2;
    density = 0.001290;
    TGeoMixture*
    pAirMaterial = new TGeoMixture("Air", nel,density);
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
    pAirMedium = new TGeoMedium("Air", numed,pAirMaterial, par);
  }


  //WORLD

  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

  // Defintion of the Mother Volume

  Double_t length = 300.;

  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);

  TGeoVolume*
  pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

#include "myDetector.geo"

======================================================================== */

 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 //TGeoMaterial *matAl  = new TGeoMaterial("Al", 26.98,13,2.7);
 //TGeoMaterial *matCu  = new TGeoMaterial("Cu",63.549,29,8.92);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 //TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 //TGeoMedium *Cu     = new TGeoMedium("Cu Medium",2, matCu);

 //TGeoVolume *al_tub = gGeoManager->MakeTube("AL_TUB", Al, 0.5, 20, 20);
 //TGeoVolume *cu_tub = gGeoManager->MakeTube("CU_TUB", Cu, 0.1, 0.5, 25);



// Mixture: CsI
//TGeoMaterial *matCsI  = new TGeoMaterial("CsI");
//TGeoMedium *CsI     = new TGeoMedium("CsI Medium",2, matCsI);
    int nel     = 2; //number of mixted material
    double density = 4.510000;
    TGeoMixture* pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    double aMat = 132.905450,   z = 55.000000,   w = 0.511549;  // CS, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCsIMaterial->DefineElement(0,aMat,z,w);//http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=141
    aMat = 126.904470;   z = 53.000000;   w = 0.488451;  // I
    pCsIMaterial->DefineElement(1,aMat,z,w);
    int numed = 801;
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
    TGeoMedium *pCsIMedium = new TGeoMedium("CsIn", numed,pCsIMaterial, par);

TGeoVolume *crystal = gGeoManager->MakePara("CRYSTAL", pCsIMedium, 0.9, 0.9, 1.25,0,0,0); //Cystal Volume Definition. Paralelepípedo. Dimensiones definidas desde el centro del cristal: Ancho=0.9*2, Alto= 0.9*2, Largo=1.25*2. Ángulo formado entre las caras: 0,0,0. Es decir, paralelepídedo regular
crystal->SetFillColor(2);
crystal->SetLineColor(2);










 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,100,100,100);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 
 /*al_tub->SetFillColor(16);
 al_tub->SetLineColor(16);
 cu_tub->SetFillColor(48);
 cu_tub->SetLineColor(48);*/

// top->AddNodeOverlap(al_tub,1,new TGeoTranslation(0,0,0));
// top->AddNodeOverlap(cu_tub,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(crystal,1,new TGeoTranslation(0,0,0));

// AddSensitiveVolume(al_tub);
// AddSensitiveVolume(cu_tub);
AddSensitiveVolume(crystal);
 } else if (type == 4) { //CsI_Al1mm5_EnsarMyDet
/* ===============================================================================

  // **************************************************************************** //
  // Material definition

  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Mixture: CsI
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2;
    density = 4.510000;
    TGeoMixture*
    pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS
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

  // Mixture: Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel     = 2;
    density = 0.001290;
    TGeoMixture*
    pAirMaterial = new TGeoMixture("Air", nel,density);
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
    pAirMedium = new TGeoMedium("Air", numed,pAirMaterial, par);
  }


  //WORLD

  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

  // Defintion of the Mother Volume

  Double_t length = 300.;

  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);

  TGeoVolume*
  pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

#include "myDetector.geo"

======================================================================== */

 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 TGeoMaterial *matAl  = new TGeoMaterial("Al", 26.98,13,2.7);
 //TGeoMaterial *matCu  = new TGeoMaterial("Cu",63.549,29,8.92);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 //TGeoMedium *Cu     = new TGeoMedium("Cu Medium",2, matCu);

 //TGeoVolume *al_tub = gGeoManager->MakeTube("AL_TUB", Al, 0.5, 20, 20);
 //TGeoVolume *cu_tub = gGeoManager->MakeTube("CU_TUB", Cu, 0.1, 0.5, 25);



// Mixture: CsI
//TGeoMaterial *matCsI  = new TGeoMaterial("CsI");
//TGeoMedium *CsI     = new TGeoMedium("CsI Medium",2, matCsI);
    int nel     = 2; //number of mixted material
    double density = 4.510000;
    TGeoMixture* pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    double aMat = 132.905450,   z = 55.000000,   w = 0.511549;  // CS, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCsIMaterial->DefineElement(0,aMat,z,w);//http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=141
    aMat = 126.904470;   z = 53.000000;   w = 0.488451;  // I
    pCsIMaterial->DefineElement(1,aMat,z,w);
    int numed = 801;
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
    TGeoMedium *pCsIMedium = new TGeoMedium("CsIn", numed,pCsIMaterial, par);

TGeoVolume *crystal = gGeoManager->MakePara("CRYSTAL", pCsIMedium, 0.9, 0.9, 1.25,0,0,0); //Cystal Volume Definition. Paralelepípedo. Dimensiones definidas desde el centro del cristal: Ancho=0.9*2, Alto= 0.9*2, Largo=1.25*2. Ángulo formado entre las caras: 0,0,0. Es decir, paralelepídedo regular
TGeoVolume *al_foil = gGeoManager->MakePara("Al_foil",Al, 0.9,0.9,0.075,0,0,0);


al_foil->SetFillColor(16);
al_foil->SetLineColor(16);
crystal->SetFillColor(2);
crystal->SetLineColor(2);




 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,100,100,100);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 
 /*al_tub->SetFillColor(16);
 al_tub->SetLineColor(16);
 cu_tub->SetFillColor(48);
 cu_tub->SetLineColor(48);*/

// top->AddNodeOverlap(al_tub,1,new TGeoTranslation(0,0,0));
// top->AddNodeOverlap(cu_tub,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(crystal,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(al_foil,1,new TGeoTranslation(0,0,-1.325));

// AddSensitiveVolume(al_tub);
// AddSensitiveVolume(cu_tub);
// AddSensitiveVolume(al_foil);
AddSensitiveVolume(crystal);
 } else if (type == 5) { //CsI_Al5mm_EnsarMyDet
/* ===============================================================================

  // **************************************************************************** //
  // Material definition

  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Mixture: CsI
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2;
    density = 4.510000;
    TGeoMixture*
    pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS
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

  // Mixture: Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel     = 2;
    density = 0.001290;
    TGeoMixture*
    pAirMaterial = new TGeoMixture("Air", nel,density);
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
    pAirMedium = new TGeoMedium("Air", numed,pAirMaterial, par);
  }


  //WORLD

  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

  // Defintion of the Mother Volume

  Double_t length = 300.;

  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);

  TGeoVolume*
  pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

#include "myDetector.geo"

======================================================================== */

 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 TGeoMaterial *matAl  = new TGeoMaterial("Al", 26.98,13,2.7);
 //TGeoMaterial *matCu  = new TGeoMaterial("Cu",63.549,29,8.92);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 //TGeoMedium *Cu     = new TGeoMedium("Cu Medium",2, matCu);

 //TGeoVolume *al_tub = gGeoManager->MakeTube("AL_TUB", Al, 0.5, 20, 20);
 //TGeoVolume *cu_tub = gGeoManager->MakeTube("CU_TUB", Cu, 0.1, 0.5, 25);



// Mixture: CsI
//TGeoMaterial *matCsI  = new TGeoMaterial("CsI");
//TGeoMedium *CsI     = new TGeoMedium("CsI Medium",2, matCsI);
    int nel     = 2; //number of mixted material
    double density = 4.510000;
    TGeoMixture* pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    double aMat = 132.905450,   z = 55.000000,   w = 0.511549;  // CS, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCsIMaterial->DefineElement(0,aMat,z,w);//http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=141
    aMat = 126.904470;   z = 53.000000;   w = 0.488451;  // I
    pCsIMaterial->DefineElement(1,aMat,z,w);
    int numed = 801;
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
    TGeoMedium *pCsIMedium = new TGeoMedium("CsIn", numed,pCsIMaterial, par);

TGeoVolume *crystal = gGeoManager->MakePara("CRYSTAL", pCsIMedium, 0.9, 0.9, 1.25,0,0,0); //Cystal Volume Definition. Paralelepípedo. Dimensiones definidas desde el centro del cristal: Ancho=0.9*2, Alto= 0.9*2, Largo=1.25*2. Ángulo formado entre las caras: 0,0,0. Es decir, paralelepídedo regular
TGeoVolume *al_foil = gGeoManager->MakePara("Al_foil",Al, 0.9,0.9,0.25,0,0,0);


al_foil->SetFillColor(16);
al_foil->SetLineColor(16);
crystal->SetFillColor(2);
crystal->SetLineColor(2);




 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,100,100,100);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 
 /*al_tub->SetFillColor(16);
 al_tub->SetLineColor(16);
 cu_tub->SetFillColor(48);
 cu_tub->SetLineColor(48);*/

// top->AddNodeOverlap(al_tub,1,new TGeoTranslation(0,0,0));
// top->AddNodeOverlap(cu_tub,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(crystal,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(al_foil,1,new TGeoTranslation(0,0,-1.50));

// AddSensitiveVolume(al_tub);
// AddSensitiveVolume(cu_tub);
// AddSensitiveVolume(al_foil);
AddSensitiveVolume(crystal);
 } else if (type == 6) { //CsI2_EnsarMyDet
/* ===============================================================================

  // **************************************************************************** //
  // Material definition

  Double_t aMat;
  Double_t z, density, w;
  Int_t nel, numed;

  // Mixture: CsI
  TGeoMedium * pCsIMedium=NULL;
  if (gGeoManager->GetMedium("CsI") ) {
    pCsIMedium=gGeoManager->GetMedium("CsI");
    //if (gGeoManager->GetMedium("aluminium") ){
    //  pCsIMedium=gGeoManager->GetMedium("aluminium");
  } else {
    nel     = 2;
    density = 4.510000;
    TGeoMixture*
    pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    aMat = 132.905450;   z = 55.000000;   w = 0.511549;  // CS
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

  // Mixture: Air
  TGeoMedium * pAirMedium=NULL;
  if (gGeoManager->GetMedium("Air") ) {
    pAirMedium=gGeoManager->GetMedium("Air");
  } else {
    nel     = 2;
    density = 0.001290;
    TGeoMixture*
    pAirMaterial = new TGeoMixture("Air", nel,density);
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
    pAirMedium = new TGeoMedium("Air", numed,pAirMaterial, par);
  }


  //WORLD

  TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();

  // Defintion of the Mother Volume

  Double_t length = 300.;

  TGeoShape *pCBWorld = new TGeoBBox("Califa_box",
                                     length/2.0,
                                     length/2.0,
                                     length/2.0);

  TGeoVolume*
  pWorld  = new TGeoVolume("CalifaWorld",pCBWorld, pAirMedium);

  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);

  // add the sphere as Mother Volume
  pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

#include "myDetector.geo"

======================================================================== */

 TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
 //TGeoMaterial *matAl  = new TGeoMaterial("Al", 26.98,13,2.7);
 //TGeoMaterial *matCu  = new TGeoMaterial("Cu",63.549,29,8.92);

 TGeoMedium *Vacuum = new TGeoMedium("Vacuum Medium",0,vacuum);
 //TGeoMedium *Al     = new TGeoMedium("Al Medium",2, matAl);
 //TGeoMedium *Cu     = new TGeoMedium("Cu Medium",2, matCu);

 //TGeoVolume *al_tub = gGeoManager->MakeTube("AL_TUB", Al, 0.5, 20, 20);
 //TGeoVolume *cu_tub = gGeoManager->MakeTube("CU_TUB", Cu, 0.1, 0.5, 25);



// Mixture: CsI
//TGeoMaterial *matCsI  = new TGeoMaterial("CsI");
//TGeoMedium *CsI     = new TGeoMedium("CsI Medium",2, matCsI);
    int nel     = 2; //number of mixted material
    double density = 4.510000;
    TGeoMixture* pCsIMaterial = new TGeoMixture("CsIn", nel,density);
    double aMat = 132.905450,   z = 55.000000,   w = 0.511549;  // CS, aMat= Standard atomic weigth, w= Fraction by weight (per atomic number z=55)
    pCsIMaterial->DefineElement(0,aMat,z,w);//http://physics.nist.gov/cgi-bin/Star/compos.pl?matno=141
    aMat = 126.904470;   z = 53.000000;   w = 0.488451;  // I
    pCsIMaterial->DefineElement(1,aMat,z,w);
    int numed = 801;
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
    TGeoMedium *pCsIMedium = new TGeoMedium("CsIn", numed,pCsIMaterial, par);

TGeoVolume *crystal = gGeoManager->MakePara("CRYSTAL", pCsIMedium, 0.9, 0.9, 1.25,0,0,0);
crystal->SetFillColor(2);
crystal->SetLineColor(2);










 TGeoVolume *top=gGeoManager->MakeBox("World",Vacuum,100,100,100);
 gGeoManager->SetTopVolume(top);
 gGeoManager->SetTopVisible(0);
 
 /*al_tub->SetFillColor(16);
 al_tub->SetLineColor(16);
 cu_tub->SetFillColor(48);
 cu_tub->SetLineColor(48);*/

// top->AddNodeOverlap(al_tub,1,new TGeoTranslation(0,0,0));
// top->AddNodeOverlap(cu_tub,1,new TGeoTranslation(0,0,0));
top->AddNodeOverlap(crystal,1,new TGeoTranslation(0,0,0));

// AddSensitiveVolume(al_tub);
// AddSensitiveVolume(cu_tub);
AddSensitiveVolume(crystal);
 } else {
 }
}


ClassImp(Crystal)
