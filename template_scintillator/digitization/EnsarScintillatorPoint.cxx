/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/

#include "EnsarScintillatorPoint.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
EnsarScintillatorPoint::EnsarScintillatorPoint() : FairMCPoint() 
{
  fX_in       = fY_in   = fZ_in   = 0.;
  fPx_in      = fPy_in  = fPz_in  = 0.;  
  fX_out      = fY_out  = fZ_out  = 0.;
  fPx_out     = fPy_out = fPz_out = 0.;
  fLightYield = -1;
}
// -----   Standard constructor   ------------------------------------------

EnsarScintillatorPoint::EnsarScintillatorPoint(Int_t trackID, Int_t volumeID,
                         Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                         TVector3 posIn,TVector3 posOut, TVector3 momIn, TVector3 momOut,
			 Double_t energy, Double_t tof, Double_t length, Double_t eLoss, Double_t lightYield)
  : FairMCPoint(trackID, volumeID, posIn, momIn, tof, length, eLoss) {
  fX_in  = posIn.X();
  fY_in  = posIn.Y();
  fZ_in  = posIn.Z();
  fPx_in = momIn.Px();
  fPy_in = momIn.Py();
  fPz_in = momIn.Pz();
  fX_out  = posOut.X();
  fY_out  = posOut.Y();
  fZ_out  = posOut.Z();
  fPx_out = momOut.Px();
  fPy_out = momOut.Py();
  fPz_out = momOut.Pz();
  fEnergy = energy;
  fTof = tof;
  fELoss  = eLoss;
  fLightYield = lightYield;
  fTrackID       = trackID;
  fVolumeID      = volumeID;
  fParentTrackID = parentTrackID; 
  fTrackPID      = trackPID;
  fTrackUniqueID = uniqueID;
}
                                                                                                           
// -----   Destructor   ----------------------------------------------------
EnsarScintillatorPoint::~EnsarScintillatorPoint() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void EnsarScintillatorPoint::Print(const Option_t* opt) const {
  cout << "-I- EnsarScintillatorPoint: Detector Point for track " << fTrackID 
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength 
       << " cm,  Energy loss " << fELoss*1.0e06 << " keV" << endl;
}

// -----   Point x coordinate from linear extrapolation   ------------------
Double_t EnsarScintillatorPoint::GetX(Double_t z) const {
  //  cout << fZ << " " << z << " " << fZ_out << endl;
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}

// -----   Point y coordinate from linear extrapolation   ------------------
Double_t EnsarScintillatorPoint::GetY(Double_t z) const {
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}

// -----   Public method IsUsable   ----------------------------------------
Bool_t EnsarScintillatorPoint::IsUsable() const {
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}

ClassImp(EnsarScintillatorPoint)
