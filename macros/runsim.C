//--------------------------------------------------------------------
//
// Define the simulation Setup
// Steering macro to main simall.C
// Author: <D.Bertini@gsi.de>
// Adapted by: <pablo.cabanelas@usc.es)
//
// Last Update: 18/09/12
//
//
//--------------------------------------------------------------------


Int_t runsim(){

  
  TString macro_all= getenv("VMCWORKDIR");
  macro_all+="/macros/simall.C"; 

   // Load the Main Simulation macro
    gROOT->LoadMacro(macro_all.Data());

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

   TString fGene="mygenerator";

   //-------------------------------------------------
   // Secondaries  generation (G4 only)
   // R3B Spec. PhysicList |     fUserPList (Bool_t)
   // ----------------------------------------------
   //     VMC Standard           kFALSE
   //     R3B Special            kTRUE;

   Bool_t fUserPList= kTRUE;


   //-------------------------------------------------
   //- Geometrical Setup Definition
   //-  Non Sensitive        |    fDetName (String)
   //-------------------------------------------------
   //   Target:                  
   //   Magnet:                  
   //-------------------------------------------------
   //-  Sensitive            |    fDetName
   //-------------------------------------------------
   //   My Detector:                 MYDET

    TObjString det0("MYDET");

    TObjArray fDetList;
    fDetList.Add(&det0);

   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 100;

   //-------------------------------------------------
   //- EventDisplay        |    fEventDisplay (Bool_t)
   //-------------------------------------------------
   //   connected:              kTRUE
   //   not connected:          kFALSE

   Bool_t fEventDisplay=kTRUE;

   // Main Sim function call
   simall(  nEvents,
            fDetList,
	    fEventDisplay,
	    fMC,
	    fGene,
	    fUserPList
          );      

}

