#include "ATLASData.hpp"

#include <TFile.h>
#include <TROOT.h>


ATLASData::ATLASData(TString fileName) : cEnt(0) {
	CollectionTree = new TChain("CollectionTree");
	CollectionTree->Add(fileName);
	CollectionTree->SetMakeClass(1);
	CollectionTree->SetBranchStatus("*",0);

        InDetTrackParticlesAux_phi = 0;
        InDetTrackParticlesAux_theta = 0;
        InDetTrackParticlesAux_qOverP = 0;
        InDetTrackParticlesAux_numberOfPixelHits = 0;
        InDetTrackParticlesAux_numberOfSCTHits = 0;
        InDetTrackParticlesAux_d0 = 0;
        InDetTrackParticlesAux_z0 = 0;
        InDetTrackParticlesAux_vz = 0;

        PrimaryVerticesAux_x = 0;
        PrimaryVerticesAux_y = 0;
        PrimaryVerticesAux_z = 0;
        PrimaryVerticesAux_vertexType = 0;

	//Activate interesting branches
	CollectionTree->SetBranchStatus("EventInfoAux.", 1);
	CollectionTree->SetBranchStatus("EventInfoAux.eventNumber", 1);
        CollectionTree->SetBranchStatus("EventInfoAux.runNumber", 1);
        CollectionTree->SetBranchStatus("EventInfoAux.lumiBlock", 1);
        CollectionTree->SetBranchStatus("EventInfoAux.timeStamp", 1);
        CollectionTree->SetBranchStatus("EventInfoAux.timeStampNSOffset", 1);
        CollectionTree->SetBranchStatus("EventInfoAux.bcid", 1);
        CollectionTree->SetBranchStatus("EventInfoAux.extendedLevel1ID", 1);

        CollectionTree->SetBranchStatus("MBTSModulesAux.", 1);
        CollectionTree->SetBranchStatus("MBTSModulesAux.e", 1);
        CollectionTree->SetBranchStatus("MBTSModulesAux.type", 1);
        CollectionTree->SetBranchStatus("MBTSModulesAux.channel", 1);


        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.phi", 1);
        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.theta", 1);
        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.qOverP", 1);
        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.numberOfPixelHits", 1);
        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.numberOfSCTHits", 1);
        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.d0", 1);
        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.z0", 1);
        CollectionTree->SetBranchStatus("InDetTrackParticlesAuxDyn.vz", 1);

        CollectionTree->SetBranchStatus("PrimaryVerticesAuxDyn.x", 1);
        CollectionTree->SetBranchStatus("PrimaryVerticesAuxDyn.y", 1);
        CollectionTree->SetBranchStatus("PrimaryVerticesAuxDyn.z", 1);
        CollectionTree->SetBranchStatus("PrimaryVerticesAuxDyn.vertexType", 1);




	//Load branches
	CollectionTree->SetBranchAddress("EventInfoAux.lumiBlock", &EventInfoAux_lumiBlock);
	CollectionTree->SetBranchAddress("EventInfoAux.eventNumber", &EventInfoAux_eventNumber, &b_EventInfoAux_eventNumber);
        CollectionTree->SetBranchAddress("EventInfoAux.runNumber", &EventInfoAux_runNumber, &b_EventInfoAux_runNumber);
        CollectionTree->SetBranchAddress("EventInfoAux.timeStamp", &EventInfoAux_timeStamp, &b_EventInfoAux_timeStamp);
        CollectionTree->SetBranchAddress("EventInfoAux.timeStampNSOffset", &EventInfoAux_timeStampNSOffset, &b_EventInfoAux_timeStampNSOffset);
        CollectionTree->SetBranchAddress("EventInfoAux.bcid", &EventInfoAux_bcid, &b_EventInfoAux_bcid);
        CollectionTree->SetBranchAddress("EventInfoAux.extendedLevel1ID", &EventInfoAux_extendedLevel1ID, &b_EventInfoAux_extendedLevel1ID);

        CollectionTree->SetBranchAddress("MBTSModulesAux.e", &MBTSModulesAux_e);
        CollectionTree->SetBranchAddress("MBTSModulesAux.type", &MBTSModulesAux_type);
        CollectionTree->SetBranchAddress("MBTSModulesAux.channel", &MBTSModulesAux_channel);


        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.phi", &InDetTrackParticlesAux_phi);
        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.theta", &InDetTrackParticlesAux_theta);
        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.qOverP", &InDetTrackParticlesAux_qOverP);
        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.numberOfPixelHits", &InDetTrackParticlesAux_numberOfPixelHits);
        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.numberOfSCTHits", &InDetTrackParticlesAux_numberOfSCTHits);

        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.d0", &InDetTrackParticlesAux_d0);
        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.z0", &InDetTrackParticlesAux_z0);
        CollectionTree->SetBranchAddress("InDetTrackParticlesAuxDyn.vz", &InDetTrackParticlesAux_vz);

        CollectionTree->SetBranchAddress("PrimaryVerticesAuxDyn.x", &PrimaryVerticesAux_x);
        CollectionTree->SetBranchAddress("PrimaryVerticesAuxDyn.y", &PrimaryVerticesAux_y);
        CollectionTree->SetBranchAddress("PrimaryVerticesAuxDyn.z", &PrimaryVerticesAux_z);
        CollectionTree->SetBranchAddress("PrimaryVerticesAuxDyn.vertexType", &PrimaryVerticesAux_vertexType);

	nEnt = CollectionTree->GetEntries();
}


		

