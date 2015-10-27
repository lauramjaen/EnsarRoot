#ifndef EnsarParticlesBuilder_h
#define EnsarParticlesBuilder_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


class EnsarParticlesBuilder : public G4VPhysicsConstructor {
public:
  EnsarParticlesBuilder(const G4String& name = "particles");
  virtual ~EnsarParticlesBuilder();

  // This method will be invoked in the Construct() method.
  // each particle type will be instantiated
  virtual void ConstructParticle();

  // This method is dummy.
  virtual void ConstructProcess() {};

};
#endif








