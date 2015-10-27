// -------------------------------------------------------------------------
// -----                      R3BToFWRaw header file   	               -----
// -----                  Created 08/10/12  by J.L. Rodriguez 	       -----
//								       -----
// ----- Modified:						       -----
//								       -----
// -------------------------------------------------------------------------
/**  R3BToFWRaw.h
 **  A ToFW raw is the representation of the ToFW data
 **  extracted (unpacked and sorted) from the lmd.
 **/

#ifndef R3BToFWRaw_H
#define R3BToFWRaw_H

#include "FairMultiLinkedData.h"

#define ToFW_Nbplastics 28

class R3BToFWRaw : public FairMultiLinkedData
{

 public:

  /** Default constructor **/
  R3BToFWRaw();

  /** Constructor with arguments
   *@param ToFW_up_Eraw[ToFW_Nbplastics]      Charge up
   *@param ToFW_down_Eraw[ToFW_Nbplastics]    Charge down
   *@param ToFW_up_Traw[ToFW_Nbplastics]      Time up
   *@param ToFW_down_Traw[ToFW_Nbplastics]    Time down
   **/

  R3BToFWRaw( UShort_t ToFW_up_Eraw[ToFW_Nbplastics], UShort_t ToFW_down_Eraw[ToFW_Nbplastics], Double_t ToFW_up_Traw[ToFW_Nbplastics], 
                   Double_t ToFW_down_Traw[ToFW_Nbplastics]);

  /** Copy constructor **/
  R3BToFWRaw(const R3BToFWRaw& raw) { *this = raw; };

  /** Destructor **/
  virtual ~R3BToFWRaw();

  /** Accessors **/
 
  UShort_t GetToFWChargeup(Int_t i)    const { return fToFW_up_Eraw[i]; }
  UShort_t GetToFWChargedown(Int_t i)  const { return fToFW_down_Eraw[i]; }
  Double_t GetToFWTrawup(Int_t i)      const { return fToFW_up_Traw[i]; }
  Double_t GetToFWTrawdown(Int_t i)    const { return fToFW_down_Traw[i]; }
  
  /** Modifiers **/

  void SetToFWChargeup(Int_t Nbplastic, UShort_t ToFW_up_E);
  void SetToFWChargedown(Int_t Nbplastic, UShort_t ToFW_down_E);
  void SetToFWTrawup(Int_t Nbplastic, Double_t ToFW_up_T);
  void SetToFWTrawdown(Int_t Nbplastic, Double_t ToFW_down_T);

  /** Output to screen **/
  virtual void Print(const Option_t* opt) const;

 protected:

  UShort_t fToFW_up_Eraw[ToFW_Nbplastics];
  UShort_t fToFW_down_Eraw[ToFW_Nbplastics];
  Double_t fToFW_up_Traw[ToFW_Nbplastics];
  Double_t fToFW_down_Traw[ToFW_Nbplastics];

  ClassDef(R3BToFWRaw,1)

};

inline void R3BToFWRaw::SetToFWChargeup(Int_t Nbplastic, UShort_t ToFW_up_E){
  fToFW_up_Eraw[Nbplastic]=ToFW_up_E;
}

inline void R3BToFWRaw::SetToFWChargedown(Int_t Nbplastic, UShort_t ToFW_down_E){
  fToFW_down_Eraw[Nbplastic]=ToFW_down_E;
}

inline void R3BToFWRaw::SetToFWTrawup(Int_t Nbplastic, Double_t ToFW_up_T){
  fToFW_up_Traw[Nbplastic]=ToFW_up_T;
}

inline void R3BToFWRaw::SetToFWTrawdown(Int_t Nbplastic, Double_t ToFW_down_T){
  fToFW_down_Traw[Nbplastic]=ToFW_down_T;
}

#endif
