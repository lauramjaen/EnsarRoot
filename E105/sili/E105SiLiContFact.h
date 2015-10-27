#ifndef PNDSILIDETECTORCONTFACT_H
#define PNDSILIDETECTORCONTFACT_H

#include "FairContFact.h"

class FairContainer;

class E105SiLiContFact : public FairContFact
{
  private:
    void setAllContainers();
  public:
    E105SiLiContFact();
    ~E105SiLiContFact() {}
    FairParSet* createContainer(FairContainer*);
    ClassDef( E105SiLiContFact,0) // Factory for all E105SiLi parameter containers
};

#endif
