// *********************************************************************
// *****             EnsarCascadeGen header file              
// *****   						
// *****      Simply Gamma Cascade Generator
// *****      with Angular Correlations                       	 
// ***** 							
// *****      Works with the data files:			
// ***** 		 -Co_cascade.dat			
// *****		 -Si_12900_cascade.dat		    	
// ***** 		 -Gamma_AngularC_200k.dat		
// ***** 	  or any file with the same data structure	
// *****							
// ***** elisabet.galiana@usc.es
// ***** Universidad de Santiago de Compostela                    
// ***** Dpto. Física de Partículas                               
// *****   							
// *********************************************************************


#ifndef ENSAR_CASCADEGENERATOR_H
#define ENSAR_CASCADEGENERATOR_H

#include "FairGenerator.h"

#include <iostream>
#include <fstream>


using namespace std;



class FairPrimaryGenerator;

class EnsarCascadeGen : public FairGenerator
{
  public:

    /** Default constructor. **/
    EnsarCascadeGen();

    /** Constructor.  **/
    EnsarCascadeGen(const char* inputFile, Int_t state); //file name

    /** Destructor. **/
    virtual ~EnsarCascadeGen();

    /** Modifiers **/
    //void SetpEnergy      (Double32_t Ep=0.0015)  {fpEnergy = Ep;  };//GeV
    
    /** Initializer **/
    Bool_t Init();
    
    /** Creates an event **/
    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
    
 private:
    ifstream*     fInputFile;       //! Input file stream
    const Char_t* fFileName; 		//! Input file Name
    Double_t* fener;				//Energy of the states
    Double_t* fnLevel_tot;			//Number of Levels
    Double_t* fener_i;				//Initial energy of each level
    Double_t* fener_f;				//Final energy of each level
    Double_t* fener_gamma;			//Gamma energy
    Double_t* fBR;					//Branching Ratios
    Double_t** fA;					//Angular Coeficients A2 and A4
    ClassDef(EnsarCascadeGen,1);
    void CloseInput();
    void ReadParameters();
    
};


#endif


