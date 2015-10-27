// -------------------------------------------------------------------------
// -----                  EnsarHPGeDetHit header file                  -----
// -----                Created 04/09/19  by P.Cabanelas               -----
// -------------------------------------------------------------------------


/**  EnsarHPGeDetHit.h
 **/


#ifndef ENSARHPGEDETHIT_H
#define ENSARHPGEDETHIT_H


#include "TObject.h"
#include "TVector3.h"

#include "FairMultiLinkedData.h"


class EnsarHPGeDetHit : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  EnsarHPGeDetHit();


  /** Constructor with arguments
   *@param trackID         Index of MCTrack
   *@param volumeID        Active Volume ID
   *@param fTrackPID;      particle identification
   *@param fParentTrackID; parent track index
   *@param fUniqueID;      particle unique id (e.g. if Delta electron, fUniqueID=9)
   *@param fEnergy         Energy deposit [GeV]
   *@param fTime           Time since event start [ns]
   *@param fSteps          Number of steps
   *@param fEinc           Incident energy [GeV]
   **/
  EnsarHPGeDetHit(Int_t trackID, Int_t volumeID,
      Int_t trackPID, Int_t parentTrackID, Int_t uniqueID,
      Double_t energy, Double_t time, Int_t steps, Double_t einc);


  /** Copy constructor **/
  EnsarHPGeDetHit(const EnsarHPGeDetHit& hit) { *this = hit; };


  /** Destructor **/
  virtual ~EnsarHPGeDetHit();


  /** Accessors **/
  Double_t GetEnergy()        const { return fEnergy; }
  Double_t GetTime()          const { return fTime; }

  /** Modifiers **/
  void SetEnergy(Double32_t energy) { fEnergy = energy; }
  void SetTime(Double32_t time)     { fTime = time; }

  void AddMoreEnergy(Double32_t moreEnergy) { fEnergy += moreEnergy; }

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;



 protected:

  Int_t fTrackID;        //track index
  Int_t fVolumeID;       //volume index
  Int_t fParentTrackID;  //parent track index
  Int_t fTrackPID;       //particle identification
  Int_t fTrackUniqueID;  //particle unique id

  Double32_t fEnergy;    //total energy in the crystal
  Double32_t fTime;      //time of the interaction
  Double32_t fEinc;      //incident energy in the crystal
  Int_t      fNSteps;    //number of interaction steps

  ClassDef(EnsarHPGeDetHit,1)

};


#endif
