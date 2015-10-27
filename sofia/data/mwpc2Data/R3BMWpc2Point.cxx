// -------------------------------------------------------------------------
// -----                      R3BMWpc2Point source file                -----
// -------------------------------------------------------------------------

#include "R3BMWpc2Point.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
R3BMWpc2Point::R3BMWpc2Point() : FairMCPoint() {
  fX_out      = fY_out  = fZ_out  = 0.;
  fPx_out     = fPy_out = fPz_out = 0.;
  fmwpc2Type = fmwpc2Copy = fmwpc2Id = -1;
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
R3BMWpc2Point::R3BMWpc2Point(Int_t trackID, Int_t detID, Int_t volid, Int_t copy, Int_t ident,
			 TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
			 Double_t tof, Double_t length, Double_t eLoss) 
  : FairMCPoint(trackID, detID, posIn, momIn, tof, length, eLoss) {
  fTrackID = trackID; 
  fX_out  = posOut.X();
  fY_out  = posOut.Y();
  fZ_out  = posOut.Z();
  fPx_out = momOut.Px();
  fPy_out = momOut.Py();
  fPz_out = momOut.Pz();
  fmwpc2Type = volid;
  fmwpc2Copy = copy;
  fmwpc2Id = ident;
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
R3BMWpc2Point::~R3BMWpc2Point() { }
// -------------------------------------------------------------------------




// -----   Public method Print   -------------------------------------------
void R3BMWpc2Point::Print(const Option_t* opt) const {
  cout << "-I- R3BMWpc2Point: STS Point for track " << fTrackID 
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength 
       << " cm,  Energy loss " << fELoss*1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------



// -----   Point x coordinate from linear extrapolation   ------------------
Double_t R3BMWpc2Point::GetX(Double_t z) const {
  //  cout << fZ << " " << z << " " << fZ_out << endl;
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------



// -----   Point y coordinate from linear extrapolation   ------------------
Double_t R3BMWpc2Point::GetY(Double_t z) const {
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------



// -----   Public method IsUsable   ----------------------------------------
Bool_t R3BMWpc2Point::IsUsable() const {
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------


ClassImp(R3BMWpc2Point)
