
#ifndef ENSAREVENTMANAGER_H
#define ENSAREVENTMANAGER_H

#include "FairEventManager.h"

class TEveViewer;
class TEveScene;
class EnsarIonName;


class EnsarEventManager : public FairEventManager
{
 public:
  static EnsarEventManager *Instance();
  EnsarEventManager();
  virtual ~EnsarEventManager(){;}
  virtual void AddParticlesToPdgDataBase(Int_t pdgCode);
  virtual void SetScaleByEnergy(Bool_t scale) {fScaleByEnergy = scale;}
  virtual Bool_t IsScaleByEnergy() {return fScaleByEnergy;}
  virtual void MultiView();

 protected:
  Bool_t fScaleByEnergy; //!
  TEveProjectionManager *fRPhiMgr;
  TEveProjectionManager *fRhoZMgr;
  TEveViewer            *f3DView;
  TEveViewer            *fRPhiView;
  TEveViewer            *fRhoZView;
  TEveScene             *fRPhiScene;
  TEveScene             *fRhoZScene;

  EnsarIonName *fIonName;

    
 private:
  static EnsarEventManager*    fgRinstance; //!
  ClassDef(EnsarEventManager,1);

};


#endif
