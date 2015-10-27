/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0 / 2014
*    ------------------------------------------------------------------------
*/

// EnsarRoot includes
#include "EnsarSiliconDigi.h"

// C++ includes
#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
EnsarSiliconDigi::EnsarSiliconDigi()
  : fTime(0.), fEnergy(0.), fxplane(0.), fyplane(0.), fzplane(0.)
{
}

// -----   Standart constructor   -------------------------------------------
EnsarSiliconDigi::EnsarSiliconDigi(Double_t time, Double_t energy, Double_t xplane,Double_t yplane, Double_t zplane)
  : fTime(time), fEnergy(energy), fxplane(xplane), fyplane(yplane), fzplane(zplane)
{
}

// -----   Copy constructor   -------------------------------------------
EnsarSiliconDigi::EnsarSiliconDigi(const EnsarSiliconDigi& right)
  : fTime(right.fTime), fEnergy(right.fEnergy),
    fxplane(right.fxplane), fyplane(right.fyplane), fzplane(right.fzplane)
{
}

// -----   Destructor   ----------------------------------------------------
EnsarSiliconDigi::~EnsarSiliconDigi()
{
}

// -----   Public method Print   -------------------------------------------
void EnsarSiliconDigi::Print(const Option_t* opt) const
{
  cout << "-I- EnsarSiliconDigi: Digi in detector " << endl;
  cout << "    Time: " << fTime << endl;
  cout << "    Energy: " << fEnergy << endl;
}
// -------------------------------------------------------------------------

ClassImp(EnsarSiliconDigi)
