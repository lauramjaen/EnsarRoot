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

void openRoots_Actiniumbackground() {


  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  

//ACTINIUM chain: /Roots_ThoriumChain/ outsim_235U.root, 231Th, 231Pa, 227Th, 223Fr, 223Ra, 219Rn, 211Pb, 211Bi

	//INPUT FILE
	//char inputFile[250] = "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_UraniumChain/outsim_214Bi.root";
	//TFile *file1 = TFile::Open(inputFile); 
	//TTree* tree = (TTree*)file1->Get("ensartree");

	//CHAIN INPUT
	TChain *fChain = new TChain("ensartree");
	fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Roots_ActiniumChain/outsim*.root",0); 


	//HISTOGRAMS def
  TH1F* hHPGe_energy_Acti   = new TH1F("hHPGe_energy_Acti ","HPGe Thorium Chain Energy",6000,0,3000);
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
  Long64_t nevents = fChain->GetEntries();

  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    energy = 0.;
    energySmearing = 0.;
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


    //LOOP in hpgeHits-------------------------------------------------------
    for(Int_t h=0;h<hpgeHitsPerEvent;h++){     
			energy = hpgeHit[h]->GetEnergy()*1000000;//keV
			energySmearing=gRandom->Gaus(energy, 0.002*energy);
			hHPGe_energy_Acti ->Fill(energy);//no weight
      //hHPGe_energy_Acti ->Fill(energySmearing,1/0.6197);//weight 
			//cout<<"hpgeHits h="<<h<<endl;
    }
    
    //LOOP in MC mother tracks------------------------------------------------
    for(Int_t h=0;h<MCtracksPerEvent;h++) {
			hMCTrack_energy->Fill(track[h]->GetEnergy()*1000000);//keV
    }

  }
  // END LOOP IN THE EVENTS---------------------------------------------------------


	//Open a file

	TFile *MyFile = new TFile("Actiniumchain_background.root","NEW");
	if ( MyFile->IsOpen() ) printf("File opened successfully\n");
	
	hHPGe_energy_Acti ->Write();
	hMCTrack_energy->Write();
	MyFile->Close(); 
}
