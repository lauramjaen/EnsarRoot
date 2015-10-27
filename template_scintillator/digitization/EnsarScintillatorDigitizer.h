/**  EnsarScintillatorDigitizer.h **/

#ifndef TSCINTIDETDIGITIZER_H
#define TSCINTIDETDIGITIZER_H 1

#include "FairTask.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom3;
class EnsarScintillatorDigi;
class EnsarScintillatorDigiPar;

struct PM_RES
{
  Double_t time,lightCFD,lightQDC,Energy;
};

/// EnsarScintillatorDigitizer.h
class EnsarScintillatorDigitizer : public FairTask {

public:
   /**
     * \author  M.I.Cherciu \n
     * Modified by: \n   
     * Comment: \n
     * \version 1.0 \n
     * \since 2014/07/02 \n
     * Change date: \n
     * \file EnsarScintillatorDigi.h
     * \brief Class for digitisation of the Scintillator Detector.
     */	
  
  /** Default Constructor **/
  EnsarScintillatorDigitizer();

  /** Constructor **/  
  EnsarScintillatorDigitizer(Int_t verbose);

  /** Default Destructor **/
  ~EnsarScintillatorDigitizer();

  /** Intialisation at begin of run. 
   * If not kSUCCESS, task will be set inactive.
   **/
  virtual InitStatus Init();
  
  /** Virtual method Execution Task **/
  virtual void Exec(Option_t* opt);
  
  /** Action after each run. **/
  virtual void Finish();
  
  /** Action after each Event. **/
  virtual void FinishEvent();
  
  /** Virtual method Reset Method **/
  virtual void Reset();
  
  /** Add Digi Point Method **/
  EnsarScintillatorDigi* AddHit(Double_t tdc, Double_t qdc, Double_t xx,Double_t yy, Double_t zz );
 
  
protected:
     
  TClonesArray *fScintillatorPoints;     /**< Container for hits. **/
  TClonesArray *fMCTrack;                /**< Container for tracks. **/
  TClonesArray *fEnsarScintillatorDigiCollection; /**< Container for digits. **/

  /** Access to the Digi Patameters List. **/
  EnsarScintillatorDigiPar *fScintillatorDigiPar;  /**< Container for digi parameters. **/

  // Control Hitograms
  TH1F *hPM; /**< Histogram from Photo Multiplicator, of time arival. **/
  TH1F *hMult; /**< Histogram from Photo Multiplicator, of multiplicity. **/
 
  Int_t eventNo; /**< Event number variable. **/
  Double_t thresh; /**< Threshold variable. **/
  Double_t plength; /**< Depth of the scintillator. **/
  Double_t att; /**< Light attenuation factor [1/cm]. **/
  Double_t c; /**< Speed of light. **/
  Double_t cMedia; /**< Speed of light in material in cm/ns. **/
  PM_RES *PM_res;

private:
  
  /** Virtual Method to Access Parameter Container. **/
  virtual void SetParContainers();

 
  ClassDef(EnsarScintillatorDigitizer,1)
};


#endif
