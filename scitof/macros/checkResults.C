#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <TRandom.h>

TRandom *rGen = new TRandom();

Double_t diffuseEnergy(double rawEnergy){
  // Diffuses the energy in the crystal in an gaussian way...
  // Trying to simulate the intrinsic crystal resolution
  // The formula is   TF1("f1","0.058*x/sqrt(x)",0,10) for 4% at 1MeV 
  //
  //  ( % * energy ) / sqrt( energy )
  //
  // and then the % is given at 1 MeV!!
 
    Double_t crystalResolution = 4;//CsI:Na=7.88% @ 1MeV (Dato lab Santiago), CeBr_{3}=4% @ 1MeV (Dato laboratorio Santiago), CeF3=14% @ 1MeV (Dato paper W.W. Moses)
    double randomIs = rGen->Gaus(0,rawEnergy*crystalResolution/(235*sqrt(rawEnergy)));
    return rawEnergy + randomIs;
}
void checkResults() {

	// ----  Load libraries   -------------------------------------------------
	gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
	basiclibs();

	gSystem->Load("libGenVector");	
	gSystem->Load("libGeoBase");
	gSystem->Load("libParBase");
	gSystem->Load("libBase");
	gSystem->Load("libMCStack");
	//gSystem->Load("libField");
	gSystem->Load("libGen");

	//SPECIFIC ENSAR LIBRARIES
	gSystem->Load("libEnsarbase");
	gSystem->Load("libEnsarData");
	gSystem->Load("libEnsarMyDet");

	//ENVIRONMENT
	//gROOT->SetStyle("Default");
	//gStyle->SetOptTitle(0);
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
	char inputFile[250] = "simout.root";
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* TCrystal = (TTree*)file1->Get("ensartree");

	//HISTOGRAMS DEFINITION
	TH1F* h1 = new TH1F("h1","Primary PDG Code",100,-100,100);
	TH1F* h2 = new TH1F("h2","Primary Energy",366,0,55);
	TH1F* h3 = new TH1F("h3","Energy deposited",4200,0,600);//700,0,35
	
	//----   MCTrack(input)   -------------------------------------------------
	TClonesArray* MCTrackCA;  
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = TCrystal ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);

	//Crystal Points (input)
	TClonesArray* myDetPointCA;  
	EnsarMyDetPoint** myDetPoint;
	myDetPointCA = new TClonesArray("EnsarMyDetPoint",5);
	TBranch *branchCrystalPoint = TCrystal ->GetBranch("MyDetPoint");
	branchCrystalPoint->SetAddress(&myDetPointCA );	

	Int_t MCtracksPerEvent = 0;
	Int_t myDetPointsPerEvent = 0;
	double energy = 0.0;

	//TREE ENTRIES	
	Long64_t nevents = TCrystal->GetEntries();

	Float_t etotal=0.;

	//LOOP IN THE EVENTS
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);
		energy = 0;
		etotal=0;
		TCrystal->GetEvent(i);

		MCtracksPerEvent = MCTrackCA->GetEntries();
		myDetPointsPerEvent = myDetPointCA->GetEntries(); 
			
		if(MCtracksPerEvent>0) {
			track = new EnsarMCTrack*[MCtracksPerEvent];
			for(Int_t j=0;j<MCtracksPerEvent;j++){
				track[j] = new EnsarMCTrack;
				track[j] = (EnsarMCTrack*) MCTrackCA->At(j);      
			}
		}
		if(myDetPointsPerEvent>0) {
			myDetPoint = new EnsarMyDetPoint*[myDetPointsPerEvent];
			for(Int_t j=0;j<myDetPointsPerEvent;j++){
				myDetPoint[j] = new EnsarMyDetPoint;
				myDetPoint[j] = (EnsarMyDetPoint*) myDetPointCA->At(j);      
			}
		}
		//LOOP in MC mother tracks
		for(Int_t h=0;h<MCtracksPerEvent;h++){
			if(track[h]->GetMotherId()<0){
				h1->Fill(track[h]->GetPdgCode());
				h2->Fill(track[h]->GetEnergy()*1000-track[h]->GetMass()*1000);
			}
		}
		//LOOP in myDetPoints
		for(Int_t h=0;h<myDetPointsPerEvent;h++){
			//energy = sqrt(myDetPoint[h]->GetPx()*myDetPoint[h]->GetPx()+myDetPoint[h]->GetPy()*myDetPoint[h]->GetPy()+myDetPoint[h]->GetPz()*myDetPoint[h]->GetPz())*1000;
			energy = diffuseEnergy(myDetPoint[h]->GetEnergyLoss()*1000);
			etotal += energy;
			//h3->Fill(energy);
			//cout<< "number= "<<h<< " energy= " << energy << endl;
		}
	//cout<< "event= "<<i<< " energy= " << etotal << endl;
	h3->Fill(etotal);
	}

	/*TCanvas* c1 = new TCanvas("pdg_2MeV","MCTrack 2MeV",0,0,400,800);
	c1->Divide(1,2);
	c1->SetFillColor(0);
	c1->SetFrameFillColor(0);
	c1->cd(1);
	h1->Draw();
	c1->cd(2);
	h2->Draw()*/;

	TCanvas* c2 = new TCanvas("Energy_deposited_2MeV","Energy deposited in CsI:Na+Al5mm",0,0,400,400);
   	c2->SetFillColor(0);
   	c2->SetBorderMode(0);
   	c2->SetBorderSize(2);
   	c2->SetFrameBorderMode(0);
   	c2->SetFrameLineWidth(2);
   	c2->SetFrameBorderMode(0);
   
	c2->SetFillColor(0);
	c2->SetFrameFillColor(0);
	Int_t ci;   // for color index setting
   	ci = TColor::GetColor("#000099");
	h3->SetTitle("#gamma #rightarrow Al+CsI:Na  P = 2MeV/c");
   	h3->SetLineColor(ci);
   	h3->SetLineWidth(2);
   	h3->GetXaxis()->SetTitle("E_{dep}(MeV)");
   	h3->GetXaxis()->SetRange(3,61);
   	h3->GetXaxis()->SetLabelSize(0.035);
   	h3->GetXaxis()->SetTitleSize(0.035);
   	h3->GetXaxis()->SetTitleFont(62);// X Title
	h3->GetXaxis()->SetLabelFont(62); // Axis numbers
	h3->GetXaxis()->SetRangeUser(0.05,2.5);


   	h3->GetYaxis()->SetTitle("Counts");
   	h3->GetYaxis()->SetLabelSize(0.035);
   	h3->GetYaxis()->SetTitleSize(0.035);
	h3->GetYaxis()->SetTitleFont(62);
   	h3->GetYaxis()->SetLabelFont(62);

   	h3->GetZaxis()->SetLabelSize(0.035);
   	h3->GetZaxis()->SetTitleSize(0.035);
   	h3->GetZaxis()->SetTitleFont(62);
	
   	h3->Draw("");


}
