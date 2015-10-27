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
  gSystem->Load("libEnsarSilicon");
                                     
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

  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");

  
  fMan->AddTask(Track);
  
fMan->Init();
  
}
