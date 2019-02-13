////////////////////////////////////////////////////////////////////////////
////																	
////		--- Natural Radioactivity Background ---				
////																	
////			Read the nai data, generate an Energy histo and save it					
////																	
////																	
////																	
//// **elisabet.galiana@usc.es											
//// ** Universidad de Santiago de Compostela							
//// ** Faculdade de Ciencias da Universidade de Lisboa 										
////////////////////////////////////////////////////////////////////////////

//NOTE: if you want to analyze the naiPoint 
//      you have to activate it before to execute runsim.C,
//	in order to create its branch
//
//	How to activate it: comment/descomment this line
//		nai: ctn/detector/EnsarnaiDet.cxx 
//				->FairRootManager::Instance()->Register("naiDetPoint", GetName(), fPointCollection, kTRUE);
//	Then, you have to do "make" again in the EnsarRoot build directory and execute runsim.C  

void openRoots_Potassiumbackground() {


  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  

	//INPUT FILE
	char inputFile[250] = "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/outsim_Potassium.root";
	//char inputFile[250] = "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/Root_Potassium/outsim_Potassium_earth12p2cm.root";
	TFile *file1 = TFile::Open(inputFile); 
	TTree* tree = (TTree*)file1->Get("ensartree");


	//HISTOGRAMS def
	//Energy Cal Pam bins=8193 emin=0.41 emax=3331.49
	//Energy cal 2 8192, -1.466902, 3328.528038
  TH1F* hnai_energy_Potassium   = new TH1F("hnai_energy_Potassium ","nai Energy",8192, -1.466902, 3328.528038);
  TH1F* hMCTrack_energy   = new TH1F("hMCTrack_energy","MCTrack Energy",8192,-1.466902,3328.528038);

  
  //----   MCTrack (input)   -------------------------------------------------------
  TClonesArray* MCTrackCA;
  EnsarMCTrack** track;
  MCTrackCA = new TClonesArray("EnsarMCTrack",5);
  TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
  branchMCTrack->SetAddress(&MCTrackCA);
  
  //nai (input)   -------------------------------------------------------
  //nai Hit
  TClonesArray* naiHitCA;
  NaIHit** naiHit;//EnsarnaiDetHit
  naiHitCA = new TClonesArray("naiHit",5);
  TBranch *branchnaiHit = tree ->GetBranch("NaIHit");
  branchnaiHit->SetAddress(&naiHitCA );

	
  Int_t MCtracksPerEvent = 0;
  Int_t naiHitsPerEvent = 0;
  Double_t energy = 0.0;
  Double_t energySmearing = 0.0;

  //TREE ENTRIES--------------------------------------------------------------------
  Long64_t nevents = tree->GetEntries();

cout<<"nevents="<<nevents<<endl;

  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    energy = 0.;
    energySmearing = 0.;
    tree->GetEvent(i);
    
    MCtracksPerEvent    = MCTrackCA->GetEntries();
    naiHitsPerEvent    = naiHitCA->GetEntries();
    
    if(MCtracksPerEvent>0) {
      track = new EnsarMCTrack*[MCtracksPerEvent];
      for(Int_t j=0;j<MCtracksPerEvent;j++){
	track[j] = new EnsarMCTrack;
	track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
      }
    }
    if(naiHitsPerEvent>0) {
      naiHit = new naiHit*[naiHitsPerEvent];
      for(Int_t j=0;j<naiHitsPerEvent;j++){
	naiHit[j] = new EnsarnaiDetHit;
	naiHit[j] = (EnsarnaiDetHit*) naiHitCA->At(j);
      }
    }
    /*if(naiPointsPerEvent>0) {
      naiPoint = new EnsarnaiDetPoint*[naiPointsPerEvent];
      for(Int_t j=0;j<naiPointsPerEvent;j++){
	naiPoint[j] = new EnsarnaiDetPoint;
	naiPoint[j] = (EnsarnaiDetPoint*) naiPointCA->At(j);
      }
		}*/

    //LOOP in naiHits-------------------------------------------------------
    for(Int_t h=0;h<naiHitsPerEvent;h++){     
			energy = naiHit[h]->GetEnergy()*1000000;//keV
			energySmearing=gRandom->Gaus(energy, 0.0008*energy);
      hnai_energy_Potassium ->Fill(energySmearing,1/1.53);//with Oldweight=1/1.53. NewWeight=1/1.5089
			//hnai_energy_Potassium ->Fill(energySmearing);//no weight
			//hnai_energy_Potassium ->Fill(energy);
    }
    
    //LOOP in MC mother tracks------------------------------------------------
    /*for(Int_t h=0;h<MCtracksPerEvent;h++) {
			hMCTrack_energy->Fill(track[h]->GetEnergy()*1000000);//keV
    }*/

  }
  // END LOOP IN THE EVENTS---------------------------------------------------------


	//Open a file

	TFile *MyFile = new TFile("Potassium_noearth_8192bins_Res0p0008_Weight1p53.root","NEW");
	if ( MyFile->IsOpen() ) printf("File opened successfully\n");
	
	hnai_energy_Potassium ->Write();
	//hMCTrack_energy->Write();
	MyFile->Close(); 
}
