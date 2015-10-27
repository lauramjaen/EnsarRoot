/** EnsarScintillatorContFact.h **/


#ifndef TSCINTIDETCONTFACT_H
#define TSCINTIDETCONTFACT_H

#include "FairContFact.h"


class FairContainer;

///  EnsarScintillatorContFact.h
class EnsarScintillatorContFact : public FairContFact
{
  public:
   /**
     * \author  M.I.Cherciu \n
     * Modified by: \n   
     * Comment: \n
     * \version 1.0 \n
     * \since 2014/07/02 \n
     * Change date: \n
     * \file EnsarScintillatorContFact.h
     * \brief Class of factory parameter container of libEnsarScintillator.
    */
    
    /** Default Contructor. */ 
    EnsarScintillatorContFact();

    /** Default Destructor. */
    ~EnsarScintillatorContFact() {};
    
    /** Create object FairParSet.
      *      \param FairContainer :  Container Name - {a base class of all factories for the parameter containers.}
      */
    FairParSet* createContainer(FairContainer*);

private:
    /** Registration for all Containers for libEnsarScintillator. **/
    void setAllContainers();
    
    ClassDef( EnsarScintillatorContFact,0) // Factory for all EnsarScintillator parameter containers
};

#endif
