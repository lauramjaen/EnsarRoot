//*-- AUTHOR : Ilse Koenig
//*-- Created : 10/11/2003

/////////////////////////////////////////////////////////////
// E105GeoShield
//
// Class for geometry of support structure
//
/////////////////////////////////////////////////////////////

#include "E105GeoShield.h"
#include "FairGeoLoader.h"
#include "FairGeoInterface.h"

ClassImp(E105GeoShield)

E105GeoShield::E105GeoShield()
  : FairGeoSet()
{
  // Constructor
  fName="pipe";
  strcpy(modName,"p");
  strcpy(eleName,"p");
  maxSectors=0;
  maxModules=1;
}
Bool_t  E105GeoShield::create ( FairGeoBuilder* build )
{
  Bool_t rc = FairGeoSet::create( build );
  if ( rc ) {
    FairGeoLoader* loader=FairGeoLoader::Instance();
    FairGeoInterface* GeoInterface =loader->getGeoInterface();

    GeoInterface->getMasterNodes()->Add( (TObject*)  getVolume("pipeCentral") );
  }
  return rc;
}

E105GeoShield::~E105GeoShield()
{

}

