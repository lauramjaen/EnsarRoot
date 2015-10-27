// -------------------------------------------------------------------------
// -----                        R3BMWpc header file                    -----
// -----                  Created 08/02/12  by J.L. Rodriguez          -----
// -----					   		       -----
// -------------------------------------------------------------------------

/**  R3BMWpc.h
 **/


#ifndef R3BMWpc_H
#define R3BMWpc_H

#include "R3BDetector.h"

#include "TLorentzVector.h"

class TClonesArray;
class R3BMWpcPoint;
class R3BMWpcHit;
class FairVolume;
class TGeoRotation;


class R3BMWpc : public R3BDetector
{

 public:

  /** Default constructor **/
  R3BMWpc();


  /** Standard constructor.
   *@param name    detector name
   *@param active  sensitivity flag
   **/
  R3BMWpc(const char* name, Bool_t active);


  /** Destructor **/
  virtual ~R3BMWpc();


  /** Virtual method ProcessHits
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates a R3BCaloPoint and adds it to the
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

  	
  /** Virtual method EndOfEvent
   **
   ** Added support for R3BMWpcHit
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void EndOfEvent();


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
  virtual void ConstructGeometry();
	
	
  void SetNonUniformity( Double_t nonU );

	
  virtual void Initialize();
  virtual void SetSpecialPhysicsCuts();
  void SetEnergyCutOff( Double_t cutE ){fCutE = cutE;}
  Double_t  GetEnergyCutOff ( ) {return fCutE;}


//  void SaveGeoParams();

  private:

    /** Track information to be stored until the track leaves the
	active volume. **/
    Int_t          fTrackID;           //!  track index
    Int_t          fVolumeID;          //!  volume id
    TLorentzVector fPosIn, fPosOut;    //!  position
    TLorentzVector fMomIn, fMomOut;    //!  momentum
    Double32_t     fTime;              //!  time
    Double32_t     fLength;            //!  length
    Double32_t     fELoss;             //!  energy loss
    Double32_t     fCutE;              //!  energy loss
    Int_t          fPosIndex;          //!
    TClonesArray*  fMWpcCollection;     //!  The hit collection
    TClonesArray*  fMWpcHitCollection;  //!  The hit collection
    Bool_t         kGeoSaved;          //!
    TList *flGeoPar;		       //!

    
    Int_t fDetectorType[16];
    Int_t fPlaneType[2];


	// Adding some non-uniformity preliminary description
	Double_t  fNonUniformity;
	
	/** Private method AddHit
     **
     ** Adds a RpcPoint to the HitCollection
     **/
    R3BMWpcPoint* AddHit(Int_t trackID, Int_t detID, Int_t volid, Int_t copy, Int_t ident,
			TVector3 posIn,
			TVector3 pos_out, TVector3 momIn, 
			TVector3 momOut, Double_t time, 
			Double_t length, Double_t eLoss);

	
	/** Private method Addmwpc1Hit
     **
     ** Adds a Rpchit to the HitCollection
     **/
    R3BMWpcHit* Addmwpc1Hit(Int_t type, Int_t copy, Int_t ident,
				Double_t energy, Double_t tof);
	
	
	/** Private method NUSmearing
     **
     ** Smears the energy according to some non-uniformity distribution	
	 ** Very simple preliminary scheme where the NU is introduced as a flat random
	 ** distribution with limits fNonUniformity (%) of the energy value.
	 **/
    Double_t NUSmearing(Double_t inputEnergy);
	
	
    /** Private method ResetParameters
     **
     ** Resets the private members for the track parameters
     **/
    void ResetParameters();
	
   TGeoRotation* createMatrix( Double_t phi, Double_t theta, Double_t psi);

   Int_t  GetDetectorType(Int_t volID);
   Int_t  GetPlaneType(Int_t volID);

    ClassDef(R3BMWpc,3);
};

inline Int_t R3BMWpc::GetDetectorType(Int_t volID) {

Int_t type=-1;

for (Int_t i=0;i<16;i++ ){
    if (volID==fDetectorType[i]) {
	type=i+1; 
	return (type);
    }
}
return type;
}

inline Int_t R3BMWpc::GetPlaneType(Int_t volID) {

Int_t type=-1;

for (Int_t i=0;i<2;i++ ){
    if (volID==fPlaneType[i]) {
	type=i+1; 
	return (type);
    }
}
return type;
}



inline void R3BMWpc::ResetParameters() {
  fTrackID = fVolumeID = 0;
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = 0;
  fPosIndex = 0;
};

#endif 
