/////////////////////////////////////////////////////////////////
//*-- AUTHOR : Pablo Cabanelas    pablo.cabanelas@usc.es
//*-- Date: 18/09/2012
// --------------------------------------------------------------
// Description:
//   Physics List 
//
// --------------------------------------------------------------
/////////////////////////////////////////////////////////////////

#include "EnsarPhysicsList.h"
#include "EnsarPhysicsListMessenger.h"

#include "EnsarParticlesBuilder.h"
#include "G4EmQEDBuilder.h"
#include "G4EmMuonBuilder.h"
#include "G4EmHadronBuilder.h"
#include "G4LowEnergyQEDBuilder.h"
#include "G4PenelopeQEDBuilder.h"
#include "G4StepLimiterBuilder.h"
#include "EnsarDecaysBuilder.h"
#include "EmHadronElasticBuilder.h"
#include "EmBinaryCascadeBuilder.h"
#include "EmIonBinaryCascadeBuilder.h"
#include "EmGammaNucleusBuilder.h"


// std G4 headers
#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

EnsarPhysicsList::EnsarPhysicsList():  G4VModularPhysicsList(){
  //
  // Constructor. Initializing values
  //
  
  emBuilderIsRegisted = false;
  decayIsRegisted = false;
  stepLimiterIsRegisted = false;
  helIsRegisted = false;
  bicIsRegisted = false;
  ionIsRegisted = false;
  gnucIsRegisted = false;
  verbose = 0;
  G4LossTableManager::Instance()->SetVerbose(0);
  defaultCutValue = 1.*CLHEP::mm;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;
  
  pMessenger = new EnsarPhysicsListMessenger(this);
  
  // Add Physics builders
  RegisterPhysics(new EnsarParticlesBuilder());
  steplimiter = new G4StepLimiterBuilder();
}


EnsarPhysicsList::~EnsarPhysicsList() {
  //
  // Destructor. Nothing to do
  //  
}


void EnsarPhysicsList::ConstructParticle() {
  //
  // Construct Particles
  //

  if(verbose > 0)
    G4cout << "Construct Particles" << G4endl;

  G4VModularPhysicsList::ConstructParticle();
}



void EnsarPhysicsList::ConstructProcess() {
  //
  // Construct Processes
  //

  if(verbose > 0)
    G4cout << "Construct Processes" << G4endl;

  if(!emBuilderIsRegisted) AddPhysicsList("standard");
  G4VModularPhysicsList::ConstructProcess();
  
  // Define energy interval for loss processes
  G4EmProcessOptions emOptions;
  emOptions.SetMinEnergy(0.1*CLHEP::keV);
  emOptions.SetMaxEnergy(100.*CLHEP::GeV);
  emOptions.SetDEDXBinning(90);
  emOptions.SetLambdaBinning(90);
  //emOptions.SetBuildPreciseRange(false);
  //  emOptions.SetApplyCuts(true);
  //emOptions.SetVerbose(0);
}

void EnsarPhysicsList::AddPhysicsList(const G4String& name){
  //
  // Registering the physics processes
  //
  if(verbose > 0) {
    G4cout << "Add Physics <" << name 
           << "> emBuilderIsRegisted= " << emBuilderIsRegisted
           << G4endl;
  }
  if ((name == "standard") && !emBuilderIsRegisted) {
    RegisterPhysics(new G4EmQEDBuilder());
    RegisterPhysics(steplimiter);
    RegisterPhysics(new G4EmMuonBuilder());
    RegisterPhysics(new G4EmHadronBuilder());
    emBuilderIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;    

    /*  } else if (name == "g4v52" && !emBuilderIsRegisted) {
    RegisterPhysics(new G4EmQEDBuilder52());
    RegisterPhysics(steplimiter);
    RegisterPhysics(new G4EmMuonBuilder52());
    RegisterPhysics(new G4EmHadronBuilder52());
    emBuilderIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "g4v71" && !emBuilderIsRegisted) {
    RegisterPhysics(new G4EmQEDBuilder71());
    RegisterPhysics(steplimiter);
    RegisterPhysics(new G4EmMuonBuilder71());
    RegisterPhysics(new G4EmHadronBuilder71());
    emBuilderIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    */
  } else if (name == "lowenergy" && !emBuilderIsRegisted) {
    RegisterPhysics(new G4LowEnergyQEDBuilder());
    RegisterPhysics(steplimiter);
    RegisterPhysics(new G4EmMuonBuilder());
    RegisterPhysics(new G4EmHadronBuilder());
    emBuilderIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "penelope" && !emBuilderIsRegisted) {
    RegisterPhysics(new G4PenelopeQEDBuilder());
    RegisterPhysics(steplimiter);
    RegisterPhysics(new G4EmMuonBuilder());
    RegisterPhysics(new G4EmHadronBuilder());
    emBuilderIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "decay" && !decayIsRegisted) {
    RegisterPhysics(new EnsarDecaysBuilder());
    decayIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "elastic" && !helIsRegisted) {
    RegisterPhysics(new EmHadronElasticBuilder());
    helIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    
  } else if (name == "binary" && !bicIsRegisted) {
    RegisterPhysics(new EmBinaryCascadeBuilder());
    bicIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    
  } else if (name == "binary_ion" && !ionIsRegisted) {
    RegisterPhysics(new EmIonBinaryCascadeBuilder());
    ionIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;

  } else if (name == "gamma_nuc" && !gnucIsRegisted) {
    RegisterPhysics(new EmGammaNucleusBuilder());
    gnucIsRegisted = true;
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" << G4endl;
    
  } else {
    G4cout << "EnsarPhysicsList::AddPhysicsList <" << name << ">" 
           << " fail - module is already regitered or is unknown " << G4endl;
  }
}





void EnsarPhysicsList::SetCuts(){
  //
  //  Sets the cut on the physics interaction calculations.
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  //
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  if (verbose>0) DumpCutValuesTable();
}


void EnsarPhysicsList::SetVerbose(G4int val){
  //
  // Selecting verbosity
  //

  verbose = val;
}


void EnsarPhysicsList::SetCutForGamma(G4double cut){
  //
  // Setting cut value for the gammas
  //

  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}


void EnsarPhysicsList::SetCutForElectron(G4double cut){
  //
  // Setting cut value for the electron
  //

  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}


void EnsarPhysicsList::SetCutForPositron(G4double cut) {
  //
  // Setting cut value for the positron
  //
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

