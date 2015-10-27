#include "E105DSSDContFact.h"
#include "E105DSSDGeoPar.h"
#include "E105DSSDConstPar.h"
#include "FairRuntimeDb.h"

#include <iostream>

ClassImp(E105DSSDContFact)

static E105DSSDContFact gE105DSSDContFact;

E105DSSDContFact::E105DSSDContFact()
  : FairContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="E105DSSDContFact";
  fTitle="Factory for parameter containers in libE105DSSD";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void E105DSSDContFact::setAllContainers()
{
  /** Creates the Container objects with all accepted
      contexts and adds them to
      the list of containers for the E105DSSD library.
  */

  FairContainer* p= new FairContainer("E105DSSDGeoPar",
                                      "E105DSSD Geometry Parameters",
                                      "TestDefaultContext");
  p->addContext("TestNonDefaultContext");

  FairContainer* p1= new FairContainer("E105DSSDConstPar",
                                       "Constant Field Parameters",
                                       "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");



  containers->Add(p);
  containers->Add(p1);
}

FairParSet* E105DSSDContFact::createContainer(FairContainer* c)
{
  /** Calls the constructor of the corresponding parameter container.
      For an actual context, which is not an empty string and not
      the default context
      of this container, the name is concatinated with the context.
  */
  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"E105DSSDGeoPar")==0) {
    p=new E105DSSDGeoPar(c->getConcatName().Data(),
                                 c->GetTitle(),c->getContext());
  }
  if(strcmp(name,"E105DSSDConstPar")==0) {
    p=new E105DSSDConstPar(c->getConcatName().Data(),
                       c->GetTitle(),c->getContext());
  }

  return p;
}
