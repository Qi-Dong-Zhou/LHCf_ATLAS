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

                TBranch        *b_InDetTrackParticlesAux_phi;   //!
                TBranch        *b_InDetTrackParticlesAux_theta;   //!
                TBranch        *b_InDetTrackParticlesAux_qOverP;   //!
                TBranch        *b_InDetTrackParticlesAux_numberOfPixelHits;   //!



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

                virtual Double_t InDetTrackPx(const UInt_t track) const {
                        return 0.001/TMath::Abs(InDetTrackParticlesAux_qOverP->at( track)) * TMath::Sin(InDetTrackParticlesAux_theta->at( track)) * TMath::Cos(InDetTrackParticlesAux_phi->at( track));
                }

                virtual Double_t InDetTrackPy(const UInt_t track) const {
                        return 0.001/TMath::Abs(InDetTrackParticlesAux_qOverP->at( track)) * TMath::Cos(InDetTrackParticlesAux_theta->at( track)) * TMath::Cos(InDetTrackParticlesAux_phi->at( track));
                }

                virtual Double_t InDetTrackPz(const UInt_t track) const {
                        return 0.001/TMath::Abs(InDetTrackParticlesAux_qOverP->at( track)) * TMath::Sin(InDetTrackParticlesAux_phi->at( track));
                }

                virtual uint8_t InDetTrackNumberOfPixelHits(const UInt_t track) const {
                        return InDetTrackParticlesAux_numberOfPixelHits->at( track);   
                }


};


#endif

