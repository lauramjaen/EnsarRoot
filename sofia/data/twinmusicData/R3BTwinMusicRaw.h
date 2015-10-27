// -------------------------------------------------------------------------
// -----                      R3BTwinMusicRaw header file   	       -----
// -----                  Created 03/10/12  by H. Alvarez Pol 	       -----
// -----                                     & J.L. Rodriguez          -----
// ----- Modified:						       -----
//                        			                       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BTwinMusicRaw.h
 **  A TwinMusic raw is the representation of the TwinMusic data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BTWINMUSICRAW_H
#define R3BTWINMUSICRAW_H

#include "FairMultiLinkedData.h"

#define TwinMusic_NbAnodesPerSide  10

class R3BTwinMusicRaw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BTwinMusicRaw();

  /** Constructor with arguments
   *@param StatusAnodeRight    
   *@param StatusAnodeLeft    
   We use binary code, first bit means:
   If anode is active 1, not active 0
   second bit means:
   If anode has amplification of beam 0, amplification of fission fragments 1 
   This means: 3 is active with amplification of fission fragments
               1 is active with amplification of beam
               0 is not active
              -1 not used

   *@param DtimeRight       Drift time anodes right
   *@param DtimeLeft        Drift time anodes left
   *@param ChargeRight      Charge anodes right
   *@param ChargeLeft       Charge anodes left
   **/

  R3BTwinMusicRaw( Int_t StatusAnodeRight[TwinMusic_NbAnodesPerSide], Int_t StatusAnodeLeft[TwinMusic_NbAnodesPerSide], 
                   Double_t DtimeRight[TwinMusic_NbAnodesPerSide], Double_t DtimeLeft[TwinMusic_NbAnodesPerSide], 
                   Double_t ChargeRight[TwinMusic_NbAnodesPerSide], Double_t ChargeLeft[TwinMusic_NbAnodesPerSide]);

  /** Copy constructor **/
  R3BTwinMusicRaw(const R3BTwinMusicRaw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BTwinMusicRaw();

  /** Geometry

  Z -> Beam direction
  Y -> Anti-gravitational direction
  X -> dextrogire
   
  Right is -X direction
  Left is X direction

   **/

  /** Accessors **/
 
  Int_t GetStatusAnodeRight(Int_t i)    const { return fStatusAnodeRight[i]; }
  Int_t GetStatusAnodeLeft(Int_t i)     const { return fStatusAnodeLeft[i]; }
  Double_t GetDtimeright(Int_t i)          const { return fDtimeRight[i]; }
  Double_t GetDtimeleft(Int_t i)           const { return fDtimeLeft[i]; }
  Double_t GetChargeright(Int_t i)         const { return fChargeRight[i]; }
  Double_t GetChargeleft(Int_t i)          const { return fChargeLeft[i]; }

  /** Modifiers **/

  void SetStatusAnodeRight(Int_t anode, Int_t status);
  void SetStatusAnodeLeft(Int_t anode, Int_t status);
  void SetDtimeright(Int_t anode, Double_t Dtime);
  void SetDtimeleft(Int_t anode, Double_t Dtime);
  void SetChargeright(Int_t anode, Double_t Charge);
  void SetChargeleft(Int_t anode, Double_t Charge);

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  Int_t fStatusAnodeRight[TwinMusic_NbAnodesPerSide];
  Int_t fStatusAnodeLeft[TwinMusic_NbAnodesPerSide];
  Double_t fDtimeRight[TwinMusic_NbAnodesPerSide];
  Double_t fDtimeLeft[TwinMusic_NbAnodesPerSide];
  Double_t fChargeRight[TwinMusic_NbAnodesPerSide];
  Double_t fChargeLeft[TwinMusic_NbAnodesPerSide];

  ClassDef(R3BTwinMusicRaw,1)

};


inline void R3BTwinMusicRaw::SetStatusAnodeRight(Int_t anode, Int_t status){
  fStatusAnodeRight[anode]=status;
}

inline void R3BTwinMusicRaw::SetStatusAnodeLeft(Int_t anode, Int_t status){
  fStatusAnodeLeft[anode]=status;
}

inline void R3BTwinMusicRaw::SetDtimeright(Int_t anode, Double_t Dtime){
  fDtimeRight[anode]=Dtime;
}

inline void R3BTwinMusicRaw::SetDtimeleft(Int_t anode, Double_t Dtime){
  fDtimeLeft[anode]=Dtime;
}

inline void R3BTwinMusicRaw::SetChargeright(Int_t anode, Double_t Charge){
  fChargeRight[anode]=Charge;
}

inline void R3BTwinMusicRaw::SetChargeleft(Int_t anode, Double_t Charge){
  fChargeLeft[anode]=Charge;
}


#endif
