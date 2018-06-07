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

void checkResults_Water_AsAcherry() {


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
/*TH1F* h1   = new TH1F("h1","Primary PDG Code",60,-30,30);
  TH1F* h1_2 = new TH1F("h1_2","Secondaries PDG Code",60,-30,30);
  TH1F* h2   = new TH1F("h2","Primary Energy",1100,0,11);
	TH1F* h2_2 = new TH1F("h2_2","Gamma Energy",1100,0,11);
  TH1F* h3   = new TH1F("h3","Cherry Total Energy",1000,0,11);
  TH1F* hx   = new TH1F("hx","X de impacto",600,-6,6);
 	TH1F *hxdif = new TH1F("hxdiff","hxdiff",500,-100,100);
  TH1F *hydif = new TH1F("hydiff","hydiff",500,-100,100);
*/
	TH1F* h3   = new TH1F("h3","Cherry Total Energy",1000,0,6);

	TH1F *hz_Frontenergy = new TH1F("hz_Frontenergy","Front and Back Energy",1100,0,11);
	TH1F *hz_Backenergy  = new TH1F("hz_Backenergy","Back Energy",1100,0,11);
	TH1F *hSkin_energy = new TH1F("hSkin_energy","Energy Skin deposited on the skin",1100,0,11);
	TH1F *hIn_energy  = new TH1F("hIn_energy","In and Skin Energy deposited in the Cherry",1100,0,11);

	TH1F *hSkin_front = new TH1F("hSkin_front","Skin front: Energy",1100,0,11);
	TH1F *hSkin_back  = new TH1F("hSkin_back","Skin back: Energy",1100,0,11);
	TH1F *hIn_front  = new TH1F("hIn_front","In front: Energy",1100,0,11);
	TH1F *hIn_back  = new TH1F("hIn_back","In back: Energy",1100,0,11);

	TH1F* h_Z_weightenergy = new TH1F("h_Z_weightenergy","Z weight Energy",40,50,70);
	TH2F* h_Z_energy  = new TH2F("h_Z_energy","Energy Profiling in Z",40,50,70,1100,0,11);
  
/*TH1F* hz_in_weightenergy = new TH1F("hz_in_weightenergy","Z_in weight Energy",200,50,70);
	TH2F* hzx_in  = new TH2F("hzx_in","IN: Depth Profiling", 200,50,70,200,-20,20);
	TH2F* hz_in_energy  = new TH2F("hz_in_energy","IN: Energy Profiling in z axis", 200,50,70,1100,0,11);	
	TH1F* hz_out_weightenergy = new TH1F("hz_out_weightenergy","Z_out weight Energy",200,50,70);
	TH2F* hzx_out  = new TH2F("hzx_out","OUT: Depth Profiling", 200,50,70,200,-20,20);
	TH2F* hz_out_energy  = new TH2F("hz_out_energy","OUT: Energy Profiling in z axis", 200,50,70,1100,0,11);
	TH1F* hz_diff_weightenergy = new TH1F("hz_diff_weightenergy","Z_diff weight Energy",200,50,70);
	TH2F* hzx_diff  = new TH2F("hzx_diff","DIFF: Depth Profiling", 200,50,70,200,-20,20);
	TH2F* hz_diff_energy  = new TH2F("hz_diff_energy","DIFF: Energy Profiling in z axis", 200,50,70,1100,0,11);
	TH2F* hz_diff_energy_2  = new TH2F("hz_diff_energy_2","DIFF_2: Energy Profiling in z axis", 400,-10,10,1100,0,11);
	TH1F* hz_diff_weightenergy = new TH1F("hz_diff_weightenergy","Z_diff weight Energy",200,-1,1);
	TH2F* hzx_diff  = new TH2F("hzx_diff","DIFF: Depth Profiling", 200,-1,1,200,-1,1);
	TH2F* hz_diff_energy  = new TH2F("hz_diff_energy","DIFF: Energy Profiling in z axis", 200,-1,1,1100,0,11);
	TH3F* hxyz = new TH3F("hxyz","Collides 3D", 80,-20,20,80,-20,20,80,50,70);  
	TH3F* hxyz_energy = new TH3F("hxyz_energy","Collides 3D and Energy", 80,-20,20,80,-20,20,80,50,70);
*/
  
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
  
  Float_t X1_in=0.,X2_in=0.,X3_in=0.,X4=0.,Xup=0.,Xdown=0.;
	Float_t X1_out=0.,X2_out=0.,X3_out=0.;
	Float_t X1_diff=0.,X2_diff=0.,X3_diff=0.;

	Double_t radius;
	Double_t Eratio;
	Double_t x1, x2, x3; 
	Double_t xx, yy, zz;  	
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
				//h1->Fill(track[h]->GetPdgCode());
				//h2->Fill(track[h]->GetEnergy()*1000);//MeV
      } else {
				//h1_2->Fill(track[h]->GetPdgCode());
							if (track[h]->GetPdgCode()==22){
								//h2_2->Fill(track[h]->GetEnergy()*1000);//MeV
							}
      }
    }
    
	//LOOP in hpgeHits-------------------------------------------------------
    for(Int_t h=0;h<hpgeHitsPerEvent;h++){
      charge = hpgeHit[h]->GetEnergy()*1000;
    
      //h3->Fill(charge);
    }
    

		//LOOP in HPGePoints
		for(Int_t r=0;r<hpgePointsPerEvent;r++) {     

					X1_out=hpgePoint[r]->GetXOut();
					X2_out=hpgePoint[r]->GetYOut();
					X3_out=hpgePoint[r]->GetZOut();

					X1_in=hpgePoint[r]->GetXIn();
					X2_in=hpgePoint[r]->GetYIn();
					X3_in=hpgePoint[r]->GetZIn();

					//Change these coordinates in order to pass from RSLab to RSCherry
					//CenterCherry (0,0,58.5)
					xx=0.;
					yy=0.;
					zz=58.5;
					
					x1=abs(X1_out-xx);
					x2=abs(X2_out-yy);
					x3=abs(X3_out-zz);

					radius=sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));

					//Total Cherry
					if (radius<=1.5){
						h_Z_weightenergy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);
						h_Z_energy->Fill(X3_out,hpgePoint[r]->GetEnergyLoss()*1000);
					
						h3->Fill(hpgePoint[r]->GetEnergyLoss()*1000);

						//Differences between Skin & Innerpart
						if (radius<1.45){//IN 0.5mm->1.45 100micro->1.49
							hIn_energy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);
							energy_in=energy_in+hpgePoint[r]->GetEnergyLoss()*1000;
								if (X3_out<zz){
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
								if (X3_out<zz){
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
						if (X3_out<zz){
							hz_Frontenergy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//MeV Front side of cherry
							energy_front=energy_front+hpgePoint[r]->GetEnergyLoss()*1000;
						}else{
							hz_Backenergy->Fill(hpgePoint[r]->GetEnergyLoss()*1000);//MeV Back side
							energy_back=energy_back+hpgePoint[r]->GetEnergyLoss()*1000;
						}

					}

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
  }
 // END LOOP IN THE EVENTS---------------------------------------------------------

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

 
	TCanvas* c1 = new TCanvas("c1","Cherry Energy Hit",0,0,400,400);                     
  c1->SetFillColor(0);
  c1->SetFrameFillColor(0);
  h3->GetXaxis()->SetTitle("Energy (MeV)");
  h3->GetYaxis()->SetTitle("Counts");
	h3->Draw(""); c1->SetLogy();
	h3->SetLineColor(2);

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
	//TFile *f1 = new TFile ("Histo_smallWaterBox_SemiSphere.root","RECREATE");
  //h_Z_weightenergy-> Write(""); 


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
  hIn_energy-> Draw(""); c13->SetLogy();		
  hIn_energy->SetLineColor(8);
  //hSkin_energy->SetFillColorAlpha(kGreen, 0.1);
  hIn_energy->GetXaxis()->SetTitle("Energy (Mev)");
  hIn_energy->GetYaxis()->SetTitle("Counts");
	hSkin_energy-> Draw("same"); 
	hSkin_energy->SetLineColor(2);
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
  hIn_front-> Draw("");	c14->SetLogy(); 		
  hIn_front->SetLineColor(kGreen+2);
  hIn_front->GetXaxis()->SetTitle("Energy (Mev)");
  hIn_front->GetYaxis()->SetTitle("Counts");
	hIn_back-> Draw("same"); 
	hIn_back->SetLineColor(8);
	hSkin_front-> Draw("same"); 
	hSkin_front->SetLineColor(2);
	hSkin_back-> Draw("same"); 
	hSkin_back->SetLineColor(kOrange);
	//////////////////////////////////
  TLegend *leg5 = new TLegend(0.3,0.8,0.6,0.9);
  leg5->AddEntry(hSkin_front, "Skin front", "pl");
	leg5->AddEntry(hSkin_back, "Skin back", "pl");
	leg5->AddEntry(hIn_front, "In front", "pl");
	leg5->AddEntry(hIn_back, "In back", "pl");
  leg5->SetFillColor(0);
  leg5->Draw();		

}
