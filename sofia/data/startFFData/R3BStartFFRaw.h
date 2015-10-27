// -------------------------------------------------------------------------
// -----                      R3BStartFFRaw header file   	       -----
// -----                  Created 08/10/12  by J.L. Rodriguez 	       -----
//								       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BStartFFRaw.h
 **  A Start raw is the representation of the Start data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BStartFFRaw_H
#define R3BStartFFRaw_H

#include "FairMultiLinkedData.h"

#define StaFF_VFTX_N    4

class R3BStartFFRaw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BStartFFRaw();

  /** Constructor with arguments
   *@param Traw_right   Time right
   *@param Traw_left    Time left
   **/

  R3BStartFFRaw(Double_t Traw_right[StaFF_VFTX_N], Double_t Traw_left[StaFF_VFTX_N]);

  /** Copy constructor **/
  R3BStartFFRaw(const R3BStartFFRaw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BStartFFRaw();

  /** Accessors **/

  Double_t GetTraw_right(Int_t i)   const { return fTraw_right[i]; }
  Double_t GetTraw_left(Int_t i)    const { return fTraw_left[i]; }

  /** Modifiers **/

  void SetTraw_right(Int_t mod, Double_t time);
  void SetTraw_left(Int_t mod, Double_t time);

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  Double_t fTraw_right[StaFF_VFTX_N];
  Double_t fTraw_left[StaFF_VFTX_N];

  ClassDef(R3BStartFFRaw,1)

};


inline void R3BStartFFRaw::SetTraw_right(Int_t mod, Double_t time){
  fTraw_right[mod]=time;
}


inline void R3BStartFFRaw::SetTraw_left(Int_t mod, Double_t time){
  fTraw_left[mod]=time;
}


#endif
