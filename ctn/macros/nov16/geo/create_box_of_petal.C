////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////																										
////							--- Simulation of the Lisbon Nov-2016 setup ---								
////
////		This macro recreates one Aluminum BOX of the petal (empty)
////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"


// Create Matrix Unity
TGeoRotation *fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
TGeoRotation *fRefRot = NULL;

TGeoManager* gGeoMan = NULL;
TGeoVolume* gTop;

Double_t fThetaX = 0.;
Double_t fThetaY = 0.;
Double_t fThetaZ = 0.;
Double_t fPhi   = 0.;
Double_t fTheta = 0.;
Double_t fPsi   = 0.;
Double_t fX = 0.;
Double_t fY = 0.;
Double_t fZ = 0.;
Bool_t fLocalTrans = kFALSE;
Bool_t fLabTrans = kFALSE;

TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef);

void ConstructGeometry(TGeoMedium *pMed, TGeoMedium *pMedAl, TGeoMedium *pMedSteel, TGeoMedium *pMedGe, TGeoMedium *pMedLi, TGeoMedium *pMedAir);


void create_geo(const char* geoTag = "test")
{
	fGlobalTrans->SetTranslation(0.0,0.0,0.0);

	// -------   Load media from media file   ----------------------------------- 
	FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
	FairGeoInterface* geoFace = geoLoad->getGeoInterface();
	TString geoPath = gSystem->Getenv("VMCWORKDIR");
	TString medFile = geoPath + "/ctn/geometry/media.geo";
	geoFace->setMediaFile(medFile);
	geoFace->readMedia();
	gGeoMan = gGeoManager;
	// --------------------------------------------------------------------------



	// -------   Geometry file name (output)   ----------------------------------
	TString geoFileName = geoPath + "/ctn/geometry/box_petal_";
	geoFileName = geoFileName + geoTag + ".geo.root";
	// --------------------------------------------------------------------------



	// -----------------   Get and create the required media    ----------------- 
	FairGeoMedia* gGeoMedia = geoFace->getMedia(); 
	FairGeoBuilder* gGeoBuild = geoLoad->getGeoBuilder(); 

	FairGeoMedium* mVac      = gGeoMedia->getMedium("vacuum");
	if ( ! mVac ) Fatal("Main", "FairMedium vacuum not found");
	gGeoBuild->createMedium(mVac);
	TGeoMedium* pMedVac = gGeoMan->GetMedium("vacuum");
	if ( ! pMedVac ) Fatal("Main", "Medium vacuum not found");   

        FairGeoMedium* mAir      = gGeoMedia->getMedium("Air");
        if ( ! mAir ) Fatal("Main", "FairMedium Air not found");
        gGeoBuild->createMedium(mAir);
        TGeoMedium* pMedAir = gGeoMan->GetMedium("Air");
        if ( ! pMedAir ) Fatal("Main", "Medium Air not found");

	FairGeoMedium* mAl      = gGeoMedia->getMedium("aluminium");
	if ( ! mAl ) Fatal("Main", "FairMedium Al not found");
	gGeoBuild->createMedium(mAl);
	TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
	if ( ! pMedAl ) Fatal("Main", "Medium Al not found");       

	FairGeoMedium* mSteel      = gGeoMedia->getMedium("Steel");
	if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
	gGeoBuild->createMedium(mSteel);
	TGeoMedium* pMedSteel = gGeoMan->GetMedium("Steel");
	if ( ! pMedSteel ) Fatal("Main", "Medium Steel not found"); 

	FairGeoMedium* mGe      = gGeoMedia->getMedium("germanium");
	if ( ! mGe ) Fatal("Main", "FairMedium Ge not found");
	gGeoBuild->createMedium(mGe);
	TGeoMedium* pMedGe = gGeoMan->GetMedium("germanium");
	if ( ! pMedGe ) Fatal("Main", "Medium Ge not found");      

	FairGeoMedium* mLi      = gGeoMedia->getMedium("litium");
	if ( ! mLi ) Fatal("Main", "FairMedium Li not found");
	gGeoBuild->createMedium(mLi);
	TGeoMedium* pMedLi = gGeoMan->GetMedium("litium");
	if ( ! pMedLi ) Fatal("Main", "Medium Li not found");     

	// --------------------------------------------------------------------------



	// --------------   Create geometry and top volume  ------------------------- 
	gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
	gGeoMan->SetName("HPGEgeom");
	gTop = new TGeoVolumeAssembly("TOP");
	gGeoMan->SetTopVolume(gTop);
	

	// --------------------------------------------------------------------------


	ConstructGeometry(pMedVac, pMedAl, pMedSteel, pMedGe, pMedLi, pMedAir); 	


	// ---------------   Finish   ----------------------------------------------- 
	gGeoMan->CloseGeometry();
	gGeoMan->CheckOverlaps(0.001);
	gGeoMan->PrintOverlaps();
	gGeoMan->Test();

	TFile* geoFile = new TFile(geoFileName, "RECREATE");
	gTop->Write();
	geoFile->Close();
	// --------------------------------------------------------------------------
}


void ConstructGeometry(TGeoMedium *pMedVac, TGeoMedium *pMedAl, TGeoMedium *pMedSteel, TGeoMedium *pMedGe, TGeoMedium *pMedLi, TGeoMedium *pMedAir)
{
	cout << endl;
	cout << "-I- Ensar::ConstructGeometry() "<< endl;
	cout << "-I- Ensar  HPGe detector "<< endl;
	cout << endl;
	
	
        // Defintion of the Mother Volume
        Double_t length = 100.;
        TGeoShape *pCtnWorld = new TGeoBBox("CtnWorldBox",length/2.0,length/2.0,length/2.0);
        TGeoVolume* pWorld = new TGeoVolume("CtnWorld", pCtnWorld, pMedAir);
        TGeoCombiTrans *t0 = new TGeoCombiTrans();
        TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
        gTop->AddNode(pWorld,0,pGlobalc);
	
	//-----------------------------------------------

	
	
//---------------------- CALIFA PETALs   -------------------------	
//-------------------------------------------------------------------------------------
  // BOX PETALS --- Box as TRAPEZOID shape
 
  //-------------------------------
  //Trapezoid outer --Steel
   TGeoVolume *trap_out = gGeoManager->MakeArb8("trap_out", pMedAl, 18.15);
   trap_out->SetFillColor(kRed);
   trap_out->SetLineColor(kRed);
   TGeoArb8 *arb_out = (TGeoArb8*)trap_out->GetShape();
   arb_out->SetVertex(0, 7.,-27.5);
   arb_out->SetVertex(1, -7.,-27.5);
   arb_out->SetVertex(2, -7.,27.5);
   arb_out->SetVertex(3, 7., 27.5);
   arb_out->SetVertex(4, 14.25,-27.5);
   arb_out->SetVertex(5, -14.25,-27.5);
   arb_out->SetVertex(6, -14.25,27.5);
   arb_out->SetVertex(7, 14.25,27.5);
  //--------------------------------
   
  //Trapezoid inner --Air--------------------------------------------
   TGeoVolume *trap_in = gGeoManager->MakeArb8("trap_in", pMedVac, 18.15);
   trap_in->SetFillColor(kBlue);
   trap_in->SetLineColor(kBlue);
   TGeoArb8 *arb_in = (TGeoArb8*)trap_in->GetShape();
   arb_in->SetVertex(0, 6.5,-27.);
   arb_in->SetVertex(1, -6.5,-27.);
   arb_in->SetVertex(2, -6.5,27.);
   arb_in->SetVertex(3, 6.5, 27.);
   arb_in->SetVertex(4, 13.75,-27.);
   arb_in->SetVertex(5, -13.75,-27.);
   arb_in->SetVertex(6, -13.75,27.);
   arb_in->SetVertex(7, 13.75,27.);
  //-------------------
 
  //down BASE of mylar
   TGeoVolume* base_mylar = gGeoManager->MakeBox("base_mylar", pMedAl, 6., 27., 0.00015);
   base_mylar->SetFillColor(5);
   base_mylar->SetLineColor(5);
  
   TGeoTranslation *trans_base_mylar = new TGeoTranslation(0.,0.,-18.14985);
   trap_in->AddNode(base_mylar,1,trans_base_mylar);
   base_mylar->SetVisLeaves(kTRUE);
  //--------------------
 
   trap_out->AddNode(trap_in, 1, new TGeoTranslation(0,0,0));
   trap_in->SetVisLeaves(kTRUE);
  //--------------------------------------------------------------
 
  //-------------------------------------------------------------
  //upper BASE of 5083-Aluminium as trapezoid
    
   TGeoVolume *base_al = gGeoManager->MakeArb8("base_al", pMedAl, 3.);
   base_al->SetFillColor(kRed);
   base_al->SetLineColor(kRed);
   TGeoArb8 *arb_base_al = (TGeoArb8*)base_al->GetShape();
   arb_base_al->SetVertex(0, 12.,-27.);
   arb_base_al->SetVertex(1, -12.,-27.);
   arb_base_al->SetVertex(2, -12.,27.);
   arb_base_al->SetVertex(3, 12.,27.);
   arb_base_al->SetVertex(4, 13.,-27.);
   arb_base_al->SetVertex(5, -13.,-27.);
   arb_base_al->SetVertex(6, -13.,27.);
   arb_base_al->SetVertex(7, 13.,27.);
  //--------------------------------
  
  
  //BOX air 1 with TAP of Steel 
   TGeoVolume* hole_air = gGeoManager->MakeBox("hole_air", pMedVac, 10.5, 9.75, 2);
   hole_air->SetFillColor(18);
   hole_air->SetLineColor(18);
  
   TGeoVolume* tap = gGeoManager->MakeBox("tap", pMedAl, 7.8, 7.8, 0.15);
   tap->SetFillColor(16);
   tap->SetLineColor(16);
   TGeoTranslation *trans_tap = new TGeoTranslation(0,0,-1.85);
   hole_air->AddNode(tap,1,trans_tap);
   tap->SetVisLeaves(kTRUE);
  
   TGeoTranslation *trans_hole_1 = new TGeoTranslation(0,-11.75,1);
   TGeoTranslation *trans_hole_2 = new TGeoTranslation(0,11.75,1);
   base_al->AddNode(hole_air,1,trans_hole_1);
   base_al->AddNode(hole_air,2,trans_hole_2);
   hole_air->SetVisLeaves(kTRUE);
  //--------------------------------
  
  //BOX air 2 (down)
   TGeoVolume* hole_air_down = gGeoManager->MakeBox("hole_air_down", pMedVac, 6.1, 6.1, 1);
   hole_air_down->SetFillColor(18);
   hole_air_down->SetLineColor(18);
  
   TGeoTranslation *trans_hole_down_1 = new TGeoTranslation(0,-11.75,-2);
   TGeoTranslation *trans_hole_down_2 = new TGeoTranslation(0,11.75,-2);
   base_al->AddNode(hole_air_down,1,trans_hole_down_1);
   base_al->AddNode(hole_air_down,2,trans_hole_down_2);
   hole_air_down->SetVisLeaves(kTRUE);
  //----------------------------------
   
   TGeoTranslation *trans_base_al = new TGeoTranslation(0.,0.,15.15);
   trap_in->AddNode(base_al,1,trans_base_al);
   base_al->SetVisLeaves(kTRUE);
   //---------------------------------
   
   //BAR of the corners   ----Al
   TGeoVolume *trap_bar = gGeoManager->MakeArb8("trap_bar", pMedAl, 15.15);
   trap_bar->SetFillColor(kBlue);
   trap_bar->SetLineColor(kBlue);
   TGeoArb8 *arb_bar = (TGeoArb8*)trap_bar->GetShape();
   arb_bar->SetVertex(0, 1.875,1.);
   arb_bar->SetVertex(1, 1.875,-1.);
   arb_bar->SetVertex(2, 4.375,-1.);
   arb_bar->SetVertex(3, 4.375,1.);
   arb_bar->SetVertex(4, -4.375,1.);
   arb_bar->SetVertex(5, -4.375,-1.);
   arb_bar->SetVertex(6, -1.875,-1.);
   arb_bar->SetVertex(7, -1.875,1.);
   
   TGeoVolume *trap_bar_rot = gGeoManager->MakeArb8("trap_bar_rot", pMedAl, 15.15);
   trap_bar_rot->SetFillColor(kBlue);
   trap_bar_rot->SetLineColor(kBlue);
   TGeoArb8 *arb_bar_rot = (TGeoArb8*)trap_bar_rot->GetShape();
   arb_bar_rot->SetVertex(0, -1.875,1.);
   arb_bar_rot->SetVertex(1, -1.875,-1.);
   arb_bar_rot->SetVertex(2, -4.375,-1.);
   arb_bar_rot->SetVertex(3, -4.375,1.);
   arb_bar_rot->SetVertex(4, 4.375,1.);
   arb_bar_rot->SetVertex(5, 4.375,-1.);
   arb_bar_rot->SetVertex(6, 1.875,-1.);
   arb_bar_rot->SetVertex(7, 1.875,1.);
   
   trap_in->AddNode(trap_bar_rot,1,new TGeoTranslation(8.175,26.,-3.));//A
   trap_in->AddNode(trap_bar_rot,2,new TGeoTranslation(8.175,-26.,-3.));//D
   trap_bar_rot->SetVisLeaves(kTRUE);
   trap_in->AddNode(trap_bar,1,new TGeoTranslation(-8.175,26.,-3.));//B
   trap_in->AddNode(trap_bar,2,new TGeoTranslation(-8.175,-26.,-3.));//C
   trap_bar->SetVisLeaves(kTRUE);
   
  //-------------------
  //-----------------------------------------------------------------
   
   
   
  //---------------------------------
  //BOX of petal
  TGeoRotation *rot_box_p5    = new TGeoRotation("rot_box_p5",0.,0.,0.);
  TGeoCombiTrans *comb_box_p5 = new TGeoCombiTrans("comb",0.,0.,0.,rot_box_p5);
  pWorld->AddNode(trap_out,1,comb_box_p5);
  trap_out->SetVisLeaves(kTRUE);



}


TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef)
{
  if (fLocalTrans == kTRUE ) {
    
    if ( ( fThetaX == 0 )  && ( fThetaY==0 )  && ( fThetaZ == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    // X axis
    Double_t xAxis[3] = { 1. , 0. , 0. };
    Double_t yAxis[3] = { 0. , 1. , 0. };
    Double_t zAxis[3] = { 0. , 0. , 1. };
    // Reference Rotation
    fRefRot = fRef->GetRotation();
    
    if (fRefRot) {
      Double_t mX[3] = {0.,0.,0.};
      Double_t mY[3] = {0.,0.,0.};
      Double_t mZ[3] = {0.,0.,0.};
      
      fRefRot->LocalToMasterVect(xAxis,mX);
      fRefRot->LocalToMasterVect(yAxis,mY);
      fRefRot->LocalToMasterVect(zAxis,mZ);
      
      Double_t a[4]={ mX[0],mX[1],mX[2], fThetaX };
      Double_t b[4]={ mY[0],mY[1],mY[2], fThetaY };
      Double_t c[4]={ mZ[0],mZ[1],mZ[2], fThetaZ };
      
      ROOT::Math::AxisAngle aX(a,a+4);
      ROOT::Math::AxisAngle aY(b,b+4);
      ROOT::Math::AxisAngle aZ(c,c+4);
      
      ROOT::Math::Rotation3D fMatX( aX );
      ROOT::Math::Rotation3D fMatY( aY );
      ROOT::Math::Rotation3D fMatZ( aZ );
      
      ROOT::Math::Rotation3D  fRotXYZ = (fMatZ * (fMatY * fMatX));
      
      //cout << fRotXYZ << endl;
      
      Double_t fRotable[9]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
      fRotXYZ.GetComponents(
			    fRotable[0],fRotable[3],fRotable[6],
			    fRotable[1],fRotable[4],fRotable[7],
			    fRotable[2],fRotable[5],fRotable[8]
			    );
      TGeoRotation *pRot = new TGeoRotation();
      pRot->SetMatrix(fRotable);
      TGeoCombiTrans *pTmp = new TGeoCombiTrans(*fGlobalTrans,*pRot);
      
      // ne peut pas etre applique ici
      // il faut differencier trans et rot dans la multi.
      TGeoRotation rot_id;
      rot_id.SetAngles(0.0,0.0,0.0);
      
      TGeoCombiTrans c1;
      c1.SetRotation(rot_id);
      const Double_t *t = pTmp->GetTranslation();
      c1.SetTranslation(t[0],t[1],t[2]);
      
      TGeoCombiTrans c2;
      c2.SetRotation(rot_id);
      const Double_t *tt = fRefRot->GetTranslation();
      c2.SetTranslation(tt[0],tt[1],tt[2]);
      
      TGeoCombiTrans cc = c1 * c2 ;
      
      TGeoCombiTrans c3;
      c3.SetRotation(pTmp->GetRotation());
      TGeoCombiTrans c4;
      c4.SetRotation(fRefRot);
      
      TGeoCombiTrans ccc = c3 * c4;
      
      TGeoCombiTrans pGlobal;
      pGlobal.SetRotation(ccc.GetRotation());
      const Double_t *allt = cc.GetTranslation();
      pGlobal.SetTranslation(allt[0],allt[1],allt[2]);
      
      return  ( new TGeoCombiTrans( pGlobal ) );
      
    }else{
      
      cout << "-E- EnsarDetector::GetGlobalPosition() \
	      No. Ref. Transformation defined ! " << endl;
      cout << "-E- EnsarDetector::GetGlobalPosition() \
	      cannot create Local Transformation " << endl;
      return NULL;
    } //! fRefRot
    
  } else {
    // Lab Transf.
    if ( ( fPhi == 0 )  && ( fTheta==0 )  && ( fPsi == 0 )
	 &&
	 ( fX == 0 ) && ( fY == 0 ) && ( fZ == 0 )
	 )  return fRef;
    
    
    return ( new TGeoCombiTrans(*fGlobalTrans,*fGlobalRot) );
    
  }
}

