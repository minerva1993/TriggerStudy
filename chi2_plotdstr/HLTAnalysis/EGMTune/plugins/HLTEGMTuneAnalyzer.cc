#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"

#include <string>
#include <iostream>
#include <memory>

class HLTEGMTuneAnalyzer : public edm::one::EDAnalyzer<edm::one::WatchRuns, edm::one::SharedResources>
{
public:
  HLTEGMTuneAnalyzer(const edm::ParameterSet& pset);
  ~HLTEGMTuneAnalyzer() {};

private:
  void analyze(const edm::Event& event, const edm::EventSetup& eventSetup) override;
  void beginRun(const edm::Run& run, const edm::EventSetup& eventSetup) override;
  void endRun(const edm::Run& run, const edm::EventSetup& eventSetup) override {};

  const edm::EDGetTokenT<EcalRecHitCollection> hltEBHitToken_, hltEEHitToken_;
  const edm::EDGetTokenT<reco::GenParticleCollection> genParticleToken_;
  const double genEleMinPt_, genEleMaxEta_;

  HLTConfigProvider hltConfig_;
  const std::vector<std::string> hltPathNames_;
  const edm::EDGetTokenT<edm::TriggerResults> trigResultsToken_;
  std::string processName_;

  TTree* tree_;
  unsigned int b_run;
  unsigned short b_lumi;
  long long b_event;

  unsigned short b_hlt_n;
  const static size_t hlt_N = 100;
  bool b_hlt_flags[hlt_N];

  unsigned short b_recoEcal_n;
  const static size_t recoEcal_N = 100;
  short b_recoEcal_region[recoEcal_N];

  unsigned short b_genEle_n;
  const static size_t genEle_N = 10;
  float b_genEle_pt[genEle_N], b_genEle_eta[genEle_N], b_genEle_phi[genEle_N];
  short b_genEle_q[genEle_N];

};

HLTEGMTuneAnalyzer::HLTEGMTuneAnalyzer(const edm::ParameterSet& pset):
  hltEBHitToken_(consumes<EcalRecHitCollection>(pset.getParameter<edm::InputTag>("hltEBHits"))),
  hltEEHitToken_(consumes<EcalRecHitCollection>(pset.getParameter<edm::InputTag>("hltEEHits"))),
  genParticleToken_(consumes<reco::GenParticleCollection>(pset.getParameter<edm::InputTag>("genParticles"))),
  genEleMinPt_(pset.getParameter<double>("genEleMinPt")),
  genEleMaxEta_(pset.getParameter<double>("genEleMaxEta")),
  hltPathNames_(pset.getParameter<std::vector<std::string>>("hltPaths")),
  trigResultsToken_(consumes<edm::TriggerResults>(pset.getParameter<edm::InputTag>("triggerResults")))
{
  processName_ = pset.getParameter<edm::InputTag>("triggerResults").process();

  usesResource("TFileService");

  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("tree", "tree");
  tree_->Branch("run", &b_run, "run/i");
  tree_->Branch("lumi", &b_lumi, "lumi/s");
  tree_->Branch("event", &b_event, "event/l");

  b_hlt_n = hltPathNames_.size();
  tree_->Branch("hlt_n", &b_hlt_n, "hlt_n/s");
  for ( size_t i=0, n=hltPathNames_.size(); i<n; ++i ) {
    const char* pathName = hltPathNames_.at(i).c_str();
    tree_->Branch(Form("hlt_%s", pathName), &b_hlt_flags[i], Form("hlt_%s[hlt_n]/O", pathName));
  }

  tree_->Branch("recoEcal_n", &b_recoEcal_n, "recoEcal_n/s");
  tree_->Branch("recoEcal_region", &b_recoEcal_region, "recoEcal_region[recoEcal_n]/S");

  tree_->Branch("genEle_n", &b_genEle_n, "genEle_n/s");
  tree_->Branch("genEle_pt", &b_genEle_pt, "genEle_pt[genEle_n]/F");
  tree_->Branch("genEle_eta", &b_genEle_eta, "genEle_eta[genEle_n]/F");
  tree_->Branch("genEle_phi", &b_genEle_phi, "genEle_phi[genEle_n]/F");
  tree_->Branch("genEle_q", &b_genEle_q, "genEle_q[genEle_n]/S");
  
}

void HLTEGMTuneAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& eventSetup)
{
  // Initialize brancies
  b_run = event.id().run();
  b_lumi = event.id().luminosityBlock();
  b_event = event.id().event();
  b_recoEcal_n = 0;
  b_genEle_n = 0;
  for ( int i=0; i<b_hlt_n; ++i ) {
    b_hlt_flags[i] = false;
  }

  // Fill triggerResults
  edm::Handle<edm::TriggerResults> trigResultsHandle;
  event.getByToken(trigResultsToken_, trigResultsHandle);
  const auto trigNames = event.triggerNames(*trigResultsHandle);
  for ( size_t i=0; i<b_hlt_n; ++i ) {
    size_t pathIndex = trigNames.triggerIndex(hltPathNames_.at(i));
    if ( pathIndex >= trigResultsHandle->size() ) continue;
    b_hlt_flags[i] = trigResultsHandle->accept(pathIndex);
  }

  if ( !event.isRealData() ) {
    // Fill generator level electrons
    edm::Handle<reco::GenParticleCollection> genParticleHandle;
    event.getByToken(genParticleToken_, genParticleHandle);
    for ( size_t i=0, n=std::min(genParticleHandle->size(), genEle_N); i<n; ++i ) {
      const auto p = genParticleHandle->at(i);
      if ( p.status() != 1 or std::abs(p.pdgId()) != 11 ) continue;
      if ( p.pt() < genEleMinPt_ or std::abs(p.eta()) >= genEleMaxEta_ ) continue;

      b_genEle_pt[b_genEle_n] = p.pt();
      b_genEle_eta[b_genEle_n] = p.eta();
      b_genEle_phi[b_genEle_n] = p.phi();
      b_genEle_q[b_genEle_n] = p.charge();
      ++b_genEle_n;
    }
  }

  tree_->Fill();
}

void HLTEGMTuneAnalyzer::beginRun(const edm::Run& run, const edm::EventSetup& eventSetup)
{
  bool changed = false;
  hltConfig_.init(run,eventSetup,processName_,changed); //as we need the orginal HLT config...
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HLTEGMTuneAnalyzer);
