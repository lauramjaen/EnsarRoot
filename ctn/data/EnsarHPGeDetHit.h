// -------------------------------------------------------------------------
// -----                  EnsarHPGeDetHit header file                  -----
// -----                Created 04/09/19  by P.Cabanelas               -----
// -------------------------------------------------------------------------


/**  EnsarHPGeDetHit.h
 **/


#ifndef EnsarHPGeDetHit_H
#define EnsarHPGeDetHit_H


#include "TObject.h"
#include "FairMultiLinkedData.h"


class EnsarHPGeDetHit : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  EnsarHPGeDetHit();


  /** Constructor with arguments
   *@param fEnergy         Energy deposit [GeV]
   *@param fTime           Time since event start [ns]
   **/
  EnsarHPGeDetHit(Double_t energy, Double_t time);


  /** Copy constructor **/
  EnsarHPGeDetHit(const EnsarHPGeDetHit&);
  
  EnsarHPGeDetHit& operator=(const EnsarHPGeDetHit&){return *this;}


  /** Destructor **/
  virtual ~EnsarHPGeDetHit();


  /** Accessors **/
  Double_t GetEnergy()        const { return fEnergy; }
  Double_t GetTime()          const { return fTime; }

  /** Modifiers **/
  void SetEnergy(Double32_t energy) { fEnergy = energy; }
  void SetTime(Double32_t time)     { fTime = time; }
  void AddEnergy(Double32_t energy) { fEnergy += energy; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;



 protected:
  Double32_t fEnergy;    //total energy in the crystal
  Double32_t fTime;      //time of the interaction
 
  ClassDef(EnsarHPGeDetHit,2)  //se ha de canviar la version cada vez que se hace una modificacion grande

};


#endif
