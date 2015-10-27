/*
 * ********************************
 *     Interface: E105SlitPar     *
 * Author: M. Babai               *
 * Version:                       *
 * Licence:                       *
 * ********************************
 */
#include "E105SlitPar.h"

#include <iostream>
#include <memory>

#include "FairDbStatement.h"
#include "FairParamList.h"
#include <string>

ClassImp(E105SlitPar);

#include "FairDbResPtr.tpl"
template class  FairDbResultPtr<E105SlitPar>;

#include "FairDbWriter.tpl"
template class  FairDbWriter<E105SlitPar>;

/**
 * Constructor:
 *
 *@param name The name of the container.
 *@param title The title of the container.
 *@param context The defined context.
 */
E105SlitPar::E105SlitPar(const char* name, const char* title, const char* context)
: FairParGenericSet(name,title,context)
{
  clear();
}

/**
 * Destructor:
 */
E105SlitPar::~E105SlitPar()
{}


string E105SlitPar::GetTableDescr(const char* alternateName)
{
  string sql("create table ");
  if ( alternateName ) { sql += alternateName; }
  else { sql += "E105SLITPAR"; }
  sql += "( SEQNO          INT NOT NULL,";
  sql += "  ROW_COUNTER    INT NOT NULL,";
  sql += "  S2TARGET       DOUBLE,";
  sql += "  SLITSHIFT      DOUBLE,";
  sql += "  primary key(SEQNO,ROW_COUNTER))";
  return sql;
}

void E105SlitPar::putParams(FairParamList* list)
{
  std::cout<<"-I- E105SlitPar::putParams() called\n";
  if(!list) { return; }
  list->add("S2Target",  fS2Target);
  list->add("SlitShift", fSlitShift);
}

Bool_t E105SlitPar::getParams(FairParamList* list)
{
  if (!list) { return kFALSE; }
  if (!list->fill("S2Target", &fS2Target)) { return kFALSE; }
  if (!list->fill("SlitShift", &fSlitShift)) { return kFALSE; }
  return kTRUE;
}

void E105SlitPar::clear()
{
  cout << "-I- E105SlitPar::clear -> clear called \n" << endl;
  fS2Target = fSlitShift = 0.000;
}


void E105SlitPar::Print()
{
  std::cout<<"Print() SQL E105SlitPar Parameters:\n"
           <<"   s2target    = "   << fS2Target
           <<"\n   slitShift   = " << fSlitShift
           << std::endl;
}

void E105SlitPar::Fill( FairDbResultSet& rs,
                        const FairDbValidityRec* vrec)
{
  std::cout << " -I- E105SlitPar::Fill(xxx) called\n";

  rs >> fS2Target  >>  fSlitShift;
  
  std::cout << " -I- E105SlitPar::Filled(xxx) called values===> "
            << fS2Target
            << " : "
            << fSlitShift
            << endl;
}

void E105SlitPar::Store(FairDbOutRowStream& ors,
                        const FairDbValidityRec* vrec) const
{
  std::cerr << "<-I-> E105SlitPar::Store(xxx) called with thefollowing values: "
            << fS2Target << " : " << fSlitShift
            << "\n";
  ors << fS2Target  << fSlitShift ;

  std::cout << "\n=========================\n<INFO> FairDbOutRowStream = " <<
    ors.GetCSV() << "\n=============\n";
}


void E105SlitPar::Fill(UInt_t rid)
{

  ValTimeStamp ts(rid);
  ValContext context(Detector::kSlit, SimFlag::kData, ts);

  FairDbResultPtr<E105SlitPar> rsCal(context, GetVersion());
  Int_t numRows = rsCal.GetNumRows();
    
  // Just use the latest row entry
  if ( numRows > 1 ) {
    numRows = 1;
  }
  
  for (int i = 0; i < numRows; ++i) {
    E105SlitPar* cgd = (E105SlitPar*) rsCal.GetRow(i);
    if (!cgd) {
      continue;
    }

    fS2Target  = cgd->GetS2Target();
    fSlitShift = cgd->GetSlitShift();
  }
}

void E105SlitPar::Store(UInt_t rid)
{
  // In this example we are fixing the database entry point. In the future
  // a variable entry can be set via the runtime DB directly.
  Int_t dbEntry = 0;
  Bool_t fail= kFALSE;
  
  FairDbMultConnector* fMultConn = FairDbTableProxyRegistry::Instance().fMultConnector;
  std::auto_ptr<FairDbStatement> stmtDbn(fMultConn->CreateStatement(dbEntry));
  
  if ( ! stmtDbn.get() ) {
    cout << "-E-  E105SlitPar::Store()  Cannot get a statement for cascade entry " << dbEntry
         << "\n    Please check the ENV_TSQL_* environment.  Quitting ... " << endl;
    exit(1);
  }

  // The definition of FairE105SlitPar is centralised in the FairE105SlitPar class.
  // The corresponding SQL is executed as follows:
  std::vector<std::string> sql_cmds;
  TString atr(GetName());
  atr.ToUpper();
  
  // Check if for this connection entry the table already exists.
  // If not call the Class Table Descriptor function
  if (! fMultConn->GetConnection(dbEntry)->TableExists("E105SLITPAR") ) {
    sql_cmds.push_back(FairDb::GetValDescr("E105SLITPAR").Data());
    sql_cmds.push_back(E105SlitPar::GetTableDescr());
  }

  // Now execute the assemble list of SQL commands.
  std::vector<std::string>::iterator itr(sql_cmds.begin()), itrEnd(sql_cmds.end());
  
  while( itr != itrEnd ) {
    std::string& sql_cmd(*itr++);
    stmtDbn->ExecuteUpdate(sql_cmd.c_str());
    if ( stmtDbn->PrintExceptions() ) {
      fail = true;
      std::cout << "\n\n\n-E- E105SlitPar::Store() ******* Error Executing SQL commands ***********\n\n\n"
                << std::endl;
    }
    std::cout << "\n\n\n\n\tsql_cmd = " << sql_cmd << "\n\n\n\n";
  }
  
  // Refresh list of tables in connected database
  // for the choosen DB entry
  fMultConn->GetConnection(dbEntry)->SetTableExists();
  std::cout << "-I-FairE105SlitPar ******* Store() called  ******** with RID#  "
            << rid << std::endl;

  FairDbWriter<E105SlitPar>  aW(this->GetRangeDTF(rid),
                                GetAggregateNo(), // Composite or Simple IO
                                GetVersion(),  // Parameter version ( Set via the Container Factory)
                                ValTimeStamp(0,0),0,"E105SlitPar parameters", "E105SLITPAR");
  aW.SetDbNo(dbEntry);
  aW.SetLogComment("E105SlitPar Test Parameter");
  aW << (*this);
  if ( ! aW.Close() ) {
    fail = true;
    cout << "-E- E105SlitPar::Store()  Cannot do IO on class# " << GetName() <<  endl;
  }
  
  // Print Info on the Central Log
  FairDbExceptionLog::GetGELog().Print();
  
  // end of store()
}
