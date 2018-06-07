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
	TGeoVolume *box_water = gGeoManager->MakeBox("Box_water", pMedH2O,4.5,4.5,0.05);//9x9x0.1 box of water
	box_water->SetFillColor(7);
  box_water->SetLineColor(7);
  box_water->SetTransparency(70);
  box_water->SetVisLeaves(kTRUE);

	TGeoRotation *rot_water    = new TGeoRotation("rot_water",0,0,0);
  /*TGeoCombiTrans *comb_water_1 = new TGeoCombiTrans("comb_water_1",0.,0.,54.,rot_water);
	TGeoCombiTrans *comb_water_2 = new TGeoCombiTrans("comb_water_2",0.,0.,55.,rot_water);
  TGeoCombiTrans *comb_water_3 = new TGeoCombiTrans("comb_water_3",0.,0.,56.,rot_water);
	TGeoCombiTrans *comb_water_4 = new TGeoCombiTrans("comb_water_4",0.,0.,57.,rot_water);
  TGeoCombiTrans *comb_water_5 = new TGeoCombiTrans("comb_water_5",0.,0.,58.,rot_water);
	TGeoCombiTrans *comb_water_6 = new TGeoCombiTrans("comb_water_6",0.,0.,59.,rot_water);
  TGeoCombiTrans *comb_water_7 = new TGeoCombiTrans("comb_water_7",0.,0.,60.,rot_water);
	TGeoCombiTrans *comb_water_8 = new TGeoCombiTrans("comb_water_8",0.,0.,61.,rot_water);
  TGeoCombiTrans *comb_water_9 = new TGeoCombiTrans("comb_water_9",0.,0.,62.,rot_water);
	TGeoCombiTrans *comb_water_10 = new TGeoCombiTrans("comb_water_10",0.,0.,63.,rot_water);
  pWorld->AddNode(box_water,1,comb_water_1);
  pWorld->AddNode(box_water,2,comb_water_2);
  pWorld->AddNode(box_water,3,comb_water_3);
  pWorld->AddNode(box_water,4,comb_water_4);
  pWorld->AddNode(box_water,5,comb_water_5);
  pWorld->AddNode(box_water,6,comb_water_6);
  pWorld->AddNode(box_water,7,comb_water_7);
  pWorld->AddNode(box_water,8,comb_water_8);
  pWorld->AddNode(box_water,9,comb_water_9);
  pWorld->AddNode(box_water,10,comb_water_10);*/

	TGeoCombiTrans *comb_water_1 = new TGeoCombiTrans("comb_water_1",0.,0.,53.5,rot_water);
	TGeoCombiTrans *comb_water_2 = new TGeoCombiTrans("comb_water_2",0.,0.,53.6,rot_water);
  TGeoCombiTrans *comb_water_3 = new TGeoCombiTrans("comb_water_3",0.,0.,53.7,rot_water);
	TGeoCombiTrans *comb_water_4 = new TGeoCombiTrans("comb_water_4",0.,0.,53.8,rot_water);
  TGeoCombiTrans *comb_water_5 = new TGeoCombiTrans("comb_water_5",0.,0.,53.9,rot_water);
	TGeoCombiTrans *comb_water_6 = new TGeoCombiTrans("comb_water_6",0.,0.,54.,rot_water);
  TGeoCombiTrans *comb_water_7 = new TGeoCombiTrans("comb_water_7",0.,0.,54.1,rot_water);
	TGeoCombiTrans *comb_water_8 = new TGeoCombiTrans("comb_water_8",0.,0.,54.2,rot_water);
  TGeoCombiTrans *comb_water_9 = new TGeoCombiTrans("comb_water_9",0.,0.,54.3,rot_water);
	TGeoCombiTrans *comb_water_10 = new TGeoCombiTrans("comb_water_10",0.,0.,54.4,rot_water);

	TGeoCombiTrans *comb_water_11 = new TGeoCombiTrans("comb_water_11",0.,0.,54.5,rot_water);
	TGeoCombiTrans *comb_water_12 = new TGeoCombiTrans("comb_water_12",0.,0.,54.6,rot_water);
  TGeoCombiTrans *comb_water_13 = new TGeoCombiTrans("comb_water_13",0.,0.,54.7,rot_water);
	TGeoCombiTrans *comb_water_14 = new TGeoCombiTrans("comb_water_14",0.,0.,54.8,rot_water);
  TGeoCombiTrans *comb_water_15 = new TGeoCombiTrans("comb_water_15",0.,0.,54.9,rot_water);
	TGeoCombiTrans *comb_water_16 = new TGeoCombiTrans("comb_water_16",0.,0.,55.,rot_water);
  TGeoCombiTrans *comb_water_17 = new TGeoCombiTrans("comb_water_17",0.,0.,55.1,rot_water);
	TGeoCombiTrans *comb_water_18 = new TGeoCombiTrans("comb_water_18",0.,0.,55.2,rot_water);
  TGeoCombiTrans *comb_water_19 = new TGeoCombiTrans("comb_water_19",0.,0.,55.3,rot_water);
	TGeoCombiTrans *comb_water_20 = new TGeoCombiTrans("comb_water_20",0.,0.,55.4,rot_water);

	TGeoCombiTrans *comb_water_21 = new TGeoCombiTrans("comb_water_21",0.,0.,55.5,rot_water);
	TGeoCombiTrans *comb_water_22 = new TGeoCombiTrans("comb_water_22",0.,0.,55.6,rot_water);
  TGeoCombiTrans *comb_water_23 = new TGeoCombiTrans("comb_water_23",0.,0.,55.7,rot_water);
	TGeoCombiTrans *comb_water_24 = new TGeoCombiTrans("comb_water_24",0.,0.,55.8,rot_water);
  TGeoCombiTrans *comb_water_25 = new TGeoCombiTrans("comb_water_25",0.,0.,55.9,rot_water);
	TGeoCombiTrans *comb_water_26 = new TGeoCombiTrans("comb_water_26",0.,0.,56.,rot_water);
  TGeoCombiTrans *comb_water_27 = new TGeoCombiTrans("comb_water_27",0.,0.,56.1,rot_water);
	TGeoCombiTrans *comb_water_28 = new TGeoCombiTrans("comb_water_28",0.,0.,56.2,rot_water);
  TGeoCombiTrans *comb_water_29 = new TGeoCombiTrans("comb_water_29",0.,0.,56.3,rot_water);
	TGeoCombiTrans *comb_water_30 = new TGeoCombiTrans("comb_water_30",0.,0.,56.4,rot_water);

	TGeoCombiTrans *comb_water_31 = new TGeoCombiTrans("comb_water_31",0.,0.,56.5,rot_water);
	TGeoCombiTrans *comb_water_32 = new TGeoCombiTrans("comb_water_32",0.,0.,56.6,rot_water);
  TGeoCombiTrans *comb_water_33 = new TGeoCombiTrans("comb_water_33",0.,0.,56.7,rot_water);
	TGeoCombiTrans *comb_water_34 = new TGeoCombiTrans("comb_water_34",0.,0.,56.8,rot_water);
  TGeoCombiTrans *comb_water_35 = new TGeoCombiTrans("comb_water_35",0.,0.,56.9,rot_water);
	TGeoCombiTrans *comb_water_36 = new TGeoCombiTrans("comb_water_36",0.,0.,57.,rot_water);
  TGeoCombiTrans *comb_water_37 = new TGeoCombiTrans("comb_water_37",0.,0.,57.1,rot_water);
	TGeoCombiTrans *comb_water_38 = new TGeoCombiTrans("comb_water_38",0.,0.,57.2,rot_water);
  TGeoCombiTrans *comb_water_39 = new TGeoCombiTrans("comb_water_39",0.,0.,57.3,rot_water);
	TGeoCombiTrans *comb_water_40 = new TGeoCombiTrans("comb_water_40",0.,0.,57.4,rot_water);

	TGeoCombiTrans *comb_water_41 = new TGeoCombiTrans("comb_water_41",0.,0.,57.5,rot_water);
	TGeoCombiTrans *comb_water_42 = new TGeoCombiTrans("comb_water_42",0.,0.,57.6,rot_water);
  TGeoCombiTrans *comb_water_43 = new TGeoCombiTrans("comb_water_43",0.,0.,57.7,rot_water);
	TGeoCombiTrans *comb_water_44 = new TGeoCombiTrans("comb_water_44",0.,0.,57.8,rot_water);
  TGeoCombiTrans *comb_water_45 = new TGeoCombiTrans("comb_water_45",0.,0.,57.9,rot_water);
	TGeoCombiTrans *comb_water_46 = new TGeoCombiTrans("comb_water_46",0.,0.,58.,rot_water);
  TGeoCombiTrans *comb_water_47 = new TGeoCombiTrans("comb_water_47",0.,0.,58.1,rot_water);
	TGeoCombiTrans *comb_water_48 = new TGeoCombiTrans("comb_water_48",0.,0.,58.2,rot_water);
  TGeoCombiTrans *comb_water_49 = new TGeoCombiTrans("comb_water_49",0.,0.,58.3,rot_water);
	TGeoCombiTrans *comb_water_50 = new TGeoCombiTrans("comb_water_50",0.,0.,58.4,rot_water);

	TGeoCombiTrans *comb_water_51 = new TGeoCombiTrans("comb_water_51",0.,0.,58.5,rot_water);
	TGeoCombiTrans *comb_water_52 = new TGeoCombiTrans("comb_water_52",0.,0.,58.6,rot_water);
  TGeoCombiTrans *comb_water_53 = new TGeoCombiTrans("comb_water_53",0.,0.,58.7,rot_water);
	TGeoCombiTrans *comb_water_54 = new TGeoCombiTrans("comb_water_54",0.,0.,58.8,rot_water);
  TGeoCombiTrans *comb_water_55 = new TGeoCombiTrans("comb_water_55",0.,0.,58.9,rot_water);
	TGeoCombiTrans *comb_water_56 = new TGeoCombiTrans("comb_water_56",0.,0.,59.,rot_water);
  TGeoCombiTrans *comb_water_57 = new TGeoCombiTrans("comb_water_57",0.,0.,59.1,rot_water);
	TGeoCombiTrans *comb_water_58 = new TGeoCombiTrans("comb_water_58",0.,0.,59.2,rot_water);
  TGeoCombiTrans *comb_water_59 = new TGeoCombiTrans("comb_water_59",0.,0.,59.3,rot_water);
	TGeoCombiTrans *comb_water_60 = new TGeoCombiTrans("comb_water_60",0.,0.,59.4,rot_water);

	TGeoCombiTrans *comb_water_61 = new TGeoCombiTrans("comb_water_61",0.,0.,59.5,rot_water);
	TGeoCombiTrans *comb_water_62 = new TGeoCombiTrans("comb_water_62",0.,0.,59.6,rot_water);
  TGeoCombiTrans *comb_water_63 = new TGeoCombiTrans("comb_water_63",0.,0.,59.7,rot_water);
	TGeoCombiTrans *comb_water_64 = new TGeoCombiTrans("comb_water_64",0.,0.,59.8,rot_water);
  TGeoCombiTrans *comb_water_65 = new TGeoCombiTrans("comb_water_65",0.,0.,59.9,rot_water);
	TGeoCombiTrans *comb_water_66 = new TGeoCombiTrans("comb_water_66",0.,0.,60.,rot_water);
  TGeoCombiTrans *comb_water_67 = new TGeoCombiTrans("comb_water_67",0.,0.,60.1,rot_water);
	TGeoCombiTrans *comb_water_68 = new TGeoCombiTrans("comb_water_68",0.,0.,60.2,rot_water);
  TGeoCombiTrans *comb_water_69 = new TGeoCombiTrans("comb_water_69",0.,0.,60.3,rot_water);
	TGeoCombiTrans *comb_water_70 = new TGeoCombiTrans("comb_water_70",0.,0.,60.4,rot_water);

	TGeoCombiTrans *comb_water_71 = new TGeoCombiTrans("comb_water_71",0.,0.,60.5,rot_water);
	TGeoCombiTrans *comb_water_72 = new TGeoCombiTrans("comb_water_72",0.,0.,60.6,rot_water);
  TGeoCombiTrans *comb_water_73 = new TGeoCombiTrans("comb_water_73",0.,0.,60.7,rot_water);
	TGeoCombiTrans *comb_water_74 = new TGeoCombiTrans("comb_water_74",0.,0.,60.8,rot_water);
  TGeoCombiTrans *comb_water_75 = new TGeoCombiTrans("comb_water_75",0.,0.,60.9,rot_water);
	TGeoCombiTrans *comb_water_76 = new TGeoCombiTrans("comb_water_76",0.,0.,61.,rot_water);
  TGeoCombiTrans *comb_water_77 = new TGeoCombiTrans("comb_water_77",0.,0.,61.1,rot_water);
	TGeoCombiTrans *comb_water_78 = new TGeoCombiTrans("comb_water_78",0.,0.,61.2,rot_water);
  TGeoCombiTrans *comb_water_79 = new TGeoCombiTrans("comb_water_79",0.,0.,61.3,rot_water);
	TGeoCombiTrans *comb_water_80 = new TGeoCombiTrans("comb_water_80",0.,0.,61.4,rot_water);

	TGeoCombiTrans *comb_water_81 = new TGeoCombiTrans("comb_water_81",0.,0.,61.5,rot_water);
	TGeoCombiTrans *comb_water_82 = new TGeoCombiTrans("comb_water_82",0.,0.,61.6,rot_water);
  TGeoCombiTrans *comb_water_83 = new TGeoCombiTrans("comb_water_83",0.,0.,61.7,rot_water);
	TGeoCombiTrans *comb_water_84 = new TGeoCombiTrans("comb_water_84",0.,0.,61.8,rot_water);
  TGeoCombiTrans *comb_water_85 = new TGeoCombiTrans("comb_water_85",0.,0.,61.9,rot_water);
	TGeoCombiTrans *comb_water_86 = new TGeoCombiTrans("comb_water_86",0.,0.,62.,rot_water);
  TGeoCombiTrans *comb_water_87 = new TGeoCombiTrans("comb_water_87",0.,0.,62.1,rot_water);
	TGeoCombiTrans *comb_water_88 = new TGeoCombiTrans("comb_water_88",0.,0.,62.2,rot_water);
  TGeoCombiTrans *comb_water_89 = new TGeoCombiTrans("comb_water_89",0.,0.,62.3,rot_water);
	TGeoCombiTrans *comb_water_90 = new TGeoCombiTrans("comb_water_90",0.,0.,62.4,rot_water);

	TGeoCombiTrans *comb_water_91 = new TGeoCombiTrans("comb_water_91",0.,0.,62.5,rot_water);
	TGeoCombiTrans *comb_water_92 = new TGeoCombiTrans("comb_water_92",0.,0.,62.6,rot_water);
  TGeoCombiTrans *comb_water_93 = new TGeoCombiTrans("comb_water_93",0.,0.,62.7,rot_water);
	TGeoCombiTrans *comb_water_94 = new TGeoCombiTrans("comb_water_94",0.,0.,62.8,rot_water);
  TGeoCombiTrans *comb_water_95 = new TGeoCombiTrans("comb_water_95",0.,0.,62.9,rot_water);
	TGeoCombiTrans *comb_water_96 = new TGeoCombiTrans("comb_water_96",0.,0.,63.,rot_water);
  TGeoCombiTrans *comb_water_97 = new TGeoCombiTrans("comb_water_97",0.,0.,63.1,rot_water);
	TGeoCombiTrans *comb_water_98 = new TGeoCombiTrans("comb_water_98",0.,0.,63.2,rot_water);
  TGeoCombiTrans *comb_water_99 = new TGeoCombiTrans("comb_water_99",0.,0.,63.3,rot_water);
	TGeoCombiTrans *comb_water_100 = new TGeoCombiTrans("comb_water_100",0.,0.,63.4,rot_water);

  pWorld->AddNode(box_water,1,comb_water_1);
  pWorld->AddNode(box_water,2,comb_water_2);
  pWorld->AddNode(box_water,3,comb_water_3);
  pWorld->AddNode(box_water,4,comb_water_4);
  pWorld->AddNode(box_water,5,comb_water_5);
  pWorld->AddNode(box_water,6,comb_water_6);
  pWorld->AddNode(box_water,7,comb_water_7);
  pWorld->AddNode(box_water,8,comb_water_8);
  pWorld->AddNode(box_water,9,comb_water_9);
  pWorld->AddNode(box_water,10,comb_water_10);

  pWorld->AddNode(box_water,11,comb_water_11);
  pWorld->AddNode(box_water,12,comb_water_12);
  pWorld->AddNode(box_water,13,comb_water_13);
  pWorld->AddNode(box_water,14,comb_water_14);
  pWorld->AddNode(box_water,15,comb_water_15);
  pWorld->AddNode(box_water,16,comb_water_16);
  pWorld->AddNode(box_water,17,comb_water_17);
  pWorld->AddNode(box_water,18,comb_water_18);
  pWorld->AddNode(box_water,19,comb_water_19);
  pWorld->AddNode(box_water,20,comb_water_20);

  pWorld->AddNode(box_water,21,comb_water_21);
  pWorld->AddNode(box_water,22,comb_water_22);
  pWorld->AddNode(box_water,23,comb_water_23);
  pWorld->AddNode(box_water,24,comb_water_24);
  pWorld->AddNode(box_water,25,comb_water_25);
  pWorld->AddNode(box_water,26,comb_water_26);
  pWorld->AddNode(box_water,27,comb_water_27);
  pWorld->AddNode(box_water,28,comb_water_28);
  pWorld->AddNode(box_water,29,comb_water_29);
  pWorld->AddNode(box_water,30,comb_water_30);

  pWorld->AddNode(box_water,31,comb_water_31);
  pWorld->AddNode(box_water,32,comb_water_32);
  pWorld->AddNode(box_water,33,comb_water_33);
  pWorld->AddNode(box_water,34,comb_water_34);
  pWorld->AddNode(box_water,35,comb_water_35);
  pWorld->AddNode(box_water,36,comb_water_36);
  pWorld->AddNode(box_water,37,comb_water_37);
  pWorld->AddNode(box_water,38,comb_water_38);
  pWorld->AddNode(box_water,39,comb_water_39);
  pWorld->AddNode(box_water,40,comb_water_40);

  pWorld->AddNode(box_water,41,comb_water_41);
  pWorld->AddNode(box_water,42,comb_water_42);
  pWorld->AddNode(box_water,43,comb_water_43);
  pWorld->AddNode(box_water,44,comb_water_44);
  pWorld->AddNode(box_water,45,comb_water_45);
  pWorld->AddNode(box_water,46,comb_water_46);
  pWorld->AddNode(box_water,47,comb_water_47);
  pWorld->AddNode(box_water,48,comb_water_48);
  pWorld->AddNode(box_water,49,comb_water_49);
  pWorld->AddNode(box_water,50,comb_water_50);

  pWorld->AddNode(box_water,51,comb_water_51);
  pWorld->AddNode(box_water,52,comb_water_52);
  pWorld->AddNode(box_water,53,comb_water_53);
  pWorld->AddNode(box_water,54,comb_water_54);
  pWorld->AddNode(box_water,55,comb_water_55);
  pWorld->AddNode(box_water,56,comb_water_56);
  pWorld->AddNode(box_water,57,comb_water_57);
  pWorld->AddNode(box_water,58,comb_water_58);
  pWorld->AddNode(box_water,59,comb_water_59);
  pWorld->AddNode(box_water,60,comb_water_60);

  pWorld->AddNode(box_water,61,comb_water_61);
  pWorld->AddNode(box_water,62,comb_water_62);
  pWorld->AddNode(box_water,63,comb_water_63);
  pWorld->AddNode(box_water,64,comb_water_64);
  pWorld->AddNode(box_water,65,comb_water_65);
  pWorld->AddNode(box_water,66,comb_water_66);
  pWorld->AddNode(box_water,67,comb_water_67);
  pWorld->AddNode(box_water,68,comb_water_68);
  pWorld->AddNode(box_water,69,comb_water_69);
  pWorld->AddNode(box_water,70,comb_water_70);

  pWorld->AddNode(box_water,71,comb_water_71);
  pWorld->AddNode(box_water,72,comb_water_72);
  pWorld->AddNode(box_water,73,comb_water_73);
  pWorld->AddNode(box_water,74,comb_water_74);
  pWorld->AddNode(box_water,75,comb_water_75);
  pWorld->AddNode(box_water,76,comb_water_76);
  pWorld->AddNode(box_water,77,comb_water_77);
  pWorld->AddNode(box_water,78,comb_water_78);
  pWorld->AddNode(box_water,79,comb_water_79);
  pWorld->AddNode(box_water,80,comb_water_80);

  pWorld->AddNode(box_water,81,comb_water_81);
  pWorld->AddNode(box_water,82,comb_water_82);
  pWorld->AddNode(box_water,83,comb_water_83);
  pWorld->AddNode(box_water,84,comb_water_84);
  pWorld->AddNode(box_water,85,comb_water_85);
  pWorld->AddNode(box_water,86,comb_water_86);
  pWorld->AddNode(box_water,87,comb_water_87);
  pWorld->AddNode(box_water,88,comb_water_88);
  pWorld->AddNode(box_water,89,comb_water_89);
  pWorld->AddNode(box_water,90,comb_water_90);

  pWorld->AddNode(box_water,91,comb_water_91);
  pWorld->AddNode(box_water,92,comb_water_92);
  pWorld->AddNode(box_water,93,comb_water_93);
  pWorld->AddNode(box_water,94,comb_water_94);
  pWorld->AddNode(box_water,95,comb_water_95);
  pWorld->AddNode(box_water,96,comb_water_96);
  pWorld->AddNode(box_water,97,comb_water_97);
  pWorld->AddNode(box_water,98,comb_water_98);
  pWorld->AddNode(box_water,99,comb_water_99);
  pWorld->AddNode(box_water,100,comb_water_100);

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

