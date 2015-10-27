//*-- AUTHOR : Denis Bertini
//*-- Created : 21/06/2005

using namespace std;
#include "E105GeoPassivePar.h"
#include "FairParamList.h"
#include <iostream>
#include <iomanip>
#include "TObjArray.h"

ClassImp(E105GeoPassivePar)

E105GeoPassivePar::E105GeoPassivePar(const char* name,const char* title,const char* context)
  : FairParGenericSet(name,title,context),
    fGeoSensNodes(new TObjArray()),
    fGeoPassNodes(new TObjArray())
{
}

E105GeoPassivePar::~E105GeoPassivePar(void)
{
}

void E105GeoPassivePar::clear(void)
{
  if(fGeoSensNodes) { delete fGeoSensNodes; }
  if(fGeoPassNodes) { delete fGeoPassNodes; }
}

void E105GeoPassivePar::putParams(FairParamList* l)
{
  if (!l) { return; }
  l->addObject("E105GeoNodes Sensitive List", fGeoSensNodes);
  l->addObject("E105GeoNodes Passive List", fGeoPassNodes);
}

Bool_t E105GeoPassivePar::getParams(FairParamList* l)
{
  if (!l) { return kFALSE; }
  if (!l->fillObject("E105GeoNodes Sensitive List", fGeoSensNodes)) { return kFALSE; }
  if (!l->fillObject("E105GeoNodes Passive List", fGeoPassNodes)) { return kFALSE; }

  return kTRUE;
}
