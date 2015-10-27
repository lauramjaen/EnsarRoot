/** EnsarScintillator.h **/


#ifndef TSCINTIDET_H
#define TSCINTIDET_H

#include "EnsarDetector.h"
#include "EnsarScintillatorDigiPar.h"
#include "EnsarScintillatorGeoPar.h"
#include "EnsarScintillatorPoint.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class EnsarScintillatorPoint;
class FairVolume;
class TClonesArray;

/// EnsarScintillator.h

class EnsarScintillator: public EnsarDetector
{
  public:

    /**
     * \author  M.I.Cherciu \n
     * Modified by: \n   
     * Comment: \n
     * \version 1.0 \n
     * \since 2014/07/02 \n
     * Change date: \n
     * \file EnsarScintillator.h
     * \brief Class for geometry definition/construction of Scintillator Detector.\n
     */	

    /**     \fn EnsarScintillator();
     *      Default contructor. */    
    EnsarScintillator();

    /**     \fn EnsarScintillator(const char* Name, Bool_t Active);
     *      Create object.
     *      \param Name :  Detector Name.
     *      \param Active : kTRUE for active detectors (ProcessHits() will be called).
     *      \param Active : kFALSE for inactive detectors. */
    EnsarScintillator(const char* Name, Bool_t Active);


    /**     \fn ~EnsarScintillator();
     *      Default descontructor.  */   
    virtual ~EnsarScintillator();

    /**     \fn ProcessHits( FairVolume* vol = 0);
     *     Hits processing in the volumes.
     *      This method is called for each step during simulation
     *       (see FairMCApplication::Stepping())  */
    
    virtual Bool_t ProcessHits(FairVolume* vol = 0);

    /**     \fn Register()
     *      Registers the produced collections in FAIRRootManager. */
    virtual void   Register();


    /**     \fn (Int_t iColl) const ;
     *      Gets the produced collections. */
    virtual TClonesArray* GetCollection(Int_t iColl) const ;

    /**     \fn Reset()
     *      Has to be called after each event to reset the containers. */
    virtual void   Reset();

    /**     \fn ConstructGeometry()
     *      Create the detector geometry. */
    void ConstructGeometry();
    
    /**     \fn  SetPositionRotation(Double_t x, Double_t y, Double_t z, Double_t rx, Double_t ry, Double_t rz);
     *      Positioning and rotating of the detector geometry. */
    int SetPositionRotation(Double_t x, Double_t y, Double_t z, Double_t rx, Double_t ry, Double_t rz);

    /**     \fn CopyClones( TClonesArray* cl1, TClonesArray* cl2, Int_t offset)
     *      Make a copy of the TClonesArray.*/
    virtual void   CopyClones( TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {;}

    /**     \fn SetSpecialPhysicsCuts()
     *      Set cuts for the physics. */    
    virtual void   SetSpecialPhysicsCuts() {;}
    
    /**     \fn Initialize();
     *      Initialization of the detector is done here. */    
    virtual void   Initialize();
    
    
    /**     \fn BeginEvent()
     *      This function is called at the start of an event.*/
    virtual void   BeginEvent() {;}
    
    /**     \fn EndOfEvent()
     *      This function is called at the end of an event.*/   
    virtual void   EndOfEvent();
    
    /**     \fn BeginPrimary()
     *      This function is called at the start of the primaries.*/ 
    virtual void   BeginPrimary() {;}

    /**     \fn FinishPrimary()
     *      This function is called at the end of the primaries.*/
    virtual void   FinishPrimary() {;}

    /**     \fn FinishRun()
     *      This function is called at the end of the run.*/
    virtual void   FinishRun() {;}

    /**     \fn PostTrack()
     *      This function is called for accesing Track information.*/
    virtual void   PostTrack() {;}
    
    /**     \fn PreTrack()
     *      This function is called for accesing Track information.*/
    virtual void   PreTrack() {;}


  private:
    
   
    Int_t          fTrackID;           /**<  Track index */
    Int_t          fVolumeID;          /**<  Volume id */
    TLorentzVector fPos;               /**<  Position at entrance */
    TLorentzVector fMom;               /**<  Momentum at entrance */
    TGeoCombiTrans* pos_rot;           /**<  Geometrical transformation matrix */
    Double_t xx;		       /**<  X position */
    Double_t yy;		       /**<  Y position */
    Double_t       fMass;              /**<  Mass */
    Double_t       fEnergy;            /**<  Energy */
    Double_t       fTime;              /**<  Time */
    Double_t       fLength;            /**<  Length */
    Double_t       fELoss;             /**<  Energy loss */
    Double_t       fLightYield;        /**<  Light yield */
    Int_t          fTrackPID;          /**<  Particle identification */
    Int_t          fParentTrackID;     /**<  Parent track index */
    Int_t          fUniqueID;          /**<  Particle unique id (e.g. if Delta electron, fUniqueID=9) */
    TLorentzVector fPosIn, fPosOut;    /**<  Position */
    TLorentzVector fMomIn, fMomOut;    /**<  Momentum */
    Double32_t     fCutE;              /**<  Energy loss */
    Bool_t         kGeoSaved;          /**<  Flag for geometry activation */
    Double_t fBirkC0,fBirkC1,fBirkC2;  /**<  Birk's constants */

  
    TClonesArray*  fEnsarScintillatorPointCollection; /**< Container for data points */
    
    /**     \fn EnsarScintillatorPoint* AddHit(Int_t trackID, Int_t detID, TVector3 pos, TVector3 mom, Double_t time, Double_t length, Double_t eLoss);
     *      Adding of points into TClonesArray.
     *      This method is an example of how to add your own points
     *      of type EnsarScintillatorPoint to the clones array */
    EnsarScintillatorPoint* AddHit(Int_t trackID, Int_t volumeID,
                        Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
			TVector3 posIn, TVector3 pos_out,
                        TVector3 momIn, TVector3 momOut,
                        Double_t energy, Double_t time, Double_t length, Double_t eLoss, Double_t lightYield);

    void ResetParameters();

    ClassDef(EnsarScintillator,1)
};

inline void EnsarScintillator::ResetParameters() {
  fTrackID = fVolumeID = fParentTrackID = fTrackPID = fUniqueID = 0;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = 0;
};


#endif //TSCINTIDET_H
