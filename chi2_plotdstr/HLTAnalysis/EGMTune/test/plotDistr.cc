// -*- C++ -*-
//
// Package:    MyCandidates
// Class:      MyCandidates
//
// Original Author:  Matteo Sani,40 3-A02,+41227671577,
//         Created:  Thu Feb 14 14:06:52 CET 2013
// $Id$
//
//

// 23 07 2015: Inclusion of RECO quantities (on RECO/AOD linked with RAW)
// 07 08 2015: dEtaSeed and other parameters
// 15 12 2015: Updated to use getByToken for 80X
// 14 01 2016: Event metadata is stored too
// 03 02 2016: Reverting back to hltPixelVertices for nVtx
// 07 03 2016: MC-only PU information and updated genParticle info
// 25 03 2016: Gen event weight
// 28 06 2016: Remove the abs of GSF vars
// 28 06 2016: Defaults moved to 999999. to be compatible with var producers
// 21 07 2016: Append "mc_" to MC-only nBX, nPU info, updated event metadata
// 13 04 2017: Added HLT Ecal recHits chi2 histograms (to be tested)

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/ElectronIsolationAssociation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaHcalIsolation.h"
#include "RecoEgamma/EgammaHLTAlgos/interface/EgammaHLTHcalIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaRecHitIsolation.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"

#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateIsolation.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

#include <string>
#include <iostream>

class plotDistr : public edm::EDAnalyzer {
public:
  explicit plotDistr(const edm::ParameterSet&);
  ~plotDistr();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  int findEleRef(reco::RecoEcalCandidateRef ref, edm::Handle<reco::ElectronCollection> hltEleH);
  void mcTruth(edm::Handle<reco::GenParticleCollection> genParticleH);

private:
  virtual void beginRun(const edm::Run& run,const edm::EventSetup& setup);
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  edm::InputTag trigResultsTag_;
  HLTConfigProvider hltConfig_; 
  std::vector<std::string> pathNames_;

  edm::EDGetTokenT<EcalRecHitCollection> hltEBHitTkn;
  edm::EDGetTokenT<EcalRecHitCollection> hltEEHitTkn;

  edm::EDGetTokenT<edm::TriggerResults> trgResTkn;

  edm::EDGetTokenT<reco::GenParticleCollection> genTkn;
  edm::EDGetTokenT<GenEventInfoProduct> genEvtTkn;
  edm::EDGetTokenT<reco::VertexCollection> vtxTkn;
  edm::EDGetTokenT< std::vector<PileupSummaryInfo> > puTkn;

  edm::EDGetTokenT<std::vector<reco::GsfElectron> > elTkn;
  edm::EDGetTokenT<std::vector<reco::Electron> > eTkn;
  edm::EDGetTokenT<std::vector<reco::RecoEcalCandidate> > cTkn;

  edm::EDGetTokenT<edm::ValueMap<float> > recoEcalTkn;
  edm::EDGetTokenT<edm::ValueMap<float> > recoHcalTkn;

  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> sieieTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> ecalTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> hcalTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> hoeTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> tkisoTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> detaTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> detaseedTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> dphiTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> eopTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> mishitsTkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> chi2Tkn;
  edm::EDGetTokenT<reco::RecoEcalCandidateIsolationMap> hitsTkn;

  edm::EDGetTokenT<double> rhoTkn;

  std::string outputFileName;
  TFile* f;
  TTree* t;
  
  Float_t rho;
  Int_t nRun, nEvt, nLumi, nBX, nOrb, nSto, accPath;
  Int_t npf, nVtx, gp_n, reco_n;
  Float_t epf[10];
  Float_t eRawpf[10];
  Float_t eoppf[10];  
  Float_t etpf[10];
  Float_t etRawpf[10];  
  Float_t sepf[10];  
  Float_t etapf[10];
  Float_t setapf[10];  
  Float_t phipf[10];   
  Float_t sphipf[10];  
  Float_t ecalpf[10];
  Float_t sieiepf[10];
  Float_t detapf[10];
  Float_t detaseedpf[10];
  Float_t dphipf[10];
  //Float_t tkptpf[10];
  //Float_t tketapf[10];
  //Float_t tkphipf[10];
  Float_t hoepf[10];
  Float_t hcalpf[10];
  Float_t tkisopf[10];
  Float_t chi2pf[10];
  Int_t mishitspf[10];
  Int_t hitspf[10];

  Float_t reco_e[10];
  Float_t reco_et[10];
  Float_t reco_eRaw[10];
  Float_t reco_etRaw[10];
  Float_t reco_pt[10];
  Float_t reco_eta[10];
  Float_t reco_phi[10];
  Float_t reco_sieie[10];
  Float_t reco_hoe[10];
  Float_t reco_ecal[10];
  Float_t reco_hcal[10];
  Float_t reco_eop[10];
  Int_t reco_mishits[10];
  Float_t reco_deta[10];
  Float_t reco_detaseed[10];
  Float_t reco_dphi[10];
  Float_t reco_chi2[10];
  Float_t reco_tkiso[10];

  Float_t gevt_wgt;
  Float_t gp_pt[10];
  Float_t gp_eta[10];
  Float_t gp_phi[10];

  Int_t mc_nBX;
  Int_t mc_BX[100];
  Int_t mc_nPUtrue;
  Int_t mc_nPUobs[100];

  bool isData;
  bool saveReco;

  bool debug;
  std::string evtAcc;

  TH1F *hitChi2EB, *hitChi2EE;
};

void plotDistr::beginRun(const edm::Run& run,const edm::EventSetup& setup) {
  std::cout <<"begining run "<<std::endl;
  bool changed = false;
  hltConfig_.init(run,setup,trigResultsTag_.process(),changed); //as we need the orginal HLT config...
  std::cout <<"table name "<<hltConfig_.tableName()<<std::endl;
}


plotDistr::plotDistr(const edm::ParameterSet& iParSet) {
  outputFileName  = iParSet.getParameter<std::string>("OutputFileName");
  isData          = iParSet.getParameter<bool>("isData");
  saveReco        = iParSet.getParameter<bool>("saveReco");
  pathNames_      = iParSet.getParameter<std::vector<std::string> >("trgSelection");
  trigResultsTag_ = iParSet.getParameter<edm::InputTag>("trgResults");
// trgResTkn     = consumes<edm::TriggerResults>(edm::InputTag("trgResults"));//
  hltEBHitTkn   = consumes<EcalRecHitCollection>(edm::InputTag("hltEcalRecHit:EcalRecHitsEB"));
  hltEEHitTkn   = consumes<EcalRecHitCollection>(edm::InputTag("hltEcalRecHit:EcalRecHitsEE"));
  trgResTkn     = consumes<edm::TriggerResults>(trigResultsTag_);
  genTkn        = consumes<reco::GenParticleCollection>(edm::InputTag("genParticles"));
  genEvtTkn     = consumes<GenEventInfoProduct>(edm::InputTag("generator"));
  vtxTkn        = consumes<reco::VertexCollection>(edm::InputTag("hltPixelVertices"));
  puTkn         = consumes<std::vector<PileupSummaryInfo> >(edm::InputTag("addPileupInfo"));

  elTkn         = consumes<std::vector<reco::GsfElectron> >(edm::InputTag("gedGsfElectrons"));
  //eTkn        = consumes<std::vector<reco::Electron> >(edm::InputTag("hltEgammaCandidates"));
  cTkn          = consumes<std::vector<reco::RecoEcalCandidate> >(edm::InputTag("hltEgammaCandidates"));

  recoEcalTkn   = consumes<edm::ValueMap<float> >(edm::InputTag("electronEcalPFClusterIsolationProducer"));
  recoHcalTkn   = consumes<edm::ValueMap<float> >(edm::InputTag("electronHcalPFClusterIsolationProducer"));

  sieieTkn      = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaClusterShape:sigmaIEtaIEta5x5"));
  ecalTkn       = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaEcalPFClusterIso"));
  hcalTkn       = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaHcalPFClusterIso"));
  hoeTkn        = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaHoverERhoCorr"));
  tkisoTkn      = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaEleGsfTrackIso"));
  detaTkn       = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaGsfTrackVars:Deta"));
  detaseedTkn   = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaGsfTrackVars:DetaSeed"));
  dphiTkn       = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaGsfTrackVars:Dphi"));
  eopTkn        = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaGsfTrackVars:OneOESuperMinusOneOP"));
  chi2Tkn       = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaGsfTrackVars:Chi2"));
  mishitsTkn    = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaGsfTrackVars:MissingHits"));
  hitsTkn       = consumes<reco::RecoEcalCandidateIsolationMap>(edm::InputTag("hltEgammaGsfTrackVars:ValidHits"));

  rhoTkn        = consumes<double>(edm::InputTag("hltFixedGridRhoFastjetAllCaloForMuons"));

  hitChi2EB = new TH1F("hitChi2EB", "", 1000, 0., 1000.);
  hitChi2EE = new TH1F("hitChi2EE", "", 1000, 0., 1000.);
}

plotDistr::~plotDistr() 
{}

int plotDistr::findEleRef(reco::RecoEcalCandidateRef ref, edm::Handle<reco::ElectronCollection> hltEleH) {

  int index = -1;
  for (unsigned int i=0; i<hltEleH->size(); i++) {
    reco::ElectronRef cand(hltEleH, i);
    if (cand->superCluster() == ref->superCluster()) 
      return i;
  }

  return index;
}

void plotDistr::mcTruth(edm::Handle<reco::GenParticleCollection> gpH) {
  
  gp_n = 0;
  
  for(size_t i = 0; i < gpH->size(); ++i) {
    
    const reco::GenParticleRef gp(gpH, i);
    
    if ((gp->status() == 23 or gp->status() == 24) and abs(gp->pdgId()) == 11) {
      if (gp->pt() > 0.) {
	gp_pt[gp_n]  = gp->pt();
	gp_eta[gp_n] = gp->eta();
	gp_phi[gp_n] = gp->phi();
	gp_n++;
      }
    }
  }
}

void plotDistr::analyze(const edm::Event& iEvt, const edm::EventSetup& iSetup) {

  //isData = iEvt.eventAuxiliary().isRealData(); // causes segfault ?_?
  debug = false;

  nRun = iEvt.eventAuxiliary().run();
  nEvt = iEvt.eventAuxiliary().event();
  nLumi = iEvt.eventAuxiliary().luminosityBlock();
  nBX = iEvt.eventAuxiliary().bunchCrossing();
  nOrb = iEvt.eventAuxiliary().orbitNumber();
  nSto = iEvt.eventAuxiliary().storeNumber();

  if (debug)
    std::cout << "Processing event " << nEvt << " in run / lumi " << nRun << " / " << nLumi << std::endl; 

  edm::Handle<edm::TriggerResults> trgResH;
  iEvt.getByToken(trgResTkn, trgResH);

  const edm::TriggerResults& trgResults = *trgResH;
  const edm::TriggerNames& trgNames = iEvt.triggerNames(trgResults);

  accPath = 0; 
  for(size_t pathNr=0;pathNr<pathNames_.size();pathNr++){
    size_t pathIndex = trgNames.triggerIndex(pathNames_[pathNr]);
    if(pathIndex<trgResults.size() &&  trgResults.accept(pathIndex)) 
      accPath = 1;
  }

  edm::Handle<reco::GenParticleCollection> gpH;
  edm::Handle<GenEventInfoProduct> gEvtH;
  edm::Handle<std::vector<PileupSummaryInfo> > puH;

  mc_nBX = 0;
  gevt_wgt = 0.;
  if (!isData) {
    iEvt.getByToken(genTkn, gpH);
    iEvt.getByToken(genEvtTkn, gEvtH);
    mcTruth(gpH);
    gevt_wgt = gEvtH->weight();

    iEvt.getByToken(puTkn, puH);
    mc_nPUtrue = puH->begin()->getTrueNumInteractions();
    std::vector<PileupSummaryInfo>::const_iterator pu;

    for(pu = puH->begin(); pu != puH->end(); ++pu) {
      mc_BX[mc_nBX]      = pu->getBunchCrossing();
      mc_nPUobs[mc_nBX]  = pu->getPU_NumInteractions();
      mc_nBX++;
    }
  }

  if (saveReco) {

    edm::Handle<std::vector<reco::GsfElectron> > elH;
    iEvt.getByToken(elTkn, elH);

    edm::Handle<edm::ValueMap<float> > recoEcalMap;
    iEvt.getByToken(recoEcalTkn, recoEcalMap);

    edm::Handle<edm::ValueMap<float> > recoHcalMap;
    iEvt.getByToken(recoHcalTkn, recoHcalMap);

    reco_n = 0;
    for (unsigned int i=0; i<elH->size(); i++) {
      if (reco_n == 9)
	continue;
      reco_e[reco_n] = 999999.;
      reco_et[reco_n] = 999999.;
      reco_eta[reco_n] = 999999.;
      reco_phi[reco_n] = 999999.;
      reco_sieie[reco_n] = 999999.;
      reco_hoe[reco_n] = 999999.;
      reco_ecal[reco_n] = 999999.;
      reco_hcal[reco_n] = 999999.;
      reco_eop[reco_n] = 999999.;
      reco_mishits[reco_n] = 999999;
      reco_deta[reco_n] = 999999.;
      reco_detaseed[reco_n] = 999999.;
      reco_dphi[reco_n] = 999999.;
      reco_chi2[reco_n] = 999999.;
      reco_tkiso[reco_n] = 999999.;
      
      reco::GsfElectronRef el(elH, i);
      reco::GsfElectron::PflowIsolationVariables pfIso = el->pfIsolationVariables();

      reco_e[reco_n] = el->superCluster()->energy();
      reco_et[reco_n] = el->superCluster()->energy()*sin(el->superCluster()->position().theta());
      reco_eRaw[reco_n] = el->superCluster()->rawEnergy();
      reco_etRaw[reco_n] = el->superCluster()->rawEnergy()*sin(el->superCluster()->position().theta());

      reco_pt[reco_n] = el->pt();
      reco_eta[reco_n] = el->superCluster()->eta();
      reco_phi[reco_n] = el->superCluster()->phi();

      reco_sieie[reco_n] = el->full5x5_sigmaIetaIeta();
      reco_hoe[reco_n] = el->hadronicOverEm();
      reco_ecal[reco_n] = (*recoEcalMap)[el];
      reco_hcal[reco_n] = (*recoHcalMap)[el];
      reco_eop[reco_n] = ( 1. / el->superCluster()->energy() ) - ( 1. / el->gsfTrack()->p() );
      reco_mishits[reco_n] = el->gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
      reco_deta[reco_n] = el->deltaEtaSuperClusterTrackAtVtx();
      reco_detaseed[reco_n] = el->deltaEtaSeedClusterTrackAtVtx();
      reco_dphi[reco_n] = el->deltaPhiSuperClusterTrackAtVtx();
      reco_chi2[reco_n] = el->gsfTrack()->normalizedChi2();
      reco_tkiso[reco_n] = pfIso.sumChargedHadronPt;

      //std::cout << "plotReco: " << reco_et[reco_n] << " " << reco_eta[reco_n] << " " << reco_phi[reco_n] << std::endl;

      reco_n++;

    }
  }

  edm::Handle<EcalRecHitCollection> hltEBHitH, hltEEHitH;
  iEvt.getByToken(hltEBHitTkn, hltEBHitH);
  if (!hltEBHitH.failedToGet()) {
    for(size_t i = 0; i < hltEBHitH->size(); ++i) {
      const EcalRecHitRef recHit(hltEBHitH, i);
       hitChi2EB->Fill(recHit->chi2());
    }
  }

  iEvt.getByToken(hltEEHitTkn, hltEEHitH);
  if (!hltEEHitH.failedToGet()) {
    for(size_t i = 0; i < hltEEHitH->size(); ++i) {
      const EcalRecHitRef recHit(hltEEHitH, i);
       hitChi2EE->Fill(recHit->chi2());
    }
  }

  edm::Handle<std::vector<reco::Electron> > eH;
  edm::Handle<std::vector<reco::RecoEcalCandidate> > cH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> sieieMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> ecalMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> hcalMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> hoeMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> tkisoMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> detaMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> detaseedMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> dphiMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> eopMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> mishitsMapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> chi2MapH;
  edm::Handle<reco::RecoEcalCandidateIsolationMap> hitsMapH;

  edm::Handle<reco::VertexCollection> vtxH;
  iEvt.getByToken(vtxTkn, vtxH);

  if (!vtxH.failedToGet())
    nVtx = vtxH->size();
  else
    nVtx = 0;

  edm::Handle<double> rhoH;
  iEvt.getByToken(rhoTkn, rhoH);

  if (!rhoH.failedToGet())
    rho = *(rhoH.product());
  else
    rho = 999999.;

  npf = 0;
  iEvt.getByToken(cTkn, cH);
  if (!cH.failedToGet()) {
    
    const reco::RecoEcalCandidateIsolationMap* sieieMapPF = 0;
    iEvt.getByToken(sieieTkn, sieieMapH);
    if (!sieieMapH.failedToGet())  
      sieieMapPF = sieieMapH.product();
      
    const reco::RecoEcalCandidateIsolationMap* ecalMapPF = 0;
    iEvt.getByToken(ecalTkn, ecalMapH);
    if (!ecalMapH.failedToGet()) 
      ecalMapPF = ecalMapH.product(); 
      
    const reco::RecoEcalCandidateIsolationMap* hcalMapPF = 0;
    iEvt.getByToken(hcalTkn, hcalMapH);
    if (!hcalMapH.failedToGet()) 
      hcalMapPF = hcalMapH.product(); 
      
    const reco::RecoEcalCandidateIsolationMap* hoeMapPF = 0;
    iEvt.getByToken(hoeTkn, hoeMapH);
    if (!hoeMapH.failedToGet()) 
      hoeMapPF = hoeMapH.product(); 
      
    const reco::RecoEcalCandidateIsolationMap* tkisoMapPF = 0;
    iEvt.getByToken(tkisoTkn, tkisoMapH);
    if (!tkisoMapH.failedToGet())
      tkisoMapPF = tkisoMapH.product();          
      
    const reco::RecoEcalCandidateIsolationMap* detaMapPF = 0;
    iEvt.getByToken(detaTkn, detaMapH);
    if (!detaMapH.failedToGet())
      detaMapPF = detaMapH.product();

    const reco::RecoEcalCandidateIsolationMap* detaseedMapPF = 0;
    iEvt.getByToken(detaseedTkn, detaseedMapH);
    if (!detaseedMapH.failedToGet())
      detaseedMapPF = detaseedMapH.product();
      
    const reco::RecoEcalCandidateIsolationMap* dphiMapPF = 0;
    iEvt.getByToken(dphiTkn, dphiMapH);
    if (!dphiMapH.failedToGet()) 
      dphiMapPF = dphiMapH.product();
      
    const reco::RecoEcalCandidateIsolationMap* eopMapPF = 0;
    iEvt.getByToken(eopTkn, eopMapH);
    if (!eopMapH.failedToGet()) 
      eopMapPF = eopMapH.product();

    const reco::RecoEcalCandidateIsolationMap* chi2MapPF = 0;
    iEvt.getByToken(chi2Tkn, chi2MapH);
    if (!chi2MapH.failedToGet()) 
      chi2MapPF = chi2MapH.product();

    const reco::RecoEcalCandidateIsolationMap* mishitsMapPF = 0;
    iEvt.getByToken(mishitsTkn, mishitsMapH);
    if (!mishitsMapH.failedToGet()) 
      mishitsMapPF = mishitsMapH.product();

    const reco::RecoEcalCandidateIsolationMap* hitsMapPF = 0;
    iEvt.getByToken(hitsTkn, hitsMapH);
    if (!hitsMapH.failedToGet()) 
      hitsMapPF = hitsMapH.product();
      
    for (unsigned int i=0; i<cH->size(); i++) {
      if (npf >= 9) continue;

      epf[npf] = 999999.;
      eRawpf[npf] = 999999.;
      eoppf[npf] = 999999.;
      etpf[npf] = 999999.;
      etRawpf[npf] = 999999.;
      etapf[npf] = 999999.;
      phipf[npf] = 999999.;
      sieiepf[npf] = 999999.;
      ecalpf[npf] = 999999;
      detapf[npf] = 999999.;
      detaseedpf[npf] = 999999.;
      dphipf[npf] = 999999.;
      //tkptpf[npf] = 999999.;
      //tketapf[npf] = 999999.;
      //tkphipf[npf] = 999999.;
      hcalpf[npf] = 999999.;
      hoepf[npf] = 999999.;
      tkisopf[npf] = 999999.;
      mishitspf[npf] = 999999.;
      chi2pf[npf] = 999999.;
      hitspf[npf] = 999999.;

      reco::RecoEcalCandidateRef c(cH, i);

      epf[npf] = c->superCluster()->energy();
      eRawpf[npf] = c->superCluster()->rawEnergy();
      etpf[npf] = c->superCluster()->energy()*sin(c->superCluster()->position().theta());
      etRawpf[npf] = c->superCluster()->rawEnergy()*sin(c->superCluster()->position().theta());
      etapf[npf] = c->eta();
      phipf[npf] = c->phi();

      if (sieieMapPF != 0) sieiepf[npf] = (*sieieMapPF)[c];
      if (ecalMapPF != 0) ecalpf[npf] = (*ecalMapPF)[c];
      if (hoeMapPF != 0) hoepf[npf] = (*hoeMapPF)[c];
      if (hcalMapPF != 0) hcalpf[npf] = (*hcalMapPF)[c];
      if (detaMapPF != 0) detapf[npf] = (*detaMapPF)[c];
      if (detaseedMapPF != 0) detaseedpf[npf] = (*detaseedMapPF)[c];
      if (dphiMapPF != 0) dphipf[npf] = (*dphiMapPF)[c];
      if (eopMapPF != 0) eoppf[npf] = (*eopMapPF)[c];
      if (mishitsMapPF != 0) mishitspf[npf] = (*mishitsMapPF)[c];
      if (hitsMapPF != 0) hitspf[npf] = (*hitsMapPF)[c];
      if (chi2MapPF != 0) chi2pf[npf] = (*chi2MapPF)[c];
      if (tkisoMapPF != 0) tkisopf[npf] = (*tkisoMapPF)[c];

      if (debug) {
        std::cout << "HLT obj e: " << epf[npf] << " et: " << etpf[npf] << " eta: " << etapf[npf] << " phi: " << phipf[npf] << std::endl;
        std::cout << "       sie: " << sieiepf[npf] << " hoe: " << hoepf[npf]/epf[npf] << " ecc: " << ecalpf[npf]/etpf[npf] << std::endl;
        std::cout << "       hcc: " << hcalpf[npf]/etpf[npf] << " eop: " << eoppf[npf] << " chi: " << chi2pf[npf] << std::endl;
        std::cout << "       mih: " << mishitspf[npf] << " des: " << detaseedpf[npf] << " dph: " << dphipf[npf] << " tki: " << tkisopf[npf]/etpf[npf] << std::endl;
      }

      npf++;

    }
  }

  t->Fill();
  evtAcc = (accPath == 1) ? "passed" : "failed";
  if (debug && npf != 0) {
  std::cout << "+++++++ " << "Event " << evtAcc << " the path!" << " +++++++" << std::endl;
  std::cout << std::endl;
  }

}
 
void plotDistr::beginJob() {
  f = new TFile(outputFileName.c_str(), "recreate");
  t = new TTree("tree", "tree");

  t->Branch("nRun", &nRun, "nRun/I");
  t->Branch("nEvt", &nEvt, "nEvt/I");
  t->Branch("nLumi", &nLumi, "nLumi/I");
  t->Branch("nBX", &nBX, "nBX/I");
  t->Branch("nOrb", &nOrb, "nOrb/I");
  t->Branch("nSto", &nSto, "nSto/I");
  
  t->Branch("accPath", &accPath, "accPath/I");
  t->Branch("nVtx", &nVtx, "nVtx/I");
  t->Branch("rho",  &rho, "rho/F");
  
  t->Branch("npf",  &npf, "npf/I");
  t->Branch("epf", epf, "epf[npf]/F");
  t->Branch("eRawpf", eRawpf, "eRawpf[npf]/F");
  t->Branch("etpf", etpf, "etpf[npf]/F");
  t->Branch("etRawpf", etRawpf, "etRawpf[npf]/F");
  t->Branch("etapf", etapf, "etapf[npf]/F");
  t->Branch("phipf", phipf, "phipf[npf]/F");
  t->Branch("sieiepf", sieiepf, "sieiepf[npf]/F");
  t->Branch("ecalpf", ecalpf, "ecalpf[npf]/F");
  t->Branch("dphipf", dphipf, "dphipf[npf]/F");
  t->Branch("detapf", detapf, "detapf[npf]/F");
  t->Branch("detaseedpf", detaseedpf, "detaseedpf[npf]/F");
  //t->Branch("tkptpf",  tkptpf, "tkptpf[npf]/F");
  //t->Branch("tketapf", tketapf, "tketapf[npf]/F");
  //t->Branch("tkphipf", tkphipf, "tkphipf[npf]/F");
  t->Branch("hoepf",   hoepf, "hoepf[npf]/F");
  t->Branch("hcalpf",   hcalpf, "hcalpf[npf]/F");
  t->Branch("tkisopf",   tkisopf, "tkisopf[npf]/F");
  t->Branch("eoppf", eoppf, "eoppf[npf]/F");
  t->Branch("chi2pf", chi2pf, "chi2pf[npf]/F");
  t->Branch("mishitspf", mishitspf, "mishitspf[npf]/I");
  t->Branch("hitspf", hitspf, "hitspf[npf]/I");

  if (saveReco) {
    t->Branch("reco_n",   &reco_n,   "reco_n/I");
    t->Branch("reco_e",  reco_e,  "reco_e[reco_n]/F");
    t->Branch("reco_et",  reco_et,  "reco_et[reco_n]/F");
    t->Branch("reco_eRaw",  reco_eRaw,  "reco_eRaw[reco_n]/F");
    t->Branch("reco_etRaw",  reco_etRaw,  "reco_etRaw[reco_n]/F");
    t->Branch("reco_pt",  reco_pt,  "reco_pt[reco_n]/F");
    t->Branch("reco_eta", reco_eta, "reco_eta[reco_n]/F");
    t->Branch("reco_phi", reco_phi, "reco_phi[reco_n]/F");
    t->Branch("reco_sieie",  reco_sieie,  "reco_sieie[reco_n]/F");
    t->Branch("reco_hoe", reco_hoe, "reco_hoe[reco_n]/F");
    t->Branch("reco_ecal", reco_ecal, "reco_ecal[reco_n]/F");
    t->Branch("reco_hcal",  reco_hcal,  "reco_hcal[reco_n]/F");
    t->Branch("reco_eop", reco_eop, "reco_eop[reco_n]/F");
    t->Branch("reco_mishits", reco_mishits, "reco_mishits[reco_n]/I");
    t->Branch("reco_deta",  reco_deta,  "reco_deta[reco_n]/F");
    t->Branch("reco_detaseed",  reco_detaseed,  "reco_detaseed[reco_n]/F");
    t->Branch("reco_dphi", reco_dphi, "reco_dphi[reco_n]/F");
    t->Branch("reco_chi2", reco_chi2, "reco_chi2[reco_n]/F");
    t->Branch("reco_tkiso", reco_tkiso, "reco_tkiso[reco_n]/F");
  }

  if (!isData) {
    t->Branch("gevt_wgt", &gevt_wgt, "gevt_wgt/F");
    t->Branch("gpn", &gp_n, "gpn/I");
    t->Branch("gppt", gp_pt, "gppt[gpn]/F");
    t->Branch("gpeta", gp_eta, "gpeta[gpn]/F");
    t->Branch("gpphi", gp_phi, "gpphi[gpn]/F");
    t-> Branch("mc_nBX", &mc_nBX, "mc_nBX/I");
    t-> Branch("mc_BX", mc_BX, "mc_BX[nBX]/I");
    t-> Branch("mc_nPUtrue", &mc_nPUtrue, "mc_nPUtrue/I");
    t-> Branch("mc_nPUobs", mc_nPUobs, "mc_nPUobs[nBX]/I");
  }
}

void plotDistr::endJob() {
  f->cd();
  t->Write();
  hitChi2EB->Write();
  hitChi2EE->Write();
  f->Close();
}

void plotDistr::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  // The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(plotDistr);
