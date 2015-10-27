#ifndef R3Btubehelium_H
#define R3Btubehelium_H

#include "FairDetector.h"
#include "R3BModule.h"
#include <TString.h>


class R3Btubehelium : public R3BModule {
  public:
    R3Btubehelium(const char * name, const char *Title="SOFIA Helium pipe");
    R3Btubehelium();
    virtual ~R3Btubehelium();
    virtual void ConstructGeometry();

 protected:
     TString *fTargetName; //!
     void ConstructGeometry1();

    ClassDef(R3Btubehelium,1)
  
};

#endif //R3Btubehelium_H

