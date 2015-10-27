// -------------------------------------------------------------------------
// -----                        R3BTof source file                     -----
// -----                  Created 04/05/12  by J.L.Rodriguez           -----
// -------------------------------------------------------------------------
#include "R3BTof.h"

#include "R3BGeoTof.h"
#include "R3BTofPoint.h"
#include "R3BGeoTofPar.h"

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

// includes for modeling
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoBBox.h"
#include "TGeoPara.h"
#include "TGeoPgon.h"
#include "TGeoSphere.h"
#include "TGeoArb8.h"
#include "TGeoCone.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;



// -----   Default constructor   -------------------------------------------
R3BTof::R3BTof() : R3BDetector("R3BTof", kTRUE, kTOF) {
  ResetParameters();
  fTofCollection = new TClonesArray("R3BTofPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fCutE=1.0e-03; // 1 MeV default
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BTof::R3BTof(const char* name, Bool_t active) 
  : R3BDetector(name, active, kTOF) {
  ResetParameters();
  fTofCollection = new TClonesArray("R3BTofPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fCutE=1.0e-03; // 1MeV default
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BTof::~R3BTof() {

  if ( flGeoPar ) delete flGeoPar;
  if (fTofCollection) {
    fTofCollection->Delete();
    delete fTofCollection;
  }
}
// -------------------------------------------------------------------------


void R3BTof::Initialize()
{
  FairDetector::Initialize();

   cout << endl;
   cout << "-I- R3BTof: initialisation" << endl;
   cout << "-I- R3BTof: Sci. Vol. (McId) " << gMC->VolId("Tof_World")<< endl;

}


void R3BTof::SetSpecialPhysicsCuts(){

   cout << endl;

   cout << "-I- R3BTof: Adding customized Physics cut ... " << endl;

   if (gGeoManager) {
     TGeoMedium* pSi = gGeoManager->GetMedium("plasticForTOF");
     if ( pSi ) {
      // Setting processes for Si only
	 gMC->Gstpar(pSi->GetId()  ,"LOSS",3);
         gMC->Gstpar(pSi->GetId()  ,"STRA",1.0);
         gMC->Gstpar(pSi->GetId()  ,"PAIR",1.0);
	 gMC->Gstpar(pSi->GetId()  ,"COMP",1.0);
	 gMC->Gstpar(pSi->GetId()  ,"PHOT",1.0);
         gMC->Gstpar(pSi->GetId()  ,"ANNI",1.0);
	 gMC->Gstpar(pSi->GetId()  ,"BREM",1.0);
	 gMC->Gstpar(pSi->GetId()  ,"HADR",1.0);
         gMC->Gstpar(pSi->GetId()  ,"DRAY",1.0);
         gMC->Gstpar(pSi->GetId()  ,"DCAY",1.0);
         gMC->Gstpar(pSi->GetId()  ,"MULS",1.0);
	 gMC->Gstpar(pSi->GetId()  ,"RAYL",1.0);

	 // Setting Energy-CutOff for Si Only
	Double_t cutE = fCutE; // GeV-> 1 keV

	cout << "-I- R3BTof Scintillator Medium Id " << pSi->GetId()
	    << " Energy Cut-Off : " << cutE
	    << endl;
        cout << endl;
        //Si
	gMC->Gstpar(pSi->GetId(),"CUTGAM",cutE);   /** gammas (GeV)*/
        gMC->Gstpar(pSi->GetId(),"CUTELE",cutE);   /** electrons (GeV)*/
	gMC->Gstpar(pSi->GetId(),"CUTNEU",cutE);   /** neutral hadrons (GeV)*/
	gMC->Gstpar(pSi->GetId(),"CUTHAD",cutE);   /** charged hadrons (GeV)*/
	gMC->Gstpar(pSi->GetId(),"CUTMUO",cutE);   /** muons (GeV)*/
	gMC->Gstpar(pSi->GetId(),"BCUTE",cutE);    /** electron bremsstrahlung (GeV)*/
	gMC->Gstpar(pSi->GetId(),"BCUTM",cutE);    /** muon and hadron bremsstrahlung(GeV)*/
	gMC->Gstpar(pSi->GetId(),"DCUTE",cutE);    /** delta-rays by electrons (GeV)*/
	gMC->Gstpar(pSi->GetId(),"DCUTM",cutE);    /** delta-rays by muons (GeV)*/
        gMC->Gstpar(pSi->GetId(),"PPCUTM",-1.);   /** direct pair production by muons (GeV)*/

     }

 } //!gGeoManager


}



// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BTof::ProcessHits(FairVolume* vol) {

   // debug
    static Int_t evt;     

  // Simple Det PLane
    if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fTrackID  = gMC->TrackPid();
//    cout << "-I- evt: " << evt++ << " track entering " << endl;
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Set additional parameters at exit of active volume. Create R3BTofPoint.
  if ( gMC->IsTrackExiting()    ||
       gMC->IsTrackStop()       ||
       gMC->IsTrackDisappeared()   ) {
   // fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
    fVolumeID = vol->getMCid();
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
      
//       cout << "1st direction: " << olddirection[0] << "," << olddirection[1] << "," << olddirection[2] << endl;

      for (Int_t i=0; i<3; i++){
	newdirection[i] = -1*olddirection[i];
      }
      
      gGeoManager->SetCurrentDirection(newdirection);
     // TGeoNode *bla = gGeoManager->FindNextBoundary(2);
      safety = gGeoManager->GetSafeDistance();


      gGeoManager->SetCurrentDirection(-newdirection[0],-newdirection[1],-newdirection[2]);
      
      for (Int_t i=0; i<3; i++){
	newpos[i] = oldpos[i] - (3*safety*olddirection[i]);
      }

      if ( fPosIn.Z() < 30. && newpos[2] > 30.02 ) {
	cerr << "2nd direction: " << olddirection[0] << "," << olddirection[1] << "," << olddirection[2] 
	     << " with safety = " << safety << endl;
	cerr << "oldpos = " << oldpos[0] << "," << oldpos[1] << "," << oldpos[2] << endl;
	cerr << "newpos = " << newpos[0] << "," << newpos[1] << "," << newpos[2] << endl;
      }

      fPosOut.SetX(newpos[0]);
      fPosOut.SetY(newpos[1]);
      fPosOut.SetZ(newpos[2]);
    }

    AddHit(fTrackID, fVolumeID,
	   TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
	   TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
	   TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
	   TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
	   fTime*1000.0, fLength, fELoss);
    
    // Increment number of TofPoints for this track
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kTOF);
    
    ResetParameters();
  }

  return kTRUE;
}


// -----   Public method EndOfEvent   -----------------------------------------
void R3BTof::BeginEvent() {

//  if (! kGeoSaved ) {
//      SaveGeoParams();
//  cout << "-I STS geometry parameters saved " << endl;
//  kGeoSaved = kTRUE;
//  }

}
// -----   Public method EndOfEvent   -----------------------------------------
void R3BTof::EndOfEvent() {

  if (fVerboseLevel) Print();
  fTofCollection->Clear();

  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----   Public method Register   -------------------------------------------
void R3BTof::Register() {
  FairRootManager::Instance()->Register("Tof_FF", GetName(), fTofCollection, kTRUE);
}
// ----------------------------------------------------------------------------



// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BTof::GetCollection(Int_t iColl) const {
  if (iColl == 0) return fTofCollection;
  else return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void R3BTof::Print() const {
  Int_t nHits = fTofCollection->GetEntriesFast();
  cout << "-I- R3BTof: " << nHits << " points registered in this event." 
       << endl;
}
// ----------------------------------------------------------------------------



// -----   Public method Reset   ----------------------------------------------
void R3BTof::Reset() {
  fTofCollection->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----   Public method CopyClones   -----------------------------------------
void R3BTof::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  cout << "-I- R3BTof: " << nEntries << " entries to add." << endl;
  TClonesArray& clref = *cl2;
  R3BTofPoint* oldpoint = NULL;
   for (Int_t i=0; i<nEntries; i++) {
   oldpoint = (R3BTofPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) R3BTofPoint(*oldpoint);
    fPosIndex++;
  }
   cout << " -I- R3BTof: " << cl2->GetEntriesFast() << " merged entries."
       << endl;
}

// -----   Private method AddHit   --------------------------------------------
R3BTofPoint* R3BTof::AddHit(Int_t trackID, Int_t detID, TVector3 posIn,
			    TVector3 posOut, TVector3 momIn, 
			    TVector3 momOut, Double_t time, 
			    Double_t length, Double_t eLoss) {
  TClonesArray& clref = *fTofCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1) 
    cout << "-I- R3BTof: Adding Point at (" << posIn.X() << ", " << posIn.Y() 
	 << ", " << posIn.Z() << ") cm,  detector " << detID << ", track "
	 << trackID << ", energy loss " << eLoss*1e06 << " keV" << endl;
  return new(clref[size]) R3BTofPoint(trackID, detID, posIn, posOut,
				      momIn, momOut, time, length, eLoss);
}
// -----   Public method ConstructGeometry   ----------------------------------
void R3BTof::ConstructGeometry() {

  // out-of-file geometry definition
   Double_t dx,dy,dz;
   Double_t a;
   Double_t thx, phx, thy, phy, thz, phz;
   Double_t z, density, w;
   Int_t nel, numed;


/****************************************************************************/
// Material definition


  // Mixture: Air
  TGeoMedium * pMed2=NULL;
   if (gGeoManager->GetMedium("Air") ){
       pMed2=gGeoManager->GetMedium("Air");
   }else{
     nel     = 2;
     density = 0.001290;
     TGeoMixture*
	 pMat2 = new TGeoMixture("Air", nel,density);
     a = 14.006740;   z = 7.000000;   w = 0.700000;  // N
     pMat2->DefineElement(0,a,z,w);
     a = 15.999400;   z = 8.000000;   w = 0.300000;  // O
     pMat2->DefineElement(1,a,z,w);
     pMat2->SetIndex(1);
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
     pMed2 = new TGeoMedium("Air", numed,pMat2, par);
   }


  // Mixture: plasticForTOF
  TGeoMedium * pMed34=NULL;
   if (gGeoManager->GetMedium("plasticForTOF") ){
       pMed34=gGeoManager->GetMedium("plasticForTOF");
   }else{
     nel     = 2;
     density = 1.032000;
     TGeoMixture*
	 pMat34 = new TGeoMixture("plasticForTOF", nel,density);
     a = 12.010700;   z = 6.000000;   w = 0.914708;  // C
     pMat34->DefineElement(0,a,z,w);
     a = 1.007940;   z = 1.000000;   w = 0.085292;  // H
     pMat34->DefineElement(1,a,z,w);
     pMat34->SetIndex(33);
     // Medium: plasticForTOF
     numed   = 33;  // medium number
     Double_t par[8];
     par[0]  = 0.000000; // isvol
     par[1]  = 0.000000; // ifield
     par[2]  = 0.000000; // fieldm
     par[3]  = 0.000000; // tmaxfd
     par[4]  = 0.000000; // stemax
     par[5]  = 0.000000; // deemax
     par[6]  = 0.000100; // epsil
     par[7]  = 0.000000; // stmin
     pMed34 = new TGeoMedium("plasticForTOF", numed,pMat34,par);
   }


  // TRANSFORMATION MATRICES
   // Combi transformation: 
    dx = -91.0; 
    dy = 0.000000;
    dz = 739.400; 

   TGeoRotation *gRot = new TGeoRotation();
   gRot->RotateX(0.);
   gRot->RotateY(-13.000000);
   gRot->RotateZ(0.);

   TGeoCombiTrans*
   pMatrix2 = new TGeoCombiTrans("", dx,dy,dz,gRot);  //

   //Top Volume
   TGeoVolume* pWorld = gGeoManager->GetTopVolume();
   pWorld->SetVisLeaves(kTRUE);


   // SHAPES, VOLUMES AND GEOMETRICAL HIERARCHY
   // Shape: TOFBox type: TGeoBBox
   dx = 90.0000/2.0;
   dy = 60.0000/2.0;
   dz = 0.50000/2.0;
   TGeoShape *pTOFBox = new TGeoBBox("TOFBox", dx,dy,dz);
   // Volume: TOFLog
   TGeoVolume*
   pTOFLog = new TGeoVolume("Tof_World",pTOFBox, pMed34);
   pTOFLog->SetVisLeaves(kTRUE);

   TGeoCombiTrans *pGlobal = GetGlobalPosition(pMatrix2);

   if (pGlobal){
       pWorld->AddNode(pTOFLog, 0, pGlobal);
   }else{
       pWorld->AddNode(pTOFLog, 0, pMatrix2);
   }

   AddSensitiveVolume(pTOFLog);
   fNbOfSensitiveVol+=1;

}

ClassImp(R3BTof)
