#ifndef EnsarPhysicsListMessenger_h
#define EnsarPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class EnsarPhysicsList;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;


class EnsarPhysicsListMessenger: public G4UImessenger {
private:
  
  EnsarPhysicsList*               pPhysicsList;
    
  G4UIdirectory*             physDir;    
  G4UIcmdWithADoubleAndUnit* gammaCutCmd;
  G4UIcmdWithADoubleAndUnit* electCutCmd;
  G4UIcmdWithADoubleAndUnit* protoCutCmd;    
  G4UIcmdWithADoubleAndUnit* allCutCmd;    
  G4UIcmdWithAnInteger*      verbCmd;
  G4UIcmdWithAString*        pListCmd;
   
public:
  
  EnsarPhysicsListMessenger(EnsarPhysicsList* );
  ~EnsarPhysicsListMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
};
#endif

