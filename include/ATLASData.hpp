#ifndef ATLASData_hpp
#define ATLASData_hpp

#include <TChain.h>
#include <TString.h>
#include <TMath.h>


#include <Rtypes.h>

#include <vector>
#include <algorithm>
#include <iostream>

class ATLASData{
public:

        // Number of entries

	Long64_t nEnt;
	Long64_t cEnt;
		

		TBranch        *b_EventInfoAux_runNumber;   //!
		TBranch        *b_EventInfoAux_eventNumber;   //!
		TBranch        *b_EventInfoAux_lumiBlock;   //!
                TBranch        *b_EventInfoAux_timeStamp;   //!
                TBranch        *b_EventInfoAux_timeStampNSOffset;   //!
                TBranch        *b_EventInfoAux_bcid;   //!
                TBranch        *b_EventInfoAux_extendedLevel1ID;   //!

                TBranch        *b_MBTSModulesAux_e;   //!
                TBranch        *b_MBTSModulesAux_type;   //!
                TBranch        *b_MBTSModulesAux_cgannel;   //!


                TBranch        *b_InDetTrackParticlesAux_phi;   //!
                TBranch        *b_InDetTrackParticlesAux_theta;   //!
                TBranch        *b_InDetTrackParticlesAux_qOverP;   //!
                TBranch        *b_InDetTrackParticlesAux_numberOfPixelHits;   //!
                TBranch        *b_InDetTrackParticlesAux_numberOfSCTHits;   //!
                TBranch        *b_InDetTrackParticlesAux_d0;   //!
                TBranch        *b_InDetTrackParticlesAux_z0;   //!
                TBranch        *b_InDetTrackParticlesAux_vz;   //!

                TBranch        *b_PrimaryVerticesAux_x;   //!
                TBranch        *b_PrimaryVerticesAux_y;   //!
                TBranch        *b_PrimaryVerticesAux_z;   //!
                TBranch        *b_PrimaryVerticesAux_vertexType;   //!



		UInt_t EventInfoAux_lumiBlock;
		ULong64_t EventInfoAux_eventNumber;
                UInt_t EventInfoAux_runNumber;
                UInt_t EventInfoAux_timeStamp;
	        UInt_t EventInfoAux_timeStampNSOffset;
	        UInt_t EventInfoAux_bcid;
                UInt_t EventInfoAux_extendedLevel1ID;

                std::vector< float >   *InDetTrackParticlesAux_phi;
                std::vector< float >   *InDetTrackParticlesAux_theta;
                std::vector< float >   *InDetTrackParticlesAux_qOverP;
                std::vector< uint8_t >   *InDetTrackParticlesAux_numberOfPixelHits;
                std::vector< uint8_t >   *InDetTrackParticlesAux_numberOfSCTHits;
                std::vector< float >   *InDetTrackParticlesAux_d0;
                std::vector< float >   *InDetTrackParticlesAux_z0;
                std::vector< float >   *InDetTrackParticlesAux_vz;

                std::vector< float >   *PrimaryVerticesAux_x;
                std::vector< float >   *PrimaryVerticesAux_y;
                std::vector< float >   *PrimaryVerticesAux_z;
                std::vector< uint8_t >   *PrimaryVerticesAux_vertexType;


                std::vector< float >   MBTSModulesAux_e;
                std::vector< Int_t >   MBTSModulesAux_type;
                std::vector< Int_t >   MBTSModulesAux_channel;


		//Chains

        TChain * CollectionTree;
				
	public:
		
		ATLASData(TString fileName);
		
		virtual ~ATLASData() {
			delete CollectionTree;
		}

		virtual void AddTreePath(const TString & nextPath) {
			CollectionTree->Add(nextPath);
			nEnt = CollectionTree->GetEntries();
		}

		virtual Long64_t getEntries() const {
			return nEnt;
		}

		virtual void getEntryTree(UInt_t iEvt) {
			CollectionTree->GetEntry(iEvt);
		}
		
		virtual Bool_t nextEntry() {
			Long64_t nnEnt = CollectionTree->GetEntries();
			if( cEnt < nnEnt ) {
				getEntryTree(cEnt);
				cEnt++;
				return kTRUE;
			} else {
				return kFALSE;
			}
		}
	
	
		private: ATLASData() {};  
                public:
	
		virtual UInt_t Lum_block() const {
			return EventInfoAux_lumiBlock;
		}

		virtual Long64_t EventNumber() const {
			return EventInfoAux_eventNumber;
		}

                virtual UInt_t RunNumber() const {
                        return EventInfoAux_runNumber;
                }

                virtual UInt_t ExtendedLevel1ID() const {
                        return EventInfoAux_extendedLevel1ID;
                }

                virtual UInt_t BcId() const {
                        return EventInfoAux_bcid;
                }

                virtual UInt_t TimeStamp() const {
                        return EventInfoAux_timeStamp;
                }

                virtual UInt_t  TimeStampNSOffset() const {
                        return EventInfoAux_timeStampNSOffset;
                }

                virtual UInt_t NumInDetTrack() const {
			return InDetTrackParticlesAux_phi->size();
		}

                // virtual Double_t InDetTrackPx(const UInt_t track) const {
                //         return 0.001/TMath::Abs(InDetTrackParticlesAux_qOverP->at( track)) * TMath::Sin(InDetTrackParticlesAux_theta->at( track)) * TMath::Cos(InDetTrackParticlesAux_phi->at( track));
                // }

                // virtual Double_t InDetTrackPy(const UInt_t track) const {
                //         return 0.001/TMath::Abs(InDetTrackParticlesAux_qOverP->at( track)) * TMath::Cos(InDetTrackParticlesAux_theta->at( track)) * TMath::Cos(InDetTrackParticlesAux_phi->at( track));
                // }

                // virtual Double_t InDetTrackPz(const UInt_t track) const {
                //         return 0.001/TMath::Abs(InDetTrackParticlesAux_qOverP->at( track)) * TMath::Sin(InDetTrackParticlesAux_phi->at( track));
                // }

  
    virtual Double_t InDetTrackPx(const UInt_t track) const {
        return 0.001 / TMath::Abs(InDetTrackParticlesAux_qOverP->at(track)) *
               TMath::Sin(InDetTrackParticlesAux_phi->at(track)) * TMath::Sin(InDetTrackParticlesAux_theta->at(track));
    }
    
    virtual Double_t InDetTrackPy(const UInt_t track) const {
        return 0.001 / TMath::Abs(InDetTrackParticlesAux_qOverP->at(track)) *
               TMath::Cos(InDetTrackParticlesAux_phi->at(track)) * TMath::Sin(InDetTrackParticlesAux_theta->at(track));
    }
    
    virtual Double_t InDetTrackPz(const UInt_t track) const {
        return 0.001 / TMath::Abs(InDetTrackParticlesAux_qOverP->at(track)) *
               TMath::Cos(InDetTrackParticlesAux_theta->at(track));
    }

                virtual uint8_t InDetTrackNumberOfPixelHits(const UInt_t track) const {
                        return InDetTrackParticlesAux_numberOfPixelHits->at( track);   
                }

                virtual uint8_t InDetTrackNumberOfSCTHits(const UInt_t track) const {
                        return InDetTrackParticlesAux_numberOfSCTHits->at( track);
                }

                virtual Double_t InDetTrackD0(const UInt_t track) const {
                        return InDetTrackParticlesAux_d0->at( track);
                }
                virtual Double_t InDetTrackZ0(const UInt_t track) const {
                        return InDetTrackParticlesAux_z0->at( track);
       	       	}
                virtual Double_t InDetTrackVz(const UInt_t track) const {
                        return InDetTrackParticlesAux_vz->at( track);
       	       	}

                virtual UInt_t NumVertices() const {
                        return PrimaryVerticesAux_x->size();
                }

                virtual Double_t VerticesX(const UInt_t vrt) const {
                        return PrimaryVerticesAux_x->at( vrt);
                }

                virtual Double_t VerticesY(const UInt_t vrt) const {
                        return PrimaryVerticesAux_y->at( vrt);
                }
                virtual Double_t VerticesZ(const UInt_t vrt) const {
                        return PrimaryVerticesAux_z->at( vrt);
                }
                virtual Double_t VerticesType(const UInt_t vrt) const {
                        return PrimaryVerticesAux_vertexType->at( vrt);
                }
                virtual Double_t MBTSEnergy(const UInt_t modul) const {
                        return MBTSModulesAux_e.at( modul);
                }
                virtual Int_t MBTSType(const UInt_t modul) const {
                        return MBTSModulesAux_type.at( modul);
                }
                virtual Int_t MBTSChannel(const UInt_t modul) const {
                        return MBTSModulesAux_channel.at( modul);
                }



};


#endif

