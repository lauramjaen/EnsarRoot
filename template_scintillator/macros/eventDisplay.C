/** --------------------------------- info ---------------------------------
*              Author: M.I. Cherciu @ Institute of Space Science 
*              E-mail: mcherciu@spacescience.ro
*              Version: 1.0
*              Submission Date:02.07.2014
*    ------------------------------------------------------------------------
*/


eventDisplay()
{
   // ----  Load generic libraries   -------------------------------------------------
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gSystem->Load("libGeoBase");
  gSystem->Load("libFairDB");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libMCStack");
  gSystem->Load("libGen");

  gSystem->Load("libEve");
  gSystem->Load("libEventDisplay");

  //----  Load detector specific libraries ---------------------------------------

  gSystem->Load("libEnsarbase");
  gSystem->Load("libEnsarGen"); 
  gSystem->Load("libEnsarData"); 
  gSystem->Load("libEnsarEvtVis"); 
  gSystem->Load("libEnsarScintillator");
                                     
  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
 
  TFile* file = new TFile("simpar.root");
  file->Get("FairBaseParSet"); 

 // -----   Runtime database   ---------------------------------------------

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIn = new FairParRootFileIo();
  parIn->open("simpar.root");
  rtdb->print();


 //-------
 
  fRun->SetInputFile("simout.root");
  fRun->SetOutputFile("test.root");

  EnsarEventManager *fMan= new EnsarEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");

  
  fMan->AddTask(Track);
  
fMan->Init();
  
}
