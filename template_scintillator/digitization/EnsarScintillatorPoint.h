/** EnsarScintillatorPoint.h **/

#ifndef TSCINTIDETPOINT_H
#define TSCINTIDETPOINT_H

#include "TObject.h"
#include "TVector3.h"

#include "FairMCPoint.h"

/// EnsarScintillatorPoint.h
class EnsarScintillatorPoint : public FairMCPoint 
{

 public:

    /**
     * \author  M.I.Cherciu \n
     * Modified by: \n   
     * Comment: \n
     * \version 1.0 \n
     * \since 2014/07/02 \n
     * Change date: \n
     * \file EnsarScintillatorPoint.h
     * \brief Class for Monte Carlo hits of Scintillator Detector.
     */	
   
    
  /** Default Constructor **/
  EnsarScintillatorPoint();

 
     /** Constructor with arguments  
     * \param trackID  Index of MCTrack
     * \param detID    Detector ID
     * \param plane    Detector ID
     * \param posIn    Ccoordinates at entrance to active volume [cm]
     * \param posOut   Coordinates at exit of active volume [cm]
     * \param momIn    Momentum of track at entrance [GeV/c]
     * \param momOut   Momentum of track at exit [GeV/c]
     * \param tof      Time since event start [ns]
     * \param length   Track length since creation [cm]
     * \param eLoss    Energy deposit [GeV]
     **/
  
  EnsarScintillatorPoint(Int_t trackID, Int_t volumeID,
              Int_t trackPID, Int_t parentTrackID, Int_t uniqueID, 
              TVector3 posIn, TVector3 posOut, TVector3 momIn, TVector3 momOut,
	      Double_t energy, Double_t tof, Double_t length, Double_t eLoss, Double_t lightYield);

  /** Copy constructor. **/
  EnsarScintillatorPoint(const EnsarScintillatorPoint& point) { *this = point; };

  /** Default Destructor.  **/
  virtual ~EnsarScintillatorPoint();


  /** Get X coordinate of track at entrance to active volume [cm]. **/
  Double_t GetXIn()   const { return fX_in; }
  
  /** Get Y coordinate of track at entrance to active volume [cm]. **/
  Double_t GetYIn()   const { return fY_in; }
  
  /** Get X coordinate of track at entrance to active volume [cm]. **/
  Double_t GetZIn()   const { return fZ_in; }
  
  /** Get X coordinate of track at exit of active volume [cm]. **/
  Double_t GetXOut()  const { return fX_out; }
  
  /** Get Y coordinate of track at exit of active volume [cm]. **/
  Double_t GetYOut()  const { return fY_out; }
  
  /** Get Z coordinate of track at exit of active volume [cm]. **/
  Double_t GetZOut()  const { return fZ_out; }
  
  /** Get Px momentum of track at exit of active volume [Gev/c]. **/
  Double_t GetPxOut() const { return fPx_out; }
  
  /** Get Py momentum of track at exit of active volume [Gev/c]. **/
  Double_t GetPyOut() const { return fPy_out; }
  
  /** Get Pz momentum of track at exit of active volume [Gev/c]. **/
  Double_t GetPzOut() const { return fPz_out; }
  
  /** Get Light Yield of active volume [Gev/c]. **/
  Double_t GetLightYield() const {return fLightYield;}
  
  /** Get Energy Loss [GeV] at interaction point. **/   
   Double_t GetEnergyLoss(){return fELoss;}

  
  /** Three Vector Position Modifier at Entrance [cm]. **/
  void PositionIn(TVector3& pos)  { pos.SetXYZ(fX, fY, fZ); }

  /** Three Vector Position Modifier at Exit [cm]. **/
  void PositionOut(TVector3& pos) { pos.SetXYZ(fX_out,fY_out,fZ_out); }
  
  /** Three Vector Momentum Modifier [Gev/c]. **/
  void MomentumOut(TVector3& mom) { mom.SetXYZ(fPx_out,fPy_out,fPz_out); }

  /** Get Kinetic Energy [GeV] at Exit. **/   
   Double_t GetKinEnergy(){return fEnergy;}
  
  /** Get Z coordinate at given X [cm]. **/
  Double_t GetX(Double_t z) const;
  
  /** Get Z coordinate at given Y [cm]. **/
  Double_t GetY(Double_t z) const;


  /** Check for distance between in and out **/
  Bool_t IsUsable() const;


  /** Set Three Vector Position at Exit [cm]. **/
  void SetPositionOut(TVector3 pos);
  
  /** Set Three Vector Momentum at Exit [GeV/c]. **/  
  void SetMomentumOut(TVector3 mom);
   
  /** Get Time of Flight [s]. **/
   Double_t GetToF(){return fTof;}
 
  /** Printing to screen. **/
  virtual void Print(const Option_t* opt) const;


 protected:
 
  Int_t fTrackID;        /**< Track index. **/
  Int_t fVolumeID;       /**< Volume index. **/
  Int_t fParentTrackID;  /**< Parent track index. **/
  Int_t fTrackPID;       /**< Particle identification. **/
  Int_t fTrackUniqueID;  /**< Particle unique id. **/
   
  Double_t fELoss; /**< Energy Loss. **/
  Double_t fEnergy; /**< Kinetic Energy. **/
  Double_t fTof; /**< Time of Flight. **/
  Double_t fLightYield; /**< Light Yield. **/
  Double_t fX_out,  fY_out,  fZ_out; /**< Position Coordinates at exit. **/
  Double_t fPx_out, fPy_out, fPz_out; /**< Momentum Coordinates at exit. **/
  Double_t fX_in,  fY_in,  fZ_in; /**< Position Coordinates at entrance. **/
  Double_t fPx_in, fPy_in, fPz_in; /**< Momentum Coordinates at entrance. **/

  ClassDef(EnsarScintillatorPoint,1)

};



inline void EnsarScintillatorPoint::SetPositionOut(TVector3 pos) {
  fX_out = pos.X();
  fY_out = pos.Y();
  fZ_out = pos.Z();
}


inline void EnsarScintillatorPoint::SetMomentumOut(TVector3 mom) {
  fPx_out = mom.Px();
  fPy_out = mom.Py();
  fPz_out = mom.Pz();
}



#endif
