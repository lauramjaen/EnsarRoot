// -------------------------------------------------------------------------
// -----                      R3BMWPC1Raw source file              -----
// -------------------------------------------------------------------------

#include "R3BMWPC1Raw.h"
#include "R3BMWPCPads.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;

// -----   Default constructor   -------------------------------------------
R3BMWPC1Raw::R3BMWPC1Raw() : FairMultiLinkedData() {
fMWPads = new TClonesArray("R3BMWPCPads");
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BMWPC1Raw::R3BMWPC1Raw(TClonesArray* Clon) : FairMultiLinkedData() {

fMWPads = Clon;

}

// -----   Destructor   ----------------------------------------------------
R3BMWPC1Raw::~R3BMWPC1Raw()
{
 delete fMWPads;
}
// -------------------------------------------------------------------------

// -----   Public method Print   ----------------------------------------------
void R3BMWPC1Raw::Print() const
{
  Int_t NbPads = fMWPads->GetEntriesFast();
  cout << "-I- R3BMWPC1Raw: " << NbPads << " Pads registered in this event."
       << endl;
}
// -------------------------------------------------------------------------
