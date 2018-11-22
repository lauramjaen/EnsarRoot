////////////////////////////////////////////////////////////////////////////
////																	
////		--- Natural Radioactivity Background ---				
////																	
////			Read the HPGe data, generate an Energy histo and save it					
////																	
////																	
////																	
//// **elisabet.galiana@usc.es											
//// ** Universidad de Santiago de Compostela							
//// ** Faculdade de Ciencias da Universidade de Lisboa 										
////////////////////////////////////////////////////////////////////////////

//NOTE: if you want to analyze the HPGePoint 
//      you have to activate it before to execute runsim.C,
//	in order to create its branch
//
//	How to activate it: comment/descomment this line
//		HPGe: ctn/detector/EnsarHPGeDet.cxx 
//				->FairRootManager::Instance()->Register("HPGeDetPoint", GetName(), fPointCollection, kTRUE);
//	Then, you have to do "make" again in the EnsarRoot build directory and execute runsim.C  

void openRoots_Uraniumbackground() {


  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  

	//INPUT FILE
	//char inputFile[250] = "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim_214Bi.root";
	//TFile *file1 = TFile::Open(inputFile); 
	//TTree* tree = (TTree*)file1->Get("ensartree");

	//CHAIN INPUT
	TChain *fChain = new TChain("ensartree");
fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim*.root",0); 
//fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim_214Bi.root",0);
//fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim_226Ra.root",0); 
//fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim_234Th.root",0); 
//fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim_234Pa.root",0); 
//fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim_214Pb.root",0);  


/*Pam:
char if_root[150]; 
sprintf(if_root,"/lstore/nucria/pteubig/CALIFA2016/root_files/run%d/run%d_f*.root", run, run); 
fChain->Add(if_root,0);
*/

	//HISTOGRAMS def
  TH1F* hHPGe_energy   = new TH1F("hHPGe_energy ","HPGe Energy",6000,0,3000);
  TH1F* hMCTrack_energy   = new TH1F("hMCTrack_energy","MCTrack Energy",3000,0,3000);

  
  //----   MCTrack (input)   -------------------------------------------------------
  TClonesArray* MCTrackCA;
  EnsarMCTrack** track;
  MCTrackCA = new TClonesArray("EnsarMCTrack",5);
  //TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	TBranch *branchMCTrack = fChain ->GetBranch("MCTrack");
  branchMCTrack->SetAddress(&MCTrackCA);
  fChain->SetBranchAddress("MCTrack",&MCTrackCA);//add this line for fChain only
  
  //HPGe (input)   -------------------------------------------------------
  //HPGe Hit
  TClonesArray* hpgeHitCA;
  EnsarHPGeDetHit** hpgeHit;
  hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
  //TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
  TBranch *branchEnsarHPGeDetHit = fChain ->GetBranch("HPGeDetHit");
  branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );
  fChain->SetBranchAddress("HPGeDetHit",&hpgeHitCA);//add this line for fChain only
	

  Int_t MCtracksPerEvent = 0;
  Int_t hpgeHitsPerEvent = 0;

  Double_t energy = 0.0;
  Double_t energySmearing = 0.0;

  //TREE ENTRIES--------------------------------------------------------------------
  //Long64_t nevents = tree->GetEntries();
 	//Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nevents = fChain->GetEntries();
cout << "1.Entries: "<<nevents << endl;

  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    energy = 0.;
    energySmearing = 0.;

//hpgeHitCA->Clear(); provar

    //tree->GetEvent(i);
		fChain->GetEvent(i);

  
    MCtracksPerEvent    = MCTrackCA->GetEntries();
    hpgeHitsPerEvent    = hpgeHitCA->GetEntries();

    
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



/*

// -----   Private method NUSmearing  --------------------------------------------
Double_t R3BCalifaPoint2CrystalCal::NUSmearing(Double_t inputEnergy) {
  // Very simple preliminary scheme where the NU is introduced as a flat random
  // distribution with limits fNonUniformity (%) of the energy value.
  //
  return gRandom->Uniform(inputEnergy - inputEnergy * fNonUniformity / 100,
			  inputEnergy + inputEnergy * fNonUniformity / 100);
}
*/


//Ask Dani about the Smearing?
//Double_t NonUniformity;
//Double_t inputEnergy;
//Double_t energy;

//NonUniformity=3.;
//inputEnergy=5.;

//TRandom* random= new TRandom();
//energy=random->Uniform (inputEnergy - inputEnergy * NonUniformity / 100,
//			  inputEnergy + inputEnergy * NonUniformity / 100);
//cout<<"Smearing energy="<<energy<<endl;

    //LOOP in hpgeHits-------------------------------------------------------
    for(Int_t h=0;h<hpgeHitsPerEvent;h++){     
			energy = hpgeHit[h]->GetEnergy()*1000000;//keV
			energySmearing=gRandom->Gaus(energy, 0.002*energy);
      hHPGe_energy ->Fill(energySmearing,1/4.98667);//weight of 4.98
			//cout<<"hpgeHits h="<<h<<endl;
    }
    
    //LOOP in MC mother tracks------------------------------------------------
    for(Int_t h=0;h<MCtracksPerEvent;h++) {
			hMCTrack_energy->Fill(track[h]->GetEnergy()*1000000);//keV
    }

  }
  // END LOOP IN THE EVENTS---------------------------------------------------------


	//Open a file

	TFile *MyFile = new TFile("238Uchain_background.root","NEW");
	if ( MyFile->IsOpen() ) printf("File opened successfully\n");
	
	hHPGe_energy ->Write();
	hMCTrack_energy->Write();
	MyFile->Close(); 
}
