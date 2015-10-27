// -------------------------------------------------------------------------
// -----                        R3BMbsFile file                        -----
// -----                  Created 26/12/12  by J.L. Rodriguez          -----
// -----                                                               -----
// -------------------------------------------------------------------------

#include "R3BMbsFile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

R3BMbsFile::R3BMbsFile(const char* name, Int_t* status) :
    fName(name)
{
    OpenFile(status);
}

Int_t* R3BMbsFile::OpenFile(Int_t* status)
{
 int inputmode=1;
 fxInputChannel=new s_evt_channel;
 void* headptr=&fxInfoHeader;

 *status = f_evt_get_open(inputmode,const_cast<char*>(fName.Data()),fxInputChannel,
                                    (Char_t**) headptr, 1,1);
 cout << "-I- Status OpenFile="<< *status<<endl;
 if(!status)PrintFileHeader(fxInfoHeader);
 
  if(*status){
  Fatal("R3BMbsFile","Cannot open input file.");
  cout << "Error opening file: " << fName.Data() << endl;
  }
  return status;
}

R3BMbsFile::~R3BMbsFile()
{
   CloseFile();
}

Int_t R3BMbsFile::CloseFile()
{
  if(!fxInputChannel)Fatal("R3BMbsFile","Cannot open input file.");
  Int_t status = f_evt_get_close(fxInputChannel);
  cout << "-I- Status CloseFile="<<status<< endl; 
  if (!status) {return kTRUE;}
  else {
   cout << "Error closing file: " << fName.Data() << endl;
   return kFALSE;}
}

Int_t R3BMbsFile::GetEvent()
{
 void* evtptr =&fxEvent; 
 void* buffptr=&fxBuffer; 

 Int_t status=f_evt_get_event(fxInputChannel, (Int_t**)evtptr,(Int_t**) buffptr);

 fuEventCounter=fxEvent->l_count;
 
 if (status==0) {
//  cout << "got an event, it's nr: "<<fuEventCounter<<endl;
  return status;}
 else if (status==3){
   cout << "-I- This is last event"<<endl;
   return status;
 }
 else{
  cout << "Error getting event: "<< endl;
  return status;}
}

Int_t R3BMbsFile::GetNrSubEvent()
{
 Int_t status=f_evt_get_subevent(fxEvent, 0, NULL, NULL, NULL);
 if (status) {

  return status;}
 else {
  cout << "No subevents found: "<< endl;
  return 0;}
}

Int_t* R3BMbsFile::GetSubEvent(Int_t n, Int_t *sebuflenght,Short_t *setype,Short_t *sesubtype,Char_t *sesubcrate)
{
 void* SubEvtptr=&fxSubEvent; 
 void* EvtDataptr=&fxEventData;
 Int_t* nrlongwords=new Int_t;
 Int_t status=f_evt_get_subevent(fxEvent, n, (Int_t**)SubEvtptr, (Int_t**)EvtDataptr, nrlongwords);
 *sebuflenght=fxSubEvent->l_dlen;
 *setype   =fxSubEvent->i_type;
 *sesubtype=fxSubEvent->i_subtype;
 *sesubcrate=fxSubEvent->h_subcrate;

 if (!status) {
  delete nrlongwords;

  return fxEventData;}
 else {
  cout << "No sub event got: "<< endl;
  delete nrlongwords;
  return NULL;
 }
}

Int_t R3BMbsFile::GetNbTrigger(){

return fxEvent->i_trigger;

}

void R3BMbsFile::PrintSubEvent(s_ves10_1* SEH, Int_t* ED){
 cout << "This is Subevent header:"<<endl;
 cout <<SEH->l_dlen<<endl;
 cout <<SEH->i_subtype<<endl;
 cout <<SEH->i_type<<endl;
 cout <<SEH->h_control<<endl;
 cout <<SEH->h_subcrate<<endl;
 cout <<SEH->i_procid<<endl;
 cout <<"##"<<endl;
}

void R3BMbsFile::PrintBufferHeader(){
 cout << "This is Buffer header:"<<endl;
 cout <<  fxBuffer->l_dlen << endl;
 cout <<  fxBuffer->l_evt << endl;
 cout <<  fxBuffer->h_begin << endl;
 cout <<  fxBuffer->h_end << endl;
}

void R3BMbsFile::PrintEventHeader(){
 cout << "This is Event header:"<<endl;
 cout << "lenght= "<< fxEvent->l_dlen << endl;
 cout << "subtyp= "<<fxEvent->i_subtype << endl;
 cout << "type  = "<<fxEvent->i_type << endl;
 cout << "trigg = "<<fxEvent->i_trigger << endl;
 cout << "count = "<<fxEvent->l_count << endl;
}

 void R3BMbsFile::PrintFileHeader()
{
  cout << "-I- This is File Header:"<<endl;
  cout << "-I- Lenght "<<fxInfoHeader->filhe_dlen << endl;
  cout << "-I- "<<fxInfoHeader->filhe_label << endl;
  cout << "-I- Time   "<<fxInfoHeader->filhe_time << endl;
  cout << "-I- File   "<<fxInfoHeader->filhe_file << endl;
}

void R3BMbsFile::PrintFileHeader(s_filhe* FH)
{
  cout << "This is File Header:"<<endl;
  cout << FH->filhe_dlen << endl;
  cout << FH->filhe_evt << endl;
  cout << FH->filhe_label << endl;
  cout << FH->filhe_run << endl;
  cout << FH->filhe_time << endl;
  cout << FH->filhe_current_i << endl;
  cout << FH->filhe_file << endl;
}
