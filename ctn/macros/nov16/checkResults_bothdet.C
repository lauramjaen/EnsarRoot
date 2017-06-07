
void checkResults_bothdet() {


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
	TH3F* hxyz_ambos = new TH3F("hxyz_ambos","Collides 3D both detectors", 180,-20.,70., 40,-10.,10. ,200,-30.,70.);
	
	TH1F* h1_Cry = new TH1F("h1_Cry","Crystal ID",150,0,150);
	TH1F* h2_Cry = new TH1F("h2_Cry","Crystal Energy (MeV)",200,0,10.2);
	TH1F* h3_Cry = new TH1F("h3_Cry","Crystal Type",32,0,32);
	TH1F* h4_Cry = new TH1F("h4_Cry","Crystal Copy",15,0,15);
	
	TH1F* h1_Cal = new TH1F("h1_Cal","Hit Energy (MeV)",200,0,10); 
	TH1F* h3_Cal = new TH1F("h3_Cal","Hit Theta",200,-3.2,3.2);
	TH1F* h4_Cal = new TH1F("h4_Cal","Hit Phi",200,-3.6,3.6);
	
	TH2F* hxz_Point = new TH2F("hxz_Point","Petals Collides 2D (x,z)", 180,-20.,70.,200,-30.,70.);
	TH3F* hxzy_Point = new TH3F("hxzy_Point","Petals Collides 3D (x,z,y)", 180,-20.,70.,200,-30.,70., 40,-10.,10.);

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
        Int_t con=0; // con,conP son dos contadores se pueden eliminar
        Int_t conP=0;
        
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
		for(Int_t h=0;h<MCtracksPerEvent;h++){
			if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
				h1->Fill(track[h]->GetPdgCode());
				h2->Fill(track[h]->GetEnergy()*1000);//MeV
                        } else {
                          h1_2->Fill(track[h]->GetPdgCode());
                        }
		}
		
		//LOOP in MC mother tracks------------------------------------------------
                for(Int_t h=0;h<MCtracksPerEvent;h++) {

                  if(track[h]->GetMotherId()<0) {    //Primary Particle is MotherId=-1
                    //LOOP in HPGe points
                    for(Int_t r=0;r<hpgePointsPerEvent;r++) {                           //LOOP in hpgeHits for each MCTrack

				    hpgePoint[r]->PositionIn(vector3);
			            h4->Fill(vector3.Phi());    
				    X1=hpgePoint[r]->GetXIn();
				    hx->Fill(X1);
                                    X2=hpgePoint[r]->GetYIn();
                                    X3=hpgePoint[r]->GetZIn();
                                    hx->Fill(X1);
                                    hxy->Fill(X1,X2);
                                    hxyz->Fill(X1,X2,X3);
                                    hxyz_ambos->Fill(X1,X2,X3);
                    }
                  }
                }
		
		//LOOP in HPGe Hits
		for(Int_t h=0;h<hpgeHitsPerEvent;h++){
		
			charge = hpgeHit[h]->GetEnergy()*1000;
			h3->Fill(charge);
		}
		
                //LOOP in crystal Hits
		for(Int_t h=0;h<crystalHitsPerEvent;h++){
			h1_Cry->Fill(crystalHit[h]->GetCrystalId());
			h2_Cry->Fill(crystalHit[h]->GetEnergy()*1000);
			h3_Cry->Fill(crystalHit[h]->GetCrystalType());
			h4_Cry->Fill(crystalHit[h]->GetCrystalCopy());
		}
		
		//LOOP in calorimeter Hits
		for(Int_t h=0;h<caloHitsPerEvent;h++){
		
			h1_Cal->Fill(caloHit[h]->GetEnergy()*1000); //MeV
			h3_Cal->Fill(caloHit[h]->GetTheta());	    //rad
			h4_Cal->Fill(caloHit[h]->GetPhi());	    //rad
		}
		
		
		//LOOP in crystal Points
		for(Int_t h=0;h<crystalPointPerEvent;h++){
		
			Double_t X=0.; Double_t X_inv=0.;
			Double_t Y=0.; Double_t Y_inv=0.;
			Double_t Z=0.; Double_t Z_inv=0.;
			
			X=crystalPoint[h]->GetXIn();
			X_inv=-X; //x inversion
			Y=crystalPoint[h]->GetYIn();
			Y_inv=-Y;
			Z=crystalPoint[h]->GetZIn();
			Z_inv=-Z;
	
			hxz_Point->Fill(X_inv,Z);
			hxzy_Point->Fill(X_inv,Z,Y);
			hxyz_ambos->Fill(X_inv,Y,Z);
			
			
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
	c1->cd(2);                  
	h2->Draw();
	h2->SetLineColor(9);        
	h2->GetXaxis()->SetTitle("Energy (MeV/c^{2})");
	h2->GetYaxis()->SetTitle("Counts");

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
	
	c8->cd(3); h3_Cry->Draw();
	h3_Cry->SetLineColor(2);
	h3_Cry->GetXaxis()->SetTitle("Crystal Type");
	h3_Cry->GetYaxis()->SetTitle("Counts");
	
	c8->cd(4); 
	h4_Cry->Draw();
	h4_Cry->SetLineColor(8);
	h4_Cry->GetXaxis()->SetTitle("Crystal Copy");
	h4_Cry->GetYaxis()->SetTitle("Counts");
	c8->cd();
	
	//CaloHit
	TCanvas* c9 = new TCanvas("CaloHit","CaloHit",0,0,720,900);
	c9->SetFillColor(0);
	c9->SetFrameFillColor(0);
	c9->cd();
	c9->Divide(2,2);
	
	c9->cd(1); 
	h1_Cal->Draw();  c9->cd(1)->SetLogy();
	h1_Cal->SetLineColor(3);
	h1_Cal->GetXaxis()->SetTitle("Energy (MeV/c^{2})");
	h1_Cal->GetYaxis()->SetTitle("Counts");
	 
	c9->cd(2); 
	h3_Cal->Draw();
	h3_Cal->SetLineColor(9);
	h3_Cal->GetXaxis()->SetTitle("Hit Theta");
	h3_Cal->GetYaxis()->SetTitle("Counts");
	
	c9->cd(3); 
	h4_Cal->Draw();
	h4_Cal->SetLineColor(13);
	h4_Cal->GetXaxis()->SetTitle("Hit Phi");
	h4_Cal->GetYaxis()->SetTitle("Counts");
	
	//Crystal Point	
	TCanvas* c3 = new TCanvas("Position","Position x and y",0,0,400,800);
        c3->SetFillColor(0);
        c3->SetFrameFillColor(0);
	//hxz-> Draw("colz");
	hxz_Point-> Draw("lego2z");   //draw a Lego 2D with a palete of colours at right side
	//hxz-> Draw("surf2z");       //draw a surface with a palete of colours at right side
	hxz_Point->GetXaxis()->SetTitle("X (cm)");
	hxz_Point->GetYaxis()->SetTitle("Z (cm)");
	
	TCanvas* c4 = new TCanvas("Position2","Position x, z and y",0,0,400,800);
        c4->SetFillColor(0);
        c4->SetFrameFillColor(0);
        hxzy_Point->Draw("");
	hxzy_Point->SetMarkerStyle(20); 
	hxzy_Point->SetMarkerSize(0.4); 
	hxzy_Point->SetMarkerColor(9); 
	hxzy_Point->GetXaxis()->SetTitle("X (cm)");
	hxzy_Point->GetZaxis()->SetTitle("Z (cm)");
	hxzy_Point->GetYaxis()->SetTitle("Y (cm)");
	
	//Both detectors at same time hxyz_ambos
	
	TCanvas* c10 = new TCanvas("Position3","Collides of both detectors 3D",0,0,400,800);
        c10->SetFillColor(0);
        c10->SetFrameFillColor(0);
        hxyz_ambos->Draw("");
	hxyz_ambos->SetMarkerStyle(20); 
	hxyz_ambos->SetMarkerSize(0.4); 
	hxyz_ambos->SetMarkerColor(6); 
	hxyz_ambos->GetXaxis()->SetTitle("X (cm)");
	hxyz_ambos->GetZaxis()->SetTitle("Z (cm)");
	hxyz_ambos->GetYaxis()->SetTitle("Y (cm)");
	
	

}

