//--------------------------------------------------------------------
//
// Define the CTN HPGe detector and Munich reaction chamber
//  simulation Setup for the nov16 experiment
// Steering macro to main simall.C
// Author: <pablo.cabanelas@usc.es>
//
// Last Update: 03/03/17
//
//
//--------------------------------------------------------------------

#include "simall_Cherry.C"

void runsim_Cherry() {

  // Input files
  TString EventFile = "evt_file.dat";

  // Output files
  TString OutFile = "outsim.root";
  TString ParFile = "outpar.root";

  //-------------------------------------------------
  // Monte Carlo type     |    fMC        (TString)
  //-------------------------------------------------
  //   Geant3:                 "TGeant3"
  //   Geant4:                 "TGeant4"
  //   Fluka :                 "TFluka"
  TString fMC ="TGeant4";

  //-------------------------------------------------
  // Primaries generation
  // Event Generator Type |   fGene       (TString)
  //-------------------------------------------------
  // Box  generator:             "box"
  // Ascii generator:            "ascii"
  // Si-12643 cascade generator: "Sicascade"
  // Si-12900 cascade generator: "SiOneCascade"
  // Co-60 cascade generator:    "Cocascade"
  // Gamma-200keV cascade generator: "Gamma_AngularC_200k"
   TString fGene="box";
  //TString fGene="Sicascade";
  //TString fGene="SiOneCascade";
  //TString fGene="Cocascade";
  //TString fGene="Gamma_AngularC_200k";


  //-------------------------------------------------
  // Secondaries  generation (G4 only)
  // R3B Spec. PhysicList |     fUserPList (Bool_t)
  // ----------------------------------------------
  //     VMC Standard           kFALSE
  //     R3B Special            kTRUE;
   Bool_t fUserPList= kTRUE;


  //-------------------------------------------------
  //- Geometrical Setup Definition
  //-  Non Sensitiv        |    fDetName (String)
  //-------------------------------------------------

  TMap detGeo;
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_1tomato.geo.root"));//One cherry	
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_7tomatoes_setup.geo.root"));//7 cherries +Setup
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_7tomatoes_skin_setup.geo.root"));//7 cherries +skin +Setup
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_OneCherry_skin_setup.geo.root"));//OneCherry +skin +Setup
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_1Active_skin_setup.geo.root"));//1Active +skin +Setup
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_1Active_NoMetalBase.geo.root"));//1Active +skin +NoBase
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_2ndCherryActive_skin_setup.geo.root"));//1Active +skin +Setup
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_2ndCherryActive_skin_NoMetalBase.geo.root"));//1Active +skin +Setup
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_Water_Box.geo.root"));//Water Box
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_Air_Box.geo.root"));//Water Box
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_Water_100.geo.root"));//Water Box
	detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_Cherry_15Layers.geo.root"));//Water Box
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_Water_smallBlock.geo.root"));//Water smallBox as cherry
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("Cherry_Lala.geo.root"));//Active Cherry 2 made with layers
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("WaterAsCh_1Box_setup_NoLayers.geo.root"));//Water Box

   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 100;


   //-------------------------------------------------
   //- EventDisplay        |    fEventDisplay (Bool_t)
   //-------------------------------------------------
   //   connected:              kTRUE
   //   not connected:          kFALSE
   Bool_t fEventDisplay=kTRUE;//kTRUE info GeoTracks

   // Main Sim function call
   simall_Cherry(nEvents, &detGeo, fEventDisplay, fMC, fGene, fUserPList,
	   OutFile, ParFile, EventFile);

}

