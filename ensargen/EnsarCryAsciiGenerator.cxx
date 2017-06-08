// ------------------------------------------------------------------------
//                 EnsarCryAsciiGenerator source file                 -----
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ----------  Use this generator with an appropiate ascii file  ----------
// ----------     generated with CRY with the "pdg" option       ----------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

#include "EnsarCryAsciiGenerator.h"

#include "FairPrimaryGenerator.h"
#include "FairRunSim.h"

#include "TDatabasePDG.h"
#include "TParticlePDG.h"

#include <iostream>
#include <fstream>

using namespace std;

using std::cout;
using std::endl;
using std::map;
using std::ifstream;

// -----   Default constructor   ------------------------------------------
EnsarCryAsciiGenerator::EnsarCryAsciiGenerator() {}
// ------------------------------------------------------------------------

// -----   Standard constructor   -----------------------------------------
EnsarCryAsciiGenerator::EnsarCryAsciiGenerator(const char* fileName) 
{

  fPDG=TDatabasePDG::Instance();
  fFileName  = fileName;
  cout << "-I- EnsarCryAsciiGenerator: Opening input file " << fileName << endl;

  fInputFile = new ifstream(fFileName);
  if ( ! fInputFile->is_open() ) Fatal("EnsarCryAsciiGenerator","Cannot open input file.");

  fX=fY=fZ=0.0;
  fPointVtxIsSet=kFALSE;
  fDX=fDY=fDZ=0.0;
  fBoxVtxIsSet=kFALSE;
}
// ------------------------------------------------------------------------


// -----   Destructor   ---------------------------------------------------
EnsarCryAsciiGenerator::~EnsarCryAsciiGenerator()
{

  CloseInput();

}
// ------------------------------------------------------------------------

// -----   Public method ReadEvent   --------------------------------------
Bool_t EnsarCryAsciiGenerator::ReadEvent(FairPrimaryGenerator* primGen) 
{

  streampos prevpos;

  // Define event variable to be read from file
  Int_t    eventId = 0;    // event id
  Int_t    nTracks = 0;    // number of tracks in the event
  Int_t    fPrevEventId=0;

  // Define track variables  
  Int_t pdgType    = 0;    // PDG code 
  Double_t KEn     = 0.;   // kinetic energy read in MeV
  Double_t cosA    = 0.;   // direction cosines
  Double_t cosB    = 0.;
  Double_t cosC    = 0.;
  Double_t vx      = 0.;   // vertex componets
  Double_t vy      = 0.;
  Double_t vz      = 0.;

  // Intermediate varibles   
  Double_t p       = 0.;   // total momentum
  Double_t px      = 0.;   // momentum componets
  Double_t py      = 0.;
  Double_t pz      = 0.;
  Double_t mass    = 0.;   // particle mass read in GeV

  Bool_t read = true;

  *fInputFile >> eventId >> nTracks >> pdgType >> KEn  >> vx >> vy >> vz >> cosA >> cosB >> cosC;

  if ( fInputFile->eof() ) {
    cout << "-I- EnsarCryAsciiGenerator: End of input file reached " << endl;
    CloseInput();
    return kFALSE;
  }
 
  while (read && !fInputFile->eof()) {

    // We extract the particle mass from its object
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(pdgType);
    mass = particle->Mass();

    // Calculations
    vx  = vx*100; // input should be in cm
    vy  = vy*100;
    vz  = vz+100;
    KEn = KEn/1000.0; // energy should be in GeV
    p   = TMath::Sqrt((KEn+mass)*(KEn+mass)-mass*mass); 
    px  = p*cosA;
    py  = p*cosB;
    pz  = p*cosC;
    pz  = pz;

    // We create the track
    primGen->AddTrack(pdgType, px, py, pz, vx, vy, vz);

    fPrevEventId = eventId;

    // Read the next line in the file, check if it belongs to the same event, and go back to previous line
    prevpos = fInputFile->tellg();
    *fInputFile >> eventId >> nTracks >> pdgType >> KEn  >> vx >> vy >> vz >> cosA >> cosB >> cosC;
    if(fPrevEventId != eventId){ 
      read = false;  
      fInputFile->seekg(prevpos);
    }

  }

  return kTRUE;

}  

// ------------------------------------------------------------------------
// -----   Private method CloseInput   ------------------------------------
void EnsarCryAsciiGenerator::CloseInput() {
  if ( fInputFile ) {
    if ( fInputFile->is_open() ) {
      cout << "-I- EnsarCryAsciiGenerator: Closing input file "
        << fFileName << endl;
      fInputFile->close();
    }
    delete fInputFile;
    fInputFile = NULL;
  }
}
// ------------------------------------------------------------------------


