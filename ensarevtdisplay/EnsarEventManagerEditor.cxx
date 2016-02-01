// -------------------------------------------------------------------------
// -----            EnsarEventManagerEditor source file                -----
// -----            Created 21/09/2012  by P.Cabanelas                 -----
// -------------------------------------------------------------------------

#include "EnsarEventManagerEditor.h"

#include "EnsarEventManager.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "TGLabel.h"
#include "TGButton.h"
#include "TGWindow.h"
#include "TGNumberEntry.h"
#include "TGeoManager.h"
#include "TChain.h"
#include "TEveGValuators.h"

#include <iostream>
using namespace std;



ClassImp(EnsarEventManagerEditor)



EnsarEventManagerEditor::EnsarEventManagerEditor(const TGWindow* p, Int_t width, Int_t height,
					     UInt_t options, Pixel_t back)
:TGedFrame(p, width, height, options | kVerticalFrame, back),
  fObject(0),
  fManager(EnsarEventManager::Instance()),
  fCurrentEvent(0),
  fCurrentPDG(0),
  fVizPri(0),
  fMinEnergy(0),
  fMaxEnergy(0)
  
{
  Init();
}

void EnsarEventManagerEditor::Init(){

  MakeTitle("EnsarEventManager Editor");

  TGVerticalFrame      *fInfoFrame= CreateEditorTabSubFrame("Ensar");

  TGCompositeFrame *title1 = new TGCompositeFrame(fInfoFrame, 250, 10,
						  kVerticalFrame | kLHintsExpandX |
						  kFixedWidth    | kOwnBackground);

  fScaleE = new TGCheckButton(title1, "Scale by Energy");
  AddFrame(fScaleE, new TGLayoutHints(kLHintsTop, 3, 1, 1, 0));
  fScaleE->Connect("Toggled(Bool_t)", "EnsarEventManagerEditor", this, "DoScaleE()");
  title1->AddFrame(fScaleE);
  
  //TGTextButton *fUpdate = new TGTextButton(title1, "Update");
  //title1->AddFrame(fUpdate, new TGLayoutHints(kLHintsRight | kLHintsExpandX, 5,5,1,1));
  //fUpdate->Connect("Clicked()", "FairEventManagerEditor", this, "SelectEvent()");
  
  fInfoFrame->AddFrame(title1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

  
} 


//______________________________________________________________________________
void EnsarEventManagerEditor::MaxEnergy()
{
  fManager->SetMaxEnergy(fMaxEnergy->GetValue());
  
}
//______________________________________________________________________________
void EnsarEventManagerEditor::MinEnergy()
{
  fManager->SetMinEnergy(fMinEnergy->GetValue());
  
}

//______________________________________________________________________________
void EnsarEventManagerEditor::DoVizPri()
{
  if (fVizPri->IsOn())fManager->SetPriOnly(kTRUE);
  else  fManager->SetPriOnly(kFALSE);
}
//______________________________________________________________________________
void EnsarEventManagerEditor::SelectPDG()
{
  fManager->SelectPDG(fCurrentPDG->GetIntNumber());
}

//______________________________________________________________________________
void EnsarEventManagerEditor::SelectEvent()
{
  fManager->GotoEvent(fCurrentEvent->GetIntNumber());
  
  fMinEnergy->SetLimits(fManager->GetEvtMinEnergy(), fManager->GetEvtMaxEnergy(), 100);
  fMinEnergy->SetValue(fManager->GetEvtMinEnergy());
  fMaxEnergy->SetLimits(fManager->GetEvtMinEnergy(), fManager->GetEvtMaxEnergy(), 100);
  fMaxEnergy->SetValue( fManager->GetEvtMaxEnergy());
  Update();
  
}
//______________________________________________________________________________
void EnsarEventManagerEditor::SetModel( TObject* obj)
{
  fObject     = obj;  
}

void EnsarEventManagerEditor::DoScaleE()
{
  if (fScaleE->IsOn()) ((EnsarEventManager*) fManager)->SetScaleByEnergy(kTRUE);
  else  ((EnsarEventManager*) fManager)->SetScaleByEnergy(kFALSE);
}




