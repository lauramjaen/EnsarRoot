////////////////////////////////////////////////////////////////////////////////////
//// ** elisabet.galiana@usc.es
//// ** Universidad de Santiago de Compostela
//// ** Dpto. Física de Partículas
////
////	--- Simulation of the Lisbon Nov-2016 setup ---
////


using namespace std;

void checkResults_Co_gammaHPGe() {
  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  
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
  Double_t maxE = 0.4;
  
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
  TH1F *h_Ge;
  
  //HISTOGRAMS CREATION ------------------------------------------------------------  Change these ranges and bins (OPTIONAL)
 
    /*TString Name1, Name2;

    TH1F* h_Califa_crystals [2][64];

		for (Int_t i=0; i<2; i++){
			for (Int_t j=0; j<64; j++){
			
					sprintf(Name1, "h_Califa_Petal_%d_Crystal_%d_energy", i+1, j+1);
					sprintf(Name2, "Energy of Petal %d Crystal %d", i+1, j+1);
					h_Califa_crystals[i][j]= new TH1F("h_Califa_crystals","Energy ",1000,0,maxE);
			}		
		}*/

		char Name1[250];
		char Name2[250];
		TH1F* h_Califa_crystals [128];
		
		for (Int_t j=0; j<128; j++){
			
					sprintf(Name1, "h_Califa_Crystal_%d_energy", j+1);
					sprintf(Name2, "Energy of Crystal %d",  j+1);
					h_Califa_crystals[j]= new TH1F(Name1,Name2,1000,0,maxE);
			}

    h_Ge    = new TH1F("h_Ge","HPGe Energy",1000,0,maxE);
    
    
 
 
  
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
         
      //LOOP in HPGe Hits ----
      for(Int_t h=0;h<hpgeHitsPerEvent;h++){
				energy = hpgeHit[h]->GetEnergy()*1000;//MeV
				h_Ge->Fill(energy);
      }
      
      //LOOP in Crystal Hits ----
      for(Int_t h=0;h<crystalHitsPerEvent;h++){
				//h1_Cry->Fill(crystalHit[h]->GetCrystalId());
				//h2_Cry->Fill(crystalHit[h]->GetEnergy()*1000);//MeV
				//h3_Cry->Fill(crystalHit[h]->GetCrystalType());
				//h4_Cry->Fill(crystalHit[h]->GetCrystalCopy());

				h_Califa_crystals[crystalHit[h]->GetCrystalId()-1]->Fill(crystalHit[h]->GetEnergy()*1000);//MeV
      }
      
     
    
    if(MCtracksPerEvent)     delete[] track;
    if(hpgeHitsPerEvent)     delete[] hpgeHit;
    if(hpgePointsPerEvent)   delete[] hpgePoint;
    if(crystalHitsPerEvent)  delete[] crystalHit;
    if(caloHitsPerEvent)     delete[] caloHit;
    if(crystalPointPerEvent) delete[] crystalPoint;
    
  }
  // END LOOP IN THE EVENTS----------------------------------------------------------------------------------------------------
  
 
  
  // HISTOGRAMS---------------------------------------------------------------------
  
		for (Int_t i=0; i<128; i++){
			Int_t entries;
			Int_t integral;
			entries=h_Califa_crystals[i]->GetEntries();
			integral=h_Califa_crystals[i]->Integral (495, 506);
			//cout<<"Crystal "<<i+1<<" Entries "<<entries<<" Integral "<<integral<<endl;
			cout<<i+1<<"  "<<integral<<endl;
		}
    
    //HPGe-------------
    TCanvas* c1 = new TCanvas("Energy in the HPGe","Energy in the HPGe",0,0,400,400);
    c1->SetFillColor(0);
    c1->SetFrameFillColor(0);
    h_Ge->SetLineColor(9);
    h_Ge->SetLineWidth(2);
    h_Ge->GetXaxis()->SetTitle("HPGe Energy (MeV)");
    h_Ge->GetYaxis()->SetTitle("Counts");
    h_Ge->Draw();

		//for (i=0; i<64; i++){
		Int_t i;
		i=0;
		//Crystals Califa ------
 		TCanvas* c2 = new TCanvas("Energy_Crystals_1","Energy Crystals 1",0,0,400,400);
    c2->SetFillColor(0);
    c2->SetFrameFillColor(0);
		c2->Divide(2,2);
		c2->cd(1);
		h_Califa_crystals[i]->Draw();
		c2->cd(2);
		h_Califa_crystals[i+1]->Draw();
		c2->cd(3);
		h_Califa_crystals[i+2]->Draw();
		c2->cd(4);
		h_Califa_crystals[i+3]->Draw();

		TCanvas* c3 = new TCanvas("Energy_Crystals_2","Energy Crystals 2",0,0,400,400);
    c3->SetFillColor(0);
    c3->SetFrameFillColor(0);
		c3->Divide(2,2);
		c3->cd(1);
		h_Califa_crystals[i+4]->Draw();
		c3->cd(2);
		h_Califa_crystals[i+5]->Draw();
		c3->cd(3);
		h_Califa_crystals[i+6]->Draw();
		c3->cd(4);
		h_Califa_crystals[i+7]->Draw();

 		TCanvas* c4 = new TCanvas("Energy_Crystals_3","Energy Crystals 3",0,0,400,400);
    c4->SetFillColor(0);
    c4->SetFrameFillColor(0);
		c4->Divide(2,2);
		c4->cd(1);
		h_Califa_crystals[i+8]->Draw();
		c4->cd(2);
		h_Califa_crystals[i+9]->Draw();
		c4->cd(3);
		h_Califa_crystals[i+10]->Draw();
		c4->cd(4);
		h_Califa_crystals[i+11]->Draw();

 		TCanvas* c5 = new TCanvas("Energy_Crystals_4","Energy Crystals 4",0,0,400,400);
    c5->SetFillColor(0);
    c5->SetFrameFillColor(0);
		c5->Divide(2,2);
		c5->cd(1);
		h_Califa_crystals[i+12]->Draw();
		c5->cd(2);
		h_Califa_crystals[i+13]->Draw();
		c5->cd(3);
		h_Califa_crystals[i+14]->Draw();
		c5->cd(4);
		h_Califa_crystals[i+15]->Draw();

}
