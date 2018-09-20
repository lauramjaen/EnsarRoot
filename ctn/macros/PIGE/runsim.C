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

#include "simall.C"

void runsim() {

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
	 // Gamma-200keV cascade generator: "Gamma_AngularC_200k"
  //TString fGene="box";
  //TString fGene="Sicascade";
  //TString fGene="SiOneCascade";
  //TString fGene="Cocascade";
  //TString fGene="Gamma_AngularC_200k";
	//TString fGene="208Tldecay";
	TString fGene="232Thchain";
	//TString fGene="238Uchain";



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
  detGeo.Add(new TObjString("HPGE"),  new TObjString("HPGe130_and_ReacChamber.geo.root"));//HPGe at 130º + ReacChamber	
 	//detGeo.Add(new TObjString("HPGE"),  new TObjString(" HPGe90_and_ReacChamber.geo.root"));//HPGe at 90º+Chamber	
  //detGeo.Add(new TObjString("CALIFA"),new TObjString("califa_petals_box.geo.root"));// PETALS not included

   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 1000000;


   //-------------------------------------------------
   //- EventDisplay        |    fEventDisplay (Bool_t)
   //-------------------------------------------------
   //   connected:              kTRUE
   //   not connected:          kFALSE
   Bool_t fEventDisplay=kFALSE;//kTRUE info GeoTracks for evtvis.C

   // Main Sim function call
   simall(nEvents, &detGeo, fEventDisplay, fMC, fGene, fUserPList,
	   OutFile, ParFile, EventFile);

}

