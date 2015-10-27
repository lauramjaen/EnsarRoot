#include "E105SiLiContFact.h"
#include "E105SiLiGeoPar.h"
#include "E105SiLiConstPar.h"
#include "FairRuntimeDb.h"

#include <iostream>

ClassImp(E105SiLiContFact)

static E105SiLiContFact gE105SiLiContFact;

E105SiLiContFact::E105SiLiContFact()
  : FairContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="E105SiLiContFact";
  fTitle="Factory for parameter containers in libE105SiLi";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void E105SiLiContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted
      contexts and adds them to
      the list of containers for the E105SiLi library.
  */

  FairContainer* p= new FairContainer("E105SiLiGeoPar",
                                      "E105SiLi Geometry Parameters",
                                      "TestDefaultContext");
  p->addContext("TestNonDefaultContext");

  FairContainer* p1= new FairContainer("FairConstPar",
                                       "Constant Field Parameters",
                                       "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");



  containers->Add(p);
  containers->Add(p1);
}

FairParSet* E105SiLiContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
      For an actual context, which is not an empty string and not
      the default context
      of this container, the name is concatinated with the context.
  */
  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"E105SiLiGeoPar")==0) {
    p=new E105SiLiGeoPar(c->getConcatName().Data(),
                                 c->GetTitle(),c->getContext());
  }
  if(strcmp(name,"FairConstPar")==0) {
    p=new E105SiLiConstPar(c->getConcatName().Data(),
                       c->GetTitle(),c->getContext());
  }

  return p;
}
