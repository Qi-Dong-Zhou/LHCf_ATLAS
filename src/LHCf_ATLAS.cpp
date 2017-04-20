#include "../include/LHCfData.hpp"
#include "../include/ATLASData.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TH1D.h>
#include <TSystem.h>

using namespace std;
const int gE_threshold = 200;
const int nE_threshold = 500;
const double bc[2] = {-2.04, 1.44};

const int NDV[] = {31, 27, 40, 24, 21};
 double TSpE_bins[31+1] = {                   200.,  300.,  400.,  500.,  600.,  700.,  800.,  900., 
                               1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900.,
                               2000., 2200., 2400., 2600., 2800., 3000., 3200., 3400., 3600., 3800.,
                               4000., 4500., 5000., 6000.};
 double TLpE_bins[27+1] = {                   200.,  300.,  400.,  500.,  600.,  700.,  800.,  900., 
                               1000., 1100., 1200., 1300., 1400., 1500., 1600., 1700., 1800., 1900.,
                               2000., 2200., 2400., 2600., 2800., 3000., 3500., 4000., 5000., 6000.};

 double TSnE_bins[40+1] = {                   500.,  750., 1000., 1250., 1500., 1750., 2000., 2250.,
                               2500., 2750., 3000., 3250., 3500., 3750., 4000., 4250., 4500., 4750.,
                               5000., 5250., 5500., 5750., 6000., 6250., 6500., 6750., 7000., 7250.,
                               7500., 7750., 8000., 8250., 8500., 8750., 9000., 9300., 9600., 9900.,
                               10300., 10900., 17000.};
 double TL1nE_bins[24+1] = {                  500.,  750., 1000., 1250., 1500., 1750., 2000., 2250.,
                               2500., 2750., 3000., 3250., 3500., 3750., 4000., 4250., 4500., 4750.,
                               5000., 5300., 5600., 6000., 6500., 7200., 15000.};
 double TL2nE_bins[21+1] = {                  500.,  750., 1000., 1250., 1500., 1750., 2000., 2250.,
                               2500., 2750., 3000., 3250., 3500., 3750., 4000., 4300., 4600., 4900., 
                               5200., 5700., 6500., 13000.};

int main(){
  TH1D* h_spec[3][2][3]; //tower; pid; type
  // -- spectrum --
  char hname[256];
  for(int it=0; it<3; ++it){
    for(int ipid=0; ipid<2; ++ipid){
      if(ipid == 0){
	for(int itp=0; itp<3; ++itp){
	  sprintf(hname,"h_spectrum_t%d_id%d_itp%d",it, ipid, itp);
	  if(it == 0)
	    h_spec[it][ipid][itp] = new TH1D(hname,hname, NDV[it], TSpE_bins);
	  else if (it == 1)
	    h_spec[it][ipid][itp] = new TH1D(hname,hname, NDV[it], TLpE_bins);
	  else
	    h_spec[it][ipid][itp] = new TH1D(hname,hname, NDV[it], TLpE_bins);
	}
      }else{
	for(int itp=0; itp<3; ++itp){
	  sprintf(hname,"h_spectrum_t%d_id%d_itp%d",it, ipid, itp);
	  if(it == 0)
	    h_spec[it][ipid][itp] = new TH1D(hname,hname, NDV[it+2], TSnE_bins);
	  else if (it == 1)
	    h_spec[it][ipid][itp] = new TH1D(hname,hname, NDV[it+2], TL1nE_bins);
	  else
	    h_spec[it][ipid][itp] = new TH1D(hname,hname, NDV[it+2], TL2nE_bins);
	}
      }
    }
  }

  TH2D *hitmap = new TH2D("hitmap","hitmap",80, -40, 40, 100, -20, 80);
  TH2D *hitmapBC = new TH2D("hitmapBC","hitmapBC",80, -40, 40, 100, -20, 80);

  TString histBaseDir = TString("PATH/YOU/WANT/TO/STORE/THE/PLOTS");
  LHCfData * ld = new LHCfData("./data/RUN42517_RUN42518.root");
  // Set offset for convert the positon from Detector coordinate to Beam Center coordinate
  // Detector coordinate: (0, 0) located at center of Small Tower
  // Beam Center coordinate: (0, 0) located at beam center (Fitting results according to LHCf)
  ld->SetPositionBeamCenter(bc[0], bc[1]);

  ATLASData * ad = new ATLASData("./data/user.ladamczy.10803057.EXT0._000046.DAOD_STDM9.test.pool.root");
  
  cout << "Number of LHCf file entries " << ld->getEntries() << endl;
  cout << "Number of ATLAS file entries " << ad->getEntries() << endl;

  for (UInt_t iEvt = 0;iEvt<ad->getEntries();iEvt++) {
    ad->getEntryTree(iEvt);
    ld->getEntryTree(iEvt);

    //cerr << ld->A1_EventQuality(0)  << "  " << ld->A1_EventQuality(1)  << endl;
    //cerr << ad->BcId() << "  " << ld->BcId() << endl;
    if( ld->A1_EventQuality(0) ){
      
      TLorentzVector trk;
      UInt_t numberOfGoodTracks = 0;

      for (UInt_t i=0; i< ad->NumInDetTrack(); i++) {
    	if( ad->InDetTrackNumberOfPixelHits(i)>0) {
    	  trk.SetXYZM(ad->InDetTrackPx(i), ad->InDetTrackPy(i), ad->InDetTrackPz(i), 0.13957);
    	  // hTrackEta->Fill(trk.Eta());
    	  // hTrackPt->Fill(trk.Pt());
    	  if( trk.Pt()>0.1 && TMath::Abs(trk.Eta())<2.5) numberOfGoodTracks++;
    	}
      }

      if(ld->A1_PID(0, 0) == LHCfData::ELEMAG && ld->A1_hitNumber(0, 0) == 1){
	if( ld->FiducialCut20mm(LHCfData::ELEMAG) && ld->A1_energy(0, 0) >gE_threshold){
	  hitmap->Fill(ld->A1_positionX(0, 0), ld->A1_positionY(0, 0));
	  hitmapBC->Fill(ld->GlobalPos_BC(0, 0, 0), ld->GlobalPos_BC(0, 1, 0) );
	  h_spec[0][0][0]->Fill( ld->A1_energy(0, 0) );
	  if(numberOfGoodTracks == 0){	    
	    h_spec[0][0][1]->Fill( ld->A1_energy(0, 0) );
	  }else{
	    h_spec[0][0][2]->Fill( ld->A1_energy(0, 0) );
	  }
	}
      }
      if(ld->A1_PID(1, 0) == LHCfData::ELEMAG && ld->A1_hitNumber(1, 0) == 1){
	if(ld->FiducialCut40mmB(LHCfData::ELEMAG) && ld->A1_energy(1, 0) >gE_threshold){
	  h_spec[1][0][0]->Fill( ld->A1_energy(1, 0) );
	  if(numberOfGoodTracks == 0){	    
	    h_spec[1][0][1]->Fill( ld->A1_energy(1, 0) );
	  }else{
	    h_spec[1][0][2]->Fill( ld->A1_energy(1, 0) );
	  } 
	}
      }
    }

    if(ld->A1_EventQuality(1)){
      TLorentzVector trk;
      UInt_t numberOfGoodTracks = 0;

      for (UInt_t i=0; i< ad->NumInDetTrack(); i++) {
	if( ad->InDetTrackNumberOfPixelHits(i)>0) {
	  trk.SetXYZM(ad->InDetTrackPx(i), ad->InDetTrackPy(i), ad->InDetTrackPz(i), 0.13957);
	  // hTrackEta->Fill(trk.Eta());
	  // hTrackPt->Fill(trk.Pt());
	  if( trk.Pt()>0.1 && TMath::Abs(trk.Eta())<2.5) numberOfGoodTracks++;
	}
      }

      if(ld->A1_PID(0, 1) == LHCfData::HADRON && ld->A1_hitNumber(0, 1) == 1){
	if( ld->FiducialCut20mm(LHCfData::HADRON, 0., 6.,225, 45) && ld->A1_energy(0, 1) >nE_threshold){
	  h_spec[0][1][0]->Fill( ld->A1_energy(0, 1) );
	  if(numberOfGoodTracks == 0){	    
	    h_spec[0][1][1]->Fill( ld->A1_energy(0, 1) );
	  }else{
	    h_spec[0][1][2]->Fill( ld->A1_energy(0, 1) );
	  }
	}
      }
      if(ld->A1_PID(1, 1) == LHCfData::HADRON && ld->A1_hitNumber(1, 1) == 1){
	if(ld->FiducialCut40mmA(LHCfData::HADRON) && ld->A1_energy(1, 1) >nE_threshold){
	  h_spec[1][1][0]->Fill( ld->A1_energy(1, 1) );
	  if(numberOfGoodTracks == 0){	    
	    h_spec[1][1][1]->Fill( ld->A1_energy(1, 1) );
	  }else{
	    h_spec[1][1][2]->Fill( ld->A1_energy(1, 1) );
	  } 
	}
      }
      if(ld->A1_PID(1, 1) == LHCfData::HADRON && ld->A1_hitNumber(1, 1) == 1){
	if(ld->FiducialCut40mmB(LHCfData::HADRON) && ld->A1_energy(1, 1) >nE_threshold){    
	  h_spec[2][1][0]->Fill( ld->A1_energy(1, 1) );
	  if(numberOfGoodTracks == 0){	    
	    h_spec[2][1][1]->Fill( ld->A1_energy(1, 1) );
	  }else{
	    h_spec[2][1][2]->Fill( ld->A1_energy(1, 1) );
	  } 
	}
      }
    }


    // It is better to extract information from one of LHCf detector at the beginning.
    // The comment out code below is for extracting the information of Arm2.



    // // ==== ARM2 ====
    // }    if( ld->A2_EventQuality(0) ){

    //   TLorentzVector trk;
    //   UInt_t numberOfGoodTracks = 0;

    //   for (UInt_t i=0; i< ad->NumInDetTrack(); i++) {
    // 	if( ad->InDetTrackNumberOfPixelHits(i)>0) {
    // 	  trk.SetXYZM(ad->InDetTrackPx(i), ad->InDetTrackPy(i), ad->InDetTrackPz(i), 0.13957);
    // 	  // hTrackEta->Fill(trk.Eta());
    // 	  // hTrackPt->Fill(trk.Pt());
    // 	  if( trk.Pt()>0.1 && TMath::Abs(trk.Eta())<2.5) numberOfGoodTracks++;
    // 	}
    //   }

    //   for(UInt_t itower=0; itower<2; itower++){

    // 	if(ld->A2_PID(itower, 0) == 0){

    // 	  bool Isphoton = true;
    // 	  int rapidity = ld->ApplyRapidityReduction(itower, -(ld->A2_positionX(itower, 0)), ld->A2_positionY(itower, 0), Isphoton);

    // 	  if(rapidity != -1){
    // 	    h_spec[rapidity][0][0]->Fill( ld->A2_energy(itower, 0) );
    // 	    if(numberOfGoodTracks == 0){	    
    // 	      h_spec[rapidity][0][1]->Fill( ld->A2_energy(itower, 0) );
    // 	    }else{
    // 	      h_spec[rapidity][0][2]->Fill( ld->A2_energy(itower, 0) );
    // 	    }
    // 	  }
    // 	}
    //   }
    // }
    // if(ld->A2_EventQuality(1)){
    //   TLorentzVector trk;
    //   UInt_t numberOfGoodTracks = 0;

    //   for (UInt_t i=0; i< ad->NumInDetTrack(); i++) {
    // 	if( ad->InDetTrackNumberOfPixelHits(i)>0) {
    // 	  trk.SetXYZM(ad->InDetTrackPx(i), ad->InDetTrackPy(i), ad->InDetTrackPz(i), 0.13957);
    // 	  // hTrackEta->Fill(trk.Eta());
    // 	  // hTrackPt->Fill(trk.Pt());
    // 	  if( trk.Pt()>0.1 && TMath::Abs(trk.Eta())<2.5) numberOfGoodTracks++;
    // 	}
    //   }

    //   for(UInt_t itower=0; itower<2; itower++){

    // 	//fout << left << setw(10) << ld->A2_PID(itower, ipid) << setw(16) << (double)ld->A2_positionX(itower, ipid) << setw(16) << (double)ld->A2_positionY(itower, ipid) << setw(16) << (double)ld->A2_energy(itower, ipid) << setw(10) << endl;

    // 	if(ld->A2_PID(itower, 1) == 1){
    // 	  //bool Isphoton = (ipid == 0) ? true : false;
    // 	  bool Isphoton =  false;
    // 	  int rapidity = ld->ApplyRapidityReduction(itower, -(ld->A2_positionX(itower, 1)), ld->A2_positionY(itower, 1), Isphoton);

    // 	  if(rapidity != -1){

    // 	    //cerr << rapidity << endl;
    // 	    h_spec[rapidity][1][0]->Fill( ld->A2_energy(itower, 1) );
    // 	    if(numberOfGoodTracks == 0){	    
    // 	      h_spec[rapidity][1][1]->Fill( ld->A2_energy(itower, 1) );
    // 	    }else{
    // 	      h_spec[rapidity][1][2]->Fill( ld->A2_energy(itower, 1) );
    // 	    }
    // 	  }
    // 	}
    //   }
    // }
  }

  
  TFile * output = new TFile("./data/Histos.root","RECREATE");
  for(int it=0; it<3; ++it){
    for(int ipid=0; ipid<2; ++ipid){
      for(int itp=0; itp<3; ++itp){
	h_spec[it][ipid][itp]->Write();
      }
    }
  }

  hitmap->Write();
  hitmapBC->Write();

  output->Write();
  output->Close();

}
