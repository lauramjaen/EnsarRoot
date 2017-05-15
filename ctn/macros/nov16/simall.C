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
            TString fGenerator = "box",
            Bool_t fUserPList = kFALSE,
            TString OutFile = "outsim.root",
            TString ParFile = "outpar.root",
            TString InFile = "evt_gen.dat")
{

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
  run->AddModule(hpgedetector);

  // CALIFA definition
  EnsarDetector* calo = new R3BCalo("Califa", kTRUE);
  ((R3BCalo *)calo)->SelectGeometryVersion(1116);//version to single petal with copies
  //Selecting the Non-uniformity of the crystals (1 means +-1% max deviation)
  ((R3BCalo *)calo)->SetNonUniformity(1.0);
  calo->SetGeometryFileName(((TObjString*)fDetList->GetValue("CALIFA"))->GetString().Data());
  run->AddModule(calo);



  // -----   Create PrimaryGenerator   --------------------------------------

  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();


  if (fGenerator.CompareTo("box") == 0  ) {
  // 2- Define the BOX generator
  Int_t pdgId          = 22;            // geant particle id of the photon beam
  Double32_t theta1    = 0.;         // polar angle distribution (degrees)
  Double32_t theta2    = 180.;
  Double32_t momentum  = 0.006068;      // GeV/c 6048keV energy of the beam
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId,10);
  boxGen->SetThetaRange (theta1,theta2);
  boxGen->SetCosTheta();
  boxGen->SetPRange     (0.001,0.001);  // momentum of the beam GeV/c
  boxGen->SetPhiRange   (0.0,360.0);
  boxGen->SetXYZ        (0.,0.,0.);  // origin of the beam in the center
  
  // add the box generator
  primGen->AddGenerator(boxGen);
  } 
  
   if (fGenerator.CompareTo("ascii") == 0  ) {
    FairAsciiGenerator* gen = new FairAsciiGenerator((dir+"/input/"+InFile).Data());
    primGen->AddGenerator(gen);
  }
  
  run->SetGenerator(primGen);


  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(fVis);

  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
  
  
  

// ----- Initialize CaloHitFinder task ------------------------------------
  Bool_t fCaloHitFinder = true;
  if(fCaloHitFinder) {
    R3BCaloHitFinder* caloHF = new R3BCaloHitFinder();
    caloHF->SetClusteringAlgorithm(1,0);    //square window
    caloHF->SetDetectionThreshold(0.000050);//50 KeV
    caloHF->SetExperimentalResolution(6.);  //percent @ 1 MeV
    caloHF->SetComponentResolution(.25);    //sigma = 0.5 MeV
    caloHF->SelectGeometryVersion(1116);
    caloHF->SetAngularWindow(3.2,3.2);      //[0.25 around 14.3 degrees, 3.2 for the complete calorimeter]
    run->AddTask(caloHF);
  }  
 
 
 


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


