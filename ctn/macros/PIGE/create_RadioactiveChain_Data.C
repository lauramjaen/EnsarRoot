//https://root.cern.ch/root/html510/TMatrixF.html

#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include "TMath.h"


//#define A_MAX 250
//#define Z_MAX 100
//#define INI_STATE 20
//#define DECAY_NUM 20

int create_RadioactiveChain_Data(){

	const int A_MAX=250;
	const int Z_MAX=100;
	const int INI_STATE=20;
	const int DECAY_NUM=20;


	Int_t Particle			[A_MAX][Z_MAX][INI_STATE]={0};//PID of the emmitted particale in the decay
	Int_t Num_Decays		[A_MAX][Z_MAX][INI_STATE]={0};//Number of decays
	Double_t Probability[A_MAX][Z_MAX][INI_STATE][DECAY_NUM]={0};//Probability of each decay
	Double_t Energy			[A_MAX][Z_MAX][INI_STATE][DECAY_NUM]={0};//Energy of each particle emmitted
	Double_t Final_A		[A_MAX][Z_MAX][INI_STATE][DECAY_NUM]={0};//Final A of each decay
	Double_t Final_Z		[A_MAX][Z_MAX][INI_STATE][DECAY_NUM]={0};//Final Z of each decay
	Double_t Final_State[A_MAX][Z_MAX][INI_STATE][DECAY_NUM]={0};//Final state of each decay


	//#include "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/UStep_238_92.dat"
	//#include "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/ThStep_234_90.dat"

	//cout<<"Particle"<<Particle[238][92][0]<<endl;
	//cout<<"Particle"<<Particle[234][90][0]<<endl;

	for (Int_t i=1; i<=10;i++){

		cout<<"i="<<i<<endl;

		Float_t ran_ini =gRandom->Rndm();
		//Float_t ran_ini =0.01;
		Double_t prob=0.5;//0.07142857
		//unsigned int A, Z;
		int A, Z;
		A=0;
		Z=0;

		bool uu;
		bool aa;
		
		uu=kFALSE;
		aa=kFALSE;
		
		

		if (ran_ini>0 && ran_ini<prob){
				uu=kTRUE;
				#include "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/UStep_238_92.dat"
				//238U
				A=238;
				Z=92;
				cout<<"Case 1:  "<<ran_ini<<" exists between 0 and "<<prob<<endl;

		} else if (ran_ini>prob && ran_ini<=2*prob){
				aa=kTRUE;
				#include "/home/elizabet/Escritorio/EnsarRoot/EnsarRoot_source/ctn/macros/PIGE/ThStep_234_90.dat"
				//234Th
				A=234;
				Z=90;
				cout<<"Case 2:  "<<ran_ini<<" exists between "<<prob<<" and "<<2*prob<<endl;

		}

		cout<<" Particle="<<Particle[A][Z][0]<<endl;//ok!
		cout<<" Num_Decays="<<Num_Decays[A][Z][0]<<endl;//ok!
			
		Int_t size;
		size=Num_Decays[A][Z][0]+1;
		Double_t limits[size];
		limits[0]=0;

		cout<<"SIZE= "<<size<<endl;//ok!
		for (Int_t i=1;i<size;i++){
			Double_t lim1, prob1;
			lim1=limits[i-1];
			prob1=Probability[A][Z][0][i];
			limits[i]= lim1+ prob1;
			//cout<<"limits["<<i<<"]"<<limits[i]<<endl;
		}

		cout<<endl<<endl;
		unsigned int zero;
		zero=0;
		for(Int_t i=1;i<3;i++){
			cout<<Probability[234][90][0][i]<<endl;//si funciona
			//cout<<Probability[A][Z][zero][i]<<endl;//Descomenta esta linea y ya no funciona
		}

//cout<<"238 Limits: "<<limits[0]<<" "<<limits[1]<<" "<<limits[2]<<endl;//Descomenta esta linea y ya no funciona

		if (uu==kTRUE){
			//A=238;
			//Z=92;
			//cout<<"238 Limits: "<<limits[0]<<" "<<limits[1]<<" "<<limits[2]<<endl;
		}

		if (aa==kTRUE){
			//A=234;
			//Z=90;
			//cout<<"234 Limits: "<<limits[0]<<" "<<limits[1]<<" "<<limits[2]<<" "<<limits[3]<<" "<<limits[4]<<endl;
		}
		

	}

return 0;
}
