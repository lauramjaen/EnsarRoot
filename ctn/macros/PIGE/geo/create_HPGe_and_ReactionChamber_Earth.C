///////////////////////////////////////////////////////////////////////
////																				       	
////	--- Simulation of the CTN/IST Lisbon ---							
////
////	This macro recreates HPGe detector at --º w.r.t. z axis 
////	and the Reaction Chamber in the center (LaboratoryRS)
////
///////////////////////////////////////////////////////////////////////

#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"
//#include <AxisAngle.h>


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

void ConstructGeometry(TGeoMedium *pMed, TGeoMedium *pMedAl, TGeoMedium *pMedSteel, TGeoMedium *pMedGe, TGeoMedium *pMedLi, TGeoMedium *pMedAir, TGeoMedium *pMedSi);


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
  TString geoFileName = geoPath + "/ctn/geometry/HPGe_and_ReacChamber";
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



  FairGeoMedium* mSi      = gGeoMedia->getMedium("silicon");
  if ( ! mSi ) Fatal("Main", "FairMedium Si not found");
  gGeoBuild->createMedium(mSi);
  TGeoMedium* pMedSi = gGeoMan->GetMedium("silicon");
  if ( ! pMedSi ) Fatal("Main", "Medium Si not found");   
  
  // --------------------------------------------------------------------------
  
  
  
  // --------------   Create geometry and top volume  ------------------------- 
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("HPGEgeom");
  gTop = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(gTop);
  
  
  // --------------------------------------------------------------------------
  
  
  ConstructGeometry(pMedVac, pMedAl, pMedSteel, pMedGe, pMedLi, pMedAir, pMedSi); 	
  
  
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






void ConstructGeometry(TGeoMedium *pMedVac, TGeoMedium *pMedAl, TGeoMedium *pMedSteel, TGeoMedium *pMedGe, TGeoMedium *pMedLi, TGeoMedium *pMedAir, TGeoMedium *pMedSi)
{
  cout << endl;
  cout << "-I- Ensar::ConstructGeometry() "<< endl;
  cout << "-I- Ensar  HPGe detector "<< endl;
  cout << endl;
  
  
  // Defintion of the Mother Volume
  Double_t length = 100000.;
  TGeoShape *pCtnWorld = new TGeoBBox("CtnWorldBox",length/2.0,length/2.0,length/2.0);
  TGeoVolume* pWorld = new TGeoVolume("CtnWorld", pCtnWorld, pMedAir);
  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  gTop->AddNode(pWorld,0,pGlobalc);
  
  //-----------------------------------------------
  
  // ----------------------------------------------
  // Auxiliar vacuum volume holding all important volumes as daugthers
  // The easiest way to rotate all volumes to the final position
  TGeoVolume *main_tube = gGeoManager->MakeTube("main_tube",pMedVac,0.0,4.035,3.505);
  main_tube->SetFillColor(5);
  main_tube->SetLineColor(5);
  main_tube->SetTransparency(70);
  main_tube->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
  // ----------------------------------------------
  // First Aluminum wrapping
  TGeoVolume *al_tub1 = gGeoManager->MakeTube("AL_TUB1", pMedAl, 0.0, 3.75, 0.05);
  al_tub1->SetFillColor(18);
  al_tub1->SetLineColor(18);
  main_tube->AddNode(al_tub1,1,new TGeoTranslation(0,0,-3.255));
  al_tub1->SetVisLeaves(kTRUE);
  TGeoVolume *al_tub2 = gGeoManager->MakeTube("AL_TUB2", pMedAl, 3.65, 3.75, 3.355);
  al_tub2->SetFillColor(18);
  al_tub2->SetLineColor(18);
  main_tube->AddNode(al_tub2,1,new TGeoTranslation(0,0,0.15));
  al_tub2->SetVisLeaves(kTRUE);
  // ----------------------------------------------

  // ----------------------------------------------
  // Second Aluminum wrapping (4mm vacuum between both wrappings)
  TGeoVolume *al_tub3 = gGeoManager->MakeTube("AL_TUB3", pMedAl, 0.0, 3.25, 0.025);
  al_tub3->SetFillColor(14);
  al_tub3->SetLineColor(14);
  main_tube->AddNode(al_tub3,1,new TGeoTranslation(0,0,-2.78));
  al_tub3->SetVisLeaves(kTRUE);
  TGeoVolume *al_tub4 = gGeoManager->MakeTube("AL_TUB4", pMedAl, 3.20, 3.25, 3.13);
  al_tub4->SetFillColor(14);
  al_tub4->SetLineColor(14);
  main_tube->AddNode(al_tub4,1,new TGeoTranslation(0,0,0.375));
  al_tub4->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
  // ----------------------------------------------
  // Lithium side-contacts
  TGeoVolume *li_tub1 = gGeoManager->MakeTube("LI_TUB1", pMedLi, 3.13, 3.20, 3.13);
  li_tub1->SetFillColor(4);
  li_tub1->SetLineColor(4);
  main_tube->AddNode(li_tub1,1,new TGeoTranslation(0,0,0.375));
  li_tub1->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
  // ----------------------------------------------
  //Ge inactive layer: 2mm
  TGeoVolume *ge_inac_tub1 = gGeoManager->MakeTube("GE_INAC_TUB1", pMedGe, 0.0, 3.13, 0.0875);
  ge_inac_tub1->SetFillColor(50);
  ge_inac_tub1->SetLineColor(50);
  main_tube->AddNode(ge_inac_tub1,1,new TGeoTranslation(0,0,-2.6675));
  ge_inac_tub1->SetVisLeaves(kTRUE);
  TGeoVolume *ge_inac_tub2 = gGeoManager->MakeTube("GE_INAC_TUB2", pMedGe, 3.0, 3.13, 3.03);
  ge_inac_tub2->SetFillColor(50);
  ge_inac_tub2->SetLineColor(50);
  main_tube->AddNode(ge_inac_tub2,1,new TGeoTranslation(0,0,0.475));
  ge_inac_tub2->SetVisLeaves(kTRUE);
  // ----------------------------------------------

  // ----------------------------------------------
  //Ge active volume
  TGeoVolume *ge_tub3 = gGeoManager->MakeTube("GE_TUB3", pMedGe, 0.0, 3.0, 0.575);
  ge_tub3->SetFillColor(30);
  ge_tub3->SetLineColor(30);
  main_tube->AddNode(ge_tub3,1,new TGeoTranslation(0,0,-2.005));
  ge_tub3->SetVisLeaves(kTRUE);
  TGeoVolume *ge_tub4 = gGeoManager->MakeTube("GE_TUB4", pMedGe, 0.45, 3.0, 2.455);
  ge_tub4->SetFillColor(30);
  ge_tub4->SetLineColor(30);
  main_tube->AddNode(ge_tub4,1,new TGeoTranslation(0,0,1.05));
  ge_tub4->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
  
  // ----------------------------------------------
  // detector at 90 degrees
  //TGeoRotation *rot     = new TGeoRotation("rot",0,220,0);//change to 130º
  //TGeoCombiTrans *comb = new TGeoCombiTrans("comb",0,6.7833,-5.6919,rot);//change to 5.55cm
  //pWorld->AddNode(main_tube,1,comb); 
  // ----------------------------------------------
  
  
  //-------------------------------------------------------------------------------------------------------------------
  //Create random REACTION CHAMBER 
  TGeoVolume *main_chamber = gGeoManager->MakeTube("main_chamber",pMedAir,0.0,11.5,13.5);//"name",pMed,rmin,rmax,height/2
  main_chamber->SetFillColor(14);
  main_chamber->SetLineColor(14);
  main_chamber->SetTransparency(70);
  main_chamber->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
  // ----------------------------------------------
  // Steel tube of the chamber (walls)
  TGeoVolume *steel_chamber1 = gGeoManager->MakeTube("steel_chamber1", pMedSteel, 9.5, 10., 11.5);
  steel_chamber1->SetFillColor(14);
  steel_chamber1->SetLineColor(14);
  main_chamber->AddNode(steel_chamber1,1,new TGeoTranslation(0,0,0));
  steel_chamber1->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
  // ----------------------------------------------
  // Vacuum inside the reaction chamber
  TGeoVolume *vacuum_chamber = gGeoManager->MakeTube("vacuum_chamber", pMedVac, 0.0, 9.499, 11.5);
  vacuum_chamber->SetFillColor(11);
  vacuum_chamber->SetLineColor(11);
  main_chamber->AddNode(vacuum_chamber,1,new TGeoTranslation(0,0,0));
  vacuum_chamber->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
  // ----------------------------------------------
  // Steel basis of the chamber
  TGeoVolume *steel_base1 = gGeoManager->MakeTube("steel_base1", pMedSteel, 0.0, 11.5, 1.0);
  steel_base1->SetFillColor(14);
  steel_base1->SetLineColor(14);
  main_chamber->AddNode(steel_base1,1,new TGeoTranslation(0,0,12.5));
  main_chamber->AddNode(steel_base1,2,new TGeoTranslation(0,0,-12.5));
  steel_base1->SetVisLeaves(kTRUE);
  // ----------------------------------------------
  
	// ----------------------------------------------
  // detector at 90 or 1t 130 degrees and at 5.55cm from the source
  TGeoRotation *rot     = new TGeoRotation("rot",130,-90,0);//now at 130º or at 90º put(90,-90,0) okk
  TGeoCombiTrans *comb = new TGeoCombiTrans("comb",-6.93653,-5.82044,0.,rot);//at 130º or at 90º put(-9.055,0.,0.,rot) okk
	main_chamber->AddNodeOverlap(main_tube,1,comb);//AddNodeOverlap

	
  // ----------------------------------------------

  // ----------------------------------------------
  // reaction chamber in the origin 
  TGeoRotation *rot_ch     = new TGeoRotation("rot_ch",0,90,0);
  TGeoCombiTrans *comb_ch = new TGeoCombiTrans("comb_ch",0,0,0,rot_ch);//
  pWorld->AddNode(main_chamber,1,comb_ch);
  // ----------------------------------------------

	// ----------------------------------------------
  // Silicon Earth
 	//TGeoVolume* earth = gGeoManager->MakeBox("earth", pMedSi, 10, 50, 10);//100cm
	//TGeoVolume* earth = gGeoManager->MakeBox("earth", pMedSi, 10, 5, 10);// 10cm
	//TGeoVolume* earth = gGeoManager->MakeBox("earth", pMedSi, 10, 10, 10);// 20cm
	TGeoVolume* earth = gGeoManager->MakeBox("earth", pMedSi, 10, 6.10, 10);// 12.2cm
	//TGeoVolume* earth = gGeoManager->MakeBox("earth", pMedSi, 10, 7.5, 10);// 12.5cm
  earth->SetFillColor(18);
	earth->SetLineColor(18);

 	TGeoRotation *rot_earth     = new TGeoRotation("rot_earth",0,0,0);
  //TGeoCombiTrans *comb_earth  = new TGeoCombiTrans("comb_earth",-10,-5026,-10,rot_earth);//for 10m
	//TGeoCombiTrans *comb_earth  = new TGeoCombiTrans("comb_earth",-10,-72.6,-10,rot_earth);//("comb_earth",-10,-52.6,-10,rot_earth);//for 100cm
	//TGeoCombiTrans *comb_earth  = new TGeoCombiTrans("comb_earth",-10,-26,-10,rot_earth);//for 10cm 
	TGeoCombiTrans *comb_earth  = new TGeoCombiTrans("comb_earth",-10,-27.1,-10,rot_earth);// 12.2cm
	//TGeoCombiTrans *comb_earth  = new TGeoCombiTrans("comb_earth",-10,-28.5,-10,rot_earth);
  pWorld->AddNode(earth,1,comb_earth);
	
  
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

