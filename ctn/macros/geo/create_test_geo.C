
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


//Bool_t fLocalTrans = kFALSE;
//Bool_t fLabTrans = kFALSE;

TGeoCombiTrans* GetGlobalPosition(TGeoCombiTrans *fRef);

void ConstructGeometry(TGeoMedium *pMed, TGeoMedium *pMedAl, TGeoMedium *pMedSteel, TGeoMedium *pMedGe, TGeoMedium *pMedLi);// mirar be!!


void create_test_geo(const char* geoTag = "test")
{
	fGlobalTrans->SetTranslation(0.0,0.0,0.0);

	// -------   Load media from media file   ----------------------------------- OK
	FairGeoLoader*    geoLoad = new FairGeoLoader("TGeo","FairGeoLoader");
	FairGeoInterface* geoFace = geoLoad->getGeoInterface();
	TString geoPath = gSystem->Getenv("VMCWORKDIR");
	TString medFile = geoPath + "/ctn/geometry/media.geo";
	geoFace->setMediaFile(medFile);
	geoFace->readMedia();
	gGeoMan = gGeoManager;
	// --------------------------------------------------------------------------



	// -------   Geometry file name (output)   ----------------------------------OK
	TString geoFileName = geoPath + "/ctn/geometry/HPGedetector_";
	geoFileName = geoFileName + geoTag + ".geo.root";
	// --------------------------------------------------------------------------



	// -----------------   Get and create the required media    ----------------- OK
	FairGeoMedia* gGeoMedia = geoFace->getMedia();    // en tragaldabas geoMedia
	FairGeoBuilder* gGeoBuild = geoLoad->getGeoBuilder();    // en tragaldabas geoBuild

	FairGeoMedium* mVac      = gGeoMedia->getMedium("vacuum");
	if ( ! mVac ) Fatal("Main", "FairMedium vacuum not found");
	gGeoBuild->createMedium(mVac);
	TGeoMedium* pMedVac = gGeoMan->GetMedium("vacuum");
	if ( ! pMedVac ) Fatal("Main", "Medium vacuum not found");       //creado el vacio

	FairGeoMedium* mAl      = gGeoMedia->getMedium("aluminium");
	if ( ! mAl ) Fatal("Main", "FairMedium Al not found");
	gGeoBuild->createMedium(mAl);
	TGeoMedium* pMedAl = gGeoMan->GetMedium("aluminium");
	if ( ! pMedAl ) Fatal("Main", "Medium Al not found");         //creado el aluminio

	FairGeoMedium* mSteel      = gGeoMedia->getMedium("Steel");
	if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
	gGeoBuild->createMedium(mSteel);
	TGeoMedium* pMedSteel = gGeoMan->GetMedium("Steel");
	if ( ! pMedSteel ) Fatal("Main", "Medium Steel not found");   // creado el acero: 0.15%C, 13%Cr, 86.85%Fe

	FairGeoMedium* mGe      = gGeoMedia->getMedium("germanium");
	if ( ! mGe ) Fatal("Main", "FairMedium Ge not found");
	gGeoBuild->createMedium(mGe);
	TGeoMedium* pMedGe = gGeoMan->GetMedium("germanium");
	if ( ! pMedGe ) Fatal("Main", "Medium Ge not found");         //creado el germanio

	FairGeoMedium* mLi      = gGeoMedia->getMedium("litium");
	if ( ! mLi ) Fatal("Main", "FairMedium Li not found");
	gGeoBuild->createMedium(mLi);
	TGeoMedium* pMedLi = gGeoMan->GetMedium("litium");
	if ( ! pMedLi ) Fatal("Main", "Medium Li not found");         //creado el litio

	// --------------------------------------------------------------------------



	// --------------   Create geometry and top volume  ------------------------- OK
	gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
	gGeoMan->SetName("TARGETgeom");
	gTop = new TGeoVolumeAssembly("TOP");
	gGeoMan->SetTopVolume(gTop);
	

	// --------------------------------------------------------------------------


	if (0 == strncmp(geoTag, "test", 6))  ConstructGeometry(pMedVac, pMedAl, pMedSteel, pMedGe, pMedLi); 	


	// ---------------   Finish   ----------------------------------------------- OK
	gGeoMan->CloseGeometry();
	gGeoMan->CheckOverlaps(0.001);
	gGeoMan->PrintOverlaps();
	gGeoMan->Test();

	TFile* geoFile = new TFile(geoFileName, "RECREATE");
	gTop->Write();
	geoFile->Close();
	// --------------------------------------------------------------------------
}


void ConstructGeometry(TGeoMedium *pMedVac, TGeoMedium *pMedAl, TGeoMedium *pMedSteel, TGeoMedium *pMedGe, TGeoMedium *pMedLi)
{
	cout << endl;
	cout << "-I- R3BTest::ConstructGeometry() "<< endl;
	cout << "-I- R3BTest  HPGe detector "<< endl;
	cout << endl;




	// ---------------------------------------------------------------------------------------------------------------------
	/* Top Volume          ya esta creado arriba -> Create geometry and top volume
	
	TGeoVolume *top=gGeoManager->MakeBox("World",pMedVac,1000,1000,1000);
	gGeoManager->SetTopVolume(top);
	gGeoManager->SetTopVisible(0);
	top->SetVisLeaves(kTRUE);*/
	

	// ----------------------------------------------

	// ----------------------------------------------
	// Auxiliar vacuum volume holding all important volumes as daugthers
	// The easiest way to rotate all volumes to the final position
	TGeoVolume *main_tube = gGeoManager->MakeTube("MAIN_TUBE",pMedVac,0.0,4.035,3.505);
	main_tube->SetFillColor(5);
	main_tube->SetLineColor(5);
	main_tube->SetTransparency(70);
	main_tube->SetVisLeaves(kTRUE);
	// ----------------------------------------------

	// ----------------------------------------------
	// Steel holder of the chamber
	TGeoVolume *steel_tub1 = gGeoManager->MakeTube("STEEL_TUB1", pMedSteel, 0.0, 3.95, 0.1);
	steel_tub1->SetFillColor(5);
	steel_tub1->SetLineColor(5);
	main_tube->AddNode(steel_tub1,1,new TGeoTranslation(0,0,-3.405));
	steel_tub1->SetVisLeaves(kTRUE);
	TGeoVolume *steel_tub2 = gGeoManager->MakeTube("STEEL_TUB2", pMedSteel, 3.75, 3.95, 3.405);
	steel_tub2->SetFillColor(5);
	steel_tub2->SetLineColor(5);
	main_tube->AddNode(steel_tub2,1,new TGeoTranslation(0,0,0.1));
	steel_tub2->SetVisLeaves(kTRUE);
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
	// Second Aluminum wrapping (4mm vacuum betwenn both wrappings)
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
	TGeoVolume *ge_inac_tub1 = gGeoManager->MakeTube("GE_TUB1", pMedGe, 0.0, 3.2, 0.1);
	ge_inac_tub1->SetFillColor(50);
	ge_inac_tub1->SetLineColor(50);
	main_tube->AddNode(ge_inac_tub1,1,new TGeoTranslation(0,0,-2.68));
	ge_inac_tub1->SetVisLeaves(kTRUE);
	TGeoVolume *ge_inac_tub2 = gGeoManager->MakeTube("GE_TUB2", pMedGe, 3.0, 3.2, 3.03);
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
	// only linear translation
	//top->AddNode(main_tube,1,new TGeoTranslation(0,0,8.855));
	// ----------------------------------------------

	// ----------------------------------------------
	// detector at 130 degrees
	TGeoRotation *rot     = new TGeoRotation("rot",0,220,0);
	TGeoCombiTrans *comb = new TGeoCombiTrans("comb",0,6.7833,-5.6919,rot);
	gTop->AddNode(main_tube,1,comb);    //estamos añadiendo todos los tubos de ditintos materiales q estan dentro del main_tub dentro del volumen principal gtop
	// ----------------------------------------------


	// ----------------------------------------------
	// Adding sensitive volumes only de active region
	//AddSensitiveVolume(ge_tub3);
	//AddSensitiveVolume(ge_tub4);
	// ----------------------------------------------


	//--------------------------------------------------------------------------------------------------------------------



}

