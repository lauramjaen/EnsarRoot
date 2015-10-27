/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0 / 2014
*    -----------------------------------------------------------------------
*/

// EnsarRoot includes
#include "EnsarSiliconContFact.h"
#include "EnsarSiliconDigiPar.h"
#include "EnsarSiliconGeoPar.h"

//FairRoot includes
#include "FairRuntimeDb.h"

//C++ includes
#include <iostream>

static EnsarSiliconContFact gEnsarSiliconContFact;

EnsarSiliconContFact::EnsarSiliconContFact()
  : FairContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="EnsarSiliconContFact";
  fTitle="Factory for parameter containers in libEnsarSilicon";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void EnsarSiliconContFact::setAllContainers()
{
  /** Creates the Container objects. 
   * Container Objects with all accepted 
   * contexts and adds them to the list of containers for the EnsarSilicon library.
  */

  FairContainer* p1= new FairContainer("EnsarSiliconDigiPar",
                                      "EnsarSilicon Digitization Parameters",
                                      "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");
  
  FairContainer* p2= new FairContainer("EnsarSiliconGeoPar",
                                      "EnsarSilicon Geometry Parameters",
                                      "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");
  
  containers->Add(p1);
  containers->Add(p2);
}

FairParSet* EnsarSiliconContFact::createContainer(FairContainer* c)
{
  
  /** Calls the constructor of the corresponding parameter container.
      For an actual context, which is not an empty string and not
      the default context of this container, the name is concatinated with the context.
  */

  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"EnsarSiliconDigiPar")==0) {
    p=new EnsarSiliconDigiPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  if (strcmp(name,"EnsarSiliconGeoPar")==0) {
    p=new EnsarSiliconDigiPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  
  return p;
}

ClassImp(EnsarSiliconContFact)