
#include <TStopwatch.h>
#include <FairLmdSource.h>
#include <R3BCaloUnpack.h>
#include <FairRunOnline.h>
//#include <R3BAladinFieldMap.h>
#include <R3BCaloRawAna.h>
#include <FairParRootFileIo.h>
//#include <R3BFieldPar.h>
#include <FairRuntimeDb.h>

int main(int argc, char** argcv)
{
  TStopwatch timer;
  timer.Start();


  // Run over all events
  Int_t nev = -1;
  
  
  // Create source with unpackers ----------------------------------------------
//  FairRemoteSource* source  = new FairRemoteSource("lxi047");
  FairLmdSource* source  = new FairLmdSource();
  source->AddFile("Co60_2014-06-20-2300_350V0214.lmd");

//  char* strCalDir = "/home/work/example/";
  Short_t type = 10;
  Short_t subType = 1;
  Short_t procId = 1;
  Short_t subCrate = -1; // All sub-crates
  Short_t control = 9;

  source->AddUnpacker(new R3BCaloUnpack("", type, subType,
                                        procId, subCrate, control));
  // ---------------------------------------------------------------------------


  // Create online run ---------------------------------------------------------
  FairRunOnline* run = new FairRunOnline(source);
  run->SetOutputFile("calo_raw_data.root");
  //  run->SetGenerateHtml(kTRUE);
  // ---------------------------------------------------------------------------
  
  
  // Create ALADIN field map ---------------------------------------------------
  //  R3BAladinFieldMap* magField = new R3BAladinFieldMap("AladinMaps");
  //Double_t fMeasCurrent = 2500.;// I_current [A]
  //magField->SetCurrent(fMeasCurrent);
  //magField->SetScale(1.);
  //run->SetField(magField);
  // ---------------------------------------------------------------------------
  
  // Add analysis task ---------------------------------------------------------
  R3BCaloRawAna* ana = new R3BCaloRawAna();
  run->AddTask(ana);
  // ---------------------------------------------------------------------------
  
  // Initialize ----------------------------------------------------------------
  run->Init();
  // ---------------------------------------------------------------------------
  
  
  // Runtime data base ---------------------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  //R3BFieldPar* fieldPar = (R3BFieldPar*) rtdb->getContainer("R3BFieldPar");
  //fieldPar->SetParameters(magField);
  //fieldPar->setChanged();
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open("r3bpar.root");
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  // ---------------------------------------------------------------------------
  
  
  // Run -----------------------------------------------------------------------
  run->Run(nev, 0);
  // ---------------------------------------------------------------------------


  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  std::cout << std::endl << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
  std::cout << "Real time " << rtime << " s, CPU time " << ctime
	    << "s" << std::endl << std::endl;
}

#define _GNU_SOURCE
#include <dlfcn.h>
//#define _FCNTL_H
//#include <bits/fcntl.h>
#define _SIGNAL_H
typedef int __pid_t;
typedef unsigned int __uid_t;
typedef long int __clock_t;
#include <bits/sigset.h>
#include <bits/siginfo.h>
#include <asm-generic/signal-defs.h>
#include <bits/sigaction.h>
#include <bits/signum.h>

extern "C"
{

int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact)
{
  if (signum != SIGUSR1 && signum != SIGUSR2)
    return 0;

  int (** _sigaction)(int, const struct sigaction *,
		      struct sigaction *);
  *((void**)_sigaction)=dlsym(RTLD_NEXT, "sigaction");
  return (*_sigaction)(signum, act, oldact);
}
}