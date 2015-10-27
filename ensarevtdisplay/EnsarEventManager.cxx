// -------------------------------------------------------------------------
// -----               EnsarEventManager source file                   -----
// -----             Created 21/09/2012  by P.Cabanelas                -----
// -------------------------------------------------------------------------

#include "EnsarEventManager.h"
#include "EnsarIonName.h"
#include "EnsarMultiView.h"
#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TGeoManager.h"
#include "TDatabasePDG.h"
#include "TEveViewer.h"
#include "TEveScene.h"
#include "TEveBrowser.h"
#include "TEveProjectionManager.h"
#include "TEveProjectionAxes.h"
#include "TGLViewer.h"


#include <iostream>
using namespace std;


EnsarEventManager *EnsarEventManager::fgRinstance= 0;

EnsarEventManager * EnsarEventManager::Instance(){
        return fgRinstance;
}


EnsarEventManager::EnsarEventManager()
  :FairEventManager()
{
  cout << " calling ctor Event Manager" << endl;
   fgRinstance=this;

}

void EnsarEventManager::MultiView() {

   EnsarMultiView *mv = new EnsarMultiView();

   if(!gEve->GetViewers()->UseLightColorSet())
	   gEve->GetViewers()->SwitchColorSet(); //white background

   gEve->Redraw3D(kTRUE);

}

void EnsarEventManager::AddParticlesToPdgDataBase(Int_t pdgCode){

  TDatabasePDG *pdgDB = TDatabasePDG::Instance();
 
 if ( !pdgDB->GetParticle(pdgCode))
  {
    int temp=pdgCode/1E8;
    temp = pdgCode - temp*1E8;
    int element = temp/1E4;
    int mass = (temp - element*1E4)/10;

    bool particleRecognised=true;
    char name[20];

    particleRecognised = ((EnsarIonName*) fIonName)->GetIonName(element,name);
    
    if (particleRecognised)
    {
      char title[20];
      sprintf(title, "%s%i",name, mass);
      pdgDB->AddParticle(name, title, mass, kTRUE,
                         0, 0, "Ion", pdgCode);
    }
  }  
}


ClassImp(EnsarEventManager)
