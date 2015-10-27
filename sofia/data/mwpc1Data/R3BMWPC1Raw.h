// -------------------------------------------------------------------------
// -----                      R3BMWPC1Raw header file   	       -----
// -----                  Created 07/10/12  by J.L. Rodriguez 	       -----
//								       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BMWPC1Raw.h
 **  A Mwpc1 raw is the representation of the Mwpc data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BMWPC1Raw_H
#define R3BMWPC1Raw_H

#include "FairMultiLinkedData.h"
#include "TClonesArray.h"

class R3BMWPC1Raw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BMWPC1Raw();

  /** Constructor with arguments   
   **/

  R3BMWPC1Raw(TClonesArray* Clon);

  /** Copy constructor **/
  R3BMWPC1Raw(const R3BMWPC1Raw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BMWPC1Raw();

  /** Accessors **/
 
  TClonesArray* GetClonesArray() const;

  /** Modifiers **/

  void SetArrayPads(TClonesArray* pads){
  fMWPads=pads;
  };
 
  /** Output to screen **/
  virtual void Print() const;

 protected:
  
  TClonesArray* fMWPads;
  
  ClassDef(R3BMWPC1Raw,1)

};

#endif
