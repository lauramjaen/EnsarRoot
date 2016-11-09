// ------------------------------------------------------------------------
// -----                TraRPCHit2Saeta  source file                  -----
// -----              Created 01/09/16  by P. Cabanelas               -----
// ------------------------------------------------------------------------

#include "TraRPCHit2Saeta.h"

#include "TMath.h"
#include "TVector3.h"
#include "TGeoMatrix.h"

#include "TClonesArray.h"
#include "TObjArray.h"
#include "TRandom.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "TGeoManager.h"

#include "TraRPCHit.h"

using std::cout;
using std::cerr;
using std::endl;


TraRPCHit2Saeta::TraRPCHit2Saeta() : FairTask("Tragaldabas Hit to Saeta task")
{

  fTraHitCA=0;     // TraRPCHit clones array
  fTraSaetaCA=0;   // TraRPCSaeta clones array

}


TraRPCHit2Saeta::~TraRPCHit2Saeta()
{

  LOG(INFO) << "TraRPCHit2Saeta: Delete instance" << FairLogger::endl;
  if(fTraHitCA) { fTraHitCA->Delete(); delete fTraHitCA; }
  if(fTraSaetaCA) { fTraSaetaCA->Delete(); delete fTraSaetaCA; }

}



void TraRPCHit2Saeta::SetParContainers()
{
}


// -----   Public method Init   --------------------------------------------
InitStatus TraRPCHit2Saeta::Init()
{

  FairRootManager* ioManager = FairRootManager::Instance();
  if ( !ioManager ) Fatal("Init", "No FairRootManager");
  fTraHitCA = (TClonesArray*) ioManager->GetObject("RPCHit");

  // Register output array TraRPCSaeta
  fTraSaetaCA = new TClonesArray("TraRPCSaeta",1000);
  ioManager->Register("RPCSaeta", "Tragaldabas Saeta", fTraSaetaCA, kTRUE);

  return kSUCCESS;

}



// -----   Public method ReInit   --------------------------------------------
InitStatus TraRPCHit2Saeta::ReInit()
{

  return kSUCCESS;

}


// -----   Public method Exec   --------------------------------------------
void TraRPCHit2Saeta::Exec(Option_t* opt)
{

  // Reset entries in output arrays, local arrays
  Reset();


  //ALGORITHMS FOR SAETA FINDING

  Double_t Xcoor=-999.;
  Double_t Ycoor=-999.;
  Double_t Xslope=-999.;
  Double_t Yslope=-999.;
  Double_t time=-1.;
  Double_t slowness=-1.;

  TraRPCHit** rpcHit;

  Int_t rpcHits;        // Nb of TraRPCHits in current event
  rpcHits = fTraHitCA->GetEntries();

  if (rpcHits>0) {
    rpcHit = new TraRPCHit*[rpcHits];
    for (Int_t i=0; i<rpcHits; i++) {
      rpcHit[i] = new TraRPCHit;
      rpcHit[i] = (TraRPCHit*) fTraHitCA->At(i);

      Xcoor = rpcHit[i]->GetCellXCenter(); //Example of accessing to RpcHit info

    }
  }

  AddSaeta(Xcoor,Xslope,Ycoor,Yslope,time,slowness);

  Double_t z=2.0;
  TMatrixD M(2,2);
  M = creaMatriz(z);
  M.Print();

}


// ---- Public method Reset   --------------------------------------------------
void TraRPCHit2Saeta::Reset()
{

  // Clear the CA structure
  if (fTraSaetaCA) fTraSaetaCA->Clear();

}


// ---- Public method Finish   --------------------------------------------------
void TraRPCHit2Saeta::Finish()
{
}


// -----   Private method AddSaeta  --------------------------------------------
TraRPCSaeta* TraRPCHit2Saeta::AddSaeta(Double_t Xcoor,Double_t Xslope,Double_t Ycoor,Double_t Yslope,Double_t time,Double_t slowness)
{

  // It fills the TraRPCSaeta array
  TClonesArray& clref = *fTraSaetaCA;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) TraRPCSaeta(Xcoor,Xslope,Ycoor,Yslope,time,slowness);

}

TMatrixD TraRPCHit2Saeta::creaMatriz(Double_t z) {

  TMatrixD A(2,2);

  A[0][0]=z;
  A[0][1]=z*z;
  A[1][0]=2*z;
  A[1][1]=z+2;

  return A;

}


ClassImp(TraRPCHit2Saeta)
