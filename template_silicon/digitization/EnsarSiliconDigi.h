/**  EnsarSiliconDigi.h **/


#ifndef TSILIDETDIGI_H
#define TSILIDETDIGI_H


#include "TObject.h"
#include "TVector3.h"


/// EnsarSiliconDigi.h

class EnsarSiliconDigi : public TObject
{

 public:
   /**
     * \author  M.I.Cherciu
     * \version 1.0
     * \since 10.2014
     * \file EnsarSiliconDigi.h
     * \brief Class for Monte Carlo digits of Silicon Detector.
     */	
   
  /** Default Constructor **/
  EnsarSiliconDigi();
  
  /** Standard constructor **/
  EnsarSiliconDigi(Double_t time, Double_t energy, Double_t xplane, Double_t yplane, Double_t zplane);
  
  /** Copy constructor **/
  EnsarSiliconDigi(const EnsarSiliconDigi&);

  /** Defaults Destructor **/
  virtual ~EnsarSiliconDigi();

  //EnsarSiliconDigi& operator=(const EnsarSiliconDigi&) { return *this; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

  /** Set time. */
  void SetTime(Double_t time){fTime = time;}

  /** Get time. */
  Double_t GetTime() const {return fTime;}

  /**  Set charge to digital. */
  void SetEnergy(Double_t energy){fEnergy = energy;}
  
  /** Get charge to digital. */
  Double_t GetEnergy(){return fEnergy;}

  /** Set hit X position. */
  void SetXplane(Double_t xpos){fxplane = xpos;}
  /** Get hit X position. */ 
  Double_t GetXplane(){return fxplane;}

  /** Set hit Y position. */
  void SetYplane(Double_t ypos){fyplane = ypos;}
  
  /** Get hit Y position. */
  Double_t GetYplane(){return fyplane;}

  /** Set hit Z position. */
  void SetZplane(Double_t zpos){fzplane = zpos;}
  
  /**  Get hit Z position. */
  Double_t GetZplane(){return fzplane;}

 protected:

  Double_t fTime;    /**< Time of the digit. **/
  Double_t fEnergy;  /**< Energy deposit of the digit. **/
  Double_t fxplane;  /**< X local interaction point in the detector. **/
  Double_t fyplane;  /**< Y local interaction point in the detector. **/
  Double_t fzplane;  /**< Z local interaction point in the detector. **/

  ClassDef(EnsarSiliconDigi,1)

};

#endif
