eventDisplay()
{
   // ----  Load libraries   -------------------------------------------------
  gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
  basiclibs();
  gSystem->Load("libGeoBase");
  gSystem->Load("libParBase");
  gSystem->Load("libBase");
  gSystem->Load("libSOFIAMCStack");
  gSystem->Load("libSOFIAField");

  //----  Load R3B specific libraries ---------------------------------------
  gSystem->Load("libSOFIAbase");
  gSystem->Load("libSOFIAGen");
  gSystem->Load("libSOFIAPassive");
  gSystem->Load("libSOFIAData");
  gSystem->Load("libSOFIACal");
  gSystem->Load("libSOFIACalo");
  gSystem->Load("libSOFIALand");
  
  
  //----  Load vis. specific libraries ---------------------------------------
  gSystem->Load("libEve");
  gSystem->Load("libEventDisplay");

                                     
  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();
 
  TFile* file = new TFile("r3bpar.root");
  file->Get("FairBaseParSet"); 

 // -----   Runtime database   ---------------------------------------------

  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo* parIn = new FairParRootFileIo();
  parIn->open("r3bpar.root");
  rtdb->setFirstInput(parIn);
  rtdb->print();


 //-------
 
  fRun->SetInputFile("r3bsim.root");

  fRun->SetOutputFile("test.root");
  //fRun->LoadGeometry();

  FairEventManager *fMan= new FairEventManager();
  FairMCTracks *Track =  new FairMCTracks ("Monte-Carlo Tracks");
  //FairMCPointDraw *LandPoints =   new FairMCPointDraw ("LandPoint",kOrange,  kFullSquare);

                                                               
  fMan->AddTask(Track);
  //fMan->AddTask(LandPoints);
    
  fMan->Init();                     
  

}
