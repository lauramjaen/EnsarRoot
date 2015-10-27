// -------------------------------------------------------------------------
// -----                  R3BMWpcHit header file                       -----
// -----               Created 06/02/12  by J. Luis Rodriguez Sanchez  -----
// -------------------------------------------------------------------------

/**  R3BMWpcHit.h
 **
 **/

#ifndef R3BMWpcHit_H
#define R3BMWpcHit_H

#include "TObject.h"
#include "FairMultiLinkedData.h"

class R3BMWpcHit : public FairMultiLinkedData 
{
public:

  /** Default constructor **/
  R3BMWpcHit();

  /** Constructor with arguments
   *@param fmwpc1Id		mwpc1 unique identifier
   *@param fmwpc1Type		mwpc1 type
   *@param fmwpc1Copy           mwpc1 copy
   *@param fEnergy		total energy deposited on the mwpc1 [GeV]
   *@param fTime                Time since event start [ns]
   **/

  R3BMWpcHit(Int_t type, Int_t copy, Int_t ident,Double_t energy, Double_t time);

  /** Copy constructor **/
  R3BMWpcHit(const R3BMWpcHit& hit) { *this = hit; };

  /** Destructor **/
  virtual ~R3BMWpcHit();

  /** Accessors **/
  Double_t Getmwpc1Type()   const { return fmwpc1Type; }
  Double_t Getmwpc1Copy()   const { return fmwpc1Copy; }
  Double_t Getmwpc1Id()     const { return fmwpc1Id; }
  Double_t GetEnergy()      const { return fEnergy; }
  Double_t GetTime()        const { return fTime; }
	
  /** Modifiers **/
  void Setmwpc1Type(Int_t type){fmwpc1Type = type;}
  void Setmwpc1Copy(Int_t copy){fmwpc1Copy = copy;}
  void Setmwpc1Id(Int_t ident){fmwpc1Id = ident;}
  void SetEnergy(Double32_t energy){fEnergy = energy;}
  void AddMoreEnergy(Double32_t moreEnergy){fEnergy += moreEnergy;}
  void AddMoreEnergyb(Double32_t moreEnergy){fEnergy = moreEnergy;}
  void SetTime(Double32_t time){fTime = time;}

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

protected:

  Double32_t fEnergy;  //total energy in the mwpc1
  Double32_t fTime;    //time of the interaction
  Int_t fmwpc1Type;  //mwpc1 type
  Int_t fmwpc1Copy;  //mwpc1 copy
  Int_t fmwpc1Id;    //mwpc1 unique identifier

  ClassDef(R3BMWpcHit,1)

};

#endif
