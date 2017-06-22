

void checkResults_bothdet_motherID() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
	char inputFile[250] = "outsim.root";                                              
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION-----------------------------------------------------------//Change this maximum energies
    TH1F* h1   = new TH1F("h1","Primary PDG Code",60,-30,30);
    TH1F* h1_2 = new TH1F("h1_2","Secondaries PDG Code",60,-30,30);
    TH1F* h2   = new TH1F("h2","Primary Energy",1100,0,11);
	TH1F* h3   = new TH1F("h3","HPGe Energy",1000,0,1.2);
	TH1F* h4   = new TH1F("h4","Theta",200,-4,4);
    TH1F* hx   = new TH1F("hx","X de impacto",600,-6,6);
	TH2F* hxy  = new TH2F("hxy","Collides 2D", 200,-4,4,200,-4,4);
	TH3F* hxyz = new TH3F("hxyz","HPGe Collides 3D", 80,-4,4,80,-4,4,80,-10,-18);
	
	TH1F* h1_Cry = new TH1F("h1_Cry","Crystal ID",150,0,150);
	TH1F* h2_Cry = new TH1F("h2_Cry","Crystal Energy (MeV)",200,0,10.2);
	
	TH1F *hmult = new TH1F("hmult","hmult",5,0,5);
	

	//----   MCTrack (input)   -------------------------------------------------------
	TClonesArray* MCTrackCA;
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);
	
	//----- HPGe detector--------------//

	//Crystal Points  
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
		
		//LOOP in MC mother tracks----------------------------------------------
		Int_t mult=0;
		for(Int_t h=0;h<MCtracksPerEvent;h++){ if(track[h]->GetMotherId()<0) mult++; }
		hmult->Fill(mult);
		
		//LOOP in MC mother tracks------------------------------------------------
                for(Int_t h=0;h<MCtracksPerEvent;h++) {

                  if(track[h]->GetMotherId()<0) {    //Primary Particle is MotherId=-1
                    //LOOP in HPGe points
                    for(Int_t r=0;r<hpgePointsPerEvent;r++) {          //LOOP in HPGePoints for each MCTrack

				    hpgePoint[r]->PositionIn(vector3);
			        h4->Fill(vector3.Phi());    
				    X1=hpgePoint[r]->GetXIn();
				    hx->Fill(X1);
                                    X2=hpgePoint[r]->GetYIn();
                                    X3=hpgePoint[r]->GetZIn();
                                    hx->Fill(X1);
                                    hxy->Fill(X1,X2);
                                    hxyz->Fill(X1,X2,X3);
                    }
                    
            		//LOOP in HPGe Hits
		    		for(Int_t h=0;h<hpgeHitsPerEvent;h++){            //LOOP in HPGeHits for each MCTrack
						charge = hpgeHit[h]->GetEnergy()*1000;
						h3->Fill(charge);
		   			}
                    
                    //LOOP in crystal Hits
		    		for(Int_t h=0;h<crystalHitsPerEvent;h++){          //LOOP in crystalHits for each MCTrack
						h1_Cry->Fill(crystalHit[h]->GetCrystalId());
						h2_Cry->Fill(crystalHit[h]->GetEnergy()*1000);
		    		}
                           
                 }
              }
		

		if(MCtracksPerEvent)     delete[] track;
		if(hpgeHitsPerEvent)     delete[] hpgeHit;
		if(hpgePointsPerEvent)   delete[] hpgePoint;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		if(caloHitsPerEvent)     delete[] caloHit;
		if(crystalPointPerEvent) delete[] crystalPoint;

	}
	// END LOOP IN THE EVENTS---------------------------------------------------------


	// HISTOGRAMS--------------------------------------------------------------------- 
	
	//HPGe-------------
	/*TCanvas* c1 = new TCanvas("MCTrack","MCTrack",0,0,400,800);
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
	c1->cd(2);                  
	h2->Draw();
	h2->SetLineColor(9);        
	h2->GetXaxis()->SetTitle("Energy (MeV/c^{2})");
	h2->GetYaxis()->SetTitle("Counts");*/

	TCanvas* c2 = new TCanvas("c2","Energy in the HPGe",0,0,400,400);
	c2->SetFillColor(0);
	c2->SetFrameFillColor(0);
	Int_t ci;// for color index setting
   	ci = TColor::GetColor("#000099");
   	h3->SetLineColor(ci);
   	h3->SetLineWidth(2);
   	h3->GetXaxis()->SetTitle("Energy (MeV/c^{2})");
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
   	
    TCanvas* c5 = new TCanvas("Region2D","Region de impactos 2D",0,0,400,800);
    c5->SetFillColor(0);
    c5->SetFrameFillColor(0);
	//hxy-> Draw("colz");  
	//hxy-> Draw("lego2z");//draw a Lego 2D with a palete of colours at right side
	hxy-> Draw("surf2z");  //draw a surface with a palete of colours at right side
	hxy->GetXaxis()->SetTitle("x (cm)");
	hxy->GetYaxis()->SetTitle("y (cm)");

	TCanvas* c6 = new TCanvas("theta","Angulo theta",0,0,400,400);
	c6->SetFillColor(0);
    c6->SetFrameFillColor(0);
	h4->Draw("");
	h4->SetLineColor(6);
	h4->GetXaxis()->SetTitle("#theta (rad)");
	h4->GetYaxis()->SetTitle("Counts");
		
	TCanvas* c7 = new TCanvas("Region3D","Region de impactos 3D",0,0,400,800);
    c7->SetFillColor(0);
    c7->SetFrameFillColor(0);
	hxyz-> Draw("");
	hxyz->SetMarkerStyle(20); 
	hxyz->SetMarkerSize(0.4); 
	hxyz->SetMarkerColor(9); 
	hxyz->GetXaxis()->SetTitle("x (cm)");
	hxyz->GetYaxis()->SetTitle("y (cm)");
	hxyz->GetZaxis()->SetTitle("z (cm)");

	//CALIFA ----------------------
	//CrystalHit
	TCanvas* c8 = new TCanvas("crystalHit","crystalHit",0,0,720,900);
	c8->SetFillColor(0);
	c8->SetFrameFillColor(0);
	c8->cd();
	c8->Divide(2,2);
	
	c8->cd(1); 
	h1_Cry->Draw();
	h1_Cry->SetLineColor(6);
	h1_Cry->GetXaxis()->SetTitle("Crystal ID");
	h1_Cry->GetYaxis()->SetTitle("Counts");
	
	c8->cd(2); 
	h2_Cry->Draw(); c8->cd(2)->SetLogy();
	h2_Cry->SetLineColor(9);
	h2_Cry->GetXaxis()->SetTitle("Energy (MeV/c^{2})");
	h2_Cry->GetYaxis()->SetTitle("Counts");
	
	

}

