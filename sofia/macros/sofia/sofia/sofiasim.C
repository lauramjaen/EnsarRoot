//--------------------------------------------------------------------
//
// Define the SOFIA simulation Setup
// Steering macros to main sofiaall.C
// Author: Jose Luis Rodriguez Sanchez <joseluis.rodriguez.sanchez@usc.es>
//
// Last Update: 31/08/2012
// Comments:
//
//
//--------------------------------------------------------------------


Int_t sofiasim(){

   // Load the Main Simulation macro
   gROOT->LoadMacro("sofiaall.C");

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
   // Ion generator:             "ion"

   TString fGene="ion";

   //-------------------------------------------------
   // Secondaries  generation (G4 only)
   // R3B Spec. PhysicList |     fUserPList (Bool_t)
   // ----------------------------------------------
   //     VMC Standard           kFALSE
   //     R3B Special            kTRUE;

   Bool_t fUserPList= kTRUE;

   // Sofia Experiment, Target type
   TString target1="H2";

   //-------------------------------------------------
   //- Geometrical Setup Definition
   //-  Non Sensitiv        |    fDetName (String)
   //-------------------------------------------------
   //   Target:                  TARGET H2
   //   Magnet:                  ALADIN
   //                            GLAD
   //-------------------------------------------------
   //-  Sensitiv            |    fDetName
   //-------------------------------------------------
   //   Calorimeter:             CALIFA
   //
   //   Tof:                     TOF     (Fission fragments)
   //                            CLP_TOF (light-charged particles)
   //
   //   Tracker:                 TWINMUSIC
   //                            MWPC_1
   //                            MWPC_2
   //                            
   //   Neutron:                 LAND

    TObjString det0("TARGET");
    TObjString det1("CALIFA");
    TObjString det2("VACUUMPIPE");
    TObjString det3("CLP_TOF");
    TObjString det4("TWINMUSIC");
    TObjString det5("MWPC_1");
    TObjString det6("ALADIN");
    TObjString det7("GLAD");
    TObjString det8("HELIUMPIPE");
    TObjString det9("MWPC_2");
    TObjString det10("TOF");
    TObjString det11("LAND");

    TObjArray fDetList;
    fDetList.Add(&det0);
    //fDetList.Add(&det1);
    fDetList.Add(&det2);
    //fDetList.Add(&det3);
    fDetList.Add(&det4);
    fDetList.Add(&det5);
    fDetList.Add(&det6);
    //fDetList.Add(&det7);
    fDetList.Add(&det8);
    fDetList.Add(&det9);
    fDetList.Add(&det10);
    //fDetList.Add(&det11);

   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 2;
   //Int_t nEvents = 3;

   //-------------------------------------------------
   //- EventDisplay        |    fEventDisplay (Bool_t)
   //-------------------------------------------------
   //   connected:              kTRUE
   //   not connected:          kFALSE

   Bool_t fEventDisplay=kTRUE;

    // Magnet Field definition
       Bool_t fR3BMagnet = kTRUE;
       //Bool_t fR3BMagnet = kFALSE;
	

   // Main Sim function call
   sofiaall( nEvents,
             fDetList,
             target1,
             fEventDisplay,
	     fMC,
	     fGene,
	     fUserPList,
             fR3BMagnet
          );      

}

