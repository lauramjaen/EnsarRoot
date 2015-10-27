// -------------------------------------------------------------------------
// -----                EnsarAsciiIonGenerator source file                 -----
// -------------------------------------------------------------------------
#include "EnsarAsciiIonGenerator.h"

#include "FairPrimaryGenerator.h"
#include "FairIon.h"
#include "FairRunSim.h"

#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include "TRandom.h"

#include <iostream>
#include <fstream>

using namespace std;

using std::cout;
using std::endl;
using std::map;

// -----   Default constructor   ------------------------------------------
EnsarAsciiIonGenerator::EnsarAsciiIonGenerator() {}
// ------------------------------------------------------------------------



// -----   Standard constructor   -----------------------------------------
EnsarAsciiIonGenerator::EnsarAsciiIonGenerator(const char* fileName) {

  fPDG=TDatabasePDG::Instance();
  fFileName  = fileName;
  cout << "-I- EnsarAsciiIonGenerator: Opening input file " << fileName << endl;

  // Open first the file to register all new ions.
  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) 
    Fatal("EnsarAsciiIonGenerator","Cannot open input file.");
  cout << "-I- EnsarAsciiIonGenerator: Looking for ions..." << endl;

  Int_t nIons = RegisterIons();
  cout << "-I- EnsarAsciiIonGenerator: " << nIons << " ions registered." 
       << endl;
  CloseInput();

  // Re-Open the file for standard streaming ...
  fInputFile = new ifstream(fFileName);

  fX=fY=fZ=0.0;
  fPointVtxIsSet=kFALSE;
  fDX=fDY=fDZ=0.0;
  fBoxVtxIsSet=kFALSE;
 
}
// ------------------------------------------------------------------------



// -----   Destructor   ---------------------------------------------------
EnsarAsciiIonGenerator::~EnsarAsciiIonGenerator() {
  CloseInput();
}
// ------------------------------------------------------------------------



// -----   Public method ReadEvent   --------------------------------------
Bool_t EnsarAsciiIonGenerator::ReadEvent(FairPrimaryGenerator* primGen) {

  // Check for input file
  if ( ! fInputFile->is_open() ) {
    cout << "-E- EnsarAsciiIonGenerator: Input file not open!" << endl;
    return kFALSE;
  }

  // Define event variable to be read from file
  Int_t    eventId = 0;    // event id
  Int_t    nTracks = 0;    // number of tracks in the event
  Double_t pBeam   = 0.;   // beam energy
  Double_t b       = 0.;   // impact parameter


  // Define track variables to be read from file
  Int_t    iA      = 0;    // mass number
  Int_t    iZ      = 0;    // atomic number
  Double_t px      = 0.;   // momentum componets
  Double_t py      = 0.;
  Double_t pz      = 0.;
  Double_t vx      = 0.;   // vertex componets
  Double_t vy      = 0.;
  Double_t vz      = 0.;
  Double_t iMass   = 0.;   // excitation energy ??


  // Read event header line from input file
  *fInputFile >> eventId >> nTracks >> pBeam >> b;

  if ( fInputFile->eof() ) {
      cout << "-I- EnsarAsciiIonGenerator: End of input file reached " << endl;
      CloseInput();
      return kFALSE;
  }

  cout << "-I- EnsarAsciiIonGenerator: Reading Event: " << eventId << ",  pBeam = "
      << pBeam << "GeV, b = " << b << " fm, multiplicity " << nTracks
      << endl;

  // Loop over tracks in the current event
  for (Int_t itrack=0; itrack<nTracks; itrack++) {

      *fInputFile >> iZ >> iA >> px >> py >> pz >> vx >> vy >> vz >> iMass;

      Int_t pdgType=0;

      char ionName[20];
      if(1 == iZ && 2 == iA) {
        sprintf(ionName, "Deuteron");
      } else {
        sprintf(ionName, "Ion_%d_%d", iA, iZ);
      }
      TParticlePDG* part = fPDG->GetParticle(ionName);
      if ( ! part ) {
          cout << "-W- EnsarAsciiIonGenerator::ReadEvent: Cannot find "
	      << ionName << " in database!" << endl;
          continue;
      }
      pdgType = part->PdgCode();

      // Give track to PrimaryGenerator
      if (fPointVtxIsSet){ 
	vx = fX;
	vy = fY;
	vz = fZ;
	if (fBoxVtxIsSet) {
	  vx = gRandom->Gaus(fX,fDX);
	  vy = gRandom->Gaus(fY,fDY); 
	  vz = gRandom->Gaus(fZ,fDZ); 
	}         	
      }

      primGen->AddTrack(pdgType, px, py, pz, vx, vy, vz);

  }//! tracks

  return kTRUE;
}
// ------------------------------------------------------------------------
// -----   Private method CloseInput   ------------------------------------
void EnsarAsciiIonGenerator::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
       cout << "-I- EnsarAsciiIonGenerator: Closing input file " 
	    << fFileName << endl;
       fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------

// -----   Private method RegisterIons   ----------------------------------
Int_t EnsarAsciiIonGenerator::RegisterIons() {

  Int_t nIons = 0;
  Int_t eventId, nTracks;
  Double_t pBeam,b;

  // Define track variables to be read from file
  Int_t    iA      = 0;
  Int_t    iZ      = 0;
  Double_t px      = 0.;
  Double_t py      = 0.;
  Double_t pz      = 0.;
  Double_t vx      = 0.;
  Double_t vy      = 0.;
  Double_t vz      = 0.;
  Double_t iMass   = 0.;

  fIonMap.clear();

  while ( ! fInputFile->eof()) {
    *fInputFile >> eventId >> nTracks >> pBeam >> b;

    if ( fInputFile->eof() ) continue;

    for (Int_t iTrack=0; iTrack<nTracks; iTrack++) {
      *fInputFile >> iZ >> iA >> px >> py >> pz >> vx >> vy >> vz >> iMass;

	char buffer[20];
 	sprintf(buffer, "Ion_%d_%d", iA, iZ);
	TString ionName(buffer);
	if ( fIonMap.find(ionName) == fIonMap.end() ) {
	  FairIon* ion = new FairIon(ionName, iZ, iA, iZ, 0., iMass);
	  fIonMap[ionName] = ion;
	  nIons++;
	}  // new ion
    }// !tracks

  }//!events

  FairRunSim* run = FairRunSim::Instance();
  map<TString, FairIon*>::iterator mapIt;
  for (mapIt=fIonMap.begin(); mapIt!=fIonMap.end(); mapIt++) {
    FairIon* ion = (*mapIt).second;
    run->AddNewIon(ion);
  }

  return nIons;
}
// ------------------------------------------------------------------------

ClassImp(EnsarAsciiIonGenerator)

