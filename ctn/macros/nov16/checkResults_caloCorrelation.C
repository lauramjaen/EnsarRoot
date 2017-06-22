////////////////////////////////////////////////////////////////////////////
////   																	////
////  This macro calculate the angle between two gamma Hits in caloHit  ////
////   																	////
////////////////////////////////////////////////////////////////////////////



void checkResults_caloCorrelation() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
	//char inputFile[250] = "/mnt/scratch/eli/outsim_iso.root";     //change       Theta Isotropic   
	//char inputFile[250] = "/mnt/scratch/eli/outsim_W.root";   //Theta with Angular Correlation              
	char inputFile[250] = "outsim.root";         
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION-----------------------------------------------------------//Change these maximum energies
	
	TH1F* h1_Cry = new TH1F("h1_Cry","Crystal ID",150,0,150);
	TH1F* h2_Cry = new TH1F("h2_Cry","Petals Crystal Energy",200,0,13.);
	TH1F* h3_Cry = new TH1F("h3_Cry","Crystal Type",32,0,32);
	TH1F* h4_Cry = new TH1F("h4_Cry","Crystal Copy",15,0,15);
	TH1F* h5_Cry = new TH1F("h5_Cry","Crystal Primary Tracks",10,0,10);
	TH1F* h5_Cry_2= new TH1F("h5_Cry_2","Crystal Multiplicity",10,0,10);
	
	TH1F* h1_Cal = new TH1F("h1_Cal","Petals Hit Energy",2000,0,15.); 
	TH1F* h3_Cal = new TH1F("h3_Cal","Hit Theta",2000,-3.2,3.2);
	TH1F* h4_Cal = new TH1F("h4_Cal","Hit Phi",200,-3.6,3.6);
	TH1F* h5_Cal = new TH1F("h5_Cal","Hits Multiplicity",10,0,10);
	
	TH1F* h_alpha_W = new TH1F("h_alpha_W","Reconstructed Angle between primary photons",200,0.,3.6);

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
    Double_t Theta1_MC=0., Theta2_MC=0.;
    Double_t Phi1_MC=0., Phi2_MC=0.;
    Double_t E1_MC=0., E2_MC=0.;
    Double_t Theta1=0., Theta2=0.;
    Double_t Phi1=0., Phi2=0.;
    Double_t E1=0., E2=0.;
    Double_t px1=0., py1=0., pz1=0.;
	Double_t px2=0., py2=0., pz2=0.;
	
	Int_t primarias = 0;
        

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
				track[h]->GetMomentum(momentum);
                
                if (h==0){
                	E1_MC     = track[h]->GetEnergy()*1000;//MeV
                	Theta1_MC = momentum.Theta();
               		Phi1_MC   = momentum.Phi();
                }else{
                	E2_MC     = track[h]->GetEnergy()*1000;//MeV
                  	Theta2_MC = momentum.Theta();
               		Phi2_MC   = momentum.Phi();
                }
                
            }          
		}	
		//LOOP in crystal Hits
		for(Int_t h=0;h<crystalHitsPerEvent;h++){
		    //cout << "GPT: " << crystalHit[h]->GetParentTrackId() << "   PDG: " << crystalHit[h]->GetTrackPID() << "    ID: " << crystalHit[h]->GetTrackId() << endl; 
			h1_Cry->Fill(crystalHit[h]->GetCrystalId());
			h2_Cry->Fill(crystalHit[h]->GetEnergy()*1000);//MeV
			h3_Cry->Fill(crystalHit[h]->GetCrystalType());
			h4_Cry->Fill(crystalHit[h]->GetCrystalCopy());
			if(crystalHit[h]->GetParentTrackId()<0) primarias++;	
			//h5_Cry->Fill(crystalHitsPerEvent);
		}
		if(primarias) h5_Cry->Fill(primarias);
		primarias=0;
		if(crystalHitsPerEvent) h5_Cry_2->Fill(crystalHitsPerEvent);
		//LOOP in calorimeter Hits
		if(caloHitsPerEvent) h5_Cal->Fill(caloHitsPerEvent);
		for(Int_t h=0;h<caloHitsPerEvent;h++){
			h1_Cal->Fill(caloHit[h]->GetEnergy()*1000); //MeV
			h3_Cal->Fill(caloHit[h]->GetTheta());	    //rad
			h4_Cal->Fill(caloHit[h]->GetPhi());	    	//rad
			
			
			if (caloHitsPerEvent==2){
			
				Theta1 = caloHit[0]->GetTheta();
				Phi1 = caloHit[0]->GetPhi();
				E1 = caloHit[0]->GetEnergy()*1000;
				
				Theta2 = caloHit[1]->GetTheta();
				Phi2 = caloHit[1]->GetPhi();
				E2 = caloHit[1]->GetEnergy()*1000;
				
				//cut angle_MC-10º < angle < angle_MC+10º (10º=0.175rad)
				if (Theta1>Theta1_MC-0.175 || Theta1< Theta1_MC+0.175){
				  if (Theta2>Theta2_MC-0.175 || Theta2< Theta2_MC+0.175){	
			
				
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
				
					m1 = pow(px1,2) + pow(py1,2) + pow(pz1,2);
					M1 = sqrt(m1);
		
					m2 = pow(px2,2) + pow(py2,2) + pow(pz2,2);
					M2 = sqrt(m2);
		
					cos_alpha = (px1*px2 + py1*py2 + pz1*pz2)/(M1*M2); 
					alpha = TMath::ACos( cos_alpha );
					h_alpha_W->Fill(alpha);
				  }
				}
			
			}
		}
	}
		

		if(MCtracksPerEvent)     delete[] track;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		if(caloHitsPerEvent)     delete[] caloHit;
		
	// END LOOP IN THE EVENTS---------------------------------------------------------


	// HISTOGRAMS--------------------------------------------------------------------- 
	
	//MCTrack-------------
	
	

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
	TCanvas* c13 = new TCanvas("Hit Angles","Hit Angles",0,0,720,900);
	c13->SetFillColor(0);
	c13->SetFrameFillColor(0);
	c13->Divide(2,2);
	
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
	
	c13->cd(3); 
	h1_Cal->Draw(); c13->cd(2)->SetLogy();
	h1_Cal->SetLineColor(8);
	h1_Cal->GetXaxis()->SetTitle("Hit Energy (MeV)");
	h1_Cal->GetYaxis()->SetTitle("Counts");
	
	TCanvas* c14 = new TCanvas("Angles","Angles",0,0,400,800);          
	c14->SetFillColor(0);       
	c14->SetFrameFillColor(0);             
	h_alpha_W->Draw();
	h_alpha_W->GetXaxis()->SetTitle("Alpha angle (rad)");
	h_alpha_W->GetYaxis()->SetTitle("Counts");
	
	TCanvas* c15 = new TCanvas("Hits Multiplicity","Hits Multiplicity",0,0,400,800);          
	c15->SetFillColor(0);       
	c15->SetFrameFillColor(0);  
	c15->Divide(1,2);
	
	c15->cd(1);           
	h5_Cal->Draw();
	h5_Cal->GetXaxis()->SetTitle("Hits Multiplicity");
	h5_Cal->GetYaxis()->SetTitle("Counts");
	
	c15->cd(2);           
	h5_Cry_2->Draw();
	h5_Cry_2->GetXaxis()->SetTitle("Crystals Multiplicity");
	h5_Cry_2->GetYaxis()->SetTitle("Counts");
	h5_Cry->Draw("same");
	h5_Cry->SetLineColor(8);
	
	//cout << endl << endl << h1_Cal->Integral(1512,1750) << endl << endl;

}
