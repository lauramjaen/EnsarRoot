// Configuration macro for Geant4 VirtualMC
void Config()
{
   // Load the User Defined Physics List
   cout << "-I- Ensar Geant4 Config(): Loading G4 User Defined Physics List " << endl;

    gSystem->Load("libEnsarPlist.so");

    cout<<endl;

    ///    Create the run configuration
   /// In constructor user has to specify the geometry input
   /// and select geometry navigation via the following options:
   /// - geomVMCtoGeant4   - geometry defined via VMC, G4 native navigation
   /// - geomVMCtoRoot     - geometry defined via VMC, Root navigation
   /// - geomRoot          - geometry defined via Root, Root navigation
   /// - geomRootToGeant4  - geometry defined via Root, G4 native navigation
   /// - geomGeant4        - geometry defined via Geant4, G4 native navigation

   // Create the Run Configuration
   //EnsarRunConfiguration* runConfiguration
   //  = new EnsarRunConfiguration("geomRootToGeant4", "stepLimiter+specialCuts+specialControls");

/// Create the G4 VMC 
   //TGeant4* geant4 = new TGeant4("TGeant4", "The Geant4 Monte Carlo",
				// runConfiguration);
   //cout << "-I- Ensar Config(): Geant4 with Ensar dedicated Physics list " << endl;
   //cout << "-I- Ensar Config(): has been created..." << endl;


   // Create Geant4 VMC
   //  
    TGeant4 *geant4 = 0;
    TG4RunConfiguration* runConfiguration 
      = new TG4RunConfiguration("geomRoot", 
          "emStandard", 
          "specialCuts+stackPopper+stepLimiter+specialControls",
          true);

    geant4 = new TGeant4("TGeant4", "The Geant4 Monte Carlo", runConfiguration);
    cout << "-I- Ensar Config(): Geant4 with Ensar dedicated Physics list " << endl;
    cout << "-I- Ensar Config(): has been created..." << endl;


/// create the Specific stack
   EnsarMCStack *stack = new EnsarMCStack(1000); 
   stack->SetDebug(kFALSE);
   stack->StoreSecondaries(kTRUE);
   stack->SetMinPoints(0);
   geant4->SetStack(stack);

   if(FairRunSim::Instance()->IsExtDecayer()){
      TVirtualMCDecayer* decayer = TPythia6Decayer::Instance();
      geant4->SetExternalDecayer(decayer);
   }
  
/// Customise Geant4 setting
/// (verbose level, global range cut, ..)

   TString configm(gSystem->Getenv("VMCWORKDIR"));
   TString configm1 = configm + "/gconfig/g4config.in";
   cout << " -I g4Config() using g4conf  macro: " << configm1 << endl;

   //set geant4 specific stuff
  geant4->SetMaxNStep(10000);  // default is 30000
  geant4->ProcessGeantMacro(configm1.Data());

}
