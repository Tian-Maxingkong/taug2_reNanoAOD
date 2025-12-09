// system include files

#include <memory>


// user include files

#include "FWCore/Framework/interface/global/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

//

// class declaration

//


class DiTauFilter : public edm::global::EDFilter<> {

public:
  explicit DiTauFilter(const edm::ParameterSet&);
  ~DiTauFilter() override;

private:
  bool filter(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;
  const edm::EDGetTokenT<std::vector<pat::Electron>> eSrc;
  const edm::EDGetTokenT<std::vector<pat::Muon>> muSrc;
  const edm::EDGetTokenT<std::vector<pat::Tau>> tauSrc;

  // ----------member data ---------------------------

};


DiTauFilter::DiTauFilter(const edm::ParameterSet& iConfig)

    : 
eSrc{consumes<std::vector<pat::Electron>>(iConfig.getParameter<edm::InputTag>("electrons"))},
muSrc{consumes<std::vector<pat::Muon>>(iConfig.getParameter<edm::InputTag>("muons"))},
tauSrc{consumes<std::vector<pat::Tau>>(iConfig.getParameter<edm::InputTag>("taus"))}
      {}


DiTauFilter::~DiTauFilter() {}


bool DiTauFilter::filter(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const {
  bool result = false;

    edm::Handle<std::vector<pat::Muon>> muons;
    iEvent.getByToken(muSrc, muons);

    edm::Handle<std::vector<pat::Electron>> electrons;
    iEvent.getByToken(eSrc, electrons);

    edm::Handle<std::vector<pat::Tau>> taus;
    iEvent.getByToken(tauSrc, taus);

    bool is_emu=false;
    bool is_ee=false;
    bool is_etau=false;
    bool is_mutau=false;
    bool is_mumu=false;
    bool is_tautau=false;

    int nmu9=0;
    int nmu18=0;
    int nmu24=0;
    int ne9=0;
    int ne23=0;
    int ne32=0;
    int ntau19=0;
    int ntau39=0;

    unsigned int nel = electrons->size();
    unsigned int nmu = muons->size();
    unsigned int ntau = taus->size();

    for (unsigned int i = 0; i < nmu; ++i) {
        const pat::Muon & mu = (*muons)[i];
        if (fabs(mu.eta())<2.4 and mu.isPFMuon() and mu.isMediumMuon()){
	  if (mu.pt()>9) nmu9++;
          if (mu.pt()>18) nmu18++;
          if (mu.pt()>24) nmu24++;
	}
    }

    for (unsigned int i = 0; i < nel; ++i) {
        const pat::Electron & el = (*electrons)[i];
	if (fabs(el.eta())<2.5){
          if (el.pt()>9) ne9++;
          if (el.pt()>23) ne23++;
          if (el.pt()>32) ne32++;
	}
    }

    for (unsigned int i = 0; i < ntau; ++i) {
        const pat::Tau & tau = (*taus)[i];
	if (fabs(tau.eta())<2.3 and tau.tauID("decayModeFindingNewDMs") and tau.tauID("byVVVLooseDeepTau2017v2p1VSjet") and tau.tauID("byVLooseDeepTau2017v2p1VSe") and tau.tauID("byVLooseDeepTau2017v2p1VSmu")){
          if (tau.pt()>19) ntau19++;
          if (fabs(tau.eta())<2.1 and tau.pt()>39) ntau39++;
	}
    }

    if ((nmu9>0 and ne23>0) or (nmu18>0 and ne9>0)) is_emu=true;
    if (ntau39>1) is_tautau=true;
    //if (ne9>1 and ne23>0) is_ee=true;
    //if (nmu9>1 and nmu18>0) is_mumu=true;
    if (ne9>1 and ne32>0) is_ee=true;
    if (nmu9>1 and nmu24>0) is_mumu=true;
    if (nmu18>0 and ntau19>0) is_mutau=true;
    if (ne23>0 and ntau19>0) is_etau=true;

    result = (is_emu or is_ee or is_mumu or is_etau or is_mutau or is_tautau);
    return result;
}

#include "FWCore/Framework/interface/MakerMacros.h"
//define this as a plug-in
DEFINE_FWK_MODULE(DiTauFilter);


