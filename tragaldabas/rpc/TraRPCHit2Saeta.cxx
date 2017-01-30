// ------------------------------------------------------------------------
// -----                TraRPCHit2Saeta  source file                  -----
// -----              Created 01/09/16  by P. Cabanelas               -----
// -----              Last modified 15/09/2016  by J. Cuenca          -----
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
#include "TraRPCPoint.h"
#include "TRandom3.h"

using std::cout;
using std::cerr;
using std::endl;


TraRPCHit2Saeta::TraRPCHit2Saeta() : FairTask("Tragaldabas Hit to Saeta task")
{

  fTraHitCA=0;     // TraRPCHit clones array
  fTraSaetaCA=0;   // TraRPCSaeta clones array
  fTraPointCA=0;   // TraRPCPoint clones array
}


TraRPCHit2Saeta::~TraRPCHit2Saeta()
{

  LOG(INFO) << "TraRPCHit2Saeta: Delete instance" << FairLogger::endl;
  if(fTraHitCA) { fTraHitCA->Delete(); delete fTraHitCA; }
  if(fTraSaetaCA) { fTraSaetaCA->Delete(); delete fTraSaetaCA; }
  if(fTraPointCA) { fTraPointCA->Delete(); delete fTraPointCA; }
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
  fTraPointCA = (TClonesArray*) ioManager->GetObject("RPCPoint");

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
  // Global variables: position of the planes, 1/sigma's. 
Double_t wx=12.0/(12.6*12.6), wy=12.0/(12.3*12.3), wt=1.0/(0.05*0.05), sv=1.0/18.0;
Double_t Z1=0.0, Z2=52.2, Z3=90.2, Z4=173.9;

void TraRPCHit2Saeta::Exec(Option_t* opt)
{

  // Reset entries in output arrays, local arrays
  Reset();

  //ALGORITHMS FOR SAETA FINDING
  
  Double_t Xcoor=0.0, Xslope=0.0, Ycoor=0.0, Yslope=0.0, time=0.0, slowness=0.0;  
  Double_t X1=0.0, X2=0.0, X3=0.0, X4=0.0, Y1=0.0, Y2=0.0, Y3=0.0, Y4=0.0, Time1=0.0, Time2=0.0, Time3=0.0, Time4=0.0;
  Double_t Xdat1=0.0, Xdat2=0.0, Xdat3=0.0, Xdat4=0.0, Ydat1=0.0, Ydat2=0.0, Ydat3=0.0, Ydat4=0.0;
  TRandom3 *ran = new TRandom3();

  Double_t random1 = 0.0;
  Double_t random2 = 0.0;
  Double_t random3 = 0.0;
  Double_t random4 = 0.0;
  Double_t random5 = 0.0;
  Double_t random6 = 0.0;

  // K Matrices. We need them to build the subsaetas. Since they are constant we calculate them before the loop in hits.
  
  TMatrixD KMatrix1(6,6), KMatrix2(6,6), KMatrix3(6,6), KMatrix4(6,6), RelativeKMatrix34(6,6), RelativeKMatrix23(6,6);
  TMatrixD KRestMatrix1(6,6), KRestMatrix2(6,6), KRestMatrix3(6,6), KRestMatrix4(6,6), KRestTot(6,6), KTot(6,6);
  KMatrix1 = CreateKMatrix(0.0);
  KMatrix2 = CreateKMatrix(Z2);
  KMatrix3 = CreateKMatrix(Z3);
  KMatrix4 = CreateKMatrix(Z4);
  RelativeKMatrix23 = CreateKMatrix(Z3-Z2);
  RelativeKMatrix34 = CreateKMatrix(Z4-Z3);

  // A-data Matrices
  TMatrixD AMatrix1(6,1), AMatrix2(6,1), AMatrix3(6,1), AMatrix4(6,1), AMatrix02(6,1), AMatrix03(6,1), RelativeAMatrix23(6,1), RelativeAMatrix34(6,1);
  TMatrixD ARestMatrix1(6,1), ARestMatrix2(6,1), ARestMatrix3(6,1), ARestMatrix4(6,1), SaetaRet(6,1);

  // Subsaeta matrices
  TMatrixD ST(6,1), Saeta34(6,1), Saeta23(6,1), Saeta12(6,1);
 
  TraRPCHit** rpcHit;

  Int_t rpcHits;        // Nb of TraRPCHits in current event
  rpcHits = fTraHitCA->GetEntries();


  
 TraRPCPoint** rpcPoint;

  Int_t rpcPoints;        // Nb of points in current event
  rpcPoints = fTraPointCA->GetEntries();


// Los hits
  if (rpcHits>0) {
    rpcHit = new TraRPCHit*[rpcHits];

    for (Int_t i=0; i<rpcHits; i++) {
      rpcHit[i] = new TraRPCHit;
      rpcHit[i] = (TraRPCHit*) fTraHitCA->At(i);
 //     Xcoor = rpcHit[i]->GetCellXCenter(); //Example of accessing to RpcHit info
    }
  }

// Los points
   if(rpcPoints>0) {
      rpcPoint = new TraRPCPoint*[rpcPoints];
      for(Int_t j=0;j<rpcPoints;j++){
        rpcPoint[j] = new TraRPCPoint;
        rpcPoint[j] = (TraRPCPoint*) fTraPointCA->At(j);
      }
    }


// --------- Loops sobre los planos para extraer coordenadas y montar subsaetas -----
// Entre planos 3 y 4:
  for(Int_t p4=0;p4<rpcHits;p4++){
    if(rpcHit[p4]->GetPlane()==4){
      random4 = ran->Uniform(0.0,1.0);
      X4 = rpcHit[p4]->GetCellXCenter();
      Y4 = rpcHit[p4]->GetCellYCenter();
      Time4 = rpcHit[p4]->GetTime();
      Xdat4 = rpcHit[p4]->GetCellXCenter()-6.3+12.6*random4;
      Ydat4 = rpcHit[p4]->GetCellYCenter()-6.15+12.3*random4;
      AMatrix4=CreateAMatrix(X4, Y4, Z4, Time4);
      cout << "Plano: " << rpcHit[p4]->GetPlane() << " ---------- " << endl;
      cout << "X4: " << X4 << "  Y4: " << Y4 << " Time4 " << Time4 <<endl;
      cout << "Randomizado Xdat4: " << Xdat4 << " Ydat4 " << Ydat4 << endl;
      cout << "Celda: (" << rpcHit[p4]->GetCellRow() << " , " << rpcHit[p4]->GetCellColumn() << ")" <<  endl;
      cout << "-----------------------" << endl;
      for(Int_t p3=0; p3< rpcHits;p3++){
        if(rpcHit[p3]->GetPlane()==3){
          random3 = ran->Uniform(0.0,1.0);
  	  X3 = rpcHit[p3]->GetCellXCenter();
	  Y3 = rpcHit[p3]->GetCellYCenter();
	  Time3 = rpcHit[p3]->GetTime();
          Xdat3 = rpcHit[p3]->GetCellXCenter()-6.3+12.6*random3;
          Ydat3 = rpcHit[p3]->GetCellYCenter()-6.15+12.3*random3;
//          Xdat3 = rpcPoint[p3]->GetXIn();
//          Ydat3 = rpcPoint[p3]->GetYIn();
	  cout << "Plano: " << rpcHit[p3]->GetPlane() << " ---------- " << endl;
	  cout << "X3: " << X3 << "  Y3: " << Y3 << " Time3 " << Time3 << endl;
	  cout << "Randomizado Xdat3: " << Xdat3 << " Ydat3 " << Ydat3 << endl;
          cout << "Celda: (" << rpcHit[p3]->GetCellRow() << " , " << rpcHit[p3]->GetCellColumn() << ")" <<  endl;
	  cout << "-----------------------" << endl;
	  AMatrix3=CreateAMatrix(X3, Y3, Z3, Time3);
	  AMatrix03 = CreateAMatrix(X3, Y3, 0.0, Time3);
	  RelativeAMatrix34 = CreateAMatrix(X4,Y4,Z4-Z3,Time4);
	  Saeta34=(KMatrix1+RelativeKMatrix34).Invert()*(AMatrix03+RelativeAMatrix34);
          // If speed is greater than c, we discard the subsaeta
	  if(Saeta34[5][0]<1.0/30.0-1.0/wt) continue;
      for(Int_t p2=0; p2< rpcHits;p2++){
        if(rpcHit[p2]->GetPlane()==2){
            random2 = ran->Uniform(0.0,1.0);
	    X2 = rpcHit[p2]->GetCellXCenter();
            Y2 = rpcHit[p2]->GetCellYCenter();
            Time2 = rpcHit[p2]->GetTime();
	    Xdat2 = rpcHit[p2]->GetCellXCenter()-6.3+12.6*random2;
            Ydat2 = rpcHit[p2]->GetCellXCenter()-6.15+12.3*random2;
            cout << "Plano: " << rpcHit[p2]->GetPlane() << " ---------- " << endl;
            cout << "X2: " << X2 << "  Y2: " << Y2 << " Time2 " << Time2 << endl;
	    cout << "Randomizado Xdat2: " << Xdat2 << " Ydat2 " << Ydat2 << endl;
	    cout << "Celda: (" << rpcHit[p2]->GetCellRow() << " , " << rpcHit[p2]->GetCellColumn() << ")" <<  endl;
            cout << "-----------------------" << endl;
	    AMatrix2 = CreateAMatrix(X2, Y2, Z2, Time2);
	    AMatrix02 = CreateAMatrix(X2, Y2, 0.0, Time2);
            RelativeAMatrix23 = CreateAMatrix(X3,Y3,Z3-Z2,Time3);
            Saeta23=(KMatrix1+RelativeKMatrix23).Invert()*(AMatrix02+RelativeAMatrix23);
            if(Saeta23[5][0]<1.0/30.0-1.0/wt) continue;
        for(Int_t p1=0; p1< rpcHits;p1++){
          if(rpcHit[p1]->GetPlane()==1){
              random1 = ran->Uniform(0.0,1.0);
              X1 = rpcHit[p1]->GetCellXCenter();
              Y1 = rpcHit[p1]->GetCellYCenter();
              Time1 = rpcHit[p1]->GetTime();
	      Xdat1 = rpcHit[p1]->GetCellXCenter()-6.3+12.6*random1;
      	      Ydat1 = rpcHit[p1]->GetCellXCenter()-6.15+12.3*random1;
              cout << "Plano: " << rpcHit[p1]->GetPlane() << " ---------- " << endl;
              cout << "X1: " << X1 << "  Y1: " << Y1 << " Time1 " << Time1 << endl;
	      cout << "Randomizado Xdat1: " << Xdat1 << " Ydat1 " << Ydat1 << endl;
              cout << "Celda: (" << rpcHit[p1]->GetCellRow() << " , " << rpcHit[p1]->GetCellColumn() << ")" <<  endl;
	  //    cout << "Mahalanobis: " << Mahalanobis(1,2,3,4,0.3) << endl;
              cout << "-----------------------" << endl;
	      AMatrix1 = CreateAMatrix(X1, Y1, Z1, Time1);
              Saeta12 = (KMatrix1+KMatrix2).Invert()*(AMatrix1+AMatrix2);
//Saeta12.Print();
//Saeta23.Print();
//Saeta34.Print();
	      // The complete saeta is S = (TotalK)^-1*TotalA 
	      ST = (KMatrix1+KMatrix2+KMatrix3+KMatrix4).Invert()*(AMatrix1+AMatrix2+AMatrix3+AMatrix4); 
//KTot=(KMatrix1+KMatrix2+KMatrix3+KMatrix4).Invert();
//((KMatrix1+KMatrix2+KMatrix3+KMatrix4).Invert()).Print();
	      ST.Print();

//-------------------------------------
// K and A Matrices including the signal drift. The total saeta ST is used as input of the iterative process. 
// We use the same set of coefficients to build both A and K together and then reuse the array
random5 = ran->Uniform(-6.3,6.3);
random6 = ran->Uniform(-6.15,6.15);
ST[0][0]=ST[0][0]+random5;
ST[2][0]=ST[2][0]+random6;

for(int k=0; k<6; k++){
Double_t coef[4];
SetCoefficients(ST[0][0],ST[1][0],ST[2][0],ST[3][0],X1,Y1,Z1,coef);
KRestMatrix1 = CreateKRestMatrix(coef[0],coef[1],coef[2],coef[3],ST[1][0],ST[3][0],ST[5][0],Z1);
ARestMatrix1 = CreateARestMatrix(coef[0],coef[1],coef[2],coef[3],ST[0][0],ST[1][0],ST[2][0],ST[3][0],ST[5][0],Xdat1,Ydat1,Z1,Time1);

SetCoefficients(ST[0][0],ST[1][0],ST[2][0],ST[3][0],X2,Y2,Z2,coef);
KRestMatrix2 = CreateKRestMatrix(coef[0],coef[1],coef[2],coef[3],ST[1][0],ST[3][0],ST[5][0],Z2);
ARestMatrix2 = CreateARestMatrix(coef[0],coef[1],coef[2],coef[3],ST[0][0],ST[1][0],ST[2][0],ST[3][0],ST[5][0],Xdat2,Ydat2,Z2,Time2);

SetCoefficients(ST[0][0],ST[1][0],ST[2][0],ST[3][0],X3,Y3,Z3,coef);
KRestMatrix3 = CreateKRestMatrix(coef[0],coef[1],coef[2],coef[3],ST[1][0],ST[3][0],ST[5][0],Z3);
ARestMatrix3 = CreateARestMatrix(coef[0],coef[1],coef[2],coef[3],ST[0][0],ST[1][0],ST[2][0],ST[3][0],ST[5][0],Xdat3,Ydat3,Z3,Time3);

SetCoefficients(ST[0][0],ST[1][0],ST[2][0],ST[3][0],X4,Y4,Z4,coef);
KRestMatrix4 = CreateKRestMatrix(coef[0],coef[1],coef[2],coef[3],ST[1][0],ST[3][0],ST[5][0],Z4);
ARestMatrix4 = CreateARestMatrix(coef[0],coef[1],coef[2],coef[3],ST[0][0],ST[1][0],ST[2][0],ST[3][0],ST[5][0],Xdat4,Ydat4,Z4,Time4);
ST=(KRestMatrix1+KRestMatrix2+KRestMatrix3+KRestMatrix4).Invert()*(ARestMatrix1+ARestMatrix2+ARestMatrix3+ARestMatrix4);
ST.Print();
}
/*
cout << "La matriz error con deriva: " << endl;
KRestTot=(KRestMatrix1+KRestMatrix2+KRestMatrix3+KRestMatrix4).Invert();
KRestTot.Print();
cout << "La matriz reducida: " << endl;
//Reduce(KRestTot);
// ((KRestMatrix1+KRestMatrix2+KRestMatrix3+KRestMatrix4).Invert()).Print();
*/
              }
	    }
	  }
        } 
      }
    }
  }
}

// ----------------- hasta aquí -------------------------------------------------------
Xcoor = ST[0][0];
Xslope = ST[1][0];
Ycoor = ST[2][0];
Yslope = ST[3][0];
time = ST[4][0];
slowness = ST[5][0];

  AddSaeta(Xcoor,Xslope,Ycoor,Yslope,time,slowness);

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
// This function sets the K matrices
  TMatrixD TraRPCHit2Saeta::CreateKMatrix(double Z){
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

// This function sets the A-data matrices.
  TMatrixD TraRPCHit2Saeta::CreateAMatrix(double X, double Y, double Z, double T){
  TMatrixD A(6,1);
  A[0][0]=X*wx;
  A[1][0]=X*Z*wx;
  A[2][0]=Y*wy;
  A[3][0]=Y*Z*wy;
  A[4][0]=T*wt;
  A[5][0]=T*Z*wt;
  return A;
}

// This function calculates some coefficients that are always present during the iterative method.
// We take the components of the Saeta as an input to calculate the coefficientes. Therefore we denote them with a letter "S": Sx, Sxp, ...
// X, Y, Z are the coordinates of the hit.
void TraRPCHit2Saeta::SetCoefficients(Double_t Sx, Double_t Sxp, Double_t Sy, Double_t Syp, Double_t Xc, Double_t Yc, Double_t Z, Double_t *coef){
  coef[0] = TMath::Sqrt(1+Sxp*Sxp+Syp*Syp); // b1
  coef[1] = TMath::Sqrt((Sx+Sxp*Z-Xc)*(Sx+Sxp*Z-Xc)+(Sy+Syp*Z-Yc)*(Sy+Syp*Z-Yc)); //b2
  coef[2] = Sx+Sxp*Z-Xc; // b3
  coef[3] = Sy+Syp*Z-Yc; // b4
  return;
}

// This function sets the K matrices with signal drift constraint.
TMatrixD TraRPCHit2Saeta::CreateKRestMatrix(Double_t b1,Double_t b2,Double_t b3,Double_t b4,Double_t Sxp,Double_t Syp,Double_t Ss,Double_t Z){
TMatrixD KR(6,6);
  KR[0][0]= wx+sv*sv*wt*b3*b3/(b2*b2);
  KR[0][1]= wx*Z+b3*sv*wt*Z*(Ss*Sxp/b1+sv*b3/b2)/b2;
  KR[0][2]= sv*sv*wt*b3*b4/(b2*b2);
  KR[0][3]= sv*wt*Z*(Ss*Syp/b1+sv*b4/b2)*b3/b2;
  KR[0][4]= sv*wt*b3/b2;
  KR[0][5]= sv*wt*Z*b3*b1/b2;
  KR[1][0]= KR[0][1];
  KR[1][1]= wx*Z*Z+wt*Z*Z*(Ss*Sxp/b1+sv*b3/b2)*(Ss*Sxp/b1+sv*b3/b2);
  KR[1][2]= sv*wt*Z*(Ss*Sxp/b1+sv*b3/b2)*b4/b2;
  KR[1][3]= wt*Z*Z*(Ss*Sxp/b1+sv*b3/b2)*(Ss*Syp/b1+sv*b4/b2);
  KR[1][4]= wt*Z*(Ss*Sxp/b1+sv*b3/b2);
  KR[1][5]= wt*Z*Z*(Ss*Sxp/b1+sv*b3/b2)*b1;
  KR[2][0]= KR[0][2];
  KR[2][1]= KR[1][2];
  KR[2][2]= wy+sv*sv*wt*b4*b4/(b2*b2);
  KR[2][3]= wy*Z+sv*wt*b4*Z*(Ss*Syp/b1+sv*b4/b2)/b2;
  KR[2][4]= sv*wt*b4/b2;
  KR[2][5]= sv*wt*Z*b4*b1/b2;
  KR[3][0]= KR[0][3];
  KR[3][1]= KR[1][3];
  KR[3][2]= KR[2][3];
  KR[3][3]= wy*Z*Z+wt*Z*Z*(Ss*Syp/b1+sv*b4/b2)*(Ss*Syp/b1+sv*b4/b2);
  KR[3][4]= wt*Z*(Ss*Syp/b1+sv*b4/b2);
  KR[3][5]= wt*Z*Z*(Ss*Syp/b1+sv*b4/b2)*b1;
  KR[4][0]= KR[0][4];
  KR[4][1]= KR[1][4];
  KR[4][2]= KR[2][4];
  KR[4][3]= KR[3][4];
  KR[4][4]= wt;
  KR[4][5]= wt*Z*b1;
  KR[5][0]= KR[0][5];
  KR[5][1]= KR[1][5];
  KR[5][2]= KR[2][5];
  KR[5][3]= KR[3][5];
  KR[5][4]= KR[4][5];
  KR[5][5]= wt*b1*b1*Z*Z;
  return KR;
}

// This function sets the A matrices with constraints. 4 coefficients, 5 saeta-coordinates, 4 hit coordinates (x,y,z,t)
TMatrixD TraRPCHit2Saeta::CreateARestMatrix(Double_t b1,Double_t b2,Double_t b3,Double_t b4,
Double_t Sx,Double_t Sxp, Double_t Sy, Double_t Syp, Double_t Ss, 
Double_t X, Double_t Y, Double_t Z, Double_t T){
TMatrixD AR(6,1);
  AR[0][0] = wx*X+sv*wt*b3/b2*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Z*(Ss*Sxp*Sxp/b1+sv*Sxp*b3/b2)+Z*(Ss*Syp*Syp/b1+sv*Syp*b4/b2));
  AR[1][0] = wx*X*Z+wt*Z*(Ss*Sxp/b1+sv*b3/b2)*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Z*(Ss*Sxp*Sxp/b1+sv*Sxp*b3/b2)+Z*(Ss*Syp*Syp/b1+sv*Syp*b4/b2));
  AR[2][0] = wy*Y+sv*wt*b4/b2*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Z*(Ss*Sxp*Sxp/b1+sv*Sxp*b3/b2)+Z*(Ss*Syp*Syp/b1+sv*Syp*b4/b2));
  AR[3][0] = wy*Y*Z+wt*(Ss*Syp*Z/b1+sv*Z*b4/b2)*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Sxp*(Ss*Sxp*Z/b1+sv*Z*b3/b2)+Syp*(Ss*Syp*Z/b1+sv*Z*b4/b2));
  AR[4][0] = wt*(T+sv*Sx*b3/b2+sv*Sy*b4/b2-sv*b2+Sxp*(Ss*Sxp*Z/b1+sv*Z*b3/b2)+Syp*(Ss*Syp*Z/b1+sv*Z*b4/b2));
  AR[5][0] = AR[4][0]*b1*Z;
return AR;
}

// This function calculates the Mahalanobis distance
Double_t TraRPCHit2Saeta::Mahalanobis(Double_t x1, Double_t y1, Double_t x2, Double_t y2, Double_t r){
Double_t DM;
DM = TMath::Sqrt(1.0/(1-r*r)*( (x1-x2)*(x1-x2)*wx+(y1-y2)*(y1-y2)*wy-2*r*(x1-x2)*(y1-y2)*TMath::Sqrt(wx*wy) ));
return DM;
}

//Esta funcion deja las matrices en funcion de sigma y rho
TMatrixD TraRPCHit2Saeta::Reduce(TMatrixD B){
B[0][0]=TMath::Sqrt(B[0][0]);
B[1][1]=TMath::Sqrt(B[1][1]);
B[2][2]=TMath::Sqrt(B[2][2]);
B[3][3]=TMath::Sqrt(B[3][3]);
B[4][4]=TMath::Sqrt(B[4][4]);
B[5][5]=TMath::Sqrt(B[5][5]);
B[0][1]=B[0][1]/(B[0][0]*B[1][1]);
B[1][0]=B[0][1];
B[0][2]=B[0][2]/(B[0][0]*B[2][2]);
B[2][0]=B[0][2];
B[0][3]=B[0][3]/(B[0][0]*B[3][3]);
B[3][0]=B[0][3];
B[0][4]=B[0][4]/(B[0][0]*B[4][4]);
B[4][0]=B[0][4];
B[0][5]=B[0][5]/(B[0][0]*B[5][5]);
B[5][0]=B[0][5];
B[1][2]=B[1][2]/(B[1][1]*B[2][2]);
B[2][1]=B[1][2];
B[1][3]=B[1][3]/(B[1][1]*B[3][3]);
B[3][1]=B[1][3];
B[1][4]=B[1][4]/(B[1][1]*B[4][4]);
B[4][1]=B[1][4];
B[1][5]=B[1][5]/(B[1][1]*B[5][5]);
B[5][1]=B[1][5];
B[2][3]=B[2][3]/(B[2][2]*B[3][3]);
B[3][2]=B[2][3];
B[2][4]=B[2][4]/(B[2][2]*B[4][4]);
B[4][2]=B[2][4];
B[2][5]=B[2][5]/(B[2][2]*B[5][5]);
B[5][2]=B[2][5];
B[3][4]=B[3][4]/(B[3][3]*B[4][4]);
B[4][3]=B[3][4];
B[3][5]=B[3][5]/(B[3][3]*B[5][5]);
B[5][3]=B[3][5];
B[4][5]=B[4][5]/(B[4][4]*B[5][5]);
B[5][4]=B[4][5];
B.Print();
return B;
}

ClassImp(TraRPCHit2Saeta)



