/////////////////////////////////////////////////////////////////
//*-- AUTHOR : Pablo Cabanelas    pablo.cabanelas@usc.es
//*-- Date: 18/09/2012
// --------------------------------------------------------------
// Description:
//   Run Configuration
//
// --------------------------------------------------------------
/////////////////////////////////////////////////////////////////

#include "G4Version.hh"
#include "EnsarRunConfiguration.h"

#include "TG4ComposedPhysicsList.h"
#include "TG4SpecialPhysicsList.h"

#if G4VERSION_NUMBER > 922
#include <QGSP_BERT.hh>
#else
#include <LHEP_BERT.hh>
#endif
#include "EnsarPhysicsList.h"

//_____________________________________________________________________________
EnsarRunConfiguration::EnsarRunConfiguration(const TString& userGeometry,
                                             const TString& specialProcess)
  : TG4RunConfiguration(userGeometry, "emStandard", specialProcess, true) 
{


}

//_____________________________________________________________________________
EnsarRunConfiguration::~EnsarRunConfiguration()
{
/// Destructor
}

//
// protected methods
//


//_____________________________________________________________________________
G4VUserPhysicsList*  EnsarRunConfiguration::CreatePhysicsList()
{
/// Override the default physics list with user defined physics list;
/// LHEP_BERT physics list should be replaced with user own physics list

  TG4ComposedPhysicsList* builder = new TG4ComposedPhysicsList();
  
  // User physics list
  G4cout << G4endl;
  G4cout << "-I- EnsarRunConfiguration Adding -Ensar- SpecialPhysicsList ...  " << G4endl;
  builder->AddPhysicsList(new EnsarPhysicsList());
    
//  G4cout << "Adding SpecialPhysicsList " << G4endl;
//  builder->AddPhysicsList(new TG4SpecialPhysicsList(
//                                 fSpecialProcessSelection.Data()));
  
  return builder;  
}  


