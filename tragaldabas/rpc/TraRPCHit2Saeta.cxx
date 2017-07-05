// ------------------------------------------------------------------------
// -----                TraRPCHit2Saeta  source file                  -----
// -----              Created 01/09/16  by P. Cabanelas               -----
// -----              Last modified 15/02/2017  by J. Cuenca          -----
// ------------------------------------------------------------------------
// TO DO: This method works for Multiplicity = 1 events. S3 and S4 methods
// should be implemented in order to access the hits that form the saeta.
// A better tracking strategy can therefore be used.

#include "TraRPCHit2Saeta.h"

#include "TMath.h"
#include "TGeoMatrix.h"
#include "TMatrixD.h"

#include "TClonesArray.h"
#include "TObjArray.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "TGeoManager.h"

#include "TraRPCHit.h"
#include "TraRPCPoint.h"
#include "TRandom3.h"

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
}

  // -----   Public method ReInit   --------------------------------------------
  InitStatus TraRPCHit2Saeta::ReInit()
{
  return kSUCCESS;
}

  // -----   Public method Exec   --------------------------------------------
  // Global variables: position of the planes, 1/sigma's. 

  Double_t const length = 11.1;
  Double_t const width = 11.6;
  Double_t const sigmaT = 0.05;
  Double_t const c = 30.0; // speed of light in cm/ns
  Double_t const percent = 0.6; // signal velocity as a percentage of c (i.e. 0.6 == 60%)  
  Double_t wx=12.0/(width*width), wy=12.0/(length*length), wt=1.0/(sigmaT*sigmaT), sv=1.0/(percent*c); // sv: signal slowness
  Double_t Z1=0.0, Z2=52.2, Z3=90.2, Z4=173.9;

  void TraRPCHit2Saeta::Exec(Option_t* opt)
{

  // Reset entries in output arrays, local arrays
  Reset();

  // Create and fill the Hits Clones array
  TraRPCHit** rpcHit;
  Int_t rpcHits;        // Nb of TraRPCHits in current event
  rpcHits = fTraHitCA->GetEntries();
  if (rpcHits>0) {
  rpcHit = new TraRPCHit*[rpcHits];
   for (Int_t i=0; i<rpcHits; i++) {
   rpcHit[i] = (TraRPCHit*) fTraHitCA->At(i);
   }
  }

  // ---- Create and add the desired saetas -----
  //  Comment SN.Print() in methods to avoid individual saeta information on screen. 
  //  CreateS2(1,Z1,2,Z2);
  //  CreateS2(2,0.0,3,Z3-Z2);
  //  CreateS2(3,0.0,4,Z4-Z3);
  //  CreateS3(1,Z1,2,Z2,3,Z3);
  CreateS4(1,Z1,2,Z2,3,Z3,4,Z4);
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
  TraRPCSaeta* TraRPCHit2Saeta::AddSaeta(TMatrixD Saeta, Int_t id, Int_t size)
{
  // It fills the TraRPCSaeta array
  TClonesArray& clref = *fTraSaetaCA;
  Int_t ArraySize = clref.GetEntriesFast();
  return new(clref[ArraySize]) TraRPCSaeta(Saeta, id, size);
}

  
  // -----  Private method ChiSquare ---------------------------------------------
  Double_t TraRPCHit2Saeta::ChiSquare(TMatrixD K, TMatrixD Dat, TMatrixD A, TMatrixD S)
{
  TMatrixD Prod(1,1);
  Double_t chi = 0.0;
  // Data transpose
  TMatrixD DatTrans(1,3);
  for(Int_t i=0; i<3;i++){
   DatTrans[0][i] = Dat[i][0];
  }

  // A transpose
  TMatrixD ATrans(1,6);
  for(Int_t i=0; i<6;i++){
   ATrans[0][i] = A[i][0];
  }

  // S transpose
  TMatrixD STrans(1,6);
  for(Int_t i=0; i<6;i++){
   STrans[0][i] = S[i][0];
  }

  // W matrix
  TMatrixD W(3,3);
  W[0][0] = wx;
  W[1][1] = wy;
  W[2][2] = wt;
  
  // Calculate the chi-value
  Prod = STrans*K*S-2.0*STrans*A+(DatTrans*W)*Dat;
  chi = Prod[0][0]; 
  return chi;
}

  // -----  Private method CreateAMatrix ---------------------------------------
  TMatrixD TraRPCHit2Saeta::CreateAMatrix(double X, double Y, double Z, double T)
{
  TMatrixD A(6,1);
  A[0][0]=X*wx;
  A[1][0]=X*Z*wx;
  A[2][0]=Y*wy;
  A[3][0]=Y*Z*wy;
  A[4][0]=T*wt;
  A[5][0]=T*Z*wt;
  return A;
}

  // ----- Private method CreateARestMatrix ---------------------------------------
  // This function sets the A matrices with constraints. 4 coefficients, 5 saeta-coordinates, 4 hit coordinates (x,y,z,t)
  TMatrixD TraRPCHit2Saeta::CreateARestMatrix(Double_t b1,Double_t b2,Double_t b3,Double_t b4,
  Double_t Sx,Double_t Sxp, Double_t Sy, Double_t Syp, Double_t Ss, 
  Double_t X, Double_t Y, Double_t Z, Double_t T)
{
  TMatrixD AR(6,1);
  AR[0][0] = wx*X+sv*wt*b3/b2*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Z*(Ss*Sxp*Sxp/b1+sv*Sxp*b3/b2)+Z*(Ss*Syp*Syp/b1+sv*Syp*b4/b2));
  AR[1][0] = wx*X*Z+wt*Z*(Ss*Sxp/b1+sv*b3/b2)*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Z*(Ss*Sxp*Sxp/b1+sv*Sxp*b3/b2)+Z*(Ss*Syp*Syp/b1+sv*Syp*b4/b2));
  AR[2][0] = wy*Y+sv*wt*b4/b2*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Z*(Ss*Sxp*Sxp/b1+sv*Sxp*b3/b2)+Z*(Ss*Syp*Syp/b1+sv*Syp*b4/b2));
  AR[3][0] = wy*Y*Z+wt*(Ss*Syp*Z/b1+sv*Z*b4/b2)*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Sxp*(Ss*Sxp*Z/b1+sv*Z*b3/b2)+Syp*(Ss*Syp*Z/b1+sv*Z*b4/b2));
  AR[4][0] = wt*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Sxp*(Ss*Sxp*Z/b1+sv*Z*b3/b2)+Syp*(Ss*Syp*Z/b1+sv*Z*b4/b2));
  AR[5][0] = AR[4][0]*b1*Z;
  return AR;
}

  // -----  Private method CreateDataMatrix -----------------------------------
  TMatrixD TraRPCHit2Saeta::CreateDataMatrix(Double_t x, Double_t y, Double_t t)
{
  TMatrixD D(3,1);
  D[0][0] = x;
  D[1][0] = y;
  D[2][0] = t;
  return D;
}

  // -----  Private method CreateKMatrix --------------------------------------
  TMatrixD TraRPCHit2Saeta::CreateKMatrix(double Z)
{
  TMatrixD K(6,6);
  K[0][0] = wx;
  K[0][1] = wx*Z;
  K[1][0] = K[0][1];
  K[1][1] = wx*Z*Z;
  K[2][2] = wy;
  K[2][3] = wy*Z;
  K[3][2] = K[2][3];
  K[3][3] = wy*Z*Z;
  K[4][4] = wt;
  K[4][5] = wt*Z;
  K[5][4] = K[4][5];
  K[5][5] = wt*Z*Z;
  return K;
}


  // -----  Private method CreateKRestMatrix ----------------------------------------
  // This function sets the K matrices with signal drift constraint.
  TMatrixD TraRPCHit2Saeta::CreateKRestMatrix(Double_t b1,Double_t b2,Double_t b3,Double_t b4,Double_t Sxp,Double_t Syp,Double_t Ss,Double_t Z)
{
  TMatrixD KR(6,6);
  KR[0][0] = wx+sv*sv*wt*b3*b3/(b2*b2);
  KR[0][1] = wx*Z+b3*sv*wt*Z*(Ss*Sxp/b1+sv*b3/b2)/b2;
  KR[0][2] = sv*sv*wt*b3*b4/(b2*b2);
  KR[0][3] = sv*wt*Z*(Ss*Syp/b1+sv*b4/b2)*b3/b2;
  KR[0][4] = sv*wt*b3/b2;
  KR[0][5] = sv*wt*Z*b3*b1/b2;
  KR[1][0] = KR[0][1];
  KR[1][1] = wx*Z*Z+wt*Z*Z*(Ss*Sxp/b1+sv*b3/b2)*(Ss*Sxp/b1+sv*b3/b2);
  KR[1][2] = sv*wt*Z*(Ss*Sxp/b1+sv*b3/b2)*b4/b2;
  KR[1][3] = wt*Z*Z*(Ss*Sxp/b1+sv*b3/b2)*(Ss*Syp/b1+sv*b4/b2);
  KR[1][4] = wt*Z*(Ss*Sxp/b1+sv*b3/b2);
  KR[1][5] = wt*Z*Z*(Ss*Sxp/b1+sv*b3/b2)*b1;
  KR[2][0] = KR[0][2];
  KR[2][1] = KR[1][2];
  KR[2][2] = wy+sv*sv*wt*b4*b4/(b2*b2);
  KR[2][3] = wy*Z+sv*wt*b4*Z*(Ss*Syp/b1+sv*b4/b2)/b2;
  KR[2][4] = sv*wt*b4/b2;
  KR[2][5] = sv*wt*Z*b4*b1/b2;
  KR[3][0] = KR[0][3];
  KR[3][1] = KR[1][3];
  KR[3][2] = KR[2][3];
  KR[3][3] = wy*Z*Z+wt*Z*Z*(Ss*Syp/b1+sv*b4/b2)*(Ss*Syp/b1+sv*b4/b2);
  KR[3][4] = wt*Z*(Ss*Syp/b1+sv*b4/b2);
  KR[3][5] = wt*Z*Z*(Ss*Syp/b1+sv*b4/b2)*b1;
  KR[4][0] = KR[0][4];
  KR[4][1] = KR[1][4];
  KR[4][2] = KR[2][4];
  KR[4][3] = KR[3][4];
  KR[4][4] = wt;
  KR[4][5] = wt*Z*b1;
  KR[5][0] = KR[0][5];
  KR[5][1] = KR[1][5];
  KR[5][2] = KR[2][5];
  KR[5][3] = KR[3][5];
  KR[5][4] = KR[4][5];
  KR[5][5] = wt*b1*b1*Z*Z;
 return KR;
}


  // -----  Private method CreateS2 -------------------------------------------
  // It builds saetas between two generic planes denoted by id1 and id2 indices

  void TraRPCHit2Saeta::CreateS2(Int_t id1, Double_t z1, Int_t id2, Double_t z2)
{
  Double_t x1=0.0, y1=0.0, time1=0.0, x2=0.0, y2=0.0, time2=0.0, chi1=0.0, chi2=0.0;
  TMatrixD KMatrix1(6,6), KMatrix2(6,6), DataMatrix1(3,1), DataMatrix2(3,1);
  KMatrix1 = CreateKMatrix(z1);
  KMatrix2 = CreateKMatrix(z2);
  TMatrixD A1(6,1), A2(6,1), S2(6,1);
  Int_t nhits =  (fTraHitCA)->GetEntries();
  for(Int_t p1=0;p1<nhits;p1++){
   if(((TraRPCHit*)(fTraHitCA->At(p1)))->GetPlane()  == id1){
   x1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetCellXCenter();
   y1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetCellYCenter();
   time1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetTime();
   A1 = CreateAMatrix(x1,y1,z1,time1);
   DataMatrix1 = CreateDataMatrix(x1,y1,time1);
    for(Int_t p2=0;p2<nhits;p2++){
     if(((TraRPCHit*)(fTraHitCA->At(p2)))->GetPlane()  == id2){
     x2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetCellXCenter();
     y2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetCellYCenter();
     time2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetTime();
     A2 = CreateAMatrix(x2,y2,z2,time2);
     DataMatrix2 = CreateDataMatrix(x2,y2,time2);
     S2=(KMatrix1+KMatrix2).Invert()*(A1+A2);
     // If the speed of the saeta is greater than c we discard the S2
      if(S2[5][0]<1.0/30.0-1.0/wt) continue;
      S2.Print();
     // Add the saeta to the array and set the indices
      TraRPCSaeta* Saeta;
      Saeta = AddSaeta(S2,id1, 2); // size = 2
      Saeta->SetHitsIndices(p1,p2,-1,-1);
     }
    }
   }
  }
}

  // ----- TO BE FINISHED & CORRECTED  Private method CreateS3 ------------------------------------------------------------
  // It builds size 3 saetas using the hits produced at three generic planes denoted by id1,id2, and id3 indices
  
  void TraRPCHit2Saeta::CreateS3(Int_t id1, Double_t z1, Int_t id2, Double_t z2, Int_t id3, Double_t z3)
{ 
  // Define the matrices and data
  TMatrixD K1(6,6), K2(6,6), K3(6,6), DataMatrix1(3,1), DataMatrix2(3,1),DataMatrix3(3,1);
  TMatrixD A1(6,1), A2(6,1), A3(6,1), S3(6,1);
  Double_t x1=0.0, y1=0.0, time1=0.0, x2=0.0, y2=0.0, time2=0.0, x3=0.0, y3=0.0, time3=0.0;
  K1 = CreateKMatrix(z1);
  K2 = CreateKMatrix(z2);
  K3 = CreateKMatrix(z3);

  Int_t nhits =  (fTraHitCA)->GetEntries();
  for(Int_t p1=0;p1<nhits;p1++){       // loop over all hits at plane #1
   if(((TraRPCHit*)(fTraHitCA->At(p1)))->GetPlane()  == id1){
   x1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetCellXCenter();
   y1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetCellYCenter();
   time1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetTime();
   A1 = CreateAMatrix(x1,y1,z1,time1);
   DataMatrix1 = CreateDataMatrix(x1,y1,time1);
    for(Int_t p2=0;p2<nhits;p2++){    // loop over all hits at plane #2
     if(((TraRPCHit*)(fTraHitCA->At(p2)))->GetPlane()  == id2){
     x2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetCellXCenter();
     y2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetCellYCenter();
     time2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetTime();
     A2 = CreateAMatrix(x2,y2,z2,time2);
     DataMatrix2 = CreateDataMatrix(x2,y2,time2);
      for(Int_t p3=0;p3<nhits;p3++){  // loop over all hits at plane #3
       if(((TraRPCHit*)(fTraHitCA->At(p3)))->GetPlane()  == id3){
       x3 = ((TraRPCHit*)(fTraHitCA->At(p3)))->GetCellXCenter();
       y3 = ((TraRPCHit*)(fTraHitCA->At(p3)))->GetCellYCenter();
       time3 = ((TraRPCHit*)(fTraHitCA->At(p3)))->GetTime();
       A3 = CreateAMatrix(x3,y3,z3,time3);
       DataMatrix3 = CreateDataMatrix(x3,y3,time3);
       // we calculate the saeta
       S3=(K1+K2+K3).Invert()*(A1+A2+A3);
       S3.Print();
       // we add it to the array
       TraRPCSaeta* Saeta;
       Saeta = AddSaeta(S3,id1, 3); // size = 3 
       Saeta->SetHitsIndices(p1,p2,p3,-1);
       }
      }
     }
    }
   }
  }
}

  // -----  Private method CreateS4 ------------------------------------------------------------------
  // It builds size 4 saetas using the hits produced at four generic planes denoted by id1, id2, id3, and id4 indices

  void TraRPCHit2Saeta::CreateS4(Int_t id1, Double_t z1, Int_t id2, Double_t z2, Int_t id3, Double_t z3, Int_t id4, Double_t z4)
{
  // Define the matrices and data
  TMatrixD K1(6,6), K2(6,6), K3(6,6), K4(6,6), DataMatrix1(3,1), DataMatrix2(3,1),DataMatrix3(3,1), DataMatrix4(3,1);
  TMatrixD A1(6,1), A2(6,1), A3(6,1), A4(6,1), S4(6,1);
  Double_t x1=0.0, y1=0.0, time1=0.0, x2=0.0, y2=0.0, time2=0.0, x3=0.0, y3=0.0, time3=0.0, x4=0.0, y4=0.0, time4=0.0;
  K1 = CreateKMatrix(z1);
  K2 = CreateKMatrix(z2);
  K3 = CreateKMatrix(z3);
  K4 = CreateKMatrix(z4);

  Int_t nhits =  (fTraHitCA)->GetEntries();
  for(Int_t p1=0;p1<nhits;p1++){      // loop over all the hits at plane #1
   if(((TraRPCHit*)(fTraHitCA->At(p1)))->GetPlane()  == id1){
   x1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetCellXCenter();
   y1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetCellYCenter();
   time1 = ((TraRPCHit*)(fTraHitCA->At(p1)))->GetTime();
   A1 = CreateAMatrix(x1,y1,z1,time1);
   DataMatrix1 = CreateDataMatrix(x1,y1,time1);
    for(Int_t p2=0;p2<nhits;p2++){    // loop over all the hits at plane #2
     if(((TraRPCHit*)(fTraHitCA->At(p2)))->GetPlane()  == id2){
     x2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetCellXCenter();
     y2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetCellYCenter();
     time2 = ((TraRPCHit*)(fTraHitCA->At(p2)))->GetTime();
     A2 = CreateAMatrix(x2,y2,z2,time2);
     DataMatrix2 = CreateDataMatrix(x2,y2,time2);
      for(Int_t p3=0;p3<nhits;p3++){  // loop over all the hits at plane #3
       if(((TraRPCHit*)(fTraHitCA->At(p3)))->GetPlane()  == id3){
       x3 = ((TraRPCHit*)(fTraHitCA->At(p3)))->GetCellXCenter();
       y3 = ((TraRPCHit*)(fTraHitCA->At(p3)))->GetCellYCenter();
       time3 = ((TraRPCHit*)(fTraHitCA->At(p3)))->GetTime();
       A3 = CreateAMatrix(x3,y3,z3,time3);
       DataMatrix3 = CreateDataMatrix(x3,y3,time3);
        for(Int_t p4=0;p4<nhits;p4++){ // loop over all the hits at plane #4
         if(((TraRPCHit*)(fTraHitCA->At(p4)))->GetPlane()  == id4){
         x4 = ((TraRPCHit*)(fTraHitCA->At(p4)))->GetCellXCenter();
         y4 = ((TraRPCHit*)(fTraHitCA->At(p4)))->GetCellYCenter();
         time4 = ((TraRPCHit*)(fTraHitCA->At(p4)))->GetTime();
         A4 = CreateAMatrix(x4,y4,z4,time4);
         DataMatrix4 = CreateDataMatrix(x4,y4,time4);
	 // we calculate the saeta
         S4=(K1+K2+K3+K4).Invert()*(A1+A2+A3+A4);
         S4.Print();
 	 // Add the saeta to the array and set the indices
         TraRPCSaeta* Saeta;
         Saeta = AddSaeta(S4,id1, 4); // size = 4
         Saeta->SetHitsIndices(p1,p2,p3,p4);
	 // If desired, perform a correction in velocity (uncomment next line to call Drift function)
         // Drift(S4,DataMatrix1,DataMatrix2,DataMatrix3,DataMatrix4);
         }
        }
       }
      }
     }
    }
   }
  }  
}
 

  // -----  Private method Drift --------------------------------------------------------------
  TMatrixD TraRPCHit2Saeta::Drift(TMatrixD Saeta4, TMatrixD Data1, TMatrixD Data2, TMatrixD Data3, TMatrixD Data4)
{

  cout << "Signal drift applied to Saeta" << endl;

  Double_t b1[4]; // Data and coefficients of plane 1
  Double_t xc1 = Data1[0][0]; 
  Double_t yc1 = Data1[1][0];
  Double_t x1  = Data1[0][0];
  Double_t y1  = Data1[1][0];
  Double_t t1  = Data1[2][0];

  Double_t b2[4]; // plane 2
  Double_t xc2 = Data2[0][0]; 
  Double_t yc2 = Data2[1][0];
  Double_t x2  = Data2[0][0];
  Double_t y2  = Data2[1][0];
  Double_t t2  = Data2[2][0];

  Double_t b3[4]; // plane 3
  Double_t xc3 = Data3[0][0]; 
  Double_t yc3 = Data3[1][0];
  Double_t x3  = Data3[0][0];
  Double_t y3  = Data3[1][0];
  Double_t t3  = Data3[2][0];
 
  Double_t b4[4]; // plane 4
  Double_t xc4 = Data4[0][0]; 
  Double_t yc4 = Data4[1][0];
  Double_t x4  = Data4[0][0];
  Double_t y4  = Data4[1][0];
  Double_t t4  = Data4[2][0];

  // We define the K and A matrices
  TMatrixD KRest1(6,6), KRest2(6,6), KRest3(6,6), KRest4(6,6), ARest1(6,1), ARest2(6,1), ARest3(6,1), ARest4(6,1); 

  for(Int_t i=0; i<100;i++){
   Double_t sx  = Saeta4[0][0];
   Double_t sxp = Saeta4[1][0];
   Double_t sy  = Saeta4[2][0];
   Double_t syp = Saeta4[3][0];
   Double_t st  = Saeta4[4][0];
   Double_t ss  = Saeta4[5][0];
   Double_t module1=0.0, module2=0.0; // to check the convergence of the iterative process

   // In order to avoid problems during the iterative process, we shift the initial Saeta 
   // only in the first iteration
   if(i == 0) {
    sx=sx+0.1; 
    sy=sy+0.1;
   }

   // We evaluate the module of the initial saeta
   module1=TMath::Sqrt(sx*sx+sxp*sxp+sy*sy+syp*syp+st*st+ss*ss);

   // We calculate all the matrices for each plane
   SetCoefficients(sx,sxp,sy,syp,xc1,yc1,Z1,b1);
   KRest1 = CreateKRestMatrix(b1[0],b1[1],b1[2],b1[3],sxp,syp,ss,Z1);
   ARest1 = CreateARestMatrix(b1[0],b1[1],b1[2],b1[3],sx,sxp,sy,syp,ss,x1,y1,Z1,t1);

   SetCoefficients(sx,sxp,sy,syp,xc2,yc2,Z2,b2);
   KRest2 = CreateKRestMatrix(b2[0],b2[1],b2[2],b2[3],sxp,syp,ss,Z2);
   ARest2 = CreateARestMatrix(b2[0],b2[1],b2[2],b2[3],sx,sxp,sy,syp,ss,x2,y2,Z2,t2);

   SetCoefficients(sx,sxp,sy,syp,xc3,yc3,Z3,b3);
   KRest3 = CreateKRestMatrix(b3[0],b3[1],b3[2],b3[3],sxp,syp,ss,Z3);
   ARest3 = CreateARestMatrix(b3[0],b3[1],b3[2],b3[3],sx,sxp,sy,syp,ss,x3,y3,Z3,t3);

   SetCoefficients(sx,sxp,sy,syp,xc4,yc4,Z4,b4);
   KRest4 = CreateKRestMatrix(b4[0],b4[1],b4[2],b4[3],sxp,syp,ss,Z4);
   ARest4 = CreateARestMatrix(b4[0],b4[1],b4[2],b4[3],sx,sxp,sy,syp,ss,x4,y4,Z4,t4);

   // We build the Saeta with restrictions
   Saeta4 =(KRest1+KRest2+KRest3+KRest4).Invert()*(ARest1+ARest2+ARest3+ARest4);
 
   // We evaluate the module of the saeta after the iteration
   module2=TMath::Sqrt(Saeta4[0][0]*Saeta4[0][0]+Saeta4[1][0]*Saeta4[1][0]+Saeta4[2][0]*Saeta4[2][0]+Saeta4[3][0]*Saeta4[3][0]+Saeta4[4][0]*Saeta4[4][0]+Saeta4[5][0]*Saeta4[5][0]);

   // If the modules satify the convergence condition we stop the iterative process  
   if (TMath::Abs(module2-module1)/module1 < 0.01){
   cout << "The convergence after " << i+1 << " iterations is reached: " << endl;
   // We add the saeta to the array
   AddSaeta(Saeta4,1, 5);
   Saeta4.Print();
   // If desired; print the reduced error matrix (uncomment the next two lines)
   // cout << "The reduced error matrix after convergence: " << endl;  
   // Reduce((KRest1+KRest2+KRest3+KRest4).Invert());
   break; 
   }
   
   // If no convergence is achieved after 100 iterations, we stop the process.
   if(i == 99) {
   cout << "No convergence achieved." << endl;
   Saeta4.Print();
   }
  }
   return Saeta4;
  

}


  // -----  Private method Reduce -------------------------------------- 
  TMatrixD TraRPCHit2Saeta::Reduce(TMatrixD B)
{
  for(Int_t i=0; i<6; i++){B[i][i]=TMath::Sqrt(B[i][i]); }
   for(Int_t i=0;i<6;i++){
    for(Int_t j=0;j<6;j++){
     if( i !=  j) B[i][j] = B[i][j]/(B[i][i]*B[j][j]);
    }
   }
  B.Print();
  return B;
}


  // ----- Private method SetCoefficients ------------------------------------- 
  // calculates some numbers that are always present during the iterative method.
  // We take the components of the Saeta as an input to calculate the coefficientes. Therefore we denote them with a letter "S": Sx, Sxp, ...
  // X, Y, Z are the coordinates of the hit.
  void TraRPCHit2Saeta::SetCoefficients(Double_t Sx, Double_t Sxp, Double_t Sy, Double_t Syp, Double_t Xc, Double_t Yc, Double_t Z, Double_t *coef)
{
  coef[0] = TMath::Sqrt(1+Sxp*Sxp+Syp*Syp); // b1 
  coef[1] = TMath::Sqrt((Sx+Sxp*Z-Xc)*(Sx+Sxp*Z-Xc)+(Sy+Syp*Z-Yc)*(Sy+Syp*Z-Yc)); //b2
  coef[2] = Sx+Sxp*Z-Xc; // b3
  coef[3] = Sy+Syp*Z-Yc; // b4
  return;
}

ClassImp(TraRPCHit2Saeta)



