void results() {


	//ROOT ENVIRONMENT
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(0);

	//INPUT FILE
 	char inputFile[250] = "tragsim.root";
	TFile *file1 = TFile::Open(inputFile);

	//READING TREE
	TTree* tree = (TTree*)file1->Get("cbmsim");

	//HISTOGRAMS DEFINITION
        TH1F* hIDp     = new TH1F("hIDp","Primary PDG Code",250,-20,230);
        TH1F* hIDs     = new TH1F("hIDs","Secondaries PDG Code",41,-20,20);
        TH1F* hEn      = new TH1F("hEn","Primary Energy",200,0,2500);
	TH1F* hChar    = new TH1F("hChar","Rpc Charge",200,0,30e-06);
	TH1F* hTheta   = new TH1F("hTheta","Deflection Angle",1000,-2,2);
        TH1F *hx       = new TH1F("hx","Primary X-coordinate",1000,-100,100);
        TH1F *hy       = new TH1F("hy","Primary Y-coordinate",500,-100,100);
	TH2F *hxy      = new TH2F("hxy", "Primary X-Y impacts",500, -100, 100, 500, -100, 100);
	TH1F* hThetae  = new TH1F("hThetae","Deflection angle of secondary electrons",1000,-2,2);
        TH1F *hxe      = new TH1F("hxe","X-Coordinate of secondary electrons",1000,-100,100);
        TH1F *hye      = new TH1F("hye","Y-Coordinate of secondary electrons",500,-100,100);
	TH2F *hxye     = new TH2F("hxye", "X-Y Impacts of secondary electrons",500, -100, 100, 500, -100, 100);
        TH1F* hThetap  = new TH1F("hThetap","Deflection angle of secondary positrons",1000,-2,2);
        TH1F *hxp      = new TH1F("hxp","X-Coordinate of secondary positrons",1000,-100,100);
        TH1F *hyp      = new TH1F("hyp","X-Coordinate of secondary positrons",500,-100,100);
        TH2F *hxyp     = new TH2F("hxyp", "X-Y Impacts of secondary positrons",500, -100, 100, 500, -100, 100);
        TH1F *hzs      = new TH1F("hzs","Z-Coordinate of secondary electrons",1000,0,160);
	TH1F *hzp      = new TH1F("hzp","Z-Coordinate of secondary positrons",1000,0,160);
	TH2F **densityMap;
        densityMap = new TH2F*[50];
        char title1[250];
        for(Int_t i=0;i<50;i++) {
          sprintf(title1,"%s_%i","DensityMapAtLevel",i);
          densityMap[i] = new TH2F(title1,title1,200,-100,100,200,-100,100);
        }

	TH1F* htime    = new TH1F("htime","Time",200,0,10);

	//----   MCTrack (input)   -------------------------------------------------
	TClonesArray* MCTrackCA;
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);

	//Crystal Points (input)   -------------------------------------------------
	TClonesArray* rpcHitCA;
	TraRPCHit** rpcHit;
	rpcHitCA = new TClonesArray("TraRPCHit",5);
	TBranch *branchRpcHit = tree ->GetBranch("RPCHit");
	branchRpcHit->SetAddress(&rpcHitCA );

	TClonesArray* rpcPointCA;
	TraRPCPoint** rpcPoint;
	rpcPointCA = new TClonesArray("TraRPCPoint",5);
	TBranch *branchRpcPoint = tree ->GetBranch("RPCPoint");
	branchRpcPoint->SetAddress(&rpcPointCA );

// Variables -------------------------------------------------------

	Int_t MCtracksPerEvent = 0;
	Int_t rpcHitsPerEvent = 0;
	Int_t rpcPointsPerEvent = 0;
        Double_t charge = 0.0;
	Double_t xcentro = 0;
	Double_t ycentro = 0;
	Double_t xrel=0;
        TVector3 momentum, vector3;
	Float_t x1, x2, x3, theta, time;
	Double_t Xhist = 0;
	Double_t Yhist = 0;
	Double_t random1 = 0;
	Double_t random2 = 0;
        Double_t random3 = 0;
        Double_t random4 = 0;
	Double_t xrandom1=0;
	Double_t yrandom1=0;
	Double_t xrandom2=0;
	Double_t yrandom2=0;
	TRandom3 *ran = new TRandom3();

// ---------------------------------------------------------------

	//TREE ENTRIES
	Long64_t nevents = tree->GetEntries();


	//LOOP IN THE EVENTS
	for(int i=0;i<nevents;i++){
	    	if(i%1== 100) printf("Event:%i\n",i);
		charge = 0.;
		tree->GetEvent(i);

		MCtracksPerEvent   = MCTrackCA->GetEntries();
		rpcHitsPerEvent    = rpcHitCA->GetEntries();
		rpcPointsPerEvent  = rpcPointCA->GetEntries();

		if(MCtracksPerEvent>0) {
			track = new EnsarMCTrack*[MCtracksPerEvent];
			for(Int_t j=0;j<MCtracksPerEvent;j++){
				track[j] = new EnsarMCTrack;
				track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
			}
		}
		if(rpcHitsPerEvent>0) {
			rpcHit = new TraRPCHit*[rpcHitsPerEvent];
			for(Int_t j=0;j<rpcHitsPerEvent;j++){
				rpcHit[j] = new TraRPCHit;
				rpcHit[j] = (TraRPCHit*) rpcHitCA->At(j);
			}
		}
		if(rpcPointsPerEvent>0) {
			rpcPoint = new TraRPCPoint*[rpcPointsPerEvent];
			for(Int_t j=0;j<rpcPointsPerEvent;j++){
				rpcPoint[j] = new TraRPCPoint;
				rpcPoint[j] = (TraRPCPoint*) rpcPointCA->At(j);
			}
		}
		//LOOP in MC mother tracks
		for(Int_t h=0;h<MCtracksPerEvent;h++){
			if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
				hIDp->Fill(track[h]->GetPdgCode());
				hEn->Fill(track[h]->GetEnergy()*1000-track[h]->GetMass()*1000);
                        } else {
                                hIDs->Fill(track[h]->GetPdgCode());
                        }
		}


		//LOOP in rpcHits

		for(Int_t h=0;h<rpcHitsPerEvent;h++){
			charge = rpcHit[h]->GetCharge();
			hChar->Fill(charge);
			if(rpcHit[h]->GetPlane()==1){
				for(Int_t j=0; j<rpcHitsPerEvent; j++){
					if(rpcHit[j]->GetPlane()==2){
						for(Int_t zlevel=0; zlevel<50;zlevel++){

		// Instead of considering the center of the cell, we create a random hit point inside the same cell
				random1 = ran->Uniform(0.0,1.0);
				random2 = ran->Uniform(0.0,1.0);
				random3 = ran->Uniform(0.0,1.0);
				random4 = ran->Uniform(0.0,1.0);
				xrandom1 = rpcHit[h]->GetCellXCenter()-6.3+12.6*random1;
				yrandom1 = rpcHit[h]->GetCellYCenter()-6.15+12.3*random2;
				xrandom2 = rpcHit[j]->GetCellXCenter()-6.3+12.6*random3;
				yrandom2 = rpcHit[j]->GetCellYCenter()-6.15+12.3*random4;
				Xhist=xrandom1+(xrandom1-xrandom2)/(-90.0)*zlevel*5;
				Yhist=yrandom1+(yrandom1-yrandom2)/(-90.0)*zlevel*5;
				densityMap[zlevel]->Fill(Xhist,Yhist);
				time = rpcHit[j]->GetTime();
				htime->Fill(time);

				}
			    }
			}
		    }
		}



// --------------------------------------------------------------------------
//                         LOOP in MC mother tracks


/* We fill a vector that stores the TrackID values and checks that it was not already used. This
avoids the double counting problem (i.e. the same track produces only one hit) */
vector <Int_t>Disc ;

                for(Int_t r=0;r<rpcPointsPerEvent;r++) {
			Int_t tID= rpcPoint[r]->GetTrackID();
			Int_t MID;
			MID= track[tID]-> GetMotherId();

		  	Bool_t exist=kFALSE;

			for(Int_t h=0; h< Disc.size();h++){

				if (Disc[h]==tID) {exist=kTRUE;
					break;
				}
			}
			if(exist)
				continue;
			Disc.push_back(tID);

			if(MID<0){


			    if(rpcPoint[r]->GetRPCId()==13) {
				    x1=rpcPoint[r]->GetXIn();
				    x2=rpcPoint[r]->GetYIn();
				    x3=rpcPoint[r]->GetZIn();
				    hx->Fill(x1);
				    hy->Fill(x2);
		                    hxy->Fill(x1,x2);
				   // If the y is <0 we define the "theta" angle as negative
                                    theta=TMath::ATan(x2/x3);
      			            hTheta->Fill(theta);

// This "theta" is not the theta angle in polar coordinates. If we want the real theta angle:
//     rpcPoint[r]->PositionIn(vector3); vector3.Theta();

                      }
                    }




// --------------------------------------------------------------------------
// LOOP for the secondaries:
// Electrons
			if(MID>=0 && track[tID]->GetPdgCode()==11){

			    if(rpcPoint[r]->GetRPCId()==13) {
				    x1=rpcPoint[r]->GetXIn();
				    x2=rpcPoint[r]->GetYIn();
				    x3=rpcPoint[r]->GetZIn();
				    hxe->Fill(x1);
				    hye->Fill(x2);
		                    hxye->Fill(x1,x2);
				    hzs->Fill(x3);
				   // If the y is <0 we define the "theta" as negative
                                    theta=TMath::ATan(x2/x3);
      			            hThetae->Fill(theta);
                      }
                    }

//Positrons

			if(MID>=0 && track[tID]->GetPdgCode()==-11){

			    if(rpcPoint[r]->GetRPCId()==13) {
				    x1=rpcPoint[r]->GetXIn();
				    x2=rpcPoint[r]->GetYIn();
  				    x3=rpcPoint[r]->GetZIn();
				    hxp->Fill(x1);
				    hyp->Fill(x2);
		                    hxyp->Fill(x1,x2);
				    hzp->Fill(x3);
				   // If the y is <0 we define the theta as negative
                                    theta=TMath::ATan(x2/x3);
      			            hThetap->Fill(theta);
                      }
                    }

}

//-----------------------------------------------------------------------------------------------

	}

	// Canvas definition
	TCanvas* c1 = new TCanvas("Coordinates"," x and y coordinates of primaries",0,0,400,800);
	c1->Divide(1,2);
	c1->SetFillColor(0);
	c1->SetFrameFillColor(0);
	c1->cd(1);
	hx->Draw();
        hx->Draw("same");
        hx->SetLineColor(2);
        hy->SetLineColor(4);
	c1->cd(2);
	hy->Draw();

 	TCanvas* c2 = new TCanvas("theta","Primary theta angle",0,0,400,400);
	c2->SetFillColor(0);
	c2->SetFrameFillColor(0);
	Int_t ci;   // for color index setting
   	ci = TColor::GetColor("#000099");
   	hTheta->SetLineColor(ci);
   	hTheta->SetLineWidth(2);
   	hTheta->GetXaxis()->SetTitle("#theta (rad)");
   	hTheta->GetXaxis()->SetLabelSize(0.035);
   	hTheta->GetXaxis()->SetTitleSize(0.035);
   	hTheta->GetYaxis()->SetTitle("Cuentas");
   	hTheta->GetYaxis()->SetLabelSize(0.035);
   	hTheta->GetYaxis()->SetTitleSize(0.035);
   	hTheta->GetZaxis()->SetLabelFont(42);
   	hTheta->GetZaxis()->SetLabelSize(0.035);
   	hTheta->GetZaxis()->SetTitleSize(0.035);
   	hTheta->GetZaxis()->SetTitleFont(42);
   	hTheta->Draw("");

        TCanvas* c3 = new TCanvas("Region","Impact zone of primaries",0,0,400,800);
        c3->SetFillColor(0);
        c3->SetFrameFillColor(0);
        hxy->Draw("colz");

        TCanvas* c4 = new TCanvas("EnerPri","Energy of primaries",0,0,400,800);
        c4->SetFillColor(0);
        c4->SetFrameFillColor(0);
        hEn->Draw();


        TCanvas* c5 = new TCanvas("Primaries","Primaries particles",0,0,400,800);
        c5->SetFillColor(0);
        c5->SetFrameFillColor(0);
        hIDp->Draw();

	TCanvas* c6 = new TCanvas("Secundaries","Total produced secondaries",0,0,400,800);
        c6->SetFillColor(0);
        c6->SetFrameFillColor(0);
        hIDs->Draw();

	TCanvas* c7 = new TCanvas("Angsec", "Theta angle of detected  secundaries",0,0,400,800);
        c7->Divide(1,2);
        c7->SetFillColor(0);
        c7->SetFrameFillColor(0);
        c7->cd(1);
        hThetae->Draw();
        hThetae->Draw("same");
        hThetae->SetLineColor(2);
        hThetap->SetLineColor(4);
        c7->cd(2);
        hThetap->Draw();

	TCanvas* c8 = new TCanvas("Coordsec","Impact coordinates of secundaries",0,0,400,800);
	c8->Divide(2,2,0,0);
	c8->cd(1);
	hxe->Draw();
	c8->cd(2);
        hxp->Draw();
	c8->cd(3);
        hye->Draw();
	c8->cd(4);
        hyp->Draw();

        TCanvas* c9 = new TCanvas("Zelec","Z impact coordinates of secundaries",0,0,400,800);
	c9->Divide(1,2);
        c9->SetFillColor(0);
        c9->SetFrameFillColor(0);
	c9->cd(1);
        hzs->Draw();
	c9->cd(2);
	hzp->Draw();

        TCanvas* c10 = new TCanvas("trazas","Density of tracks",0,0,400,800);
        c10->SetFillColor(0);
        c10->SetFrameFillColor(0);
        c10->Divide(5,10);
        for(Int_t i=0;i<50;i++){
        c10->cd(i+1);
          densityMap[i]->Draw("COLZ");
          }

        TCanvas* c11 = new TCanvas("Charge","Charge in the Rpc",0,0,400,400);
        c11->SetFillColor(0);
        c11->SetFrameFillColor(0);
        Int_t cj;   // for color index setting
        cj = TColor::GetColor("#000099");
        hChar->SetLineColor(cj);
        hChar->SetLineWidth(2);
        hChar->GetXaxis()->SetTitle("Charge (Unknown)");
        hChar->GetXaxis()->SetLabelSize(0.035);
        hChar->GetXaxis()->SetTitleSize(0.035);
        hChar->GetYaxis()->SetTitle("Counts");
        hChar->GetYaxis()->SetLabelSize(0.035);
        hChar->GetYaxis()->SetTitleSize(0.035);
        hChar->GetZaxis()->SetLabelFont(42);
        hChar->GetZaxis()->SetLabelSize(0.035);
        hChar->GetZaxis()->SetTitleSize(0.035);
        hChar->GetZaxis()->SetTitleFont(42);
        hChar->Draw("");

	TCanvas* c12 = new TCanvas("Time","Time",0,0,400,800);
        c12->SetFillColor(0);
        c12->SetFrameFillColor(0);
        htime->Draw();



}
