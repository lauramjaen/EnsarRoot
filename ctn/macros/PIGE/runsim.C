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
	//TString fGene="232Thchain";
	//TString fGene="238Uchain";
	//TString fGene="ascii";
	//	TString fGene="URANIUMchain_238U";
	//	TString fGene="URANIUMchain_234Th";
	//	TString fGene="URANIUMchain_234Pa";
	//	TString fGene="URANIUMchain_234U";
	//	TString fGene="URANIUMchain_230Th";
	//	TString fGene="URANIUMchain_226Ra";
	//	TString fGene="URANIUMchain_214Pb";
	//	TString fGene="URANIUMchain_214Bi";
	//	TString fGene="URANIUMchain_210Pb";
	//TString fGene="Potassium";
	//	TString fGene="THORIUMchain_232Th";
	//	TString fGene="THORIUMchain_228Ra";
	//	TString fGene="THORIUMchain_228Th";
	//	TString fGene="THORIUMchain_224Ra";
	//	TString fGene="THORIUMchain_212Pb";
	//	TString fGene="THORIUMchain_212Bi";
	//	TString fGene="THORIUMchain_208Tl";
	//	TString fGene="THORIUMchain_228Ac";//Problem


	//	TString fGene="ACTINIUMchain_235U"; ok
	//	TString fGene="ACTINIUMchain_231Th";ok
	//	TString fGene="ACTINIUMchain_231Pa";ok
	//	TString fGene="ACTINIUMchain_227Ac";ok
	//	TString fGene="ACTINIUMchain_227Th";ok
	//	TString fGene="ACTINIUMchain_223Ra";ok
	//	TString fGene="ACTINIUMchain_219Rn";ok
	//	TString fGene="ACTINIUMchain_211Pb";ok
		TString fGene="ACTINIUMchain_211Bi";
	//	TString fGene="ACTINIUMchain_223Fr";ok



  //-------------------------------------------------
  // Secondaries  generation (G4 only)
  // R3B Spec. PhysicList |     fUserPList (Bool_t)
  // ----------------------------------------------
  //     VMC Standard           kFALSE
  //     R3B Special            kTRUE;
   Bool_t fUserPList= kFALSE;


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

   Int_t nEvents = 997199;//208T:348800, 1395200 999998


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

