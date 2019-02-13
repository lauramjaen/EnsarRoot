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

void openRoots_238U_simple() {


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
	fChain->Add("/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/outsim_238U_simple.root",0); 

/*Pam:
char if_root[150]; 
sprintf(if_root,"/lstore/nucria/pteubig/CALIFA2016/root_files/run%d/run%d_f*.root", run, run); 
fChain->Add(if_root,0);
*/

	//HISTOGRAMS def
  TH1F* hHPGe_energy_simple   = new TH1F("hHPGe_energy_simple ","HPGe Energy",1000,0,3000);
  TH1F* hMCTrack_energy_simple   = new TH1F("hMCTrack_energy_simple","MCTrack Energy",1000,0,3000);

  
  //----   MCTrack (input)   -------------------------------------------------------
  TClonesArray* MCTrackCA;
  EnsarMCTrack** track;
  MCTrackCA = new TClonesArray("EnsarMCTrack",5);
  //TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	TBranch *branchMCTrack = fChain ->GetBranch("MCTrack");
  branchMCTrack->SetAddress(&MCTrackCA);
  
  //HPGe (input)   -------------------------------------------------------
  //HPGe Hit
  TClonesArray* hpgeHitCA;
  EnsarHPGeDetHit** hpgeHit;
  hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
  //TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
  TBranch *branchEnsarHPGeDetHit = fChain ->GetBranch("HPGeDetHit");
  branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );
  
  //HPGe Point
  /*TClonesArray* hpgePointCA;
  EnsarHPGeDetPoint** hpgePoint;
  hpgePointCA = new TClonesArray("EnsarHPGeDetPoint",5);
  TBranch *branchEnsarHPGeDetPoint = tree ->GetBranch("HPGeDetPoint");
  branchEnsarHPGeDetPoint->SetAddress(&hpgePointCA );*/
	
  Int_t MCtracksPerEvent = 0;
  Int_t hpgeHitsPerEvent = 0;
  //Int_t hpgePointsPerEvent = 0;
  Double_t charge = 0.0;
  
  //TREE ENTRIES--------------------------------------------------------------------
  //Long64_t nevents = tree->GetEntries();
  Long64_t nevents = fChain->GetEntries();

  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    charge = 0.;
    //tree->GetEvent(i);
		fChain->GetEvent(i);
    
    MCtracksPerEvent    = MCTrackCA->GetEntries();
    hpgeHitsPerEvent    = hpgeHitCA->GetEntries();
    //hpgePointsPerEvent  = hpgePointCA->GetEntries();
    
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
    /*if(hpgePointsPerEvent>0) {
      hpgePoint = new EnsarHPGeDetPoint*[hpgePointsPerEvent];
      for(Int_t j=0;j<hpgePointsPerEvent;j++){
	hpgePoint[j] = new EnsarHPGeDetPoint;
	hpgePoint[j] = (EnsarHPGeDetPoint*) hpgePointCA->At(j);
      }
		}*/
    
   //cout<<"MCtracksPerEvent h="<<MCtracksPerEvent<<endl; 
 	 //cout<<"hpgeHitsPerEvent h="<<hpgeHitsPerEvent<<endl; 
    //LOOP in hpgeHits-------------------------------------------------------
    for(Int_t h=0;h<hpgeHitsPerEvent;h++){     
			charge = hpgeHit[h]->GetEnergy()*1000000;//keV
      hHPGe_energy_simple ->Fill(charge);
			//cout<<"hpgeHits h="<<h<<endl;
    }
    
    //LOOP in MC mother tracks------------------------------------------------
    for(Int_t h=0;h<MCtracksPerEvent;h++) {
			hMCTrack_energy_simple->Fill(track[h]->GetEnergy()*1000000);//keV
    }

  }
  // END LOOP IN THE EVENTS---------------------------------------------------------


	//Open a file

	TFile *MyFile = new TFile("238Uchain_simple_background.root","NEW");
	if ( MyFile->IsOpen() ) printf("File opened successfully\n");
	
	hHPGe_energy_simple ->Write();
	hMCTrack_energy_simple->Write();
	MyFile->Close(); 
}
