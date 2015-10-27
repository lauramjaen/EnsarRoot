//  -------------------------------------------------------------------------
//
//   ----- General Macro for SOFIA simulation
//
//         Author: Jose Luis Rodriguez Sanchez <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 30/08/2012
//
//         Comments:
//
//
//  -------------------------------------------------------------------------
//
//   Usage inside ROOT interpreter: 
//        1) root -l
//        2)[root] .L sofiaall.C 
//                         
//        3)[root] sofiaall( nevt,
//                         fDetList,     // List of Detectors
//                         TargetType,    // "H2" 
//                         Visualization, // kFalse or kTRUE   
//                         fMC ,        // "TGeant3" or "TGeant4"   
//                         fGenerator   // Generator type
//
//  -------------------------------------------------------------------------



void sofiaall(Int_t nEvents = 1,
	    TObjArray& fDetList,
	    TString Target = "H2",
		Bool_t fVis=kFALSE,
		TString fMC="TGeant4",
	    TString fGenerator="box",
	    Bool_t fUserPList= kFALSE,
		Bool_t fR3BMagnet= kTRUE )
{


  TString dir = getenv("VMCWORKDIR");
  TString r3bdir = dir + "/macros";

  TString r3b_geomdir = dir + "/geometry";
  gSystem->Setenv("GEOMPATH",r3b_geomdir.Data());

  TString r3b_confdir = dir + "gconfig";
  gSystem->Setenv("CONFIG_DIR",r3b_confdir.Data());

// Output files
  TString OutFile = "sofiasim.root";
  TString ParFile = "sofiapar.root";


  // In general, the following parts need not be touched
  // ========================================================================

  // ----    Debug option   -------------------------------------------------
  gDebug = 0;
  // ------------------------------------------------------------------------

  // -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  // ----  Load libraries   -------------------------------------------------
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();

  gSystem->Load("libGenVector");
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libGen");
  gSystem->Load("libSOFIAMCStack");
  gSystem->Load("libSOFIAField");

  //----  Load R3B specific libraries ---------------------------------------
  gSystem->Load("libSOFIAbase");
  gSystem->Load("libSOFIAGen");
  gSystem->Load("libSOFIAPassive");
  gSystem->Load("libSOFIAData");
  gSystem->Load("libSOFIACal");
  gSystem->Load("libSOFIACalo");
  gSystem->Load("libSOFIATwinMusic");
  gSystem->Load("libSOFIAMwpc1");
  gSystem->Load("libSOFIAMwpc2");
  gSystem->Load("libSOFIATof");
  gSystem->Load("libSOFIALand");
  
 
  // -----   Create simulation run   ----------------------------------------
  FairRunSim* run = new FairRunSim();
  run->SetName(fMC.Data());                  // Transport engine
  run->SetOutputFile(OutFile.Data());        // Output file
  FairRuntimeDb* rtdb = run->GetRuntimeDb();

  //  R3B Special Physics List in G4 case
  if ( (fUserPList  == kTRUE ) &&
       (fMC.CompareTo("TGeant4")   == 0)
      ){
       //run->SetUserConfig("g4R3bConfig.C");
       run->SetUserConfig("g4SofiaConfig.C");
       run->SetUserCuts("SetR3BCuts.C");
   }


  // -----   Create media   -------------------------------------------------
  run->SetMaterials("media_r3b.geo");       // Materials
  

  // Magnetic field map type
   Int_t fFieldMap = 0;


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


  // -----   Create SOFIA geometry --------------------------------------------
  //SOFIA Cave definition
  FairModule* cave= new R3BCave("CAVE");
  cave->SetGeometryFileName("r3b_cave.geo");
  run->AddModule(cave);

  //SOFIA Target definition
  if (fDetList.FindObject("TARGET") ) {
      R3BModule* target= new R3BTarget(Target.Data());
      // Global Lab. Rotation
      phi    =  0.0; // (deg)
      theta  =  0.0; // (deg)
      psi    =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
     //target->SetRotAnglesEuler(phi,theta,psi);
     target->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
     target->SetTranslation(tx,ty,tz);
     run->AddModule(target);
  }

  //SOFIA Califa
  if (fDetList.FindObject("CALIFA") ) {
      // CALIFA Calorimeter
      R3BDetector* calo = new R3BCalo("Califa", kTRUE);
	  //Selecting the geometry
	  // 0- CALIFA 5.0, including BARREL and ENDCAP.
	  // 1- CALIFA 7.05, only BARREL
	  // 2- CALIFA 7.07, only BARREL
	  // 3- CALIFA 7.09, only BARREL (ongoing work)
	  // 4- CALIFA 7.17, only ENDCAP (in CsI[Tl])
	  // 5- CALIFA 7.07+7.17, 
	  // 6- CALIFA 7.09+7.17, (ongoing work)
	  // 10- CALIFA 8.11, 
	  // ...
	  ((R3BCalo *)calo)->SelectGeometryVersion(10);
	  //Selecting the Non-uniformity of the crystals (1 means +-1% max deviation)
	  ((R3BCalo *)calo)->SetNonUniformity(1.0);
      // Global position of the Module
      phi   =  0.0; // (deg)
      theta =  0.0; // (deg)
      psi   =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
      //calo->SetRotAnglesEuler(phi,theta,psi);
      calo->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      calo->SetTranslation(tx,ty,tz);
      //calo->SetVerboseLevel(2);
      run->AddModule(calo);
  }
  
   //SOFIA vacuum pipe definition
  if (fDetList.FindObject("VACUUMPIPE") ) {
      R3BModule* tube= new R3Btubevacuum("VacuumPipe");
      // Global Lab. Rotation
      phi    =  0.0; // (deg)
      theta  =  0.0; // (deg)
      psi    =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
     //target->SetRotAnglesEuler(phi,theta,psi);
     tube->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
     tube->SetTranslation(tx,ty,tz);
     run->AddModule(tube);
  }
  
  //SOFIA Twin Music definition
  if (fDetList.FindObject("TWINMUSIC") ) {
      R3BDetector* twinmusic = new R3BTwinMusic("TwinMusic", kTRUE);
      // Global Lab. Rotation
      phi    =  0.0; // (deg)
      theta  =  0.0; // (deg)
      psi    =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
     //targetsofia->SetRotAnglesEuler(phi,theta,psi);
     twinmusic->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
     twinmusic->SetTranslation(tx,ty,tz);
     run->AddModule(twinmusic);
  }
 
  // SOFIA MWPC1
  if (fDetList.FindObject("MWPC_1")  ) {
      R3BDetector* mwpc = new R3BMWpc("Mwpc_1", kTRUE);
      // Global position of the Module
      phi   =  0.0; // (deg)
      theta =  0.0; // (deg)
      psi   =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
      //tra->SetRotAnglesEuler(phi,theta,psi);
      mwpc->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      mwpc->SetTranslation(tx,ty,tz);
      run->AddModule(mwpc);
  }

  //SOFIA Magnet definition
  if (fDetList.FindObject("ALADIN") ) {
      fFieldMap = 0;
      R3BModule* mag = new R3BMagnet("AladinMagnet");
      // Selecting the geometry
          // 1- Old geometry(default).
          // 2- New geometry.
          ((R3BMagnet *)mag)->SelectGeometry(2);
      // Selecting the gap medium
          // 0- Air(default)
          // 1- Vacuum
          // 2- Helium
          ((R3BMagnet *)mag)->SetGapMedium(2);
      // Global position of the Module
      phi   =  0.0; // (deg)
      theta =  0.0; // (deg)
      psi   =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
      //mag->SetRotAnglesEuler(phi,theta,psi);
      mag->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      mag->SetTranslation(tx,ty,tz);
      run->AddModule(mag);
  }

    //SOFIA Magnet definition
  if (fDetList.FindObject("GLAD") ) {
      fFieldMap = 1;
      R3BModule* mag = new R3BGladMagnet("GladMagnet");
      // Global position of the Module
      phi   =  0.0; // (deg)
      theta =  0.0; // (deg)
      psi   =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
      //mag->SetRotAnglesEuler(phi,theta,psi);
      mag->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      mag->SetTranslation(tx,ty,tz);
      run->AddModule(mag);
  }

   //SOFIA helium pipe definition
  if (fDetList.FindObject("HELIUMPIPE") ) {
      R3BModule* tube1= new R3Btubehelium("HeliumPipe");
      // Global Lab. Rotation
      phi    =  0.0; // (deg)
      theta  =  0.0; // (deg)
      psi    =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
     //target->SetRotAnglesEuler(phi,theta,psi);
     tube1->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
     tube1->SetTranslation(tx,ty,tz);
     run->AddModule(tube1);
  }
 
  // SOFIA MWPC2
  if (fDetList.FindObject("MWPC_2")  ) {
      R3BDetector* mwpc2 = new R3BMWpc2("Mwpc_2", kTRUE);
      // Global position of the Module
      phi   =  0.0; // (deg)
      theta =  0.0; // (deg)
      psi   =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  0.0; // (cm)
      //tra->SetRotAnglesEuler(phi,theta,psi);
      mwpc2->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      mwpc2->SetTranslation(tx,ty,tz);
      run->AddModule(mwpc2);
  }

  // SOFIA Tof for fission fragments
  if (fDetList.FindObject("TOF") ) {
      R3BDetector* tof = new R3BTof("Tof", kTRUE);
      // Global position of the Module
      thetaX   =  0.0; // (deg)
      thetaY   =  0.0; // (deg)
      thetaZ   =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx       =  0.0; // (cm)
      ty       =  0.0; // (cm)
      tz       =  0.0; // (cm)
      tof->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      tof->SetTranslation(tx,ty,tz);
      // User defined Energy CutOff
      Double_t fCutOffSci = 1.0e-05;  // Cut-Off -> 10.KeV only in Sci.
      ((R3BTof*) tof)->SetEnergyCutOff(fCutOffSci);
      run->AddModule(tof);
  }

  // SOFIA Land Detector
  if (fDetList.FindObject("LAND") ) {
      R3BDetector* land = new R3BLand("Land", kTRUE);
      // Global position of the Module
      phi   =  0.0; // (deg)
      theta =  0.0; // (deg)
      psi   =  0.0; // (deg)
      // Rotation in Ref. Frame.
      thetaX =  0.0; // (deg)
      thetaY =  0.0; // (deg)
      thetaZ =  0.0; // (deg)
      // Global translation in Lab
      tx    =  0.0; // (cm)
      ty    =  0.0; // (cm)
      tz    =  1000.0; // (cm)
      //land->SetRotAnglesEuler(phi,theta,psi);
      land->SetRotAnglesXYZ(thetaX,thetaY,thetaZ);
      land->SetTranslation(tx,ty,tz);
      run->AddModule(land);
  }

  // -----   Create SOFIA  magnetic field ----------------------------------------
  Int_t typeOfMagneticField = 0;
  Int_t fieldScale = 1;
  Bool_t fVerbose = kFALSE;

  // If the Global Position of the Magnet is changed
  // the Field Map has to be transformed accordingly

  if (fFieldMap == 0) {
  //  R3BFieldMap* magField = new R3BFieldMap(typeOfMagneticField,fVerbose);
  //  magField->SetPosition(0., 0., 0.);
  //  magField->SetScale(fieldScale);

    R3BAladinFieldMap* magField = new R3BAladinFieldMap("AladinMaps");
    Double_t fMeasCurrent = 2350;// --Current experiment --
    magField->SetCurrent(fMeasCurrent);
    //magField->SetScale(fieldScale);


    if ( fR3BMagnet == kTRUE ) {
	run->SetField(magField);
    } else {
	run->SetField(NULL);
    }
  } else if(fFieldMap == 1){
    R3BGladFieldMap* magField = new R3BGladFieldMap("R3BGladMap");
    magField->SetPosition(0., 0., +350-119.94);
    magField->SetScale(fieldScale);

    if ( fR3BMagnet == kTRUE ) {
	run->SetField(magField);
    } else {
	run->SetField(NULL);
    }
  }  //! end of field map section

  // -----   Create PrimaryGenerator   --------------------------------------

  // 1 - Create the Main API class for the Generator
  FairPrimaryGenerator* primGen = new FairPrimaryGenerator();

  if (fGenerator.CompareTo("ion") == 0  ) {
	  // R3B Ion Generator
          Int_t z = 30;  // Atomic number
          Int_t a = 65; // Mass number
          Int_t q = 0;   // Charge State 
          Int_t m = 1;   // Multiplicity
          Double_t px = 0.05/a;  // X-Momentum / per nucleon!!!!!!
          Double_t py = 0.05/a;  // Y-Momentum / per nucleon!!!!!!
          Double_t pz = 70./a;  // Z-Momentum / per nucleon!!!!!!
	  R3BIonGenerator* ionGen = new R3BIonGenerator(z,a,q,m,px,py,pz);
	  ionGen->SetSpotRadius(1,1,0);
	  // add the ion generator
	  primGen->AddGenerator(ionGen);
  } 


  if (fGenerator.CompareTo("box") == 0  ) {
	  // 2- Define the BOX generator
	  Double_t pdgId=211; // pion beam
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
	
  if (fGenerator.CompareTo("gammas") == 0  ) {
	// 2- Define the CALIFA Test gamma generator
	Double_t pdgId=22; // gamma emission
	//Double_t pdgId=2212; // proton emission
	Double_t theta1= 90.;  // polar angle distribution
	Double_t theta2= 180.;	
	//Double_t theta2= 90.;	
	//Double_t momentum=0.002; // 0.010 GeV/c = 10 MeV/c 
	Double_t momentumI=0.019; // 0.010 GeV/c = 10 MeV/c 
	Double_t momentumF=0.045; // 0.010 GeV/c = 10 MeV/c 
	//Double_t momentumF=0.808065; // 0.808065 GeV/c (300MeV Kin Energy for protons) 
	//Double_t momentumI=0.31016124; // 0.31016124 GeV/c (50MeV Kin Energy for protons)
	//Double_t momentum=0.4442972; // 0.4442972 GeV/c (100MeV Kin Energy for protons)
	//Double_t momentum=0.5509999; // 0.5509999 GeV/c (150MeV Kin Energy for protons)
	//Double_t momentumI=0.64405; // 0.64405 GeV/c (200MeV Kin Energy for protons) 
	Int_t multiplicity = 1;
	R3BCALIFATestGenerator* gammasGen = new R3BCALIFATestGenerator(pdgId, multiplicity);
	gammasGen->SetThetaRange (theta1,   theta2);
	gammasGen->SetCosTheta();
	gammasGen->SetPRange(momentumI,momentumF);
	gammasGen->SetPhiRange(0.,360.);
	//gammasGen->SetXYZ(0.0,0.0,-1.5);
	//gammasGen->SetXYZ(0.0,0.0,0);
	gammasGen->SetBoxXYZ(-0.1,0.1,-0.1,0.1,-0.1,0.1);
	gammasGen->SetLorentzBoost(0.8197505718204776); //beta=0.81975 for 700 A MeV
	// add the gamma generator
	primGen->AddGenerator(gammasGen);
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

  // ------  Increase nb of step for CALO
  Int_t nSteps = -25000;
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


