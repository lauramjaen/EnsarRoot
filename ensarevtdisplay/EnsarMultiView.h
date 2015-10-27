// ------------------------------------------------------------------------
// -----                EnsarMultiView header file                    -----
// -----             Created 21/09/2012  by P.Cabanelas               -----
// ------------------------------------------------------------------------


#ifndef EnsarMultiView_H
#define EnsarMultiView_H

#include <TEveManager.h>

#include <TEveViewer.h>
#include <TGLViewer.h>

#include <TEveScene.h>
#include <TEveGeoShape.h>

#include <TEveProjectionManager.h>
#include <TEveProjectionAxes.h>

#include <TEveBrowser.h>
#include <TEveWindow.h>

//______________________________________________________________________________
// Short description of EnsarMultiView
//

class EnsarMultiView
{
public:
  EnsarMultiView();
  virtual ~EnsarMultiView() {}

  static EnsarMultiView* Instance();

  void InitGeomGentle(TEveGeoShape* g3d, TEveGeoShape* grphi, TEveGeoShape* grhoz);

  //-------------------------------------------------------------------------

  void SetDepth(Float_t d);

  //-------------------------------------------------------------------------

  void ImportGeomRPhi(TEveElement* el);
  void ImportGeomRhoZ(TEveElement* el);
  void ImportEventRPhi(TEveElement* el);
  void ImportEventRhoZ(TEveElement* el);

  void DestroyEventRPhi();
  void DestroyEventRhoZ();

  void SetCenterRPhi(Double_t x, Double_t y, Double_t z);
  void SetCenterRhoZ(Double_t x, Double_t y, Double_t z);

  void DestroyAllGeometries();

  //-------------------------------------------------------------------------

  TEveViewer* Get3DView()   { return f3DView; }
  TEveViewer* GetRPhiView() { return fRPhiView; }
  TEveViewer* GetRhoZView() { return fRhoZView; }


protected:
  TEveProjectionManager *fRPhiMgr; // Obvious meaning.
  TEveProjectionManager *fRhoZMgr; // Obvious meaning.

  TEveViewer            *f3DView;   // Obvious meaning.
  TEveViewer            *fRPhiView; // Obvious meaning.
  TEveViewer            *fRhoZView; // Obvious meaning.

  TEveScene             *fRPhiGeomScene;  // Obvious meaning.
  TEveScene             *fRhoZGeomScene;  // Obvious meaning.
  TEveScene             *fRPhiEventScene; // Obvious meaning.
  TEveScene             *fRhoZEventScene; // Obvious meaning.

  TEveGeoShape          *fGeomGentle;     // Obvious meaning.
  TEveGeoShape          *fGeomGentleRPhi; // Obvious meaning.
  TEveGeoShape          *fGeomGentleRhoZ; // Obvious meaning.

  static EnsarMultiView* fgInstance;     // Obvious meaning.

private:
  EnsarMultiView(const EnsarMultiView&);            // Not implemented
  EnsarMultiView& operator=(const EnsarMultiView&); // Not implemented

  ClassDef(EnsarMultiView, 0); // Multiple-views.
};

#endif
