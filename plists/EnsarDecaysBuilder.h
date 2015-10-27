#ifndef EnsarDecaysBuilder_h
#define EnsarDecaysBuilder_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


class EnsarDecaysBuilder : public G4VPhysicsConstructor {
private:

   // hide assignment operator
  EnsarDecaysBuilder & operator=(const EnsarDecaysBuilder &right);
  EnsarDecaysBuilder(const EnsarDecaysBuilder&);

public:

  EnsarDecaysBuilder(const G4String& name = "decays");
  virtual ~EnsarDecaysBuilder();

  // This method is dummy for physics
  virtual void ConstructParticle();

  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type
  virtual void ConstructProcess();

};
#endif








