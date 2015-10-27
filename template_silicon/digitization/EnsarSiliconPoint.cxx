/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0 / 2014
*    ------------------------------------------------------------------------
*/

// EnsarRoot includes
#include "EnsarSiliconPoint.h"

// C++ includes
#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
EnsarSiliconPoint::EnsarSiliconPoint() : FairMCPoint() 
{
  fX_in       = fY_in   = fZ_in   = 0.;
  fPx_in      = fPy_in  = fPz_in  = 0.;  
  fX_out      = fY_out  = fZ_out  = 0.;
  fPx_out     = fPy_out = fPz_out = 0.;
}

// -----   Standard constructor   ------------------------------------------
EnsarSiliconPoint::EnsarSiliconPoint(Int_t trackID, Int_t volumeID,
                         Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
                         TVector3 posIn,TVector3 posOut, TVector3 momIn, TVector3 momOut,
			 Double_t energy, Double_t tof, Double_t length, Double_t eLoss)
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
  fTrackID       = trackID;
  fVolumeID      = volumeID;
  fParentTrackID = parentTrackID; 
  fTrackPID      = trackPID;
  fTrackUniqueID = uniqueID;
}
                                                                                                           
// -----   Default Destructor   ----------------------------------------------------
EnsarSiliconPoint::~EnsarSiliconPoint() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void EnsarSiliconPoint::Print(const Option_t* opt) const {
  cout << "-I- EnsarSiliconPoint: Detector Point for track " << fTrackID 
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength 
       << " cm,  Energy loss " << fELoss*1.0e06 << " MeV" << endl;
}

ClassImp(EnsarSiliconPoint)
