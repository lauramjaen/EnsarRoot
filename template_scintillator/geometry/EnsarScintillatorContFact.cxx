/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/

// Ensar includes
#include "EnsarScintillatorContFact.h"
#include "EnsarScintillatorDigiPar.h"
#include "EnsarScintillatorGeoPar.h"
#include "FairRuntimeDb.h"

// C++ includes
#include <iostream>

static EnsarScintillatorContFact gEnsarScintillatorContFact;

EnsarScintillatorContFact::EnsarScintillatorContFact()
  : FairContFact()
{
  /** Constructor (called when the library is loaded) */
  fName="EnsarScintillatorContFact";
  fTitle="Factory for parameter containers in libEnsarScintillator";
  setAllContainers();
  FairRuntimeDb::instance()->addContFactory(this);
}

void EnsarScintillatorContFact::setAllContainers()
{
  /** Creates the Container objects. 
   * Container Objects with all accepted 
   * contexts and adds them to the list of containers for the EnsarScintillator library.
  */

  FairContainer* p1= new FairContainer("EnsarScintillatorDigiPar",
                                      "EnsarScintillator Digitization Parameters",
                                      "TestDefaultContext");
  p1->addContext("TestNonDefaultContext");
  
  FairContainer* p2= new FairContainer("EnsarScintillatorGeoPar",
                                      "EnsarScintillator Geometry Parameters",
                                      "TestDefaultContext");
  p2->addContext("TestNonDefaultContext");
  
  containers->Add(p1);
  containers->Add(p2);
}

FairParSet* EnsarScintillatorContFact::createContainer(FairContainer* c)
{
  
  /** Calls the constructor of the corresponding parameter container.
      For an actual context, which is not an empty string and not
      the default context of this container, the name is concatinated with the context.
  */

  const char* name=c->GetName();
  FairParSet* p=NULL;
  if (strcmp(name,"EnsarScintillatorDigiPar")==0) {
    p=new EnsarScintillatorDigiPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  if (strcmp(name,"EnsarScintillatorGeoPar")==0) {
    p=new EnsarScintillatorDigiPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
  }
  
  return p;
}

ClassImp(EnsarScintillatorContFact)