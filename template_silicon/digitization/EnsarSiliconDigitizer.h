/**  EnsarSiliconDigitizer.h **/
#ifndef TSILIDETDIGITIZER_H
#define TSILIDETDIGITIZER_H 1

#include "FairTask.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom3;
class EnsarSiliconDigi;
class EnsarSiliconDigiPar;


/// EnsarSiliconDigitizer.h

class EnsarSiliconDigitizer : public FairTask {

public:
   /**
     * \author  M.I.Cherciu
     * \version 1.0
     * \since 10.2014
     * \file EnsarSiliconDigi.h
     * \brief Class for digitisation of the Silicon Detector.
     */	
  
  /** Default Constructor **/
  EnsarSiliconDigitizer();

  /** Constructor **/  
  EnsarSiliconDigitizer(Int_t verbose);

  /** Default Destructor **/
  ~EnsarSiliconDigitizer();

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
  EnsarSiliconDigi* AddHit(Double_t time, Double_t energy, Double_t xplane, Double_t yplane, Double_t zplane );
 
  
protected:
     
  TClonesArray *fSiliconPoints;     /**< Container for hits. **/
  TClonesArray *fMCTrack;                /**< Container for tracks. **/
  TClonesArray *fEnsarSiliconDigiCollection; /**< Container for digits. **/

  /** Access to the Digi Patameters List. **/
  EnsarSiliconDigiPar *fSiliconDigiPar;  /**< Container for digi parameters. **/

  // Control Hitograms
  TH1F *hSD; /**< Histogram from Silicon Detector, of time arival. **/
  TH1F *hMult; /**< Histogram from Silicon Detector, of multiplicity. **/
 
  Int_t eventNo; /**< Event number variable. **/
  Double_t thresh; /**< Threshold variable. **/
  Double_t plength; /**< Depth of the silicon. **/

private:
  
  /** Virtual Method to Access Parameter Container. **/
  virtual void SetParContainers();

 
  ClassDef(EnsarSiliconDigitizer,1)
};


#endif
