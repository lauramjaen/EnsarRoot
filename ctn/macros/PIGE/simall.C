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



  // -----   Create PrimaryGenerator   --------------------------------------

  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();


  if (fGenerator.CompareTo("box") == 0  ) {
  // 2- Define the BOX generator
  Int_t pdgId          = 22;            // geant particle id of the photon beam
  Double32_t theta1    = 90.;            // polar angle distribution (degrees) 70.
  Double32_t theta2    = 90.;						// 110.
	Double32_t phi1    	 = 90.;            // azimutal angle distribution (degrees) 160.
  Double32_t phi2      = 90.;						// 200.
  Double32_t momentum  = 0.006068;      // GeV/c 6048keV energy of the beam
  //FairBoxGenerator* boxGen = new FairBoxGenerator(pdgId,50); //multiplicity
	EnsarBoxGenerator* boxGen = new EnsarBoxGenerator(pdgId,5); //multiplicity
  boxGen->SetThetaRange (theta1,theta2);
  boxGen->SetCosTheta();
  boxGen->SetPRange     (0.002230,0.00100);// momentum of the beam GeV/c
  boxGen->SetPhiRange   (phi1,phi2);    // azimuthal angle (degrees)
  //boxGen->SetXYZ        (0.,0.,0.);     // Point source
//SetBoxXYZ (Double32_t x1=0, Double32_t y1=0, Double32_t x2=0, Double32_t y2=0, Double32_t z=0)
  boxGen->SetBoxXYZ  (-0.05,-0.05,0.05,0.05,0.); //Square source
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
// if (fGenerator.CompareTo("232Thchain") == 0  ) {
//    EnsarThoriumChainGen* ThGen = new EnsarThoriumChainGen("232ThChain.dat");   
		//ThGen->SetXYZ(-10.,-10.,-10.);
//		ThGen->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
//		ThGen->SetPhiRange(90,90);
//    primGen->AddGenerator(ThGen);
//  }

  //add the 238U chain of natural background cascade generator
// if (fGenerator.CompareTo("238Uchain") == 0  ) {
//    EnsarUraniumChainGen* UGen = new EnsarUraniumChainGen("238UChain.dat");   
		//UGen->SetXYZ(0.,0.,0.);
//		UGen->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
//		UGen->SetThetaRange(90,90);
//		UGen->SetPhiRange(90,90);
//    primGen->AddGenerator(UGen);
//  }


//------------------------------------------------------------------------------------------------
// ---- Nucleus of the URANIUM chain generator

	//add the 238U nucleus
 if (fGenerator.CompareTo("URANIUMchain_238U") == 0  ) {
    EnsarUraniumChainGen_238U* UGen_238U = new EnsarUraniumChainGen_238U("dummy.dat");   
		//UGen->SetXYZ(0.,0.,0.);
		//UGen_238U->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_238U->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_238U->SetThetaRange(90,90);
		UGen_238U->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_238U);
  }

	//add the 234Th nucleus
 if (fGenerator.CompareTo("URANIUMchain_234Th") == 0  ) {
    EnsarUraniumChainGen_234Th* UGen_234Th = new EnsarUraniumChainGen_234Th("dummy.dat");
		//UGen_234Th->SetXYZ(0.,0.,0.);
		//UGen_234Th->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_234Th->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_234Th->SetThetaRange(90,90);
		UGen_234Th->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_234Th);
  }

	//add the 234Pa nucleus
 if (fGenerator.CompareTo("URANIUMchain_234Pa") == 0  ) {
    EnsarUraniumChainGen_234Pa* UGen_234Pa = new EnsarUraniumChainGen_234Pa("dummy.dat");
		//UGen_234Pa->SetXYZ(0.,0.,0.);
		//UGen_234Pa->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_234Pa->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_234Pa->SetThetaRange(90,90);
		UGen_234Pa->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_234Pa);
  }
  
	//add the 234U nucleus
 if (fGenerator.CompareTo("URANIUMchain_234U") == 0  ) {
    EnsarUraniumChainGen_234U* UGen_234U = new EnsarUraniumChainGen_234U("dummy.dat");
		//UGen_234U->SetXYZ(0.,0.,0.);
		//UGen_234U->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_234U->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_234U->SetThetaRange(90,90);
		UGen_234U->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_234U);
  }

	//add the 230Th nucleus
 if (fGenerator.CompareTo("URANIUMchain_230Th") == 0  ) {
    EnsarUraniumChainGen_230Th* UGen_230Th = new EnsarUraniumChainGen_230Th("dummy.dat");
		//UGen_230Th->SetXYZ(0.,0.,0.);
		//UGen_230Th->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_230Th->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_230Th->SetThetaRange(90,90);
		UGen_230Th->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_230Th);
  }
	//add the 226Ra nucleus
 if (fGenerator.CompareTo("URANIUMchain_226Ra") == 0  ) {
    EnsarUraniumChainGen_226Ra* UGen_226Ra = new EnsarUraniumChainGen_226Ra("dummy.dat");
		//UGen_226Ra->SetXYZ(0.,0.,0.);
		//UGen_226Ra->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_226Ra->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_226Ra->SetThetaRange(90,90);
		UGen_226Ra->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_226Ra);
  }
	//add the 214Pb nucleus
 if (fGenerator.CompareTo("URANIUMchain_214Pb") == 0  ) {
    EnsarUraniumChainGen_214Pb* UGen_214Pb = new EnsarUraniumChainGen_214Pb("dummy.dat");
		//UGen_214Pb->SetXYZ(0.,0.,0.);
		//UGen_214Pb->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_214Pb->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_214Pb->SetThetaRange(90,90);
		UGen_214Pb->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_214Pb);
  }

	//add the 214Bi nucleus
 if (fGenerator.CompareTo("URANIUMchain_214Bi") == 0  ) {
    EnsarUraniumChainGen_214Bi* UGen_214Bi = new EnsarUraniumChainGen_214Bi("dummy.dat");
		//UGen_214Bi->SetXYZ(0.,0.,0.);
		//UGen_214Bi->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_214Bi->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_214Bi->SetThetaRange(90,90);
		UGen_214Bi->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_214Bi);
  }

	//add the 210Pb nucleus
 if (fGenerator.CompareTo("URANIUMchain_210Pb") == 0  ) {
    EnsarUraniumChainGen_210Pb* UGen_210Pb = new EnsarUraniumChainGen_210Pb("dummy.dat");
		//UGen_210Pb->SetXYZ(0.,0.,0.);
		//UGen_210Pb->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		UGen_210Pb->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		UGen_210Pb->SetThetaRange(90,90);
		UGen_210Pb->SetPhiRange(90,90);
    primGen->AddGenerator(UGen_210Pb);
  }
//------------------------------------------------------------------------------------------------
	

	//add Potassium generator
 if (fGenerator.CompareTo("Potassium") == 0  ) {
    EnsarPotassiumGen* Potassium = new EnsarPotassiumGen("dummy.dat");
		//Potassium->SetXYZ(0.,0.,0.);
		Potassium->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		Potassium->SetThetaRange(90,90);
		Potassium->SetPhiRange(90,90);
    primGen->AddGenerator(Potassium);
  }//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// ---- Nucleus of the THORIUM chain generator

 //add the 232Th nucleus
 if (fGenerator.CompareTo("THORIUMchain_232Th") == 0  ) {
    EnsarThoriumChainGen_232Th* Thorium_232Th = new EnsarThoriumChainGen_232Th("dummy.dat");
		//Thorium_232Th->SetXYZ(0.,0.,0.);
		//Thorium_232Th->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		Thorium_232Th->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		Thorium_232Th->SetThetaRange(90,90);
		Thorium_232Th->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_232Th);
  }
 //add the 228Ra nucleus
 if (fGenerator.CompareTo("THORIUMchain_228Ra") == 0  ) {
    EnsarThoriumChainGen_228Ra* Thorium_228Ra = new EnsarThoriumChainGen_228Ra("dummy.dat");
		//Thorium_228Ra->SetXYZ(0.,0.,0.);
		//Thorium_228Ra->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		Thorium_228Ra->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		Thorium_228Ra->SetThetaRange(90,90);
		Thorium_228Ra->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_228Ra);
  }
 //add the 228Th nucleus
 if (fGenerator.CompareTo("THORIUMchain_228Th") == 0  ) {
    EnsarThoriumChainGen_228Th* Thorium_228Th = new EnsarThoriumChainGen_228Th("dummy.dat");
		//Thorium_228Th->SetXYZ(0.,0.,0.);
		//Thorium_228Th->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		Thorium_228Th->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		Thorium_228Th->SetThetaRange(90,90);
		Thorium_228Th->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_228Th);
  }
 //add the 224Ra nucleus
 if (fGenerator.CompareTo("THORIUMchain_224Ra") == 0  ) {
    EnsarThoriumChainGen_224Ra* Thorium_224Ra = new EnsarThoriumChainGen_224Ra("dummy.dat");
		//Thorium_224Ra->SetXYZ(0.,0.,0.);
		//Thorium_224Ra->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		Thorium_224Ra->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		Thorium_224Ra->SetThetaRange(90,90);
		Thorium_224Ra->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_224Ra);
  }
 //add the 212Pb nucleus
 if (fGenerator.CompareTo("THORIUMchain_212Pb") == 0  ) {
    EnsarThoriumChainGen_212Pb* Thorium_212Pb = new EnsarThoriumChainGen_212Pb("dummy.dat");
		//Thorium_212Pb->SetXYZ(0.,0.,0.);
		//Thorium_212Pb->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		Thorium_212Pb->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		Thorium_212Pb->SetThetaRange(90,90);
		Thorium_212Pb->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_212Pb);
  }
 //add the 212Bi nucleus
 if (fGenerator.CompareTo("THORIUMchain_212Bi") == 0  ) {
    EnsarThoriumChainGen_212Bi* Thorium_212Bi = new EnsarThoriumChainGen_212Bi("dummy.dat");
		//Thorium_212Bi->SetXYZ(0.,0.,0.);
		//Thorium_212Bi->SetBoxXYZ(-1,-10,-1,-15,-10,-15);
		Thorium_212Bi->SetBoxXYZ(13,-200,13,-15,-200,-15);//earth effect
		Thorium_212Bi->SetThetaRange(90,90);
		Thorium_212Bi->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_212Bi);
  }
 //add the 208Tl nucleus
 if (fGenerator.CompareTo("THORIUMchain_208Tl") == 0  ) {
    EnsarThoriumChainGen_208Tl* Thorium_208Tl = new EnsarThoriumChainGen_208Tl("dummy.dat");
		//Thorium_208Tl->SetXYZ(0.,0.,0.);
		//Thorium_208Tl->SetBoxXYZ(-1,-10,-1,-15,-10,-15);//prop BoxA
		//Thorium_208Tl->SetBoxXYZ(-1,-10036,-1,-15,-10036,-15);//lluny BoxB
		//Thorium_208Tl->SetBoxXYZ(13,-10036,13,-15,-10036,-15);, with more sup BoxC
		//Thorium_208Tl->SetBoxXYZ(-1,-200,-1,-15,-200,-15);// BoxD
		Thorium_208Tl->SetBoxXYZ(13,-200,13,-15,-200,-15);//BoxE
		//Thorium_208Tl->SetBoxXYZ(13,-40,13,-15,-40,-15);//BoxF
		//Thorium_208Tl->SetBoxXYZ(13,-350,13,-15,-350,-15);//BoxG
		Thorium_208Tl->SetThetaRange(90,90);
		Thorium_208Tl->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_208Tl);
  }
 //add the 228Ac nucleus
 if (fGenerator.CompareTo("THORIUMchain_228Ac") == 0  ) {
    EnsarThoriumChainGen_228Ac* Thorium_228Ac = new EnsarThoriumChainGen_228Ac("dummy.dat");
		//Thorium_228Ac->SetXYZ(0.,0.,0.);
		//Thorium_228Ac->SetBoxXYZ(-1,-10,-1,-15,-10,-15);//prop
		Thorium_228Ac->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Thorium_228Ac->SetThetaRange(90,90);
		Thorium_228Ac->SetPhiRange(90,90);
    primGen->AddGenerator(Thorium_228Ac);
  }
//------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------
// ---- Nucleus of the ACTINIUM chain generator

 //add the 235U nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_235U") == 0  ) {
    EnsarActiniumChainGen_235U* Actinium_235U = new EnsarActiniumChainGen_235U("dummy.dat");
		//Actinium_235U->SetXYZ(0.,0.,0.);
		Actinium_235U->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_235U->SetThetaRange(90,90);
		Actinium_235U->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_235U);
  }

 //add the 231Th nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_231Th") == 0  ) {
    EnsarActiniumChainGen_231Th* Actinium_231Th = new EnsarActiniumChainGen_231Th("dummy.dat");
		//Actinium_231Th->SetXYZ(0.,0.,0.);
		Actinium_231Th->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_231Th->SetThetaRange(90,90);
		Actinium_231Th->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_231Th);
  }

 //add the 231Pa nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_231Pa") == 0  ) {
    EnsarActiniumChainGen_231Pa* Actinium_231Pa = new EnsarActiniumChainGen_231Pa("dummy.dat");
		//Actinium_231Pa->SetXYZ(0.,0.,0.);
		Actinium_231Pa->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_231Pa->SetThetaRange(90,90);
		Actinium_231Pa->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_231Pa);
  }

 //add the 227Ac nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_227Ac") == 0  ) {
    EnsarActiniumChainGen_227Ac* Actinium_227Ac = new EnsarActiniumChainGen_227Ac("dummy.dat");
		//Actinium_227Ac->SetXYZ(0.,0.,0.);
		Actinium_227Ac->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_227Ac->SetThetaRange(90,90);
		Actinium_227Ac->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_227Ac);
  }

 //add the 227Th nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_227Th") == 0  ) {
    EnsarActiniumChainGen_227Th* Actinium_227Th = new EnsarActiniumChainGen_227Th("dummy.dat");
		//Actinium_227Th->SetXYZ(0.,0.,0.);
		Actinium_227Th->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_227Th->SetThetaRange(90,90);
		Actinium_227Th->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_227Th);
  }

 //add the 223Ra nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_223Ra") == 0  ) {
    EnsarActiniumChainGen_223Ra* Actinium_223Ra = new EnsarActiniumChainGen_223Ra("dummy.dat");
		//Actinium_223Ra->SetXYZ(0.,0.,0.);
		Actinium_223Ra->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_223Ra->SetThetaRange(90,90);
		Actinium_223Ra->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_223Ra);
  }

 //add the 219Rn nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_219Rn") == 0  ) {
    EnsarActiniumChainGen_219Rn* Actinium_219Rn = new EnsarActiniumChainGen_219Rn("dummy.dat");
		//Actinium_219Rn->SetXYZ(0.,0.,0.);
		Actinium_219Rn->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_219Rn->SetThetaRange(90,90);
		Actinium_219Rn->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_219Rn);
  }

 //add the 211Pb nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_211Pb") == 0  ) {
    EnsarActiniumChainGen_211Pb* Actinium_211Pb = new EnsarActiniumChainGen_211Pb("dummy.dat");
		//Actinium_211Pb->SetXYZ(0.,0.,0.);
		Actinium_211Pb->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_211Pb->SetThetaRange(90,90);
		Actinium_211Pb->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_211Pb);
  }

 //add the 211Bi nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_211Bi") == 0  ) {
    EnsarActiniumChainGen_211Bi* Actinium_211Bi = new EnsarActiniumChainGen_211Bi("dummy.dat");
		//Actinium_211Bi->SetXYZ(0.,0.,0.);
		Actinium_211Bi->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_211Bi->SetThetaRange(90,90);
		Actinium_211Bi->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_211Bi);
  }

 //add the 223Fr nucleus
 if (fGenerator.CompareTo("ACTINIUMchain_223Fr") == 0  ) {
    EnsarActiniumChainGen_223Fr* Actinium_223Fr = new EnsarActiniumChainGen_223Fr("dummy.dat");
		//Actinium_223Fr->SetXYZ(0.,0.,0.);
		Actinium_223Fr->SetBoxXYZ(13,-200,13,-15,-200,-15);//lluny
		Actinium_223Fr->SetThetaRange(90,90);
		Actinium_223Fr->SetPhiRange(90,90);
    primGen->AddGenerator(Actinium_223Fr);
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


