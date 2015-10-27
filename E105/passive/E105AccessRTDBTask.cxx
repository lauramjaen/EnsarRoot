/*
 * ********************************
 *     Interface: E105SlitPar     *
 * Author: M. Babai               *
 * Version:                       *
 * Licence:                       *
 * ********************************
 */
#include "E105AccessRTDBTask.h"

#include "FairRuntimeDb.h"
#include "FairRunAna.h"

ClassImp(E105AccessRTDBTask);

/** Default constructor **/
E105AccessRTDBTask::E105AccessRTDBTask()
{}

/** Destructor **/
E105AccessRTDBTask::~E105AccessRTDBTask()
{}

void E105AccessRTDBTask::SetParContainers()
{
  FairRunAna* ana = FairRunAna::Instance();
  FairRuntimeDb* rtdb=ana->GetRuntimeDb();
  fEnsarSlitPar = (E105SlitPar*) (rtdb->getContainer("E105SlitPar"));
}

InitStatus E105AccessRTDBTask::Init()
{
  if (!fEnsarSlitPar) {
    std::cout << "-E- E105AccessRTDBTask: fEnsarSlitPar Container does not exist!"
              << std::endl;
    return kERROR;
  }
  return kSUCCESS;
}

InitStatus E105AccessRTDBTask::ReInit()
{
  SetParContainers();
  return kSUCCESS;
}

void E105AccessRTDBTask::Exec(Option_t* opt)
{
  std::cout << " -I- Execute is called.\n"
            << "Printing the parameters.\n";
  fEnsarSlitPar->Print();
  
  std::cout << "exec is finished.\n";
  return;
}

void E105AccessRTDBTask::Finish()
{
  std::cout << " -I- E105AccessRTDBTask::Finish() *****\n";
  return;
}
