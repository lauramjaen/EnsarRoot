// -------------------------------------------------------------------------
// -----                     Crystal header file                    -----
// -----                 Created 11/09/12  by P.Cabanelas              -----
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

/**  Crystal.h
 **/


#ifndef CRYSTAL_H
#define CRYSTAL_H

#include "EnsarDetector.h"

#include "TLorentzVector.h"

class TClonesArray;
class EnsarMyDetPoint;
class FairVolume;
class TGeoRotation;


class Crystal : public EnsarDetector
{

 public:

  /** Default constructor **/
  Crystal();


  /** Standard constructor.
   *@param name    detetcor name
   *@param active  sensitivity flag
   **/
  Crystal(const char* name, Bool_t active);


  /** Destructor **/
  virtual ~Crystal();


  /** Virtual method ProcessHits
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a EnsarMyDetPoint and adds it to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t ProcessHits(FairVolume* vol = 0);


  /** Virtual method BeginEvent
   **
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void BeginEvent();

	
  /** Virtual method EndOfEvent
   **
   ** Added support for EnsarMyDetCrystalHit
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void EndOfEvent();


  /** Virtual method Register
   **
   ** Registers the hit collection in the ROOT manager.
   **/
  virtual void Register();


  /** Accessor to the hit collection **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;


  /** Virtual method Print
   **
   ** Screen output of hit collection.
   **/
  virtual void Print() const;


  /** Virtual method Reset
   **
   ** Clears the hit collection
   **/
  virtual void Reset();


  /** Virtual method CopyClones
   **
   ** Copies the hit collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset
   **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2,
			  Int_t offset);


  /** Virtual method Construct geometry
   **
   ** Constructs the STS geometry
   **/
  virtual void ConstructGeometry(int type);

	
  virtual void Initialize();
  virtual void SetSpecialPhysicsCuts();
  void SetEnergyCutOff( Double_t cutE ){fCutE = cutE;}
  Double_t  GetEnergyCutOff ( ) {return fCutE;}


//  void SaveGeoParams();

  private:

    /** Track information to be stored until the track leaves the
	active volume. **/
    Int_t          fTrackID;           //!  track index
    Int_t          fTrackPID;          //!  particle identification
    Int_t          fVolumeID;          //!  volume id
    Int_t          fParentTrackID;     //!  parent track index
    Int_t          fUniqueID;          //!  particle unique id (e.g. if Delta electron, fUniqueID=9)
    TLorentzVector fPosIn, fPosOut;    //!  position
    TLorentzVector fMomIn, fMomOut;    //!  momentum
    Double32_t     fTime;              //!  time
    Double32_t     fLength;            //!  length
    Double32_t     fELoss;             //!  energy loss
    Double32_t     fCutE;              //!  energy loss
    Int_t          fPosIndex;          //!
    Bool_t         kGeoSaved;          //!
    TList*         flGeoPar;	       //!

    TClonesArray*  fMyDetCollection;              //!  The point collection

	
    /** Private method AddHit
     **
     ** Adds a MyDetPoint to the PointCollection
     **/
    EnsarMyDetPoint* AddHit(Int_t trackID, Int_t volumeID,
                        Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
			TVector3 posIn, TVector3 pos_out,
                        TVector3 momIn, TVector3 momOut,
                        Double_t time, Double_t length, Double_t eLoss);

	
	
    /** Private method ResetParameters
     **
     ** Resets the private members for the track parameters
     **/
    void ResetParameters();
	
    TGeoRotation* createMatrix( Double_t phi, Double_t theta, Double_t psi);

    ClassDef(Crystal,1);
};


inline void Crystal::ResetParameters() {
  fTrackID = fVolumeID = fParentTrackID = fTrackPID = fUniqueID = 0;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = 0;
  fPosIndex = 0;
};

#endif 
