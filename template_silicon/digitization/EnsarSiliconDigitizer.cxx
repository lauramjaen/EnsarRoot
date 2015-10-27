/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0 / 2014
*    ------------------------------------------------------------------------
*/

// EnsarRoot includes
#include "EnsarSiliconPoint.h"
#include "EnsarSiliconDigi.h"
#include "EnsarSiliconDigiPar.h"
#include "EnsarSiliconDigitizer.h"

// FariRoot includes
#include "FairRootManager.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

// Root includes
#include "TGeoManager.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TMath.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"

// C++ includes
#include <string>
#include <iostream>
#include <fstream>


// ----------------------------------------------------------------------------
EnsarSiliconDigitizer::EnsarSiliconDigitizer()
: FairTask("Digitization scheme")
{
}

// ----------------------------------------------------------------------------
EnsarSiliconDigitizer::EnsarSiliconDigitizer(Int_t verbose)
: FairTask("Digitization scheme ", verbose)
{
}

// ----------------------------------------------------------------------------
EnsarSiliconDigitizer::~EnsarSiliconDigitizer()
{
   if (fEnsarSiliconDigiCollection) {
    fEnsarSiliconDigiCollection->Delete();
    delete fEnsarSiliconDigiCollection;
  }
   if (fSiliconPoints) {
    fSiliconPoints->Delete();
    delete fSiliconPoints;
  }
   if (fMCTrack) {
    fMCTrack->Delete();
    delete fMCTrack;
  }
   if (fSiliconDigiPar) {
    fSiliconDigiPar->Delete();
    delete fSiliconDigiPar;
  } 
}

// ----------------------------------------------------------------------------
void EnsarSiliconDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunSim* run = FairRunSim::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");
  
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
  
  fSiliconDigiPar = (EnsarSiliconDigiPar*)(rtdb->getContainer("EnsarSiliconDigiPar"));
  
  if ( fVerbose && fSiliconDigiPar ) {
    LOG(INFO) << "EnsarSiliconDigitizer::SetParContainers() "<< FairLogger::endl;
    LOG(INFO) << "Container EnsarSiliconDigiPar loaded " << FairLogger::endl;
  }
}

// ----------------------------------------------------------------------------
InitStatus EnsarSiliconDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  fSiliconPoints = (TClonesArray*) ioman->GetObject("EnsarSiliconPoint");
  
  // Register output array SiliconDigi
  fEnsarSiliconDigiCollection = new TClonesArray("EnsarSiliconDigi");
  ioman->Register("EnsarSiliconDigi", "Digital response in Silicon", fEnsarSiliconDigiCollection, kTRUE);
  if ( fEnsarSiliconDigiCollection ){LOG(INFO) << " Digitization Collection Loaded ..."<<FairLogger::endl;}
  
  eventNo = 0;	
   
  // Initialise control histograms
  // Control histogram on time
  hSD = new TH1F("SD", "Arrival times at SD", 100, 0., 100.);
  
  Int_t nbins = 100;
  Float_t nmax = 100.;

  // Silicon detector
  // nbins = 20;
  // nmax = 20.;
  // Control histogram on multiplicity
  hMult = new TH1F("Multiplicity", "Silicon multiplicity", nbins, 0., nmax);

  return kSUCCESS;
}
// -------------------------------------------------------------------------



// -----   Public method Exec   --------------------------------------------
void EnsarSiliconDigitizer::Exec(Option_t* opt)
{
  // Reset entries in output arrays
  Reset();
  eventNo += 1;
  
  Double_t Thresh = 0.12; // silicon detector treshold to produce a digit
  Int_t nentries = fSiliconPoints->GetEntries(); // MC entries 
  Int_t SDmult = 0; // sensitve detector multiplicity
  Double_t xplane, yplane, zplane; // point of entrance of the track in sensitve detector
  Double_t edep = 0.; // deposited energy over the steps  
  
  LOG(INFO) << "EnsarSiliconPoints: "<<nentries<<" entries"<<FairLogger::endl;

  if(nentries > 0){  // exist hits in the event
   for (Int_t l=0; l < nentries; l++) {

    EnsarSiliconPoint *sdpoint_obj = (EnsarSiliconPoint*) fSiliconPoints->At(l);
    
    Double_t eloss = sdpoint_obj->GetEnergyLoss()*1000.; // in MeV
    Double_t x = sdpoint_obj->GetXIn();
    Double_t y = sdpoint_obj->GetYIn();
    Double_t z = sdpoint_obj->GetZIn();
    Double_t time = sdpoint_obj->GetToF();
    
    // digi generation only if energy deposit greater than threshold
    if (eloss > Thresh) {
      SDmult += 1;
      edep += eloss;
//      Int_t m = SDmult;
      
     if (SDmult>100) {
       LOG(ERROR) << "EnsarSiliconDigitizer: Too many hits in one silicon: "<< SDmult << " hits" << FairLogger::endl
        << "entry " << l << " after sorting" << FairLogger::endl
        << "silicon mult " << SDmult << FairLogger::endl
        << "eloss " << eloss << FairLogger::endl
        << "time " << time << FairLogger::endl
        << "X In " << x << FairLogger::endl
        << "Y In " << y << FairLogger::endl
        << "Z In " << z << FairLogger::endl;       
        return;
      }

      xplane = x;
      yplane = y;
      zplane = z;
      
    AddHit(time, eloss, xplane, yplane, zplane);

    } // eloss
  
  } // MC hits

} // exist hits in the event

  // control histograms
  if(SDmult>0){
    hMult ->Fill(SDmult,1.);
  }
  
  if(fVerbose) {
    LOG(INFO) << "EnsarSiliconDigitizer: "
    << fEnsarSiliconDigiCollection->GetEntries() << " digis" << FairLogger::endl;
  }

  
}

// ----------------------------------------------------------------------------
void EnsarSiliconDigitizer::Reset()
{
  if (fEnsarSiliconDigiCollection) {
    fEnsarSiliconDigiCollection->Clear();
  }   
}
// ----------------------------------------------------------------------------
void EnsarSiliconDigitizer::FinishEvent()
{
}
// ----------------------------------------------------------------------------
void EnsarSiliconDigitizer::Finish()
{
  hSD->Write();
  hMult->Write();
}

// ----------------------------------------------------------------------------
EnsarSiliconDigi* EnsarSiliconDigitizer::AddHit(Double_t time, Double_t energy, Double_t xplane, Double_t yplane, Double_t zplane)
{
  EnsarSiliconDigi *digi = new((*fEnsarSiliconDigiCollection)[fEnsarSiliconDigiCollection->GetEntriesFast()]) EnsarSiliconDigi( time, energy, xplane, yplane, zplane);
  return digi;
}
// ----------------------------------------------------------------------------


ClassImp(EnsarSiliconDigitizer)
