// ----------------------------------------------------------------
// -----                  NaIHit header file                  -----
// -----	     Created 17/11/16 by P.Cabanelas          -----
// ----------------------------------------------------------------

/**  NaIHit.h
 **  Hit representation of the NaI detector
 **/

#ifndef NaIHit_H
#define NaIHit_H

#include "TObject.h"
#include "FairMultiLinkedData.h"

class NaIHit : public FairMultiLinkedData
{
public:

  /** Default constructor **/
  NaIHit();

  /** Constructor with arguments
   *@param fEnergy      Energy deposited on the NaI detector ([GeV] in sim)
   *@param fTime        Time since event start [ns]
   **/
  NaIHit(Double_t energy, Double_t time);

  /** Copy constructor **/
  NaIHit(const NaIHit&);

  NaIHit& operator=(const NaIHit&) { return *this; }

  /** Destructor **/
  virtual ~NaIHit();

  /** Accessors **/
  Double_t GetEnergy()        const { return  fEnergy;  }
  Double_t GetTime()          const { return  fTime;    }

  /** Modifiers **/
  void SetEnergy(Double32_t energy){fEnergy = energy;}
  void AddEnergy(Double32_t energy){fEnergy += energy;}
  void SetTime(Double32_t time){fTime = time;}

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

protected:
  Double32_t fEnergy;  //total energy in the NaI detector
  Double32_t fTime;    //Time

// --------------------------
  ClassDef(NaIHit,1)
};

#endif
