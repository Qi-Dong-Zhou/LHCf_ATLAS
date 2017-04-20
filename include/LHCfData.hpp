#ifndef LHCfData_hpp
#define LHCfData_hpp

#include <iostream>
#include <string>
#include <iomanip>
#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include <TFile.h>
#include <TMath.h>

using namespace std;

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class LHCfData {
public :
   TChain          *LHCfCollectionTree;   //!pointer to the analyzed TTree or TChain
   // Number of entries
   Long64_t nEnt;
   Long64_t cEnt;

   // Declaration of leaf types
   UInt_t          EventInfo_runNumber;
   UInt_t          EventInfo_EventNumber;
   UInt_t          EventInfo_timeStamp_s;
   UInt_t          EventInfo_timeStamp_us;
   UInt_t          EventInfo_BCID;
   UInt_t          EventInfo_ATLAS_l1ID;
   UInt_t          EventInfo_EventFlags;
   UInt_t          EventPhysValue_A1_hitNumber[2][2];
   UInt_t          EventPhysValue_A1_PID[2][2];
   Double_t        EventPhysValue_A1_energy[2][2];
   Double_t        EventPhysValue_A1_positionX[2][2];
   Double_t        EventPhysValue_A1_positionY[2][2];
   UInt_t          EventPhysValue_A1_flags;
   UInt_t          EventPhysValue_A2_hitNumber[2][2];
   UInt_t          EventPhysValue_A2_PID[2][2];
   Double_t        EventPhysValue_A2_energy[2][2];
   Double_t        EventPhysValue_A2_positionX[2][2];
   Double_t        EventPhysValue_A2_positionY[2][2];
   UInt_t          EventPhysValue_A2_flags;

   // List of branches
   TBranch        *b_run_N;   //!
   TBranch        *b_event_gN;   //!
   TBranch        *b_time_s;   //!
   TBranch        *b_time_us;   //!
   TBranch        *b_BunchID;   //!
   TBranch        *b_ATLAS_l1ID;   //!
   TBranch        *b_eventFlag;   //!
   TBranch        *b_A1_hit_N;   //!
   TBranch        *b_A1_PID;   //!
   TBranch        *b_A1_energy;   //!
   TBranch        *b_A1_posX;   //!
   TBranch        *b_A1_posY;   //!
   TBranch        *b_A1_flags;   //!
   TBranch        *b_A2_hit_N;   //!
   TBranch        *b_A2_PID;   //!
   TBranch        *b_A2_energy;   //!
   TBranch        *b_A2_posX;   //!
   TBranch        *b_A2_posY;   //!
   TBranch        *b_A2_flags;   //!
  
public :
   LHCfData(TString fileName);
   virtual ~LHCfData(){
     if (!LHCfCollectionTree) return;
     delete LHCfCollectionTree->GetCurrentFile();
   };
   
   virtual void AddTreePath(const TString & nextPath) {
     LHCfCollectionTree->Add(nextPath);
     nEnt = LHCfCollectionTree->GetEntries();
   }

   virtual Long64_t getEntries() const {
     return nEnt;
   }
   
   virtual void getEntryTree(UInt_t iEvt) {
     LHCfCollectionTree->GetEntry(iEvt);
   }

   virtual Bool_t nextEntry() {
     Long64_t nnEnt = LHCfCollectionTree->GetEntries();
     if( cEnt < nnEnt ) {
       getEntryTree(cEnt);
       cEnt++;
       return kTRUE;
     } else {
       return kFALSE;
     }
   }

  //====== ANALYSIS FUNTION ===========
  static const int ELEMAG = 0;                  //! 
  static const int HADRON = 1;                  //!

  static const double d_IP1toTAN; //[mm] Distance from IP1 to TAN 
  static const double r_BEAMPIPE; //[mm] elliptical vaccum pipe

  bool FiducialCut20mm(int ipid);

protected:
  static double sPositionBeamCenter[2];


private: LHCfData() {};  
public: 
  virtual UInt_t EventNumber() const {
    return EventInfo_EventNumber;
  }
  
  virtual UInt_t RunNumber() const {
    return EventInfo_runNumber;
  }

  virtual UInt_t TimeStamp() const {
    return EventInfo_timeStamp_s + EventInfo_timeStamp_us*1e-6;
  }
  
  virtual UInt_t ATLASLevel1ID() const {
    return EventInfo_ATLAS_l1ID;
  }

  virtual UInt_t BcId() const {
    return EventInfo_BCID;
  }
  // EventQuality: "SINGLE" hit on either SmallTower or LargeTower -> Good qualtiy, return true
  virtual Bool_t EventQuality() const{
    if(EventInfo_EventFlags & 0x1)
      return kTRUE;
    else
      return kFALSE;
  }
  //==============================
  // Get physics value of arm1
  //===============================

  // A1_EventQuality: "SINGLE" hit on either SmallTower or LargeTower -> Good qualtiy, return true
  virtual Bool_t A1_EventQuality(const UInt_t tower) const{
    if((EventPhysValue_A1_flags >> tower) & 0x1)
      return kTRUE;
    else
      return kFALSE;
  }

  // Get hit number (maximum is 2) of each tower of arm1, even EventQuality is bad
  virtual UInt_t A1_hitNumber( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A1_hitNumber[tower][pid];
  }


  // Physics value (PID, energy position) is only available for the corresponding hit of each tower

  // 0: photon, 1: hadron
  // The 2nd argment "pid" means the photon analysis or hadron analysis
  virtual UInt_t A1_PID( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A1_PID[tower][pid];
  }
  
  // 0: photon, 1: hadron, 2: photon@photon analysis but hadron@hadron analysis,
                         //3: hadron@photon analysis but photon@hadron analysis
  //================= Be careful to use 2 or 3 ================================
  virtual UInt_t A1_FinalPID( const UInt_t tower) const{
    if(A1_PID(tower, 0) == 0 && A1_PID(tower, 1) == 0)
      return 0;
    else if (A1_PID(tower, 0) == 1 && A1_PID(tower, 1) == 1)
      return 1;
    else if (A1_PID(tower, 0) == 0 && A1_PID(tower, 1) == 1)
      return 2;
    else if (A1_PID(tower, 0) == 1 && A1_PID(tower, 1) == 0)
      return 3;
    else 
      return 9;
  }

  virtual Double_t A1_energy( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A1_energy[tower][pid];
  }
  // Coordinate; (0, 0) is the center of arm1 small tower
  virtual Double_t A1_positionX( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A1_positionX[tower][pid];
  }

  virtual Double_t A1_positionY( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A1_positionY[tower][pid];
  }
  
  //==============================
  // Get physics value of arm2
  //===============================

  // A2_EventQuality: "SINGLE" hit on either SmallTower or LargeTower -> Good qualtiy, return true
  virtual Bool_t A2_EventQuality(const UInt_t tower) const{
    if((EventPhysValue_A2_flags >> tower) & 0x1)
      return kTRUE;
    else
      return kFALSE;
  }

  // Get hit number (maximum is 2) of each tower of arm1, even EventQuality is bad
  virtual UInt_t A2_hitNumber( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A2_hitNumber[tower][pid];
  }


  // Physics value (PID, energy position) is only available for the corresponding hit of each tower

  // 0: photon, 1: hadron  
  // The 2nd argment "pid" means the photon analysis or hadron analysis
  virtual UInt_t A2_PID( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A2_PID[tower][pid];
  }

  // 0: photon, 1: hadron, 2: photon@photon analysis but hadron@hadron analysis,
                         //3: hadron@photon analysis but photon@hadron analysis
  //================= Be careful to use 2 or 3 ================================
  virtual UInt_t A2_FinalPID( const UInt_t tower) const{
    if(A2_PID(tower, 0) == 0 && A2_PID(tower, 1) == 0)
      return 0;
    else if (A2_PID(tower, 0) == 1 && A2_PID(tower, 1) == 1)
      return 1;
    else if (A2_PID(tower, 0) == 0 && A2_PID(tower, 1) == 1)
      return 2;
    else if (A2_PID(tower, 0) == 1 && A2_PID(tower, 1) == 0)
      return 3;
    else 
      return 9;
  }
  
  virtual Double_t A2_energy( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A2_energy[tower][pid];
  }
  // Coordinate; (0, 0) is the center of arm2 small tower
  virtual Double_t A2_positionX( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A2_positionX[tower][pid];
  }

  virtual Double_t A2_positionY( const UInt_t tower, const UInt_t pid) const{
    return EventPhysValue_A2_positionY[tower][pid];
  }

 //======== The Original Coordinate System Are Detecter Coordinate       ================= 
 //======== Convert Hit Position To Beam Center Coordinate System.       ================= 
 //======== Detector Coordinate: (0, 0) located at Center of Small Tower ================= 
 //======== Beam Center Coordinate: (0, 0) located at Beam Center (LHCf fitted result) =========== 

  double GlobalPos_BC(int tower, int ixy, int ipid,
                      double offsetx=GetOffsetBC(0),
                      double offsety=GetOffsetBC(1));  // Hit Position in the global beam center coordinate 

  // Static Functions  
  static int    SetPositionBeamCenter(double x,double y);// Set the Beam Center Position in the Global_Detertor coordinate.[mm]
  static double GetOffsetBC(int xy){return sPositionBeamCenter[xy];}      // = sPositionBeamCenter[xy]   

  bool CutCircle( double x, double y, double circle_r);
  bool CutHalfCircle(double x, double y, double circle_r);
  bool CutPhi( double gx_bc, double gy_bc, double s_degree, double e_degree);
  // +++++++++++++++++ FiducialCut20mm ++++++++++++++++++
  bool FiducialCut20mm(int ipid, double r_min, double r_max, double phi_degree_start, double phi_degree_stop);
  // ++++++++++++++++ FiducialCut40mmA() ++++++++++++++++++++
  bool FiducialCut40mmA(int ipid);
  // ++++++++++++++++ FiducialCut40mmB() ++++++++++++++++++++
  bool FiducialCut40mmB(int ipid);
  int ApplyRapidityReduction(const int tower, double x, double y, bool photon);
  // ++++++++++++++++ Momentum ++++++++++++++++++++
  double A1_Momentum(int it,int dir, int ipid, double offsetx=GetOffsetBC(0),
                                               double offsety=GetOffsetBC(1));
    // Return the momentum [GeV/c].
    // This value is reconstructed from energy, hit position, 
    // neutral beam center positon and detector position.
    // 1st argment is ID of tower(0:20mm,1:40mm). 
    // 2nd is ID of axis (0=x,1=y,2=z) on the global coordinate system.
    // 3rd and 4th are for the offset of beam center position. 
    //   offsetx and offsety indicate the beam center position.
    //   if the beam center was at (-0.6, 0.0), please set as (-0.6, 0.0)
    //   In the previous version, it was (+0.6, 0.0) for the beam center position
  double A2_Momentum(int it,int dir, int ipid);
    // Return the momentum [GeV/c].
    // This value is reconstructed from energy, hit position, 
    // neutral beam center positon and detector position.
    // 1st argment is ID of tower(0:25mm,1:32mm). 
    // 2nd is ID of axis (0=x,1=y,2=z) on the global coordinate system.
    // The output of Arm2's position is already Beam Center coordinate system. 

  Double_t A1_Px(const UInt_t tower, const UInt_t pid);
  Double_t A1_Py(const UInt_t tower, const UInt_t pid);
  Double_t A1_Pz(const UInt_t tower, const UInt_t pid);
  Double_t A2_Px(const UInt_t tower, const UInt_t pid);
  Double_t A2_Py(const UInt_t tower, const UInt_t pid);
  Double_t A2_Pz(const UInt_t tower, const UInt_t pid);
  Double_t A1_Pt(const UInt_t tower, const UInt_t pid);
  Double_t A2_Pt(const UInt_t tower, const UInt_t pid);
  Double_t A1_Eta(const UInt_t tower, const UInt_t pid);
  Double_t A2_Eta(const UInt_t tower, const UInt_t pid);

};

#endif


