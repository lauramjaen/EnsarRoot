////////////////////////////////////////////////////////////////////////////
////   																	////
////  This macro calculate the angle between two primary gamma tracks   ////
////   																	////
////////////////////////////////////////////////////////////////////////////


void checkResults_MCCorrelation() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
	//char inputFile[250] = "/mnt/scratch/eli/outsim_iso.root"; //change       Theta Isotropic   
	//char inputFile[250] = "/mnt/scratch/eli/outsim_W.root";   //Theta with Angular Correlation  
	char inputFile[250] = "outsim.root";                                         
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION-----------------------------------------------------------//Change these maximum energies
    TH1F* h1   = new TH1F("h1","Primary PDG Code",60,-30,30);
    TH1F* h1_2 = new TH1F("h1_2","Secondaries PDG Code",60,-30,30);
    TH1F* h2   = new TH1F("h2","Primary Energy",1100,0,15);
	TH1F* h3_T = new TH1F("h3_T","Primary Theta",200,0,3.2);
	TH1F* h4_T = new TH1F("h4_T","Primary Phi",200,-3.2,3.2);
	
	TH1F* h1_Cry = new TH1F("h1_Cry","Crystal ID",150,0,150);
	TH1F* h2_Cry = new TH1F("h2_Cry","Petals Crystal Energy",200,0,15);
	TH1F* h3_Cry = new TH1F("h3_Cry","Crystal Type",32,0,32);
	TH1F* h4_Cry = new TH1F("h4_Cry","Crystal Copy",15,0,15);
	
	TH1F* h1_Cal = new TH1F("h1_Cal","Petals Hit Energy",2000,0,15); 
	TH1F* h3_Cal = new TH1F("h3_Cal","Hit Theta",2000,-3.2,3.2);
	TH1F* h4_Cal = new TH1F("h4_Cal","Hit Phi",200,-3.6,3.6);
	
	TH1F* h_alpha = new TH1F("h_alpha","Reconstructed Angle between primary photons",200,0.,3.6);
	TH1F* h_theta = new TH1F("h_theta","Theta increment",200,-3.6,3.6);
	
	

	//----   MCTrack (input)   -------------------------------------------------------
	TClonesArray* MCTrackCA;
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);
	
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


	//Variables definition ---------
	Int_t MCtracksPerEvent = 0;
	Int_t crystalHitsPerEvent = 0;
	Int_t caloHitsPerEvent=0;

    TVector3 momentum;
    Double_t Theta1=0., Theta2=0.;
    Double_t Phi1=0., Phi2=0.;
    Double_t E1=0., E2=0.;
    Double_t px1=0., py1=0., pz1=0.;
	Double_t px2=0., py2=0., pz2=0.;
        

	//TREE ENTRIES--------------------------------------------------------------------
	Long64_t nevents = tree->GetEntries();
        
	//LOOP IN THE EVENTS--------------------------------------------------------------
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);

		tree->GetEvent(i);

		MCtracksPerEvent     = MCTrackCA->GetEntries();
		crystalHitsPerEvent  = crystalHitCA->GetEntries();
		caloHitsPerEvent     = caloHitCA->GetEntries();

		if(MCtracksPerEvent>0) {
			track = new EnsarMCTrack*[MCtracksPerEvent];
			for(Int_t j=0;j<MCtracksPerEvent;j++){
				track[j] = new EnsarMCTrack;
				track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
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
		
		//LOOP in MC mother tracks----------------------------------------------
		for(Int_t h=0;h<MCtracksPerEvent;h++){
			if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
				h1->Fill(track[h]->GetPdgCode());
				h2->Fill(track[h]->GetEnergy()*1000);//MeV
				track[h]->GetMomentum(momentum);
                h3_T->Fill(momentum.Theta());
				h4_T->Fill(momentum.Phi());
                
                if (h==0){
                	E1     = track[h]->GetEnergy()*1000;//MeV
                	Theta1 = momentum.Theta();
               		Phi1   = momentum.Phi();
                }else{
                	E2     = track[h]->GetEnergy()*1000;//MeV
                  	Theta2 = momentum.Theta();
               		Phi2   = momentum.Phi();
                }
                
            } else {
                 h1_2->Fill(track[h]->GetPdgCode());
            }              
		}
		
		// Angle between two primary gamma MCTrack
		px1= E1*TMath::Sin(Theta1)*TMath::Cos(Phi1);
		py1= E1*TMath::Sin(Theta1)*TMath::Sin(Phi1);
		pz1= E1*TMath::Cos(Theta1);
		
		px2= E2*TMath::Sin(Theta2)*TMath::Cos(Phi2);
		py2= E2*TMath::Sin(Theta2)*TMath::Sin(Phi2);
		pz2= E2*TMath::Cos(Theta2);
		
		Double_t m1=0., m2=0.;
		Double_t M1=0., M2=0.;
		Double_t cos_alpha=0.;
		Double_t alpha=0.; //angle between two primary gammas
		Double_t inc_theta=0.;
		
		inc_theta= Theta1-Theta2;
		h_theta->Fill(inc_theta);
		
		m1 = pow(px1,2) + pow(py1,2) + pow(pz1,2);
		M1 = sqrt(m1);
		
		m2 = pow(px2,2) + pow(py2,2) + pow(pz2,2);
		M2 = sqrt(m2);
		
		cos_alpha = (px1*px2 + py1*py2 + pz1*pz2)/(M1*M2); 
		alpha = TMath::ACos( cos_alpha );
		h_alpha->Fill(alpha);
		
		//New Function for fitting
		//TF1 *fW = new TF1("fW"," (-TMath::Sin(x) +0.95*TMath::Sin(x)*pow((TMath::Cos(x)),2) +0.95*TMath::Sin(x)*pow((TMath::Cos(x)),4)", 0, 300);
		//fW->SetParameter(0,0.95);
		//fW->SetParameter(1,0.95);
		
		
		
		//cout<<"Angle between two primary gammas is alpha= "<<alpha*180/TMath::Pi()<<endl;
		
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
			h4_Cal->Fill(caloHit[h]->GetPhi());	    	//rad
		}
		

		if(MCtracksPerEvent)     delete[] track;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		if(caloHitsPerEvent)     delete[] caloHit;

	}
	

 
	// END LOOP IN THE EVENTS---------------------------------------------------------


	// HISTOGRAMS--------------------------------------------------------------------- 
	
	//MCTrack-------------
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


	TCanvas* c11 = new TCanvas("MCTrack_angles","MCTrack_angles",0,0,400,800);
	c11->Divide(1,2);           
	c11->SetFillColor(0);       
	c11->SetFrameFillColor(0);
	c11->cd(1);                 
	h3_T->Draw();
	h3_T->GetXaxis()->SetTitle("Theta MCTrack");
	h3_T->GetYaxis()->SetTitle("Counts"); 
	c11->cd(2);                  
	h4_T->Draw();
	h4_T->SetLineColor(9);        
	h4_T->GetXaxis()->SetTitle("Phi MCTrack");
	h4_T->GetYaxis()->SetTitle("Counts");
	

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
	
	
	TCanvas* c12 = new TCanvas("Energy","Energy",0,0,400,800);
	c12->Divide(2,2);           
	c12->SetFillColor(0);       
	c12->SetFrameFillColor(0);
	c12->cd(1);                 
	h2->Draw();	 c12->cd(1)->SetLogy();
	h2->SetLineColor(6); 
	h2->GetXaxis()->SetTitle("MCTrack Energy");
	h2->GetYaxis()->SetTitle("Counts"); 
	c12->cd(2);  c12->cd(2)->SetLogy();            
	h2_Cry->Draw();
	h2_Cry->SetLineColor(8);        
	h2_Cry->GetXaxis()->SetTitle("Crystal Energy");
	h2_Cry->GetYaxis()->SetTitle("Counts");
	c12->cd(3);  c12->cd(3)->SetLogy();         
	h1_Cal->Draw();
	h1_Cal->SetLineColor(30);        
	h1_Cal->GetXaxis()->SetTitle("Petals Energy");
	h1_Cal->GetYaxis()->SetTitle("Counts");
	
	
	TCanvas* c13 = new TCanvas("Hit Angles","Hit Angles",0,0,720,900);
	c13->SetFillColor(0);
	c13->SetFrameFillColor(0);
	c13->Divide(1,2);
	c13->cd(1);  
	h3_Cal->Draw();
	h3_Cal->SetLineColor(9);
	h3_Cal->GetXaxis()->SetTitle("Hit Theta");
	h3_Cal->GetYaxis()->SetTitle("Counts");
	c13->cd(2); 
	h4_Cal->Draw();
	h4_Cal->SetLineColor(13);
	h4_Cal->GetXaxis()->SetTitle("Hit Phi");
	h4_Cal->GetYaxis()->SetTitle("Counts");
	
	TCanvas* c14 = new TCanvas("Angles","Angles",0,0,400,800);          
	c14->SetFillColor(0);       
	c14->SetFrameFillColor(0);  
	//c14->Divide(1,2);
	//c14->cd(1);             
	h_alpha->Draw(); //h_alpha->Fit("fit");
	h_alpha->GetXaxis()->SetTitle("Alpha angle (rad)");
	h_alpha->GetYaxis()->SetTitle("Counts");
	//c14->cd(2);
	//h_theta->Draw();
	//h_theta->GetXaxis()->SetTitle("Theta increment (rad)");
	//h_theta->GetYaxis()->SetTitle("Counts");
	

}

