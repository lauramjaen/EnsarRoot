void runSim(Int_t nEvents = 3)
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
  
  // // Box Generator
  // FairBoxGenerator* boxGen = new FairBoxGenerator(2212, 3); // 13 = muon; 1 = multipl.
  // boxGen->SetPRange(2., 2.);    // GeV/c //setPRange vs setPtRange
  // boxGen->SetPhiRange(0, 360);  // Azimuth angle range [degree]
  // boxGen->SetThetaRange(3, 10); // Polar angle in lab system range [degree]
  // boxGen->SetCosTheta();//uniform generation on all the solid angle(default)
  
  E105AsciiGenerator* ASCIIGen = new E105AsciiGenerator("eventGenerator/B150Ni56onalphaElas.dat");
  primGen->AddGenerator(ASCIIGen);
  
  fRun->SetStoreTraj(kTRUE);
  
  fRun->Init();
  
  
  // -Trajectories Visualization (TGeoManager Only )
  // -----------------------------------------------
  
  // Set cuts for storing the trajectpries
   FairTrajFilter* trajFilter = FairTrajFilter::Instance();
   trajFilter->SetStepSizeCut(0.01); // 1 cm
   trajFilter->SetVertexCut(-2000., -2000., 4., 2000., 2000., 100.);
   trajFilter->SetMomentumCutP(10e-3); // p_lab > 10 MeV
   trajFilter->SetEnergyCut(0., 1.02); // 0 < Etot < 1.04 GeV
   trajFilter->SetStorePrimaries(kTRUE);
   trajFilter->SetStoreSecondaries(kTRUE);
    
  
  // Fill the Parameter containers for this run
  //-------------------------------------------
  
  FairRuntimeDb *rtdb=fRun->GetRuntimeDb();
  Bool_t kParameterMerged=kTRUE;
  FairParRootFileIo* output=new FairParRootFileIo(kParameterMerged);
  output->open("testparams.root");
  rtdb->setOutput(output);
  
  rtdb->saveOutput();
  rtdb->print();
  
  // Transport nEvents
  // -----------------
  
  //  Int_t nEvents = 1;
  fRun->Run(nEvents);
  
  fRun->CreateGeometryFile("geofile_full.root");
  
  
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  printf("RealTime=%f seconds, CpuTime=%f seconds\n",rtime,ctime);
}
