// -------------------------------------------------------------------------
// -----                        R3BMWpc source file                    -----
// -----                  Created 05/02/12  by J.L. Rodriguez          -----
// -----			                		       -----
// -------------------------------------------------------------------------
#include "R3BMWpc.h"

#include "R3BGeoMWpc.h"
#include "R3BMWpcHit.h"
#include "R3BMWpcPoint.h"
#include "R3BGeoMWpcPar.h"

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
R3BMWpc::R3BMWpc() : R3BDetector("R3BMWpc1", kTRUE, kTRA) {
  ResetParameters();
  fMWpcCollection = new TClonesArray("R3BMWpcPoint");
  fMWpcHitCollection = new TClonesArray("R3BMWpcHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fNonUniformity = 0.;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BMWpc::R3BMWpc(const char* name, Bool_t active) 
  : R3BDetector(name, active, kTRA) {
  ResetParameters();
  fMWpcCollection = new TClonesArray("R3BMWpcPoint");
  fMWpcHitCollection = new TClonesArray("R3BMWpcHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fNonUniformity = 0.;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BMWpc::~R3BMWpc() {

  if ( flGeoPar ) delete flGeoPar;
  if (fMWpcCollection) {
    fMWpcCollection->Delete();
    delete fMWpcCollection;
  }
  if (fMWpcHitCollection) {
	fMWpcHitCollection->Delete();
	delete fMWpcHitCollection;
  }
}
// -------------------------------------------------------------------------
void R3BMWpc::Initialize()
{
  FairDetector::Initialize();

   cout << endl;
   cout << "-I- R3BMWpc1: initialisation" << endl;
   cout << "-I- R3BMWpc1: Vol (McId) def." << endl;

   Char_t buffer[126];

  
	for (Int_t i=0;i<1;i++ ) {
     sprintf(buffer,"Det_%i",i+1);
     cout << "-I- R3BMWpc1: Detector   : " << i << " connected to (McId) ---> " <<  gMC->VolId(buffer)<< endl;
     fDetectorType[i] = gMC->VolId(buffer);
    }


}


void R3BMWpc::SetSpecialPhysicsCuts(){

   cout << endl;

   cout << "-I- R3BMWpc1: Adding customized Physics cut ... " << endl;
   cout << "-I- R3BMWpc1: Yet not implemented !... " << endl;

   cout << endl;

}




// -----   Public method ProcessHits  --------------------------------------
Bool_t R3BMWpc::ProcessHits(FairVolume* vol) {
		
   // Getting the Infos from mwpc1 Volumes
   Int_t cp1 = -1; Int_t volId1 = -1; Int_t cpPla = -1; Int_t volIdPla = -1;
   // Strip Ids
 
   const char* bufferName = gMC->CurrentVolName();
   volId1 =  gMC->CurrentVolID(cp1);


   Int_t mwpc1Type = 0;
   Int_t mwpc1Copy = 0;
   Int_t mwpc1Id = 0;
   
	
	  if(GetPlaneType(volIdPla)!=-1){
		   mwpc1Type = GetDetectorType(volId1);
		   mwpc1Copy = mwpc1Type;
		   mwpc1Id = 1+mwpc1Type;
                   //cout<< "mwpc1= "<< mwpc1Type << endl;
		   if(mwpc1Type>2) 
			   cout << "-E- R3BMWpc1: Wrong number. " << endl;
	   }
	   
          
	
    if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fTrackID  = gMC->TrackPid();
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

  // Set additional parameters at exit of active volume. Create R3BMWpcPoint.
  if ( gMC->IsTrackExiting()    ||
       gMC->IsTrackStop()       ||
       gMC->IsTrackDisappeared()   ) {
    //fTrackID  = gMC->GetStack()->GetCurrentTrackNumber();
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
      
//cout << "1st direction: " << olddirection[0] << "," << olddirection[1] << "," << olddirection[2] << endl;

      for (Int_t i=0; i<3; i++){
	newdirection[i] = -1*olddirection[i];
      }
      
      gGeoManager->SetCurrentDirection(newdirection);
//TGeoNode *bla = gGeoManager->FindNextBoundary(2);
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

    // We use micro(10^-6 m) in X and Y positions.

    AddHit(fTrackID, fVolumeID, mwpc1Type , mwpc1Copy , mwpc1Id,
	   TVector3(fPosIn.X()*10000.0,   fPosIn.Y()*10000.0,   fPosIn.Z()),
	   TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
	   TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
	   TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
	   fTime, fLength, fELoss);
    
    // Increment number of MWpcPoints for this track
    FairStack* stack = (FairStack*) gMC->GetStack();
    stack->AddPoint(kTRA);
    
    ResetParameters();
  }

  return kTRUE;
}
// ----------------------------------------------------------------------------
//void R3BMWpc::SaveGeoParams(){
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
void R3BMWpc::BeginEvent() {

//  if (! kGeoSaved ) {
//      SaveGeoParams();
//  cout << "-I STS geometry parameters saved " << endl;
//  kGeoSaved = kTRUE;
//  }

}
// -----   Public method EndOfEvent   -----------------------------------------
void R3BMWpc::EndOfEvent() {
	
  if (fVerboseLevel) Print();

  fMWpcCollection->Clear();
  fMWpcHitCollection->Clear();

  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----   Public method Register   -------------------------------------------
void R3BMWpc::Register() {
  FairRootManager::Instance()->Register("MWPC_1", GetName(), fMWpcCollection, kTRUE);
  //FairRootManager::Instance()->Register("Rpc", GetName(), fMWpcHitCollection, kTRUE);

}
// ----------------------------------------------------------------------------



// -----   Public method GetCollection   --------------------------------------
TClonesArray* R3BMWpc::GetCollection(Int_t iColl) const {
	
 // if (iColl == 0) return fMWpcHitCollection;
  if (iColl == 0) return fMWpcCollection;
  else return NULL;
}
// ----------------------------------------------------------------------------



// -----   Public method Print   ----------------------------------------------
void R3BMWpc::Print() const {
  Int_t nHits = fMWpcCollection->GetEntriesFast();
  cout << "-I- R3BMWpc1: " << nHits << " points registered in this event." 
       << endl;
  Int_t nmwpc1Hits = fMWpcHitCollection->GetEntriesFast();
  cout << "-I- R3BMWpc1: " << nmwpc1Hits << " hits registered in this event." 
	<< endl;
}
// ----------------------------------------------------------------------------



// -----   Public method Reset   ----------------------------------------------
void R3BMWpc::Reset() {
  fMWpcCollection->Clear();
  fMWpcHitCollection->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------



// -----   Public method CopyClones   -----------------------------------------
void R3BMWpc::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  Int_t nEntries = cl1->GetEntriesFast();
  cout << "-I- R3BMWpc1: " << nEntries << " entries to add." << endl;
  TClonesArray& clref = *cl2;
  R3BMWpcPoint* oldpoint = NULL;
   for (Int_t i=0; i<nEntries; i++) {
   oldpoint = (R3BMWpcPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) R3BMWpcPoint(*oldpoint);
    fPosIndex++;
  }
   cout << " -I- R3BMWpc1: " << cl2->GetEntriesFast() << " merged entries."
       << endl;
}

// -----   Private method AddHit   --------------------------------------------
R3BMWpcPoint* R3BMWpc::AddHit(Int_t trackID, Int_t detID, Int_t volid , Int_t copy, Int_t ident, 
							  TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut, 
							  Double_t time, Double_t length, Double_t eLoss) {
  TClonesArray& clref = *fMWpcCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1) 
    cout << "-I- R3BMWpc1: Adding Point at (" << posIn.X() << ", " << posIn.Y() 
	 << ", " << posIn.Z() << ") cm,  detector " << detID << ", track "
	 << trackID << ", energy loss " << eLoss*1e06 << " keV" << endl;
  return new(clref[size]) R3BMWpcPoint(trackID, detID, volid, copy , ident, posIn, posOut,
				      momIn, momOut, time, length, eLoss);
}

// -----   Private method Addmwpc1Hit   --------------------------------------------
R3BMWpcHit* R3BMWpc::Addmwpc1Hit(Int_t type, Int_t copy, Int_t ident, Double_t energy, Double_t time) {

	TClonesArray& clref = *fMWpcHitCollection;
	Int_t size = clref.GetEntriesFast();
	if (fVerboseLevel>1) 
		cout << "-I- R3BMWpc1: Adding Hit in detector type " << type << ", and copy " << copy 
		<< " with unique identifier " << ident << ", depositing " << energy*1e06 << " keV" << endl;
	return new(clref[size]) R3BMWpcHit(type, copy, ident, energy, time);
}

// -----   Private method NUSmearing  --------------------------------------------
Double_t R3BMWpc::NUSmearing(Double_t inputEnergy) {
	// Very simple preliminary scheme where the NU is introduced as a flat random
	// distribution with limits fNonUniformity (%) of the energy value.
	//
 
        return gRandom->Uniform(inputEnergy-inputEnergy*fNonUniformity/100,inputEnergy+inputEnergy*fNonUniformity/100);
}



// -----  Public method SetNonUniformity  ----------------------------------
void R3BMWpc::SetNonUniformity(Double_t nonU)
{	
	fNonUniformity = nonU;
	cout << "-I- R3BMWpc1::SetNonUniformity to " << fNonUniformity << " %." << endl;
}


// -----   Public method ConstructGeometry   ----------------------------------
void R3BMWpc::ConstructGeometry() {

	

    cout << endl;
    cout << "-I- R3BMWpc1:: ConstructGeometry() "<< endl;
    cout << endl;

    Double_t dx,dy,dz,rmax,rmin;
    Double_t a;
    Double_t thx, phx, thy, phy, thz, phz;
    Double_t  phi1, phi2,dphi, phi;
    Double_t z, density, w;
    Int_t nel, numed;
    Double_t radl    = 0.;
    Double_t absl    = 0.;
    Int_t nedges, nz;

   // MATERIALS, MIXTURES AND TRACKING MEDIA

   // --  Material: Lead

   TGeoMedium   * pMedLead=NULL;
   TGeoMaterial * pMatLead=NULL;

   if (gGeoManager->GetMedium("Lead") ){
       cout << "-I- TGeoManager: Lead Medium already defined " << endl;
       pMedLead = gGeoManager->GetMedium("Lead");
   }else{
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
       pMedCu=gGeoManager->GetMedium("Copper");
   }else{
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
    par[6]  = 0.000000; // epsil
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
    par[6]  = 0.000000; // epsil
    par[7]  = 0.000000; // stmin
    pMedAl  = new TGeoMedium("aluminium", numed,pMatAl, par);
   }

   // --  Material: Iron 
 
   TGeoMedium * pMedFe=NULL;
   if (gGeoManager->GetMedium("Iron") ){
       cout << "-I- TGeoManager: Iron Medium already defined " << endl;
       pMedFe=gGeoManager->GetMedium("Iron");
   }else{
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

   //-- Material: Argon

    TGeoMedium * pMedAr=NULL;
   if (gGeoManager->GetMedium("Ar") ){
       cout << "-I- TGeoManager: Argon Medium already defined " << endl;
       pMedAr=gGeoManager->GetMedium("Ar");
   }else{
       a       = 39.948;
       z       = 18.000;
       density = 0.001396;
    TGeoMaterial*
	pMatAr = new TGeoMaterial("Ar", a,z,density);
    pMatAr->SetIndex(761);
    numed   =761;  // medium number
    Double_t par[8];
       par[0]  = 0.000000; // isvol
       par[1]  = 0.000000; // ifield
       par[2]  = 0.000000; // fieldm
       par[3]  = 0.000000; // tmaxfd
       par[4]  = 0.000000; // stemax
       par[5]  = 0.000000; // deemax
       par[6]  = 0.000100; // epsil
       par[7]  = 0.000000; // stmin
       pMedAr = new TGeoMedium("Ar", numed,pMatAr,par);
   }

    //-- Material: Neon

    TGeoMedium * pMedNeon=NULL;
   if (gGeoManager->GetMedium("neon") ){
       cout << "-I- TGeoManager: Neon Medium already defined " << endl;
       pMedNeon=gGeoManager->GetMedium("neon");
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

        TGeoRotation *rot_mwpc = new TGeoRotation("MWPCrot");
        rot_mwpc->RotateY(0.0);
    
        // Defintion of the Mother Volume
	
	//Double_t length = 54.;
	
	TGeoShape *pCBWorld = new TGeoBBox("MWPC_box",
									   55.0/2.0,
									   58.0/2.0,
									   6.0/2.0);
	
	TGeoVolume*
	pWorld  = new TGeoVolume("MWPC1_World",pCBWorld, pMedAr);
	
	TGeoCombiTrans *t0 = new TGeoCombiTrans(0.0,0.0,207.0,rot_mwpc);
	TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
	
	// add the sphere as Mother Volume
	pAWorld->AddNodeOverlap(pWorld, 0, pGlobalc);

 
   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = 0.000;
   TGeoCombiTrans* pMatrix1 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 53.800;
   dy = 57.000;
   dz = 1.2000;
   TGeoShape *base1 = new TGeoBBox("base1", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log1 = new TGeoVolume("base_log1",base1, pMedAl);
   base_log1->SetVisLeaves(kTRUE);
   base_log1->SetLineColor(3);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal1 = GetGlobalPosition(pMatrix1);
  // pWorld->AddNode(base_log1, 0, pGlobal1);

   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.500;
   dy = 21.500;
   dz = 2.2000;
   TGeoShape *base2 = new TGeoBBox("base2", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log2 = new TGeoVolume("base_log2",base2, pMedAl);
   base_log2->SetVisLeaves(kTRUE);
   base_log2->SetLineColor(2);

   // Position Mother Volume
  // pWorld->AddNode(base_log2, 0, pGlobal1);


   
   TGeoSubtraction* basesub1 = new TGeoSubtraction(base1,base2,pMatrix1,pMatrix1);
   TGeoShape *basesub_1 = new TGeoCompositeShape("GE", basesub1);
   TGeoVolume*
   basesub_log1 = new TGeoVolume("",basesub_1, pMedAl);
   basesub_log1->SetVisLeaves(kTRUE);
   basesub_log1->SetLineColor(4);

  // pWorld->AddNode(basesub_log1, 0, pGlobal1);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 25.60+10.0/2.0-6.80;
   dy = 0.0;
   dz = 0.000;
   TGeoCombiTrans* pMatrix3 = new TGeoCombiTrans("", dx,dy,dz,0);

   // Shape: sofia1 type: TGeoTubeSeg
   dx = 10.000;
   dy = 25.000;
   dz = 2.2000;
   TGeoShape *base3 = new TGeoBBox("base3", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log3 = new TGeoVolume("base_log3",base3, pMedAl);
   base_log3->SetVisLeaves(kTRUE);
   base_log3->SetLineColor(2);


   TGeoSubtraction* basesub2 = new TGeoSubtraction(basesub_1,base3,pMatrix1,pMatrix3);
   TGeoShape *basesub_2 = new TGeoCompositeShape("GE", basesub2);
   TGeoVolume*
   basesub_log2 = new TGeoVolume("",basesub_2, pMedAl);
   basesub_log2->SetVisLeaves(kTRUE);
   basesub_log2->SetLineColor(4);

  // pWorld->AddNode(basesub_log2, 0, pGlobal1);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = -25.60-10.0/2.0+8.00;
   dy = 0.0;
   dz = 0.000;
   TGeoCombiTrans* pMatrix3b = new TGeoCombiTrans("", dx,dy,dz,0);

   
   TGeoSubtraction* basesub3 = new TGeoSubtraction(basesub_2,base3,pMatrix1,pMatrix3b);
   TGeoShape *basesub_3 = new TGeoCompositeShape("GE", basesub3);
   TGeoVolume*
   basesub_log3 = new TGeoVolume("",basesub_3, pMedAl);
   basesub_log3->SetVisLeaves(kTRUE);
   basesub_log3->SetLineColor(4);

   //pWorld->AddNode(basesub_log3, 0, pGlobal1);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.00;
   dy = 27.0+18.0/2.0-8.0;
   dz = 0.000;
   TGeoCombiTrans* pMatrix4 = new TGeoCombiTrans("", dx,dy,dz,0);

    // Shape: sofia1 type: TGeoTubeSeg
   dx = 17.000;
   dy = 18.000;
   dz = 2.2000;
   TGeoShape *base4 = new TGeoBBox("base4", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log4 = new TGeoVolume("base_log4",base4, pMedAl);
   base_log4->SetVisLeaves(kTRUE);
   base_log4->SetLineColor(2);

   
   TGeoSubtraction* basesub4 = new TGeoSubtraction(basesub_3,base4,pMatrix1,pMatrix4);
   TGeoShape *basesub_4 = new TGeoCompositeShape("GE", basesub4);
   TGeoVolume*
   basesub_log4 = new TGeoVolume("",basesub_4, pMedAl);
   basesub_log4->SetVisLeaves(kTRUE);
   basesub_log4->SetLineColor(4);

  // pWorld->AddNode(basesub_log4, 0, pGlobal1);


   // Combi transformation: 
   dx = 0.00;
   dy = -30.0-18.0/2.0+11.0;
   dz = 0.000;
   TGeoCombiTrans* pMatrix4b = new TGeoCombiTrans("", dx,dy,dz,0);

   TGeoSubtraction* basesub5 = new TGeoSubtraction(basesub_4,base4,pMatrix1,pMatrix4b);
   TGeoShape *basesub_5 = new TGeoCompositeShape("GE", basesub5);
   TGeoVolume*
   basesub_log5 = new TGeoVolume("plane1",basesub_5, pMedAl);
   basesub_log5->SetVisLeaves(kTRUE);
   basesub_log5->SetLineColor(18);

   pWorld->AddNode(basesub_log5, 0, pGlobal1);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = -1.200;
   TGeoCombiTrans* pMatrix6 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 39.000;
   dy = 38.000;
   dz = 1.2000;
   TGeoShape *base6 = new TGeoBBox("base6", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log6 = new TGeoVolume("base_log6",base6, pMedAl);
   base_log6->SetVisLeaves(kTRUE);
   base_log6->SetLineColor(2);

   // Position Mother Volume
   //TGeoCombiTrans* pGlobal2 = GetGlobalPosition(pMatrix6);
   //pWorld->AddNode(base_log6, 0, pGlobal2);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.000;
   dy = 21.000;
   dz = 2.2000;
   TGeoShape *base7 = new TGeoBBox("base7", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log7 = new TGeoVolume("base_log7",base7, pMedAl);
   base_log7->SetVisLeaves(kTRUE);
   base_log7->SetLineColor(7);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal2 = GetGlobalPosition(pMatrix6);
  // pWorld->AddNode(base_log7, 0, pGlobal2);
   

   TGeoSubtraction* basesub6 = new TGeoSubtraction(base6,base7,pMatrix1,pMatrix1);
   TGeoShape *basesub_6 = new TGeoCompositeShape("GE", basesub6);
   TGeoVolume*
   basesub_log6 = new TGeoVolume("",basesub_6, pMedAl);
   basesub_log6->SetVisLeaves(kTRUE);
   basesub_log6->SetLineColor(2);

   //pWorld->AddNode(basesub_log6, 0, pGlobal2);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 39.0/2.0+2.5;
   dy = 17.0/2.0+1.75;
   dz = 0.000;
   TGeoCombiTrans* pMatrix7 = new TGeoCombiTrans("", dx,dy,dz,0);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 5.000;
   dy = 3.500;
   dz = 1.2000;
   TGeoShape *base8 = new TGeoBBox("base8", dx/2.,dy/2.,dz/2.);

   TGeoUnion* basesub7 = new TGeoUnion(basesub_6, base8, pMatrix1, pMatrix7);
   TGeoShape *basesub_7 = new TGeoCompositeShape("GE", basesub7);
   TGeoVolume*
   basesub_log7 = new TGeoVolume("",basesub_7, pMedAl);
   basesub_log7->SetVisLeaves(kTRUE);
   basesub_log7->SetLineColor(2);

  // pWorld->AddNode(basesub_log7, 0, pGlobal2);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 39.0/2.0+2.5;
   dy = -17.0/2.0-1.75;
   dz = 0.000;
   TGeoCombiTrans* pMatrix7b = new TGeoCombiTrans("", dx,dy,dz,0);


   TGeoUnion* basesub8 = new TGeoUnion(basesub_7, base8, pMatrix1, pMatrix7b);
   TGeoShape *basesub_8 = new TGeoCompositeShape("GE", basesub8);
   TGeoVolume*
   basesub_log8 = new TGeoVolume("plane2",basesub_8, pMedAl);
   basesub_log8->SetVisLeaves(kTRUE);
   basesub_log8->SetLineColor(29);

   pWorld->AddNode(basesub_log8, 0, pGlobal2);
   


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.000;
   dy = 21.000;
   dz = 1.2000;

   TGeoShape *Detector1 = new TGeoBBox("Detector_1", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   Detector_log1 = new TGeoVolume("Det_1",Detector1, pMedAr);
   Detector_log1->SetVisLeaves(kTRUE);
   Detector_log1->SetLineColor(3);

   pWorld->AddNode(Detector_log1, 0, pGlobal2);

   AddSensitiveVolume(Detector_log1);
   fNbOfSensitiveVol+=1;


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.8/2.0;
   TGeoCombiTrans* pMatrix9 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 39.000;
   dy = 38.000;
   dz = 0.8000;
   TGeoShape *base9 = new TGeoBBox("base9", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log9 = new TGeoVolume("base_log9",base9, pMedAl);
   base_log9->SetVisLeaves(kTRUE);
   base_log9->SetLineColor(2);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal3 = GetGlobalPosition(pMatrix9);
   //pWorld->AddNode(base_log9, 0, pGlobal3);
   

   // Shape: sofia1 type: TGeoTubeSeg
   dx = 30.350;
   dy = 26.500;
   dz = 1.8000;
   TGeoShape *base10 = new TGeoBBox("base10", dx/2.,dy/2.,dz/2.);

    TGeoSubtraction* basesub9 = new TGeoSubtraction(base9,base10,pMatrix1,pMatrix1);
   TGeoShape *basesub_9 = new TGeoCompositeShape("GE", basesub9);
   TGeoVolume*
   basesub_log9 = new TGeoVolume("plane3",basesub_9, pMedAl);
   basesub_log9->SetVisLeaves(kTRUE);
   basesub_log9->SetLineColor(29);

   pWorld->AddNode(basesub_log9, 0, pGlobal3);


  
   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.8-0.5/2.0;
   TGeoCombiTrans* pMatrix11 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 39.000;
   dy = 38.000;
   dz = 0.5000;
   TGeoShape *base11 = new TGeoBBox("base11", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log11 = new TGeoVolume("base_log11",base11, pMedAl);
   base_log11->SetVisLeaves(kTRUE);
   base_log11->SetLineColor(28);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal4 = GetGlobalPosition(pMatrix11);
  // pWorld->AddNode(base_log11, 1, pGlobal4);

   // Shape: sofia1 type: TGeoTubeSeg
   dx = 25.250;
   dy = 26.500;
   dz = 2.2000;
   TGeoShape *base12 = new TGeoBBox("base12", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log12 = new TGeoVolume("base_log12",base12, pMedAl);
   base_log12->SetVisLeaves(kTRUE);
   base_log12->SetLineColor(28);

   // Position Mother Volume
  // TGeoCombiTrans* pGlobal2 = GetGlobalPosition(pMatrix6);
  // pWorld->AddNode(base_log7, 0, pGlobal2);
   

   TGeoSubtraction* basesub13 = new TGeoSubtraction(base11,base12,pMatrix1,pMatrix1);
   TGeoShape *basesub_13 = new TGeoCompositeShape("GE", basesub13);
   TGeoVolume*
   basesub_log13 = new TGeoVolume("",basesub_13, pMedAl);
   basesub_log13->SetVisLeaves(kTRUE);
   basesub_log13->SetLineColor(28);

   //pWorld->AddNode(basesub_log13, 0, pGlobal4);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = -39.0/2.0-2.5;
   dy = -24.5/2.0+1.50;
   dz = 0.000;
   TGeoCombiTrans* pMatrix12 = new TGeoCombiTrans("", dx,dy,dz,0);
   

   // Shape: sofia1 type: TGeoTubeSeg
   dx = 5.000;
   dy = 3.000;
   dz = 0.500;
   TGeoShape *base13 = new TGeoBBox("base13", dx/2.,dy/2.,dz/2.);

   TGeoUnion* basesub14 = new TGeoUnion(basesub_13, base13, pMatrix1, pMatrix12);
   TGeoShape *basesub_14 = new TGeoCompositeShape("GE", basesub14);
   TGeoVolume*
   basesub_log14 = new TGeoVolume("",basesub_14, pMedAl);
   basesub_log14->SetVisLeaves(kTRUE);
   basesub_log14->SetLineColor(28);

   //pWorld->AddNode(basesub_log14, 0, pGlobal4);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = -39.0/2.0-2.5;
   dy = +24.5/2.0-1.50;
   dz = 0.000;
   TGeoCombiTrans* pMatrix12b = new TGeoCombiTrans("", dx,dy,dz,0);


   TGeoUnion* basesub15 = new TGeoUnion(basesub_14, base13, pMatrix1, pMatrix12b);
   TGeoShape *basesub_15 = new TGeoCompositeShape("GE", basesub15);
   TGeoVolume*
   basesub_log15 = new TGeoVolume("plane4",basesub_15, pMedAl);
   basesub_log15->SetVisLeaves(kTRUE);
   basesub_log15->SetLineColor(29);

   pWorld->AddNode(basesub_log15, 0, pGlobal4);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.8-0.5-0.5;
   TGeoCombiTrans* pMatrix14 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 39.000;
   dy = 38.000;
   dz = 1.0000;
   TGeoShape *base14 = new TGeoBBox("base14", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log14 = new TGeoVolume("base_log14",base14, pMedAl);
   base_log14->SetVisLeaves(kTRUE);
   base_log14->SetLineColor(10);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal5 = GetGlobalPosition(pMatrix14);
   //pWorld->AddNode(base_log14, 0, pGlobal5);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.000;
   dy = 21.000;
   dz = 2.2000;
   TGeoShape *base15 = new TGeoBBox("base15", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log15 = new TGeoVolume("base_log15",base15, pMedAl);
   base_log15->SetVisLeaves(kTRUE);
   base_log15->SetLineColor(10);

   // Position Mother Volume
  // TGeoCombiTrans* pGlobal5 = GetGlobalPosition(pMatrix14);
  // pWorld->AddNode(base_log15, 0, pGlobal5);
   

   TGeoSubtraction* basesub16 = new TGeoSubtraction(base14,base15,pMatrix1,pMatrix1);
   TGeoShape *basesub_16 = new TGeoCompositeShape("GE", basesub16);
   TGeoVolume*
   basesub_log16 = new TGeoVolume("",basesub_16, pMedAl);
   basesub_log16->SetVisLeaves(kTRUE);
   basesub_log16->SetLineColor(10);

   //pWorld->AddNode(basesub_log16, 0, pGlobal5);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 39.0/2.0-8.2-1.75;
   dy = 38.0/2.0+2.5;
   dz = 0.000;
   TGeoCombiTrans* pMatrix15 = new TGeoCombiTrans("", dx,dy,dz,0);

    // Shape: sofia1 type: TGeoTubeSeg
   dx = 3.500;
   dy = 5.000;
   dz = 1.000;
   TGeoShape *base16 = new TGeoBBox("base16", dx/2.,dy/2.,dz/2.);

   TGeoUnion* basesub17 = new TGeoUnion(basesub_16, base16, pMatrix1, pMatrix15);
   TGeoShape *basesub_17 = new TGeoCompositeShape("GE", basesub17);
   TGeoVolume*
   basesub_log17 = new TGeoVolume("",basesub_17, pMedAl);
   basesub_log17->SetVisLeaves(kTRUE);
   basesub_log17->SetLineColor(10);

  // pWorld->AddNode(basesub_log17, 0, pGlobal5);

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 39.0/2.0-8.2-1.75;
   dy = -38.0/2.0-2.5;
   dz = 0.000;
   TGeoCombiTrans* pMatrix15b = new TGeoCombiTrans("", dx,dy,dz,0);


   TGeoUnion* basesub18 = new TGeoUnion(basesub_17, base16, pMatrix1, pMatrix15b);
   TGeoShape *basesub_18 = new TGeoCompositeShape("GE", basesub18);
   TGeoVolume*
   basesub_log18 = new TGeoVolume("",basesub_18, pMedAl);
   basesub_log18->SetVisLeaves(kTRUE);
   basesub_log18->SetLineColor(10);

   //pWorld->AddNode(basesub_log18, 0, pGlobal5);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = -39.0/2.0+6.8+1.75;
   dy = 38.0/2.0+2.5;
   dz = 0.000;
   TGeoCombiTrans* pMatrix15c = new TGeoCombiTrans("", dx,dy,dz,0);


   TGeoUnion* basesub19 = new TGeoUnion(basesub_18, base16, pMatrix1, pMatrix15c);
   TGeoShape *basesub_19 = new TGeoCompositeShape("GE", basesub19);
   TGeoVolume*
   basesub_log19 = new TGeoVolume("",basesub_19, pMedAl);
   basesub_log19->SetVisLeaves(kTRUE);
   basesub_log19->SetLineColor(10);

   //pWorld->AddNode(basesub_log19, 0, pGlobal5);

    // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = -39.0/2.0+6.8+1.75;
   dy = -38.0/2.0-2.5;
   dz = 0.000;
   TGeoCombiTrans* pMatrix15d = new TGeoCombiTrans("", dx,dy,dz,0);


   TGeoUnion* basesub20 = new TGeoUnion(basesub_19, base16, pMatrix1, pMatrix15d);
   TGeoShape *basesub_20 = new TGeoCompositeShape("GE", basesub20);
   TGeoVolume*
   basesub_log20 = new TGeoVolume("plane5",basesub_20, pMedAl);
   basesub_log20->SetVisLeaves(kTRUE);
   basesub_log20->SetLineColor(29);

   pWorld->AddNode(basesub_log20, 0, pGlobal5);


   
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.000;
   dy = 21.000;
   dz = 1.0000;

   TGeoShape *Detector2 = new TGeoBBox("Detector_2", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   Detector_log2 = new TGeoVolume("Det_2",Detector2, pMedAr);
   Detector_log2->SetVisLeaves(kTRUE);
   Detector_log2->SetLineColor(3);

   //pWorld->AddNode(Detector_log2, 0, pGlobal5);

   //AddSensitiveVolume(Detector_log2);
   //fNbOfSensitiveVol+=1;

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.8-0.5-1.0-0.6;
   TGeoCombiTrans* pMatrix17 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 39.000;
   dy = 38.000;
   dz = 1.2000;
   TGeoShape *base17 = new TGeoBBox("base17", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log17 = new TGeoVolume("base_log17",base17, pMedAl);
   base_log17->SetVisLeaves(kTRUE);
   base_log17->SetLineColor(7);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal6 = GetGlobalPosition(pMatrix17);
   //pWorld->AddNode(base_log14, 0, pGlobal5);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.000;
   dy = 21.000;
   dz = 2.2000;
   TGeoShape *base18 = new TGeoBBox("base18", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log18 = new TGeoVolume("base_log18",base18, pMedAl);
   base_log18->SetVisLeaves(kTRUE);
   base_log18->SetLineColor(7);

   // Position Mother Volume
  // TGeoCombiTrans* pGlobal5 = GetGlobalPosition(pMatrix14);
  // pWorld->AddNode(base_log15, 0, pGlobal5);
   

   TGeoSubtraction* basesub21 = new TGeoSubtraction(base17,base18,pMatrix1,pMatrix1);
   TGeoShape *basesub_21 = new TGeoCompositeShape("GE", basesub21);
   TGeoVolume*
   basesub_log21 = new TGeoVolume("",basesub_21, pMedAl);
   basesub_log21->SetVisLeaves(kTRUE);
   basesub_log21->SetLineColor(7);

   //pWorld->AddNode(basesub_log21, 0, pGlobal6);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 4.000;
   dy = 6.600;
   dz = 4.000;
   TGeoShape *base19 = new TGeoBBox("base19", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log19 = new TGeoVolume("base_log19",base19, pMedAl);
   base_log19->SetVisLeaves(kTRUE);
   base_log19->SetLineColor(18);

   dx = 39.0/2.0-2.0;
   dy = 38.0/2.0+3.3;
   dz = -2.000-0.6;
   TGeoCombiTrans* pMatrix17b = new TGeoCombiTrans("gaz", dx,dy,dz,0);
   TGeoCombiTrans* pGlobal6b = GetGlobalPosition(pMatrix17b);
   pWorld->AddNode(base_log19, 0, pGlobal6b);

   dx = -39.0/2.0-2.0;
   dy = -38.0/2.0+3.3;
   dz = 2.000-0.6;
   TGeoCombiTrans* pMatrix18 = new TGeoCombiTrans("", dx,dy,dz,0);

   TGeoUnion* basesub22 = new TGeoUnion(basesub_21,base19,pMatrix1,pMatrix18);
   TGeoShape *basesub_22 = new TGeoCompositeShape("GE", basesub22);
   TGeoVolume*
   basesub_log22 = new TGeoVolume("Plane6",basesub_22, pMedAl);
   basesub_log22->SetVisLeaves(kTRUE);
   basesub_log22->SetLineColor(17);

   pWorld->AddNode(basesub_log22, 0, pGlobal6);


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.8-0.5-1.0-1.2-0.2-0.0020;
   TGeoCombiTrans* pMatrix19 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 22.950;
   dy = 22.950;
   dz = 0.4000;
   TGeoShape *base20 = new TGeoBBox("base20", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log20 = new TGeoVolume("base_log20",base20, pMedAl);
   base_log20->SetVisLeaves(kTRUE);
   base_log20->SetLineColor(17);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal7 = GetGlobalPosition(pMatrix19);
   //pWorld->AddNode(base_log14, 0, pGlobal5);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.000;
   dy = 21.000;
   dz = 1.0000;
   TGeoShape *base21 = new TGeoBBox("base21", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log21 = new TGeoVolume("base_log21",base21, pMedAl);
   base_log21->SetVisLeaves(kTRUE);
   base_log21->SetLineColor(7);

   // Position Mother Volume
  // TGeoCombiTrans* pGlobal7 = GetGlobalPosition(pMatrix19);
  // pWorld->AddNode(base_log21, 0, pGlobal7);
   

   TGeoSubtraction* basesub23 = new TGeoSubtraction(base20,base21,pMatrix1,pMatrix1);
   TGeoShape *basesub_23 = new TGeoCompositeShape("GE", basesub23);
   TGeoVolume*
   basesub_log23 = new TGeoVolume("",basesub_23, pMedAl);
   basesub_log23->SetVisLeaves(kTRUE);
   basesub_log23->SetLineColor(17);

   pWorld->AddNode(basesub_log23, 0, pGlobal7);

   dx = 0.000;
   dy = 0.000;
   dz = 0.60+0.0010+0.2;
   TGeoCombiTrans* pMatrix19b = new TGeoCombiTrans("", dx,dy,dz,0);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal7b = GetGlobalPosition(pMatrix19b);
   pWorld->AddNode(basesub_log23, 1, pGlobal7b);


   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.8-0.5-1.0-1.2-0.0010;
   TGeoCombiTrans* pMatrix20 = new TGeoCombiTrans("", dx,dy,dz,0);

   // Shape: sofia1 type: TGeoTubeSeg
   dx = 22.90;
   dy = 22.90;
   dz = 0.0020;
   TGeoShape *base22 = new TGeoBBox("base22", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log22 = new TGeoVolume("base_log22",base22, pMedMylar);// first Kapton
   base_log22->SetVisLeaves(kTRUE);
   base_log22->SetLineColor(28);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal8 = GetGlobalPosition(pMatrix20);
   pWorld->AddNode(base_log22, 1, pGlobal8);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 21.00;
   dy = 21.00;
   dz = 0.0020;
   TGeoShape *base22b = new TGeoBBox("base22b", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log22b = new TGeoVolume("base_log22b",base22b, pMedMylar);// Second Kapton
   base_log22b->SetVisLeaves(kTRUE);
   base_log22b->SetLineColor(28);

   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.8-0.5+0.0010;
   TGeoCombiTrans* pMatrix20b = new TGeoCombiTrans("", dx,dy,dz,0);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal8b = GetGlobalPosition(pMatrix20b);
   pWorld->AddNode(base_log22b, 1, pGlobal8b);

   dx = 0.000;
   dy = 0.000;
   dz = -1.800-0.0010;
   TGeoCombiTrans* pMatrix20c = new TGeoCombiTrans("", dx,dy,dz,0);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal8c = GetGlobalPosition(pMatrix20c);
   pWorld->AddNode(base_log22b, 2, pGlobal8c);

   dx = 0.000;
   dy = 0.000;
   dz = 0.60+0.0010;
   TGeoCombiTrans* pMatrix20d = new TGeoCombiTrans("", dx,dy,dz,0);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal8d = GetGlobalPosition(pMatrix20d);
   pWorld->AddNode(base_log22, 2, pGlobal8d);



   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 39.0/2.0+7.0/2.0;
   dy = 0.000;
   dz = -1.200;
   TGeoCombiTrans* pMatrix21 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 7.0000;
   dy = 16.950;
   dz = 0.4000;
   TGeoShape *base23 = new TGeoBBox("base23", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log23 = new TGeoVolume("base_log23",base23, pMedMylar);
   base_log23->SetVisLeaves(kTRUE);
   base_log23->SetLineColor(3);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal9 = GetGlobalPosition(pMatrix21);
   pWorld->AddNode(base_log23, 0, pGlobal9); 

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = -39.0/2.0-7.0/2.0;
   dy = 0.000;
   dz = -1.800-0.8-0.5/2.0;
   TGeoCombiTrans* pMatrix21b = new TGeoCombiTrans("", dx,dy,dz,0);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal9b = GetGlobalPosition(pMatrix21b);
   pWorld->AddNode(base_log23, 1, pGlobal9b); 

   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = -0.7000;
   dy = 38.0/2.0+3.5;
   dz = -1.800-0.8-0.5-0.5;

   TGeoRotation *rot_1 = new TGeoRotation("rot_1");
   rot_1->RotateZ(90.0);

   TGeoCombiTrans* pMatrix21c = new TGeoCombiTrans("", dx,dy,dz, rot_1);
   TGeoCombiTrans* pMatrix21d = new TGeoCombiTrans("", dx,-1.0*dy,dz, rot_1);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal9c = GetGlobalPosition(pMatrix21c);
   pWorld->AddNode(base_log23, 2, pGlobal9c); 

   TGeoCombiTrans* pGlobal9d = GetGlobalPosition(pMatrix21d);
   pWorld->AddNode(base_log23, 3, pGlobal9d); 


   // TRANSFORMATION MATRICES
   // Combi transformation: 
   dx = 53.80/2.0+2.15;
   dy = 0.000;
   dz = -0.60;
   TGeoCombiTrans* pMatrix22 = new TGeoCombiTrans("", dx,dy,dz,0);
   // Shape: sofia1 type: TGeoTubeSeg
   dx = 16.7;
   dy = 31.0;
   dz = 0.70;
   TGeoShape *base24 = new TGeoBBox("base24", dx/2.,dy/2.,dz/2.);
   // Volume: 
   TGeoVolume*
   base_log24 = new TGeoVolume("base_log24",base24, pMedAl);
   base_log24->SetVisLeaves(kTRUE);
   base_log24->SetLineColor(25);

   // Position Mother Volume
   TGeoCombiTrans* pGlobal10 = GetGlobalPosition(pMatrix22);
  // pWorld->AddNode(base_log24, 0, pGlobal10);


   // Shape: sofia1 type: TGeoTubeSeg
   dx = 6.50;
   dy = 25.0;
   dz = 1.70;
   TGeoShape *base25 = new TGeoBBox("base25", dx/2.,dy/2.,dz/2.);

   dx = -8.35+3.10;
   dy = 0.000;
   dz = 0.000;
   TGeoCombiTrans* pMatrix23 = new TGeoCombiTrans("", dx,dy,dz,0);

   TGeoSubtraction* basesub24 = new TGeoSubtraction(base24,base25,pMatrix1,pMatrix23);
   TGeoShape *basesub_24 = new TGeoCompositeShape("GE", basesub24);
   TGeoVolume*
   basesub_log24 = new TGeoVolume("",basesub_24, pMedAl);
   basesub_log24->SetVisLeaves(kTRUE);
   basesub_log24->SetLineColor(25);

   //pWorld->AddNode(basesub_log24, 0, pGlobal10);

    // Shape: sofia1 type: TGeoTubeSeg
   dx = 4.10;
   dy = 9.0;
   dz = 1.70;
   TGeoShape *base26 = new TGeoBBox("base25", dx/2.,dy/2.,dz/2.);

   dx = -8.35+6.20+2.0;
   dy = 0.000;
   dz = 0.000;
   TGeoCombiTrans* pMatrix24 = new TGeoCombiTrans("", dx,dy,dz,0);

   TGeoSubtraction* basesub25 = new TGeoSubtraction(basesub_24,base26,pMatrix1,pMatrix24);
   TGeoShape *basesub_25 = new TGeoCompositeShape("GE", basesub25);
   TGeoVolume*
   basesub_log25 = new TGeoVolume("",basesub_25, pMedAl);
   basesub_log25->SetVisLeaves(kTRUE);
   basesub_log25->SetLineColor(25);

   //pWorld->AddNode(basesub_log25, 0, pGlobal10);


   TGeoRotation *rot_2 = new TGeoRotation("rot_2");
   rot_2->RotateZ(60.0);

   dx = 8.35;
   dy = 15.50;
   dz = 0.000;
   TGeoCombiTrans* pMatrix25 = new TGeoCombiTrans("", dx,dy,dz,rot_2);

   // Shape: sofia1 type: TGeoTubeSeg
   dx = 10.72;
   dy = 19.72;
   dz = 1.70;
   TGeoShape *base27 = new TGeoBBox("base27", dx/2.,dy/2.,dz/2.);

   TGeoSubtraction* basesub26 = new TGeoSubtraction(basesub_25,base27,pMatrix1,pMatrix25);
   TGeoShape *basesub_26 = new TGeoCompositeShape("GE", basesub26);
   TGeoVolume*
   basesub_log26 = new TGeoVolume("",basesub_26, pMedAl);
   basesub_log26->SetVisLeaves(kTRUE);
   basesub_log26->SetLineColor(25);

  // pWorld->AddNode(basesub_log26, 0, pGlobal10);

   TGeoRotation *rot_3 = new TGeoRotation("rot_3");
   rot_3->RotateZ(-60.0);

   dx = 8.35;
   dy = -15.50;
   dz = 0.000;
   TGeoCombiTrans* pMatrix26 = new TGeoCombiTrans("", dx,dy,dz,rot_3);


   TGeoSubtraction* basesub27 = new TGeoSubtraction(basesub_26,base27,pMatrix1,pMatrix26);
   TGeoShape *basesub_27 = new TGeoCompositeShape("GE", basesub27);
   TGeoVolume*
   basesub_log27 = new TGeoVolume("",basesub_27, pMedAl);
   basesub_log27->SetVisLeaves(kTRUE);
   basesub_log27->SetLineColor(24);

   pWorld->AddNode(basesub_log27, 0, pGlobal10);


}

ClassImp(R3BMWpc)
