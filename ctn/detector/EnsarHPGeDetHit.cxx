// -------------------------------------------------------------------------
// -----                      EnsarHPGeDetHit source file                  -----
// -------------------------------------------------------------------------

#include "EnsarHPGeDetHit.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
EnsarHPGeDetHit::EnsarHPGeDetHit() : FairMultiLinkedData() {

  fEnergy = -1;
  fTime   = -1;
  fEinc   = -1;
  fNSteps = -1;

}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
EnsarHPGeDetHit::EnsarHPGeDetHit(Int_t trackID, Int_t volumeID,
      Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
      Double_t energy, Double_t time, Int_t steps, Double_t einc) 
  : FairMultiLinkedData() {

  fTrackID       = trackID;
  fVolumeID      = volumeID;
  fParentTrackID = parentTrackID; 
  fTrackPID      = trackPID;
  fTrackUniqueID = uniqueID;
  fEnergy        = energy;
  fTime          = time;
  fEinc          = einc;
  fNSteps        = steps;

}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
EnsarHPGeDetHit::~EnsarHPGeDetHit() { }
// -------------------------------------------------------------------------




// -----   Public method Print   -------------------------------------------
void EnsarHPGeDetHit::Print(const Option_t* opt) const {
  cout << "-I- EnsarHPGeDetHit for track " << fTrackID 
       << " in detector " << fVolumeID << endl;
  cout << "    Time " << fTime << " ns, Energy loss " << fEnergy*1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------



ClassImp(EnsarHPGeDetHit)
