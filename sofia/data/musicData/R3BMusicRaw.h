// -------------------------------------------------------------------------
// -----                      R3BMusicRaw header file   	       -----
// -----                  Created 08/10/12  by J.L. Rodriguez &        ----- 
// -----                                       H. Alvarez Pol	       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BMusicRaw.h
 **  A Music1 raw is the representation of the Music1 data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BMusicRaw_H
#define R3BMusicRaw_H

#include "FairMultiLinkedData.h"

#define Music_NbAnodes 8

class R3BMusicRaw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BMusicRaw();

  /** Constructor with arguments
   *@param StatusAnode                If anode is active 1, not active 0, not used -1
   *@param Charge[Music1_NbAnodes]    Charge
   **/

  R3BMusicRaw(Int_t Charge[Music_NbAnodes], Int_t StatusAnode[Music_NbAnodes]);

  /** Copy constructor **/
  R3BMusicRaw(const R3BMusicRaw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BMusicRaw();

  /** Accessors **/
 
  Int_t GetStatusAnode(Int_t i)    const { return fStatusAnode[i]; }
  Int_t GetCharge(Int_t i)         const { return fCharge[i]; }

  /** Modifiers **/

  void SetStatusAnode(Int_t Anode, Int_t Status);
  void SetCharge(Int_t Anode, Int_t Charge);

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  Int_t fStatusAnode[Music_NbAnodes];
  Int_t fCharge[Music_NbAnodes];
  
  ClassDef(R3BMusicRaw,1)

};

inline void R3BMusicRaw::SetStatusAnode(Int_t Anode, Int_t Status){
  fStatusAnode[Anode]=Status;
}

inline void R3BMusicRaw::SetCharge(Int_t Anode, Int_t Charge){
  fCharge[Anode]=Charge;
}

#endif
