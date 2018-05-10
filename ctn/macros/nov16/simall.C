//  -------------------------------------------------------------------------
//
//   ----- General Macro simulation
//
//         Author: Pablo Cabanelas <pablo.cabanelas@usc.es>
//
//  -------------------------------------------------------------------------
//
//   Usage inside ROOT interpreter: 
//        1) root -l
//        2)[root] .L simall.C 
//                         
//        3)[root] simall( nevt,
//                         fDetList,     // List of Detectors
//                         TargetType,    // "LeadTarget" 
//                         Visualization, // kFalse or kTRUE   
//                         fMC ,        // "TGeant3" or "TGeant4"   
//                         fGenerator   // Generator type
//
//  -------------------------------------------------------------------------



void simall(Int_t nEvents = 1,
            TMap* fDetList = NULL,
            Bool_t fVis = kFALSE,
            TString fMC = "TGeant3",
            TString fGenerator = "Box",
            Bool_t fUserPList = kFALSE,
            TString OutFile = "outsim.root",
            TString ParFile = "outpar.root",
            TString InFile = "evt_gen.dat")
{
	// TString fGenerator = "box",

  TString dir = getenv("VMCWORKDIR");
  TString macrosdir = dir + "/ctn/macros/nov16";

  TString geomdir = dir + "/ctn/geometry/";
  gSystem->Setenv("GEOMPATH",geomdir.Data());

  TString confdir = dir + "gconfig";
  gSystem->Setenv("CONFIG_DIR",confdir.Data());



  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------


  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName(fMC.Data());                    // Transport engine
  run->SetOutputFile(OutFile.Data());          // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  //  R3B Special Physics List in G4 case
  if ( (fUserPList  == kTRUE ) &&
       (fMC.CompareTo("TGeant4")   == 0)){
       run->SetUserConfig("g4Config.C");
       run->SetUserCuts("SetCuts.C");
   }


  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media.geo");              // Materials


  // Global Transformations
  //- Two ways for a Volume Rotation are supported
  //-- 1) Global Rotation (Euler Angles definition)
  //-- This represent the composition of : first a rotation about Z axis with
  //-- angle phi, then a rotation with theta about the rotated X axis, and
  //-- finally a rotation with psi about the new Z axis.
  Double_t phi,theta,psi;

  //-- 2) Rotation in Ref. Frame of the Volume
  //-- Rotation is Using Local Ref. Frame axis angles
  Double_t thetaX,thetaY,thetaZ;


  //- Global Translation  Lab. frame.
  Double_t tx,ty,tz;


  // -----   Create geometry --------------------------------------------

  //R3B Cave definition
  FairModule* cave= new FairCave("CAVE");
  cave->SetGeometryFileName("cave.geo");
  run->AddModule(cave);

  //HPGe Detector and chamber definition
  EnsarDetector* hpgedetector = new EnsarHPGeDet("EnsarHPGeDet",kTRUE);
  hpgedetector->SetGeometryFileName(((TObjString*)fDetList->GetValue("HPGE"))->GetString().Data());
  hpgedetector->SetVerboseLevel(0); //screen info
  run->AddModule(hpgedetector);


  // -----   Create PrimaryGenerator   --------------------------------------

  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();


  if (fGenerator.CompareTo("box") == 0  ) {
  // 2- Define the BOX generator 
  Int_t pdgId          = 11;            // geant particle id of the electron beam
  Double32_t theta1    = 0.;            // polar angle distribution (degrees)
  Double32_t theta2    = 2.;
  Double32_t momentum  = 0.006068;      // GeV/c 6048keV energy of the beam
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId,1); //multiplicity
  boxGen->SetThetaRange (theta1,theta2);
  boxGen->SetCosTheta();
  boxGen->SetPRange     (0.01049,0.01049);// momentum of the beam GeV/c
  boxGen->SetPhiRange   (0.0,360.0);    // azimuthal angle (degrees)
  boxGen->SetXYZ        (0.,0.,0.);     // origin of the beam in the center
  
  // add the box generator
  primGen->AddGenerator(boxGen);
  } 
  
   if (fGenerator.CompareTo("ascii") == 0  ) {
    FairAsciiGenerator* gen = new FairAsciiGenerator((dir+"/input/"+InFile).Data());
    primGen->AddGenerator(gen);
  }
  
  //add the Si-12643 cascade generator
   if (fGenerator.CompareTo("Sicascade") == 0  ) {
    EnsarSiGenerator* SiGen = new EnsarSiGenerator("Si_12643_cascade.dat",1);   
    primGen->AddGenerator(SiGen);
  }
  
  //add the Si-12900 cascade generator
   if (fGenerator.CompareTo("SiOneCascade") == 0  ) {
    EnsarCascadeGen* CascadeGen = new EnsarCascadeGen("Si_12900_cascade.dat",1);   
    primGen->AddGenerator(CascadeGen);
  }
  
  //add the Co cascade generator
   if (fGenerator.CompareTo("Cocascade") == 0  ) {
    EnsarCascadeGen* CascadeGen = new EnsarCascadeGen("Co_cascade.dat",1);   
    primGen->AddGenerator(CascadeGen);
  }

  //add the Gamma cascade generator
   if (fGenerator.CompareTo("Gamma_AngularC_200k") == 0  ) {
    EnsarCascadeGen* CascadeGen = new EnsarCascadeGen("Gamma_AngularC_200k.dat",1);   
    primGen->AddGenerator(CascadeGen);
  }
  
  run->SetGenerator(primGen);


  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(fVis);

  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  


  // -----   Initialize simulation run   ------------------------------------
  run->Init();


  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(ParFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
   
  // -----   Start run   ----------------------------------------------------
   if(nEvents > 0) {
    run->Run(nEvents);
  }
  
  // -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Output file is "    << OutFile << endl;
  cout << "Parameter file is " << ParFile << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime 
       << "s" << endl << endl;
  // ------------------------------------------------------------------------

  cout << " Test passed" << endl;
  cout << " All ok " << endl;

}


