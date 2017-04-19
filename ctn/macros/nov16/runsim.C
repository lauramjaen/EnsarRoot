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
  // Ascii generator:          "ascii"
  TString fGene="box";

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
  detGeo.Add(new TObjString("HPGE"),  new TObjString("HPGedetector_and_chamber.geo.root"));
  detGeo.Add(new TObjString("CALIFA"),new TObjString("califa_petals.geo.root"));

   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 10000;

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

