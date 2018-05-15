////////////////////////////////////////////////////////////////////////////////////
//// ** elisabet.galiana@usc.es
//// ** Universidad de Santiago de Compostela
//// ** Dpto. Física de Partículas
////
////	--- Simulation of the Lisbon Nov-2016 setup ---
////
////	Macro to check:
////			-Detectors Responses: HPGe and Petals
////			-Gamma angular Correlation
////			-Angles of the Cystals (Petals)
////
////	Detailed study:
////			-- The HPGe & Petals Responses --
////	It analyzes the MCTracks of the primary and secondary particles
////	and the detector responses of the two Petals (three branches
////	of information: Point, Crystal and CaloHits) and the HPGe detector
////	(two branches: Point and Hits). (see note1)
////
////	Calculate:
////		-MCTrack: Primary and Secondary PDG Code,
////		 	  energy, theta&phi angles of Primary Particles
////		-HPGe: energy
////		-Point HPGE: Collides 2D & 3D
////		-Crystal: ID, Type, Copy, energy
////		-Calo: energy, theta&phi angles
////		-Point Petals: Collides 2D & 3D
////		-Both detectors: Collides 3D
////	Usage: DET_RESPONSE = true
////
////		        -- Gamma Angular Correlations --
////	It analyzes the Angular Correlations between primary gammas in MCTrack and
////	in the detectors applying the HPGe condition (see note2)
////
////	Calculate:
////		-MCTrack: angle between Primary Gammas
////		-HPGe: energy
////		-Applying the HPGe condition:
////			·Calo petals energy
////			·Calo energy of petal at 90º & 180º w.r.t.HPGe
////			·Calo theta & phi angles
////			·Multiplicity
////			·Theta vs Energy Petals, Phi vs Energy Petals
////			·Angle between gammas
////	Usage: ANG_CORRELATIONS = true
////
////			-- Angles of the Cystals --
////	It analyses the angles of each crystal in MCTrack and Calo
////	and check the differences between them
////	The crystals could be identified by their CrystalId (see note3)
////
////	Calculate:
////		-Theta angle (Differences between MCTrack and Calo)
////		-Phi angle (Differences between MCTrack and Calo)
////		-Theta MCTrack + Calo Theta angle (as points from the data file Calo_CryID_Theta_Phi.dat)
////		-Phi MCTrack + Calo Phi angle (as points from the same data file)
////		-Theta vs Phi Calo angles
////	Usage: CRY_ANGLES = true
////
////	General Usage:
////		1st: select the root file and the settings:
////			-maximum energy
////			-the type of analysis that you want	
////                     (activate or desactivate them using the bool true/false)
////			-choose one option for the energy limits 
////                     (if you are studying Angular Correlations in gammas)
////			-Histograms: change the ranges and bins (OPTIONAL)
////
////		2nd: root -l checkResults.C
////
////////////////////////////////////////////////////////////////////////////////////
//
//NOTE1: if you want to analyze the HPGePoint & CrystalPoint
// you have to activate them before to execute runsim.C, in order to create their branches
//
//	How to activate/desactivate them: descomment/comment these lines
//	HPGe: 
//       ctn/detector/EnsarHPGeDet.cxx
//          ->FairRootManager::Instance()->Register("HPGeDetPoint", GetName(), fPointCollection, kTRUE);
//	Califa: 
//        calo/cal/R3BCalo.cxx
//          ->FairRootManager::Instance()->Register("CrystalPoint", GetName(), fCaloCollection, kTRUE);
//
//NOTE2: The HPGe condition
//  It is the application of the condition that one low energy gamma arrives to the HPGe detector
//  and the other gamma, more energetic, arrives to one of both petals
//  we have to change the energy limits (limit1&limit2) for different cascades
//
//NOTE3:The Crystals could be identified by their CrystalId:
//		-Petal at 180º w.r.t.HPGe: CryId from 1 to 64
//		-Petal at 90º w.r.t.HPGe: CryId from 65 to 128

void checkResults() {
  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  
  /////////////////////////////////////////////////////////////
  //// SETTINGS: SELECT HERE THE SETTING CONTROLING THE MACRO
  /////////////////////////////////////////////////////////////
  //INPUT FILE
  char inputFile[250] = "outsim.root";  //change root file
  TFile *file1 = TFile::Open(inputFile);
  //READING TREE
  TTree* tree = (TTree*)file1->Get("ensartree");
  
  //If you are studying the Angular Correlations in gammas
  //Choose one option for these energy limits:
  //	1- if you want analyze Si-12900 cascade
  //	2- if you want analyze Co-60 cascade
  //	3- if you are not analyzing any cascade
  Int_t option=2;
  
  //Change the maximum energy (MeV), examples:
  //	Si-12900 cascade(12.9MeV): maxE=13.;
  //	Co-60 cascade(2.504MeV): maxE=3.;
  Double_t maxE = 3.0;
  
  //Choose the type of analysis that you want:
  //	-Detectors Responses (DET_RESPONSE = true)
  //	-Angular Correlations in gammas (ANG_CORRELATIONS = true)
  //	-Angles of the Cystals (CRY_ANGLES=true)
  Bool_t DET_RESPONSE = true;
  Bool_t ANG_CORRELATIONS = true;
  Bool_t CRY_ANGLES= false;
  
  ////////////////////////////////////////////////////////////////
  //// END OF SETTINGS: HISTOGRAMS RANGE COULD BE MODIFIED BELOW
  ////////////////////////////////////////////////////////////////
  
  //VARIABLES DEFINITION
  Float_t X1=0.,X2=0.,X3=0.,X4=0.,Xup=0.,Xdown=0.;
  Double_t energyGe = 0.0;
  Double_t energy = 0.0;
  Double_t Theta1_MC=0., Theta2_MC=0.;
  Double_t Phi1_MC=0., Phi2_MC=0.;
  Double_t E1_MC=0., E2_MC=0.;
  Double_t px1_MC=0., py1_MC=0., pz1_MC=0.;
  Double_t px2_MC=0., py2_MC=0., pz2_MC=0.;
  Double_t m1_MC=0., m2_MC=0.;
  Double_t M1_MC=0., M2_MC=0.;
  Double_t cos_alpha_MC=0.;
  Double_t alpha_MC=0.;
  Double_t limit1 = 0.0, limit2=0.0;
  Double_t X=0., Y=0., Z=0.;
  TVector3 momentum, momentum2, vector3;
  
  switch(option){
  case 1:
    limit1=1.779 - 0.01*1.779;//-1% of gamma energy=1.779
    limit2=1.779 + 0.01*1.779;//+1%
    break;
  case 2:
    limit1=1.172 - 0.01*1.172;//-1% of gamma energy=1.172
    limit2=1.172 + 0.01*1.172;//+1%
    break;
  case 3:
    limit1=0.0;
    limit2=0.0;
    break;
  }
  
  //HISTOGRAMS DEFINITION-----------------------------------------------------------
  TH1F *h1, *h1_2, *h2, *h3, *h4, *h_Ge, *h1_Cry, *h2_Cry, *h3_Cry, *h4_Cry, *h1_Cal, *h2_Cal, *h3_Cal;
  TH1F *h2_Ge, *h4_Cal, *h5_Cal_1, *h5_Cal_2, *h6_Cal, *h7_Cal, *h8_Cal, *h_alpha_MC, *h_alpha_W;
  TH2F *hyz_Ge, *hxz_Point, *h9_Cal, *h10_Cal, *h1_CryAng, *h2_CryAng, *h3_CryAng, *h4_CryAng, *h5_CryAng;
  TH3F *hxyz_Ge, *hxyz_Point, *hxyz_both;
  
  //HISTOGRAMS CREATION ------------------------------------------------------------  Change these ranges and bins (OPTIONAL)
  //Detectors Response ------
  if(DET_RESPONSE){
    h1   = new TH1F("h1","Primary PDG Code",60,30,30);
    h1_2 = new TH1F("h1_2","Secondaries PDG Code",60,30,30);
    h2   = new TH1F("h2","Primary Energy",1000,0,maxE);
    h3   = new TH1F("h3","Primary Theta",320,0,3.2);
    h4   = new TH1F("h4","Primary Phi",640,-3.2,3.2);
    
    h_Ge    = new TH1F("h_Ge","HPGe Energy",1000,0,maxE);
    hyz_Ge  = new TH2F("hyz_Ge","HPGe Collides 2D (y,z)", 100,-3,3,100,-3,3);
    hxyz_Ge = new TH3F("hxyz_Ge","HPGe Collides 3D (x,y,z)", 80,-4,4,80,-4,4,80,-10,-18);
    
    h1_Cry = new TH1F("h1_Cry","Crystal ID",130,0,130);
    h2_Cry = new TH1F("h2_Cry","Crystal Energy",200,0,maxE);
    h3_Cry = new TH1F("h3_Cry","Crystal Type",32,0,32);
    h4_Cry = new TH1F("h4_Cry","Crystal Copy",15,0,15);
    
    h1_Cal = new TH1F("h1_Cal","Petals Energy",200,0,maxE);
    h2_Cal = new TH1F("h2_Cal","Calo Theta",320,0,3.2);
    h3_Cal = new TH1F("h3_Cal","Calo Phi",640,-3.2,3.2);
    
    hxz_Point  = new TH2F("hxz_Point","Petals Collides 2D (x,z)", 200,-30.,70.,160,-20.,60.);
    hxyz_Point = new TH3F("hxyz_Point","Petals Collides 3D (x,z,y)", 200,-30.,70.,40,-10.,10.,160,-20.,60.);
    hxyz_both  = new TH3F("hxyz_both","Collides 3D both detectors", 200,-30.,70., 40,-10.,10. ,160,-20.,60.);
  }
  
  //Angular Correlations -----
  if(ANG_CORRELATIONS){
    h2_Ge    = new TH1F("h2_Ge","HPGe Total Energy",1000,0.,maxE);
    h4_Cal   = new TH1F("h4_Cal","Both Petals Energy with HPGe condition",1000,0,maxE);
    h5_Cal_1 = new TH1F("h5_Cal_1","Energy of Petal at 90#circ w.r.t. HPGe",1000,0,maxE);
    h5_Cal_2 = new TH1F("h5_Cal_2","Energy of Petal at 180#circ w.r.t. HPGe",1000,0,maxE);
    h6_Cal   = new TH1F("h6_Cal","CaloHits Multiplicity with HPGe condition",10,0,10);
    h7_Cal   = new TH1F("h7_Cal","Calo Theta  with HPGe condition",320,0,3.2);
    h8_Cal   = new TH1F("h8_Cal","Calo Phi  with HPGe condition",640,-3.2,3.2);
    h9_Cal   = new TH2F("h9_Cal","Theta vs Energy Petals with HPGe condition", 32,0,3.2,100,0.,maxE);
    h10_Cal  = new TH2F("h10_Cal","Phi vs Energy Petals with HPGe condition", 64,-3.2,3.2,100,0.,maxE);
    h_alpha_MC  = new TH1F("h_alpha_MC","MCTrack Reconstructed Angle",360,0.,3.6);
    h_alpha_W   = new TH1F("h_alpha_W","HPGe & Calo Reconstructed Angle",360,0.,3.6);
  }
  
  //Angles of the Crystals
  if(CRY_ANGLES){
    h1_CryAng = new TH2F("h1_CryAng","Theta Angle (MCTrack-Calo)", 130,0,130,500,-0.45,0.45);
    h2_CryAng = new TH2F("h2_CryAng","Phi Angle (MCTrack-Calo)", 130,0,130,1000,-3.14,3.14);
    h3_CryAng = new TH2F("h3_CryAng","Theta MCTrack for each CryId", 130,0,130,500,0.,2.5);
    h4_CryAng = new TH2F("h4_CryAng","Phi MCTrack for each CryId", 130,0,130,3600,-3.14,3.14);
    h5_CryAng = new TH2F("h5_CryAng","Theta vs Phi Calo angles for each CryId", 1800,0.,3.14,3600,-3.14,3.14); // the bins that agree on the Crystal Resoltuion are:
    // bin_Theta=1800 Theta [0.,3.14]
    // bin_Phi=3600   Phi [-3.14,3.14]
  }
  
  //GRAPH definition
  TGraph *gr1, *gr2;
  
  
  //----   MCTrack (input)   -------------------------------------------------------
  TClonesArray* MCTrackCA;
  EnsarMCTrack** track;
  MCTrackCA = new TClonesArray("EnsarMCTrack",5);
  TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
  branchMCTrack->SetAddress(&MCTrackCA);
  
  //----- HPGe detector--------------//
  
  //HPGe Hits
  TClonesArray* hpgeHitCA;
  EnsarHPGeDetHit** hpgeHit;
  hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
  TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
  branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );
  
  //HPGe Points
  TClonesArray* hpgePointCA;
  EnsarHPGeDetPoint** hpgePoint;
  hpgePointCA = new TClonesArray("EnsarHPGeDetPoint",5);
  TBranch *branchEnsarHPGeDetPoint = tree ->GetBranch("HPGeDetPoint");
  branchEnsarHPGeDetPoint->SetAddress(&hpgePointCA );
  
  
  //----- CALIFA detector--------------//
  //Crystal Points
  TClonesArray* crystalHitCA;
  R3BCaloCrystalHitSim** crystalHit;
  crystalHitCA = new TClonesArray("R3BCaloCrystalHitSim",5);
  TBranch *branchCrystalHit = tree->GetBranch("CrystalHitSim");
  branchCrystalHit->SetAddress(&crystalHitCA);
  
  //Calo Hits
  TClonesArray* caloHitCA;
  R3BCaloHitSim** caloHit;
  caloHitCA = new TClonesArray("R3BCaloHitSim",5);
  TBranch *branchCaloHit = tree->GetBranch("CaloHitSim");
  branchCaloHit->SetAddress(&caloHitCA);
  
  
  //Calo Points
  TClonesArray* crystalPointCA;
  R3BCaloPoint** crystalPoint;
  crystalPointCA = new TClonesArray("R3BCaloPoint",5);
  TBranch *branchcrystalPoint = tree->GetBranch("CrystalPoint");
  branchcrystalPoint->SetAddress(&crystalPointCA);
  
  
  Int_t MCtracksPerEvent = 0;
  Int_t hpgeHitsPerEvent = 0;
  Int_t hpgePointsPerEvent = 0;
  Int_t crystalHitsPerEvent = 0;
  Int_t caloHitsPerEvent=0;
  Int_t crystalPointPerEvent=0;
  
  
  //TREE ENTRIES--------------------------------------------------------------------
  Long64_t nevents = tree->GetEntries();
  
  
  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    energy = 0.;
    tree->GetEvent(i);
    
    MCtracksPerEvent     = MCTrackCA->GetEntries();
    hpgeHitsPerEvent     = hpgeHitCA->GetEntries();
    hpgePointsPerEvent   = hpgePointCA->GetEntries();
    crystalHitsPerEvent  = crystalHitCA->GetEntries();
    caloHitsPerEvent     = caloHitCA->GetEntries();
    crystalPointPerEvent = crystalPointCA->GetEntries();
    
    if(MCtracksPerEvent>0) {
      track = new EnsarMCTrack*[MCtracksPerEvent];
      for(Int_t j=0;j<MCtracksPerEvent;j++){
	track[j] = new EnsarMCTrack;
	track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
      }
    }
    if(hpgeHitsPerEvent>0) {
      hpgeHit = new EnsarHPGeDetHit*[hpgeHitsPerEvent];
      for(Int_t j=0;j<hpgeHitsPerEvent;j++){
	hpgeHit[j] = new EnsarHPGeDetHit;
	hpgeHit[j] = (EnsarHPGeDetHit*) hpgeHitCA->At(j);
      }
    }
    if(hpgePointsPerEvent>0) {
      hpgePoint = new EnsarHPGeDetPoint*[hpgePointsPerEvent];
      for(Int_t j=0;j<hpgePointsPerEvent;j++){
	hpgePoint[j] = new EnsarHPGeDetPoint;
	hpgePoint[j] = (EnsarHPGeDetPoint*) hpgePointCA->At(j);
      }
    }
    if(crystalHitsPerEvent>0) {
      crystalHit = new R3BCaloCrystalHitSim*[crystalHitsPerEvent];
      for(Int_t j=0;j<crystalHitsPerEvent;j++){
	crystalHit[j] = new R3BCaloCrystalHitSim;
	crystalHit[j] = (R3BCaloCrystalHitSim*) crystalHitCA->At(j);
      }
    }
    if(caloHitsPerEvent>0) {
      caloHit = new R3BCaloHitSim*[caloHitsPerEvent];
      for(Int_t j=0;j<caloHitsPerEvent;j++){
	caloHit[j] = new R3BCaloHitSim;
	caloHit[j] = (R3BCaloHitSim*) caloHitCA->At(j);
      }
    }
    if(crystalPointPerEvent>0) {
      crystalPoint = new R3BCaloPoint*[crystalPointPerEvent];
      for(Int_t j=0;j<crystalPointPerEvent;j++){
	crystalPoint[j] = new R3BCaloPoint;
	crystalPoint[j] = (R3BCaloPoint*) crystalPointCA->At(j);
      }
    }
    
    //ANALYSIS of the Detectors Response -------------------------------------------------------------------
    if(DET_RESPONSE){
      
      //LOOP in MC mother tracks ----
      for(Int_t h=0;h<MCtracksPerEvent;h++){
				if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
					h1->Fill(track[h]->GetPdgCode());
					h2->Fill(track[h]->GetEnergy()*1000);//MeV
					track[h]->GetMomentum(momentum);
					h3->Fill(momentum.Theta());
					h4->Fill(momentum.Phi());
				} else {
					h1_2->Fill(track[h]->GetPdgCode());
				}
	
				//LOOP in HPGe Points
				for(Int_t r=0;r<hpgePointsPerEvent;r++) { //LOOP in HPGe Points for each MCTrack
					hpgePoint[r]->PositionIn(vector3);
					X1=hpgePoint[r]->GetXIn();
					X2=hpgePoint[r]->GetYIn();
					X3=hpgePoint[r]->GetZIn();
					hyz_Ge->Fill(X2,X3);
					hxyz_Ge->Fill(X1,X2,X3);
					hxyz_both->Fill(X1,X2,X3);
				}
      }
      
      
      //LOOP in HPGe Hits ----
      for(Int_t h=0;h<hpgeHitsPerEvent;h++){
	energy = hpgeHit[h]->GetEnergy()*1000;//MeV
	h_Ge->Fill(energy);
      }
      
      //LOOP in Crystal Hits ----
      for(Int_t h=0;h<crystalHitsPerEvent;h++){
	h1_Cry->Fill(crystalHit[h]->GetCrystalId());
	h2_Cry->Fill(crystalHit[h]->GetEnergy()*1000);//MeV
	h3_Cry->Fill(crystalHit[h]->GetCrystalType());
	h4_Cry->Fill(crystalHit[h]->GetCrystalCopy());
      }
      
      //LOOP in Calorimeter Hits ----
      for(Int_t h=0;h<caloHitsPerEvent;h++){
	h1_Cal->Fill(caloHit[h]->GetEnergy()*1000); //MeV
	h2_Cal->Fill(caloHit[h]->GetTheta());	    //rad
	h3_Cal->Fill(caloHit[h]->GetPhi());	    	//rad
      }
      
      
      //LOOP in Crystal Points ----
      for(Int_t h=0;h<crystalPointPerEvent;h++){
	
	X=crystalPoint[h]->GetXIn();
	Y=crystalPoint[h]->GetYIn();
	Z=crystalPoint[h]->GetZIn();
	hxz_Point->Fill(X,Z);
	hxyz_Point->Fill(X,Y,Z);
	hxyz_both->Fill(X,Y,Z);
      }
      
    }//END analysis of the Destectors Response -----------------------------------------------------------
    
    
    
    //ANALYSIS of the Angular Correlations in gammas -----------------------------------------------------
    if(ANG_CORRELATIONS){
      
      //LOOP in MC mother tracks ----
      for(Int_t h=0;h<MCtracksPerEvent;h++){
	
	if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
	  
	  track[h]->GetMomentum(momentum2);
	  if (h==0){
	    E1_MC     = track[h]->GetEnergy()*1000;//MeV
	    Theta1_MC = momentum2.Theta();
	    Phi1_MC   = momentum2.Phi();
	  }else{
	    E2_MC     = track[h]->GetEnergy()*1000;//MeV
	    Theta2_MC = momentum2.Theta();
	    Phi2_MC   = momentum2.Phi();
	  }
	}
      }
      
      
      
      px1_MC= E1_MC*TMath::Sin(Theta1_MC)*TMath::Cos(Phi1_MC);
      py1_MC= E1_MC*TMath::Sin(Theta1_MC)*TMath::Sin(Phi1_MC);
      pz1_MC= E1_MC*TMath::Cos(Theta1_MC);
      
      px2_MC= E2_MC*TMath::Sin(Theta2_MC)*TMath::Cos(Phi2_MC);
      py2_MC= E2_MC*TMath::Sin(Theta2_MC)*TMath::Sin(Phi2_MC);
      pz2_MC= E2_MC*TMath::Cos(Theta2_MC);
      
      m1_MC = pow(px1_MC,2) + pow(py1_MC,2) + pow(pz1_MC,2);
      M1_MC = sqrt(m1_MC);
      
      m2_MC = pow(px2_MC,2) + pow(py2_MC,2) + pow(pz2_MC,2);
      M2_MC = sqrt(m2_MC);
      
      cos_alpha_MC = (px1_MC*px2_MC + py1_MC*py2_MC + pz1_MC*pz2_MC)/(M1_MC*M2_MC);
      alpha_MC = TMath::ACos( cos_alpha_MC );
      h_alpha_MC->Fill(alpha_MC,1/TMath::Sin(alpha_MC));
      
      //LOOP in HPGe Hits ----
      for(Int_t h=0;h<hpgeHitsPerEvent;h++){
	
	energyGe = hpgeHit[h]->GetEnergy()*1000;//MeV
	h2_Ge->Fill(energyGe);
	
	if (energyGe>limit1 && energyGe<limit2){ //Applying the HPGe condition
	  //change the limit1 & limit2 for different cascades
	  Double_t px1=0., py1=0., pz1=0.;
	  Double_t px2=0., py2=0., pz2=0.;
	  Double_t Theta2=0., ThetaGe=0.;
	  Double_t Phi2=0., PhiGe=0.;
	  Double_t E2=0.;
	  
	  //Angles of the Ge detector
	  ThetaGe = TMath::Pi()/2.;
	  PhiGe   = TMath::Pi();
	  
	  px1= energyGe*TMath::Sin(ThetaGe)*TMath::Cos(PhiGe);
	  py1= energyGe*TMath::Sin(ThetaGe)*TMath::Sin(PhiGe);
	  pz1= energyGe*TMath::Cos(ThetaGe);
	  
	  //LOOP in Calo Hits-------
	  for(Int_t i=0;i<caloHitsPerEvent;i++){
	    
	    h4_Cal->Fill(caloHit[i]->GetEnergy()*1000);  //MeV
	    h7_Cal->Fill(caloHit[i]->GetTheta());	 //rad
	    h8_Cal->Fill(caloHit[i]->GetPhi());	    	 //rad
	    
	    Theta2 = caloHit[i]->GetTheta();
	    Phi2 = caloHit[i]->GetPhi();
	    E2 = caloHit[i]->GetEnergy()*1000;
	    
	    px2= E2*TMath::Sin(Theta2)*TMath::Cos(Phi2);
	    py2= E2*TMath::Sin(Theta2)*TMath::Sin(Phi2);
	    pz2= E2*TMath::Cos(Theta2);
	    
	    if (caloHitsPerEvent>0){
	      h6_Cal->Fill(caloHitsPerEvent); //Multiplicity
	    }
	    
	    if (Theta2<1.){
	      h5_Cal_1->Fill(caloHit[i]->GetEnergy()*1000); //Petal at 90º w.r.t. HPGe
	    }else {
	      h5_Cal_2->Fill(caloHit[i]->GetEnergy()*1000); //Petal at 180º w.r.t. HPGe
	    }
	    
	    h9_Cal->Fill(Theta2,E2);
	    h10_Cal->Fill(Phi2,E2);
	    
	    
	    
	    Double_t alpha=0.; //angle between two gammas, one to the HPGe and the other to one Petal
	    if (caloHitsPerEvent>0){
	      Double_t m1=0., m2=0.;
	      Double_t M1=0., M2=0.;
	      Double_t cos_alpha=0.;
	      
	      
	      m1 = pow(px1,2) + pow(py1,2) + pow(pz1,2);
	      M1 = sqrt(m1);
	      
	      m2 = pow(px2,2) + pow(py2,2) + pow(pz2,2);
	      M2 = sqrt(m2);
	      
	      cos_alpha = (px1*px2 + py1*py2 + pz1*pz2)/(M1*M2);
	      alpha = TMath::ACos( cos_alpha );
	      h_alpha_W->Fill(alpha,1/TMath::Sin(alpha));
	      
	    }
	  }
	}
      }
      
    }//END analysis Angular Correlations -----------------------------------------------------------
    
    
    //ANALYSIS of the Angles Crystals --------------------------------------------------------------
    if(CRY_ANGLES){
      
      //LOOP in MC mother tracks----------------------------------------------
      Double_t Theta_MC2=0.;
      Double_t Phi_MC2=0.;
      
      for(Int_t h=0;h<MCtracksPerEvent;h++){
	
	if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
	  
	  track[h]->GetMomentum(momentum);
	  if (h==0){
	    Theta_MC2 = momentum.Theta();
	    Phi_MC2   = momentum.Phi();
	  }
	}
      }
      
      //LOOP in Calo Hits----------------
      Double_t Theta_Calo=0.;
      Double_t Phi_Calo=0.;
      
      for(Int_t i=0;i<caloHitsPerEvent;i++){
	
	h5_CryAng->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
	
	if (caloHitsPerEvent==1 && caloHit[i]->GetNbOfCrystalHits()==1){
	  Theta_Calo=caloHit[i]->GetTheta();
	  Phi_Calo=caloHit[i]->GetPhi();
	}
      }
      
      //LOOP in Crystal Hits-------------
      Int_t CryId=0.;
      
      for(Int_t j=0;j<crystalHitsPerEvent;j++){
	if(crystalHitsPerEvent==1){
	  CryId=crystalHit[j]->GetCrystalId();
	}
      }
      
      //Calculation of the difference
      Double_t diff=0., diff2=0.;
      
      if(Theta_Calo>0) diff  = Theta_MC2 - Theta_Calo;
      diff2 = Phi_MC2 - Phi_Calo;
      
      if(CryId>0 && Theta_Calo>0){
	h1_CryAng->Fill(CryId, diff);
	h2_CryAng->Fill(CryId, diff2);
	h3_CryAng->Fill(CryId, Theta_MC2);
	h4_CryAng->Fill(CryId, Phi_MC2);
      }
      
    }//END analysis Angles Crystals ----------------------------------------------------------------
    
    
    if(MCtracksPerEvent)     delete[] track;
    if(hpgeHitsPerEvent)     delete[] hpgeHit;
    if(hpgePointsPerEvent)   delete[] hpgePoint;
    if(crystalHitsPerEvent)  delete[] crystalHit;
    if(caloHitsPerEvent)     delete[] caloHit;
    if(crystalPointPerEvent) delete[] crystalPoint;
    
  }
  // END LOOP IN THE EVENTS----------------------------------------------------------------------------------------------------
  
  if(CRY_ANGLES){
    //Read File CrystalId & Theta,Phi of CaloHit
    ifstream *File = new ifstream("/home/fpddv1/elisabet.galiana/Escritorio/ENSARRoot/EnsarRoot_source/ctn/macros/nov16/Calo_CryID_Theta_Phi.dat");
    
    const Int_t nLevel=128;
    Double_t CryId_array[nLevel];
    Double_t Theta_Calo_array[nLevel];
    Double_t Phi_Calo_array[nLevel];
    
    for(Int_t i=0;i<8;i++){
      File->ignore(256,'\n');
    }
    
    for(Int_t i = 0; i < nLevel; i++){
      *File >> CryId_array[i] >> Theta_Calo_array[i] >> Phi_Calo_array[i];
      cout<<"CryId= "<< CryId_array[i] <<"\t Theta_Calo= "<< Theta_Calo_array[i] <<"\t Phi_Calo= "<< Phi_Calo_array[i] << endl;
    }
    
    //GRAPH
    gr1 = new TGraph(nLevel,CryId_array,Theta_Calo_array);
    gr2 = new TGraph(nLevel,CryId_array,Phi_Calo_array);
  }
  
  // HISTOGRAMS---------------------------------------------------------------------
  
  if(DET_RESPONSE){
    //MCTrack-------------
    TCanvas* c1 = new TCanvas("MCTrack","MCTrack",0,0,400,800);
    c1->Divide(1,2);
    c1->SetFillColor(0);
    c1->SetFrameFillColor(0);
    c1->cd(1);
    h1_2->Draw();
    h1_2->GetXaxis()->SetTitle("PDG Code");
    h1_2->GetYaxis()->SetTitle("Counts");
    h1_2->GetYaxis()->SetRangeUser(0.,300000);
    h1->Draw("same");
    h1->SetLineColor(2);
    h1_2->SetLineColor(8);
    //////////////////////
    TLegend *leg1 = new TLegend(0.2,0.7,0.5,0.8);
    leg1->AddEntry(h1, "Primary Particles", "l");
    leg1->SetFillColor(0);
    leg1->Draw();
    leg1->AddEntry(h1_2, "Secondary Particles", "l");
    ////////////////////////////////
    c1->cd(2);
    h2->Draw();
    h2->SetLineColor(9);
    h2->GetXaxis()->SetTitle("MCTrack Primary Energy (MeV)");
    h2->GetYaxis()->SetTitle("Counts");
    
    TCanvas* c2 = new TCanvas("MCTrack Angles","MCTrack Angles",0,0,400,800);
    c2->Divide(2,1);
    c2->SetFillColor(0);
    c2->SetFrameFillColor(0);
    c2->cd(1);
    h3->Draw();
    h3->SetLineColor(9);
    h3->GetXaxis()->SetTitle("MCTrack Primary Theta (rad)");
    h3->GetYaxis()->SetTitle("Counts");
    c2->cd(2);
    h4->Draw();
    h4->SetLineColor(9);
    h4->GetXaxis()->SetTitle("MCTrack Primary Phi (rad)");
    h4->GetYaxis()->SetTitle("Counts");
    
    
    //HPGe-------------
    TCanvas* c3 = new TCanvas("Energy in the HPGe","Energy in the HPGe",0,0,400,400);
    c3->SetFillColor(0);
    c3->SetFrameFillColor(0);
    Int_t ci;// for color index setting
    ci = TColor::GetColor("#000099");
    h_Ge->SetLineColor(ci);
    h_Ge->SetLineWidth(2);
    h_Ge->GetXaxis()->SetTitle("HPGe Energy (MeV)");
    h_Ge->GetYaxis()->SetTitle("Counts");
    h_Ge->Draw();
    
    TCanvas* c4 = new TCanvas("Collides 2D HPGe","Collides 2D HPGe",0,0,400,800);
    c4->SetFillColor(0);
    c4->SetFrameFillColor(0);
    hyz_Ge-> Draw("colz");
    //hyz_Ge-> Draw("lego2z");  //draw a Lego 2D with a palete of colours at right side
    //hyz_Ge-> Draw("surf2z");  //draw a surface with a palete of colours at right side
    hyz_Ge->GetXaxis()->SetTitle("x (cm)");
    hyz_Ge->GetYaxis()->SetTitle("y (cm)");
    
    TCanvas* c5 = new TCanvas("Collides 3D HPGe","Collides 3D HPGe",0,0,400,800);
    c5->SetFillColor(0);
    c5->SetFrameFillColor(0);
    hxyz_Ge-> Draw("");
    hxyz_Ge->SetMarkerStyle(20);
    hxyz_Ge->SetMarkerSize(0.4);
    hxyz_Ge->SetMarkerColor(9);
    hxyz_Ge->GetXaxis()->SetTitle("x (cm)");
    hxyz_Ge->GetYaxis()->SetTitle("y (cm)");
    hxyz_Ge->GetZaxis()->SetTitle("z (cm)");
    
    //CALIFA ----------------------
    //CrystalHit
    TCanvas* c6 = new TCanvas("CrystalHit","CrystalHit",0,0,720,900);
    c6->SetFillColor(0);
    c6->SetFrameFillColor(0);
    c6->Divide(2,2);
    c6->cd(1);
    h1_Cry->Draw();
    h1_Cry->SetLineColor(6);
    h1_Cry->GetXaxis()->SetTitle("Crystal ID");
    h1_Cry->GetYaxis()->SetTitle("Counts");
    c6->cd(2);
    h2_Cry->Draw();
    h2_Cry->SetLineColor(9);
    h2_Cry->GetXaxis()->SetTitle("Crystal Energy (MeV)");
    h2_Cry->GetYaxis()->SetTitle("Counts");
    c6->cd(3);
    h3_Cry->Draw();
    h3_Cry->SetLineColor(2);
    h3_Cry->GetXaxis()->SetTitle("Crystal Type");
    h3_Cry->GetYaxis()->SetTitle("Counts");
    c6->cd(4);
    h4_Cry->Draw();
    h4_Cry->SetLineColor(8);
    h4_Cry->GetXaxis()->SetTitle("Crystal Copy");
    h4_Cry->GetYaxis()->SetTitle("Counts");
    
    
    //CaloHit
    TCanvas* c7 = new TCanvas("CaloHit","CaloHit",0,0,720,900);
    c7->SetFillColor(0);
    c7->SetFrameFillColor(0);
    c7->Divide(2,2);
    c7->cd(1);
    h1_Cal->Draw();
    h1_Cal->SetLineColor(kOrange);
    h1_Cal->GetXaxis()->SetTitle("Calo Energy (MeV)");
    h1_Cal->GetYaxis()->SetTitle("Counts");
    c7->cd(2);
    h2_Cal->Draw();
    h2_Cal->SetLineColor(9);
    h2_Cal->GetXaxis()->SetTitle("Calo Theta (rad)");
    h2_Cal->GetYaxis()->SetTitle("Counts");
    c7->cd(3);
    h3_Cal->Draw();
    h3_Cal->SetLineColor(13);
    h3_Cal->GetXaxis()->SetTitle("Calo Phi (rad)");
    h3_Cal->GetYaxis()->SetTitle("Counts");
    
    //Crystal Point
    TCanvas* c8 = new TCanvas("Collides 2D Petals","Collides 2D (x,z) ",0,0,400,800);
    c8->SetFillColor(0);
    c8->SetFrameFillColor(0);
    hxz_Point-> Draw("colz");
    hxz_Point->GetXaxis()->SetTitle("X (cm)");
    hxz_Point->GetYaxis()->SetTitle("Z (cm)");
    
    TCanvas* c9 = new TCanvas("Collides 3D Petals","Collides 3D (x,y,z)",0,0,400,800);
    c9->SetFillColor(0);
    c9->SetFrameFillColor(0);
    hxyz_Point->Draw("");
    hxyz_Point->SetMarkerStyle(20);
    hxyz_Point->SetMarkerSize(0.4);
    hxyz_Point->SetMarkerColor(9);
    hxyz_Point->GetXaxis()->SetTitle("X (cm)");
    hxyz_Point->GetZaxis()->SetTitle("Z (cm)");
    hxyz_Point->GetYaxis()->SetTitle("Y (cm)");
    
    //HPGePoint & CrystalPoint at the same time
    TCanvas* c10 = new TCanvas("Collides3D both det","Collides 3D of both detectors",0,0,400,800);
    c10->SetFillColor(0);
    c10->SetFrameFillColor(0);
    hxyz_both->Draw();
    hxyz_both->SetMarkerStyle(20);
    hxyz_both->SetMarkerSize(0.4);
    hxyz_both->SetMarkerColor(6);
    hxyz_both->GetXaxis()->SetTitle("X (cm)");
    hxyz_both->GetYaxis()->SetTitle("Y (cm)");
    hxyz_both->GetZaxis()->SetTitle("Z (cm)");
    
    
    TCanvas* c11 = new TCanvas("Energies of the Detectors","Energies of the Detectors",0,0,400,800);
    c11->SetFillColor(0);
    c11->SetFrameFillColor(0);
    c11->Divide(2,2);
    c11->cd(1);
    h2->Draw();		c11->cd(1)->SetLogy();
    h2->SetLineColor(6);
    h2->GetXaxis()->SetTitle("MCTrack Energy (MeV)");
    h2->GetYaxis()->SetTitle("Counts");
    c11->cd(2);
    h_Ge->Draw(); 	c11->cd(2)->SetLogy();
    h_Ge->SetLineColor(9);
    h_Ge->GetXaxis()->SetTitle("HPGe Energy (MeV)");
    h_Ge->GetYaxis()->SetTitle("Counts");
    c11->cd(3);
    h2_Cry->Draw();	c11->cd(3)->SetLogy();
    h2_Cry->SetLineColor(8);
    h2_Cry->GetXaxis()->SetTitle("Crystal Energy (MeV)");
    h2_Cry->GetYaxis()->SetTitle("Counts");
    c11->cd(4);
    h1_Cal->Draw();	c11->cd(4)->SetLogy();
    h1_Cal->SetLineColor(kOrange);
    h1_Cal->GetXaxis()->SetTitle("Calo Energy (MeV)");
    h1_Cal->GetYaxis()->SetTitle("Counts");
    
  }
  
  
  
  if(ANG_CORRELATIONS){
    
    //HPGe & CaloHit
    TCanvas* c12 = new TCanvas("Energy and Petals with HPGe Condition","Energy HPGe and Petals with HPGe condition",0,0,400,800);
    c12->SetFillColor(0);
    c12->SetFrameFillColor(0);
    c12->Divide(2,2);
    c12->cd(1);
    h2_Ge->Draw();		c12->cd(1)->SetLogy();
    h2_Ge->SetLineColor(9);
    h2_Ge->GetXaxis()->SetTitle("HPGe Energy (MeV)");
    h2_Ge->GetYaxis()->SetTitle("Counts");
    c12->cd(2);
    h4_Cal->Draw(); 	c12->cd(2)->SetLogy();
    h4_Cal->SetLineColor(kOrange);
    h4_Cal->GetXaxis()->SetTitle("Calo Energy with HPGe condition (MeV)");
    h4_Cal->GetYaxis()->SetTitle("Counts");
    c12->cd(3);
    h5_Cal_1->Draw(); 	c12->cd(3)->SetLogy();
    h5_Cal_1->SetLineColor(kRed);
    h5_Cal_1->GetXaxis()->SetTitle("Calo Energy with HPGe condition (MeV)");
    h5_Cal_1->GetYaxis()->SetTitle("Counts");
    c12->cd(4);
    h5_Cal_2->Draw(); 	c12->cd(4)->SetLogy();
    h5_Cal_2->SetLineColor(kGreen);
    h5_Cal_2->GetXaxis()->SetTitle("Calo Energy with HPGe condition (MeV)");
    h5_Cal_2->GetYaxis()->SetTitle("Counts");
    
    //CaloHit
    TCanvas* c13 = new TCanvas("CaloHits","CaloHits",0,0,400,800);
    c13->SetFillColor(0);
    c13->SetFrameFillColor(0);
    c13->Divide(2,2);
    c13->cd(1);
    h6_Cal->Draw();
    h6_Cal->SetLineColor(7);
    h6_Cal->GetXaxis()->SetTitle("Multiplicity");
    h6_Cal->GetYaxis()->SetTitle("Counts");
    c13->cd(2);
    h7_Cal->Draw();
    h7_Cal->SetLineColor(9);
    h7_Cal->GetXaxis()->SetTitle("Calo Theta (rad)");
    h7_Cal->GetYaxis()->SetTitle("Counts");
    c13->cd(3);
    h8_Cal->Draw();
    h8_Cal->SetLineColor(13);
    h8_Cal->GetXaxis()->SetTitle("Calo Phi (rad)");
    h8_Cal->GetYaxis()->SetTitle("Counts");
    
    TCanvas* c14 = new TCanvas("Calo with HPGe Condition","Calo with HPGe Condition",0,0,720,900);
    c14->SetFillColor(0);
    c14->SetFrameFillColor(0);
    h9_Cal->Draw("colz");
    h9_Cal->GetXaxis()->SetTitle("Calo Theta (rad)");
    h9_Cal->GetYaxis()->SetTitle("Energy (MeV)");
    
    TCanvas* c15 = new TCanvas("Calo2 with HPGe Condition","Calo2 with HPGe Condition",0,0,720,900);
    c15->SetFillColor(0);
    c15->SetFrameFillColor(0);
    h10_Cal->Draw("colz");
    h10_Cal->GetXaxis()->SetTitle("Calo Phi (rad)");
    h10_Cal->GetYaxis()->SetTitle("Energy (MeV)");
    
    //Reconstructed angle between gammas
    TCanvas* c16 = new TCanvas("Reconstructed angle between gammas","Reconstructed angle between gammas",0,0,400,800);
    c16->SetFillColor(0);
    c16->SetFrameFillColor(0);
    c16->Divide(1,2);
    c16->cd(1);
    h_alpha_MC->Draw();
    h_alpha_MC->SetLineColor(9);
    h_alpha_MC->GetXaxis()->SetTitle("Angle between primary gammas MCTrack (rad)");
    h_alpha_MC->GetYaxis()->SetTitle("Counts");
    c16->cd(2);
    h_alpha_W->Draw();
    h_alpha_W->SetLineColor(kOrange);
    h_alpha_W->GetXaxis()->SetTitle("Angle between gammas HPGe & Calo (rad)");
    h_alpha_W->GetYaxis()->SetTitle("Counts");
  }
  
  
  if(CRY_ANGLES){
    TLine *line1= new TLine (0,0,130,0);//lines in zero Theta&Phi
    TLine *line2= new TLine (0,0,130,0);
    
    //MCTrack & CaloHit & CryHit----------------------
    TCanvas* c17 = new TCanvas("Theta angle difference","Theta angle difference",0,0,720,900);
    c17->SetFillColor(0);
    c17->SetFrameFillColor(0);
    h1_CryAng->Draw("colz");
    h1_CryAng->GetXaxis()->SetTitle("CryId");
    h1_CryAng->GetYaxis()->SetTitle("Theta_MC-Theta_Calo (rad)");
    line1->Draw();
    line1->SetLineColor(kRed);
    line1->SetLineWidth(2);
    
    TCanvas* c18 = new TCanvas("Phi angle difference","Phi angle difference",0,0,720,900);
    c18->SetFillColor(0);
    c18->SetFrameFillColor(0);
    h2_CryAng->Draw("colz");
    h2_CryAng->GetXaxis()->SetTitle("CryId");
    h2_CryAng->GetYaxis()->SetTitle("Phi_MC-Phi_Calo (rad)");
    line2->Draw();
    line2->SetLineColor(kRed);
    line2->SetLineWidth(2);
    
    TCanvas* c19 = new TCanvas("Theta_MCtrack & Calo_Points for each CrystalId","Theta_MCtrack & Calo_Points for each CrystalId",0,0,720,900);
    c19->SetFillColor(0);
    c19->SetFrameFillColor(0);
    h3_CryAng->Draw("colz");
    h3_CryAng->GetXaxis()->SetTitle("CryId");
    h3_CryAng->GetYaxis()->SetTitle("Calo Theta (rad)");
    gr1->SetMarkerStyle(22);
    gr1->SetMarkerColor(kRed);
    gr1->Draw("Psame");
    //////////////////////////////////
    TLegend *leg3 = new TLegend(0.5,0.7,0.8,0.8);
    leg3->AddEntry(gr1, "Theta point of CaloHit", "p");
    leg3->SetFillColor(0);
    leg3->Draw();
    //////////////////////////////////////////////////
    
    
    TCanvas* c20 = new TCanvas("Phi_MCtrack & Calo_Points for each CrystalId","Phi_MCtrack & Calo_Points for each CrystalId",0,0,720,900);
    c20->SetFillColor(0);
    c20->SetFrameFillColor(0);
    h4_CryAng->Draw("colz");
    h4_CryAng->GetXaxis()->SetTitle("CryId");
    h4_CryAng->GetYaxis()->SetTitle("Calo Phi (rad)");
    gr2->SetMarkerStyle(22);
    gr2->SetMarkerColor(kRed);
    gr2->Draw("Psame");
    //////////////////////////////////
    TLegend *leg4 = new TLegend(0.2,0.7,0.5,0.8);
    leg4->AddEntry(gr2, "Phi point of CaloHit", "p");
    leg4->SetFillColor(0);
    leg4->Draw();
    //////////////////////////////////////////////////
    
    TCanvas* c21 = new TCanvas("Theta vs Phi for each CrystalId","Theta vs Phi for each CrystalId",0,0,720,900);
    c21->SetFillColor(0);
    c21->SetFrameFillColor(0);
    h5_CryAng->Draw("colz");
    h5_CryAng->GetXaxis()->SetTitle("Calo Theta (rad)");
    h5_CryAng->GetYaxis()->SetTitle("Calo Phi (rad)"); 
  }  
}
