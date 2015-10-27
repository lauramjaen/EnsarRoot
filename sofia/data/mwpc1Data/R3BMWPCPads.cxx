// -------------------------------------------------------------------------
// -----                      R3BMWPCPads source file              -----
// -------------------------------------------------------------------------

#include "R3BMWPCPads.h"

#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
R3BMWPCPads::R3BMWPCPads() : FairMultiLinkedData() {
 fPad=-1;
 fPosX_Chargeup=0;
 fPosX_Chargedown=0;
 fPosY_Charge=0;
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
R3BMWPCPads::R3BMWPCPads(Int_t Pad, UShort_t PosX_Chargeup, UShort_t PosX_Chargedown, UShort_t PosY_Charge) 
  : FairMultiLinkedData() {
 fPad=Pad;
 fPosX_Chargeup=PosX_Chargeup;
 fPosX_Chargedown=PosX_Chargedown;
 fPosY_Charge=PosY_Charge;
}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
R3BMWPCPads::~R3BMWPCPads() { }
// -------------------------------------------------------------------------

// -----   Public method Print   -------------------------------------------
void R3BMWPCPads::Print(const Option_t* opt) const {
  cout << "-I- R3BMWPCPads:" << endl;
  cout << "Pad = " << fPad << endl;
  cout << "X Charge up = " << fPosX_Chargeup << endl;
  cout << "X Charge down = " << fPosX_Chargedown << endl;
  cout << "Y Charge = " << fPosY_Charge << endl;
}
// -------------------------------------------------------------------------
