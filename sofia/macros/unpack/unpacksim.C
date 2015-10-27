//--------------------------------------------------------------------
//
// Define the SOFIA analysis Setup
// Steering macros to main unpackall.C
// Author: Jose Luis Rodriguez Sanchez <joseluis.rodriguez.sanchez@usc.es>
//
// Last Update: 31/10/2012
// Comments:
//
//
//--------------------------------------------------------------------


Int_t unpacksim(){

   // Load the Main unpack macro
   gROOT->LoadMacro("unpackall.C");


   // Load list of runs
   TString InFiles="runlist.dat" ;

   //-------------------------------------------------
   //- Geometrical Setup Definition
   //-  Non Sensitiv        |    fDetName (String)
   //-------------------------------------------------
   //   Target:                  TARGET H2
   //   Magnet:                  ALADIN
   //-------------------------------------------------
   //-  Sensitive           |    fDetName
   //-------------------------------------------------
   //
   //   Tof:                     TOF     (Fission fragments)
   //                            LCP_TOF (light-charged particles)
   //
   //   Tracker:                 TWINMUSIC
   //                            MWPC_1
   //                            MWPC_2
   //                            
   //   Neutron:                 LAND

    TObjString det0("Crate1");
    TObjString det1("Crate2");
    TObjString det2("LCP_TOF");
    TObjString det3("LAND");

    TObjArray fDetList;
    //fDetList.Add(&det0);
    //fDetList.Add(&det1);
    //fDetList.Add(&det2);
    fDetList.Add(&det3);

   // Main Sim function call
   sofiaall( InFiles,
             fDetList
          );      

}

