#include "ESRGenerator.hh"

using namespace std;

ESRGenerator::ESRGenerator()
{
  // set ascii file name
  //////////////////////
  setASCIFileName("B150Ni56onalphaElas.dat");

  // particle definition 
  //////////////////////
  Part_t *Ni56X = PDefinition("56NiX",-1, 28, 56, 28, 52.0955 , 2.0e-2);   // 56Ni 
  Part_t *Ni56  = PDefinition("56Ni", -1, 28, 56, 28, 52.0955 , 0.);       // 56Ni 
  Part_t *Ni58  = PDefinition("58Ni", -1, 28, 58, 28, 53.9522 , 0.);       // 58Ni 
  Part_t *Fe52  = PDefinition("52Fe", -1, 26, 52, 26, 48.3761 , 0.);       // 52Fe 
  Part_t *alpha = PDefinition("alpha", 1,  0, 1000020040,  2,  3.72738, 0.);  // alpha with pdg
  Part_t *proton= PDefinition("proton",1,  0, 2212,  2,  0.938272, 0.);  // proton with pdg

  // set number of Events
  ///////////////////////
  setNEvents(1000000);

  // set deacy flag
  /////////////////
  setDecayFlag("off");
  setEjectileLimits(70,90,-15,15);

  // set interaction position
  ///////////////////////////
  setVertexPosition(0,0,0);
  
  // set interaction profile width
  ///////////////////////////////
  setInteractionProfileWidth(0,0,0);   // [mm]

  // impose constrain on light ejectile
  // set event modification flag
  /////////////////////////////////////
  setLimitsFlag("off");
  //  setEjectileLimits(70,87,-15,15);

  // incoming channel
  ///////////////////
  incomingChannel(Ni58,proton,0.4*58); // beam#1, target#2, beam kin energy

  // outgoing channel
  ///////////////////
  if(getDecayFlag()=="off")     outgoingChannel(proton,Ni58);
  else if(getDecayFlag()=="on") outgoingChannel(alpha,Ni56X,Fe52,alpha);
}

ESRGenerator::~ESRGenerator() 
{}

// Initialization for decay process
///////////////////////////////////
void ESRGenerator::decayChannel(TLorentzVector *pm)
{
  // produce decay fragments
  TLorentzVector W2(pm->Px(),pm->Py(),pm->Pz(),pm->E());
  event2->SetDecay(W2, 2, mass2);
  event2->Generate();
}

// set outgoing channel (light ejectile , heavy projectile,
// heavy fragment, light fragment)
/////////////////////////////////////////////////////////// 
void ESRGenerator::outgoingChannel(Part_t *ePart,  Part_t *pPart,
				   Part_t *hfPart, Part_t *lfPart) 
{
  Double_t mass1[]={ePart->m,pPart->m};
  event1 = new TGenPhaseSpace();
  event1->SetDecay(W1, 2, mass1); 

  // final channel
  mass2[0] = hfPart->m;
  mass2[1] = lfPart->m;
  event2   = new TGenPhaseSpace();

  // pointer assignments
  part[0] = ePart;
  part[1] = pPart;
  part[2] = hfPart;
  part[3] = lfPart;
}

// set outgoing channel (light ejectile , heavy projectile)
///////////////////////////////////////////////////////////
void ESRGenerator::outgoingChannel(Part_t *ePart, Part_t *pPart)
{
  Double_t mass1[]={ePart->m,pPart->m};
  event1 = new TGenPhaseSpace();
  event1->SetDecay(W1, 2, mass1); 

  // pointer assignments
  part[0] = ePart;
  part[1] = pPart;
}

// set incoming channel (beam particle, target particle, beam kinetic energy)
/////////////////////////////////////////////////////////////////////////////
void ESRGenerator::incomingChannel(Part_t *bPart, Part_t *tPart, Double_t kin)
{
  // target 4-vector
  TLorentzVector target(0.0, 0.0, 0.0, tPart->m);  

  // beam 4-vector
  Double_t Ene = kin + bPart->m;
  Double_t Mom = TMath::Sqrt(Ene*Ene-bPart->m*bPart->m); 
  beam = TLorentzVector(0.0, 0.0, Mom, Ene);

  // total 4-vector in the lab frame
  W1 = beam + target;
}

// Vertex position 
//////////////////
void ESRGenerator::setVertexPosition(Double_t vx, Double_t vy, Double_t vz)
{
  fVX=vx; fVY=vy; fVZ=vz;
}

// Interaction profile width 
////////////////////////////
void ESRGenerator::setInteractionProfileWidth(Double_t sx, Double_t sy, Double_t sz)
{
  fSX=sx; fSY=sy; fSZ=sz;

  // Random number generator initialization
  posGen = new TRandom();
}

// particle definition 
//////////////////////
Part_t *ESRGenerator::PDefinition(TString name, Int_t pdg, 
		   UInt_t  z, UInt_t  a, Int_t q, Double_t m, Double_t e) 
{
  Part_t *newPart = new Part_t;

  newPart->name = name;
  newPart->pdg = pdg;
  newPart->z = z; newPart->a=a; newPart->q=q; 
  newPart->e=e;   newPart->m = m + e;   // this has to be in GeV

  return newPart;
}

// get vertex position
//////////////////////
void ESRGenerator::getVertexPosition()
{
  Double_t x = fVX; 
  Double_t y = fVY; 
  Double_t z = fVZ; 
  if(fSX) x += posGen->Gaus(0,fSX);
  if(fSY) y += posGen->Gaus(0,fSY);
  if(fSZ) z += posGen->Gaus(0,fSZ);

  vertexPos.SetXYZ(x,y,z);
}

// print to asci file in required format
////////////////////////////////////////
void ESRGenerator::printToASCIFile(Int_t eId, Int_t nTracks)
{
  outFile << eId << " " << nTracks << " " << 0 << " " << 0 << "\n";
  for(Int_t i=0; i<nTracks; i++) {
    if(nTracks==1 && i==1) continue;
  outFile << part[i]->pdg << " " << part[i]->z << " " << part[i]->a << " " 
	  << pPar[i]->Px() << " " << pPar[i]->Py() << " " << pPar[i]->Pz() 
	  << " " << vertexPos.X() << " " << vertexPos.Y() << " " << vertexPos.Z() << " " 
	  << part[i]->m << "\n";
  }
}

// generate engine (GENBOD)
// event generation based on the phase space
////////////////////////////////////////////
void ESRGenerator::genbod()
{
  // open asci file
  /////////////////
  outFile.open(getASCIFileName());

  Int_t cnt=0;  // good events

  for(Int_t i=0; i<nEvents; i++) {
    // generate event
    event1->Generate();
  
    // First decay products
    pPar[0] = event1->GetDecay(0);
    pPar[1] = event1->GetDecay(1);
    
    // produce and then set vertex position for extended target
    getVertexPosition();

    // decide to accept or reject the event, or correct it
    if(correctAcceptReject()) {
            
      // if asked, decay on-the-fly
      if(getDecayFlag()=="on") {
	decayChannel(pPar[1]);
	
	// do a new decay 
	pPar[2] = event2->GetDecay(0);
	pPar[3] = event2->GetDecay(1);
	
	// write into the file
	printToASCIFile(cnt,3);
      }
      else {
	// write into the file
	printToASCIFile(cnt,2);
      }
      // number good events in file
      cnt++;
    }
  }

  // close the asci file
  //////////////////////
  outFile.close();
}

Bool_t ESRGenerator::correctAcceptReject()
{
  if(limitsFlag=="off") return true;

  // obtain angles
  Double_t r2d = 180./TMath::Pi();
  Double_t phi = pPar[0]->Phi()*r2d;
  Double_t the = pPar[0]->Theta()*r2d;

  if(the>minTheta && the<maxTheta) {
  
    if(phi>minPhi && phi<maxPhi) 
      return true;
    else {

      if(phi>maxPhi && phi<180) {
	phi-=maxPhi;
	phi = maxPhi*phi/(180-maxPhi);
	pPar[0]->SetPhi(phi/r2d);
	pPar[1]->SetPhi(phi/r2d-180./r2d);
      }
      else if(phi<-maxPhi && phi>-180) {
	phi+=maxPhi;
	phi = maxPhi*phi/(180-maxPhi);
	pPar[0]->SetPhi(phi/r2d);
	pPar[1]->SetPhi(phi/r2d+180./r2d);      
      }
    }
    return true;
  }

  return false;
}

void ESRGenerator::setEjectileLimits(Double_t min_the, Double_t max_the, 
				     Double_t min_phi, Double_t max_phi)
{
  minTheta = min_the; maxTheta = max_the;
  minPhi   = min_phi; maxPhi   = max_phi;
  limitsFlag="on";
}
