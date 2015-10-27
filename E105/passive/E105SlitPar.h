/*
 * ********************************
 * Interface: E105SlitPar         *
 * Author: M. Babai               *
 * Version:                       *
 * Licence:                       *
 * ********************************
 */
#pragma once
#ifndef E105SLITPAR_H
#define E105SLITPAR_H

#include <string>

#include "FairParGenericSet.h"
#include "db_detector_def.h"
class FairDbTableRow;
class FairParamList;

class E105SlitPar : public FairParGenericSet
{
 public :
  /**
   * Constructor:
   *
   *@param name The name of the container.
   *@param title The title of the container.
   *@param context The defined context.
   */
  E105SlitPar (const char* name="E105SlitPar",
               const char* title="E105 Tutorial parameter",
               const char* context="DBDefaultContext");
  
  /**
   * Destructor:
   */
  virtual ~E105SlitPar();

  /**
   * Clear the values of the the current container.
   */  
  void   clear();
  
  /**
   *Adds the current parameter to the parameter list.
   *
   *@param list The list to add the current parameter to.
   */
  void   putParams(FairParamList* list);

  /**
   * Find the current parameter in the current list.
   *@param list the list of parameters to search in.
   */
  Bool_t getParams(FairParamList* list);
  
  /**
   * Print the current parameter container.
   */
  void Print();
  
  /** Accessor functions **/
  inline Double_t GetS2Target() const {
    return fS2Target;
  }
  
  inline Double_t GetSlitShift() const {
    return fSlitShift;
  }
  
  inline void SetS2Target(Double_t x = 3.0) {
    fS2Target = x;
  }

  inline void SetSlitShift(Double_t y = -0.75) {
    fSlitShift = y;
  }

  /**
   * SQL descriptors for the current parameter.
   */
  virtual std::string GetTableDescr(const char* alternateName = 0);
  
  /**
   * Create a row in SQL table for the current parameter.
   */
  //inline virtual FairDbTableRow* CreateTableRow() const {
    //return new E105SlitPar();
  //}
  virtual FairDbObjTableMap* CreateObjTableMap() const {
      return new E105SlitPar();
    }
  
  // I/O member functions to perform database IO.
  /*virtual void Fill(FairDbResultSet& rs,
                    const FairDbValidityRec* vrec);
  
  virtual void Store(FairDbOutRowStream& ors,
                     const FairDbValidityRec* vrec) const;*/
  // Atomic IO (intrinsic)
    virtual void Fill(FairDbResultPool& res_in,
                      const FairDbValRecord* valrec);
    virtual void Store(FairDbOutTableBuffer& res_out,
                       const FairDbValRecord* valrec) const;
  // Global IO using run_id
  virtual void Fill(UInt_t rid);
  
  virtual void Store(UInt_t rid);
  
  // Validity frame definition
  //virtual ValContext GetContextDTF(UInt_t rid) {
  
    /* return ValContext(Detector::kGfi, */
    /*                   SimFlag::kData, */
    /*                   ValTimeStamp(rid)); */
    //return ValContext (Detector::kSlit, SimFlag::kData, rid);
  //}
  
  // Validity frame definition
    virtual ValCondition GetContext(UInt_t rid) {
      return ValCondition(Detector::kSlit,
                          DataType::kData,
                          ValTimeStamp(rid));
    }
  
 private:
  // Copy and assignment
  E105SlitPar( E105SlitPar const& oth);
  E105SlitPar operator=( E105SlitPar const&);

  // Example Strip Parameters
  Double_t fS2Target;// = 3.0;      // cm   //----------> Par to DB
  Double_t fSlitShift;// = -0.75;   // cm   //----------> Par to DB
  
  ClassDef(E105SlitPar, 0); //
};
#endif /* DBTUTPAR_H*/
