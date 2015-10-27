void sim(Int_t nEvents=1)
{
  TStopwatch timer;
  timer.Start();
  gDebug=0;

  // Use non default gconfig and geometry directories
  TString dir = getenv("VMCWORKDIR");
  TString tutdir = dir + "/example";

  TString tut_geomdir = tutdir + "/geometry";
  gSystem->Setenv("GEOMPATH",tut_geomdir.Data());

  TString tut_configdir = tutdir + "/gconfig";
  gSystem->Setenv("CONFIG_DIR",tut_configdir.Data());

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

  // create Instance of Run Manager class
  FairRunSim *fRun = new FairRunSim();
  
  // set the MC version used
  // ------------------------

  fRun->SetName("TGeant3");
  // Choose the Geant Navigation System
  // fRun->SetGeoModel("G3Native");
  
  fRun->SetOutputFile("testrun.root");

  // Set Material file Name
  //-----------------------
  fRun->SetMaterials("media.geo");
  
  // Create and add detectors
  //-------------------------
  FairModule *Cave = new FairCave("CAVE");
  Cave->SetGeometryFileName("cave.geo");
  fRun->AddModule(Cave);

  // DSSDs
  E105DSSD *DSSD = new E105DSSD("DSSD", kTRUE);
  fRun->AddModule(DSSD);

  // SiLis
  E105SiLi *SiLi = new E105SiLi("SiLi", kTRUE);
  fRun->AddModule(SiLi);

  // Slit
  E105Slit *Slit = new E105Slit("Slit");
  fRun->AddModule(Slit);  

  // Shield
  E105Shield *Shield = new E105Shield("Shield");
  fRun->AddModule(Shield);  

  // Create and Set Event Generator
  //-------------------------------
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();
  fRun->SetGenerator(primGen);
  
  E105AsciiGenerator* ASCIIGen = 
    new E105AsciiGenerator("eventGenerator/B150Ni56onalphaElas.dat");
  primGen->AddGenerator(ASCIIGen);
  
  fRun->SetStoreTraj(kTRUE);
  

  // Fill the Parameter containers for this run
  //-------------------------------------------
  // Generate a unique RunID
  
  FairRunIdGenerator runID;
  UInt_t runId =  runID.generateId();

  FairRuntimeDb *rtdb=fRun->GetRuntimeDb();
  Bool_t kParameterMerged=kTRUE;
  
  // Set the Ascii IO as first input
  FairParAsciiFileIo* inp1 = new FairParAsciiFileIo();
  

  TString work = getenv("VMCWORKDIR");
  TString filename = work + "/EnsarE105/macro/Ascii_inp_slid.par";
  inp1->open(filename.Data(),"in");
  rtdb->setFirstInput(inp1);


  // Set the SQL based IO as second input
  FairParTSQLIo* outp2 = new FairParTSQLIo();
  outp2->open();
  
  // DB Input
  //rtdb->setFirstInput(outp2);
  rtdb->setSecondInput(outp2);
  
  // DB Output
  rtdb->setOutput(outp2);
  rtdb->initContainers(runId);


  // FairParRootFileIo* output=new FairParRootFileIo(kParameterMerged);
  // output->open("testparams.root");
  // rtdb->setOutput(output);
  

  
  std::cout << "Print rtdb here after:\n";
  rtdb->print();

  // Transport nEvents
  // -----------------
  fRun->Init();
  fRun->Run(nEvents);

  rtdb->saveOutput();
  rtdb->writeContainers();

  fRun->CreateGeometryFile("geofile_full.root");

  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
  exit(0);
}
