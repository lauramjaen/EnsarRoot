// -------------------------------------------------------------------------
// -----                      R3BMusic2Raw header file   	       -----
// -----                  Created 08/10/12  by J.L. Rodriguez 	       -----
//								       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BMusic2Raw.h
 **  A Music2 raw is the representation of the Music2 data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BMUSIC2RAW_H
#define R3BMUSIC2RAW_H

#include "FairMultiLinkedData.h"

#define Music2_NbAnodes 4

class R3BMusic2Raw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BMusic2Raw();

  /** Constructor with arguments
   *@param fCharge[Music2_NbAnodes]    Charge
   **/

  R3BMusic2Raw(Double_t Charge[Music2_NbAnodes]);

  /** Copy constructor **/
  R3BMusic2Raw(const R3BMusic2Raw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BMusic2Raw();

  /** Geometry
  Z -> Beam direction
  Y -> Anti-gravitational direction
  X -> dextrogire 
   **/

  /** Accessors **/
 
  Double_t GetCharge(Int_t i)    const { return fCharge[i]; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  Double_t fCharge[Music2_NbAnodes];
  
  ClassDef(R3BMusic2Raw,1)

};


#endif
