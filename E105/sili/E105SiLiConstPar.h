 /** E105SiLiConstPar.h
 ** @author M.Al-Turany
 ** @since 30.01.2007
 ** @version 1.0
 **
 ** Parameter set for the region between Solenoid and dipole. For the runtime database.
 **/


#ifndef E105SiLiConstPAR_H
#define E105SiLiConstPAR_H 1
#include "E105SiLiMapPar.h"
class FairParamList;

class E105SiLiConstPar : public E105SiLiMapPar
{

  public:


    /** Standard constructor  **/
    E105SiLiConstPar(const char* name, const char* title, const char* context);

    /** default constructor  **/
    E105SiLiConstPar();

    /** Destructor **/
    ~E105SiLiConstPar();

    void putParams(FairParamList* list);


    /** Get parameters **/
    Bool_t getParams(FairParamList* list);


    /** Set parameters from FairField  **/
    //    void SetParameters(FairField* field);


    Double_t GetBx()        const { return fBx; }
    Double_t GetBy()        const { return fBy; }
    Double_t GetBz()        const { return fBz; }

  protected:

    /** Field values in [kG] **/
    Double_t fBx, fBy, fBz;

    ClassDef(E105SiLiConstPar,1);

};


#endif
