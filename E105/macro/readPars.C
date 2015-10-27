void readPars()
{
  TStopwatch timer;
  timer.Start();
  gDebug=0;

  // ----  Load libraries   -------------------------------------------------
  gROOT->LoadMacro("$VMCWORKDIR/example/gconfig/basiclibs.C");
  basiclibs();
  gSystem->Load("libFairTools");
  gSystem->Load("libFairDB");
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libMCStack");
  gSystem->Load("libGen");
  gSystem->Load("libPassive");
  // ENSAR detector.
  gSystem->Load("libE105.so");


  // Create a Runtime Database singleton.
  FairRuntimeDb* db = FairRuntimeDb::instance();

  // Set the SQL IO as first input
  FairParTSQLIo* inp = new FairParTSQLIo();
  inp->open();
  db->setFirstInput(inp);

  //E105SlitPar* p1 = (E105SlitPar*) db->getContainer("E105SlitPar");
  /*
   * UInt_t year, UInt_t month, UInt_t day, UInt_t hour, UInt_t min,
   *  UInt_t sec, UInt_t nsec=0, Bool_t isUTC=kTRUE, Int_t
   *  secOffset=0);
   */
  ValTimeStamp tStamp(2013, 07, 16, 11, 33, 21);
  UInt_t runId = tStamp.GetSec();
  db->initContainers(runId);
  //p1->Print();

  //////////////////////////////////////////////
  FairLogger *logger = FairLogger::GetLogger();
  logger->SetLogVerbosityLevel("LOW");
  
  FairRunAna *fRun= new FairRunAna();
  fRun->SetInputFile("testrun.root");
  fRun->SetOutputFile("DummyDum.root");
  
  E105AccessRTDBTask* tsk = new E105AccessRTDBTask();
  fRun->AddTask(tsk);
  fRun->Init();
  fRun->Run();

 exit(0);
}
