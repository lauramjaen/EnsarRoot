//  -------------------------------------------------------------------------
//
//   ----- General Macro for SOFIA analysis
//
//         Author: Jose Luis Rodriguez Sanchez <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 30/10/2012
//
//         Comments:
//
//
//  -------------------------------------------------------------------------
//
//   Usage inside ROOT interpreter: 
//        1) root -l
//        2)[root] .L unpackall.C 
//                         
//        3)[root] unpackall( InFiles,      // List of Runs
//                           fDetList      // List of Detectors
//                          );
//
//  -------------------------------------------------------------------------



void sofiaall(TString InFiles = "runlist.dat",
	    TObjArray& fDetList )
{

  // Output files
  TString outFile = "./r3bunpack.root";



  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // ----  Load libraries   -------------------------------------------------
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gSystem->Load("libFairTools");
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libR3BMbs");
  gSystem->Load("libMbsAPI");
  gSystem->Load("libSOFIAMCStack");
  gSystem->Load("libR3BRootEvent");
  gSystem->Load("libR3BLANDEvent");
  gSystem->Load("libR3BUnpack");
  //gSystem->Load("libR3BMusicEvent");
  //gSystem->Load("libR3BProEvent");
  //gSystem->Load("libR3BCrateEvent");

 
  // -----   Create analysis run   ----------------------------------------
  FairRunAna *fRun= new FairRunAna();
  fRun->SetOutputFile(outFile);

  // ----  Load MBS
  MBSUnpack *MBSunpack= new MBSUnpack("MBS unpack", InFiles);
  fRun->AddTask(MBSunpack);

  //SOFIA Crate 1
  if (fDetList.FindObject("Crate1") ) {
    CrateUnpack *CRATEunpack= new CrateUnpack("unpack");
    fRun->AddTask(CRATEunpack);
  }

  //SOFIA Crate 2
  if (fDetList.FindObject("Crate2") ) {
    MUSICUnpack *MUSICunpack= new MUSICUnpack("MUSIC unpack");
    fRun->AddTask(MUSICunpack);
  }

  // SOFIA Tof for LCP
  if (fDetList.FindObject("LCP_TOF") ) {
    ProUnpack *PROunpack= new ProUnpack("PROTON unpack");
    fRun->AddTask(PROunpack);
  }

  // SOFIA Land Detector
  if (fDetList.FindObject("LAND") ) {
    LANDUnpack *LANDunpack= new LANDUnpack("LAND unpack");
    fRun->AddTask(LANDunpack);
  }

  // -----   Initialize analysis run   --------------------------------------
  fRun->Init();
  fRun->RunOnLmdFiles(); 
   
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Real time " << rtime/60.0 << " min, CPU time " << ctime/60.0 
       << "min" << endl << endl;
  // ------------------------------------------------------------------------

  cout << " Test passed" << endl;
  cout << " All ok " << endl;

}


