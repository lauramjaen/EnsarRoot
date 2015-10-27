// -------------------------------------------------------------------------
// -----                      R3BMWPC2Raw source file              -----
// -------------------------------------------------------------------------

#include "R3BMWPC2Raw.h"
#include "R3BMWPCPads.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BMWPC2Raw::R3BMWPC2Raw() : FairMultiLinkedData() {
fMWPads = new TClonesArray("R3BMWPCPads");
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BMWPC2Raw::R3BMWPC2Raw(TClonesArray* Clon) : FairMultiLinkedData() {

fMWPads = Clon;

}

// -----   Destructor   ----------------------------------------------------
R3BMWPC2Raw::~R3BMWPC2Raw()
{
 delete fMWPads;
}
// -------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void R3BMWPC2Raw::Print() const
{
  Int_t NbPads = fMWPads->GetEntriesFast();
  cout << "-I- R3BMWPC2Raw: " << NbPads << " Pads registered in this event."
       << endl;
}
// -------------------------------------------------------------------------
