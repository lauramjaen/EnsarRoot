// -------------------------------------------------------------------------
// -----                  TraRPCHit header file                -----
// -----	     Last modification 18/10/14 by H. Alvarez        -----
// -------------------------------------------------------------------------

/**  TraRPCHit.h
 **  A RPC hit is the representation of the
 **  information obtained from each RPC.
 **/

#ifndef TraRPCHit_H
#define TraRPCHit_H

#include "TObject.h"
#include "FairMultiLinkedData.h"

class TraRPCHit : public FairMultiLinkedData
{
public:

  /** Default constructor **/
  TraRPCHit();

  /** Constructor with arguments
   *@param fRPCId           RPC unique identifier
   *@param fCharge          Total charge deposited on the RPC ([GeV] in sim)
   *@param fTime            Time since event start [ns]
   *@param fPosXIn,fPosYIn  Incoming position given by Geant [cm]
   **/
  TraRPCHit(Int_t id, Double_t charge, Double_t time, Double_t posxin, Double_t posyin);

  /** Copy constructor **/
  TraRPCHit(const TraRPCHit&);

  TraRPCHit& operator=(const TraRPCHit&) { return *this; }

  /** Destructor **/
  virtual ~TraRPCHit();

  /** Accessors **/
  // Hit: Fired cell with a charge value bigger than a given threshold.
  // fRPCId decodes the plane number (0 to 3, 3 bits PPP),
  // row (X direction, 0 to 11 in 4 bits RRRR),
  // column (Y direction, 0 to 9, 4 bits TTTT)
  // acording to 0PPP00RRRR00TTTT
  // GetRPCId(), etc give the cell information in decimal format from the binary fRPCId

  Int_t    GetRPCId()         const { return  fRPCId;   }
  Int_t    GetCellColumn()    const { return  (((fRPCId & 960) >> 6) +1); }
  Int_t    GetCellRow()       const { return  ((fRPCId & 15) +1); }
  Int_t    GetPlane()         const { return  (((fRPCId & 28672) >> 12)+1);  }
  Double_t GetCellXCenter()   const { return  69.3-(12-this->GetCellColumn())*12.6; }
  Double_t GetCellYCenter()   const { return  54.45-(this->GetCellRow()-1)*12.10; }
  Double_t GetCharge()        const { return  fCharge;  }
  Double_t GetTime()          const { return  fTime;    }
  Double_t GetPosXIn()        const { return  fPosXIn;  }
  Double_t GetPosYIn()        const { return  fPosYIn;  }

  /** Modifiers **/
  void SetRPCId(Int_t id) { fRPCId = id; }
  void SetCharge(Double32_t charge){fCharge = charge;}
  void AddCharge(Double32_t charge){fCharge += charge;}
  void SetTime(Double32_t time){fTime = time;}
  void SetPosXIn(Double_t posxin){fPosXIn = posxin;}
  void SetPosYIn(Double_t posyin){fPosYIn = posyin;}

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

protected:
  Double32_t fCharge;  //total charge in the RPC
  Double32_t fTime;    //TDC time
  Int_t fRPCId;        //RPC unique identifier
  Double_t fPosXIn;    //Xin position from Geant
  Double_t fPosYIn;    //Yin position from Geant

  Double32_t fRPCcol;
// --------------------------
  ClassDef(TraRPCHit,2)
};

#endif
