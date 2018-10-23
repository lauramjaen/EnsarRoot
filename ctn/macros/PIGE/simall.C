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
            TString fMC = "TGeant4",
            TString fGenerator = "box",
            Bool_t fUserPList = kFALSE,
            TString OutFile = "outsim.root",
            TString ParFile = "outpar.root",
            TString InFile = "ascii_test.dat")
{
//TString InFile = "evt_gen.dat")

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

  // CALIFA definition
  //EnsarDetector* calo = new R3BCalo("Califa", kTRUE);
  //((R3BCalo *)calo)->SelectGeometryVersion(1116);//version to single petal with copies
  //Selecting the Non-uniformity of the crystals (1 means +-1% max deviation)
  //((R3BCalo *)calo)->SetNonUniformity(1.0);//change Resolution 5%, more realistic case
  //calo->SetGeometryFileName(((TObjString*)fDetList->GetValue("CALIFA"))->GetString().Data());
  //calo->SetVerboseLevel(0); //screen info
  //run->AddModule(calo);



  // -----   Create PrimaryGenerator   --------------------------------------

  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();


  if (fGenerator.CompareTo("box") == 0  ) {
  // 2- Define the BOX generator
  Int_t pdgId          = 22;            // geant particle id of the photon beam
  Double32_t theta1    = 90.;            // polar angle distribution (degrees) 70.
  Double32_t theta2    = 180.;						// 110.
	Double32_t phi1    	 = 90.;            // azimutal angle distribution (degrees) 160.
  Double32_t phi2      = 270.;						// 200.
  Double32_t momentum  = 0.006068;      // GeV/c 6048keV energy of the beam
  //FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId,50); //multiplicity
	EnsarBoxGenerator* boxGen = new EnsarBoxGenerator(pdgId,1); //multiplicity
  boxGen->SetThetaRange (theta1,theta2);
  boxGen->SetCosTheta();
  boxGen->SetPRange     (0.002230,0.002230);// momentum of the beam GeV/c
  boxGen->SetPhiRange   (phi1,phi2);    // azimuthal angle (degrees)
  boxGen->SetXYZ        (0.,0.,0.);     // Point source
//SetBoxXYZ (Double32_t x1=0, Double32_t y1=0, Double32_t x2=0, Double32_t y2=0, Double32_t z=0)
  //boxGen->SetBoxXYZ  (-0.05,-0.05,0.05,0.05,0.); //Square source
  // add the box generator
  primGen->AddGenerator(boxGen);
  } 
  
   if (fGenerator.CompareTo("ascii") == 0  ) {
    EnsarAsciiGenerator* gen = new EnsarAsciiGenerator((dir+"/ctn/macros/PIGE/ascii_test2.dat").Data());
		//FairAsciiGenerator* gen = new FairAsciiGenerator((dir+"/ctn/macros/PIGE/"+InFile).Data());
		//FairAsciiGenerator* gen = new FairAsciiGenerator(("ascii_test.dat").Data());
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
  
  //add the Gamma cascade generator with Angular Correlations, 60Co in this case
   if (fGenerator.CompareTo("Cocascade") == 0  ) {
    EnsarCascadeGen* CascadeGen = new EnsarCascadeGen("Co_cascade.dat",1);   
    primGen->AddGenerator(CascadeGen);
  }

  //add the Gamma cascade generator, the 208Tl decay
  if (fGenerator.CompareTo("208Tldecay") == 0  ) {
    EnsarGammaCascadeGen* GammaGen = new EnsarGammaCascadeGen("208Tldecay.dat");   
		//ThGen->SetXYZ(-10.,-10.,-10.);
		GammaGen->SetBoxXYZ(15.,-10.,15.,-15.,-10.,-15.);//(x1,y1,x2,y2)=(-20,-20,0.,-20); HPGe_only=(-1,-10,-1,-15,-10,-15)
		GammaGen->SetThetaRange(90,90);
		GammaGen->SetPhiRange(90,90);
    primGen->AddGenerator(GammaGen);
  }
	
	//add the Decay generator which emmits gammas from the de-excitation of daughter nucleus
 if (fGenerator.CompareTo("DecayGen") == 0  ) {
    EnsarDecayGenerator* DecayGen = new EnsarDecayGenerator("238UChain.dat");
		//DecayGen->SetXYZ(0.,0.,0.);
		DecayGen->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		DecayGen->SetThetaRange(90,90);
		DecayGen->SetPhiRange(90,90);
    primGen->AddGenerator(DecayGen);
  }

  //add the 232Th chain of natural background cascade generator
 if (fGenerator.CompareTo("232Thchain") == 0  ) {
    EnsarThoriumChainGen* ThGen = new EnsarThoriumChainGen("232ThChain.dat");   
		//ThGen->SetXYZ(-10.,-10.,-10.);
		ThGen->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		ThGen->SetPhiRange(90,90);
    primGen->AddGenerator(ThGen);
  }

  //add the 238U chain of natural background cascade generator
 if (fGenerator.CompareTo("238Uchain") == 0  ) {
    EnsarUraniumChainGen* UGen = new EnsarUraniumChainGen("238UChain.dat");   
		//UGen->SetXYZ(0.,0.,0.);
		UGen->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen->SetThetaRange(90,90);
		UGen->SetPhiRange(90,90);
    primGen->AddGenerator(UGen);
  }
//------------------------------------------------------------------------------------------------
	//add the 238U nucleus of URANIUM chain generator
 if (fGenerator.CompareTo("URANIUMchain_238U") == 0  ) {
    EnsarUraniumChainGen_238U* UGen_238U = new EnsarUraniumChainGen_238U("238UChain.dat");   
		//UGen->SetXYZ(0.,0.,0.);
		UGen_238U->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_238U->SetThetaRange(90,90);
		UGen_238U->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_238U);
  }

	//add the 234Th nucleus
 if (fGenerator.CompareTo("URANIUMchain_234Th") == 0  ) {
    EnsarUraniumChainGen_234Th* UGen_234Th = new EnsarUraniumChainGen_234Th("238UChain.dat");
		//UGen_234Th->SetXYZ(0.,0.,0.);
		UGen_234Th->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_234Th->SetThetaRange(90,90);
		UGen_234Th->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_234Th);
  }
  
	//add the 234U nucleus
 if (fGenerator.CompareTo("URANIUMchain_234U") == 0  ) {
    EnsarUraniumChainGen_234U* UGen_234U = new EnsarUraniumChainGen_234U("238UChain.dat");
		//UGen_234U->SetXYZ(0.,0.,0.);
		UGen_234U->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_234U->SetThetaRange(90,90);
		UGen_234U->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_234U);
  }

	//add the 230Th nucleus
 if (fGenerator.CompareTo("URANIUMchain_230Th") == 0  ) {
    EnsarUraniumChainGen_230Th* UGen_230Th = new EnsarUraniumChainGen_230Th("238UChain.dat");
		//UGen_230Th->SetXYZ(0.,0.,0.);
		UGen_230Th->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_230Th->SetThetaRange(90,90);
		UGen_230Th->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_230Th);
  }
	//add the 226Ra nucleus
 if (fGenerator.CompareTo("URANIUMchain_226Ra") == 0  ) {
    EnsarUraniumChainGen_226Ra* UGen_226Ra = new EnsarUraniumChainGen_226Ra("238UChain.dat");
		//UGen_226Ra->SetXYZ(0.,0.,0.);
		UGen_226Ra->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_226Ra->SetThetaRange(90,90);
		UGen_226Ra->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_226Ra);
  }
	//add the 210Pb nucleus
 if (fGenerator.CompareTo("URANIUMchain_210Pb") == 0  ) {
    EnsarUraniumChainGen_210Pb* UGen_210Pb = new EnsarUraniumChainGen_210Pb("238UChain.dat");
		//UGen_210Pb->SetXYZ(0.,0.,0.);
		UGen_210Pb->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_210Pb->SetThetaRange(90,90);
		UGen_210Pb->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_210Pb);
  }
	//add the 234Pa nucleus
 if (fGenerator.CompareTo("URANIUMchain_234Pa") == 0  ) {
    EnsarUraniumChainGen_234Pa* UGen_234Pa = new EnsarUraniumChainGen_234Pa("238UChain.dat");
		//UGen_234Pa->SetXYZ(0.,0.,0.);
		UGen_234Pa->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_234Pa->SetThetaRange(90,90);
		UGen_234Pa->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_234Pa);
  }
	//add the 214Pb nucleus
 if (fGenerator.CompareTo("URANIUMchain_214Pb") == 0  ) {
    EnsarUraniumChainGen_214Pb* UGen_214Pb = new EnsarUraniumChainGen_214Pb("238UChain.dat");
		//UGen_214Pb->SetXYZ(0.,0.,0.);
		UGen_214Pb->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_214Pb->SetThetaRange(90,90);
		UGen_214Pb->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_214Pb);
  }
	//add the 214Bi nucleus
 if (fGenerator.CompareTo("URANIUMchain_214Bi") == 0  ) {
    EnsarUraniumChainGen_214Bi* UGen_214Bi = new EnsarUraniumChainGen_214Bi("238UChain.dat");
		//UGen_214Bi->SetXYZ(0.,0.,0.);
		UGen_214Bi->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_214Bi->SetThetaRange(90,90);
		UGen_214Bi->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_214Bi);
  }

//------------------------------------------------------------------------------------------------
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


