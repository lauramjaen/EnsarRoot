/// /////////////////////////////////////////////////////////////////////////
///
/// E. Nacher (OCT. 2011)
///
/// Simple "Giant+Pigmy Resonance" Event Generator for R3BRoot
///
/// It generates an output file in the appropriate format to use
/// the ascii generator of R3BRoot
///
/// It must be compiled and linked to root libraries:
/// g++ `root-config --cflags` `root-config --glibs` sourceFile.cc -o execFile.x
///
/// /////////////////////////////////////////////////////////////////////////


//General C++ and C Includes
#include<fstream>
#include<iostream>
#include<iomanip>
#include<cmath>
#include <string>
#include <cstdlib>

//Root Includes
#include <TH1I.h>
#include <TH1F.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TFile.h>
#include <TBrowser.h>
#include <TROOT.h>
#include<TRandom3.h>

using namespace std;
	
int main(int argc, char* argv[]){
	
	TRandom3 ran0, ran1, ran2, ran3;

	double resMean, resWidth, pigmyMean, pigmyWidth, primEnergy, shoot, px, py, pz;
	double betaFactor, nominalBetaFactor, gammaFactor, uncertBeta;
    double ener[30], pop[30], cumpop[30], br[30][30], cumbr[30][30], gamEner[30];
    double pi = 3.141592654, twopi = 6.283185307, cosTheta, theta, phi;
    int nEvents, nLevels, initLevel, finalLevel, mul;
    bool lorentzBoost = 1;
	
	/// Initialize arrays to 0
	for (int i=0; i<30; i++) {
		ener[i] = 0.;
		pop[i] = 0.;
		cumpop[i] = 0.;
		gamEner[i] = 0.;
		for (int j=0; j<30; j++) {
			br[i][j] = 0.;
			cumbr[i][j] = 0.;
		}
	}
    
	string dummy;
	ifstream inputFile("pigmyRes_132Sn.dat");
	ofstream outputFile("pigmyRes.out");
    
	TH1F* h1 = new TH1F("population","",4096,0,35);
    TH1F* h2 = new TH1F("Gamma Energy (CM)","",4096,0,35);
    TH1I* h3 = new TH1I("Multiplicity","",15,0,15);
    TH1F* h4 = new TH1F("Polar Angle (CM)","",180,0,180);
    TH1F* h5 = new TH1F("Azimutal Angle (CM)","",180,0,360);
    TH1F* h6 = new TH1F("Polar Angle (Lab)","",180,0,180);
    TH1F* h7 = new TH1F("Gamma Energy (Lab)","",4096,0,50);
    TH1F* h8 = new TH1F("betaFactor","",100,0,1);

/// /////////////////////////////////////////////////////////////////
///  Creating the population array and the branching ratio matrix    
                                                                     
	for(int i = 0; i < 11; i++) getline (inputFile,dummy);
	inputFile >> nEvents;                                            
	getline (inputFile,dummy);                                       
                                                                     
    getline (inputFile,dummy);                                       
    inputFile >> nominalBetaFactor >> uncertBeta;                           
    getline (inputFile,dummy);                                       
                                                                     
    getline (inputFile,dummy);                                       
    inputFile >> nLevels;                                            
    getline (inputFile,dummy);                                       
                                                                     
    getline (inputFile,dummy);                                       
    inputFile >> resMean >> resWidth >> pop[nLevels-1];                
    getline (inputFile,dummy);                                       

    getline (inputFile,dummy);                                       
    inputFile >> pigmyMean >> pigmyWidth >> pop[nLevels-2];                
    getline (inputFile,dummy);
		
    getline (inputFile,dummy);                                       
    for(int i = 0; i < nLevels-2; i++){                                
		inputFile >> ener[i] >> pop[i]; 
	} 
	
    getline (inputFile,dummy);                                       
                                                                     
	getline (inputFile,dummy);                                       
	for(int i = 1; i <= nLevels-1; i++){                               
		getline (inputFile,dummy);                                   
		for(int j = 0; j < i; j++){                                  
			inputFile >> br[i][j];
		}                   
		getline (inputFile,dummy);                                   
	} 
		
/// //////////////////////////////////////////////////////////////////
///  Building the cumulative populations and branchings               
                                                                      
    cumpop[0] = pop[0];                                               
    for(int i = 1; i <= nLevels-1; i++){                                
		cumpop[i] = pop[i] + cumpop[i-1];                                 
	}                                                                 
    for(int i = 0; i <= nLevels-1 ; i++){                               
		cumpop[i] = cumpop[i]/cumpop[nLevels-1];                            
    }
	                                                  
    for(int i = 1; i <= nLevels-1; i++){                                
		cumbr[i][0] = br[i][0];                                           
		for(int j = 1; j < i; j++){                                   
			cumbr[i][j] = br[i][j] + cumbr[i][j-1];                   
		}                                                             
	}                                                                 
     for(int i = 1; i <= nLevels-1; i++){                               
		 for(int j = 0; j < i; j++){                                  
			 cumbr[i][j] = cumbr[i][j]/cumbr[i][i-1];                 
		 }                                                            
	 }
/// //////////////////////////////////////////////////////////////////

	/// Loop over the total number of events nEvents
    for(int event = 0; event < nEvents; event++){
		

		betaFactor = nominalBetaFactor + 
		((-1 + 2 * ran0.Rndm()) * ((uncertBeta/100.)*nominalBetaFactor));

		h8 -> Fill(betaFactor);
      
		mul = 0;
      
		/// Decide the primary level
		shoot = ran1.Rndm();
      
		if(shoot <= cumpop[0]) primEnergy = ener[0];
		else if(shoot > cumpop[nLevels-2]){
			primEnergy = ran2.Landau(resMean,resWidth);
			initLevel = nLevels-1;
		}
		else if(shoot > cumpop[nLevels-3]){
			primEnergy = ran2.Gaus(pigmyMean,pigmyWidth);
			initLevel = nLevels-2;
		}
		else{
			int flag = 0;
			for(int i = 0; flag < 1; i++){
				if(shoot <= cumpop[i]){
					primEnergy = ener[i];
					flag++;
				}
				initLevel = i;
			}
		}      
      
		h1 -> Fill(primEnergy);
		/// End of primary level

		/// Generate the gamma cascade
		shoot = ran1.Rndm();

		while(initLevel > 0){

			int flag = 0;
			for(int j = 0; flag < 1; j++){
				if(shoot <= cumbr[initLevel][j]){

					if(initLevel == nLevels-1 || initLevel == nLevels-2) gamEner[j] = primEnergy - ener[j];
					else gamEner[j] = ener[initLevel] - ener[j];
					h2 -> Fill(gamEner[j]);
					finalLevel = j;
					flag++;
					mul++;
				}
			}			
			initLevel = finalLevel;
		} 
		h3 -> Fill(mul);
		/// End of gamma cascade
      
		outputFile << setw (5) << left << setfill (' ')  << event  << "\t\t" << mul << "\t\t1.5e4\t\t" << betaFactor << endl;
     
		/// Write out the gamma cascade with the format: PDGid  px  py  pz  vx  vy  vz
		/// Loop over the different gamma-rays of the cascade (i goes from 0 to 'multiplicity')
		for (int i = 0; i < mul; i++){
	
			cosTheta = -1 + 2*(ran3.Rndm());
			theta = acos(cosTheta); phi = ran3.Rndm() * twopi;
			px = gamEner[i]*sin(theta)*cos(phi); py = gamEner[i]*sin(theta)*sin(phi); pz = gamEner[i]*cos(theta);
	    
			h4 -> Fill(theta*180/pi);
			h5 -> Fill(phi*180/pi);

			/// Applying Lorentz Boost
			if(lorentzBoost){ 
				gammaFactor = 1 / sqrt(1-betaFactor*betaFactor);
				pz = (pz + betaFactor * gamEner[i]) * gammaFactor;
				if(pz > 0) theta = atan(sqrt(px*px + py*py)/pz);
				if(pz < 0) theta = pi + atan(sqrt(px*px + py*py)/pz);
				if(pz == 0) theta = pi/2.;
			}
			outputFile << left << fixed << setfill ('0') 
			<<"22\t\t22\t\t22\t\t" << setw (10) << setprecision (5) << px/1000. << "\t\t" 
			<< setw (10) << setprecision (5) << py/1000. << "\t\t" 
			<< setw (10) << setprecision (5) << pz/1000. 
			<< "\t\t0.\t\t0.\t\t0." << endl;
	
			h6 -> Fill(theta*180/pi);                /// Direction and energy in 'lab' system (modified by the 
			h7 -> Fill(sqrt(px*px + py*py + pz*pz)); /// boost)
		}
	}
    
	TApplication* rootapp = new TApplication("",&argc, argv);

	TFile *rootOutputFile = new TFile("pigmyRes.root","recreate");
    
	gROOT->SetStyle("Plain");
    gStyle->SetPalette(1);
    gStyle->SetOptStat(1000201);
    
	h1 -> GetXaxis()->SetTitle("Energy (MeV)");
	h1 -> GetYaxis()->SetTitle("Population");
	h2 -> GetXaxis()->SetTitle("Energy_{CM} (MeV)");
	h2 -> GetYaxis()->SetTitle("Counts");
	h3 -> GetXaxis()->SetTitle("Multiplicity");
	h3 -> GetYaxis()->SetTitle("Events");
	h4 -> GetXaxis()->SetTitle("#Theta_{CM} (deg)");
	h4 -> GetYaxis()->SetTitle("Counts");
	h5 -> GetXaxis()->SetTitle("#phi_{CM} (deg)");
	h5 -> GetYaxis()->SetTitle("Counts");
	h6 -> GetXaxis()->SetTitle("#Theta_{Lab} (deg)");
	h6 -> GetYaxis()->SetTitle("Counts");
	h7 -> GetXaxis()->SetTitle("Energy_{Lab} (MeV)");
	h7 -> GetYaxis()->SetTitle("Counts");
	h8 -> GetXaxis()->SetTitle("Beta Factor");
	h8 -> GetYaxis()->SetTitle("Counts");

    h1 -> Write();
    h2 -> Write();
    h3 -> Write();
    h4 -> Write();
    h5 -> Write();
    h6 -> Write();
    h7 -> Write();
    h8 -> Write();

    new TBrowser();
    rootapp -> Run();


	return 0;
	
}
