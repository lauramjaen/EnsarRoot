// -----------------------------------------------------------------
// Example of compiled macro used to analyse several root files.
// Loops in material, thickness and energy. 
// It reads histograms and stores information in a .txt file.
// Last modified: 26.01.2017 by Jose Cuenca
//                josejavier.cuenca@usc.es
// To compile: g++ -o Name.exe compiled.C `root-config --cflags --glibs` -I(PathTo)/FairRoot/include -I(PathToExternal)/external/include -L (PathTo)/buildEnsar/lib/ -lTraRPC -lTraRPCGen -lTraRPCData 
//             -lEnsarGen -lEnsarData
//
// ----------------------------------------------------------------- 
#include <iostream>
#include <fstream>
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "/WholePathTo/EnsarRoot/tragaldabas/data/TraRPCPoint.h"
#include "/WholePathTo/EnsarRoot/ensardata/EnsarMCTrack.h"
using namespace std;

// Global variables ----------------------------------------------------
Float_t x=0.0, y=0.0, z=0.0, theta=0.0; // coordinates and angle
Float_t const c=0.03; // speed of light in cm/ps
Float_t dist=0.0, tret=0.0; // distance and time delay
Int_t MCtracksPerEvent = 0;
Int_t rpcPointsPerEvent = 0;

// Labels during analysis:
TString elemento[4]={"Fe","Pb","U", "W"};
TString espesor[4]={"1cm","5cm","10cm","20cm"};
TString energia[7]={"01","02","03","04","05","1","2"};
Float_t pars[4][4][7][11];
// ---------------------------------------------------------------------

// Function that runs the analysis -------------------------------------
void analysis(Float_t* out, TString name) {

	//Input file
	TFile* file1 = TFile::Open(name);

	//Reading the tree
	TTree* tree = (TTree*)file1->Get("cbmsim");

	//Histograms definition
	TH1F hIDp("hIDp","Primary PDG Code",250,-20,230);
	TH1F hIDs("hIDs","Secondaries PDG Code",41,-20,20);
	TH1F hEn("hEn","Primary Energy",200,0,2500);
	TH1F hTheta("hTheta","Primary Scattering Angle",1000,-2,2);
	TH1F hx("hx","Primary X coordinate",1000,-100,100);
	TH1F hy("hy","Primary Y coordinate",1000,-100,100);
	TH2F hxy("hxy", "Primary x-y impacts",1000, -100, 100, 1000, -100, 100);
	TH1F hThetae("hThetae","Secondary electrons scattering angle",1000,-2,2);
	TH1F hxe("hxe","Secondary electrons X coordinate",1000,-100,100);
	TH1F hye("hye","Secondary electrons Y coordinate",1000,-100,100);
	TH2F hxye("hxye", "Secondary electrons x-y impacts",1000, -100, 100, 1000, -100, 100);
	TH1F hThetap("hThetap","Secondary positrons scattering angle",1000,-2,2);
	TH1F hxp("hxp","Secondary positrons X coordinate",1000,-100,100);
	TH1F hyp("hyp","Secondary positrons Y coordinate",1000,-100,100);
	TH2F hxyp("hxyp", "Secondary positrons x-y impacts",1000, -100, 100, 1000, -100, 100);
	TH1F hzs("hzs","Secondary electrons Z coordinate",1000,0,160);
	TH1F hzp("hzp","Secondary positrons Z coordinate",1000,0,160);
	TH1F hret("hret","Primary delay time", 200, 0,30);

	// --- TClonesArrays (inputs) ----------------------------------
	// Tracks:
	TClonesArray* MCTrackCA;
	EnsarMCTrack** track;
	MCTrackCA = new TClonesArray("EnsarMCTrack",5);
	TBranch *branchMCTrack = tree ->GetBranch("MCTrack");
	branchMCTrack->SetAddress(&MCTrackCA);

	// Points:
	TClonesArray* rpcPointCA;
	TraRPCPoint** rpcPoint;
	rpcPointCA = new TClonesArray("TraRPCPoint",5);
	TBranch *branchRpcPoint = tree ->GetBranch("RPCPoint");
	branchRpcPoint->SetAddress(&rpcPointCA );
	// ------------------------------------------------------------

	// Tree entries
	Long64_t nevents = tree->GetEntries();

	// Loop in the events
	for(int i=0;i<nevents;i++){
	  MCTrackCA->Clear();
	  rpcPointCA->Clear();
	  // if(i%100000 == 1) printf("Event:%i\n",i);
	  tree->GetEvent(i);
	  MCtracksPerEvent   = MCTrackCA->GetEntries();
	  rpcPointsPerEvent  = rpcPointCA->GetEntries();

	  // Create the track and rpcPoint objects from TClonesArrays -
	  // track:
	  if(MCtracksPerEvent==0) continue; // If there is no track, we skip this event and no rpcPoint is created
	  track = new EnsarMCTrack*[MCtracksPerEvent];
	    for(Int_t j=0;j<MCtracksPerEvent;j++){
	      //track[j] = new EnsarMCTrack;
	      track[j] = (EnsarMCTrack*) MCTrackCA->At(j);
	    }
	  // rpcPoint:
	  if(rpcPointsPerEvent>0) {
	  rpcPoint = new TraRPCPoint*[rpcPointsPerEvent];
	    for(Int_t j=0;j<rpcPointsPerEvent;j++){
	      //rpcPoint[j] = new TraRPCPoint;
	      rpcPoint[j] = (TraRPCPoint*) rpcPointCA->At(j);
	    }
	  } // --------------------------------------------------------

	// Loop in tracks: ---------------------------------------------
	for(Int_t h=0;h<MCtracksPerEvent;h++){
	  if(track[h]->GetMotherId()<0) { //Primary Particle is MotherId=-1
	    hIDp.Fill(track[h]->GetPdgCode());
	    hEn.Fill(track[h]->GetEnergy()*1000-track[h]->GetMass()*1000);
	    } else {
	    hIDs.Fill(track[h]->GetPdgCode());
	      }
	  }
	// -------------------------------------------------------------

	// Loop in points: ---------------------------------------------
	// The discrimination vector avoids double counting in points:
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
	if(exist) continue;
	Disc.push_back(tID);
	// Mother particles (MID=-1):
	if(MID<0){
	  if(rpcPoint[r]->GetRPCId()==13) {
	  x=rpcPoint[r]->GetXIn();
	  y=rpcPoint[r]->GetYIn();
	  z=rpcPoint[r]->GetZIn();
	  hx.Fill(x);
	  hy.Fill(y);
	  hxy.Fill(x,y);
	  dist=TMath::Sqrt(x*x+y*y+z*z); // distance in cm
	  tret=(dist-z)/c; // delay in ps
	  // If the y is <0 we define the theta as negative
	  theta=TMath::ATan(y/z);
	  hTheta.Fill(theta);
	  hret.Fill(tret);
	  }
        } // End of Mother particles

	// Secondary particles  ----------------------------------------
	// Electrons:
	if(MID>=0 && track[tID]->GetPdgCode()==11){
	  if(rpcPoint[r]->GetRPCId()==13) {
	  x=rpcPoint[r]->GetXIn();
	  y=rpcPoint[r]->GetYIn();
	  z=rpcPoint[r]->GetZIn();
	  hxe.Fill(x);
	  hye.Fill(y);
	  hxye.Fill(x,y);
	  hzs.Fill(z);
	  // If the y is <0 we define the theta as negative
	  theta=TMath::ATan(y/z);
	  hThetae.Fill(theta);
	  }
	}

        // Positrons:
	if(MID>=0 && track[tID]->GetPdgCode()==-11){
	  if(rpcPoint[r]->GetRPCId()==13) {
            x=rpcPoint[r]->GetXIn();
	    y=rpcPoint[r]->GetYIn();
	    z=rpcPoint[r]->GetZIn();
	    hxp.Fill(x);
	    hyp.Fill(y);
	    hxyp.Fill(x,y);
	    hzp.Fill(z);
	    // If the y is <0 we define the theta as negative
	    theta=TMath::ATan(y/z);
	    hThetap.Fill(theta);
	    }
	  }
	} // end of loop in points
	delete[] track;
	if(rpcPointsPerEvent>0) delete[] rpcPoint;
	} // end of loop in events

	// Histograms and curves: -------------------------------------
	// Gaussian fit curve for the "theta" angle
	TF1 f("fit","gaus",-0.5,0.5);
	hTheta.Fit(&f,"QR"," ");

	// Parameters to be printed
	out[0]=hTheta.GetMean();
	out[1]=hTheta.GetRMS();
	out[2]=f.GetParameter(2);
	out[3]=f.GetParError(2);
	out[4]=hTheta.GetEntries();
	out[5]=hIDs.GetEntries();
	out[6]=hx.GetMean();
	out[7]=hx.GetRMS();
	out[8]=hy.GetMean();
	out[9]=hy.GetRMS();
	out[10]=hTheta.GetKurtosis();

	// Write histograms to output file
	TFile* fOut= new TFile(name+"histograms.root","recreate");
	hEn.Write();
	hTheta.Write();
	hx.Write();
	hy.Write();
	hxy.Write();
	hThetae.Write();
	hxe.Write();
	hye.Write();
	hxye.Write();
	hThetap.Write();
	hxp.Write();
	hyp.Write();
	hxyp.Write();
	hzs.Write();
	hzp.Write();
	hIDs.Write();
	hret.Write();
	// Close the file
	fOut->Close();
	file1->Close();
} // End of function analysis -----------------------------------------

// Main function:
int main(){
	// We create a .txt file to write the output parameters
	ofstream output;
	output.open("Muones.txt");
	for (int i=0; i<1;i++){
	  output << elemento[i] << " " ;
	    for (int j=0; j<1;j++){
	      output << espesor[j] << " " ;
	        for (int k=0; k<1;k++){
		 output << energia[k] << "GeV ";
		 // We perform the analysis:
		 analysis(pars[i][j][k], "PathToFiles/Muones/"+elemento[i]+"/"+espesor[j]+"/"+energia[k]+"GeV.root");
		   // We write the parameters:
		   for (int l=0; l<11;l++){
		   output << " " << pars[i][j][k][l] ;
		   }
		   output << endl;
		}
	    }
	}
	output.close();
	return 0;
}

