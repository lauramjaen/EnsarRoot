#ifndef EnsarPhysicsList_h
#define EnsarPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class EnsarPhysicsListMessenger;
class G4StepLimiterBuilder;

class EnsarPhysicsList: public G4VModularPhysicsList {
private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4int    verbose;
  G4bool   emBuilderIsRegisted;
  G4bool   decayIsRegisted;
  G4bool   stepLimiterIsRegisted;
  G4bool   helIsRegisted;
  G4bool   bicIsRegisted;
  G4bool   ionIsRegisted;
  G4bool   gnucIsRegisted;
  
  EnsarPhysicsListMessenger* pMessenger;
  G4StepLimiterBuilder* steplimiter;
  
public:
  EnsarPhysicsList();
  ~EnsarPhysicsList();
  
  // SetCuts()   
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);

  void AddPhysicsList(const G4String&);
  void SetVerbose(G4int val);
};
#endif



