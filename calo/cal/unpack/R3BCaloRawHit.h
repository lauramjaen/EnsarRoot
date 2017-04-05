// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                          R3BCaloRawHit                            -----
// -----                           Version 0.1                             -----
// -----                    Created 15.10.2013 by Y.Gonzalez               -----
// -----                    Modified 24/09/2014 by A.Perea                                               -----
// -----------------------------------------------------------------------------
#ifndef R3BCALORAWHIT_H
#define R3BCALORAWHIT_H
#include "TObject.h"
#include <stdint.h>
class R3BCaloUnpack;

class R3BCaloRawHit : public TObject {
  friend class R3BCaloUnpack;
// rootcint does not understand decltype. so we put everything together
#define GETTER(TYPE, NAME)						\
  public: inline const TYPE& Get ## NAME () const { return f ##NAME; } 
#define DECLFIELD(TYPE, NAME)					\
  protected: TYPE f##NAME=0;
#define DECLFIELDGETTER(TYPE, NAME) DECLFIELD(TYPE, NAME) GETTER(TYPE, NAME)
  //     UShort_t& GetCrystalId() const { return fCrystalId; }

public:
  // Default Constructor
  R3BCaloRawHit();
  
  //Destructor
  ~R3BCaloRawHit() { }
  
  //Getters
  DECLFIELDGETTER(uint16_t, CrystalId);
  DECLFIELDGETTER( int32_t, Energy);
  DECLFIELDGETTER( int32_t, Nf);
  DECLFIELDGETTER( int32_t, Ns);
  DECLFIELDGETTER(uint64_t, WRTS);
  DECLFIELDGETTER(uint64_t, FebexTS);
  DECLFIELDGETTER( int16_t, Tot);
  DECLFIELDGETTER(uint16_t, Overflow);  

public:
  ClassDef(R3BCaloRawHit,1)
};

#endif
