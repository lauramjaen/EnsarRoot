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
   **/
  TraRPCSaeta(Double_t x, Double_t slopex, Double_t y, Double_t slopey , Double_t time, Double_t slowness);

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

  /** Modifiers **/
  void SetX(Double_t x)               { fX = x; }
  void SetY(Double_t y)               { fY = y; }
  void SetSlopeX(Double_t slopex)     { fSlopeX = slopex; }
  void SetSlopeY(Double_t slopey)     { fSlopeY = slopey; }
  void SetTime(Double_t time)         { fTime = time; }
  void SetSlowness(Double_t slowness) { fSlowness = slowness; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

protected:
  Double_t fX;         // X coordinate
  Double_t fY;         // Y coordinate
  Double_t fSlopeX;    // X slope 
  Double_t fSlopeY;    // Y slope
  Double_t fTime;      // Track time
  Double_t fSlowness;  // Track slowness (1/v)

// --------------------------
  ClassDef(TraRPCSaeta,1)
};

#endif
