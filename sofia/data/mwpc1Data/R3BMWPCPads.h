// -------------------------------------------------------------------------
// -----                      R3BMWPCPads header file   	       -----
// -----                  Created 07/10/12  by J.L. Rodriguez 	       -----
//								       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BMWPCPads.h
 **  A Mwpc pad is the representation of the Mwpc pad data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BMWPCPads_H
#define R3BMWPCPads_H

#include "FairMultiLinkedData.h"

class R3BMWPCPads : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BMWPCPads();

  /** Constructor with arguments   
   *@param PosX_Chargeup     X Charge up  
   *@param PosX_Chargedown   X Charge down
   *@param PosY_Charge       Y Charge 
   **/

  R3BMWPCPads(Int_t pad, UShort_t PosX_Chargeup, UShort_t PosX_Chargedown, UShort_t PosY_Charge);

  /** Copy constructor **/
  R3BMWPCPads(const R3BMWPCPads& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BMWPCPads();

  /** Accessors **/
 
  Int_t    GetPad()               const { return fPad; }
  UShort_t GetPosX_Chargeup()     const { return fPosX_Chargeup; }
  UShort_t GetPosX_Chargedown()   const { return fPosX_Chargedown; }
  UShort_t GetPosY_Charge()       const { return fPosY_Charge; }

  /** Modifiers **/

  void SetPad(Int_t pad);
  void SetPosX_Chargeup(UShort_t Charge);
  void SetPosX_Chargedown(UShort_t Charge);
  void SetPosY_Charge(UShort_t Charge);
  
  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  Int_t    fPad;
  UShort_t fPosX_Chargeup;
  UShort_t fPosX_Chargedown;
  UShort_t fPosY_Charge;
  
  ClassDef(R3BMWPCPads,1)

};

inline void R3BMWPCPads::SetPad(Int_t pad){
  fPad=pad;
}

inline void R3BMWPCPads::SetPosX_Chargeup(UShort_t Charge){
  fPosX_Chargeup=Charge;
}

inline void R3BMWPCPads::SetPosX_Chargedown(UShort_t Charge){
  fPosX_Chargedown=Charge;
}

inline void R3BMWPCPads::SetPosY_Charge(UShort_t Charge){
  fPosY_Charge=Charge;
}

#endif
