#ifndef ENSARMCTRACKS_H
#define ENSARMCTRACKS_H

#include "FairMCTracks.h"



class EnsarMCTracks : public FairMCTracks {

public:

  EnsarMCTracks();
  EnsarMCTracks(const char* name, Int_t iVerbose = 1);
  virtual ~EnsarMCTracks(){;}

  virtual InitStatus Init();
  virtual void Exec(Option_t* option);

 ClassDef(EnsarMCTracks,1);
};
#endif

