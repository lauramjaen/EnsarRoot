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
  char inputFile[250] = "outsim_H2O_e_10MeV.root";   //select root file                                             
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

	/*TH1F *hz_Frontenergy = new TH1F("hz_Frontenergy","Front Energy",100,50,70);
	TH1F *hz_Backenergy  = new TH1F("hz_Backenergy","Back Energy",1100,0,11);
	TH1F *hSkin_energy = new TH1F("hSkin_energy","Energy Skin deposited on the skin",1100,0,11);
	TH1F *hIn_energy  = new TH1F("hIn_energy","Energy In deposited in the Cherry",1100,0,11);

	TH1F *hSkin_front = new TH1F("hSkin_front","Skin front: Energy",1100,0,11);
	TH1F *hSkin_back  = new TH1F("hSkin_back","Skin back: Energy",1100,0,11);
	TH1F *hIn_front  = new TH1F("hIn_front","In front: Energy",1100,0,11);
	TH1F *hIn_back  = new TH1F("hIn_back","In back: Energy",1100,0,11);*/
  
	TH1F* hz_in_weightenergy = new TH1F("hz_in_weightenergy","Z_in weight Energy",200,50,70);
	TH2F* hzx_in  = new TH2F("hzx_in","IN: Depth Profiling", 200,50,70,200,-20,20);
	TH2F* hz_in_energy  = new TH2F("hz_in_energy","IN: Energy Profiling in z axis", 200,50,70,1100,0,11);
	
	TH1F* hz_out_weightenergy = new TH1F("hz_out_weightenergy","Z_out weight Energy",200,50,70);
	TH2F* hzx_out  = new TH2F("hzx_out","OUT: Depth Profiling", 200,50,70,200,-20,20);
	TH2F* hz_out_energy  = new TH2F("hz_out_energy","OUT: Energy Profiling in z axis", 200,50,70,1100,0,11);

	TH1F* hz_diff_weightenergy = new TH1F("hz_diff_weightenergy","Z_diff weight Energy",200,50,70);
	TH2F* hzx_diff  = new TH2F("hzx_diff","DIFF: Depth Profiling", 200,50,70,200,-20,20);
	TH2F* hz_diff_energy  = new TH2F("hz_diff_energy","DIFF: Energy Profiling in z axis", 200,50,70,1100,0,11);

	TH2F* hz_diff_energy_2  = new TH2F("hz_diff_energy_2","DIFF_2: Energy Profiling in z axis", 400,-10,10,1100,0,11);

/*
TH1F* hz_diff_weightenergy = new TH1F("hz_diff_weightenergy","Z_diff weight Energy",200,-1,1);
	TH2F* hzx_diff  = new TH2F("hzx_diff","DIFF: Depth Profiling", 200,-1,1,200,-1,1);
	TH2F* hz_diff_energy  = new TH2F("hz_diff_energy","DIFF: Energy Profiling in z axis", 200,-1,1,1100,0,11);
*/
  
	TH3F* hxyz = new TH3F("hxyz","Collides 3D", 80,-20,20,80,-20,20,80,50,70);  
	TH3F* hxyz_energy = new TH3F("hxyz_energy","Collides 3D and Energy", 80,-20,20,80,-20,20,80,50,70);

  
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
  Float_t X1_in=0.,X2_in=0.,X3_in=0.,X4=0.,Xup=0.,Xdown=0.;
	Float_t X1_out=0.,X2_out=0.,X3_out=0.;
	Float_t X1_diff=0.,X2_diff=0.,X3_diff=0.;
	Double_t xx, yy, zz;
	Double_t r;
	Double_t Eratio;
	  
	//Change these coordinates in order to pass from RSLab to RSCherry
	//CenterCherry (0,0,58.5)
	xx=0.;
	yy=0.;
  zz=80.;//zz=58.5;
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
					X1_out=hpgePoint[r]->GetXOut();
					X2_out=hpgePoint[r]->GetYOut();
					X3_out=hpgePoint[r]->GetZOut();

					X1_in=hpgePoint[r]->GetXIn();
					X2_in=hpgePoint[r]->GetYIn();
					X3_in=hpgePoint[r]->GetZIn();

					X1_diff=X1_out-X1_in;
					X2_diff=X2_out-X2_in;
					X3_diff=X3_out-X3_in;

					r=sqrt(pow(X1_diff,2)+pow(X2_diff,2)+pow(X3_diff,2));
					
					
					
					if (r>0){
						cout<<"X_out="<<X1_out<<" X_in="<<X1_in<<" X_diff="<<X1_diff<<endl;
						cout<<"Y_out="<<X2_out<<" Y_in="<<X2_in<<" Y_diff="<<X2_diff<<endl;
						cout<<"Z_out="<<X3_out<<" Z_in="<<X3_in<<" Z_diff="<<X3_diff<<endl;
						cout<<"r="<<r<<endl;
						Eratio=hpgePoint[r]->GetEnergyLoss()*1000/r;
						cout<<"Eratio="<<Eratio<<endl;
					}

					

					//hx->Fill(X1_out);
					//hxy->Fill(X1_out,X2_out);					
					//hxyz->Fill(X1_out,X2_out,X3_out);
					//hxyz_energy->Fill(X1,X2,X3,hpgePoint[r]->GetEnergyLoss());
					//hx_energy->Fill(X1,hpgePoint[r]->GetEnergyLoss()*1000);//MeV


					hz_diff_energy_2->Fill(X3_in-X3_out,hpgePoint[r]->GetEnergyLoss()*1000);//MeV

					hzx_out->Fill(X3_out,X1_out);
					hz_out_energy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);//MeV					
					hz_out_weightenergy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);//z with the energy weight 

					hzx_in->Fill(X3_in,X1_in);
					hz_in_energy->Fill(X3_in,hpgePoint[r]->GetEnergyLoss()*1000);//MeV
					hz_in_weightenergy->Fill(X3_in,hpgePoint[r]->GetEnergyLoss()*1000);//z with the energy weight 


					hzx_diff->Fill(X3_diff,X1_diff);
					hz_diff_energy->Fill(X3_diff,hpgePoint[r]->GetEnergyLoss()*1000);//MeV
					hz_diff_weightenergy->Fill(X3_diff,hpgePoint[r]->GetEnergyLoss()*1000);//z with the energy weight 


					/*Double_t radius;
					Double_t x1, x2, x3;
					x1=abs(X1-xx);
					x2=abs(X2-yy);
					x3=abs(X3-zz);
					radius=sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));*/
					//cout<<"x="<<X1<<" y="<<X2<<" z="<<X3<<" "<<x3;
					//cout<<"  radius= "<<radius<<endl;

					//Differences between Skin & Innerpart
					/*if (radius<1.49){//IN
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
					}*/

					/*
					//Differences between Front & Back side
					if (X3<zz){
						hz_Frontenergy->Fill(X3,hpgePoint[r]->GetEnergyLoss()*1000);//MeV Front side of cherry
						energy_front=energy_front+hpgePoint[r]->GetEnergyLoss()*1000;
					}else{
						hz_Backenergy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//MeV Back side
						energy_back=energy_back+hpgePoint[r]->GetEnergyLoss()*1000;
					}
					*/
					
			}



 // END LOOP IN THE EVENTS---------------------------------------------------------  

    

  }
 
	/////////////////////////////////

 // END LOOP IN THE EVENTS---------------------------------------------------------

	/*Double_t sum;
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
*/
 
	TCanvas* c1 = new TCanvas("c1","Cherry Energy Hit",0,0,400,400);                     
  c1->SetFillColor(0);
  c1->SetFrameFillColor(0);
  h3->GetXaxis()->SetTitle("Energy (MeV)");
  h3->GetYaxis()->SetTitle("Counts");
	h3->Draw(""); 
	h3->SetLineColor(9);

  
  /*TCanvas* c5 = new TCanvas("Collides_xy","Collides: Front View",0,0,400,800);
  c5->SetFillColor(0);
  c5->SetFrameFillColor(0);
  //hxy-> Draw("colz");            
  hxy-> Draw("lego2z"); 		
  //hxy-> Draw("surf2z");
  hxy->GetXaxis()->SetTitle("x (cm)");
  hxy->GetYaxis()->SetTitle("y (cm)");*/
  

  /*TCanvas* c6 = new TCanvas("theta","Theta Angle",0,0,400,400);
  c6->SetFillColor(0);
  c6->SetFrameFillColor(0);
  h4->Draw("");
  h4->SetLineColor(6);          
  h4->GetXaxis()->SetTitle("#theta (rad)");
  h4->GetYaxis()->SetTitle("Counts");*/
  
  /*TCanvas* c7 = new TCanvas("Collides_xyz","Collides 3D",0,0,400,800);
  c7->SetFillColor(0);
  c7->SetFrameFillColor(0);
  hxyz-> Draw("");
  hxyz->SetMarkerStyle(20); 
  hxyz->SetMarkerSize(0.4); 
  hxyz->SetMarkerColor(2); 
  hxyz->GetXaxis()->SetTitle("x (cm)");
  hxyz->GetYaxis()->SetTitle("y (cm)");
  hxyz->GetZaxis()->SetTitle("z (cm)");*/

/*
hzx_in->Fill(X3_in,X1_in);
hz_in_energy->Fill(X3_in,hpgePoint[r]->GetEnergyLoss()*1000);//MeV
hz_out_weightenergy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);//z with the energy weight 


*/

	TCanvas* c8_out = new TCanvas("Colides_zx_out","Collides Out: Depth Profiling",0,0,400,800);
  c8_out->SetFillColor(0);
  c8_out->SetFrameFillColor(0);
  //hzx-> Draw("colz");            
  hzx_out-> Draw("lego2z"); 		
  //hzx-> Draw("surf2z");
  hzx_out->GetXaxis()->SetTitle("z_out (cm)");
  hzx_out->GetYaxis()->SetTitle("x_out (cm)");

	TCanvas* c8_in = new TCanvas("Colides_zx_in","Collides In: Depth Profiling",0,0,400,800);
  c8_in->SetFillColor(0);
  c8_in->SetFrameFillColor(0);       
  hzx_in-> Draw("lego2z"); 		
  hzx_in->GetXaxis()->SetTitle("z_in (cm)");
  hzx_in->GetYaxis()->SetTitle("x_in (cm)");

//Diff
	TCanvas* c8_diff = new TCanvas("Colides_zx_diff","Collides Diff: Depth Profiling",0,0,400,800);
  c8_diff->SetFillColor(0);
  c8_diff->SetFrameFillColor(0);       
  hzx_diff-> Draw("colz"); 		
  hzx_diff->GetXaxis()->SetTitle("z_diff (cm)");
  hzx_diff->GetYaxis()->SetTitle("x_diff (cm)");

	TCanvas* c15 = new TCanvas("Z_diff_Eweight","Z_diff & Energy weight",0,0,400,800);
  c15->SetFillColor(0);
  c15->SetFrameFillColor(0);       
  hz_diff_weightenergy-> Draw(""); 		
  hz_diff_weightenergy->GetXaxis()->SetTitle("z_diff (cm)");
  hz_diff_weightenergy->GetYaxis()->SetTitle("Events");

	TCanvas* c16 = new TCanvas("Z_diff_Energy","Z_diff & Energy",0,0,400,800);
  c16->SetFillColor(0);
  c16->SetFrameFillColor(0);       
  hz_diff_energy-> Draw("colz"); 		
  hz_diff_energy->GetXaxis()->SetTitle("z_diff (cm)");
  hz_diff_energy->GetYaxis()->SetTitle("Energy (MeV)");

//Out
	TCanvas* c17 = new TCanvas("Z_out_Eweight","Z_out & Energy weight",0,0,400,800);
  c17->SetFillColor(0);
  c17->SetFrameFillColor(0);       
  hz_out_weightenergy-> Draw(""); 		
  hz_out_weightenergy->GetXaxis()->SetTitle("z_out (cm)");
  hz_out_weightenergy->GetYaxis()->SetTitle("Events");

	TCanvas* c18 = new TCanvas("Z_out_Energy","Z_out & Energy",0,0,400,800);
  c18->SetFillColor(0);
  c18->SetFrameFillColor(0);       
  hz_out_energy-> Draw("colz"); 		
  hz_out_energy->GetXaxis()->SetTitle("z_out (cm)");
  hz_out_energy->GetYaxis()->SetTitle("Energy (MeV)");


	TCanvas* c19 = new TCanvas("Z_out_Eweight_Both","Both: Z_out & Energy weight",0,0,400,800);
  c19->SetFillColor(0);
  c19->SetFrameFillColor(0);       
  hz_out_weightenergy-> Draw(""); 		
	hz_out_weightenergy->SetLineColor(kOrange);
  hz_out_weightenergy->GetXaxis()->SetTitle("z (cm)");
  hz_out_weightenergy->GetYaxis()->SetTitle("Events");
	hz_diff_weightenergy-> Draw("same"); 		
	hz_diff_weightenergy->SetLineColor(kGreen);

/*
	TCanvas* c9 = new TCanvas("Colides_xyz_energy","Collides 3D and Energy",0,0,400,800);
  c9->SetFillColor(0);
  c9->SetFrameFillColor(0);
  hzx-> Draw("colz");            
  //hxyz_energy-> Draw("LEGO2"); 		
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
	hz_Frontenergy->SetLineColor(7);
  hz_Frontenergy->GetXaxis()->SetTitle("Energy (Mev)");
  hz_Frontenergy->GetYaxis()->SetTitle("Counts");
	hz_Backenergy-> Draw("same"); 
	hz_Backenergy->SetLineColor(9);
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
  hSkin_energy->SetLineColor(2);
  hSkin_energy->GetXaxis()->SetTitle("Energy (Mev)");
  hSkin_energy->GetYaxis()->SetTitle("Counts");
	hIn_energy-> Draw("same"); 
	hIn_energy->SetLineColor(8);
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
  hSkin_front->SetLineColor(kGreen+4);
  hSkin_front->GetXaxis()->SetTitle("Energy (Mev)");
  hSkin_front->GetYaxis()->SetTitle("Counts");
	hSkin_back-> Draw("same"); 
	hSkin_back->SetLineColor(8);
	hIn_front-> Draw("same"); 
	hIn_front->SetLineColor(46);
	hIn_back-> Draw("same"); 
	hIn_back->SetLineColor(2);
	//////////////////////////////////
  TLegend *leg5 = new TLegend(0.3,0.8,0.6,0.9);
  leg5->AddEntry(hSkin_front, "Skin front", "pl");
	leg5->AddEntry(hSkin_back, "Skin back", "pl");
	leg5->AddEntry(hIn_front, "In front", "pl");
	leg5->AddEntry(hIn_back, "In back", "pl");
  leg5->SetFillColor(0);
  leg5->Draw();
*/
}
