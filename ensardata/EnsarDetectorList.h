// -------------------------------------------------------------------------
// -----                EnsarDetectorList header file                  -----
// -----               Created 11/09/12  by P.Cabanelas                -----
// -------------------------------------------------------------------------


/** Unique identifier for detector systems **/

#ifndef ENSARDETECTORLIST_H
#define ENSARDETECTORLIST_H 1

enum DetectorId { kUNKNOWN, kMYDET, kTSCINTDET, kTSILIDET, kCAL, kCALIFA, kRPC, kTra };
    
/** Unique identifier for Point and Hit types **/

enum fDetectorType { kUnknown, kMyDetPoint, kTScintiDet, kTSiliDet, kCalPoint, kCalifaPoint, kRPCPoint,kRPCHit };

enum SensorSide { kTOP, kBOTTOM };

#endif
