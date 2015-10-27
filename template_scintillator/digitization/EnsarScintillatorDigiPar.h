/**  EnsarScintillatorDigiPar.h **/

#ifndef TSCINTIDETDIGIPAR_H
#define TSCINTIDETDIGIPAR_H

#include <TVector3.h>
#include <TObjString.h>

#include "FairParGenericSet.h"
#include "FairParamList.h"

/// EnsarScintillatorDigiPar.h
class EnsarScintillatorDigiPar : public FairParGenericSet
{
    public :
    /**
     * \author  M.I.Cherciu \n
     * Modified by: \n   
     * Comment: \n
     * \version 1.0 \n
     * \since 2014/07/02 \n
     * Change date: \n
     * \file EnsarScintillatorDigiPar.h
     * \brief Class for digitization parameters of Scintillator Detector.
     */	
      
       /** Default constructor **/
	EnsarScintillatorDigiPar (const char* name="EnsarScintillatorDigiPar",
			const char* title="Tutorial  parameter",
			const char* context="TestDefaultContext");
      /** Defaults Destructor **/
	~EnsarScintillatorDigiPar(void){};

	/** Putting Digi parameters to the List. **/
	void putParams(FairParamList* list);
	
	/** Getting Digi parameters from the List. **/
	Bool_t getParams(FairParamList* list);
 
	/** Print information to the output. **/
	void Print(Option_t* option="") const;
	
	/** Get Physical Length of the Scintillator. **/ 
	const Double_t GetScintillatorLength() { return  sciLength;}
	
	/** Get Physical Height of the Scintillator. **/
	const Double_t GetScintillatorHeight() { return  sciHeight;}
	
	/** Get Physical Depth of the Scintillator. **/
	const Double_t GetScintillatorDepth() { return  sciDepth;}
	
	/** Set Physical Length of the Scintillator. **/
	void SetScintillatorLength(Double_t length){sciLength=length;}
	
	/** Set Physical Height of the Scintillator. **/
	void SetScintillatorHeight(Double_t height){sciHeight=height;}
	
	/** Set Physical Depth of the Scintillator. **/
	void SetScintillatorDepth(Double_t depth){sciDepth=depth;}

  private:
  // Digi. Parameters
  Double_t sciLength; /**< Scintillator Length. **/
  Double_t sciHeight; /**< Scintillator Height. **/
  Double_t sciDepth; /**< Scintillator Depth. **/
  
   ClassDef(EnsarScintillatorDigiPar,2); //
};

#endif /* !TSCINTIDETDIGIPAR_H*/

