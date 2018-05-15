////////////////////////////////////////////////////////////////////////////
////																	
////		--- Simulation of Cherry Tomato ---				
////																	
////			Macro to check the Results of the Cherry					
////																	
////		Usage:														
////			1st: select the root file & the ranges (OPTIONAL)		
////			2nd: root -l checkResults_Cherry.C								
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

void checkResults_7Cherry_tomatoes() {


  //ROOT ENVIRONMENT
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);
  
  //INPUT FILE
  char inputFile[250] = "outsim.root";   //select root file                                             
  TFile *file1 = TFile::Open(inputFile);
  
  //READING TREE
  TTree* tree = (TTree*)file1->Get("ensartree");
  
  //HISTOGRAMS DEFINITION-----------------------------------------------------------//Change these ranges
  TH1F* h1   = new TH1F("h1","Primary PDG Code",60,-30,30);
  TH1F* h1_2 = new TH1F("h1_2","Secondaries PDG Code",60,-30,30);
  TH1F* h2   = new TH1F("h2","Primary Energy",1100,0,11);
	TH1F* h2_2 = new TH1F("h2_2","Gamma Energy",1100,0,11);
  TH1F* h3   = new TH1F("h3","HPGe Energy",1000,0,11);
  TH1F* h4   = new TH1F("h4","Theta",400,-4,4);
  TH1F* hx   = new TH1F("hx","X de impacto",600,-6,6);
 	TH1F *hxdif = new TH1F("hxdiff","hxdiff",500,-100,100);
  TH1F *hydif = new TH1F("hydiff","hydiff",500,-100,100);
  
	TH2F* hxy  = new TH2F("hxy","Front View", 200,-14,14,200,-14,14);
	TH2F* hzx  = new TH2F("hzx","Depth Profiling", 200,-20,-28,200,-14,14);
	TH2F* hz_energy  = new TH2F("hz_energy","Energy Profiling in z axis", 200,-20,-28,1100,0,11);
	TH2F* hx_energy  = new TH2F("hx_energy","Energy Profiling in x axis", 200,-14,14,1100,0,11);
  
	TH3F* hxyz = new TH3F("hxyz","Collides 3D", 80,-4,4,80,-4,4,80,-10,-18);  
	TH3F* hxyz_energy = new TH3F("hxyz_energy","Collides 3D and Energy", 80,-14,14,80,-14,14,80,-20,-28);
  
  //----   MCTrack (input)   -------------------------------------------------------
  TClonesArray* MCTrackCA;
  EnsarMCTrack** track;
  MCTrackCA = new TClonesArray("EnsarMCTrack",5);
  TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
  branchMCTrack->SetAddress(&MCTrackCA);
  
  //HPGe (input)   -------------------------------------------------------
  //HPGe Hit
  TClonesArray* hpgeHitCA;
  EnsarHPGeDetHit** hpgeHit;
  hpgeHitCA = new TClonesArray("EnsarHPGeDetHit",5);
  TBranch *branchEnsarHPGeDetHit = tree ->GetBranch("HPGeDetHit");
  branchEnsarHPGeDetHit->SetAddress(&hpgeHitCA );
  
  //HPGe Point
  TClonesArray* hpgePointCA;
  EnsarHPGeDetPoint** hpgePoint;
  hpgePointCA = new TClonesArray("EnsarHPGeDetPoint",5);
  TBranch *branchEnsarHPGeDetPoint = tree ->GetBranch("HPGeDetPoint");
  branchEnsarHPGeDetPoint->SetAddress(&hpgePointCA );
	
  Int_t MCtracksPerEvent = 0;
  Int_t hpgeHitsPerEvent = 0;
  Int_t hpgePointsPerEvent = 0;
  Double_t charge = 0.0;
  
  TVector3 momentum, vector3;
  Float_t X1=0.,X2=0.,X3=0.,X4=0.,Xup=0.,Xdown=0.;
  
  

  //TREE ENTRIES--------------------------------------------------------------------
  Long64_t nevents = tree->GetEntries();
  
  //LOOP IN THE EVENTS--------------------------------------------------------------
  for(int i=0;i<nevents;i++){
    if(i%1== 100) printf("Event:%i\n",i);
    charge = 0.;
    tree->GetEvent(i);
    
    MCtracksPerEvent    = MCTrackCA->GetEntries();
    hpgeHitsPerEvent    = hpgeHitCA->GetEntries();
    hpgePointsPerEvent  = hpgePointCA->GetEntries();
    
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
    
    //LOOP in MC mother tracks----------------------------------------------
    for(Int_t h=0;h<MCtracksPerEvent;h++){
      if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
				h1->Fill(track[h]->GetPdgCode());
				h2->Fill(track[h]->GetEnergy()*1000);//MeV
      } else {
				h1_2->Fill(track[h]->GetPdgCode());
							if (track[h]->GetPdgCode()==22){
								h2_2->Fill(track[h]->GetEnergy()*1000);//MeV
							}
      }
    }
    
    //LOOP in hpgeHits-------------------------------------------------------
    for(Int_t h=0;h<hpgeHitsPerEvent;h++){
      charge = hpgeHit[h]->GetEnergy()*1000;
    
      h3->Fill(charge);
    }
    
    //LOOP in MC mother tracks------------------------------------------------
    for(Int_t h=0;h<MCtracksPerEvent;h++) {
      
      if(track[h]->GetMotherId()<0) {    //Primary Particle is MotherId=-1

	//track[h]->GetMomentum(momentum);
	
				for(Int_t r=0;r<hpgePointsPerEvent;r++) {     //LOOP in hpgeHits for each MCTrack

					//hxdif->Fill(hpgePoint[r]->GetXIn());
					//hydif->Fill(hpgePoint[r]->GetYIn());
					hpgePoint[r]->PositionIn(vector3);
					//cout << " ---- " << vector3.Theta() << endl ;
					h4->Fill(vector3.Phi());    
					X1=hpgePoint[r]->GetXIn();
					hx->Fill(X1);
					X2=hpgePoint[r]->GetYIn();
					X3=hpgePoint[r]->GetZIn();
					hx->Fill(X1);
					hxy->Fill(X1,X2);
					hzx->Fill(X3,X1);
					hxyz->Fill(X1,X2,X3);
					hxyz_energy->Fill(X1,X2,X3,hpgePoint[r]->GetEnergyLoss());
					hz_energy->Fill(X3,hpgePoint[r]->GetEnergyLoss()*1000);//MeV
					hx_energy->Fill(X1,hpgePoint[r]->GetEnergyLoss()*1000);//MeV
				}
      }
    }

  }
  // END LOOP IN THE EVENTS---------------------------------------------------------

  TCanvas* c1 = new TCanvas("MCTrack","MCTrack",0,0,400,800);	
  c1->Divide(1,2);           
  c1->SetFillColor(0);      
  c1->SetFrameFillColor(0);
  c1->cd(1);                
  h1_2->Draw();
  h1_2->GetXaxis()->SetTitle("PDG Code");
  h1_2->GetYaxis()->SetTitle("Counts");
  h1->Draw("same");
  h1->SetLineColor(2);        
  h1_2->SetLineColor(8);   
	//////////////////////////////////
  TLegend *leg1 = new TLegend(0.2,0.7,0.5,0.8);
  leg1->AddEntry(h1_2, "PDG Code Secondary Particles", "pl");
	leg1->AddEntry(h1, "PDG Code Primary Particles", "pl");
  leg1->SetFillColor(0);
  leg1->Draw();
  //////////////////////////////////////////////////   
  c1->cd(2);                
  h2->Draw();
  h2->SetLineColor(9);      
  h2->GetXaxis()->SetTitle("Energy of primary particles (MeV)");
  h2->GetYaxis()->SetTitle("Counts");

 
  TCanvas* c2 = new TCanvas("c2","Cherry Energy",0,0,400,400);                     
  c2->SetFillColor(0);
  c2->SetFrameFillColor(0);
  h2_2->GetXaxis()->SetTitle("Energy (MeV)");
  h2_2->GetYaxis()->SetTitle("Counts");
	h2_2->Draw();c2->SetLogy();
	h2_2->SetLineColor(5);
	h3->Draw("same"); 
	h3->SetLineColor(9);
	//////////////////////////////////
  TLegend *leg2 = new TLegend(0.3,0.8,0.6,0.9);
  leg2->AddEntry(h3, "Electron Energy", "pl");
	leg2->AddEntry(h2_2, "Gamma Energy", "pl");
  leg2->SetFillColor(0);
  leg2->Draw();
  //////////////////////////////////////////////////   


/*
TCanvas* c2 = new TCanvas("c2","Cherry Energy",0,0,400,400);                     
  c2->SetFillColor(0);
  c2->SetFrameFillColor(0);
  Int_t ci;                    // for color index setting
  ci = TColor::GetColor("#000099");
  h3->SetLineColor(ci);
  h3->SetLineWidth(2);
  h3->GetXaxis()->SetTitle("Energy (MeV)");
  h3->GetXaxis()->SetLabelSize(0.035);
  h3->GetXaxis()->SetTitleSize(0.035);
  h3->GetYaxis()->SetTitle("Counts");
  h3->GetYaxis()->SetLabelSize(0.035);
  h3->GetYaxis()->SetTitleSize(0.035);
  h3->GetZaxis()->SetLabelFont(42);
  h3->GetZaxis()->SetLabelSize(0.035);
  h3->GetZaxis()->SetTitleSize(0.035);
  h3->GetZaxis()->SetTitleFont(42);
  h3->Draw(""); 
	h2_2->Draw("same");   
*/     
  
  TCanvas* c5 = new TCanvas("Collides_xy","Collides: Front View",0,0,400,800);
  c5->SetFillColor(0);
  c5->SetFrameFillColor(0);
  //hxy-> Draw("colz");            
  hxy-> Draw("lego2z"); 		
  //hxy-> Draw("surf2z");
  hxy->GetXaxis()->SetTitle("x (cm)");
  hxy->GetYaxis()->SetTitle("y (cm)");
  

  TCanvas* c6 = new TCanvas("theta","Theta Angle",0,0,400,400);
  c6->SetFillColor(0);
  c6->SetFrameFillColor(0);
  h4->Draw("");
  h4->SetLineColor(6);          
  h4->GetXaxis()->SetTitle("#theta (rad)");
  h4->GetYaxis()->SetTitle("Counts");
  
  TCanvas* c7 = new TCanvas("Collides_xyz","Collides 3D",0,0,400,800);
  c7->SetFillColor(0);
  c7->SetFrameFillColor(0);
  hxyz-> Draw("");
  hxyz->SetMarkerStyle(20); 
  hxyz->SetMarkerSize(0.4); 
  hxyz->SetMarkerColor(2); 
  hxyz->GetXaxis()->SetTitle("x (cm)");
  hxyz->GetYaxis()->SetTitle("y (cm)");
  hxyz->GetZaxis()->SetTitle("z (cm)");

	TCanvas* c8 = new TCanvas("Colides_zx","Collides: Depth Profiling",0,0,400,800);
  c8->SetFillColor(0);
  c8->SetFrameFillColor(0);
  //hzx-> Draw("colz");            
  hzx-> Draw("lego2z"); 		
  //hzx-> Draw("surf2z");
  hzx->GetXaxis()->SetTitle("z (cm)");
  hzx->GetYaxis()->SetTitle("x (cm)");

	TCanvas* c9 = new TCanvas("Colides_xyz_energy","Collides 3D and Energy",0,0,400,800);
  c9->SetFillColor(0);
  c9->SetFrameFillColor(0);
  //hzx-> Draw("colz");            
  hxyz_energy-> Draw("LEGO2"); 		
  //hzx-> Draw("surf2z");
  hxyz_energy->GetXaxis()->SetTitle("x (cm)");
  hxyz_energy->GetYaxis()->SetTitle("y (cm)");
	hxyz_energy->GetZaxis()->SetTitle("z (cm)");
	
	TCanvas* c10 = new TCanvas("Energy_Profiling_z","Energy Profiling in z axis",0,0,400,800);
  c10->SetFillColor(0);
  c10->SetFrameFillColor(0);
  //hzx-> Draw("colz");            
  hz_energy-> Draw("colz"); 		
  //hzx-> Draw("surf2z");
  hz_energy->GetXaxis()->SetTitle("z (cm)");
  hz_energy->GetYaxis()->SetTitle("Energy (Mev)");

	TCanvas* c11 = new TCanvas("Energy_Profiling_x","Energy Profiling in x axis",0,0,400,800);
  c11->SetFillColor(0);
  c11->SetFrameFillColor(0);
  //hzx-> Draw("colz");            
  hx_energy-> Draw("colz"); 		
  //hzx-> Draw("surf2z");
  hx_energy->GetXaxis()->SetTitle("x (cm)");
  hx_energy->GetYaxis()->SetTitle("Energy (Mev)");
  
}
