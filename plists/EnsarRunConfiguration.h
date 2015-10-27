#ifndef ENSARRUNCONFIGURATION_H
#define ENSARRUNCONFIGURATION_H


#include "TG4RunConfiguration.h"


class EnsarRunConfiguration : public TG4RunConfiguration
{
  public:
    EnsarRunConfiguration(const TString& userGeometry,
                          const TString& specialProcess = "stepLimiter" );
    virtual ~EnsarRunConfiguration();

    // methods
    virtual G4VUserPhysicsList*  CreatePhysicsList();


};

#endif //ENSARRUNCONFIGURATION_H

