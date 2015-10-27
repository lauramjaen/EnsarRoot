//  -------------------------------------------------------------------------
//
//   ----- General Macro for SOFIA simulation
//
//         Author: Jose Luis Rodriguez Sanchez <joseluis.rodriguez.sanchez@usc.es>
//
//         Last Update: 30/08/2012
//
//         Comments:
//
//
//  -------------------------------------------------------------------------

sofiaevtvis()
{
   // ----  Load libraries   -------------------------------------------------
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libGen");
  gSystem->Load("libSOFIAMCStack");
  gSystem->Load("libSOFIAField");

  // Load FairRoot base Event Display Libs
  gSystem->Load("libEve");
  gSystem->Load("libEventDisplay");

  //----  Load R3B specific libraries ---------------------------------------
  gSystem->Load("libSOFIAbase");
  gSystem->Load("libSOFIAGen");
  gSystem->Load("libSOFIAPassive");
  gSystem->Load("libSOFIAData");
  gSystem->Load("libSOFIACal");
  gSystem->Load("libSOFIACalo");
  gSystem->Load("libSOFIATwinMusic");
  gSystem->Load("libSOFIAMwpc1");
  gSystem->Load("libSOFIAMwpc2");
  gSystem->Load("libSOFIATof");
  gSystem->Load("libSOFIALand");

  //----  Load R3B vis. specific libraries --------------------------  
  gSystem->Load("libSOFIAEvtVis");

                                     
  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
  TFile* file = new TFile("sofiapar.root");
  file->Get("FairBaseParSet"); 

 // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIn = new FairParRootFileIo();
  parIn->open("sofiapar.root");
  rtdb->print();

  fRun->SetInputFile("sofiasim.root");
  fRun->SetOutputFile("test.root");
  
  R3BEventManager *fMan= new R3BEventManager();
  R3BMCTracks *Track =  new R3BMCTracks ("Monte-Carlo Tracks");

  fMan->AddTask(Track);

  fMan->Init();
  
  gEve->GetDefaultGLViewer()->SetClearColor(20);                     
  
}
