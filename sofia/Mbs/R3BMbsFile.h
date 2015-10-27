// -------------------------------------------------------------------------
// -----                        R3BMbsFile header file                 -----
// -----                  Created 26/12/12  by J.L. Rodriguez          -----
// -----                                                               -----
// -------------------------------------------------------------------------

/**  R3BMbsFile.h
 **/

#ifndef R3BMbsFile_H
#define R3BMbsFile_H

#ifdef __CINT__
struct s_filhe;
struct s_bufhe;
struct s_evt_channel;
struct s_ve10_1;
struct s_ves10_1;
#else
extern "C"
{
  #include "f_evt.h"
  #include "s_filhe_swap.h"
  #include "s_bufhe_swap.h"
}
#endif

#include "TObject.h"
#include "TString.h"


class R3BMbsFile : public TObject {
  public:

    R3BMbsFile(const char* name, Int_t* status);
    ~R3BMbsFile();
    Int_t* OpenFile(Int_t* status);    //  uses f_evt_get_open  opens an event stream from specified channel
    Int_t CloseFile();                 //  uses f_evt_get_close  closes event stream of specified channel
    void PrintFileHeader(s_filhe* FH); //  print something from file header
    Int_t GetEvent();                  //  uses f_evt_get_event  returnes address of event
    Int_t GetNbTrigger();              //  return trigger number
    Int_t GetNrSubEvent();             //  uses f_evt_get_subevent get number of subevent
    Int_t* GetSubEvent(Int_t NrSubEvt, Int_t *sebuflenght, 
    Short_t *setype, Short_t *sesubtype,Char_t *sesubcrate); // uses f_evt_get_subevent get subevent pointer
    void PrintSubEvent(s_ves10_1* SEH,Int_t* ED); // print something from subevent
    void PrintFileHeader();            // print something from file header
    void PrintBufferHeader();          // print something from Buffer header
    void PrintEventHeader();           // print something from Event header
 
  private:
    TString fName;
    //  Implemented by Paolo
    s_evt_channel* fxInputChannel; //  Event channel structure used by event source
    s_ves10_1* fxSubEvent;         //  GSI VME Subevent header
    s_ve10_1*  fxEvent;            //  GSI VME Event header
    s_filhe* fxInfoHeader;         //  File header
    s_bufhe * fxBuffer;            //  GSI Buffer header
    Int_t* fxEventData;            //  Event data pointer
    ULong_t fuEventCounter;        //  Current event index counter
 

    /** Points to the current gsi buffer structure filled by the event source. */
    /** Reference to header info delivered by source. */
    Bool_t fbIsOpen;
    /** switches buildmbs event mode: if true, data buffer is copied
    * to own buffer of subevent. If false (default), mbs subevent uses
    * reference to external data buffer of eventapi. */
    Bool_t fbDataCopyMode;
    /** Indicates if first event should be extracted */
    Bool_t fbFirstEvent;
    /** Index of first event to process. */
    ULong_t fuStartEvent;
    /** Index of last event to process. */
    ULong_t fuStopEvent;
    /** Number of events to skip from file in between two read events  */
    ULong_t fuEventInterval;
    /** Timeout in seconds for mbs getevent. If -1, no timeout (default)  */
    Int_t fiTimeout;


    ClassDef(R3BMbsFile, 1)
};

#endif //R3BMbsFile_H
