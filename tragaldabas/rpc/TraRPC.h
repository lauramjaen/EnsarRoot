// -------------------------------------------------------------------------
// -----                        TraRPC header file                     -----
// -----           Created by H.Alvarez (hector.alvarez@usc.es)        -----
// -----           Last modification: 26/10/2014 by H. Alvarez         -----
// -------------------------------------------------------------------------

/**  TraRPC.h
 **/

#ifndef TRARPC_H
#define TRARPC_H

#include "EnsarDetector.h"
#include "TF1.h"
#include "TLorentzVector.h"

class TClonesArray;
class TraRPCPoint;
class TraRPCHit;
class FairVolume;
class TGeoRotation;

class TraRPC : public EnsarDetector
{

 public:
  /** Default constructor **/
  TraRPC();

  /** Standard constructor.
   *@param name    detector name
   *@param active  sensitivity flag
   **/
  TraRPC(const char* name, Bool_t active);

  /** Destructor **/
  virtual ~TraRPC();

  /** Virtual method ProcessHits
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a TraRPCPoint and adds it to the
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
   ** Added support for TraRPCHit
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
  virtual void Print(Option_t* option="") const;

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
   ** Constructs the geometry
   **/
  virtual void ConstructGeometry();

  virtual Bool_t CheckIfSensitive(std::string name);

  /** Select the version of the geometry
   **
   *@param version
   **/
  void SelectGeometryVersion(Int_t version);

  virtual void Initialize();
  virtual void SetSpecialPhysicsCuts() {}

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
    Int_t          fPosIndex;          //!
    Int_t          fNSteps;            //!  Number of steps in the active volume
    Double32_t     fEinc;              //!  Total incident energy
    Bool_t         kGeoSaved;          //!

    TClonesArray*  fRPCCollection;         //!  The points collection
    TClonesArray*  fRPCHitCollection;      //!  The hit collection

    // Selecting the geometry
    Int_t fGeometryVersion;

    /** Private method AddHit
     **
     ** Adds a RPCPoint to the HitCollection
     **/
    TraRPCPoint* AddHit(Int_t trackID, Int_t detID, Int_t volid,
			 TVector3 posIn, TVector3 pos_out, TVector3 momIn,
			 TVector3 momOut, Double_t time, Double_t length, Double_t eLoss);


    /** Private method AddCrystalHit
     **
     ** Adds a RPCHit to the HitCollection
     **/
    TraRPCHit* AddRPCHit(Int_t detID, Double_t charge, Double_t tof,
					Int_t steps, Double_t einc,
					Int_t trackid, Int_t volid, Int_t partrackid,
					Int_t pdgid, Int_t uniqueid,
                                        Double_t posxin, Double_t posyin);


    /** Private method ResetParameters
     **
     ** Resets the private members for the track parameters
     **/
    void ResetParameters();

    ClassDef(TraRPC,1);

    // Private method IsInsideActiveZone dice si el hit cae dentro de la zona activa
    // de la celda
    Bool_t IsInsideActiveZone(Float_t , Float_t, Float_t , Float_t );
};

inline void TraRPC::ResetParameters() {
  fTrackID = fVolumeID = fParentTrackID = fTrackPID = fUniqueID = 0;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = fEinc = 0;
  fPosIndex = 0;
  fNSteps = 0;
};


#endif
