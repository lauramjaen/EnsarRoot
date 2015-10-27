evtvis()
{

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna *fRun= new FairRunAna();

  // -----   Runtime database   ---------------------------------------------
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open("simpar.root");
  rtdb->setFirstInput(parIo1);
  rtdb->print();


  fRun->SetInputFile("simout.root");
  fRun->SetOutputFile("test.root");
  
  EnsarEventManager *fMan = new EnsarEventManager();
  EnsarMCTracks    *Track = new EnsarMCTracks("Monte-Carlo Tracks");

  fMan->AddTask(Track);

  fMan->Init();  

  //fMan->MultiView(); //not working properly yet                     
  
}
