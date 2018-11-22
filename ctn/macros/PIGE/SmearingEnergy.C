
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include "TMath.h"
#include "TRandom.h"



int macro(){

	for(Int_t i=0;i<10;i++){

		Double_t NonUniformity;
		Double_t inputEnergy;
		Double_t energy;

		NonUniformity=3.;
		inputEnergy=5.;

		TRandom* random= new TRandom();
		//energy=random->Uniform (inputEnergy - inputEnergy * NonUniformity / 100,inputEnergy + inputEnergy * NonUniformity / 100);
		//cout<<"Smearing energy="<<energy<<endl;
		


		//Alrededor de un valor
 		Double_t Rndm;
		Double_t energy_smearing;
 		Rndm=gRandom->Rndm()-1;
		energy_smearing=energy+Rndm;
		//cout<<"2. "<<energy_smearing<<endl;

		//Gaussian smearing
		energy=gRandom->Gaus(inputEnergy, 0.002 *inputEnergy);
		cout<<"Gauss energy="<<energy<<endl;

		energy=0;
		delete random;
	}	


return 0;
}
