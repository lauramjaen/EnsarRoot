//--------------------------------------------------------------------
//
// Define the simulation Setup
// Steering macro to main simall.C
// Author: <D.Bertini@gsi.de>
// Adapted by: M.I.Cherciu <mcherciu@spacescience.ro>
//
// Last Update: 27/10/2015 
//
//
//--------------------------------------------------------------------

#include "simall.C"

void runsim(){


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

   TString fGene="box";

   //-------------------------------------------------
   // Secondaries  generation (G4 only)
   // R3B Spec. PhysicList |     fUserPList (Bool_t)
   // ----------------------------------------------
   //     VMC Standard           kFALSE : just electromagnetic
   //     R3B Special            kTRUE : QGSP_BIC_HP for G4

   Bool_t fUserPList= kFALSE;


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

    TObjString det0("EnsarScintDet");

    TObjArray fDetList;
    fDetList.Add(&det0);

   //-------------------------------------------------
   //- N# of Sim. Events   |    nEvents     (Int_t)
   //-------------------------------------------------

   Int_t nEvents = 10;

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

