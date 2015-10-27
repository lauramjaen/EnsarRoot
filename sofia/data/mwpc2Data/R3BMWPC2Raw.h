// -------------------------------------------------------------------------
// -----                      R3BMWPC2Raw header file   	       -----
// -----                  Created 07/10/12  by J.L. Rodriguez 	       -----
//								       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BMWPC2Raw.h
 **  A MWPC2 raw is the representation of the MWPC data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BMWPC2Raw_H
#define R3BMWPC2Raw_H

#include "FairMultiLinkedData.h"
#include "TClonesArray.h"

class R3BMWPCPads;

class R3BMWPC2Raw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BMWPC2Raw();

  /** Constructor with arguments   
   **/

  R3BMWPC2Raw(TClonesArray* Clon);

  /** Copy constructor **/
  R3BMWPC2Raw(const R3BMWPC2Raw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BMWPC2Raw();

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
  
  ClassDef(R3BMWPC2Raw,1)

};

#endif
