/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/

// Ensar includes
#include "EnsarScintillatorDigiPar.h"

// ROOT includes
#include "TMath.h"

// C++ includes
#include <iostream>

EnsarScintillatorDigiPar::EnsarScintillatorDigiPar(const char* name, const char* title, const char* context)
  : FairParGenericSet(name,title,context)
{
  clear();
}

void EnsarScintillatorDigiPar::putParams(FairParamList* list)
{
  std::cout<<"-I- EnsarScintillatorDigiPar::putParams() called"<<std::endl;
  if(!list) return;
  list->add("sci_length", (Double_t)sciLength);
  list->add("sci_height", (Double_t)sciHeight);
  list->add("sci_depth", (Double_t)sciDepth);
}

Bool_t EnsarScintillatorDigiPar::getParams(FairParamList* list)
{
  std::cout<<"-I- EnsarScintillatorDigiPar::getParams() called"<<std::endl;
  if (!list) return kFALSE;
  std::cout<<"-I- EnsarScintillatorDigiPar::getParams() 1 ";
  
  if (!list->fill("sci_length", &sciLength)) return kFALSE;
  if (!list->fill("sci_heigth", &sciHeight)) return kFALSE;
  if (!list->fill("sci_depth", &sciDepth)) return kFALSE;
  return kTRUE;
}


void EnsarScintillatorDigiPar::Print(Option_t* option) const
{
    std::cout<<"-I- Land Digi Parameters:"<<std::endl;
    std::cout<<"   Scintillator Length   = "<<sciLength<<std::endl;
    std::cout<<"   Scintillator Heigth   = "<<sciHeight<<std::endl;
    std::cout<<"   Scintillator Depth    = "<<sciDepth<<std::endl;
}

ClassImp(EnsarScintillatorDigiPar);
