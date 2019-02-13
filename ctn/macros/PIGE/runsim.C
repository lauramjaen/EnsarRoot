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
 // TString fGene="box";
  //TString fGene="Sicascade";
  //TString fGene="SiOneCascade";
  //TString fGene="Cocascade";
  //TString fGene="Gamma_AngularC_200k";
	//TString fGene="208Tldecay";
	//TString fGene="232Thchain";
	//TString fGene="238Uchain";
	//TString fGene="ascii";

	//-------------------------------------------------
	// Environmental Background Generator
	// It includes: Uranium, Thorium, Actinium chains
	// 							and Potassium decay
	//-------------------------------------------------
	//	TString fGene="URANIUMchain_238U";  //no signal  
	//	TString fGene="URANIUMchain_234Th"; //earth signal 
	//	TString fGene="URANIUMchain_234Pa"; //earth signal  
	//	TString fGene="URANIUMchain_234U";  //no signal 
	//	TString fGene="URANIUMchain_230Th"; //no signal 
	//	TString fGene="URANIUMchain_226Ra"; //earth signal 
	//	TString fGene="URANIUMchain_214Pb"; //earth signal 
		TString fGene="URANIUMchain_214Bi";	  //earth signal 
	//	TString fGene="URANIUMchain_210Pb"; //no signal 

	//	TString fGene="Potassium";

	//	TString fGene="THORIUMchain_232Th"; //no signal  
	//	TString fGene="THORIUMchain_228Ra"; //no signal  
	//	TString fGene="THORIUMchain_228Th"; //no signal 
	//	TString fGene="THORIUMchain_224Ra"; //earth signal 
	//	TString fGene="THORIUMchain_212Pb"; //earth signal, peak of 265 keV si
	//	TString fGene="THORIUMchain_212Bi"; //earth signal   
	//	TString fGene="THORIUMchain_208Tl"; //earth signal  
	//	TString fGene="THORIUMchain_228Ac"; //earth signal   

	//	TString fGene="ACTINIUMchain_235U"; //earth signal
	//	TString fGene="ACTINIUMchain_231Th";//earth signal
	//	TString fGene="ACTINIUMchain_231Pa";//earth signal
	//	TString fGene="ACTINIUMchain_227Ac";//no signal
	//	TString fGene="ACTINIUMchain_227Th";//earth signal
	//	TString fGene="ACTINIUMchain_223Ra";//earth signal
	//	TString fGene="ACTINIUMchain_219Rn";//earth signal
	//	TString fGene="ACTINIUMchain_211Pb";//earth signal
	//	TString fGene="ACTINIUMchain_211Bi";
	//	TString fGene="ACTINIUMchain_223Fr";
	//-------------------------------------------------


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
  //detGeo.Add(new TObjString("HPGE"),  new TObjString("HPGe130_and_ReacChamber.geo.root"));//HPGe at 130º + ReacChamber	POTASSIUM
  //detGeo.Add(new TObjString("HPGE"),  new TObjString("HPGe_and_ReacChamberearth.geo.root"));//HPGe at 130º + ReacChamber	+ Earth effect
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("HPGe_and_ReacChamberearth_10cm.geo.root"));//HPGe at 130º + ReacChamber	+ Earth effect
	//detGeo.Add(new TObjString("HPGE"),  new TObjString("HPGe_and_ReacChamber_earth_20cm.geo.root"));//HPGe at 130º + ReacChamber	+ Earth effect
 	//detGeo.Add(new TObjString("HPGE"),  new TObjString(" HPGe90_and_ReacChamber.geo.root"));//HPGe at 90º+Chamber	
  //detGeo.Add(new TObjString("CALIFA"),new TObjString("califa_petals_box.geo.root"));// PETALS not included


detGeo.Add(new TObjString("HPGE"),  new TObjString("HPGe_and_ReacChamberearth_12p2.geo.root"));//HPGe at 130º + ReacChamber	+ Earth effect

   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 10;//si 208T:1000000 el resto de la cadena de Thorio 2866972  ---- 3000000 resto 8600916
													 //5000000 para todos cadena Uranio i Actinio               ---- 7000000
													 //potassium 5 000 000


   //-------------------------------------------------
   //- EventDisplay        |    fEventDisplay (Bool_t)
   //-------------------------------------------------
   //   connected:              kTRUE
   //   not connected:          kFALSE
   Bool_t fEventDisplay=kTRUE;//kTRUE info GeoTracks for evtvis.C

   // Main Sim function call
   simall(nEvents, &detGeo, fEventDisplay, fMC, fGene, fUserPList,
	   OutFile, ParFile, EventFile);

}

