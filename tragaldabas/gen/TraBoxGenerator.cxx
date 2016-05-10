// -------------------------------------------------------------------------
// -----          TraBoxGenerator source file                          -----
// -----          Created 19/06/15  by Javier Diaz		       -----
// -----          Modified 02/02/16 by pablo.cabanelas@usc.es          -----
// -------------------------------------------------------------------------

#include "TraBoxGenerator.h"

#include "FairPrimaryGenerator.h"

#include "TRandom.h"
#include "TParticlePDG.h"
#include "TDatabasePDG.h"
#include "TMath.h"

// ------------------------------------------------------------------------
TraBoxGenerator::TraBoxGenerator() :
fPDGType(0),fMult(0),fPDGMass(0),fPtMin(0),fPtMax(0),
fPhiMin(0),fPhiMax(0),fEtaMin(0),fEtaMax(0),fYMin(0),fYMax(0),
fPMin(0),fPMax(0),fThetaMin(0),fThetaMax(0),fX(0),fY(0),fZ(0),
fX1(0),fY1(0), fZ1(0),fX2(0),fY2(0),fZ2(0),
fEtaRangeIsSet(0),fYRangeIsSet(0),fThetaRangeIsSet(0),
fCosThetaIsSet(0),fPtRangeIsSet(0),fPRangeIsSet(0),
fPointVtxIsSet(0),fBoxVtxIsSet(0),fDebug(0),
fxMax(0),fxMin(0),fyMax(0),fyMin(0),
fXhRangeIsSet(0),fYhRangeIsSet(0),
xgen(0),ygen(0),zgen(0),xdown(0), ydown(0), zup(0), zdown(0), xup(0), yup(0)
{
  // Default constructor
}

// ------------------------------------------------------------------------
TraBoxGenerator::TraBoxGenerator(Int_t pdgid, Int_t mult) :
fPDGType(pdgid),fMult(mult),fPDGMass(0),fPtMin(0),fPtMax(0),
fPhiMin(0),fPhiMax(0),fEtaMin(0),fEtaMax(0),fYMin(0),fYMax(0),
fPMin(0),fPMax(0),fThetaMin(0),fThetaMax(0),fX(0),fY(0),fZ(0),
fX1(0),fY1(0), fZ1(0),fX2(0),fY2(0),fZ2(0),
fEtaRangeIsSet(0), fYRangeIsSet(0),fThetaRangeIsSet(0),
fCosThetaIsSet(0), fPtRangeIsSet(0), fPRangeIsSet(0),
fPointVtxIsSet(0),fBoxVtxIsSet(0),fDebug(0),
fxMax(0),fxMin(0),fyMax(0),fyMin(0),
fXhRangeIsSet(0),fYhRangeIsSet(0),
xgen(0),ygen(0),zgen(0),xdown(0), ydown(0), zup(0), zdown(0), xup(0), yup(0)
{
  // Constructor. Set default kinematics limits
  SetPhiRange();
}
// ------------------------------------------------------------------------
Bool_t  TraBoxGenerator::Init()
{
  // Initialize generator


  if (fPhiMax-fPhiMin>360)
    Fatal("Init()","TraBoxGenerator: phi range is too wide: %f<phi<%f",
          fPhiMin,fPhiMax);
  if (fPRangeIsSet && fPtRangeIsSet) {
    Fatal("Init()","TraBoxGenerator: Cannot set P and Pt ranges simultaneously");
  }
  if (fPRangeIsSet && fYRangeIsSet) {
    Fatal("Init()","TraBoxGenerator: Cannot set P and Y ranges simultaneously");
  }
  if ( (fThetaRangeIsSet && fYRangeIsSet) ||
       (fThetaRangeIsSet && fEtaRangeIsSet) ||
       (fYRangeIsSet     && fEtaRangeIsSet) ) {
    Fatal("Init()","TraBoxGenerator: Cannot set Y, Theta or Eta ranges simultaneously");
  }
  if (fPointVtxIsSet && fBoxVtxIsSet) {
    Fatal("Init()","TraBoxGenerator: Cannot set point and box vertices simultaneously");
  }

  // Check for particle type
  TDatabasePDG* pdgBase = TDatabasePDG::Instance();
  TParticlePDG* particle = pdgBase->GetParticle(fPDGType);

  if (! particle) {
    Fatal("TraBoxGenerator","PDG code %d not defined.",fPDGType);
  } else {
    fPDGMass = particle->Mass();
    return kTRUE;
  }

  return kTRUE;
}

// ------------------------------------------------------------------------

Bool_t TraBoxGenerator::ReadEvent(FairPrimaryGenerator* primGen)
{
  // Generate one event: produce primary particles emitted from one vertex.
  // Primary particles are distributed uniformly along
  // those kinematics variables which were limitted by setters.
  // if SetCosTheta() function is used, the distribution will be uniform in
  // cos(theta)

  Double32_t pabs=0., phi=0., pt=0., theta=0., eta=0.,
      y=0., mt=0., px=0., py=0., pz=0.,
      DistPlanoGen=0., delta=5., PlaneSelector=0.5;

  // Generate particles
  for (Int_t k = 0; k < fMult; k++) {
    phi = gRandom->Uniform(fPhiMin,fPhiMax) * TMath::DegToRad();

    if      (fPRangeIsSet ) { pabs = gRandom->Uniform(fPMin,fPMax); }
    else if (fPtRangeIsSet) { pt   = gRandom->Uniform(fPtMin,fPtMax); }

    if      (fThetaRangeIsSet) {
      if (fCosThetaIsSet)
        theta = acos(gRandom->Uniform(cos(fThetaMin* TMath::DegToRad()),
                                      cos(fThetaMax* TMath::DegToRad())));
      else {
        theta = gRandom->Uniform(fThetaMin,fThetaMax) * TMath::DegToRad();
      }
    } else if (fEtaRangeIsSet) {
      eta   = gRandom->Uniform(fEtaMin,fEtaMax);
      theta = 2*TMath::ATan(TMath::Exp(-eta));
    } else if (fYRangeIsSet) {
      y     = gRandom->Uniform(fYMin,fYMax);
      mt = TMath::Sqrt(fPDGMass*fPDGMass + pt*pt);
      pz = mt * TMath::SinH(y);
    }

    if (fThetaRangeIsSet || fEtaRangeIsSet) {
      if      (fPRangeIsSet ) {
        pz = pabs*TMath::Cos(theta);
        pt = pabs*TMath::Sin(theta);
      } else if (fPtRangeIsSet) {
        pz = pt/TMath::Tan(theta);
      }
    }



    //Parameters for the evaluation of the geometric efficiency
    //Events are generated in a plane located at a given distance above the RPC planes

    DistPlanoGen=100.;                                 //Distance to the generator plane
    PlaneSelector=gRandom->Uniform();                 //Random selection of the RPC plane (only two planes are considered)

   if (PlaneSelector < 0.5){                          //Hit definition in the lower plane and proyected to the generator plane

      xdown=gRandom->Uniform(fxMin,fxMax);
      ydown=gRandom->Uniform(fxMin,fxMax);
      zdown=0;

      xgen=xdown + DistPlanoGen*TMath::Cos(phi)*TMath::Tan(theta);  //Event generation coordinates
      ygen=ydown + DistPlanoGen*TMath::Sin(phi)*TMath::Tan(theta);
      zgen=DistPlanoGen;

        if (fPointVtxIsSet){             //Set the vertex coordinates as the generation point in the plane
           fX=xgen;
           fY=ygen;
           fZ=zgen;
        }
     }

   if (PlaneSelector > 0.5){                          //Hit definition in the upper plane and proyected to the generator plane
    xup=gRandom->Uniform(fxMin,fxMax);
    yup=gRandom->Uniform(fyMin,fyMax);
    zup=90;

    xgen=xup + (DistPlanoGen-zup)*TMath::Cos(phi)*TMath::Tan(theta);  //Event generation coordinates
    ygen=yup + (DistPlanoGen-zup)*TMath::Sin(phi)*TMath::Tan(theta);
    zgen=DistPlanoGen;

        if (fPointVtxIsSet){             //Set the vertex coordinates as the generation point in the plane
           fX=xgen;
           fY=ygen;
           fZ=zgen;
        }

     }



    px = pt*TMath::Cos(phi);
    py = pt*TMath::Sin(phi);

    if (fBoxVtxIsSet) {
      fX = gRandom->Uniform(fX1,fX2);
      fY = gRandom->Uniform(fY1,fY2);
    }




    if (fDebug)
      printf("BoxGen: kf=%d, p=(%.2f, %.2f, %.2f) GeV, x=(%.1f, %.1f, %.1f) cm\n",
             fPDGType, px, py, pz, fX, fY, fZ);

primGen->AddTrack(fPDGType, px, py, pz, fX, fY, fZ);
  }
  return kTRUE;

}

// ------------------------------------------------------------------------



// ------------------------------------------------------------------------


ClassImp(TraBoxGenerator)
