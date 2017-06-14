#include "LHCfData.hpp"

#include <TFile.h>
#include <TROOT.h>

const double LHCfData::d_IP1toTAN = 141050.0; //[mm], ref:A1Phys; //
const double LHCfData::r_BEAMPIPE = 20.0/TMath::Sqrt(2)+5.0+0.5*87.7; //[mm] 87.7 is elliptical vaccum pipe
double LHCfData::sPositionBeamCenter[2] = {0.};

const double r_rapidity_TS     = 5.0;  //[mm]
const double r_rapidity_TLa    = 28.0; //[mm]
const double r_rapidity_TLb    = 35.0; //[mm]
const double r_rapidity_TLc    = 42.0; //[mm]

LHCfData::LHCfData(TString fileName) : cEnt(0)
{
  
  LHCfCollectionTree = new TChain("LHCfCollectionTree");
  LHCfCollectionTree->Add(fileName);
  LHCfCollectionTree->SetMakeClass(1);
  LHCfCollectionTree->SetBranchStatus("*",0);

  LHCfCollectionTree->SetBranchAddress("EventInfo_runNumber", &EventInfo_runNumber, &b_run_N);
  LHCfCollectionTree->SetBranchAddress("EventInfo_EventNumber", &EventInfo_EventNumber, &b_event_gN);
  LHCfCollectionTree->SetBranchAddress("EventInfo_timeStamp_s", &EventInfo_timeStamp_s, &b_time_s);
  LHCfCollectionTree->SetBranchAddress("EventInfo_timeStamp_us", &EventInfo_timeStamp_us, &b_time_us);
  LHCfCollectionTree->SetBranchAddress("EventInfo_BCID", &EventInfo_BCID, &b_BunchID);
  LHCfCollectionTree->SetBranchAddress("EventInfo_ATLAS_l1ID", &EventInfo_ATLAS_l1ID, &b_ATLAS_l1ID);
  LHCfCollectionTree->SetBranchAddress("EventInfo_EventFlags", &EventInfo_EventFlags, &b_eventFlag);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A1_hitNumber", EventPhysValue_A1_hitNumber, &b_A1_hit_N);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A1_PID", EventPhysValue_A1_PID, &b_A1_PID);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A1_energy", EventPhysValue_A1_energy, &b_A1_energy);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A1_positionX", EventPhysValue_A1_positionX, &b_A1_posX);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A1_positionY", EventPhysValue_A1_positionY, &b_A1_posY);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A1_flags", &EventPhysValue_A1_flags, &b_A1_flags);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A2_hitNumber", EventPhysValue_A2_hitNumber, &b_A2_hit_N);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A2_PID", EventPhysValue_A2_PID, &b_A2_PID);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A2_energy", EventPhysValue_A2_energy, &b_A2_energy);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A2_positionX", EventPhysValue_A2_positionX, &b_A2_posX);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A2_positionY", EventPhysValue_A2_positionY, &b_A2_posY);
  LHCfCollectionTree->SetBranchAddress("EventPhysValue_A2_flags", &EventPhysValue_A2_flags, &b_A2_flags);

  nEnt = LHCfCollectionTree->GetEntries();

}
//======== The Original Coordinate System Are Detecter Coordinate       ================= 
//======== Convert Hit Position To Beam Center Coordinate System.       ================= 
//======== Detector Coordinate: (0, 0) located at Center of Small Tower ================= 
//======== Beam Center Coordinate: (0, 0) located at Beam Center (LHCf fitted result) ===========

double LHCfData::GlobalPos_BC(int itower, int ixy,  int ipid, double offsetx,double offsety){
  // Return the hit position in the global beam center coordinate. 
  // The offset paramters should be the beam center position on the global detector coordinate system 
  // If offsetx, offsety are not given, the defaults value which are calculated from 
  // sPositionBeamCenter, are used. The sPositionBeamCenter can be setted by SetPositionBeamCenter()
  if(ixy == 0)
    return A1_positionX(itower, ipid) - offsetx;
  else
    return A1_positionY(itower, ipid) - offsety;
}

// // Static Functions  
int LHCfData::SetPositionBeamCenter(double x,double y){
  sPositionBeamCenter[0] = x;
  sPositionBeamCenter[1] = y;
  return 0;
}


bool LHCfData::CutCircle( double x, double y, double circle_r){

  double r = sqrt( x*x + y*y );

  if( r < circle_r && r < r_BEAMPIPE)
    return true;
  else 
    return false;
}

bool LHCfData::CutHalfCircle(double x, double y, double circle_r){

  double r = sqrt( x*x + y*y );

  if(x-y < 0)
    return false;

  if(r < circle_r && r < r_BEAMPIPE)
    return true;
  else
    return false;
}

bool LHCfData::CutPhi( double gx_bc, double gy_bc, double s_degree, double e_degree){

  Double_t r,phi;
  r   = TMath::Sqrt(gx_bc*gx_bc + gy_bc*gy_bc);
  phi = TMath::Abs(TMath::ASin(gy_bc/r));
  if     (gx_bc>=0. && gy_bc >=0.) phi = phi;
  else if(gx_bc< 0. && gy_bc >=0.) phi = TMath::Pi() - phi;
  else if(gx_bc< 0. && gy_bc < 0.) phi = TMath::Pi() + phi;
  else if(gx_bc>=0. && gy_bc < 0.) phi = 2.*TMath::Pi() - phi; 
  phi = phi * TMath::RadToDeg();  // the valuable range : 0 - 360

  if( (( s_degree < e_degree) && (phi > s_degree && phi < e_degree)) ||  // For cases like 45 - 225
      (( s_degree > e_degree) && (phi > s_degree || phi < e_degree)))   // For cases like 225 - 45
    return true;
  else
    return false;
}

  // +++++++++++++++++ FiducialCut20mm ++++++++++++++++++
bool LHCfData::FiducialCut20mm(int ipid){
  // BC_x : Global Beam Center Coordinate
  // BC_y : Global Beam Center Coordinate
  double BC_x = GlobalPos_BC(0, 0, ipid);  
  double BC_y = GlobalPos_BC(0, 1, ipid);
  // double BC_x = rec->GetResultHitPosition(0, 0);  
  // double BC_y = rec->GetResultHitPosition(0, 1);
  return   CutHalfCircle(BC_x, BC_y, r_rapidity_TS );
}
  // +++++++++++++++++ FiducialCut20mm ++++++++++++++++++
bool LHCfData::FiducialCut20mm(int ipid, double r_min, double r_max, double phi_degree_start, double phi_degree_stop){

  // Global postions from the beam center
  double gx_bc = GlobalPos_BC(0, 0, ipid);
  double gy_bc = GlobalPos_BC(0, 1, ipid);

  // --- phi angle ---
  if( CutPhi( gx_bc, gy_bc, phi_degree_start, phi_degree_stop)==false )
    return false;

  // --- rapidity --
  if( CutCircle( gx_bc, gy_bc, r_min)==false && CutCircle( gx_bc, gy_bc, r_max)==true ) 
    return true;
  else
    return false;

}
  // ++++++++++++++++ FiducialCut40mmA() ++++++++++++++++++++
bool LHCfData::FiducialCut40mmA(int ipid){
  // BC_x : Global Beam Center Coordinate
  // BC_y : Global Beam Center Coordinate
  double BC_x = GlobalPos_BC(1, 0, ipid);  
  double BC_y = GlobalPos_BC(1, 1, ipid);
  // --- phi angle ---
  // double phi_up = (80.0*TMath::Pi() )/180.0 ; 
  // double phi_down = (100.0*TMath::Pi() )/180.0 ; 
  if( CutPhi( BC_x, BC_y, 80, 100)==false )
    return false;
    
  // --- rapidity --
  if( CutCircle( BC_x, BC_y, r_rapidity_TLa)==false && CutCircle( BC_x, BC_y, r_rapidity_TLb)==true ) 
    return true;
  else
    return false;

}

// ++++++++++++++++ FiducialCut40mmB() ++++++++++++++++++++
bool LHCfData::FiducialCut40mmB(int ipid){
  // BC_x : Global Beam Center Coordinate
  // BC_y : Global Beam Center Coordinate
  double BC_x = GlobalPos_BC(1, 0, ipid);  
  double BC_y = GlobalPos_BC(1, 1, ipid);
  // --- phi angle ---
  // double phi_up = (80.0*TMath::Pi() )/180.0 ; 
  // double phi_down = (100.0*TMath::Pi() )/180.0 ; 
  if( CutPhi( BC_x, BC_y, 80, 100)==false )
    return false;
    
  // --- rapidity --
  if( CutCircle( BC_x, BC_y, r_rapidity_TLb)==false && CutCircle( BC_x, BC_y, r_rapidity_TLc)==true ) 
    return true;
  else
    return false;

}

int LHCfData::ApplyRapidityReduction(const int tower, double x, double y, bool photon)
{
  const double r = TMath::Sqrt(TMath::Power(x, 2.) + TMath::Power(y, 2.));
  //cerr << x << "    " << y << "   " <<r << endl;
  if (tower == 0) // small tower
    {
      if(photon)
	return (r < 6. && x > 0) ? 0 : -1;
      else
	return (r < 5. && x > 0) ? 0 : -1;

    }
  else if (tower == 1) // large tower
    {
      if(!photon){
	const double phi = TMath::ATan(-x/y)/TMath::Pi()*180.;
	if (x<0. && r>28. && r<35. && phi > 20. && phi < 40.)
	  return 1;
	if (x<0. && r>35. && r<42. && phi > 30. && phi < 50.)
	  return 2;
	else
	  return -1;
      }else{
	const double phi = TMath::ATan(- y / x) / TMath::Pi() * 180.; // deg
	return (r > 35. && r < 42. && phi > 45. && phi < 65.) ? 1 : -1;
      }
    }

  return -1;
}


  // ++++++++++++++++ Momentum ++++++++++++++++++++
double LHCfData::A1_Momentum(int it,int dir, int ipid, double offsetx,double offsety){
  // Return the momentum [GeV/c].
  // This value is reconstructed from energy, hit position, 
  // neutral beam center positon and detector position.
  // 1st argment is ID of tower(0:20mm,1:40mm). 
  // 2nd is ID of axis (0=x,1=y,2=z) on the global coordinate system.
  // 3rd and 4th are for the offset of beam center position. 
  //   offsetx and offsety indicate the beam center position.
  //   if the beam center was at (-0.6, 0.0), please set as (-0.6, 0.0)
  //   In the previous version, it was (+0.6, 0.0) for the beam center position
  
  if(dir==0 || dir==1){
    double hitpoint;
    hitpoint = GlobalPos_BC(it, dir, ipid, offsetx, offsety);
    return A1_energy(it, ipid)*hitpoint/d_IP1toTAN;
  }
  // For Z
  else if(dir==2){ 
    double x,y;
    x =  GlobalPos_BC(it, 0, ipid, offsetx, offsety);
    y =  GlobalPos_BC(it, 1, ipid, offsetx, offsety);
    return A1_energy(it, ipid)*sqrt(1-(x*x+y*y)/(d_IP1toTAN*d_IP1toTAN));
  }
  return 0.;

}

double LHCfData::A2_Momentum(int it,int dir, int ipid){
  // Return the momentum [GeV/c].
  // This value is reconstructed from energy, hit position, 
  // neutral beam center positon and detector position.
  // 1st argment is ID of tower(0:25mm,1:32mm). 
  // 2nd is ID of axis (0=x,1=y,2=z) on the global coordinate system.
  // The output of Arm2's position is already Beam Center coordinate system. 
  if(dir==0){
    double hitpoint;
    hitpoint = A2_positionX(it, ipid);
    return A2_energy(it, ipid)*hitpoint/d_IP1toTAN;
  }
  else if(dir == 1){
    double hitpoint;
    hitpoint = A2_positionY(it, ipid);
    return A2_energy(it, ipid)*hitpoint/d_IP1toTAN;
  }
  // For Z
  else if(dir==2){ 
    double x,y;
    x =  A2_positionX(it, ipid);
    y =  A2_positionY(it, ipid);
    return A2_energy(it, ipid)*sqrt(1-(x*x+y*y)/(d_IP1toTAN*d_IP1toTAN));
  }
  return 0.;
}


Double_t LHCfData::A1_Px(const UInt_t tower, const UInt_t pid){
  return A1_Momentum(tower, 0, pid);
}

Double_t LHCfData::A1_Py(const UInt_t tower, const UInt_t pid){
  return A1_Momentum(tower, 1, pid);
}

Double_t LHCfData::A1_Pz(const UInt_t tower, const UInt_t pid){
  return A1_Momentum(tower, 2, pid);
}
  
Double_t LHCfData::A2_Px(const UInt_t tower, const UInt_t pid){
  return A2_Momentum(tower, 0, pid);
}

Double_t LHCfData::A2_Py(const UInt_t tower, const UInt_t pid){
  return A2_Momentum(tower, 1, pid);
}

Double_t LHCfData::A2_Pz(const UInt_t tower, const UInt_t pid){
  return A2_Momentum(tower, 2, pid);
}

Double_t LHCfData::A1_Pt(const UInt_t tower, const UInt_t pid){
  return TMath::Sqrt( TMath::Power(A1_Px(tower, pid), 2.) + TMath::Power(A1_Py(tower, pid), 2.) );
}

Double_t LHCfData::A2_Pt(const UInt_t tower, const UInt_t pid){
  return TMath::Sqrt( TMath::Power(A2_Px(tower, pid), 2.) + TMath::Power(A2_Py(tower, pid), 2.) );
}

Double_t LHCfData::A1_Eta(const UInt_t tower, const UInt_t pid){
  Double_t A1_P = TMath::Sqrt(TMath::Power(A1_Px(tower, pid), 2.0) + TMath::Power(A1_Py(tower, pid), 2.0) + TMath::Power(A1_Pz(tower, pid), 2.0));
  return -TMath::Log( (A1_P+A1_Pz(tower, pid))/(A1_P-A1_Pz(tower, pid)));
}

Double_t LHCfData::A2_Eta(const UInt_t tower, const UInt_t pid){
  Double_t A2_P = TMath::Sqrt(TMath::Power(A2_Px(tower, pid), 2.) + TMath::Power(A2_Py(tower, pid), 2.) + TMath::Power(A2_Pz(tower, pid), 2.));
  return -TMath::Log( (A2_P+A2_Pz(tower, pid))/(A2_P-A2_Pz(tower, pid)));
}


