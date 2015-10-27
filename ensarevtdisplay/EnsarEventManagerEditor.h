
#ifndef ENSAREVENTMANAGEREDITOR
#define ENSAREVENTMANAGEREDITOR

#include "TGedFrame.h"
class EnsarEventManager;
class TGNumberEntry;
class TGCheckButton;
class TEveGValuator;

class EnsarEventManagerEditor : public TGedFrame
{
   EnsarEventManagerEditor(const EnsarEventManagerEditor&);            // Not implemented
   EnsarEventManagerEditor& operator=(const EnsarEventManagerEditor&); // Not implemented

protected:
   TObject *fObject;
   EnsarEventManager  *fManager;
   TGNumberEntry  *fCurrentEvent, *fCurrentPDG;
   TGCheckButton  *fVizPri;
   TGCheckButton  *fScaleE;
   //TGCheckButton  *fProjections;
   TEveGValuator *fMinEnergy ,*fMaxEnergy;  

public:
   EnsarEventManagerEditor(const TGWindow* p=0, Int_t width=170, Int_t height=30,
                      UInt_t options = kChildFrame, Pixel_t back=GetDefaultFrameBackground());
   virtual ~EnsarEventManagerEditor() {}
   void SetModel( TObject* obj);
   virtual void SelectEvent();
   virtual void SelectPDG();                  
   void DoVizPri();
   virtual void MaxEnergy();
   virtual void MinEnergy();
   virtual void Init();
   virtual void DoScaleE();
   //virtual void ShowProjections();
   ClassDef(EnsarEventManagerEditor, 0); // Specialization of TGedEditor for proper update propagation to TEveManager.
};

#endif
