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

void openRoots_Actibackground() {


  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  

	//CHAIN INPUT
	TChain *fChain = new TChain("ensartree");
	fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/earth_Roots_ActiniumChain/outsim*.root",0);


	//HISTOGRAMS def
  //TH1F* hHPGe_energy   = new TH1F("hHPGe_energy ","HPGe Energy",6000,0,3000);
	TH1F* hHPGe_energy   = new TH1F("hHPGe_energy ","HPGe Energy",8192, -1.466902, 3328.528038);
 
  //HPGe (input)   -------------------------------------------------------
  //HPGe Hit
  TClonesArray* hpgeHitCA;
  EnsarHPGeDetHit** hpgeHit;
  hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
  //TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
  TBranch *branchEnsarHPGeDetHit = fChain ->GetBranch("HPGeDetHit");
  branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );
  fChain->SetBranchAddress("HPGeDetHit",&hpgeHitCA);//add this line for fChain only
	

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
 
    hpgeHitsPerEvent    = hpgeHitCA->GetEntries();

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
			energySmearing=gRandom->Gaus(energy, 0.0008*energy);
      hHPGe_energy ->Fill(energySmearing, 1/0.9216);//weight from UraniumChain and 0.046 from activity diff for 7millions
																									//so, the total weight will be 0.03565 for 5millions
			//hHPGe_energy ->Fill(energy);
    }
    

  }
  // END LOOP IN THE EVENTS---------------------------------------------------------


	//Open a file

	TFile *MyFile = new TFile("Actiniumchain_earth12p2_8192bins_Res0p0008_Uraweight_last.root","NEW");
	if ( MyFile->IsOpen() ) printf("File opened successfully\n");
	
	hHPGe_energy ->Write();
	MyFile->Close(); 
}
