/*
 * ********************************
 *     Interface: E105SlitPar     *
 * Author: M. Babai               *
 * Version:                       *
 * Licence:                       *
 * ********************************
 */
/*
 * Example of fair container factory.
*/
#pragma once
#ifndef E105CONTFACT_H
#define E105CONTFACT_H

#include "FairContFact.h"

class E105ContFact : public FairContFact
{
 public:
  /**
   * Default constructor.
   */
  E105ContFact();
  /**
   * Destructor.
   */
  virtual ~E105ContFact();

  FairParSet* createContainer(FairContainer* cont);

 private:
  E105ContFact( E105ContFact const& ot);
  E105ContFact operator= (E105ContFact const &ot);

  void setAllContainers();
  
  ClassDef( E105ContFact, 0); // Factory for parameter containers
};
#endif  /* !E105CONTFACT_H */
