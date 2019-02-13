//--------------------------------------------------------------------
//
// Define the simulation Setup
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
  // Ascii generator:          "ascii"
  //TString fGene="box";

	//-------------------------------------------------
	// Environmental Background Generator
	// It includes: Uranium, Thorium, Actinium chains
	// 							and Potassium decay
	//-------------------------------------------------
	//		TString fGene="URANIUMchain_238U";
	//	TString fGene="URANIUMchain_234Th";
	//	TString fGene="URANIUMchain_234Pa";
	//	TString fGene="URANIUMchain_234U";
	//	TString fGene="URANIUMchain_230Th";
	//	TString fGene="URANIUMchain_226Ra";
	//	TString fGene="URANIUMchain_214Pb";
	//	TString fGene="URANIUMchain_214Bi";
	//	TString fGene="URANIUMchain_210Pb";

		TString fGene="Potassium";

	//	TString fGene="THORIUMchain_232Th";
	//	TString fGene="THORIUMchain_228Ra";
	//	TString fGene="THORIUMchain_228Th";
	//	TString fGene="THORIUMchain_224Ra";
	//	TString fGene="THORIUMchain_212Pb";//peak of 265 keV
	//	TString fGene="THORIUMchain_212Bi";
	//	TString fGene="THORIUMchain_208Tl"; 
	//	TString fGene="THORIUMchain_228Ac";

	//	TString fGene="ACTINIUMchain_235U";
	//	TString fGene="ACTINIUMchain_231Th";
	//	TString fGene="ACTINIUMchain_231Pa";
	//	TString fGene="ACTINIUMchain_227Ac";
	//	TString fGene="ACTINIUMchain_227Th";
	//	TString fGene="ACTINIUMchain_223Ra";
	//	TString fGene="ACTINIUMchain_219Rn";
	//	TString fGene="ACTINIUMchain_211Pb";
	//	TString fGene="ACTINIUMchain_211Bi";
	//	TString fGene="ACTINIUMchain_223Fr";

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
  detGeo.Add(new TObjString("NAI"),
  	     new TObjString("NaIdetector_LuisLab.geo.root"));


   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 5000000;

   //-------------------------------------------------
   //- EventDisplay        |    fEventDisplay (Bool_t)
   //-------------------------------------------------
   //   connected:              kTRUE
   //   not connected:          kFALSE
   Bool_t fEventDisplay=kTRUE;

   // Main Sim function call
   simall(nEvents, &detGeo, fEventDisplay, fMC, fGene, fUserPList,
	   OutFile, ParFile, EventFile);

}


