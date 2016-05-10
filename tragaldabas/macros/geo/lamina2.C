#include <iomanip>
#include <iostream>
#include "TGeoManager.h"
#include "TMath.h"

// Create Matrix Unity
TGeoRotation *fGlobalRot = new TGeoRotation();

// Create a null translation
TGeoTranslation *fGlobalTrans = new TGeoTranslation();
fGlobalTrans->SetTranslation(0.0,0.0,0.0);
TGeoRotation *fRefRot = NULL;

TGeoManager*   gGeoMan           = NULL;

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

// -------   Main function for creating the geo   ------------------------------
void lamina2(const char* geoTag)
{
  // Usage: select the geoTag when calling the macro for a new CALIFA geo
  // Possible geoTag values: 
  // geoTag           - Short description 
  // 0                - Initial Tragaldabas basic geometry


  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/tragaldabas/geometry/media_ensar.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // --------------------------------------------------------------------------

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/tragaldabas/geometry/lamina2_";
  geoFileName = geoFileName + geoTag + ".geo.root";
  // --------------------------------------------------------------------------

  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium* mAir      = geoMedia->getMedium("Air");
  if ( ! mAir ) Fatal("Main", "FairMedium Air not found");
  geoBuild->createMedium(mAir);
  TGeoMedium* pAirMedium = gGeoMan->GetMedium("Air");
  if ( ! pAirMedium ) Fatal("Main", "Medium Air not found");

  FairGeoMedium* mAluminium      = geoMedia->getMedium("aluminium");
  if ( ! mAluminium ) Fatal("Main", "FairMedium Aluminium not found");
  geoBuild->createMedium(mAluminium);
  TGeoMedium* pAluminiumMedium = gGeoMan->GetMedium("aluminium");
  if ( ! pAluminiumMedium ) Fatal("Main", "Medium Aluminium not found");

  FairGeoMedium* mCopper      = geoMedia->getMedium("copper");
  if ( ! mCopper ) Fatal("Main", "FairMedium Copper not found");
  geoBuild->createMedium(mCopper);
  TGeoMedium* pCopperMedium = gGeoMan->GetMedium("copper");
  if ( ! pCopperMedium ) Fatal("Main", "Medium Copper not found");

  FairGeoMedium* mCar      = geoMedia->getMedium("CarbonFibre");
  if ( ! mCar ) Fatal("Main", "FairMedium CarbonFibre not found");
  geoBuild->createMedium(mCar);
  TGeoMedium* pCarbonFibreMedium = gGeoMan->GetMedium("CarbonFibre");
  if ( ! pCarbonFibreMedium ) Fatal("Main", "Medium CarbonFibre not found");

  FairGeoMedium* mLead      = geoMedia->getMedium("lead");
  if ( ! mLead ) Fatal("Main", "FairMedium lead not found");
  geoBuild->createMedium(mLead);
  TGeoMedium* pLeadMedium = gGeoMan->GetMedium("lead");
  if ( ! pLeadMedium ) Fatal("Main", "Medium lead not found");


//TODO: create metacrilate!!!! (create in media_ensar.geo)
  //FairGeoMedium* mMetacrilate      = geoMedia->getMedium("metacrilate");
  //if ( ! mMetacrilate ) Fatal("Main", "FairMedium Metacrilate not found");
  //geoBuild->createMedium(mMetacrilate);
  //TGeoMedium* pMetacrilateMedium = gGeoMan->GetMedium("Metacrilate");
  //if ( ! pMetacrilateMedium ) Fatal("Main", "Medium Metacrilate not found");

  FairGeoMedium* mRPCGlass      = geoMedia->getMedium("RPCglass");
  if ( ! mRPCGlass ) Fatal("Main", "FairMedium RPCGlass not found");
  geoBuild->createMedium(mRPCGlass);
  TGeoMedium* pRPCGlassMedium = gGeoMan->GetMedium("RPCglass");
  if ( ! pRPCGlassMedium ) Fatal("Main", "Medium RPCGlass not found");

//TODO: change to RPCGasR134A material  (create in media_ensar.geo)
  FairGeoMedium* mRPCGasR134A      = geoMedia->getMedium("RPCgas");
  if ( ! mRPCGasR134A ) Fatal("Main", "FairMedium RPCGasR134A not found");
  geoBuild->createMedium(mRPCGasR134A);
  TGeoMedium* pRPCGasR134AMedium = gGeoMan->GetMedium("RPCgas");
  if ( ! pRPCGasR134AMedium ) Fatal("Main", "Medium RPCGasR134A not found");
 
  FairGeoMedium* mTfl      = geoMedia->getMedium("Tefflon");
  if ( ! mTfl ) Fatal("Main", "FairMedium Tefflon not found");
  geoBuild->createMedium(mTfl);
  TGeoMedium* pWrappingMedium = gGeoMan->GetMedium("Tefflon");
  if ( ! pWrappingMedium ) Fatal("Main", "Medium Tefflon not found");
  // --------------------------------------------------------------------------

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("TragaldabasGeom");
  TGeoVolume* top = new TGeoVolumeAssembly("TRA_World");
  gGeoMan->SetTopVolume(top);
  // --------------------------------------------------------------------------

  //WORLD
  //TGeoVolume *pAWorld  =  top;
  
  // Defintion of the Mother Volume (cube of 2.0 meters side)
  Double_t length = 300.;
  TGeoShape *pTragaShape = new TGeoBBox("Traga_boxOut",
  		length/2.0,length/2.0,length/2.0);
  TGeoVolume* pTragaWorld = new TGeoVolume("TragaWorld",pTragaShape, pAirMedium);
  
  TGeoCombiTrans *t0 = new TGeoCombiTrans();
  TGeoCombiTrans *pGlobalc = GetGlobalPosition(t0);
  
  // add the cube (pTragaWorld) as Mother Volume
  top->AddNode(pTragaWorld, 0, pGlobalc);

//TODO: Check the X-Y dimensions for all the objects defined below!!

  //MODULE KeepIn Volume, 1 cm larger in each dimension than aluminum box
  TGeoVolume *RPC_KeepInVolume=gGeoManager->MakeBox("RPC_KeepInVolume", pAirMedium,
  		166.0/2,129.5/2,3.6/2); 
  RPC_KeepInVolume->SetVisibility(kFALSE); //invisible KIV

TGeoVolume *RPC_Lead=gGeoManager->MakeBox("RPC_Lead", pLeadMedium,
  		10.0/2,10.0/2,10.0/2); 
  TGeoRotation *rotUni = new TGeoRotation(); //unitary rotation  

//  pTragaWorld->AddNode(RPC_KeepInVolume,0,new TGeoCombiTrans(0.,0.,-150.,rotUni)); 
//  pTragaWorld->AddNode(RPC_KeepInVolume,1,new TGeoCombiTrans(0.,0.,-100.,rotUni));
  pTragaWorld->AddNode(RPC_Lead,0,new TGeoCombiTrans(0.,0.,0.,rotUni));
  pTragaWorld->AddNode(RPC_KeepInVolume,0,new TGeoCombiTrans(0.,0.,150.,rotUni)); 
//  pTragaWorld->AddNode(RPC_KeepInVolume,3,new TGeoCombiTrans(0.,0.,100.,rotUni)); 

  //Aluminum box containing the whole chamber, 3mm thick, laterals 5mm thick
  TGeoVolume *RPC_AluBox=gGeoManager->MakeBox("RPC_AluBox", pAluminiumMedium,
  		165.0/2,128.5/2,2.6/2); 
  RPC_KeepInVolume->AddNode(RPC_AluBox,0,new TGeoCombiTrans(0.,0.,0.,rotUni));
  RPC_AluBox->SetVisibility(kTRUE); //invisible KIV
  RPC_AluBox->SetLineColor(kRed); 

  TGeoVolume *RPC_AluInBox=gGeoManager->MakeBox("RPC_AluInBox", pAirMedium,
  		164.0/2,127.5/2,2.0/2); 
  RPC_AluBox->AddNode(RPC_AluInBox,0,new TGeoCombiTrans(0.,0.,0,rotUni)); 
  RPC_AluInBox->SetVisibility(kFALSE); //invisible interior volume (air)

  //Foam (box) between aluminium box and copper PCB 
  //TODO! Change to a proper Medium: needed the density and composition 
  TGeoVolume *RPC_FoamBox=gGeoManager->MakeBox("RPC_FoamBox", pAirMedium, 	
  		163.0/2,126.5/2,0.87/2); 
  RPC_AluInBox->AddNode(RPC_FoamBox,0,new TGeoCombiTrans(0.,0.,0.565,rotUni)); 
  RPC_FoamBox->SetLineColor(kYellow); 

  //copper (box) between foam box and the PCB
  //NOTE: It is not divided at this stage. It is also NOT the sensitive
  //material, but signals are going to be taken from the gas, dividing it
  //with the proper copper-pad segmentation.
  TGeoVolume *RPC_CopperBox=gGeoManager->MakeBox("RPC_CopperBox", pCopperMedium, 	
  		163.0/2,126.5/2,0.003/2); 
  RPC_AluInBox->AddNode(RPC_CopperBox,0,new TGeoCombiTrans(0.,0.,0.1285,rotUni)); 
  RPC_CopperBox->SetLineColor(kRed); 

 //PCB (box) between copper and the metacrilate
  //NOTE (TODO?) Using Carbon Fibre material for the PCB. Ask for more info.
  TGeoVolume *RPC_PCBBox=gGeoManager->MakeBox("RPC_PCBBox", pCarbonFibreMedium, 	
  		163.0/2,126.5/2,0.157/2); 
  RPC_AluInBox->AddNode(RPC_PCBBox,0,new TGeoCombiTrans(0.,0.,0.0485,rotUni)); 
  RPC_PCBBox->SetLineColor(kGreen); 


  //Metacrilate box containing gas and glass
  //TODO: change the medium to metacrilate
  TGeoVolume *RPC_MetaBox=gGeoManager->MakeBox("RPC_MetaBox", pRPCGlassMedium, 	
  		163.0/2,126.5/2,0.97/2); 
  RPC_AluInBox->AddNode(RPC_MetaBox,0,new TGeoCombiTrans(0.,0.,-0.515,rotUni)); 
  RPC_MetaBox->SetLineColor(kWhite); 

  TGeoVolume *RPC_MetaInBox=gGeoManager->MakeBox("RPC_MetaInBox", pRPCGasR134AMedium, 	
  		160.0/2,123.5/2,0.77/2); 
  RPC_MetaBox->AddNode(RPC_MetaInBox,0,new TGeoCombiTrans(0.,0.,0.,rotUni)); 
  RPC_MetaInBox->SetVisibility(kFALSE); //invisible interior volume (gas)

  //Glass planes in the gas (medium of RPC_MetaInBox volume)
  TGeoVolume *RPC_GlassBox=gGeoManager->MakeBox("RPC_GlassBox", pRPCGlassMedium, 	
  		155.5/2,122.5/2,0.19/2); 
  RPC_MetaInBox->AddNode(RPC_GlassBox,0,new TGeoCombiTrans(0.,0.,-0.29,rotUni)); 
  RPC_MetaInBox->AddNode(RPC_GlassBox,1,new TGeoCombiTrans(0.,0.,0.,rotUni)); 
  RPC_MetaInBox->AddNode(RPC_GlassBox,2,new TGeoCombiTrans(0.,0.,0.29,rotUni)); 
  RPC_GlassBox->SetLineColor(kBlue); 

  //Gas (planes) already in the gas (sensitive volumes where ionization takes place)
  TGeoVolume *RPC_GasBox=gGeoManager->MakeBox("RPC_GasBox", pRPCGasR134AMedium, 	
  		151.2/2,121.0/2,0.1/2); 
  RPC_MetaInBox->AddNode(RPC_GasBox,0,new TGeoCombiTrans(0.,0.,-0.145,rotUni)); 
  RPC_MetaInBox->AddNode(RPC_GasBox,1,new TGeoCombiTrans(0.,0.,0.145,rotUni)); 
  RPC_GasBox->SetVisibility(kFALSE); //invisible interior volume (gas)

  // ---------------   Finish   -----------------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001);
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}
// ----------------------------------------------------------------------------

// -------   Global positioning   ---------------------------------------------
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
    fRefRot = fRef;
    
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
      c4.SetRotation(fRefRot->GetRotation());
      
      TGeoCombiTrans ccc = c3 * c4;
      
      TGeoCombiTrans pGlobal;
      pGlobal.SetRotation(ccc.GetRotation());
      const Double_t *allt = cc.GetTranslation();
      pGlobal.SetTranslation(allt[0],allt[1],allt[2]);
      
      return  ( new TGeoCombiTrans( pGlobal ) );
      
    }else{
      cout << "-E- R3BDetector::GetGlobalPosition() \
	      No. Ref. Transformation defined ! " << endl;
      cout << "-E- R3BDetector::GetGlobalPosition() \
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


