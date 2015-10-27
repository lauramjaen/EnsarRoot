#ifndef R3Btubevacuum_H
#define R3Btubevacuum_H

#include "FairDetector.h"
#include "R3BModule.h"
#include <TString.h>


class R3Btubevacuum : public R3BModule {
  public:
    R3Btubevacuum(const char * name, const char *Title="SOFIA Vacuum pipe");
    R3Btubevacuum();
    virtual ~R3Btubevacuum();
    virtual void ConstructGeometry();

 protected:
     TString *fTargetName; //!
     void ConstructGeometry1();

    ClassDef(R3Btubevacuum,1)
  
};

#endif //R3Btubevacuum_H

