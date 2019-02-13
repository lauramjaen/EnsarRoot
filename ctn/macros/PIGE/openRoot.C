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

void openRoot() {


  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  

	//INPUT FILE
	//URANIUM chain:  outsim_214Bi.root, 226Ra, 234Th, 234Pa, 214Pb
	//THORIUM chain:  outsim_224Ra.root, 212Pb, 212Bi, 228Ac, 208Tl


char inputFile[250] = "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/earth_Roots_ThoriumChain/Thorium_12p20_last_moreEvents/outsim_224Ra.root";
//	char inputFile[250] = "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/earth_Roots_UraniumChain/Uranium_12p20_last_moreEvents/outsim_214Bi.root";
	

	TFile *file1 = TFile::Open(inputFile); 
	TTree* tree = (TTree*)file1->Get("ensartree");


	//HISTOGRAMS def
	TH1F* hHPGe_energy   = new TH1F("hHPGe_energy ","HPGe Energy",8192,-1.466902,3328.528038);
  //TH1F* hHPGe_energy   = new TH1F("hHPGe_energy ","HPGe Energy",8192,0.003424,3330.676847);
  //TH1F* hMCTrack_energy   = new TH1F("hMCTrack_energy","MCTrack Energy",3000,0,3000);

  
  //----   MCTrack (input)   -------------------------------------------------------
  /*TClonesArray* MCTrackCA;
  EnsarMCTrack** track;
  MCTrackCA = new TClonesArray("EnsarMCTrack",5);
  TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
  branchMCTrack->SetAddress(&MCTrackCA);*/
  
  
  //HPGe (input)   -------------------------------------------------------
  //HPGe Hit
  TClonesArray* hpgeHitCA;
  EnsarHPGeDetHit** hpgeHit;
  hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
  TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
  branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );

  Int_t MCtracksPerEvent = 0;
  Int_t hpgeHitsPerEvent = 0;

  Double_t energy = 0.0;
  Double_t energySmearing = 0.0;

  //TREE ENTRIES--------------------------------------------------------------------
  Long64_t nevents = tree->GetEntries();
cout << "1.Entries: "<<nevents << endl;

  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    energy = 0.;
    energySmearing = 0.;

    tree->GetEvent(i);
  
    //MCtracksPerEvent    = MCTrackCA->GetEntries();
    hpgeHitsPerEvent    = hpgeHitCA->GetEntries();

    
    /*if(MCtracksPerEvent>0) {
      track = new EnsarMCTrack*[MCtracksPerEvent];
      for(Int_t j=0;j<MCtracksPerEvent;j++){
	track[j] = new EnsarMCTrack;
	track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
      }
    }*/
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
			energySmearing=gRandom->Gaus(energy, 0.0008*energy);//0.001 Resolution
      hHPGe_energy ->Fill(energySmearing,1/1.297);//weight of Uranium=1/0.9216 Thorium=1/1.297
    }
    
    //LOOP in MC mother tracks------------------------------------------------
    //for(Int_t h=0;h<MCtracksPerEvent;h++) {
		//	hMCTrack_energy->Fill(track[h]->GetEnergy()*1000000);//keV
    //}

  }
  // END LOOP IN THE EVENTS---------------------------------------------------------


	//Open a file

	//TFile *MyFile = new TFile("Histogram_THORIUM_212Bi_earth10cm_0p001Res.root","NEW");
	TFile *MyFile = new TFile("Histograms_Thorium_last/Histogram_Thorium_224Ra.root","NEW");
	if ( MyFile->IsOpen() ) printf("File opened successfully\n");
	
	hHPGe_energy ->Write();
	//hMCTrack_energy->Write();
	MyFile->Close(); 
}
