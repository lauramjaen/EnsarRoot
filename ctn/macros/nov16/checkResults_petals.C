
void checkResults_petals() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
	char inputFile[250] = "outsim.root";                                              
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION----------------------------------------------------------- Change the intervals for each run energy
  	
	TH1F* h1_Cry = new TH1F("h1_Cry","Crystal ID",150,0,150);
	TH1F* h2_Cry = new TH1F("h2_Cry","Crystal Energy (MeV)",200,0,10.2);
	TH1F* h3_Cry = new TH1F("h3_Cry","Crystal Type",32,0,32);
	TH1F* h4_Cry = new TH1F("h4_Cry","Crystal Copy",15,0,15);
	
	TH1F* h1_Cal = new TH1F("h1_Cal","Hit Energy (MeV)",200,0,10); //Change this maximum energy
	TH1F* h3_Cal = new TH1F("h3_Cal","Hit Theta",200,-3.2,3.2);
	TH1F* h4_Cal = new TH1F("h4_Cal","Hit Phi",200,-3.6,3.6);//-3.2,3.2
	
	TH2F* hxz_Point = new TH2F("hxz_Point","Position x and z", 180,-20.,70.,200,-30.,70.);
	TH3F* hxzy_Point = new TH3F("hxzy_Point","Position x, z and y", 180,-20.,70.,200,-30.,70., 40,-10.,10.);
	
	
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
	//--------------------------------------------------------------------------------


	Int_t crystalHitsPerEvent = 0;
	Int_t caloHitsPerEvent=0;
	Int_t crystalPointPerEvent=0;

	//TREE ENTRIES--------------------------------------------------------------------
	Long64_t nevents = tree->GetEntries();
       
	//LOOP IN THE EVENTS--------------------------------------------------------------
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);
		tree->GetEvent(i);

		crystalHitsPerEvent  = crystalHitCA->GetEntries();
		caloHitsPerEvent     = caloHitCA->GetEntries();
		crystalPointPerEvent = crystalPointCA->GetEntries();

		// loop to read the branches Crystal and Calo
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
		
                //loop in crystal Hits
		for(Int_t h=0;h<crystalHitsPerEvent;h++){
			h1_Cry->Fill(crystalHit[h]->GetCrystalId());
			h2_Cry->Fill(crystalHit[h]->GetEnergy()*1000);
			h3_Cry->Fill(crystalHit[h]->GetCrystalType());
			h4_Cry->Fill(crystalHit[h]->GetCrystalCopy());
		}
		if(crystalHitsPerEvent) delete[] crystalHit;
		
		//loop in calorimeter Hits
		for(Int_t h=0;h<caloHitsPerEvent;h++){
		
			h1_Cal->Fill(caloHit[h]->GetEnergy()*1000); //MeV
			h3_Cal->Fill(caloHit[h]->GetTheta());	    //rad
			h4_Cal->Fill(caloHit[h]->GetPhi());	    //rad
			/*h1_Cal->Fill(caloHit[h]->GetNbOfCrystalHits());	
			h2_Cal->Fill(caloHit[h]->GetEnergy()*1000);	
			h3_Cal->Fill(caloHit[h]->GetTheta());	
			h4_Cal->Fill(caloHit[h]->GetPhi());
			h2_CC->Fill(GetCMEnergy(caloHit[h]->GetTheta(),caloHit[h]->GetEnergy()*1000,beta));			   //with Boost of Lorentz
			h2_CC2->Fill(caloHit[h]->GetTheta(),GetCMEnergy(caloHit[h]->GetTheta(),caloHit[h]->GetEnergy()*1000,beta));//with Boost of Lorentz*/
		}
		if(caloHitsPerEvent)    delete[] caloHit;
		
		//loop in crystal points
		for(Int_t h=0;h<crystalPointPerEvent;h++){
		
			Double_t X=0.; Double_t X2=0.;
			Double_t Y=0.; Double_t Y2=0.;
			Double_t Z=0.; Double_t Z2=0.;
			
			X=crystalPoint[h]->GetXIn();
			X2=-X; 
			Y=crystalPoint[h]->GetYIn();
			Y2=-Y;
			Z=crystalPoint[h]->GetZIn();
			Z2=-Z;
			
			hxz_Point->Fill(X2,Z);
			hxzy_Point->Fill(X2,Z,Y);
			
		}
		if(crystalPointPerEvent)    delete[] crystalPoint;
		
	}
	
	// END LOOP IN THE EVENTS---------------------------------------------------------


	// DRAW HISTOGRAMS----------------------------------------------------------------
	
	//CrystalHit
	TCanvas* c8 = new TCanvas("crystalHit","crystalHit",0,0,720,900);
	c8->SetFillColor(0);
	c8->SetFrameFillColor(0);
	c8->cd();
	c8->Divide(2,2);//Change if you want to plot h3_Cry,h4_Cry
	
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
	
	c8->cd(3); 
	h3_Cry->Draw();
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
	TCanvas* c1 = new TCanvas("CaloHit","CaloHit",0,0,720,900);
	c1->SetFillColor(0);
	c1->SetFrameFillColor(0);
	c1->cd();
	c1->Divide(2,2);
	
	c1->cd(1); 
	h1_Cal->Draw();  c1->cd(1)->SetLogy();
	h1_Cal->SetLineColor(3);
	h1_Cal->GetXaxis()->SetTitle("Energy (MeV/c^{2})");
	h1_Cal->GetYaxis()->SetTitle("Counts");
	 
	c1->cd(2); 
	h3_Cal->Draw();
	h3_Cal->SetLineColor(9);
	h3_Cal->GetXaxis()->SetTitle("Hit Theta");
	h3_Cal->GetYaxis()->SetTitle("Counts");
	
	
	c1->cd(3); 
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
	
	
}

