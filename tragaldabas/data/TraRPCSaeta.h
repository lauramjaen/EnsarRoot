// -----------------------------------------------------------------------
// -----                  TraRPCSaeta header file                    -----
// -----	     Created 29/08/2016 by P. Cabanelas              -----
// -----------------------------------------------------------------------

/**  TraRPCSaeta.h
 **  A RPC saeta is the representation of the
 **  information obtained after applying the TimTrack to the RPC Hits.
 **/

#ifndef TraRPCSaeta_H
#define TraRPCSaeta_H

#include "TObject.h"
#include "FairMultiLinkedData.h"
#include "TMatrixD.h"
#include "TPolyLine3D.h"

class TraRPCSaeta : public FairMultiLinkedData
{
public:

  /** Default constructor **/
  TraRPCSaeta();

  /** Constructor with arguments
   *@param fX          X axis coordinate
   *@param fSlopeX     Slope in X
   *@param fY          Y axis coordinate
   *@param fSlopeY     Slope in Y
   *@param fTime       Time since event start [ns]
   *@param fSlowness   Track slowness (1/velocity)
   *@param fSize       Number of hits in saeta
   *@param fID         ID of the origin plane from 1 to 4
   *@param fIndexHit   Index to get the hit information
   **/
  TraRPCSaeta(TMatrixD Saeta, Int_t ID, Int_t size);

  /** Copy constructor **/
  TraRPCSaeta(const TraRPCSaeta&);

  TraRPCSaeta& operator=(const TraRPCSaeta&) { return *this; }

  /** Destructor **/
  virtual ~TraRPCSaeta();

  /** Accessors **/
  Double_t GetX()         const { return fX; }
  Double_t GetY()         const { return fY; }
  Double_t GetSlopeX()    const { return fSlopeX; }
  Double_t GetSlopeY()    const { return fSlopeY; }
  Double_t GetTime()      const { return fTime; }
  Double_t GetSlowness()  const { return fSlowness; }
  Int_t    GetSize()      const { return fSize; }
  Double_t GetID()        const { return fID; }

  void GetHitsIndices(Int_t& index1,Int_t& index2,Int_t& index3,Int_t& index4)   
  {
	  index1 = fIndexHit1;
	  index2 = fIndexHit2;
	  index3 = fIndexHit3;
	  index4 = fIndexHit4;
  } 

  Int_t GetHitIndex(Int_t n) {
	  if(n == 0)      return fIndexHit1; 
	  else if(n == 1) return fIndexHit2;  
	  else if(n == 2) return fIndexHit3;
	  else if(n == 3) return fIndexHit4;
	  else return -1;
  }

  /** Modifiers **/
  void SetX(Double_t x)               { fX = x; }
  void SetY(Double_t y)               { fY = y; }
  void SetSlopeX(Double_t slopex)     { fSlopeX = slopex; }
  void SetSlopeY(Double_t slopey)     { fSlopeY = slopey; }
  void SetTime(Double_t time)         { fTime = time; }
  void SetSlowness(Double_t slowness) { fSlowness = slowness; }
  void SetSize(Int_t size)            { fSize = size; }                       
  void SetHitsIndices(Int_t index1, Int_t index2, Int_t index3, Int_t index4)  { fIndexHit1 = index1; fIndexHit2 = index2; fIndexHit3 = index3; fIndexHit4 = index4; }                       
  void SetID(Int_t id) { fID = id; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

  /** Transport saeta **/
  void Transport(Double_t deltaX, Double_t deltaY, Double_t deltaZ) { fX=fX+deltaX+fSlopeX*deltaZ; fY=fY+deltaY+fSlopeY*deltaZ; }

  /** Generate a straight trajectory from saeta **/
  TPolyLine3D GenerateLine(TraRPCSaeta& saeta);

protected:
  Double_t fX;         // X coordinate
  Double_t fY;         // Y coordinate
  Double_t fSlopeX;    // X slope 
  Double_t fSlopeY;    // Y slope
  Double_t fTime;      // Track time
  Double_t fSlowness;  // Track slowness (1/v)
  Double_t fID;        // ID of the origin plane
  Int_t    fSize;      // Number of hits in saeta
  Int_t    fIndexHit1;
  Int_t    fIndexHit2;
  Int_t    fIndexHit3;
  Int_t    fIndexHit4;

// --------------------------
  ClassDef(TraRPCSaeta,1)
};

#endif
