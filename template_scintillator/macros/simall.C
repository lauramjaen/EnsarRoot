//  -------------------------------------------------------------------------
//
//   ----- General Macro simulation
//
//         Author: Pablo Cabanelas <pablo.cabanelas@usc.es>
//         Adapted by: M. I. Cherciu <mcherciu@spacescience.ro>
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
	    TObjArray& fDetList,
            Bool_t fVis=kFALSE,
            TString fMC="TGeant4",
	    TString fGenerator="box",
	    Bool_t fUserPList= kFALSE
	   )
{


  TString dir = getenv("VMCWORKDIR");
  TString simdir = dir + "/macros";

  TString sim_geomdir = dir + "/geometry";
  gSystem->Setenv("GEOMPATH",sim_geomdir.Data());

  TString sim_confdir = dir + "gconfig";
  gSystem->Setenv("CONFIG_DIR",sim_confdir.Data());

// Output files
  TString OutFile = "outfile.root";
  TString ParFile = "parfile.root";


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
       (fMC.CompareTo("TGeant4")   == 0)
      ){
       run->SetUserConfig("g4Config.C");
       run->SetUserCuts("SetCuts.C");
   }


  // -----   Create media   -------------------------------------------------
  //run->SetMaterials("media_r3b.geo");       // Materials
  

  // Magnetic field map type
  // Int_t fFieldMap = 0;


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

  if (fDetList.FindObject("EnsarScintDet") )   {
      //My Detector definition
      EnsarDetector* mydet = new EnsarScintillator("EnsarScintillator", kTRUE);
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  +10.0; // (cm)
      mydet -> SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      mydet -> SetTranslation(tx, ty, tz);
      run->AddModule(mydet);
  // Activate Digitization Task
      EnsarScintillatorDigitizer *mydigi = new EnsarScintillatorDigitizer();
      run->AddTask(mydigi);
  }

  // -----   Create PrimaryGenerator   --------------------------------------

  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();


  if (fGenerator.CompareTo("box") == 0  ) {
  // 2- Define the generator
  Double_t pdgId=2212;   // proton beam
  Double_t theta1= 0.;  // polar angle distribution
  Double_t theta2= 7.;
  Double_t momentum=0.80; // 0.8 GeV/c
  Int_t     multiplicity = 1; // multiplicity (nb particles per event)
  FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId,multiplicity);
  boxGen->SetThetaRange (   theta1,   theta2);
  boxGen->SetPRange     (momentum,momentum*2.);
  boxGen->SetPhiRange   (0.,360.);
  boxGen->SetXYZ(0.0,0.0,-30.);
  // add the box generator
  primGen->AddGenerator(boxGen);
  } 
  
  run->SetGenerator(primGen);


  //-------Set visualisation flag to true------------------------------------
  if (fVis==kTRUE){
     run->SetStoreTraj(kTRUE);
  }else{
     run->SetStoreTraj(kFALSE);
  }   

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


