#ifndef PNDDSSDDETECTORCONTFACT_H
#define PNDDSSDDETECTORCONTFACT_H

#include "FairContFact.h"

class FairContainer;

class E105DSSDContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    E105DSSDContFact();
    ~E105DSSDContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( E105DSSDContFact,0) // Factory for all E105DSSD parameter containers
};

#endif
