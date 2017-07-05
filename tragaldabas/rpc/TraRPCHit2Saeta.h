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
    TClonesArray* fTraPointCA;

  private:

   /** Private method AddSaeta
    **
    ** Adds a RPCSaeta to the Collection
    **/
    TraRPCSaeta* AddSaeta(TMatrixD Saeta, Int_t id, Int_t size);

    // It calculates the chi-square of a saeta
    Double_t ChiSquare(TMatrixD, TMatrixD, TMatrixD, TMatrixD);

    // It creates a reduced data matrix
    TMatrixD CreateAMatrix(Double_t, Double_t, Double_t, Double_t);

    // It creates a data matrix
    TMatrixD CreateDataMatrix(Double_t, Double_t, Double_t);

    // It creates a K matrix
    TMatrixD CreateKMatrix(Double_t);

    // It constructs a S2 saeta and adds it to the array
    void CreateS2(Int_t, Double_t, Int_t, Double_t);

    // It constructs a S3 saeta
    void CreateS3(Int_t, Double_t , Int_t , Double_t, Int_t, Double_t);

    // It constructs a S3 saeta
    void CreateS4(Int_t, Double_t , Int_t , Double_t, Int_t, Double_t, Int_t, Double_t);

    // It writes error matrix as a function of sigma and rho
    TMatrixD Reduce(TMatrixD);

    // It calculates the drift of the signal
    TMatrixD Drift(TMatrixD, TMatrixD, TMatrixD, TMatrixD, TMatrixD);

    // It sets the recurrent coefficients during the drift calculation
    void SetCoefficients(Double_t, Double_t, Double_t, Double_t,  Double_t, Double_t, Double_t, Double_t*); 

    // It creates the K matrix with restrictions
    TMatrixD CreateKRestMatrix(Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t, Double_t );

    // It creates the A matrix with restrictions
    TMatrixD CreateARestMatrix(Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t,Double_t);

ClassDef(TraRPCHit2Saeta,1);
};

#endif
