
#ifndef TSCINTIDETGEOPAR_H
#define TSCINTIDETGEOPAR_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

/// EnsarScintillatorGeoPar.h

class EnsarScintillatorGeoPar: public FairParGenericSet
{
  public:

    /**
     * \author  M.I.Cherciu \n
     * Modified by: \n   
     * Comment: \n
     * \version 1.0 \n
     * \since 2014/07/02 \n
     * Change date: \n
     * \file EnsarScintillatorGeoPar.h
     * \brief Class for geometry parameters definition of Scintillator Detector.
     */	
    
    /** Default Contructor.*/	 
    EnsarScintillatorGeoPar(const char* name="EnsarScintillatorGeoPar",
                      const char* title="EnsarScintillator Geometry Parameters",
                      const char* context="TestDefaultContext");
    
    /** Default Destructor.  */
    ~EnsarScintillatorGeoPar(void);
    
    /** Clearing Objects Array.  */    
    void clear(void);
    
    /** Register the parameters.  */
    void putParams(FairParamList*);
    
    /** Get registered parameters.  */
    Bool_t getParams(FairParamList*);
    
    /** Get Object Array of Sensitive Nodes.  */
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    
    /** Get Object Array of Passive Nodes.  */
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

    /** List of FairGeoNodes for sensitive  volumes. */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for passive  volumes. */
    TObjArray*      fGeoPassNodes;
    
    
   ClassDef(EnsarScintillatorGeoPar,1)
};

#endif
