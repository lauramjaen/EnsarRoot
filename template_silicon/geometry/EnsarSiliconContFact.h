/** EnsarSiliconContFact.h **/


#ifndef TSILIDETCONTFACT_H
#define TSILIDETCONTFACT_H

#include "FairContFact.h"


class FairContainer;

///  EnsarSiliconContFact.h

class EnsarSiliconContFact : public FairContFact
{
  public:
    /** EnsarSiliconContFact class
    * \author  M.I.Cherciu
    * \version 1.0
    * \since 10.2014
    * \file EnsarSiliconContFact.h
    * \brief Class of factory parameter container of libEnsarSilicon.
    * 
    */
    
    /** Default Contructor. */ 
    EnsarSiliconContFact();

    /** Default Destructor. */
    ~EnsarSiliconContFact() {};
    
    /** Create object FairParSet.
      *      \param FairContainer :  Container Name - {a base class of all factories for the parameter containers.}
      */
    FairParSet* createContainer(FairContainer*);

private:
    /** Registration for all parametric containers for libEnsarSilicon. **/
    void setAllContainers();
    
    ClassDef( EnsarSiliconContFact,0) // Factory for all EnsarSilicon parameter containers
};

#endif
