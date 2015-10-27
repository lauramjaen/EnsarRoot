/*
 * ********************************
 *     Interface: E105SlitPar     *
 * Author: M. Babai               *
 * Version:                       *
 * Licence:                       *
 * ********************************
 */
#include "E105ContFact.h"

#include <iostream>
#include <iomanip>

#include "FairRuntimeDb.h"
#include "FairParRootFileIo.h"
#include "FairParAsciiFileIo.h"

#include "E105SlitPar.h"

ClassImp(E105ContFact);

static E105ContFact gE105ContFact;

E105ContFact::E105ContFact()
{
  // Constructor (called when the library is loaded)
  fName  = "E105ContFact";
  fTitle = "E105 factory for parameter containers";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

E105ContFact::~E105ContFact()
{}

void E105ContFact::setAllContainers()
{
  /**
   * Creates the Container objects with all accepted contexts and adds
   * them to the list of containers.
   */
  
  FairContainer* p1 = new FairContainer("E105SlitPar",
                                        "E105 Slit Parameters",
                                        "TestDefaultContext_SLIT");
  p1->addContext("TestNonDefaultContext_SLIT");
  containers->Add(p1);
}

FairParSet* E105ContFact::createContainer(FairContainer* c)
{
  /**
   * Calls the constructor of the corresponding parameter container.
   * For an actual context, which is not an empty string and not the
   * default context of this container, the name is concatinated with
   * the context.
   */
  const char* name=c->GetName();
  std::cout << "\n\t-I-E105ContFact::createContainer "
            << name << std::endl;
  
  FairParSet* p = NULL;
  
  if (strcmp(name, "E105SlitPar") == 0) {
    std::cout << " <E105ContFact> Container was empty new is made\n\n";
    p = new E105SlitPar(c->getConcatName().Data(), c->GetTitle(), c->getContext());
    p->SetVersion(0);
  }
  return p;
}
