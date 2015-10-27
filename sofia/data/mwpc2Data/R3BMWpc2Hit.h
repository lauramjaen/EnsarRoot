// -------------------------------------------------------------------------
// -----                  R3BMWpc2Hit header file                      -----
// -----               Created 06/02/12  by J. Luis Rodriguez Sanchez  -----
// -------------------------------------------------------------------------

/**  R3BMWpc2Hit.h
 **
 **/

#ifndef R3BMWpc2Hit_H
#define R3BMWpc2Hit_H

#include "TObject.h"
#include "FairMultiLinkedData.h"

class R3BMWpc2Hit : public FairMultiLinkedData 
{
public:

  /** Default constructor **/
  R3BMWpc2Hit();

  /** Constructor with arguments
   *@param fmwpc2Id		mwpc2 unique identifier
   *@param fmwpc2Type		mwpc2 type
   *@param fmwpc2Copy           mwpc2 copy
   *@param fEnergy		total energy deposited on the mwpc2 [GeV]
   *@param fTime                Time since event start [ns]
   **/
	R3BMWpc2Hit(Int_t type, Int_t copy, Int_t ident,Double_t energy, Double_t time);

  /** Copy constructor **/
  R3BMWpc2Hit(const R3BMWpc2Hit& hit) { *this = hit; };

  /** Destructor **/
  virtual ~R3BMWpc2Hit();

  /** Accessors **/
  Double_t Getmwpc2Type()   const { return fmwpc2Type; }
  Double_t Getmwpc2Copy()   const { return fmwpc2Copy; }
  Double_t Getmwpc2Id()     const { return fmwpc2Id; }
  Double_t GetEnergy()      const { return fEnergy; }
  Double_t GetTime()        const { return fTime; }
	
  /** Modifiers **/
  void Setmwpc2Type(Int_t type){fmwpc2Type = type;}
  void Setmwpc2Copy(Int_t copy){fmwpc2Copy = copy;}
  void Setmwpc2Id(Int_t ident){fmwpc2Id = ident;}
  void SetEnergy(Double32_t energy){fEnergy = energy;}
  void AddMoreEnergy(Double32_t moreEnergy){fEnergy += moreEnergy;}
  void AddMoreEnergyb(Double32_t moreEnergy){fEnergy = moreEnergy;}
  void SetTime(Double32_t time){fTime = time;}

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

protected:

  Double32_t fEnergy;  //total energy in the mwpc2
  Double32_t fTime;    //time of the interaction
  Int_t fmwpc2Type;    //mwpc2 type
  Int_t fmwpc2Copy;    //mwpc2 copy
  Int_t fmwpc2Id;      //mwpc2 unique identifier

  ClassDef(R3BMWpc2Hit,1)

};

#endif
