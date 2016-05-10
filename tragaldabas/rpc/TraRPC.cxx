// -------------------------------------------------------------------------
// -----                         TraRPC source file                    -----
// -----           Created by H.Alvarez (hector.alvarez@usc.es)        -----
// -----	          Last modification 26/10/14 by H. Alvarez           -----
// -------------------------------------------------------------------------
#include <iostream>
#include <stdlib.h>

#include "TraRPC.h"

#include "TraRPCPoint.h"
#include "TraRPCHit.h"
#include "EnsarMCStack.h"

#include "FairGeoInterface.h"
#include "FairGeoLoader.h"
#include "FairGeoNode.h"
#include "FairGeoRootBuilder.h"
#include "FairRootManager.h"
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
#include "TGeoBoolNode.h"
#include "TGeoCompositeShape.h"

using std::cout;
using std::cerr;
using std::endl;

// -----   Default constructor   -------------------------------------------
TraRPC::TraRPC() : EnsarDetector("TraRPC", kTRUE, kRPC)
{
  ResetParameters();
  fRPCCollection = new TClonesArray("TraRPCPoint");
  fRPCHitCollection = new TClonesArray("TraRPCHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  fGeometryVersion = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
TraRPC::TraRPC(const char* name, Bool_t active)
    : EnsarDetector(name, active, kRPC)
{
  ResetParameters();
  fRPCCollection = new TClonesArray("TraRPCPoint");
  fRPCHitCollection = new TClonesArray("TraRPCHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  fGeometryVersion = 1;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
TraRPC::~TraRPC()
{
  if (fRPCCollection) {
    fRPCCollection->Delete();
    delete fRPCCollection;
  }
  if (fRPCHitCollection) {
    fRPCHitCollection->Delete();
    delete fRPCHitCollection;
  }
}
// -------------------------------------------------------------------------
void TraRPC::Initialize()
{
  FairDetector::Initialize();

  LOG(INFO) << "TraRPC: initialisation" << FairLogger::endl;
  LOG(DEBUG) << "-I- TraRPC: Vol (McId) def" << FairLogger::endl;

  TGeoVolume *vol = gGeoManager->GetVolume("TragaWorld");
  vol->SetVisibility(kFALSE);
}



// -----   Public method ProcessHits  --------------------------------------
Bool_t TraRPC::ProcessHits(FairVolume* vol)
{
  Int_t volId1=0; // the gas volume Id
  Int_t cp1=0; // the copy of the gas (0 or 1 for the lower or upper gas gap)
  Int_t volIdMetaIn=0; // metacrilate inner box volume Id
  Int_t cpMetaIn=0; // copy of metacrilate inner box (always 0)
  Int_t volIdMeta=0; // metacrilate box volume Id
  Int_t cpMeta=0; // copy of metacrilate box (always 0)
  Int_t volIdAluIn=0; // aluminum inner box volume Id
  Int_t cpAluIn=0; // copy of aluminum inner box (always 0)
  Int_t volIdAlu=0; // aluminum box volume Id
  Int_t cpAlu=0; // copy of aluminum  box (always 0)
  Int_t volIdKIV=0; // KIV box volume Id
  Int_t cpKIV=0; // copy of KIV  box (from 0 to 3 in present Tragaldabas)

  Int_t RPCId =0; //RPC WITH SIGNAL!!!

  // RPC readout
  const char* bufferName = gMC->CurrentVolName();
  volId1 = gMC->CurrentVolID(cp1);
  volIdMetaIn = gMC->CurrentVolOffID(1,cpMetaIn);
  volIdMeta = gMC->CurrentVolOffID(2,cpMeta);
  volIdAluIn = gMC->CurrentVolOffID(3,cpAluIn);
  volIdAlu = gMC->CurrentVolOffID(4,cpAlu);
  volIdKIV = gMC->CurrentVolOffID(5,cpKIV);

  LOG(DEBUG) << "CurrentVolName: " << bufferName << ", volId: "
    <<  volId1 << ", cp: " << cp1 << FairLogger::endl;
  //if (fGeometryVersion==0){
  // TRAGALDABAS 2014 description
  // the RPC plane is given by the variable cpKIV,
  // running from 0 (lower at -90cm) to 3 (upper at +90)
  //}
  //else LOG(ERROR) << "TraRPC: Geometry version not available in TraRPC::ProcessHits(). "
  //  << FairLogger::endl;

  LOG(DEBUG) << "TraRPC: Processing Points in RPC_Plane Nb "
    << cpKIV << FairLogger::endl;

    // Hit: Fired cell with a charge value bigger than a given threshold.
    // fRPCId decodes the plane number (1 to 4, 3 bits PPP),
    // column (X direction, 0 to 11 in 4 bits TTTT),
    // row (Y direction, 0 to 9, 4 bits RRRR)
    // acording to 0PPP00TTTT00CCCC
    // The numbers 12.6 and 12.3 come from our cell dimensions. The origin is placed at the center of the plane
    gMC->TrackPosition(fPosIn);
    RPCId = cpKIV*4096 + int((fPosIn.X()+6*12.6)/12.6)*64 + int((-fPosIn.Y()+5*12.3)/12.3);


  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fNSteps  = 0; // FIXME
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fEinc   = gMC->Etot();                  //be aware!! Relativistic mass!
  }

  // Sum energy loss for all steps in the active volume
  Double_t dE = gMC->Edep() * 1000.;         //in MeV
  Double_t post_E = (gMC->Etot() - gMC->TrackMass()) * 1000.;      //in MeV
  // TString motherID = gMC->GetStack()->GetCurrentTrack()->GetMotherID();
//  manera de crear un pointer para los parmametros de la traza
// mirar si es gMC->MotherID()
 TString ptype = gMC->GetStack()->GetCurrentTrack()->GetName();

  fELoss += dE / 1000.;       //back to GeV
  fNSteps++;

  // Set additional parameters at exit of active volume. Create TraRPCPoint.
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

      gGeoManager->SetCurrentDirection(-newdirection[0],
				       -newdirection[1],
				       -newdirection[2]);

      for (Int_t i=0; i<3; i++) {
        newpos[i] = oldpos[i] - (3*safety*olddirection[i]);
      }

      fPosOut.SetX(newpos[0]);
      fPosOut.SetY(newpos[1]);
      fPosOut.SetZ(newpos[2]);
    }

    AddHit(fTrackID, fVolumeID, RPCId,
           TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
           fTime, fLength, fELoss);

    // Increment number of RPCPoints for this track
    EnsarMCStack* stack = (EnsarMCStack*) gMC->GetStack();
    stack->AddPoint(kTra);

    //Adding a RPCHIT support
    Int_t nHits = fRPCHitCollection->GetEntriesFast();
    Bool_t existHit = 0;

    if (nHits==0) AddRPCHit(RPCId, fELoss, fTime, fNSteps,
				       fEinc, fTrackID, fVolumeID,
				       fParentTrackID, fTrackPID, fUniqueID);
    else {
      for (Int_t i=0; i<nHits; i++) {
        if ( ((TraRPCHit *)(fRPCHitCollection->At(i)))->GetRPCId() == RPCId ) {
          ((TraRPCHit *)(fRPCHitCollection->At(i)))->AddCharge(fELoss);
          if ( ((TraRPCHit *)(fRPCHitCollection->At(i)))->GetTime() > fTime ) {
            ((TraRPCHit *)(fRPCHitCollection->At(i)))->SetTime(fTime);
          }
          existHit=1; //to avoid the creation of a new Hit
          break;
        }
      }
      if (!existHit) AddRPCHit(RPCId, fELoss, fTime, fNSteps,
               fEinc, fTrackID, fVolumeID,
               fParentTrackID, fTrackPID, fUniqueID);
    }

    existHit=0;

    ResetParameters();
  }
  return kTRUE;
}
// ----------------------------------------------------------------------------

// -----   Public method EndOfEvent   -----------------------------------------
void TraRPC::BeginEvent()
{
}
// ----------------------------------------------------------------------------

// -----   Public method EndOfEvent   -----------------------------------------
void TraRPC::EndOfEvent()
{
  if (fVerboseLevel) Print();

  fRPCCollection->Clear();
  fRPCHitCollection->Clear();

  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method Register   -------------------------------------------
void TraRPC::Register()
{
 FairRootManager::Instance()->Register("RPCPoint", GetName(),
                                       fRPCCollection, kTRUE);
  FairRootManager::Instance()->Register("RPCHit", GetName(),
    fRPCHitCollection, kTRUE);

}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* TraRPC::GetCollection(Int_t iColl) const
{
  if(iColl == 0) {
    return fRPCCollection;
  } else if(iColl == 2) {
    return fRPCHitCollection;
  }
  else return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void TraRPC::Print(Option_t* option) const
{
  Int_t nPoints = fRPCCollection->GetEntriesFast();
  LOG(INFO) << "TraRPC: " << nPoints << " points registered in this event"
	    << FairLogger::endl;
  Int_t nRPCHits = fRPCHitCollection->GetEntriesFast();
  LOG(INFO) << "TraRPC: " << nRPCHits << " hits registered in this event."
	    << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void TraRPC::Reset()
{
  fRPCCollection->Clear();
  fRPCHitCollection->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void TraRPC::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "TraRPC: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  TraRPCPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (TraRPCPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) TraRPCPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(INFO) << "TraRPC: " << cl2->GetEntriesFast() << " merged entries"
	    << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddHit   --------------------------------------------
TraRPCPoint* TraRPC::AddHit(Int_t trackID, Int_t detID, Int_t volid,
            TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
                              Double_t time, Double_t length, Double_t eLoss)
{
  TClonesArray& clref = *fRPCCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1)
    LOG(INFO) << "TraRPC: Adding Point at (" << posIn.X() << ", " << posIn.Y()
	      << ", " << posIn.Z() << ") cm,  detector " << detID << ", track "
	      << trackID << ", energy loss " << eLoss*1e06 << " keV"
	      << FairLogger::endl;
  return new(clref[size]) TraRPCPoint(trackID, detID, volid,
				       posIn, posOut, momIn, momOut, time, length, eLoss);
}
// ----------------------------------------------------------------------------

// -----   Private method AddRPCHit   --------------------------------------------
TraRPCHit* TraRPC::AddRPCHit(Int_t detID,Double_t energy, Double_t time,
					     Int_t steps, Double_t einc,
					     Int_t trackid, Int_t volid,
					     Int_t partrackid, Int_t pdgtype,
					     Int_t uniqueid)
{
  TClonesArray& clref = *fRPCHitCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1) {
    LOG(INFO) << "-I- TraRPC: Adding Hit in detector with unique identifier " << detID
	      << " entering with " << einc*1e06 << " keV, depositing " << energy*1e06
	      << " keV" << FairLogger::endl;
    LOG(INFO) << " -I- trackid: " << trackid << " volume id: " << volid
	      << " partrackid : " << partrackid << " type: " << pdgtype
	      << " unique id: " << uniqueid << FairLogger::endl;
  }
  return new(clref[size]) TraRPCHit(detID, energy, time);
}

// -----   Public method ConstructGeometry   ----------------------------------
void TraRPC::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing Tragaldabas geometry from ROOT file " << fileName.Data()
	      << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Tragaldabas geometry file name is not specified" << FairLogger::endl;
    exit(1);
  }
}
// ----------------------------------------------------------------------------


Bool_t TraRPC::CheckIfSensitive(std::string name)
{
  if(TString(name).Contains("RPC_GasBox")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------


ClassImp(TraRPC)
