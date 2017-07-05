// -------------------------------------------------------------------------
// -----                  TraRPCSaeeta source file                -----
// -------------------------------------------------------------------------

#include "TraRPCSaeta.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
TraRPCSaeta::TraRPCSaeta()
  : FairMultiLinkedData(),
  fX(-999.), fSlopeX(-999.), fY(-999.) , fSlopeY(-999.),
   fTime(-1.), fSlowness(-1), fID(-1), fSize(-1), fIndexHit1(-1), fIndexHit2(-1),fIndexHit3(-1),fIndexHit4(-1)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
TraRPCSaeta::TraRPCSaeta(TMatrixD S, Int_t id, Int_t size) 
  : FairMultiLinkedData(),
     fX(S[0][0]), fSlopeX(S[1][0]), fY(S[2][0]), fSlopeY(S[3][0]),
     fTime(S[4][0]), fSlowness(S[5][0]), fID(id), fSize(size)
{
}
// -------------------------------------------------------------------------

TraRPCSaeta::TraRPCSaeta(const TraRPCSaeta& saeta)
  : FairMultiLinkedData(saeta),
    fX(saeta.fX), fSlopeX(saeta.fSlopeX), fY(saeta.fY), fSlopeY(saeta.fSlopeY),
    fTime(saeta.fTime), fSlowness(saeta.fSlowness), fID(saeta.fID), fSize(saeta.fSize)
{
}
// -------------------------------------------------------------------------


// -----   Destructor   ----------------------------------------------------
TraRPCSaeta::~TraRPCSaeta()
{
}
// -------------------------------------------------------------------------

// -----   Public method Generate  -----------------------------------------
TPolyLine3D TraRPCSaeta::GenerateLine(TraRPCSaeta& S)
{
  // Saeta components:
  Double_t x0 = S.GetX();
  Double_t xp = S.GetSlopeX();
  Double_t y0 = S.GetY();
  Double_t yp = S.GetSlopeY();

  Int_t nPoints = 5; // Total number of points in the line
  Double_t zInf=-1.0, zSup=2.0; // z limits
  Double_t Dz=(zSup-zInf)/(nPoints-1); // z increment

  // We store the points in arrays
  Double_t x[nPoints];	
  Double_t y[nPoints];
  Double_t z[nPoints];
  TPolyLine3D line(nPoints, x, y, z);
   for(Int_t i=0;i<nPoints;i++){
   x[i]=x0+xp*(zInf+i*Dz);
   y[i]=y0+yp*(zInf+i*Dz);
   z[i]=i;
   }
  return line;
}
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void TraRPCSaeta::Print(const Option_t* opt) const 
{
  cout << "-I- TraRPCSaeta: a RPC Saeta in RPC detector with " << endl;
  cout << "    Time =" << fTime << " ns  " << endl;
  cout << "    Slowness = " << fSlowness << " (arbitrary units)" << endl;
  cout << "    X = " << fX << " and X' = " << fSlopeX << endl;
  cout << "    Y = " << fY << " and Y' = " << fSlopeY << endl;
  cout << "    ID = " << fID << endl;
  cout << " Size = " << fSize << endl;
  cout << "Hit index1 = " << fIndexHit1 << endl; 
  cout << "Hit index2 = " << fIndexHit2 << endl;
  cout << "Hit index3 = " << fIndexHit3 << endl;
  cout << "Hit index4 = " << fIndexHit4 << endl;
}
// -------------------------------------------------------------------------

