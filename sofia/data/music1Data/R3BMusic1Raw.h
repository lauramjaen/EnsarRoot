// -------------------------------------------------------------------------
// -----                      R3BMusicRaw header file   	       -----
// -----                  Created 08/10/12  by J.L. Rodriguez 	       -----
//								       -----
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
   *@param Charge[Music1_NbAnodes]    Charge
   **/

  R3BMusicRaw(Int_t Charge[Music_NbAnodes]);

  /** Copy constructor **/
  R3BMusicRaw(const R3BMusicRaw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BMusicRaw();

  /** Accessors **/
 
  Double_t GetCharge(Int_t i)    const { return fCharge[i]; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  Int_t fCharge[Music_NbAnodes];
  
  ClassDef(R3BMusicRaw,1)

};


#endif
