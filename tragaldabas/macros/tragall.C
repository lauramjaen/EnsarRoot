//  -------------------------------------------------------------------------
//
//   ----- General Macro for R3B simulation
//
//         Author: Denis Bertini <D.Bertini@gsi.de>
//
//         Last Update: 30/06/12 (marc Labiche)
//
//         Comments:
//               - 30/06/12 Adding Vaccum Vessel and cooling for new Si tracker
//               - 17/08/09 Adding R3BModule R3BDetector
//                          for global positionning
//               - 12/08/09 Adding R3B Special Physics List
//               - 06/08/09 Adding R3B specific Event Generator
//
//
//  -------------------------------------------------------------------------
//
//   Usage inside ROOT interpreter:
//        1) root -l
//        2)[root] .L r3ball.C
//
//        3)[root] r3ball( nevt,
//                         fDetList,     // List of Detectors
//                         TargetType,    // "LeadTarget"
//                         Visualization, // kFalse or kTRUE
//                         fMC ,        // "TGeant3" or "TGeant4"
//                         fGenerator   // Generator type
//
//  -------------------------------------------------------------------------



void tragall(Int_t nEvents = 1,
            TMap* fDetList = NULL,
            Bool_t fVis = kFALSE,
            TString fMC = "TGeant3",
            TString fGenerator = "box",
            Bool_t fUserPList = kFALSE,
            Bool_t fHit2Saeta = kFALSE,
            TString OutFile = "tragsim.root",
            TString ParFile = "tragpar.root",
            TString InFile = "evt_gen.dat")
{

  TString dir = getenv("VMCWORKDIR");
  TString tragdir = dir + "/tragaldabas/macros";

  TString trag_geomdir = dir + "/tragaldabas/geometry/";
  gSystem->Setenv("GEOMPATH",trag_geomdir.Data());

  TString trag_confdir = dir + "gconfig";
  gSystem->Setenv("CONFIG_DIR",trag_confdir.Data());


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
  run->SetName(fMC.Data());              // Transport engine
  run->SetOutputFile(OutFile.Data());          // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  //  R3B Special Physics List in G4 case
  if ( (fUserPList  == kTRUE ) &&
      (fMC.CompareTo("TGeant4")   == 0)) {
    run->SetUserConfig("g4Config.C");
    run->SetUserCuts("SetCuts.C");
  }


  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media_ensar.geo");       // Materials


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
  cave->SetGeometryFileName("r3b_cave.geo");
  run->AddModule(cave);

  //Tragaldabas definition
  EnsarDetector* tragaldabas = new TraRPC("Rpc",kTRUE);
  tragaldabas->SetGeometryFileName(((TObjString*)fDetList->GetValue("RPC"))->GetString().Data());
  run->AddModule(tragaldabas);


  // -----   Create PrimaryGenerator   --------------------------------------
  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  if (fGenerator.CompareTo("box") == 0  ) {
    // 2- Define the BOX generator
    Int_t pdgId = 13; // pion beam
    Double32_t theta1 = -30.0;  // polar angle distribution
    Double32_t theta2 = 30.0;
    Double32_t momentum = 2.18;
    FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 1);
    boxGen->SetThetaRange(theta1, theta2);
    boxGen->SetPRange(momentum, momentum);
    boxGen->SetPhiRange(-100., 100.);
    boxGen->SetXYZ(0.0, 0.0, -2000.0);
//    boxGen->SetXYZ(0.0, 0.0, -300.);
    // add the box generator
    primGen->AddGenerator(boxGen);
  }

  if (fGenerator.CompareTo("ascii") == 0  ) {
    FairAsciiGenerator* gen = new FairAsciiGenerator((dir+"/input/"+InFile).Data());
    primGen->AddGenerator(gen);
  }

  if (fGenerator.CompareTo("tragbox") == 0  ) {

    // 2- Define the Tragaldabas BOX generator
    Double_t pdgId    = 13; // Muon emission
    Double_t theta1   = 0;  // polar angle distribution
    Double_t theta2   = 30;
    Double_t muonmass = 105.7; // Muonmass[MeV/c**2]
    Double_t momentum = 2.18;     //  GeV/c
    Double_t energy   = TMath::Sqrt((momentum*1000)*(momentum*1000)+muonmass*muonmass);

    TraBoxGenerator* tragalGen = new TraBoxGenerator(pdgId, 1);
    tragalGen->SetThetaRange (   theta1,   theta2);
    tragalGen->SetPRange     (-momentum,-momentum);
    tragalGen->SetPhiRange   (0.,360.);
    tragalGen->SetCosTheta();
    tragalGen->SetXhRange(-100.,100.);
    tragalGen->SetYhRange(-100.,100.);
    tragalGen->SetXYZ();
    // add the box generator
    primGen->AddGenerator(tragalGen);

 }

  run->SetGenerator(primGen);


  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(fVis);

  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");


  // ----- Initialize TimTrack task: Hit to Saeta ---------------------------
  if(fHit2Saeta) {
    TraRPCHit2Saeta* hit2saeta = new TraRPCHit2Saeta();
    run->AddTask(hit2saeta);
  }


  // -----   Initialize simulation run   ------------------------------------
  run->Init();


  // -----   Runtime database   ---------------------------------------------
  //R3BFieldPar* fieldPar = (R3BFieldPar*) rtdb->getContainer("R3BFieldPar");
  //if(NULL != magField)
  //{
  //    fieldPar->SetParameters(magField);
  //    fieldPar->setChanged();
  //}
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
