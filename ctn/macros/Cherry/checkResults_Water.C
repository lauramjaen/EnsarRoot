////////////////////////////////////////////////////////////////////////////
////																	
////		--- Simulation of Cherry Tomato ---				
////																	
////			Macro to check the Results of 10cm Water Box 					
////																	
////		Usage:														
////			1st: select the root file & the ranges (OPTIONAL)		
////			2nd: root -l checkResults_Water.C								
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

void checkResults_Water() {


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


	TH1F* h_Z_weightenergy = new TH1F("h_Z_weightenergy","Z weight Energy",100,50,70);//100
	TH2F* h_Z_energy  = new TH2F("h_Z_energy","Energy Profiling in Z",100,50,70,1100,0,11);//100

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
  
  TVector3 momentum;
  Float_t X1_in=0.,X2_in=0.,X3_in=0.,X4=0.,Xup=0.,Xdown=0.;
	Float_t X1_out=0.,X2_out=0.,X3_out=0.;
	Float_t X1_diff=0.,X2_diff=0.,X3_diff=0.;
	Double_t xx, yy, zz;
	Double_t d;
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

					X1_out=hpgePoint[r]->GetXOut();
					X2_out=hpgePoint[r]->GetYOut();
					X3_out=hpgePoint[r]->GetZOut();

					X1_in=hpgePoint[r]->GetXIn();
					X2_in=hpgePoint[r]->GetYIn();
					X3_in=hpgePoint[r]->GetZIn();

					h_Z_weightenergy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);
					h_Z_energy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);

				/* Considering the Energy loss in steps of 0.1mm
					X1_diff=X1_out-X1_in;
					X2_diff=X2_out-X2_in;
					X3_diff=X3_out-X3_in;
					d=sqrt(pow(X1_diff,2)+pow(X2_diff,2)+pow(X3_diff,2));
					
					if (d>0){
								//cout<<"X_out="<<X1_out<<" X_in="<<X1_in<<" X_diff="<<X1_diff<<endl;
								//cout<<"Y_out="<<X2_out<<" Y_in="<<X2_in<<" Y_diff="<<X2_diff<<endl;
								//cout<<"Z_out="<<X3_out<<" Z_in="<<X3_in<<" Z_diff="<<X3_diff<<endl;
								//cout<<"d="<<d<<endl;
								//cout<<"Energy="<<hpgePoint[r]->GetEnergyLoss()*1000<<endl;
						Eratio=hpgePoint[r]->GetEnergyLoss()*100/X3_diff;//MeV/mm						
								//cout<<"Eratio="<<Eratio<<endl;
							
						for(Double_t j=X3_in; j<X3_out; j=j+0.1){//step of 0.1mm, basis unit in cm
							//h_Z_weightenergy->Fill(j,Eratio);
							//h_Z_energy->Fill(j,Eratio);
						}		
					}else{
						h_Z_weightenergy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);
						h_Z_energy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);

					}*/
					
					

					
					/*hz_diff_energy_2->Fill(X3_in-X3_out,hpgePoint[r]->GetEnergyLoss()*1000);//MeV

					hzx_out->Fill(X3_out,X1_out);
					hz_out_energy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);//MeV					
					hz_out_weightenergy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);//z with the energy weight 

					hzx_in->Fill(X3_in,X1_in);
					hz_in_energy->Fill(X3_in,hpgePoint[r]->GetEnergyLoss()*1000);//MeV
					hz_in_weightenergy->Fill(X3_in,hpgePoint[r]->GetEnergyLoss()*1000);//z with the energy weight 


					hzx_diff->Fill(X3_diff,X1_diff);
					hz_diff_energy->Fill(X3_diff,hpgePoint[r]->GetEnergyLoss()*1000);//MeV
					hz_diff_weightenergy->Fill(X3_diff,hpgePoint[r]->GetEnergyLoss()*1000);//z with the energy weight 


					Double_t radius;
					Double_t x1, x2, x3;
					x1=abs(X1-xx);
					x2=abs(X2-yy);
					x3=abs(X3-zz);
					radius=sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
					//cout<<"x="<<X1<<" y="<<X2<<" z="<<X3<<" "<<x3;
					//cout<<"  radius= "<<radius<<endl;
				*/	
			}



 // END LOOP IN THE EVENTS---------------------------------------------------------  

    

  }
 
	/////////////////////////////////

 // END LOOP IN THE EVENTS---------------------------------------------------------


 
/*	TCanvas* c1 = new TCanvas("c1","Cherry Energy Hit",0,0,400,400);                     
  c1->SetFillColor(0);
  c1->SetFrameFillColor(0);
  h3->GetXaxis()->SetTitle("Energy (MeV)");
  h3->GetYaxis()->SetTitle("Counts");
	h3->Draw(""); 
	h3->SetLineColor(9);

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

TH1F* h_Z_weightenergy = new TH1F("h_Z_weightenergy","Z weight Energy",200,50,70);
	TH2F* h_Z_energy  = new TH2F("h_Z_energy","Energy Profiling in Z",200,50,70,1100,0,11);


*/

	TCanvas* c2 = new TCanvas("c2","Z & Energy weight",0,0,400,800);
  c2->SetFillColor(0);
  c2->SetFrameFillColor(0);       
  h_Z_weightenergy-> Draw(""); 		
	h_Z_weightenergy-> SetLineColor(kBlue); 		
  h_Z_weightenergy->GetXaxis()->SetTitle("Z (cm)");
  h_Z_weightenergy->GetYaxis()->SetTitle("Events");

	TCanvas* c3 = new TCanvas("c3","Z vs Energy",0,0,400,800);
  c3->SetFillColor(0);
  c3->SetFrameFillColor(0);       
  h_Z_energy-> Draw("colz"); 				
  h_Z_energy->GetXaxis()->SetTitle("Z (cm)");
  h_Z_energy->GetYaxis()->SetTitle("Energy (MeV)");

	//open a new file
	//TFile *f1 = new TFile ("Histo_Cherry_Layers.root","RECREATE");
	TFile *f1 = new TFile ("Histo_SemiSphere_BackNoActive.root","RECREATE");
  h_Z_weightenergy-> Write(""); 		

}
