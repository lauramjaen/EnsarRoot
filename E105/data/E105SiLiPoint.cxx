#include "E105SiLiPoint.h"

#include <iostream>
using std::cout;
using std::endl;


// -----   Default constructor   -------------------------------------------
E105SiLiPoint::E105SiLiPoint()
  : FairMCPoint()
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
E105SiLiPoint::E105SiLiPoint(Int_t trackID, Int_t detID,
    TVector3 pos, TVector3 mom,
    TVector3 posOut, TVector3 momOut,
    Double_t tof, Double_t length,
    Double_t eLoss)
  : FairMCPoint(trackID, detID, pos, mom, tof, length, eLoss),
    fX_out(posOut.X()),
    fY_out(posOut.Y()),
    fZ_out(posOut.Z()),
    fPx_out(momOut.Px()),
    fPy_out(momOut.Py()),
    fPz_out(momOut.Pz())
{
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
E105SiLiPoint::~E105SiLiPoint() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void E105SiLiPoint::Print(const Option_t* opt) const
{
  cout << "-I- E105SiLiPoint: E105SiLi point for track " << fTrackID
       << " in detector " << fDetectorID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ
       << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz
       << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength
       << " cm,  Energy loss " << fELoss*1.0e06 << " keV" << endl;
}
// -------------------------------------------------------------------------

ClassImp(E105SiLiPoint)

