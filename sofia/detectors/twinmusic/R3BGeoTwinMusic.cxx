/////////////////////////////////////////////////////////////
// R3BGeoTwinMusic
//
// Class for geometry of R3BTwinMusic
//
// Author: J.L. Rodriguez
// Created : 05/02/2012
//
/////////////////////////////////////////////////////////////

#include "R3BGeoTwinMusic.h"
#include "FairGeoNode.h"

#include <iostream>

using std::cout;
using std::endl;


ClassImp(R3BGeoTwinMusic)

R3BGeoTwinMusic::R3BGeoTwinMusic() {
  // Constructor
  fName="sts";
  maxSectors=0;
  maxModules=99;
}

const char* R3BGeoTwinMusic::getModuleName(Int_t m) {
  // Returns the module name of sts number m
  if ( m < 0 ) {
  	cout <<"-E- R3BGeoTwinMusic::getModuleName:: Module number "
  	       << m << " not known!" << endl;
	 return "";
	 }
  if ( m < 9 ) sprintf(modName,"twinmusic0%i",m+1);
  else  sprintf(modName,"twinmusic%i",m+1);
  return modName;
}

const char* R3BGeoTwinMusic::getEleName(Int_t m) {
  // Returns the element name of sts number m
  sprintf(eleName,"twinmusic%i",m+1);
  return eleName;
}
