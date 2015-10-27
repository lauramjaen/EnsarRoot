/*
 * ********************************
 *     Interface: E105SlitPar     *
 * Author: M. Babai               *
 * Version:                       *
 * Licence:                       *
 * ********************************
 */
/*
 * Example task to read the parameter from a database using the
 * fairbase parameter handling interface.
 */
#pragma once
#ifndef E105ACCESSRTDBTASK_H
#define E105ACCESSRTDBTASK_H


#include "FairTask.h"
#include "E105SlitPar.h"

#include <string>
#include <vector>

class TClonesArray;

class E105AccessRTDBTask: public FairTask
{
 public:
  
  /** Default constructor **/
  E105AccessRTDBTask();
  
  /** Destructor **/
  virtual ~E105AccessRTDBTask();
    
  /** Virtual method Init **/
  virtual void SetParContainers();
  virtual InitStatus Init();
  virtual InitStatus ReInit();
  
  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);
  /** Virtual method Finish **/
  virtual void Finish();

 private:
  E105AccessRTDBTask (E105AccessRTDBTask const &ot);
  E105AccessRTDBTask operator= (E105AccessRTDBTask const &o);

  // Example parameter
  E105SlitPar* fEnsarSlitPar;
  
  ClassDef(E105AccessRTDBTask, 0);
};
#endif
