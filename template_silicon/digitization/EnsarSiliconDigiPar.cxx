/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0 / 2014
*    ------------------------------------------------------------------------
*/

// EnsarRoot includes
#include "EnsarSiliconDigiPar.h"

// Root includes
#include "TMath.h"

// C++ includes
#include <iostream>

EnsarSiliconDigiPar::EnsarSiliconDigiPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name,title,context)
{
  clear();
}

void EnsarSiliconDigiPar::putParams(FairParamList* list)
{
  std::cout<<"-I- EnsarSiliconDigiPar::putParams() called"<<std::endl;
  if(!list) return;
  list->add("silidet_length", (Double_t)silidetLength);
  list->add("silidet_height", (Double_t)silidetHeight);
  list->add("silidet_depth", (Double_t)silidetDepth);
}

Bool_t EnsarSiliconDigiPar::getParams(FairParamList* list)
{
  std::cout<<"-I- EnsarSiliconDigiPar::getParams() called"<<std::endl;
  if (!list) return kFALSE;
  std::cout<<"-I- EnsarSiliconDigiPar::getParams() 1 ";
  
  if (!list->fill("silidet_length", &silidetLength)) return kFALSE;
  if (!list->fill("silidet_heigth", &silidetHeight)) return kFALSE;
  if (!list->fill("silidet_depth", &silidetDepth)) return kFALSE;
  return kTRUE;
}


void EnsarSiliconDigiPar::Print(Option_t* option) const
{
    std::cout<<"-I- Land Digi Parameters:"<<std::endl;
    std::cout<<"   Silicon Length   = "<<silidetLength<<std::endl;
    std::cout<<"   Silicon Heigth   = "<<silidetHeight<<std::endl;
    std::cout<<"   Silicon Depth    = "<<silidetDepth<<std::endl;
}

ClassImp(EnsarSiliconDigiPar);
