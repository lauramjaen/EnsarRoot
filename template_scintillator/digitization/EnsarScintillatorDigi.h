/**  EnsarScintillatorDigi.h **/


#ifndef TSCINTIDETDIGI_H
#define TSCINTIDETDIGI_H


#include "TObject.h"
#include "TVector3.h"


/// EnsarScintillatorDigi.h
class EnsarScintillatorDigi : public TObject
{

 public:
   /**
     * \author  M.I.Cherciu \n
     * Modified by: \n   
     * Comment: \n
     * \version 1.0 \n
     * \since 2014/07/02 \n
     * Change date: \n
     * \file EnsarScintillatorDigi.h
     * \brief Class for Monte Carlo digits of Scintillator Detector.
     */	
   
  /** Default Constructor **/
  EnsarScintillatorDigi();
  
  /** Standard constructor **/
  EnsarScintillatorDigi(Double_t tdc, Double_t qdc, Double_t xx, Double_t yy, Double_t zz);
  
  /** Copy constructor **/
  EnsarScintillatorDigi(const EnsarScintillatorDigi&);

  /** Defaults Destructor **/
  virtual ~EnsarScintillatorDigi();

  
  //EnsarScintillatorDigi& operator=(const EnsarScintillatorDigi&) { return *this; }
  
  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

  /** Set time to digital. */
  void SetTdc(Double_t time){fTdc = time;}

  /** Get time to digital. */
  Double_t GetTdc() const {return fTdc;}

  /**  Set charge to digital. */
  void SetQdc(Double_t charge){fQdc = charge;}
  
  /** Get charge to digital. */
  Double_t GetQdc(){return fQdc;}

  /** Set hit X position. */
  void SetXX(Double_t pos){fxx = pos;}
  /** Get hit X position. */ 
  Double_t GetXX(){return fxx;}

  /** Set hit Y position. */
  void SetYY(Double_t pos){fyy = pos;}
  
  /** Get hit Y position. */
  Double_t GetYY(){return fyy;}

  /** Set hit Z position. */
  void SetZZ(Double_t pos){fzz = pos;}
  
  /**  Get hit Z position. */
  Double_t GetZZ(){return fzz;}


 protected:

  Double_t fTdc; /**< Ttime to digital converter. **/
  Double_t fQdc; /**< Charge to digital converter. **/
  Double_t fxx;  /**< X local interaction point in the detector. **/ 
  Double_t fyy;  /**< Y local interaction point in the detector. **/
  Double_t fzz;  /**< Z local interaction point in the detector. **/


  ClassDef(EnsarScintillatorDigi,1)

};

#endif
