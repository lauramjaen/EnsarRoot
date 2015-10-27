/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/

// Ensar includes
#include "EnsarScintillatorDigi.h"

// C++ includes
#include <iostream>

using std::cout;
using std::endl;
using std::flush;


// -----   Default constructor   -------------------------------------------
EnsarScintillatorDigi::EnsarScintillatorDigi()
  : fTdc(0.), fQdc(0.), fxx(0.), fyy(0.), fzz(0.)
{
}

// -----   Standart constructor   -------------------------------------------
EnsarScintillatorDigi::EnsarScintillatorDigi(Double_t tdc, Double_t qdc, Double_t xx,Double_t yy, Double_t zz )
  : fTdc(tdc), fQdc(qdc),
    fxx(xx), fyy(yy), fzz(zz)
{
}

// -----   Copy constructor   -------------------------------------------
EnsarScintillatorDigi::EnsarScintillatorDigi(const EnsarScintillatorDigi& right)
  : fTdc(right.fTdc), fQdc(right.fQdc),
    fxx(right.fxx), fyy(right.fyy), fzz(right.fzz)
{
}

// -----   Destructor   ----------------------------------------------------
EnsarScintillatorDigi::~EnsarScintillatorDigi()
{
}

// -----   Public method Print   -------------------------------------------
void EnsarScintillatorDigi::Print(const Option_t* opt) const
{
  cout << "-I- EnsarScintillatorDigi: Digi in detector " << endl;
  cout << "    Tdc: " << fTdc << endl;
  cout << "    Qdc: " << fQdc << endl;
}
// -------------------------------------------------------------------------

ClassImp(EnsarScintillatorDigi)
