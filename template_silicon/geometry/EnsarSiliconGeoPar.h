
/** EnsarSiliconGeoPar.h **/

#ifndef TSILIDETGEOPAR_H
#define TSILIDETGEOPAR_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

/// EnsarSiliconGeoPar.h

class EnsarSiliconGeoPar: public FairParGenericSet
{
  public:

    /**
     * \author  M.I.Cherciu
     * \version 1.0
     * \since 10.2014
     * \file EnsarSiliconGeoPar.h
     * \brief Class for geometry parameters definition of Silicon Detector.
     */	
    
    /** Default Contructor.*/	 
    EnsarSiliconGeoPar(const char* name="EnsarSiliconGeoPar",
                      const char* title="EnsarSilicon Geometry Parameters",
                      const char* context="TestDefaultContext");
    
    /** Default Destructor.  */
    ~EnsarSiliconGeoPar(void);
    
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
    
    
   ClassDef(EnsarSiliconGeoPar,1)
};

#endif
