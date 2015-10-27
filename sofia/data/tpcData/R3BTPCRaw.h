// -------------------------------------------------------------------------
// -----                      R3BTPCRaw header file   	               -----
// -----                  Created 10/10/12  by J.L. Rodriguez &        ----- 
// -----                                       H. Alvarez Pol	       -----
//								       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BTPCRaw.h
 **  A TPC raw is the representation of the TPC data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BTPCRaw_H
#define R3BTPCRaw_H

#include "FairMultiLinkedData.h"

#define TPC_NbAnodes      4
#define TPC_NbDelayLines  2

class R3BTPCRaw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BTPCRaw();

  /** Constructor with arguments
   *@param StatusAnode            Status anode, if anode is active 1, not active 0, not used -1
   *@param TimeAnode              Time anode
   *@param ChargeAnode            Charge right
   *@param Time_DelayLineRight    Time delay right
   *@param Charge_DelayLineRight  Charge delay right
   *@param Time_DelayLineLeft     Time delay left
   *@param Charge_DelayLineLeft   Charge delay left
   **/

  R3BTPCRaw(Int_t StatusAnode[TPC_NbAnodes], Int_t TimeAnode[TPC_NbAnodes], Int_t ChargeAnode[TPC_NbAnodes],
            Int_t Time_DelayLineRight[TPC_NbDelayLines], Int_t Charge_DelayLineRight[TPC_NbDelayLines], 
            Int_t Time_DelayLineLeft[TPC_NbDelayLines], Int_t Charge_DelayLineLeft[TPC_NbDelayLines]);

  /** Copy constructor **/
  R3BTPCRaw(const R3BTPCRaw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BTPCRaw();

  /** Accessors **/
 
  Int_t GetStatusAnode(Int_t i)              const { return fStatusAnode[i]; }
  Int_t GetTimeAnode(Int_t i)                const { return fTimeAnode[i]; }
  Int_t GetChargeAnode(Int_t i)              const { return fChargeAnode[i]; }
  Int_t GetTime_DelayLineRight(Int_t i)      const { return fTime_DelayLineRight[i]; }
  Int_t GetCharge_DelayLineRight(Int_t i)    const { return fCharge_DelayLineRight[i]; }
  Int_t GetTime_DelayLineLeft(Int_t i)       const { return fTime_DelayLineLeft[i]; }
  Int_t GetCharge_DelayLineLeft(Int_t i)     const { return fCharge_DelayLineLeft[i]; }

  /** Modifiers **/

  void SetStatusAnode(Int_t Anode, Int_t Status);
  void SetTimeAnode(Int_t Anode, Int_t Time);
  void SetChargeAnode(Int_t Anode, Int_t Charge);
  void SetTime_DelayLineRight(Int_t NbDelay, Int_t Time_DLR);
  void SetCharge_DelayLineRight(Int_t NbDelay, Int_t Charge_DLR);
  void SetTime_DelayLineLeft(Int_t NbDelay, Int_t Time_DLL);
  void SetCharge_DelayLineLeft(Int_t NbDelay, Int_t Charge_DLL);

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  Int_t fStatusAnode[TPC_NbAnodes];
  Int_t fTimeAnode[TPC_NbAnodes];
  Int_t fChargeAnode[TPC_NbAnodes];
  Int_t fTime_DelayLineRight[TPC_NbDelayLines];
  Int_t fCharge_DelayLineRight[TPC_NbDelayLines];
  Int_t fTime_DelayLineLeft[TPC_NbDelayLines];
  Int_t fCharge_DelayLineLeft[TPC_NbDelayLines];

  ClassDef(R3BTPCRaw,1)

};


inline void R3BTPCRaw::SetStatusAnode(Int_t Anode, Int_t Status){
  fStatusAnode[Anode]=Status;
}

inline void R3BTPCRaw::SetTimeAnode(Int_t Anode, Int_t Time){
  fTimeAnode[Anode]=Time;
}

inline void R3BTPCRaw::SetChargeAnode(Int_t Anode, Int_t Charge){
  fChargeAnode[Anode]=Charge;
}

inline void R3BTPCRaw::SetTime_DelayLineRight(Int_t NbDelay, Int_t Time_DLR){
  fTime_DelayLineRight[NbDelay]=Time_DLR;
}

inline void R3BTPCRaw::SetCharge_DelayLineRight(Int_t NbDelay, Int_t Charge_DLR){
  fCharge_DelayLineRight[NbDelay]=Charge_DLR;
}

inline void R3BTPCRaw::SetTime_DelayLineLeft(Int_t NbDelay, Int_t Time_DLL){
  fTime_DelayLineLeft[NbDelay]=Time_DLL;
}

inline void R3BTPCRaw::SetCharge_DelayLineLeft(Int_t NbDelay, Int_t Charge_DLL){
  fCharge_DelayLineLeft[NbDelay]=Charge_DLL;
}

#endif
