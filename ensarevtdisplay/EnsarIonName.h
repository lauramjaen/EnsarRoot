// -------------------------------------------------------------------------
// -----                   EnsarIonName header file                      -----
// -----              Created 21/09/2012  by P.Cabanelas               -----
// -------------------------------------------------------------------------

#ifndef ENSARIONNAME_H
#define ENSARIONNAME_H

#include "TNamed.h"

class EnsarIonName: public TNamed
{
  public:

    /** Default constructor **/
    EnsarIonName();


    /** Destructor **/
    virtual ~EnsarIonName();


    /** Accessors **/
    /**
    * Return the ion name if exists
    */
    Bool_t GetIonName(Int_t element, Char_t *name);

  private:

    /** Data members **/

    ClassDef(EnsarIonName,1);

};

#endif
