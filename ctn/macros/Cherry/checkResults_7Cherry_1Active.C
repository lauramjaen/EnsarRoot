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

void checkResults_7Cherry_1Active() {


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
  TH1F* h3   = new TH1F("h3","Cherry Total Energy",1000,0,11);
  TH1F* h4   = new TH1F("h4","Theta",400,-4,4);
  TH1F* hx   = new TH1F("hx","X de impacto",600,-6,6);
 	TH1F *hxdif = new TH1F("hxdiff","hxdiff",500,-100,100);
  TH1F *hydif = new TH1F("hydiff","hydiff",500,-100,100);

	TH1F *hz_Frontenergy = new TH1F("hz_Frontenergy","Front Energy",1100,0,11);
	TH1F *hz_Backenergy  = new TH1F("hz_Backenergy","Back Energy",1100,0,11);
	TH1F *hSkin_energy = new TH1F("hSkin_energy","Energy Skin and In",1100,0,11);
	TH1F *hIn_energy  = new TH1F("hIn_energy","Energy In deposited in the Cherry",1100,0,11);
	
	TH1F *hSkin_front = new TH1F("hSkin_front","Energy in different parts",1100,0,11);
	TH1F *hSkin_back  = new TH1F("hSkin_back","Skin back: Energy",1100,0,11);
	TH1F *hIn_front  = new TH1F("hIn_front","In front: Energy",1100,0,11);
	TH1F *hIn_back  = new TH1F("hIn_back","In back: Energy",1100,0,11);
  
	TH2F* hxy  = new TH2F("hxy","Front View", 200,-4,4,200,-4,4);
	TH2F* hzx  = new TH2F("hzx","Depth Profiling", 200,56,61,200,-4,4);
	TH2F* hz_energy  = new TH2F("hz_energy","Energy Profiling in z axis", 200,56,61,1100,0,11);
	TH2F* hx_energy  = new TH2F("hx_energy","Energy Profiling in x axis", 200,-4,4,1100,0,11);
  
	TH3F* hxyz = new TH3F("hxyz","Collides 3D", 100,-4,4,100,-4,4,100,57,70);  
	TH3F* hxyz_energy = new TH3F("hxyz_energy","Collides 3D and Energy", 100,-4,4,100,-4,4,100,56,61);

  
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
	Double_t xx, yy, zz;  
	//Change these coordinates in order to pass from RSLab to RSCherry
	//CenterCherry (0,0,58.5)
	xx=0.;
	yy=3.;
  zz=58.5;
	Double_t energy_in, energy_skin, energy_front, energy_back;
	Double_t energy_in_front, energy_skin_front, energy_in_back, energy_skin_back;
	energy_in=0.;
	energy_skin=0.;
	energy_front=0.;
	energy_back=0.;
	energy_in_front=0.;
	energy_skin_front=0.;
	energy_in_back=0.;
	energy_skin_back=0.;


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
    

		//LOOP in HPGePoints
		for(Int_t r=0;r<hpgePointsPerEvent;r++) {     

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

					Double_t radius;
					Double_t x1, x2, x3;
					x1=abs(X1-xx);
					x2=abs(X2-yy);
					x3=abs(X3-zz);
					radius=sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
					//cout<<"x="<<X1<<" y="<<X2<<" z="<<X3<<" "<<x3;
					//cout<<"  radius= "<<radius<<endl;

					//Differences between Skin & Innerpart
					if (radius<1.45){//IN 0.5mm->1.45 100micro->1.49
						hIn_energy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);
						energy_in=energy_in+hpgePoint[r]->GetEnergyLoss()*1000;
							if (X3<zz){
								//In front side	
								hIn_front->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//In front
								energy_in_front=energy_in_front+hpgePoint[r]->GetEnergyLoss()*1000;	
							}else{
								//In back side
								hIn_back->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//In back
								energy_in_back=energy_in_back+hpgePoint[r]->GetEnergyLoss()*1000;	
							}

					}else{//SKIN
						hSkin_energy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);
						energy_skin=energy_skin+hpgePoint[r]->GetEnergyLoss()*1000;
							if (X3<zz){
								//Skin front side	
								hSkin_front->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//Skin front
								energy_skin_front=energy_skin_front+hpgePoint[r]->GetEnergyLoss()*1000;				
							}else{
								//Skin back side
								hSkin_back->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//Skin back
								energy_skin_back=energy_skin_back+hpgePoint[r]->GetEnergyLoss()*1000;	
							}
					}

					//Differences between Front & Back side
					if (X3<zz){
						hz_Frontenergy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//MeV Front side of cherry
						energy_front=energy_front+hpgePoint[r]->GetEnergyLoss()*1000;
					}else{
						hz_Backenergy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//MeV Back side
						energy_back=energy_back+hpgePoint[r]->GetEnergyLoss()*1000;
					}

					
			}



 // END LOOP IN THE EVENTS---------------------------------------------------------  

    

  }
 
	/////////////////////////////////

 // END LOOP IN THE EVENTS---------------------------------------------------------
/*
energy_front=0.;
	energy_back=0.;
	energy_in_front=0.;
	energy_skin_front=0.;
	energy_in_back=0.;
	energy_skin_back=0.;
*/
//Double_t sum2;
//sum2=energy_in+energy_skin;
//cout<<"sum2= "<<sum2<<endl;
	//Total energy
	Double_t sum;
	sum= energy_in_front + energy_skin_front+ energy_in_back + energy_skin_back;

	cout<<"energy_in= "<<energy_in<<" "<<100*energy_in/sum<<"%"<<endl;
	cout<<"energy_skin= "<<energy_skin<<" "<<100*energy_skin/sum<<"%"<<endl<<endl;
	cout<<"energy_front= "<<energy_front<<" "<<100*energy_front/sum<<"%"<<endl;
	cout<<"energy_back= "<<energy_back<<" "<<100*energy_back/sum<<"%"<<endl<<endl;


	cout<<"sum= "<<sum<<endl;
	cout<<"energy_in_front= "<<energy_in_front<<" "<<100*energy_in_front/sum<<"%"<<endl;
	cout<<"energy_skin_front= "<<energy_skin_front<<" "<<100*energy_skin_front/sum<<"%"<<endl;
	cout<<"energy_in_back= "<<energy_in_back<<" "<<100*energy_in_back/sum<<"%"<<endl;
	cout<<"energy_skin_back= "<<energy_skin_back<<" "<<100*energy_skin_back/sum<<"%"<<endl;

	//cout<<"Total Energy In="<<hIn_energy->Integral(0,1100)<<endl;
	//cout<<"Total Energy Skin="<<hSkin_energy->Integral(0,1100)<<endl;
	//cout<<"Back Energy="<<hz_Backenergy->Integral(0,1100)<<endl;
	//cout<<"Front Energy="<<hz_Frontenergy->Integral(0,1100)<<endl;
 
TCanvas* c1 = new TCanvas("c1","Cherry Energy",0,0,400,400);                     
  c1->SetFillColor(0);
  c1->SetFrameFillColor(0);
  h3->GetXaxis()->SetTitle("Energy (MeV)");
  h3->GetYaxis()->SetTitle("Counts");
	h3->Draw(""); 
	h3->SetLineColor(2);

  
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
  hx_energy-> Draw("colz"); 		
  hx_energy->GetXaxis()->SetTitle("x (cm)");
  hx_energy->GetYaxis()->SetTitle("Energy (Mev)");

	TCanvas* c12 = new TCanvas("Front & Back Energy","Front Energy",0,0,400,800);
  c12->SetFillColor(0);
  c12->SetFrameFillColor(0);           
  hz_Frontenergy-> Draw(""); 	c12->SetLogy();	
	hz_Frontenergy->SetLineColor(9);
  hz_Frontenergy->GetXaxis()->SetTitle("Energy (Mev)");
  hz_Frontenergy->GetYaxis()->SetTitle("Counts");
	hz_Backenergy-> Draw("same"); 
	hz_Backenergy->SetLineColor(7);
	//////////////////////////////////
  TLegend *leg3 = new TLegend(0.3,0.8,0.6,0.9);
  leg3->AddEntry(hz_Frontenergy, "Front Energy", "pl");
	leg3->AddEntry(hz_Backenergy, "Back Energy", "pl");
  leg3->SetFillColor(0);
  leg3->Draw();
	/////////////////////////////////

	TCanvas* c13 = new TCanvas("Skin & In Energy","Skin & In Energy",0,0,400,800);
  c13->SetFillColor(0);
  c13->SetFrameFillColor(0);           
  hSkin_energy-> Draw(""); c13->SetLogy();		
  hSkin_energy->SetLineColor(8);
  //hSkin_energy->SetFillColorAlpha(kGreen, 0.1);
  hSkin_energy->GetXaxis()->SetTitle("Energy (Mev)");
  hSkin_energy->GetYaxis()->SetTitle("Counts");
	hIn_energy-> Draw("same"); 
	hIn_energy->SetLineColor(2);
	//hIn_energy->SetFillColorAlpha(kRed, 0.1);
	//////////////////////////////////
  TLegend *leg4 = new TLegend(0.3,0.8,0.6,0.9);
  leg4->AddEntry(hIn_energy, "In Energy", "pl");
	leg4->AddEntry(hSkin_energy, "Skin Energy", "pl");
  leg4->SetFillColor(0);
  leg4->Draw();

	TCanvas* c14 = new TCanvas("Different Parts","Different Parts",0,0,400,800);
  c14->SetFillColor(0);
  c14->SetFrameFillColor(0);           
  hSkin_front-> Draw("");	c14->SetLogy(); 		
  hSkin_front->SetLineColor(kGreen+2);
  hSkin_front->GetXaxis()->SetTitle("Energy (Mev)");
  hSkin_front->GetYaxis()->SetTitle("Counts");
	hSkin_back-> Draw("same"); 
	hSkin_back->SetLineColor(8);
	hIn_front-> Draw("same"); 
	hIn_front->SetLineColor(2);
	hIn_back-> Draw("same"); 
	hIn_back->SetLineColor(kOrange);
	//////////////////////////////////
  TLegend *leg5 = new TLegend(0.3,0.8,0.6,0.9);
  leg5->AddEntry(hSkin_front, "Skin front", "pl");
	leg5->AddEntry(hSkin_back, "Skin back", "pl");
	leg5->AddEntry(hIn_front, "In front", "pl");
	leg5->AddEntry(hIn_back, "In back", "pl");
  leg5->SetFillColor(0);
  leg5->Draw();

}
