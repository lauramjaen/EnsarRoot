/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/

// Ensar includes
#include "EnsarScintillatorGeoPar.h"
#include "FairParamList.h"

// ROOT includes
#include "TObjArray.h"

// C++ includes
#include <iostream>

ClassImp(EnsarScintillatorGeoPar)

EnsarScintillatorGeoPar ::EnsarScintillatorGeoPar(const char* name,
                                      const char* title,
                                      const char* context)
  : FairParGenericSet(name,title,context),
    fGeoSensNodes(new TObjArray()),
    fGeoPassNodes(new TObjArray())
{
}

EnsarScintillatorGeoPar::~EnsarScintillatorGeoPar(void)
{
}

void EnsarScintillatorGeoPar::clear(void)
{
  if(fGeoSensNodes) { delete fGeoSensNodes; }
  if(fGeoPassNodes) { delete fGeoPassNodes; }
}

void EnsarScintillatorGeoPar::putParams(FairParamList* l)
{
  if (!l) { return; }
  l->addObject("FairGeoNodes Sensitive List", fGeoSensNodes);
  l->addObject("FairGeoNodes Passive List", fGeoPassNodes);
}

Bool_t EnsarScintillatorGeoPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  if (!l->fillObject("FairGeoNodes Sensitive List", fGeoSensNodes)) { return kFALSE; }
  if (!l->fillObject("FairGeoNodes Passive List", fGeoPassNodes)) { return kFALSE; }
  return kTRUE;
}
