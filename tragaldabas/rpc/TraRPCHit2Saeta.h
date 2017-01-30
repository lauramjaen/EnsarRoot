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
// los points
    TClonesArray* fTraPointCA;

  private:

   /** Private method AddSaeta
    **
    ** Adds a RPCSaeta to the Collection
    **/
    TraRPCSaeta* AddSaeta(Double_t Xcoor,Double_t Xslope,Double_t Ycoor,Double_t Yslope,Double_t time,Double_t slowness);


// voy a añadir el prototipo de la funcion creadora de matrices
TMatrixD CreateKMatrix(Double_t);
TMatrixD CreateAMatrix(Double_t, Double_t, Double_t, Double_t);
void SetCoefficients(Double_t, Double_t, Double_t, Double_t,  Double_t, Double_t, Double_t, Double_t*); 
TMatrixD CreateKRestMatrix(Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t );
TMatrixD CreateARestMatrix(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);
Double_t Mahalanobis(Double_t, Double_t, Double_t, Double_t, Double_t);
TMatrixD Reduce(TMatrixD);
ClassDef(TraRPCHit2Saeta,1);
};

#endif
