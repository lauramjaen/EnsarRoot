
#include <TStopwatch.h>
#include <FairLmdSource.h>
#include <R3BCaloUnpack.h>
#include <FairRunOnline.h>
//#include <R3BAladinFieldMap.h>
#include <R3BCaloRawAna.h>
#include <FairParRootFileIo.h>
//#include <R3BFieldPar.h>
#include <FairRuntimeDb.h>
#include <boost/program_options.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <vector>
#include <string>
#include <stdexcept>
#include <FairLogger.h>
#include <iostream>
int main(int argc, char** argv)
{
  std::vector<std::string> in;
  std::string out;
  bool timer;
  int loglevel;
  {
    using namespace boost::program_options;
    using namespace std;
  //argument parsing with boost: copied from Max's unpacker. 
    options_description opt_desc("Unpacker options");
    opt_desc.add_options()
      ("help,h", "Show this help message")
      ("in,i", value< vector<string> >()->default_value({}, ""), "Input lmd files")
      ("out,o", value<string>()->default_value("/dev/null"), "Output root file")
      ("timer,t", value<bool>()->default_value(false)->implicit_value(true), "use timer")
      ("loglevel,l", value<int>()->default_value(1)
       ->notifier([](const int &n){*(const_cast<int*>(&n))=std::min((int)(sizeof(LogLevelString)/sizeof(LogLevelString[0]))-1, std::max(n, 0));}),
       "log level (screen)")
      //("num-events", value<uint32_t>(&max_num_events)->default_value(0), "Maximum number of events to process")
      //("no-traces", "Disable unpacking of traces")
      //("modules", value<uint16_t>(&num_modules)->default_value(3), "Number of FEBEX modules")
      //("channels", value<uint16_t>(&num_channels)->default_value(4), "Number of channels per module")
      ;
  
    positional_options_description p_desc;
    p_desc.add("in", -1);
    variables_map args;
    store(command_line_parser(argc, argv).options(opt_desc)
    	  .positional(p_desc).run(), args);
    notify(args);
#define ASSIGN_ARG(r, data, elem) elem=args[BOOST_PP_STRINGIZE(elem)].as<BOOST_TYPEOF(elem)>();
    BOOST_PP_SEQ_FOR_EACH(ASSIGN_ARG, , (in)(out)(timer)(loglevel) ) ; 

    if (args.count("help")) {
      cout << opt_desc << "\n";
      return 1;
    }

  }
  std::cout<<"setting loglevel to "<<loglevel<<" ("<< LogLevelString[loglevel]<<")\n\n";
  FairLogger::GetLogger()->SetLogScreenLevel(LogLevelString[loglevel]);
  std::cout<<std::showbase<<std::hex;
  std::cerr<<std::showbase<<std::hex;
  if (in.empty())
    {
      LOG(FATAL)<<("FairLmdSource will segfault without input files, please give at least one.");
      throw std::runtime_error("input missing");
    }

  TStopwatch* tsw;
  if (timer)
    {
      tsw=new TStopwatch;
      tsw->Start();
    }

  // Run over all events
  Int_t nev = -1;
  
  
  // Create source with unpackers ----------------------------------------------
//  FairRemoteSource* source  = new FairRemoteSource("lxi047");
  FairLmdSource* source  = new FairLmdSource();
  for(const auto i: in)
    source->AddFile(i.c_str());

//  char* strCalDir = "/home/work/example/";
  // Short_t type = 10;
  //Short_t subType = 1;
  //Short_t procId = 1;
  //Short_t subCrate = -1; // All sub-crates
  //Short_t control = 9;

  int16_t type=100;
  int16_t subType=10000;
  int16_t procId=2;
  int16_t control= 9;
  int16_t subCrate=-1;
    
  source->AddUnpacker(new R3BCaloUnpack(const_cast<char*>(""), // char*, really?
					type, subType,    // what is this, ROOT?
					procId, subCrate, control));

  // ---------------------------------------------------------------------------


  // Create online run ---------------------------------------------------------
  FairRunOnline* run = new FairRunOnline(source);
  
  run->SetOutputFile(out.c_str());
  //  run->SetGenerateHtml(kTRUE);
  // ---------------------------------------------------------------------------
  
  
  // Create ALADIN field map ---------------------------------------------------
  //  R3BAladinFieldMap* magField = new R3BAladinFieldMap("AladinMaps");
  //Double_t fMeasCurrent = 2500.;// I_current [A]
  //magField->SetCurrent(fMeasCurrent);
  //magField->SetScale(1.);
  //run->SetField(magField);
  // ---------------------------------------------------------------------------
  
  // Add analysis task ---------------------------------------------------------
  R3BCaloRawAna* ana = new R3BCaloRawAna();
  run->AddTask(ana);
  // ---------------------------------------------------------------------------
  
  // Initialize ----------------------------------------------------------------
  run->Init();
  // ---------------------------------------------------------------------------
  
  // Runtime data base ---------------------------------------------------------
  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  //3BFieldPar* fieldPar = (R3BFieldPar*) rtdb->getContainer("R3BFieldPar");
  //fieldPar->SetParameters(magField);
  //fieldPar->setChanged();
  //Bool_t kParameterMerged = kTRUE;
  //FairParRootFileIo* parOut = new FairParRootFileIo(kParameterMerged);
  //parOut->open("r3bpar.root");
  //rtdb->setOutput(parOut);
  //rtdb->saveOutput();
  //rtdb->print();
  // ---------------------------------------------------------------------------
  
  
  // Run -----------------------------------------------------------------------
  run->Run(nev, 0);
  // ---------------------------------------------------------------------------

  if (timer)
    {
      tsw->Stop();
      Double_t rtime = tsw->RealTime();
      Double_t ctime = tsw->CpuTime();
      std::cout << "Real time " << rtime << " s, CPU time " << ctime
		<< "s" << std::endl << std::endl;
    }
  return 0;
}


