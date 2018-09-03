// *********************************************************************
// *****             EnsarThoriumBackgroundGen header file              
// *****   						
// *****      Gamma Thorium Background Generator                     	 
// ***** 							
// ***** 	  This Generator reproduce different gammas which 
// *****		have come from 232Th-chain	
// ***** 			
// ***** elisabet.galiana@usc.es
// ***** Universidad de Santiago de Compostela                    
// ***** Dpto. Física de Partículas                               
// *****   							
// *********************************************************************


#ifndef ENSAR_232THORIUMCHAINGENERATOR_H
#define ENSAR_232THORIUMCHAINGENERATOR_H

#include "FairGenerator.h"

#include <iostream>
#include <fstream>


using namespace std;



class FairPrimaryGenerator;

class Ensar232ThoriumChainGen : public FairGenerator
{
  public:

    /** Default constructor. **/
    Ensar232ThoriumChainGen();

    /** Constructor.  **/
    Ensar232ThoriumChainGen(const char* inputFile);

    /** Destructor. **/
    virtual ~Ensar232ThoriumChainGen();
  
    /** Initializer **/
    Bool_t Init();
    
    /** Creates an event **/
    virtual Bool_t ReadEvent(FairPrimaryGenerator* primGen);
    
 private:

		ifstream*     fInputFile; //! Input file stream
    const Char_t* fFileName; 	//! Input file Name
		Int_t fnumGammas;				//total number of gammas in the chain
    Double_t* fenergy;			//Energy of gammas
    Double_t* fprobability;	//Probability of each gamma
    
   
    ClassDef(Ensar232ThoriumChainGen,1);
    void CloseInput();
    bool ReadParameters();
    
    
};


#endif
