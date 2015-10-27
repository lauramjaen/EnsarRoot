//--------------------------------------------------------------------
//
// Define the Tragaldabas simulation Setup
//--------------------------------------------------------------------

#include "tragall.C"

void tragsim() {
  
  // Load the Main Simulation macro
  TString macro_tragall = getenv("VMCWORKDIR");
  macro_tragall += "/tragaldabas/macros/tragall.C";
  gROOT->LoadMacro(macro_tragall.Data());
  
  // Input files
  TString EventFile = "evt_file.dat";

  // Output files
  TString OutFile = "tragsim.root";
  TString ParFile = "tragpar.root";
  
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
  // Box generator:             "box"
  // CALIFA generator:          "gammas"
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
  detGeo.Add(new TObjString("RPC"),
  	     new TObjString("tragaldabas_test.geo.root"));


   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 50;

   //-------------------------------------------------
   //- EventDisplay        |    fEventDisplay (Bool_t)
   //-------------------------------------------------
   //   connected:              kTRUE
   //   not connected:          kFALSE
   Bool_t fEventDisplay=kTRUE;

   // Main Sim function call
   tragall(nEvents, &detGeo, fEventDisplay, fMC, fGene, fUserPList,
	   OutFile, ParFile, EventFile);      

}

