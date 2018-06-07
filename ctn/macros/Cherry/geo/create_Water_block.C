///////////////////////////////////////////////////////////////////////
////																				       	
////	--- Simulation of Cherries ---							
////		Created by E.Galiana
////
///////////////////////////////////////////////////////////////////////

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

void ConstructGeometry(TGeoMedium *pMedVac, TGeoMedium *pMedH2O, TGeoMedium *pMedAir, TGeoMedium *pMedSteel, TGeoMedium *pMedAl, TGeoMedium *pMedAcrylic);


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
  TString geoFileName = geoPath + "/ctn/geometry/Cherry_";
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
  
  FairGeoMedium* mH2O     = gGeoMedia->getMedium("H2O");
  if ( ! mH2O ) Fatal("Main", "FairMedium H2O not found");
  gGeoBuild->createMedium(mH2O);
  TGeoMedium* pMedH2O = gGeoMan->GetMedium("H2O");
  if ( ! pMedH2O ) Fatal("Main", "Medium H2O not found");      

 	FairGeoMedium* mSteel     = gGeoMedia->getMedium("Steel");
  if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
  gGeoBuild->createMedium(mSteel);
  TGeoMedium* pMedSteel = gGeoMan->GetMedium("Steel");
  if ( ! pMedSteel ) Fatal("Main", "Medium Steel not found");    

	FairGeoMedium* mAl     = gGeoMedia->getMedium("aluminium");
  if ( ! mAl ) Fatal("Main", "FairMedium Aluminium not found");
  gGeoBuild->createMedium(mAl);
  TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
  if ( ! pMedAl ) Fatal("Main", "Medium Aluminum not found");      
   
	FairGeoMedium* mAcrylic     = gGeoMedia->getMedium("acrylicPMMA");
  if ( ! mAcrylic ) Fatal("Main", "FairMedium Acrylic not found");
  gGeoBuild->createMedium(mAcrylic);
  TGeoMedium* pMedAcrylic = gGeoMan->GetMedium("acrylicPMMA");
  if ( ! pMedAcrylic ) Fatal("Main", "Medium Acrylic not found");      
   
  
  // --------------------------------------------------------------------------
  
  
  
  // --------------   Create geometry and top volume  ------------------------- 
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("CHERRYgeom");
  gTop = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(gTop);
  
  
  // --------------------------------------------------------------------------
  
  
  ConstructGeometry(pMedVac, pMedH2O, pMedAir, pMedSteel, pMedAl, pMedAcrylic); 	
  
  
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


void ConstructGeometry(TGeoMedium *pMedVac, TGeoMedium *pMedH2O, TGeoMedium *pMedAir, TGeoMedium *pMedSteel, TGeoMedium *pMedAl, TGeoMedium *pMedAcrylic)
{
  cout << endl;
  cout << "-I- Ensar::ConstructGeometry() "<< endl;
  cout << "-I- Ensar  HPGe detector "<< endl;
  cout << endl;
  
  
  // Defintion of the Mother Volume
  Double_t length = 2000.;
  TGeoShape *pCherryWorld = new TGeoBBox("CherryWorldBox",length/2.0,length/2.0,length/2.0);
  TGeoVolume* pWorld = new TGeoVolume("CtnWorld", pCherryWorld, pMedAir);
  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  gTop->AddNode(pWorld,0,pGlobalc); 
  //-----------------------------------------------
  

	// ----------------------------------------------
  // 1 Cherry tomato ACTIVE VOLUME
	// Cherry sphere out
	/*TGeoVolume *sphere_out = gGeoManager->MakeSphere("Sphere_out",pMedH2O,0.,1.5,0,180,0,360);
  sphere_out->SetFillColor(2);
  sphere_out->SetLineColor(2);
  sphere_out->SetTransparency(70);
  sphere_out->SetVisLeaves(kTRUE);

	// Cherry sphere in
	TGeoVolume *sphere_in = gGeoManager->MakeSphere("Sphere_in",pMedH2O,0.0,1.49,0,180,0,360);//100 micras skin
  sphere_in->SetFillColor(2);
  sphere_in->SetLineColor(2);
  sphere_in->SetTransparency(70);
	sphere_out->AddNode(sphere_in,1,new TGeoTranslation(0,0,0));
  sphere_in->SetVisLeaves(kTRUE);

	// ----------------------------------------------


	// ----------------------------------------------
	// 6 Cherries NO active
	// Cherry sphere out
	TGeoVolume *NoActive_sphere_out = gGeoManager->MakeSphere("NoActive_sphere_out",pMedH2O,0.,1.5,0,180,0,360);
  NoActive_sphere_out->SetFillColor(6);
  NoActive_sphere_out->SetLineColor(6);
  NoActive_sphere_out->SetTransparency(70);
  NoActive_sphere_out->SetVisLeaves(kTRUE);

	// Cherry sphere in
	TGeoVolume *NoActive_sphere_in = gGeoManager->MakeSphere("NoActive_sphere_in",pMedH2O,0.0,1.49,0,180,0,360);//100 micras skin
  NoActive_sphere_in->SetFillColor(6);
  NoActive_sphere_in->SetLineColor(6);
  NoActive_sphere_in->SetTransparency(70);
	NoActive_sphere_out->AddNode(NoActive_sphere_in,1,new TGeoTranslation(0,0,0));
  NoActive_sphere_in->SetVisLeaves(kTRUE);
	// ----------------------------------------------

	TGeoRotation *rot_1     = new TGeoRotation("rot_1",0,0,0);//Cherry tomato in the center
  TGeoCombiTrans *comb_1 = new TGeoCombiTrans("comb_1",0,0,58.5,rot_1);
  pWorld->AddNode(NoActive_sphere_out,1,comb_1); 

  TGeoRotation *rot_2     = new TGeoRotation("rot_2",0,0,0);
  TGeoCombiTrans *comb_2 = new TGeoCombiTrans("comb_2",0,3,58.5,rot_2);
  pWorld->AddNode(sphere_out,2,comb_2); 

  TGeoRotation *rot_3     = new TGeoRotation("rot_3",0,0,0);
  TGeoCombiTrans *comb_3 = new TGeoCombiTrans("comb_3",-2.598,1.5,58.5,rot_3);
  pWorld->AddNode(NoActive_sphere_out,3,comb_3); 

  TGeoRotation *rot_4     = new TGeoRotation("rot_4",0,0,0);
  TGeoCombiTrans *comb_4 = new TGeoCombiTrans("comb_4",-2.598,-1.5,58.5,rot_4);
  pWorld->AddNode(NoActive_sphere_out,4,comb_4); 

  TGeoRotation *rot_5     = new TGeoRotation("rot_5",0,0,0);
  TGeoCombiTrans *comb_5 = new TGeoCombiTrans("comb_5",0,-3,58.5,rot_5);
  pWorld->AddNode(NoActive_sphere_out,5,comb_5); 

  TGeoRotation *rot_6     = new TGeoRotation("rot_6",0,0,0);
  TGeoCombiTrans *comb_6 = new TGeoCombiTrans("comb_6",2.598,-1.5,58.5,rot_6);
  pWorld->AddNode(NoActive_sphere_out,6,comb_6); 

  TGeoRotation *rot_7     = new TGeoRotation("rot_7",0,0,0);
  TGeoCombiTrans *comb_7 = new TGeoCombiTrans("comb_7",2.598,1.5,58.5,rot_7);
  pWorld->AddNode(NoActive_sphere_out,7,comb_7);*/ 
	// ----------------------------------------------

	//Box of Water
	TGeoVolume *box_water = gGeoManager->MakeBox("Box_water", pMedH2O,4.5,4.5,5.);//9x9x3 box of water
	box_water->SetFillColor(7);
  box_water->SetLineColor(7);
  box_water->SetTransparency(70);
  box_water->SetVisLeaves(kTRUE);

	TGeoRotation *rot_water    = new TGeoRotation("rot_water",0,0,0);
  TGeoCombiTrans *comb_water = new TGeoCombiTrans("comb_water",0.,0.,58.5,rot_water);
  pWorld->AddNode(box_water,1,comb_water);

	//Source layer "Al"
	TGeoVolume *source_layer = gGeoManager->MakeBox("Source_layer", pMedAl,3.975,3.975,0.002);//20 micrometers
	source_layer->SetFillColor(9);
  source_layer->SetLineColor(9);
  source_layer->SetTransparency(70);
  source_layer->SetVisLeaves(kTRUE);
	
	TGeoRotation *rot_8    = new TGeoRotation("rot_8",0,0,0);
  TGeoCombiTrans *comb_8 = new TGeoCombiTrans("comb_8",0.,0.,0.5,rot_8);
  pWorld->AddNode(source_layer,1,comb_8);
  // ----------------------------------------------

	//Layers under the tomatoes
	//First Acrylic (PMMA) layer 
	TGeoVolume *first_acry_layer = gGeoManager->MakeBox("First_acry_layer", pMedAcrylic,15.,15.,0.5);
	first_acry_layer->SetFillColor(29);
  first_acry_layer->SetLineColor(29);
  first_acry_layer->SetTransparency(70);
  first_acry_layer->SetVisLeaves(kTRUE);
	
	//Other Acrylic (PMMA) layer 
	TGeoVolume *other_acry_layer = gGeoManager->MakeBox("Other_acry_layer", pMedAcrylic,20.,20.,0.5);
	other_acry_layer->SetFillColor(29);//5
  other_acry_layer->SetLineColor(29);
  other_acry_layer->SetTransparency(70);
  other_acry_layer->SetVisLeaves(kTRUE);
	
	//1st
	TGeoRotation *rot = new TGeoRotation("rot",0,0,0);
  TGeoCombiTrans *comb_9 = new TGeoCombiTrans("comb_9",0.,0.,60.5,rot);
  //pWorld->AddNode(first_acry_layer,1,comb_9);
	//2nd
  TGeoCombiTrans *comb_10 = new TGeoCombiTrans("comb_10",0.,0.,61.5,rot);
  //pWorld->AddNode(other_acry_layer,1,comb_10);
	//3rd
	TGeoCombiTrans *comb_11 = new TGeoCombiTrans("comb_11",0.,0.,62.5,rot);
  //pWorld->AddNode(other_acry_layer,2,comb_11);
	//4th
	TGeoCombiTrans *comb_12 = new TGeoCombiTrans("comb_12",0.,0.,63.5,rot);
  //pWorld->AddNode(other_acry_layer,3,comb_12);
	//5th
	TGeoCombiTrans *comb_13 = new TGeoCombiTrans("comb_13",0.,0.,64.5,rot);
  //pWorld->AddNode(other_acry_layer,4,comb_13);
  // ----------------------------------------------


	//Metal support
	TGeoVolume *metal_support = gGeoManager->MakeBox("Support", pMedSteel,20.,20.,1.65);
	metal_support->SetFillColor(12);
  metal_support->SetLineColor(12);
  metal_support->SetTransparency(70);
  metal_support->SetVisLeaves(kTRUE);

  TGeoCombiTrans *comb_14 = new TGeoCombiTrans("comb_14",0.,0.,66.65,rot);
  //pWorld->AddNode(metal_support,1,comb_14);
	// ----------------------------------------------
	


  
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

