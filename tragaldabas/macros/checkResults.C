
void checkResults() {


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
        TH1F* h1   = new TH1F("h1","Primary PDG Code",250,-20,230);
        TH1F* h1_2 = new TH1F("h1_2","Secondaries PDG Code",250,-20,230);
        TH1F* h2   = new TH1F("h2","Primary Energy",200,0,2500);
	TH1F* h3   = new TH1F("h3","Rpc Charge",200,0,30e-06);
	TH1F* h4   = new TH1F("h4","Theta",200,-2,2);
        TH1F *hx   = new TH1F("hx","X de impacto",1000,-100,100);
	TH2F* hxy = new TH2F("hxy","Impactos", 500, -100, 100, 500, -100, 100);

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

	Int_t MCtracksPerEvent = 0;
	Int_t rpcHitsPerEvent = 0;
	Int_t rpcPointsPerEvent = 0;
        Double_t charge = 0.0;

        TVector3 momentum, vector3;
        Float_t X1=0.,X2=0.,X3=0.,X4=0.,Xup=0.,Xdown=0.;
        
        TH1F *hxdif = new TH1F("hxdiff","hxdiff",500,-100,100);
        TH1F *hydif = new TH1F("hydiff","hydiff",500,-100,100);

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
				h1->Fill(track[h]->GetPdgCode());
				h2->Fill(track[h]->GetEnergy()*1000-track[h]->GetMass()*1000);
                        } else {
                          h1_2->Fill(track[h]->GetPdgCode());
                        }
		}
		//LOOP in rpcHits
		for(Int_t h=0;h<rpcHitsPerEvent;h++){
			charge = rpcHit[h]->GetCharge();
			h3->Fill(charge);
		}

                // --------------------------------------------------------------------------
		//LOOP in MC mother tracks
                for(Int_t h=0;h<MCtracksPerEvent;h++) {

                  if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1

                    //track[h]->GetMomentum(momentum);

                    for(Int_t r=0;r<rpcPointsPerEvent;r++) { //LOOP in rpcHits for each MCTrack
                      //if(rpcHit[r]->GetRPCId()>4000&&rpcHit[r]->GetRPCId()<5000)   X1 = momentum.X();
//cout << "RPC ID: " << rpcPoint[r]->GetRPCId() <<" ------------ " << endl;
//rpcPoint[r]->Print();
			    if(rpcPoint[r]->GetRPCId()==13) {
				    hxdif->Fill(rpcPoint[r]->GetXIn());
				    hydif->Fill(rpcPoint[r]->GetYIn());
				    rpcPoint[r]->PositionIn(vector3);
				    cout << " ---- " << vector3.Theta() << endl ;
			            h4->Fill(vector3.Phi());    
				   X1=rpcPoint[r]->GetXIn();
				   hx->Fill(X1);
                                   X2=rpcPoint[r]->GetYIn();
                                   X3=rpcPoint[r]->GetZIn();
                                   hx->Fill(X1);
                                  // hx->Fill(rpcPoint[r]->GetXIn());
                                  //  hy->Fill(rpcPoint[r]->GetXIn());
                                  //  hxy->Fill(rpcPoint[r]->GetXIn(),rpcPoint[r]->GetYIn());
                                   hxy->Fill(X1,X2);

				   
	}
                  //    if(rpcHit[r]->GetRPCId()>8000&&rpcHit[r]->GetRPCId()<9000)   X2 = momentum.X();
                     // if(rpcHit[r]->GetRPCId()>12000&&rpcHit[r]->GetRPCId()<13000) X3 = momentum.X();
                     // if(rpcHit[r]->GetRPCId()>13000)                              X4 = momentum.X();
                    }

                   // Xup   = (X1+X2)/2;
                   // Xdown = (X3+X4)/2;


                  }

                }
                // --------------------------------------------------------------------------

	}

                     Xup=hx->GetRMS();
                     cout << "RMS" << Xup << endl;


	TCanvas* c1 = new TCanvas("MCTrack","MCTrack",0,0,400,800);
	c1->Divide(1,2);
	c1->SetFillColor(0);
	c1->SetFrameFillColor(0);
	c1->cd(1);
	h1_2->Draw();
        h1->Draw("same");
        h1->SetLineColor(2);
        h1_2->SetLineColor(4);
	c1->cd(2);
	h2->Draw();

	TCanvas* c2 = new TCanvas("c2","Charge in the Rpc",0,0,400,400);
	c2->SetFillColor(0);
	c2->SetFrameFillColor(0);
	Int_t ci;   // for color index setting
   	ci = TColor::GetColor("#000099");
   	h3->SetLineColor(ci);
   	h3->SetLineWidth(2);
   	h3->GetXaxis()->SetTitle("Charge (Unknown)");
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
        TCanvas* c5 = new TCanvas("Region","Region de impactos",0,0,400,800);
        c5->SetFillColor(0);
        c5->SetFrameFillColor(0);
	hxy-> Draw("colz");

TCanvas* c6 = new TCanvas("theta","Angulo theta",0,0,400,400);
h4->Draw("");
}
