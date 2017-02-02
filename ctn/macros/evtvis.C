void evtvis() {

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo = new FairParRootFileIo();
  parIo->open("outpar.root");
  rtdb->setFirstInput(parIo);
  rtdb->print();

  fRun->SetInputFile("outsim.root");
  fRun->SetOutputFile("test.root");
  
  EnsarEventManager *fMan = new EnsarEventManager();
  EnsarMCTracks    *Track = new EnsarMCTracks ("Monte-Carlo Tracks");

  fMan->AddTask(Track);
  fMan->Init();                     
  
}
