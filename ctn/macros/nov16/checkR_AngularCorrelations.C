////////////////////////////////////////////////////////////////////////////////////
////																			
////		--- Simulation of the Lisbon Nov-2016 setup ---						
////																			
////		Macro to check Angular Correlations between primary gammas			
////		applying the HPGe condition (see note1)								
////																			
////		Usage:																
////			1st: select the root file path & change the ranges (OPTIONAL)	
////			2nd: root -l checkR_AngularCorrelations.C						
////																			
////		Calculate: 															
////			-MCTrack: angle between Primary Gammas							
////																			
////			-Calo: petals Hit energy										
////																			
////			 Applying the HPGe condition:									
////				·petals Hit energy											
////				·Hit energy of petal at 90º & 180º w.r.t.HPGe				
////				·theta & phi angles 										
////				·multiplicity												
////				·Theta vs Energy Petals, Phi vs Energy Petals				
////				·Theta vs Phi												
////				·angle between gammas 										
////				·angle between gammas vs Energy								
////																			
////			-Save the histograms in a root file-> Histograms.root			
////																			
//// **elisabet.galiana@usc.es													
//// ** Universidad de Santiago de Compostela									
//// ** Dpto. Física de Partículas 												
////////////////////////////////////////////////////////////////////////////////////



//NOTE1: The HPGe condition
//  It is the application of the condition that one low energy gamma arrives to the HPGe detector 
//  and the other gamma, more energetic, arrives to one of both petals
//  we have to change the energy limits (limit1&limit2) for different cascades

//NOTE2: if you want to analyze the HPGePoint & CrystalPoint
//      you have to activate them before to execute runsim.C,
//		in order to create their branches
//
//		How to activate them: comment/descomment these lines
//		HPGe: ctn/detector/EnsarHPGeDet.cxx 
//				->FairRootManager::Instance()->Register("HPGeDetPoint", GetName(), fPointCollection, kTRUE);
//		Califa: calo/cal/R3BCalo.cxx
//				->FairRootManager::Instance()->Register("CrystalPoint", GetName(), fCaloCollection, kTRUE);
//		Then, you have to do "make" again in the EnsarRoot build directory and execute runsim.C  

void checkR_AngularCorrelations() {

	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE 
	char inputFile[250] = "outsim.root";//change the root file                        

	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION-----------------------------------------------------------//Change these ranges
	TH1F* h3         = new TH1F("h3","HPGe Total Energy",1000,0,13.);
	
	TH1F* h1_Cal     = new TH1F("h1_Cal","Petals Hit Total Energy",2000,0,15.); 
	TH1F* h1_Cal_2   = new TH1F("h1_Cal_2","Petals Hit Energy with Ge Condition",2000,0,13.); 
	TH1F* h2_Cal_1   = new TH1F("h2_Cal_1","Energy of Petals",2000,0,13.);
	TH1F* h2_Cal_2   = new TH1F("h2_Cal_2","Energy of Petals",2000,0,13.);
	TH1F* h3_Cal     = new TH1F("h3_Cal","Hit Theta",2000,-3.2,3.2);
	TH1F* h4_Cal     = new TH1F("h4_Cal","Hit Phi",200,-3.6,3.6);
	TH1F* h5_Cal     = new TH1F("h5_Cal","Hits Multiplicity",10,0,10);
	TH1F* h8_Cal_90  = new TH1F("h8_Cal_90","Energy of Petal around 90º",2000,0,13.);
	TH1F* h8_Cal_180 = new TH1F("h8_Cal_180","Energy of Petal around 180º",2000,0,13.);
	TH2F* h6_Cal     = new TH2F("h6_Cal","Theta vs Energy Petals", 2000,-3.2,3.2,2000,0,13.);
	TH2F* h7_Cal     = new TH2F("h7_Cal","Phi vs Energy Petals", 2000,-3.6,3.6,2000,0,13.);
	TH2F* h9_Cal     = new TH2F("h_9_Cal","Theta vs Phi", 720,0.,3.6,720,-3.6,3.6);	
	TH2F* h9_Cal_90  = new TH2F("h_9_Cal_90","Theta vs Phi (90)", 720,0.,3.6,720,-3.6,3.6);	
	TH2F* h9_Cal_180 = new TH2F("h_9_Cal_180","Theta vs Phi (180)", 720,0.,3.6,720,-3.6,3.6);	
	
	TH1F* h_alpha_W   = new TH1F("h_alpha_W","Reconstructed Angle between primary photons",200,0.,3.6);
	TH1F* h_alpha_MC  = new TH1F("h_alpha_MC","MCTrack Reconstructed Angle",200,0.,3.6);
	TH2F* h_alpha_egy = new TH2F("h_alpha_egy","Alpha vs Energy", 2000,0.,3.6,2000,0,12.);

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
	/*TClonesArray* hpgePointCA;
	EnsarHPGeDetPoint** hpgePoint;
	hpgePointCA = new TClonesArray("EnsarHPGeDetPoint",5);
	TBranch *branchEnsarHPGeDetPoint = tree ->GetBranch("HPGeDetPoint");
	branchEnsarHPGeDetPoint->SetAddress(&hpgePointCA );*/
	
	
	//----- CALIFA detector--------------//
	//Crystal Hits  
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
	
	
	//Crystal Points
	/*TClonesArray* crystalPointCA;  
	R3BCaloPoint** crystalPoint;
	crystalPointCA = new TClonesArray("R3BCaloPoint",5);
	TBranch *branchcrystalPoint = tree->GetBranch("CrystalPoint");
	branchcrystalPoint->SetAddress(&crystalPointCA);*/


	Int_t MCtracksPerEvent = 0;
	Int_t hpgeHitsPerEvent = 0;
	Int_t hpgePointsPerEvent = 0;
	Int_t crystalHitsPerEvent = 0;
	Int_t caloHitsPerEvent=0;
	Int_t crystalPointPerEvent=0;
    Double_t energyGe = 0.0;
    Double_t limit1 = 0.0, limit2=0.0;
    TVector3 momentum;
    
    
    //lower and upper limits Si-12900 cascade Generator    		change these limits 
    limit1=1.779 - 0.01*1.779;//-1% of gamma energy=1.779
    limit2=1.779 + 0.01*1.779;//+1%
    
    //lower and upper limits Co-60 cascade Generator
    //limit1=1.172 - 0.01*1.172;//-1% of gamma energy=1.172
    //limit2=1.172 + 0.01*1.172;//+1% 
    

	//TREE ENTRIES--------------------------------------------------------------------
	Long64_t nevents = tree->GetEntries();
   
        
	//LOOP IN THE EVENTS--------------------------------------------------------------
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);
		energyGe = 0.;
		tree->GetEvent(i);

		MCtracksPerEvent     = MCTrackCA->GetEntries();
		hpgeHitsPerEvent     = hpgeHitCA->GetEntries();
		//hpgePointsPerEvent   = hpgePointCA->GetEntries();
		crystalHitsPerEvent  = crystalHitCA->GetEntries();
		caloHitsPerEvent     = caloHitCA->GetEntries();
		//crystalPointPerEvent = crystalPointCA->GetEntries();

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
		/*if(crystalPointPerEvent>0) {
			crystalPoint = new R3BCaloPoint*[crystalPointPerEvent];
			for(Int_t j=0;j<crystalPointPerEvent;j++){
				crystalPoint[j] = new R3BCaloPoint;
				crystalPoint[j] = (R3BCaloPoint*) crystalPointCA->At(j);      
			}
		}*/
		
		//LOOP in MC mother tracks----------------------------------------------		
		Double_t Theta1_MC=0., Theta2_MC=0.;
    	Double_t Phi1_MC=0., Phi2_MC=0.;
    	Double_t E1_MC=0., E2_MC=0.;
    	
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
         
    		Double_t px1_MC=0., py1_MC=0., pz1_MC=0.;
    		Double_t px2_MC=0., py2_MC=0., pz2_MC=0.;
    		Double_t m1_MC=0., m2_MC=0.;
			Double_t M1_MC=0., M2_MC=0.;
			Double_t cos_alpha_MC=0.;
			Double_t alpha_MC=0.; 
                
            px1_MC= E1_MC*TMath::Sin(Theta1_MC)*TMath::Cos(Phi1_MC);
			py1_MC= E1_MC*TMath::Sin(Theta1_MC)*TMath::Sin(Phi1_MC);
			pz1_MC= E1_MC*TMath::Cos(Theta1_MC);
		
			px2_MC= E2_MC*TMath::Sin(Theta2_MC)*TMath::Cos(Phi2_MC);
			py2_MC= E2_MC*TMath::Sin(Theta2_MC)*TMath::Sin(Phi2_MC);
			pz2_MC= E2_MC*TMath::Cos(Theta2_MC);
				
			m1_MC = pow(px1_MC,2) + pow(py1_MC,2) + pow(pz1_MC,2);
			M1_MC = sqrt(m1_MC);
				
			m2_MC = pow(px2_MC,2) + pow(py2_MC,2) + pow(pz2_MC,2);
			M2_MC = sqrt(m2_MC);
		
			cos_alpha_MC = (px1_MC*px2_MC + py1_MC*py2_MC + pz1_MC*pz2_MC)/(M1_MC*M2_MC); 
			alpha_MC = TMath::ACos( cos_alpha_MC );
			h_alpha_MC->Fill(alpha_MC,1/TMath::Sin(alpha_MC));
        
        
        
        
         
        
        
         //LOOP in Calo Hits----------------    
         for(Int_t i=0;i<caloHitsPerEvent;i++){	
			h1_Cal->Fill(caloHit[i]->GetEnergy()*1000); //MeV 
			h9_Cal->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());     
			
			//Double_t Theta_out=0.;
			//Double_t Phi_out=0.;
			
			//Theta_out= caloHit[i]->GetTheta();
			//Phi_out = caloHit[i]->GetPhi();
			
			//cout<<"Phi_out= "<<Phi_out<<endl;
			//cout<<"Theta_out= "<<Theta_out<<endl;
		}
		
		//LOOP in HPGe Hits----------------
		for(Int_t h=0;h<hpgeHitsPerEvent;h++){ 
		
			energyGe = hpgeHit[h]->GetEnergy()*1000;//MeV
			h3->Fill(energyGe);
			
			if (energyGe>limit1 && energyGe<limit2){ //Applying the HPGe condition
													 //change the limit1 & limit2 for different cascades
			 Double_t px1=0., py1=0., pz1=0.;			
			 Double_t px2=0., py2=0., pz2=0.;	
			 Double_t Theta2=0., ThetaGe=0.; 
			 Double_t Phi2=0., PhiGe=0.;
			 Double_t E2=0.;
			 
			 //Angles of the Ge detector
			 ThetaGe = TMath::Pi()/2.;
			 PhiGe   = TMath::Pi();
		 
			 px1= energyGe*TMath::Sin(ThetaGe)*TMath::Cos(PhiGe);
			 py1= energyGe*TMath::Sin(ThetaGe)*TMath::Sin(PhiGe);
			 pz1= energyGe*TMath::Cos(ThetaGe);
			 
				//LOOP in Calo Hits-------
				for(Int_t i=0;i<caloHitsPerEvent;i++){
				
					h1_Cal_2->Fill(caloHit[i]->GetEnergy()*1000); //MeV
					h3_Cal->Fill(caloHit[i]->GetTheta());	      //rad
					h4_Cal->Fill(caloHit[i]->GetPhi());	    	  //rad
					
					Theta2 = caloHit[i]->GetTheta();
					Phi2 = caloHit[i]->GetPhi();
					E2 = caloHit[i]->GetEnergy()*1000;
					
					px2= E2*TMath::Sin(Theta2)*TMath::Cos(Phi2);
					py2= E2*TMath::Sin(Theta2)*TMath::Sin(Phi2);
					pz2= E2*TMath::Cos(Theta2);
					
					if (caloHitsPerEvent>0){
						h5_Cal->Fill(caloHitsPerEvent);
					}
					
					if (Theta2<1.){
						h2_Cal_1->Fill(caloHit[i]->GetEnergy()*1000); //Petal at 90º 
					}else {
						h2_Cal_2->Fill(caloHit[i]->GetEnergy()*1000); //Petal at 180º
					}
					
					h6_Cal->Fill(Theta2,E2);
					h7_Cal->Fill(Phi2,E2);
					
					
					//LOOP in Crystal Hits----------------    
		     		for(Int_t j=0;j<crystalHitsPerEvent;j++){
								 	//cout<<"CrystalId= "<<crystalHit[i]->GetCrystalId()<<endl;
								 	//cout<<"CrystalType= "<<crystalHit[i]->GetCrystalType()<<endl;
								 	//cout<<"CrystalCopy= "<<crystalHit[i]->GetCrystalCopy()<<endl;
					 	Double_t CryID=0.;
					 	CryID=crystalHit[j]->GetCrystalId();
					 	
					 	if (CryID>32 && CryID<41){
					 			h8_Cal_90->Fill(caloHit[i]->GetEnergy()*1000); //Petal around 90º
					 			h9_Cal_90->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		     			}
		     			if (CryID==98|| CryID==97|| CryID==90|| CryID==89|| CryID==95|| CryID==96|| CryID==103|| CryID==104){
					 			h8_Cal_180->Fill(caloHit[i]->GetEnergy()*1000); //Petal around 180º
					 			h9_Cal_180->Fill(caloHit[i]->GetTheta(),caloHit[i]->GetPhi());
		     			}
					
					
					
					}
				
			
					Double_t alpha=0.; //angle between two primary gammas	Prod Escalar
					if (caloHitsPerEvent>0){
						Double_t m1=0., m2=0.;
						Double_t M1=0., M2=0.;
						Double_t cos_alpha=0.;
				
				
						m1 = pow(px1,2) + pow(py1,2) + pow(pz1,2);
						M1 = sqrt(m1);
				
						m2 = pow(px2,2) + pow(py2,2) + pow(pz2,2);
						M2 = sqrt(m2);
		
						cos_alpha = (px1*px2 + py1*py2 + pz1*pz2)/(M1*M2); 
						alpha = TMath::ACos( cos_alpha );
						h_alpha_W->Fill(alpha);
			
						//cout<<"Gamma1= "<<px1<<", "<<py1<<", "<<pz1<<endl;
						//cout<<"Gamma2= "<<px2<<", "<<py2<<", "<<pz2<<endl;
						//cout<<"Alpha= "<<alpha<<endl;	
					}
				
					for(Int_t i=0;i<caloHitsPerEvent;i++){
						h_alpha_egy->Fill(alpha,caloHit[i]->GetEnergy()*1000);
					}
			
         }
	 	}
		

		if(MCtracksPerEvent)     delete[] track;
		if(hpgeHitsPerEvent)     delete[] hpgeHit;
		//if(hpgePointsPerEvent)   delete[] hpgePoint;
		if(crystalHitsPerEvent)  delete[] crystalHit;
		if(caloHitsPerEvent)     delete[] caloHit;
		//if(crystalPointPerEvent) delete[] crystalPoint;

	}
	
	}
	// END LOOP IN THE EVENTS---------------------------------------------------------

	//Save Histograms in a file
	TFile* Histos=new TFile("Histosgrams.root","recreate");
	h3->Write();
	h1_Cal->Write();
	h1_Cal_2->Write();
	h2_Cal_1->Write();
	h2_Cal_2->Write();
	h3_Cal->Write();
	h4_Cal->Write();
	h5_Cal->Write();
	h6_Cal->Write();
	h7_Cal->Write();
	h_alpha_W->Write();
	h8_Cal_90->Write();
	h8_Cal_180->Write();
	h9_Cal->Write();
	h9_Cal_90->Write();
	h9_Cal_180->Write();
	Histos->Close();
	
	
	// HISTOGRAMS--------------------------------------------------------------------- 
	
	//HPGe-------------
	TCanvas* c2 = new TCanvas("c2","Energy in the HPGe",0,0,400,400);
	c2->SetFillColor(0);
	c2->SetFrameFillColor(0);
	Int_t ci;// for color index setting
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

	//CALIFA ----------------------
	//CaloHit
	TCanvas* c13 = new TCanvas("Angles","Angles",0,0,720,900);
	c13->SetFillColor(0);
	c13->SetFrameFillColor(0);
	c13->Divide(2,2);
	
	c13->cd(1);  
	h3_Cal->Draw();
	h3_Cal->SetLineColor(9);
	h3_Cal->GetXaxis()->SetTitle("Theta (rad)");
	h3_Cal->GetYaxis()->SetTitle("Counts");
	
	c13->cd(2); 
	h4_Cal->Draw();
	h4_Cal->SetLineColor(13);
	h4_Cal->GetXaxis()->SetTitle("Phi (rad)");
	h4_Cal->GetYaxis()->SetTitle("Counts");
	
	c13->cd(3);          
	h_alpha_egy->Draw("colz");
	h_alpha_egy->SetLineColor(4);
	h_alpha_egy->GetXaxis()->SetTitle("Detectors Alpha angle (rad)");
	h_alpha_egy->GetYaxis()->SetTitle("Energy (MeV)");
	
	c13->cd(4); 
	h_alpha_MC->Draw();
	h_alpha_MC->SetLineColor(3);
	h_alpha_MC->GetXaxis()->SetTitle("MCTrack Alpha angle (rad)");
	h_alpha_MC->GetYaxis()->SetTitle("Counts");
	//------------------
	

	TCanvas* c14 = new TCanvas("Energy Petals","Energy Petals",0,0,400,800);          
	c14->SetFillColor(0);       
	c14->SetFrameFillColor(0);  
	c14->Divide(2,1); 
	
	c14->cd(1);
	h1_Cal->Draw(); c13->cd(2)->SetLogy();
	h1_Cal->SetLineColor(8);
	h1_Cal->GetXaxis()->SetTitle("Hit Total Energy (MeV)");
	h1_Cal->GetYaxis()->SetTitle("Counts");
	
	c14->cd(2);          
	h1_Cal_2->Draw();
	h1_Cal_2->SetLineColor(4);
	h1_Cal_2->GetXaxis()->SetTitle("Hit Energy with Ge Condition (MeV)");
	h1_Cal_2->GetYaxis()->SetTitle("Counts");
	//------------------
	
	
	TCanvas* c3 = new TCanvas("Hits Multiplicity","Hits Multiplicity",0,0,400,800);          
	c3->SetFillColor(0);       
	c3->SetFrameFillColor(0);           
	h5_Cal->Draw();
	h5_Cal->SetLineColor(7);
	h5_Cal->GetXaxis()->SetTitle("Hits Multiplicity");
	h5_Cal->GetYaxis()->SetTitle("Counts");
	//------------------
	
	
	TCanvas* c15 = new TCanvas("Recontructed Angle","Recontructed Angle",0,0,400,800);          
	c15->SetFillColor(0);       
	c15->SetFrameFillColor(0);           
	h_alpha_W->Draw();
	h_alpha_W->SetLineColor(7);
	h_alpha_W->GetXaxis()->SetTitle("Recontructed Angle (rad)");
	h_alpha_W->GetYaxis()->SetTitle("Counts");	
	//------------------
	
	
	TCanvas* c16 = new TCanvas("Energy","Energy of Petals",0,0,400,800);          
	c16->SetFillColor(0);       
	c16->SetFrameFillColor(0);  
	c16->Divide(2,2);
	
	c16->cd(1);
	h2_Cal_1->Draw(); 
	h2_Cal_1->SetLineColor(7);
	h2_Cal_1->GetXaxis()->SetTitle("Energy Petal 90 (MeV)");
	h2_Cal_1->GetYaxis()->SetTitle("Counts");
	
	c16->cd(2);
	h2_Cal_2->Draw(); 
	h2_Cal_2->SetLineColor(8);
	h2_Cal_2->GetXaxis()->SetTitle("Energy Petal 180 (MeV)");
	h2_Cal_2->GetYaxis()->SetTitle("Counts");
	
	c16->cd(3);
	h6_Cal->Draw("colz"); 
	h6_Cal->GetXaxis()->SetTitle("Theta");
	h6_Cal->GetYaxis()->SetTitle("Energy Hit (MeV)");
	
	c16->cd(4);
	h7_Cal->Draw("colz"); 
	h7_Cal->GetXaxis()->SetTitle("Phi");
	h7_Cal->GetYaxis()->SetTitle("Energy Hit (MeV)");
	//------------------
	
	
	TCanvas* c17 = new TCanvas("Energy angles cuts","Energy of Petals with angles cuts",0,0,400,800);          
	c17->SetFillColor(0);       
	c17->SetFrameFillColor(0);  
	c17->Divide(2,2);
	
	c17->cd(1);
	h8_Cal_90->Draw(); 
	h8_Cal_90->SetLineColor(7);
	h8_Cal_90->GetXaxis()->SetTitle("Energy Petal around 90 windows (MeV)");
	h8_Cal_90->GetYaxis()->SetTitle("Counts");
	
	c17->cd(2);
	h8_Cal_180->Draw(); 
	h8_Cal_180->SetLineColor(8);
	h8_Cal_180->GetXaxis()->SetTitle("Energy Petal around 180 windows (MeV)");
	h8_Cal_180->GetYaxis()->SetTitle("Counts");
	
	c17->cd(3);
	h9_Cal_90->Draw("colz"); 
	h9_Cal_90->GetXaxis()->SetTitle("Theta (rad)");
	h9_Cal_90->GetYaxis()->SetTitle("Phi (rad)");
	
	c17->cd(4);
	h9_Cal_180->Draw("colz"); 
	h9_Cal_180->GetXaxis()->SetTitle("Theta (rad)");
	h9_Cal_180->GetYaxis()->SetTitle("Phi (rad)");
	//------------------
	
	
	TCanvas* c18 = new TCanvas("Theta vs Phi","Theta vs Phi",0,0,400,800);          
	c18->SetFillColor(0);       
	c18->SetFrameFillColor(0);           
	h9_Cal->Draw("colz"); 
	h9_Cal->GetXaxis()->SetTitle("Theta (rad)");
	h9_Cal->GetYaxis()->SetTitle("Phi (rad)");
	//------------------
	
	TCanvas* c19 = new TCanvas("Energy of both Petals","Energy of both Petals",0,0,400,800);          
	c16->SetFillColor(0);       
	c16->SetFrameFillColor(0);  
	h2_Cal_1->Draw(); 
	h2_Cal_1->SetLineColor(2);
	h2_Cal_1->GetXaxis()->SetTitle("Energy Petals (MeV)");
	h2_Cal_1->GetYaxis()->SetTitle("Counts");
	h2_Cal_2->Draw("same"); 
	h2_Cal_2->SetLineColor(8);
	


}

