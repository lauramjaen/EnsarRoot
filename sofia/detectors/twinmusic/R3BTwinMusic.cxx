// -------------------------------------------------------------------------
// -----                  R3BTwinMusic source file                     -----
// -----            Created 26/03/12  by J.L. Rodriguez                -----
// -----        				                       -----
// -------------------------------------------------------------------------
#include "R3BTwinMusic.h"

#include "R3BGeoTwinMusic.h"
#include "R3BTwinMusicPoint.h"
#include "R3BGeoTwinMusicPar.h"

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
#include "TRotation.h"

// includes for modeling
#include "TGeoManager.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TVector3.h"
#include "TGeoMatrix.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TGeoArb8.h"
#include "TGeoBBox.h"
#include "TGeoPgon.h"
#include "TGeoCone.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include "TGeoTube.h"
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"
#include <iostream>


using std::cout;
using std::cerr;
using std::endl;


// -----   Default constructor   -------------------------------------------
R3BTwinMusic::R3BTwinMusic() : R3BDetector("R3BTwinMusic", kTRUE, kCAL) {
  ResetParameters();
  ftwinCollection = new TClonesArray("R3BTwinMusicPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fGeoVersion = 1;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BTwinMusic::R3BTwinMusic(const char* name, Bool_t active) 
  : R3BDetector(name, active, kCAL) {
  ResetParameters();
  ftwinCollection = new TClonesArray("R3BTwinMusicPoint");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fGeoVersion = 1;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BTwinMusic::~R3BTwinMusic() {

  if ( flGeoPar ) delete flGeoPar;
  if (ftwinCollection) {
   ftwinCollection->Delete();
   delete ftwinCollection;
  }
}
// -------------------------------------------------------------------------

void R3BTwinMusic::Initialize()
{
  FairDetector::Initialize();

  cout << endl;
  cout << "-I- R3BTwinMusic initialisation" << endl;
  
  Char_t buffer[126];
  
    for (Int_t i=0;i<2;i++ ) {
     sprintf(buffer,"Detector%i",i+1);
     cout << "-I- R3BTwinMusic: Detector   : " << i << " connected to (McId) ---> " <<  gMC->VolId(buffer)<< endl;
     fDetectorType[i] = gMC->VolId(buffer);
    }



}


void R3BTwinMusic::SetSpecialPhysicsCuts(){

  cout << endl;

  cout << "-I- R3BTwinMusic: Adding customized Physics cut ... " << endl;
  cout << "-I- R3BTwinMusic: Yet not implemented !... " << endl;

  cout << endl;

}



// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BTwinMusic::ProcessHits(FairVolume* vol) {

   // Getting the Infos from Volumes
   Int_t cp1 = -1; Int_t volId1 = -1;
   // Ids
   const char* bufferName = gMC->CurrentVolName();
   volId1 =  gMC->CurrentVolID(cp1);

   
   Int_t twinId = 0;


  // cout<< "detector= " << GetDetectorType(volId1) <<endl;
	
	  if(GetDetectorType(volId1)!=-1){
		   twinId = GetDetectorType(volId1);
                   //cout<< "twin= "<< twinType << endl;
		   if(twinId>2) 
			   cout << "-E- R3BTwinMusic: Wrong number. " << endl;
	   }
	   

  if ( gMC->IsTrackEntering() ) {
    fELoss = 0.;
    fTime = gMC->TrackTime() * 1.0e09;    //[ns]
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fPdgId = gMC->TrackPid();

  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Set additional parameters at exit of active volume. Create R3BTwinMusicPoint.
  if ( gMC->IsTrackExiting() ||
     gMC->IsTrackStop() ||
     gMC->IsTrackDisappeared() ) {
    fTrackID = gMC->GetStack()->GetCurrentTrackNumber();
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

      //     cout << "1st direction: " << olddirection[0] << "," << olddirection[1] << "," << olddirection[2] << endl;

      for (Int_t i=0; i<3; i++){
        newdirection[i] = -1*olddirection[i];
      }

      gGeoManager->SetCurrentDirection(newdirection);
      //  TGeoNode *bla = gGeoManager->FindNextBoundary(2);
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

    AddHit(fTrackID, fVolumeID, twinId, fPdgId,
      TVector3(fPosIn.X(), fPosIn.Y(), fPosIn.Z()),
      TVector3(fPosOut.X(), fPosOut.Y(), fPosOut.Z()),
      TVector3(fMomIn.Px(), fMomIn.Py(), fMomIn.Pz()),
      TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
      fTime, fLength, fELoss);

    // Increment number of TwinMusicPoints for this track
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kCAL);

    ResetParameters();
  }
  return kTRUE;
}
// ----------------------------------------------------------------------------
/*
void R3BTwinMusic::SaveGeoParams(){
  cout << " -I Save STS geo params " << endl;

  TFolder *mf = (TFolder*) gDirectory->FindObjectAny("cbmroot");
  cout << " mf: " << mf << endl;
  TFolder *stsf = NULL;
  if (mf ) stsf = (TFolder*) mf->FindObjectAny(GetName());
  cout << " stsf: " << stsf << endl;
  if (stsf) stsf->Add( flGeoPar0 ) ;
  FairRootManager::Instance()->WriteFolder();
  mf->Write("cbmroot",TObject::kWriteDelete);
}
*/


// -----   Public method EndOfEvent   -----------------------------------------
void R3BTwinMusic::BeginEvent() {

//  if (! kGeoSaved ) {
//    SaveGeoParams();
//  cout << "-I STS geometry parameters saved " << endl;
//  kGeoSaved = kTRUE;
//  }

}
// -----   Public method EndOfEvent   -----------------------------------------
void R3BTwinMusic::EndOfEvent() {

  if (fVerboseLevel) Print();
  ftwinCollection->Clear();

  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----   Public method Register   -------------------------------------------
void R3BTwinMusic::Register() {
  FairRootManager::Instance()->Register("Twin_Music", GetName(), ftwinCollection, kTRUE);
}
// ----------------------------------------------------------------------------



// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BTwinMusic::GetCollection(Int_t iColl) const {
  if (iColl == 0) return ftwinCollection;
  else return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void R3BTwinMusic::Print() const {
  Int_t nHits = ftwinCollection->GetEntriesFast();
  cout << "-I- R3BTwinMusic: " << nHits << " points registered in this event." << endl;
}
// ----------------------------------------------------------------------------



// -----   Public method Reset   ----------------------------------------------
void R3BTwinMusic::Reset() {
  ftwinCollection->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----   Public method CopyClones   -----------------------------------------
void R3BTwinMusic::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  cout << "-I- R3BTwinMusic: " << nEntries << " entries to add." << endl;
  TClonesArray& clref = *cl2;
  R3BTwinMusicPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++)
  {
    oldpoint = (R3BTwinMusicPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) R3BTwinMusicPoint(*oldpoint);
    fPosIndex++;
  }
  cout << " -I- R3BTwinMusic: " << cl2->GetEntriesFast() << " merged entries." << endl;
}

// -----   Private method AddHit   --------------------------------------------
R3BTwinMusicPoint* R3BTwinMusic::AddHit(Int_t trackID, Int_t detID, Int_t type, Int_t cp, TVector3 posIn,
        TVector3 posOut, TVector3 momIn, 
        TVector3 momOut, Double_t time, 
        Double_t length, Double_t eLoss) {
  TClonesArray& clref = *ftwinCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1) 
    cout << "-I- R3BTwinMusic: Adding Point at (" << posIn.X() << ", " << posIn.Y() 
     << ", " << posIn.Z() << ") cm, detector " << detID << ", track "
     << trackID << ", energy loss " << eLoss*1e06 << " keV" << endl;
  return new(clref[size]) R3BTwinMusicPoint(trackID, detID, type, cp , posIn, posOut,
            momIn, momOut, time, length, eLoss);
}


void R3BTwinMusic::ConstructGeometry(){
 
    cout << endl;
    cout << "-I- R3BTwinMusic:: ConstructGeometry() "<< endl;
    cout << endl;

   Double_t dx,dy,dz,rmax,rmin;
    Double_t a;
    Double_t thx, phx, thy, phy, thz, phz;
    Double_t  phi1, phi2,dphi, phi;
    Double_t z, density, w;
    Int_t nel, numed;
    Int_t nedges, nz;

   // MATERIALS, MIXTURES AND TRACKING MEDIA

   // --  Material: Lead

   TGeoMedium   * pMedLead=NULL;
   TGeoMaterial * pMatLead=NULL;

   if (gGeoManager->GetMedium("Lead") ){
       cout << "-I- TGeoManager: Lead Medium already defined " << endl;
       pMedLead = gGeoManager->GetMedium("Lead");
   }else{
    cout << "-I- R3BTwinMusic: Lead Medium defined " << endl;
    a       = 207.190000;
    z       = 82.000000;
    density = 11.350000;
    pMatLead = new TGeoMaterial("Lead", a,z,density);
    pMatLead->SetIndex(600);
    numed   = 26;  // medium number
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pMedLead  = new TGeoMedium("Lead", numed,pMatLead, par);
   }

   // --  Material: Copper
   TGeoMedium * pMedCu=NULL;
   if (gGeoManager->GetMedium("Copper") ){
       cout << "-I- TGeoManager: Copper Medium already defined " << endl;
       pMedCu=gGeoManager->GetMedium("Copper");
   }else{
    cout << "-I- R3BTwinMusic: Copper Medium defined " << endl;
    a       = 63.540000;
    z       = 29.000000;
    density = 8.960000;
    TGeoMaterial*
	pMatCu = new TGeoMaterial("Copper", a,z,density);
    pMatCu->SetIndex(702);
    numed   = 24;  // medium number
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pMedCu = new TGeoMedium("Copper", numed,pMatCu, par);
   }

   // --  Material: Aluminium

   TGeoMedium   * pMedAl=NULL;
   TGeoMaterial * pMatAl=NULL;

   if (gGeoManager->GetMedium("aluminium") ){
       cout << "-I- TGeoManager: Aluminium Medium already defined " << endl;
       pMedAl = gGeoManager->GetMedium("aluminium");
   }else{
    cout << "-I- R3BTwinMusic: Aluminium Medium defined " << endl;
    a       = 26.980000;
    z       = 13.000000;
    density = 2.7000000;
    pMatAl = new TGeoMaterial("aluminium", a,z,density);
    pMatAl->SetIndex(601);
    numed   = 27;  // medium number
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pMedAl  = new TGeoMedium("aluminium", numed,pMatAl, par);
   }

   // --  Material: Iron 
 
   TGeoMedium * pMedFe=NULL;
   if (gGeoManager->GetMedium("Iron") ){
       cout << "-I- TGeoManager: Iron Medium already defined " << endl;
       pMedFe=gGeoManager->GetMedium("Iron");
   }else{
    cout << "-I- R3BTwinMusic: Iron Medium defined " << endl;
    a       = 55.850000;
    z       = 26.000000;
    density = 7.870000;
    TGeoMaterial*
	pMatFe = new TGeoMaterial("Iron", a,z,density);
    pMatFe->SetIndex(701);
    numed   = 23;  // medium number
    Double_t par[8];
    par[0]  = 0.000000; // isvol
    par[1]  = 0.000000; // ifield
    par[2]  = 0.000000; // fieldm
    par[3]  = 0.000000; // tmaxfd
    par[4]  = 0.000000; // stemax
    par[5]  = 0.000000; // deemax
    par[6]  = 0.000100; // epsil
    par[7]  = 0.000000; // stmin
    pMedFe = new TGeoMedium("Iron", numed,pMatFe, par);
   }

	// Mixture: Mylar
	TGeoMedium * pMedMylar=NULL;
	if (gGeoManager->GetMedium("mylar") ){
                cout << "-I- TGeoManager: Mylar Medium already defined " << endl;
		pMedMylar=gGeoManager->GetMedium("mylar");
	}else{ 
                cout << "-I- R3BTwinMusic: Mylar Medium defined " << endl;
		nel     = 3;
		density = 1.690000;
		TGeoMixture*
		pMat = new TGeoMixture("Mylar", nel,density);
		a = 12.010700;   z = 6.000000;   w = 0.844907;  // C
		pMat->DefineElement(0,a,z,w);
		a = 1.007940;   z = 1.000000;   w = 0.042543;  // H
		pMat->DefineElement(1,a,z,w);
		a = 15.999400;   z = 8.000000;   w = 0.112550;  // O
		pMat->DefineElement(2,a,z,w);
		// Medium: CarbonFibre
		numed   = 803;  // medium number
		pMat->SetIndex(numed);
		Double_t par[8];
		par[0]  = 0.000000; // isvol
		par[1]  = 0.000000; // ifield
		par[2]  = 0.000000; // fieldm
		par[3]  = 0.000000; // tmaxfd
		par[4]  = 0.000000; // stemax
		par[5]  = 0.000000; // deemax
		par[6]  = 0.000100; // epsil
		par[7]  = 0.000000; // stmin
		pMedMylar = new TGeoMedium("Mylar", numed,pMat,par);
	}  

   //-- Mixture: Air

   TGeoMedium * pMedAir=NULL;
   if (gGeoManager->GetMedium("Air") ){
       cout << "-I- TGeoManager: Air Medium already defined " << endl;
       pMedAir=gGeoManager->GetMedium("Air");
   }else{
       cout << "-I- R3BTwinMusic: Air Medium defined " << endl;
       nel     = 2;
       density = 0.001290;
       TGeoMixture*
	   pMat2 = new TGeoMixture("Air", nel,density);
       a = 14.006740;   z = 7.000000;   w = 0.700000;  // N
       pMat2->DefineElement(0,a,z,w);
       a = 15.999400;   z = 8.000000;   w = 0.300000;  // O
       pMat2->DefineElement(1,a,z,w);
       pMat2->SetIndex(1);
       numed = 14;
       Double_t par[8];
       par[0]  = 0.000000; // isvol
       par[1]  = 0.000000; // ifield
       par[2]  = 0.000000; // fieldm
       par[3]  = 0.000000; // tmaxfd
       par[4]  = 0.000000; // stemax
       par[5]  = 0.000000; // deemax
       par[6]  = 0.000100; // epsil
       par[7]  = 0.000000; // stmin
       pMedAir = new TGeoMedium("Air", numed,pMat2,par);
   }


    //-- Mixture: Neon more CH4

   TGeoMedium * pMedNeCH4=NULL;
   if (gGeoManager->GetMedium("NeCH4") ){
       cout << "-I- TGeoManager: Ne CH4 Medium already defined " << endl;
       pMedNeCH4=gGeoManager->GetMedium("NeCH4");
   }else{
       cout << "-I- R3BTwinMusic: Ne CH4 Medium defined " << endl;
       nel     = 3;
       density = 0.001290;
       TGeoMixture*
	   pMat2 = new TGeoMixture("NeCH4", nel,density);
       a = 20.1797;   z = 10.000000;   w = 0.850000;  // Ne
       pMat2->DefineElement(0,a,z,w);
       a = 12.01070;   z = 6.000000;   w = 0.112500;  // C
       pMat2->DefineElement(1,a,z,w);
       a = 1.007940;   z = 1.000000;   w = 0.037500;  // H
       pMat2->DefineElement(2,a,z,w);
       pMat2->SetIndex(147);
       numed = 147;
       Double_t par[8];
       par[0]  = 0.000000; // isvol
       par[1]  = 0.000000; // ifield
       par[2]  = 0.000000; // fieldm
       par[3]  = 0.000000; // tmaxfd
       par[4]  = 0.000000; // stemax
       par[5]  = 0.000000; // deemax
       par[6]  = 0.000100; // epsil
       par[7]  = 0.000000; // stmin
       pMedNeCH4 = new TGeoMedium("NeCH4", numed,pMat2,par);
   }




   //-- Material: Helio

    TGeoMedium * pMedHe=NULL;
   if (gGeoManager->GetMedium("He") ){
       cout << "-I- TGeoManager: Helium Medium already defined " << endl;
       pMedHe=gGeoManager->GetMedium("He");
   }else{
       a       = 4.0026;
       z       = 2.000000;
       density = 0.000166;
    TGeoMaterial*
	pMatHe = new TGeoMaterial("He", a,z,density);
    pMatHe->SetIndex(766);
    numed   =766;  // medium number
    Double_t par[8];
       par[0]  = 0.000000; // isvol
       par[1]  = 0.000000; // ifield
       par[2]  = 0.000000; // fieldm
       par[3]  = 0.000000; // tmaxfd
       par[4]  = 0.000000; // stemax
       par[5]  = 0.000000; // deemax
       par[6]  = 0.000100; // epsil
       par[7]  = 0.000000; // stmin
       pMedHe = new TGeoMedium("He", numed,pMatHe,par);
   }

    //-- Material: Neon

    TGeoMedium * pMedNeon=NULL;
   if (gGeoManager->GetMedium("neon") ){
       cout << "-I- TGeoManager: Neon Medium already defined " << endl;
       pMedHe=gGeoManager->GetMedium("neon");
   }else{
       a       = 20.1797;
       z       = 10.0000;
       density = 0.000839;
    TGeoMaterial*
	pMatNeon = new TGeoMaterial("Neon", a,z,density);
    pMatNeon->SetIndex(767);
    numed   =767;  // medium number
    Double_t par[8];
       par[0]  = 0.000000; // isvol
       par[1]  = 0.000000; // ifield
       par[2]  = 0.000000; // fieldm
       par[3]  = 0.000000; // tmaxfd
       par[4]  = 0.000000; // stemax
       par[5]  = 0.000000; // deemax
       par[6]  = 0.000100; // epsil
       par[7]  = 0.000000; // stmin
       pMedNeon = new TGeoMedium("Neon", numed,pMatNeon,par);
   }

   // Mixture: CH2

   TGeoMedium * pMedCH2=NULL;
   if (gGeoManager->GetMedium("CH2") ){
       cout << "-I- TGeoManager: CH2 Medium already defined " << endl;
       pMedCH2=gGeoManager->GetMedium("CH2");
   }else{
     nel = 2;
     density = 0.930000;
     TGeoMixture*
	 pMat16 = new TGeoMixture("CH2", nel,density);
     a = 12.010700;   z = 6.000000;   w = 0.856281;  // C
     pMat16->DefineElement(0,a,z,w);
     a = 1.007940;   z = 1.000000;   w = 0.143719;  // H
     pMat16->DefineElement(1,a,z,w);
     pMat16->SetIndex(15);
     // Medium: CH2
     numed   = 15;  // medium number
     Double_t par[8];
      par[0]  = 0.000000; // isvol
      par[1]  = 0.000000; // ifield
      par[2]  = 0.000000; // fieldm
      par[3]  = 0.000000; // tmaxfd
      par[4]  = 0.000000; // stemax
      par[5]  = 0.000000; // deemax
      par[6]  = 0.000100; // epsil
      par[7]  = 0.000000; // stmin
     pMedCH2 = new TGeoMedium("CH2", numed,pMat16, par);
   }

   // Material: Vacuum

   TGeoMedium * pMedVac=NULL;
   if (gGeoManager->GetMedium("vacuum") ){
       cout << "-I- TGeoManager: Vacuum Medium already defined " << endl;
       pMedVac=gGeoManager->GetMedium("vacuum");
   }else{
     a       = 1.e-16;
     z       = 1.e-16;
     density = 1.e-16;
     TGeoMaterial*
	pMat17 = new TGeoMaterial("vacuum", a,z,density);
     pMat17->SetIndex(150);
     numed   = 150;  // medium number
     Double_t par[8];
      par[0]  = 0.000000; // isvol
      par[1]  = 0.000000; // ifield
      par[2]  = 0.000000; // fieldm
      par[3]  = 0.000000; // tmaxfd
      par[4]  = 0.000000; // stemax
      par[5]  = 0.000000; // deemax
      par[6]  = 0.000100; // epsil
      par[7]  = 0.000000; // stmin
     pMedVac = new TGeoMedium("vacuum", numed,pMat17, par);
   }

        //WORLD
	
	TGeoVolume *pAWorld  =  gGeoManager->GetTopVolume();
    
        // Defintion of the Mother Volume
	
	Double_t length = 60.;
	
	TGeoShape *pCBWorld = new TGeoBBox("TwinMusic_box",
									   length/2.0,
									   length/2.0,
									   length/2.0);
	
	TGeoVolume*
	pWorld  = new TGeoVolume("TwinMusic_World",pCBWorld, pMedNeCH4);
	
	TGeoCombiTrans *t0 = new TGeoCombiTrans(0,0,168.0,0);
	TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
	
	// add the sphere as Mother Volume
	pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

  
  double par=2.5;

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 5.10;
   dy = 0.000;
   dz = par-2.5;
   TGeoCombiTrans* pMatrix111 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 10.000;
   dy = 20.000;
   dz = 50.000;
   TGeoShape *twin111 = new TGeoBBox("twin111", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   twin111_log = new TGeoVolume("Detector1",twin111, pMedNeCH4);
   twin111_log->SetVisLeaves(kTRUE);
   twin111_log->SetLineColor(2);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal111 = GetGlobalPosition(pMatrix111);
   pWorld->AddNode(twin111_log, 1, pGlobal111);

   AddSensitiveVolume(twin111_log);
   fNbOfSensitiveVol+=1;


   // Combi transformation: 
   dx = -5.10;
   dy = 0.000;
   dz = par-2.5;
   TGeoCombiTrans* pMatrix122 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Volume: 
   TGeoVolume*
   twin122_log = new TGeoVolume("Detector2",twin111, pMedNeCH4);
   twin122_log->SetVisLeaves(kTRUE);
   twin122_log->SetLineColor(6);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal122= GetGlobalPosition(pMatrix122);
   pWorld->AddNode(twin122_log, 1, pGlobal122);

   AddSensitiveVolume(twin122_log);
   fNbOfSensitiveVol+=1;


   
   dx = 0.000;
   dy = -17.0;
   dz = 0.000;
   TGeoCombiTrans* pMatrix21 = new TGeoCombiTrans("", dx,dy,dz,0);
   dx = 32.60;
   dy = 1.000;
   dz = 54.00;
   TGeoShape *twin2 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   twin21_log = new TGeoVolume("twin21_log",twin2, pMedAl);
   twin21_log->SetVisLeaves(kTRUE);
   twin21_log->SetLineColor(17);

   TGeoCombiTrans* pGlobal21 = GetGlobalPosition(pMatrix21);
   pWorld->AddNode(twin21_log, 0, pGlobal21);

   dx = 0.000;
   dy = 17.00;
   dz = 0.000;
   TGeoCombiTrans* pMatrix22 = new TGeoCombiTrans("", dx,dy,dz,0);

   TGeoCombiTrans* pGlobal22 = GetGlobalPosition(pMatrix22);
   pWorld->AddNode(twin21_log, 1, pGlobal22);

   
   dx = 16.80;
   dy = 0.000;
   dz = 0.000;
   TGeoCombiTrans* pMatrix23 = new TGeoCombiTrans("", dx,dy,dz,0);
   dx = 1.000;
   dy = 35.00;
   dz = 54.00;
   TGeoShape *twin3 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   twin23_log = new TGeoVolume("twin23_log",twin3, pMedAl);
   twin23_log->SetVisLeaves(kTRUE);
   twin23_log->SetLineColor(17);

   TGeoCombiTrans* pGlobal23 = GetGlobalPosition(pMatrix23);
   pWorld->AddNode(twin23_log, 0, pGlobal23);

   dx = -16.8;
   dy = 0.000;
   dz = 0.000;
   TGeoCombiTrans* pMatrix24 = new TGeoCombiTrans("", dx,dy,dz,0);

   // Volume: 
   TGeoVolume*
   twin24_log = new TGeoVolume("twin24_log",twin3, pMedAl);
   twin24_log->SetVisLeaves(kTRUE);
   twin24_log->SetLineColor(17);

   TGeoCombiTrans* pGlobal24 = GetGlobalPosition(pMatrix24);
   pWorld->AddNode(twin24_log, 0, pGlobal24);


   dx = 0.000;
   dy = 0.000;
   dz = 0.000;
   TGeoCombiTrans* pMatrix0 = new TGeoCombiTrans("", dx,dy,dz,0);
   TGeoCombiTrans* pGlobal0 = GetGlobalPosition(pMatrix0);

   dx = 0.100;
   dy = 20.00;
   dz = 54.00;
   TGeoShape *twin4 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   twin25_log = new TGeoVolume("twin25_log",twin4, pMedCu);
   twin25_log->SetVisLeaves(kTRUE);
   twin25_log->SetLineColor(48);

   pWorld->AddNode(twin25_log, 0, pGlobal0);


   dx = 0.000;
   dy = 0.000;
   dz = -27.5;
   TGeoCombiTrans* pMatrix26 = new TGeoCombiTrans("", dx,dy,dz,0);
   dx = 0.000;
   dy = 0.000;
   dz = 55.00;
   TGeoCombiTrans* pMatrix27 = new TGeoCombiTrans("", dx,dy,dz,0);
   dx = 34.60;
   dy = 35.00;
   dz = 1.000;
   TGeoShape *twin5 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);

   dx = 20.00;
   dy = 20.00;
   dz = 3.500;
   TGeoShape *twin6 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);

   TGeoSubtraction* twins = new TGeoSubtraction(twin5,twin6,pMatrix26,pMatrix26);
   TGeoShape *twin7 = new TGeoCompositeShape("GE", twins);
   TGeoVolume*
   twin27_log = new TGeoVolume("twin27_log",twin7, pMedAl);
   twin27_log->SetVisLeaves(kTRUE);
   twin27_log->SetLineColor(17);
   pWorld->AddNode(twin27_log, 0, pGlobal0);
   pWorld->AddNode(twin27_log, 1, pMatrix27);

   dx = 0.000;
   dy = 0.000;
   dz = 27.95;
   TGeoCombiTrans* pMatrix28 = new TGeoCombiTrans("", dx,dy,dz,0);
   dx = 0.000;
   dy = 0.000;
   dz = -27.95;
   TGeoCombiTrans* pMatrix29 = new TGeoCombiTrans("", dx,dy,dz,0);
   dx = 20.00;
   dy = 20.00;
   dz = 0.005;
   TGeoShape *twin8 = new TGeoBBox("", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   twin28_log = new TGeoVolume("twin28_log",twin8, pMedMylar);
   twin28_log->SetVisLeaves(kTRUE);
   twin28_log->SetLineColor(43);

   TGeoCombiTrans* pGlobal28 = GetGlobalPosition(pMatrix28);
   pWorld->AddNode(twin28_log, 1, pGlobal28);

   TGeoCombiTrans* pGlobal29 = GetGlobalPosition(pMatrix29);
   pWorld->AddNode(twin28_log, 2, pGlobal29);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = 0.000;
   
   TGeoCombiTrans* pMatrix30 = new TGeoCombiTrans("", dx,dy,dz,0);

   // Shape: sofiae5 type: TGeoBBox
   dx = 22.000;
   dy = 22.000;
   dz = 2.0000;
   TGeoShape *twin29 = new TGeoBBox("twin29", dx/2.,dy/2.,dz/2.);
   // Volume:
   TGeoVolume* twin29_log = new TGeoVolume("twin29_log",twin29, pMedAl);
   twin29_log->SetVisLeaves(kTRUE);
   twin29_log->SetLineColor(17);

   // Position Volume
   TGeoCombiTrans* pGlobal30 = GetGlobalPosition(pMatrix30);
  // pWorld->AddNode(twin29_log, 0, pGlobal30);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = 0.000;
   
   TGeoCombiTrans* pMatrix31 = new TGeoCombiTrans("", dx,dy,dz,0);

   // Shape: sofiae5 type: TGeoBBox
   dx = 20.000;
   dy = 20.000;
   dz = 3.0000;
   TGeoShape *twin30 = new TGeoBBox("twin30", dx/2.,dy/2.,dz/2.);
   // Volume:
   TGeoVolume* twin30_log = new TGeoVolume("twin30_log",twin30, pMedAl);
   twin30_log->SetVisLeaves(kTRUE);
   twin30_log->SetLineColor(17);

   // Position Volume
   TGeoCombiTrans* pGlobal31 = GetGlobalPosition(pMatrix31);
  // pWorld->AddNode(twin30_log, 0, pGlobal31);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.00000;
   dy = 0.000;
   dz = -29.000;
   // Rotation: 
   TGeoRotation *rot_t1 = new TGeoRotation("rot1");
   rot_t1->RotateX(0.0);

   TGeoCombiTrans* pMatrix32 = new TGeoCombiTrans("", dx,dy,dz,rot_t1);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.00000;
   dy = 0.000;
   dz = 29.000;
   // Rotation: 
   TGeoRotation *rot_t2 = new TGeoRotation("rot2");
   rot_t2->RotateX(0.0);

   TGeoCombiTrans* pMatrix33 = new TGeoCombiTrans("", dx,dy,dz,rot_t2);

   // Shape: Sofiae7 type: TGeoTubeSeg

   TGeoSubtraction* twin31 = new TGeoSubtraction(twin29,twin30,pMatrix30,pMatrix31);
   TGeoShape *Booltwin31= new TGeoCompositeShape("GE", twin31);
   TGeoVolume*
   twin31_log = new TGeoVolume("twin31_log",Booltwin31, pMedAl);
   twin31_log->SetVisLeaves(kTRUE);
   twin31_log->SetLineColor(17);

   TGeoCombiTrans* pGlobal32 = GetGlobalPosition(pMatrix32);
   TGeoCombiTrans* pGlobal33 = GetGlobalPosition(pMatrix33);

   pWorld->AddNode(twin31_log, 1, pGlobal32);
   pWorld->AddNode(twin31_log, 2, pGlobal33);

   
   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 5.0+32.600/2.0+1.0;
   dy = 0.000;
   dz = 0.000;
   
   TGeoCombiTrans* pMatrix34 = new TGeoCombiTrans("", dx,dy,dz,0);

   TGeoCombiTrans* pMatrix35 = new TGeoCombiTrans("", -dx,dy,dz,0);

   // Shape: sofiae5 type: TGeoBBox
   dx = 10.000;
   dy = 18.000;
   dz = 32.0000;
   TGeoShape *twin32 = new TGeoBBox("twin32", dx/2.,dy/2.,dz/2.);
   // Volume:
   TGeoVolume* twin32_log = new TGeoVolume("twin32_log",twin32, pMedAl);
   twin32_log->SetVisLeaves(kTRUE);
   twin32_log->SetLineColor(28);

   // Position Volume
   TGeoCombiTrans* pGlobal34 = GetGlobalPosition(pMatrix34);
   pWorld->AddNode(twin32_log, 1, pGlobal34);

   TGeoCombiTrans* pGlobal35 = GetGlobalPosition(pMatrix35);
   pWorld->AddNode(twin32_log, 2, pGlobal35);

}
ClassImp(R3BTwinMusic)
