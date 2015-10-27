//  -------------------------------------------------------------------------
//
//   ----- General Macro for Tragaldabas simulation
//
//  -------------------------------------------------------------------------
//
//   Usage inside ROOT interpreter: 
//        1) root -l
//        2)[root] .L tragall.C 
//                         
//        3)[root] tragall( nevt,
//                         fDetList,      // List of Detectors
//                         Visualization, // kFalse or kTRUE   
//                         fMC ,          // "TGeant3" or "TGeant4"   
//                         fGenerator     // Generator type
//
//  -------------------------------------------------------------------------

void tragall(Int_t nEvents = 1,            
	    TMap* fDetList=NULL,
            Bool_t fVis=kFALSE,
	    TString fMC="TGeant3",
	    TString fGenerator="box",
	    Bool_t fUserPList= kFALSE,
            TString OutFile = "tragsim.root",
            TString ParFile = "tragpar.root",
            TString InFile = "evt_file.dat")
{


  TString dir = getenv("VMCWORKDIR");
  TString tragdir = dir + "/tragaldabas/macros";

  TString trag_geomdir = dir + "/tragaldabas/geometry";
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
  run->SetName(fMC.Data());                  // Transport engine
  run->SetOutputFile(OutFile.Data());        // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  //  Special Physics List in G4 case
  if ( (fUserPList  == kTRUE ) &&
       (fMC.CompareTo("TGeant4")   == 0)
      ){
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

  //Cave definition
  FairModule* cave= new R3BCave("CAVE");
  cave->SetGeometryFileName("r3b_cave.geo");
  run->AddModule(cave);

  //Tragaldabas definition
  EnsarDetector* tragaldabas = new TraRPC("Rpc",kTRUE);
  tragaldabas->SetGeometryFileName(((TObjString*)fDetList.GetValue("RPC"))->GetString().Data());
  run->AddModule(tragaldabas);

  // -----   Create PrimaryGenerator   --------------------------------------

  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  if (fGenerator.CompareTo("ascii") == 0  ) {
    FairAsciiGenerator* gen = new FairAsciiGenerator((dir+"/input/"+InFile).Data());
    primGen->AddGenerator(gen);
  }

  if (fGenerator.CompareTo("box") == 0  ) {
	  // 2- Define the BOX generator
	  //Double_t pdgId=22; // gamma emission
	  Double_t pdgId=2212; // proton emission
	  //Double_t pdgId=211; // pion beam
	  Double_t theta1= 30.;  // polar angle distribution
	  //Double_t theta2= 7.;
	  Double_t theta2= 160.;
	  Double_t momentum=.8; // 10 GeV/c
	  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId, 10);
	  boxGen->SetThetaRange (   theta1,   theta2);
	  boxGen->SetPRange     (momentum,momentum*2.);
	  boxGen->SetPhiRange   (0.,360.);
	  //boxGen->SetXYZ(0.0,0.0,-1.5);
	  boxGen->SetXYZ(0.0,0.0,0.0);
	  // add the box generator
	  primGen->AddGenerator(boxGen);
  } 
	

  run->SetGenerator(primGen);

  //-------Set visualisation flag to true------------------------------------
  run->SetStoreTraj(fVis);

  FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");

  // -----   Initialize simulation run   ------------------------------------
  run->Init();

  // ------  Increase nb of step 
  Int_t nSteps = -15000;
  gMC->SetMaxNStep(nSteps);
  
  // -----   Runtime database   ---------------------------------------------
  Bool_t kParameterMerged = kTRUE;
  FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open(ParFile.Data());
  rtdb->setOutput(parOut);
  rtdb->saveOutput();
  rtdb->print();
  
// -----   Start run   ----------------------------------------------------
  if (nEvents>0) run->Run(nEvents);
  
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


