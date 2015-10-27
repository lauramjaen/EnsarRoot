/////////////////////////////////////////////////////////////////
//*-- AUTHOR : Pablo Cabanelas    pablo.cabanelas@usc.es
//*-- Date: 18/09/2012
// --------------------------------------------------------------
// Description:
//   Physics List -- Decays builder
//
// --------------------------------------------------------------
/////////////////////////////////////////////////////////////////

#include "EnsarDecaysBuilder.h"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4Decay.hh"


EnsarDecaysBuilder::EnsarDecaysBuilder(const G4String& name)
   :  G4VPhysicsConstructor(name){}


EnsarDecaysBuilder::~EnsarDecaysBuilder(){}


void EnsarDecaysBuilder::ConstructParticle(){}


void EnsarDecaysBuilder::ConstructProcess() {
  //
  // Add the decay proccesses.
  //

  // Add Decay Process

  G4Decay* fDecayProcess = new G4Decay();

  aParticleIterator->reset();
  while( (*aParticleIterator)() ){
    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (fDecayProcess->IsApplicable(*particle)) {


      // set ordering for PostStepDoIt and AtRestDoIt
      //pmanager ->SetProcessOrdering(fDecayProcess, idxPostStep);
      //pmanager ->SetProcessOrdering(fDecayProcess, idxAtRest);
      pmanager->AddProcess(fDecayProcess, 1,-1, 5);

    }
  }
}

