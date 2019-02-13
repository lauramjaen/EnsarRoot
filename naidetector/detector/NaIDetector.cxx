// -------------------------------------------------------------------------
// -----                    NaIDetector source file                    -----
// -----        Created by P.Cabanelas (pablo.cabanelas@usc.es)        -----
// -------------------------------------------------------------------------
#include <iostream>
#include <stdlib.h>

#include "NaIDetector.h"

#include "NaIPoint.h"
#include "NaIHit.h"
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
NaIDetector::NaIDetector() : EnsarDetector("NaI", kTRUE, kNAI)
{
  ResetParameters();
  fPointCollection = new TClonesArray("NaIPoint");
  fHitCollection   = new TClonesArray("NaIHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  fGeometryVersion = 1;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
NaIDetector::NaIDetector(const char* name, Bool_t active)
    : EnsarDetector(name, active, kNAI)
{
  ResetParameters();
  fPointCollection = new TClonesArray("NaIPoint");
  fHitCollection   = new TClonesArray("NaIHit");
  fPosIndex = 0;
  kGeoSaved = kFALSE;
  fVerboseLevel = 1;
  fGeometryVersion = 1;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
NaIDetector::~NaIDetector()
{
  if (fPointCollection) {
    fPointCollection->Delete();
    delete fPointCollection;
  }
  if (fHitCollection) {
    fHitCollection->Delete();
    delete fHitCollection;
  }
}
// -------------------------------------------------------------------------
void NaIDetector::Initialize()
{
  FairDetector::Initialize();

  LOG(INFO) << "NaIDetector: initialisation" << FairLogger::endl;
  LOG(DEBUG) << "-I- NaIDetector: Vol (McId) def" << FairLogger::endl;

  TGeoVolume *vol = gGeoManager->GetVolume("Target");
  vol->SetVisibility(kFALSE);
}



// -----   Public method ProcessHits  --------------------------------------
Bool_t NaIDetector::ProcessHits(FairVolume* vol)
{

  if ( gMC->IsTrackEntering() ) {
    fELoss  = 0.;
    fNSteps  = 0;
    fTime   = gMC->TrackTime() * 1.0e09;
    fLength = gMC->TrackLength();
    gMC->TrackPosition(fPosIn);
    gMC->TrackMomentum(fMomIn);
    fEinc   = gMC->Etot();                  //be aware!! Relativistic mass!
  }

  // Sum energy loss for all steps in the active volume
  fELoss += gMC->Edep();

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

    AddPoint(fTrackID, fVolumeID, fVolumeID,
           TVector3(fPosIn.X(),   fPosIn.Y(),   fPosIn.Z()),
           TVector3(fPosOut.X(),  fPosOut.Y(),  fPosOut.Z()),
           TVector3(fMomIn.Px(),  fMomIn.Py(),  fMomIn.Pz()),
           TVector3(fMomOut.Px(), fMomOut.Py(), fMomOut.Pz()),
           fTime, fLength, fELoss);

    // Increment number of Points for this track
    EnsarMCStack* stack = (EnsarMCStack*) gMC->GetStack();
    stack->AddPoint(kNAI);

    //Adding Hit support
    Int_t nHits = fHitCollection->GetEntriesFast();
    Bool_t existHit = 0;

    if (nHits==0) AddHit(fELoss, fTime, fNSteps,
				       fEinc, fTrackID, fVolumeID,
				       fParentTrackID, fTrackPID, fUniqueID);
    else {
      for (Int_t i=0; i<nHits; i++) {
        ((NaIHit *)(fHitCollection->At(i)))->AddEnergy(fELoss);
        if ( ((NaIHit *)(fHitCollection->At(i)))->GetTime() > fTime ) {
          ((NaIHit *)(fHitCollection->At(i)))->SetTime(fTime);
        }
        existHit=1; //to avoid the creation of a new Hit
        break;
      }
      if (!existHit) AddHit(fELoss, fTime, fNSteps,
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
void NaIDetector::BeginEvent()
{
}
// ----------------------------------------------------------------------------

// -----   Public method EndOfEvent   -----------------------------------------
void NaIDetector::EndOfEvent()
{
  if (fVerboseLevel) Print();

  fPointCollection->Clear();
  fHitCollection->Clear();

  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method Register   -------------------------------------------
void NaIDetector::Register()
{
 FairRootManager::Instance()->Register("NaIPoint", GetName(),
                                       fPointCollection, kTRUE);
  FairRootManager::Instance()->Register("NaIHit", GetName(),
    fHitCollection, kTRUE);

}
// ----------------------------------------------------------------------------

// -----   Public method GetCollection   --------------------------------------
TClonesArray* NaIDetector::GetCollection(Int_t iColl) const
{
  if(iColl == 0) {
    return fPointCollection;
  } else if(iColl == 2) {
    return fHitCollection;
  }
  else return NULL;
}
// ----------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void NaIDetector::Print(Option_t* option) const
{
  Int_t nPoints = fPointCollection->GetEntriesFast();
  LOG(INFO) << "NaI: " << nPoints << " points registered in this event"
	    << FairLogger::endl;
  Int_t nHits = fHitCollection->GetEntriesFast();
  LOG(INFO) << "NaI: " << nHits << " hits registered in this event."
	    << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void NaIDetector::Reset()
{
  fPointCollection->Clear();
  fHitCollection->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void NaIDetector::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
{
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "NaI: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  NaIPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
    oldpoint = (NaIPoint*) cl1->At(i);
    Int_t index = oldpoint->GetTrackID() + offset;
    oldpoint->SetTrackID(index);
    new (clref[fPosIndex]) NaIPoint(*oldpoint);
    fPosIndex++;
  }
  LOG(INFO) << "NaI: " << cl2->GetEntriesFast() << " merged entries"
	    << FairLogger::endl;
}
// ----------------------------------------------------------------------------

// -----   Private method AddPoint   --------------------------------------------
NaIPoint* NaIDetector::AddPoint(Int_t trackID, Int_t detID, Int_t volid,
            TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
                              Double_t time, Double_t length, Double_t eLoss)
{
  TClonesArray& clref = *fPointCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1)
    LOG(INFO) << "NaI: Adding Point at (" << posIn.X() << ", " << posIn.Y()
	      << ", " << posIn.Z() << ") cm,  detector " << detID << ", track "
	      << trackID << ", energy loss " << eLoss*1e06 << " keV"
	      << FairLogger::endl;
  return new(clref[size]) NaIPoint(trackID, detID, volid,
				       posIn, posOut, momIn, momOut, time, length, eLoss);
}
// ----------------------------------------------------------------------------

// -----   Private method AddHit   --------------------------------------------
NaIHit* NaIDetector::AddHit(Double_t energy, Double_t time,
					     Int_t steps, Double_t einc,
					     Int_t trackid, Int_t volid,
					     Int_t partrackid, Int_t pdgtype,
					     Int_t uniqueid)
{
  TClonesArray& clref = *fHitCollection;
  Int_t size = clref.GetEntriesFast();
  if (fVerboseLevel>1) {
    LOG(INFO) << "-I- NaI: Adding Hit in detector entering with " << einc*1e06 << " keV, depositing " << energy*1e06
	      << " keV" << FairLogger::endl;
    LOG(INFO) << " -I- trackid: " << trackid << " volume id: " << volid
	      << " partrackid : " << partrackid << " type: " << pdgtype
	      << " unique id: " << uniqueid << FairLogger::endl;
  }
  return new(clref[size]) NaIHit(energy, time);
}

// -----   Public method ConstructGeometry   ----------------------------------
void NaIDetector::ConstructGeometry()
{
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing geometry from ROOT file " << fileName.Data()
	      << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(FATAL) << "Detector geometry file name is not specified" << FairLogger::endl;
    exit(1);
  }
}
// ----------------------------------------------------------------------------


Bool_t NaIDetector::CheckIfSensitive(std::string name)
{
  if(TString(name).Contains("CrystalNaI")) {//for the original NaI detector this must be empty, ""
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------


ClassImp(NaIDetector)
