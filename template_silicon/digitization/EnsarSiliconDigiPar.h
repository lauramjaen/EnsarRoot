/**  EnsarSiliconDigiPar.h **/

#ifndef TSILIDETDIGIPAR_H
#define TSILIDETDIGIPAR_H

#include <TVector3.h>
#include <TObjString.h>

#include "FairParGenericSet.h"
#include "FairParamList.h"

/// EnsarSiliconDigiPar.h

class EnsarSiliconDigiPar : public FairParGenericSet
{
    public :
    /**
     * \author  M.I.Cherciu
     * \version 1.0
     * \since 10.2014
     * \file EnsarSiliconDigiPar.h
     * \brief Class for digitization parameters of Silicon Detector.
     */	
      
       /** Default constructor **/
	EnsarSiliconDigiPar (const char* name="EnsarSiliconDigiPar",
			const char* title="Tutorial  parameter",
			const char* context="TestDefaultContext");
      /** Defaults Destructor **/
	~EnsarSiliconDigiPar(void){};

	/** Putting Digi parameters to the List. **/
	void putParams(FairParamList* list);
	
	/** Getting Digi parameters from the List. **/
	Bool_t getParams(FairParamList* list);
 
	/** Print information to the output. **/
	void Print(Option_t* option="") const;
	
	/** Get Physical Length of the Silicon. **/ 
	const Double_t GetSiliconLength() { return  silidetLength;}
	
	/** Get Physical Height of the Silicon. **/
	const Double_t GetSiliconHeight() { return  silidetHeight;}
	
	/** Get Physical Depth of the Silicon. **/
	const Double_t GetSiliconDepth() { return silidetDepth;}
	
	/** Set Physical Length of the Silicon. **/
	void SetSiliconLength(Double_t length){silidetLength=length;}
	
	/** Set Physical Height of the Silicon. **/
	void SetSiliconHeight(Double_t height){silidetHeight=height;}
	
	/** Set Physical Depth of the Silicon. **/
	void SetSiliconDepth(Double_t depth){silidetDepth=depth;}

  private:
  // Digi. Parameters
  Double_t silidetLength; /**< Silicon Length. **/
  Double_t silidetHeight; /**< Silicon Height. **/
  Double_t silidetDepth; /**< Silicon Depth. **/
  
   ClassDef(EnsarSiliconDigiPar,2); //
};

#endif /* !TSILIDETDIGIPAR_H*/

