/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/


// Ensar includes
#include "FairRootManager.h"
#include "FairRunSim.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"
#include "EnsarScintillatorPoint.h"
#include "EnsarScintillatorDigi.h"
#include "EnsarScintillatorDigiPar.h"
#include "EnsarScintillatorDigitizer.h"

// ROOT includes
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

// ----------------------------------------  Default Constructor Method of the Digitizer Class   ---------------------------
EnsarScintillatorDigitizer::EnsarScintillatorDigitizer()
: FairTask("Digitization scheme")
{
}

// ----------------------------------------  Standard Constructor Method of the Digitizer Class  ---------------------------
EnsarScintillatorDigitizer::EnsarScintillatorDigitizer(Int_t verbose)
: FairTask("Digitization scheme ", verbose)
{
}
// ---------------------------------------- DeConstructor Method of the Digitizer Class  -----------------------------------
EnsarScintillatorDigitizer::~EnsarScintillatorDigitizer()
{  
   if (fEnsarScintillatorDigiCollection) {
    fEnsarScintillatorDigiCollection->Delete();
    delete fEnsarScintillatorDigiCollection;
  }
   if (fScintillatorPoints) {
    fScintillatorPoints->Delete();
    delete fScintillatorPoints;
  }
   if (fMCTrack) {
    fMCTrack->Delete();
    delete fMCTrack;
  }
   if (fScintillatorDigiPar) {
    fScintillatorDigiPar->Delete();
    delete fScintillatorDigiPar;
  } 
}

// ---------------------------------------- Parameter Container Method of the Digitizer Class  ------------------------------
void EnsarScintillatorDigitizer::SetParContainers()
{
  // Get run and runtime database
  FairRunSim* run = FairRunSim::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");
  
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
  
  fScintillatorDigiPar = (EnsarScintillatorDigiPar*)(rtdb->getContainer("EnsarScintillatorDigiPar"));
  
  if ( fVerbose && fScintillatorDigiPar ) {
    LOG(INFO) << "EnsarScintillatorDigitizer::SetParContainers() "<< FairLogger::endl;
    LOG(INFO) << "Container EnsarScintillatorDigiPar loaded " << FairLogger::endl;
  }
}

// ---------------------------------------- Initialization of the Digitizer Class -------------------------------------------
InitStatus EnsarScintillatorDigitizer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  fScintillatorPoints = (TClonesArray*) ioman->GetObject("EnsarScintillatorPoint");
  
  // Register output array ScintillatorDigi
  fEnsarScintillatorDigiCollection = new TClonesArray("EnsarScintillatorDigi");
  ioman->Register("EnsarScintillatorDigi", "Digital response in Scintillator", fEnsarScintillatorDigiCollection, kTRUE);
  if ( fEnsarScintillatorDigiCollection ){LOG(INFO) << " Digitization Collection Loaded ..."<<FairLogger::endl;}
  
  c = 2.99792458E8;
  cMedia = 18.974;  // c*10 / 1.58 * 1e9; --> speed of light in BC408 material in [cm/ns]
  eventNo = 0;	
  
  // Initialise data structure
  PM_res = new PM_RES[1500];
  
  // Initialise control histograms
  // Control histogram on time
  hPM = new TH1F("PM", "Arrival times at PM", 100, 0., 10.);
  
  Int_t nbins = 100;  // bin numbers over the interval
  Float_t nmax = 50.; // maximum number of the interval

  // Control histogram on multiplicity
  hMult = new TH1F("Multiplicity", "Scintillator multiplicity", nbins, 0., nmax);

  return kSUCCESS;
}

// ---------------------------------------- Execution Method of the Digitizer Class   ---------------------------------------

void EnsarScintillatorDigitizer::Exec(Option_t* opt)
{
  // Reset entries in output arrays
  Reset();
  eventNo += 1;

  Double_t timeRes = 0.15; // ns

  // light attenuation of plastic scintillator
    att = 0.008; // [1/cm]
  //att = 0.0047619047619; // [1/cm]
  
  // decay constant of BC408 scintillator
  Double_t lambda=1./2.1; // [1/ns]
  
  // depth of a scintillator
 
 plength = fScintillatorDigiPar->GetScintillatorDepth(); // [cm]
 LOG(INFO) << " ++++++++++++ Length of the scintillator = "<<plength<<FairLogger::endl;
  
  Double_t tofRange = 40.; // [ns]
  Double_t Thresh = 0.16;
  Int_t nentries = fScintillatorPoints->GetEntries();
  Int_t PMmult = 0;
  Double_t QDC_temp, TDC_temp;
  Int_t mult = 0;
  Double_t xpos_temp, ypos_temp, zpos_temp;
  Double_t xpaddle, ypaddle, zpaddle;
  Double_t edep = 0.;

  LOG(INFO) << "EnsarScintillatorDigitizer ---> Points Entries = "<<nentries<<FairLogger::endl;   

  
  //---------------------------------------------- TO BE MODIFIED ACCORDINGLY WITH THE DETECTOR ------------------------------
 if(nentries > 0){  // exist hits in the event
  for (Int_t l=0; l < nentries; l++) {

    EnsarScintillatorPoint *land_obj = (EnsarScintillatorPoint*) fScintillatorPoints->At(l);
    
    Double_t eloss = land_obj->GetEnergyLoss()*1000.;
    Double_t light = land_obj->GetLightYield()*1000.;
    Double_t x = land_obj->GetXIn();
    Double_t y = land_obj->GetYIn();
    Double_t z = land_obj->GetZIn();
    Double_t time = land_obj->GetToF();
      
    if (eloss > 0.) { // eloss >0
      PMmult += 1; 
      edep += eloss;
      
      if (PMmult>1500) {
       LOG(ERROR) << "EnsarScintillatorDigitizer: Too many hits in one scintillator: "<< PMmult << " hits" << FairLogger::endl
        << "entry " << l << " after sorting" << FairLogger::endl
        << "scintillator mult " << PMmult << FairLogger::endl
        << "eloss " << eloss << FairLogger::endl
        << "light yield " << light << FairLogger::endl
        << "time " << time << FairLogger::endl
        << "X In " << x << FairLogger::endl
        << "Y In " << y << FairLogger::endl
        << "Z In " << z << FairLogger::endl;       
        return;
      }

      // get global point position of geometries
      gGeoManager->FindNode(x,y,z);
      gGeoManager->CdUp();
      Double_t local_point[] = {0., 0., 0.};
      Double_t global_point[3];
      gGeoManager->LocalToMaster(local_point, global_point);
      xpaddle = global_point[0];
      ypaddle = global_point[1];
      zpaddle = global_point[2];

      // scintillator 
        PM_res[PMmult].time = time+(plength-z)/cMedia;
        PM_res[PMmult].lightCFD = light*exp(-att*(plength-z));
        PM_res[PMmult].lightQDC = light*exp(-att*(plength-z));

    } // eloss

  } // MC hits

} // exist hits in the event

// Check for leading edge
  Double_t triggerTime=1e100;
//  Double_t triggerTime=40.;

    
    for (Int_t j = 0; j < PMmult; j++) { //digis
      PM_res[j+1].Energy=lambda*PM_res[j+1].lightCFD;

      if(PM_res[j+1].Energy > Thresh) { // if above threshold
        if(PM_res[j+1].time < triggerTime) { //find minimum
          triggerTime = PM_res[j+1].time;
        }// find minimum
      }// if above threshold
    }// digis
  

  Double_t temp[1500][3];
  
    Double_t tof = 0.;
    Double_t light = 0.;

    PM_res[0].Energy=0.;
    PM_res[0].time=PM_res[1].time;
    
    for(Int_t j = 0; j < PMmult; j++) {
        if(PM_res[j+1].Energy > Thresh) {

        // This is supposed to mimic a QDC and a TDC;
        // Check if light is larger than threshold and register time
        // Take also time resolution of sigma=timeRes ps into account

        Double_t num=gRandom->Gaus(0., timeRes);
        tof=PM_res[j+1].time+num;
        mult=mult+1;
        light=0.;
	
  // add all times inside +- tofRange (ns) for QDC

	if(TMath::Abs(PM_res[j+1].time-triggerTime) <= tofRange/2) {
            light = light + PM_res[j+1].lightQDC;
            hPM->Fill(PM_res[j+1].time);
        } // respect time gate
        
      } // if above threshold
    } // loop over hits in the event
   LOG(INFO)<<" Digits multiplicity = "<<mult<<FairLogger::endl;
    if(mult > 0) { // Multiplicity if the PM has fired
      
      light *= TMath::Exp((2.*(plength))*att/2.);

      Double_t k = 0.012;
      light = light / (1. + k*light);
      light = gRandom->Gaus(light, 0.05*light);   
     
      QDC_temp = TMath::Sqrt(light*light);
      TDC_temp = triggerTime - plength/cMedia;
            
      // vertical paddles
      xpos_temp = xpaddle;
      ypos_temp = ypaddle;
      zpos_temp = tof*cMedia;
      
      // Here is an example how to fill the EnsarScintillatorDigi structure
      Double_t qdc=QDC_temp;
      Double_t tdc=TDC_temp;
      Double_t xx=xpos_temp;
      Double_t yy=ypos_temp;
      Double_t zz=zpos_temp;
      
      AddHit(tdc, qdc, xx, yy, zz);
      
    } // Multiplicity if the PM has fired

// ---------------------------------------- ----------------------------------------  -------------------------------------
  
  // control histograms
  if(mult>0){
    hMult ->Fill(mult,1.);
  }
  
//  if(fVerbose) {
//    LOG(INFO) << "EnsarScintillatorDigitizer: produced "
//    << fEnsarScintillatorDigiCollection->GetEntries() << " digis" << FairLogger::endl;
//  }

  
}

// ---------------------------------------- Reset Method of the Digitizer Class   ------------------------------------------
void EnsarScintillatorDigitizer::Reset()
{
  if (fEnsarScintillatorDigiCollection) {
    fEnsarScintillatorDigiCollection->Clear();
  }   
}

// ---------------------------------------- FinishEvent Method called at the end of events  --------------------------------

void EnsarScintillatorDigitizer::FinishEvent()
{
}
// ---------------------------------------- Finish Method called at the end of events --------------------------------------
void EnsarScintillatorDigitizer::Finish()
{
  hPM->Write();
  hMult->Write();
  delete [] PM_res;
}
// ---------------------------------------- AddHit Method caleld for each digit  ----------------------------------------
EnsarScintillatorDigi* EnsarScintillatorDigitizer::AddHit(Double_t tdc, Double_t qdc, Double_t xx, Double_t yy, Double_t zz)
{
  EnsarScintillatorDigi *digi = new((*fEnsarScintillatorDigiCollection)[fEnsarScintillatorDigiCollection->GetEntriesFast()]) EnsarScintillatorDigi( tdc, qdc, xx, yy, zz);
  return digi;
}
// ---------------------------------------- ----------------------------------------  ---------------------------------------

ClassImp(EnsarScintillatorDigitizer)
