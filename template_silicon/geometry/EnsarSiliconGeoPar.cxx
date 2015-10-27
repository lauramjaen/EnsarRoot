/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0 / 2014
*    -----------------------------------------------------------------------
*/

// EnsarRoot includes
#include "EnsarSiliconGeoPar.h"

// FairRoot includes
#include "FairParamList.h"

// Root includes
#include "TObjArray.h"

//C++ includes
#include <iostream>

ClassImp(EnsarSiliconGeoPar)

EnsarSiliconGeoPar ::EnsarSiliconGeoPar(const char* name,
                                      const char* title,
                                      const char* context)
  : FairParGenericSet(name,title,context),
    fGeoSensNodes(new TObjArray()),
    fGeoPassNodes(new TObjArray())
{
}

EnsarSiliconGeoPar::~EnsarSiliconGeoPar(void)
{
}

void EnsarSiliconGeoPar::clear(void)
{
  if(fGeoSensNodes) { delete fGeoSensNodes; }
  if(fGeoPassNodes) { delete fGeoPassNodes; }
}

void EnsarSiliconGeoPar::putParams(FairParamList* l)
{
  if (!l) { return; }
  l->addObject("FairGeoNodes Sensitive List", fGeoSensNodes);
  l->addObject("FairGeoNodes Passive List", fGeoPassNodes);
}

Bool_t EnsarSiliconGeoPar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  if (!l->fillObject("FairGeoNodes Sensitive List", fGeoSensNodes)) { return kFALSE; }
  if (!l->fillObject("FairGeoNodes Passive List", fGeoPassNodes)) { return kFALSE; }
  return kTRUE;
}
