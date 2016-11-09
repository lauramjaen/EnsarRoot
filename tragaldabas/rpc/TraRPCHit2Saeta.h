// -------------------------------------------------------------------------
// -----                   TraRPCHit2Saeta source file                -----
// -----                  Created 01/09/16  by P. Cabanelas           -----
// -------------------------------------------------------------------------

/**  TraRPCHit2Saeta.h
 **/


#ifndef TRARPCHIT2SAETA_H
#define TRARPCHIT2SAETA_H

#include "FairTask.h"

#include "TraRPCSaeta.h"
#include "TMatrixD.h"

class TClonesArray;

class TraRPCHit2Saeta : public FairTask
{

  public:

    /** Default constructor **/
    TraRPCHit2Saeta();

    /** Destructor **/
    ~TraRPCHit2Saeta();

    /** Virtual method Exec **/
    virtual void Exec(Option_t* opt);

    /** Virtual method Reset **/
    virtual void Reset();


  protected:


    /** Virtual method Init **/
    virtual InitStatus Init();

    /** Virtual method ReInit **/
    virtual InitStatus ReInit();

    /** Virtual method SetParContainers **/
    virtual void SetParContainers();

    /** Virtual method Finish **/
    virtual void Finish();


    TClonesArray* fTraHitCA;
    TClonesArray* fTraSaetaCA;


  private:

   /** Private method AddSaeta
    **
    ** Adds a RPCSaeta to the Collection
    **/
    TraRPCSaeta* AddSaeta(Double_t Xcoor,Double_t Xslope,Double_t Ycoor,Double_t Yslope,Double_t time,Double_t slowness);

    TMatrixD creaMatriz(Double_t z);

    ClassDef(TraRPCHit2Saeta,1);

};


#endif
