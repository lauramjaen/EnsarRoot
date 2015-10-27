evtvis()
{

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo = new FairParRootFileIo();
  parIo->open("parfile.root");
  rtdb->setFirstInput(parIo);
  rtdb->print();

  fRun->SetInputFile("outfile.root");
  fRun->SetOutputFile("test.root");
  
  EnsarEventManager *fMan = new EnsarEventManager();
  EnsarMCTracks    *Track = new EnsarMCTracks ("Monte-Carlo Tracks");

  fMan->AddTask(Track);
  fMan->Init();                     
  
}
