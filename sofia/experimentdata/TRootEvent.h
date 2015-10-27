// -------------------------------------------------------------------------
// -----                        R3BTRootEvent header file              -----
// -----                  Created 26/11/12  by J.L. Rodriguez          -----
// -----                                                               -----
// -------------------------------------------------------------------------

/**  TRootEvent.h
 **/


#ifndef TROOTEVENT_H
#define TROOTEVENT_H

////////////////////////////////////////////
// TRootEvent class
// generic interface to Root/Trees 
// e.d.f revision 03/2011
//       revision 01/2012   Land tree
////////////////////////////////////////////
#include "TTree.h"
#include "TFile.h"
#include "TNamed.h"

//#include "FairMultiLinkedData.h"

const int dmax = 500;
const int dmaxTW = 100;
const int fNumOfRingMBall = 4;
const int dmaxM=32;


// Generic raw Chimera event
// Note that //[multi] are not C++ comments but 
// mandatory Root dictionary directives

class TRootLANDEvent : public TNamed
{
 public:
/*
 rawmulti=numer of paddle with at least 1 PM time fired
 padID= plane*100+paddlenumber veto->plane=10
 qdc1-2[padID] = left and right qdcs raw                  
 tac1-2[padID] = left and right raw time                  
 t171-2[padID] = left and right t17                  
 tcl1-2[padID] = left and right number of clock cycles                  
 hit[padID]= 0 (only 1 pm fired) 1 (both pms fired)
 tcal1-2[padID][0,1]=(a*time+b)+25*tcl-(c+t17+d)   calibrated times
*/
 int rawmulti;
 int *padID;//[rawmulti]
 double *qdc1;//[rawmulti]
 double *qdc2;//[rawmulti];
 double *tac1;//[rawmulti]
 double *tac2;//[rawmulti]
 double *t171;//[rawmulti]
 double *t172;//[rawmulti]
 int    *tcl1;//[rawmulti]
 int    *tcl2;//[rawmulti]
 int    *hit;//[rawmulti]
 double *tcal1;//[rawmulti]
 double *tcal2;//[rawmulti]
 
//constructor 
TRootLANDEvent() 
{
 printf("TRootLANDEvent initialized\n");
 rawmulti=0;
 padID=new int[dmax];
 qdc1=new double[dmax];
 qdc2=new double[dmax];
 tac1=new double[dmax];
 tac2=new double[dmax];
 t171=new double[dmax];
 t172=new double[dmax];
 tcl1=new int[dmax];
 tcl2=new int[dmax];
 hit=new int[dmax];
 tcal1=new double[dmax];
 tcal2=new double[dmax];
}
 
//destructor
 virtual ~TRootLANDEvent() {
  delete [] padID;
  delete [] qdc1;
  delete [] qdc2;
  delete [] tac1;
  delete [] tac2;
  delete [] t171;
  delete [] t172;
  delete [] tcl1;
  delete [] tcl2;
  delete [] hit;
  delete [] tcal1;
  delete [] tcal2;
 }

 ClassDef(TRootLANDEvent,1);
};
class TRootDefine 
{
 private:
  TTree *ftr;
  TFile *frootW;
  TRootLANDEvent *flandevt;

 public:
  TRootDefine() {ftr=0; 
   frootW=0; 
   flandevt=new TRootLANDEvent();
  }

  void Open(const char *name);
  void Close();
  TTree *GetTree() {return ftr;}
  TRootLANDEvent *GetLANDEvt() {return flandevt;}
    
  ClassDef(TRootDefine,1);
};

#endif
