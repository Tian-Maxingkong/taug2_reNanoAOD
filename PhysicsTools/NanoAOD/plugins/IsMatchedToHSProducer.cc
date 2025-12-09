// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/PatCandidates/interface/IsolatedTrack.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h" 

#include "TLorentzVector.h"

//
// class declaration
//

class IsMatchedToHSProducer : public edm::global::EDProducer<> {
   public:
      explicit IsMatchedToHSProducer(const edm::ParameterSet& iConfig):
        pc_(consumes<pat::PackedCandidateCollection>(iConfig.getParameter<edm::InputTag>("packedPFCandidates1"))), // pf candidates
        gp_(consumes<edm::View<pat::PackedGenParticle>>(iConfig.getParameter<edm::InputTag>("packedGenParticles1")))
      {
        produces<edm::ValueMap<bool>>("isMatchedToGen");
      }
      ~IsMatchedToHSProducer() override {};

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      void produce(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;


      // ----------member data ---------------------------
      edm::EDGetTokenT<pat::PackedCandidateCollection> pc_;
      edm::EDGetTokenT<edm::View<pat::PackedGenParticle>> gp_;
};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// member functions
//

// ------------ method called to produce the data  ------------
void IsMatchedToHSProducer::produce(edm::StreamID streamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const
{

   // packedPFCandidate collection
   edm::Handle<pat::PackedCandidateCollection> pc_handle;
   iEvent.getByToken( pc_, pc_handle );

   // packedGenParticles collection
   edm::Handle<edm::View<pat::PackedGenParticle>> gp_handle;
   iEvent.getByToken( gp_, gp_handle );

   // the map cannot be filled straight away, so create an intermediate vector
   unsigned int Nit = pc_handle->size();
   std::vector<bool> v_isMatchedToGen(Nit,false);

   if (gp_handle.isValid()){
      unsigned int iit=0;
      for (auto iter_pf = pc_handle->begin(); iter_pf != pc_handle->end(); iter_pf++) {
        auto const& pc = *iter_pf;
        bool isMatchedToGen=false;
        if (pc.charge()!=0){
          TLorentzVector recotk;
          recotk.SetPtEtaPhiM(pc.pt(),pc.eta(),pc.phi(),0);
          for (unsigned int j = 0; j<gp_handle->size(); ++j){
             pat::PackedGenParticle const& pk = gp_handle->at(j);
             TLorentzVector genp;
             genp.SetPtEtaPhiM(pk.pt(),pk.eta(),pk.phi(),0);
             if (recotk.DeltaR(genp)<0.1 and (fabs(recotk.Pt()-genp.Pt())/recotk.Pt())<0.5){
                isMatchedToGen=true;
             }
          }
        }
        v_isMatchedToGen[iit] = isMatchedToGen;
        ++iit;
      }
   }

   std::unique_ptr<edm::ValueMap<bool>> vm_isMatchedToGen(new edm::ValueMap<bool>());
   edm::ValueMap<bool>::Filler filler1(*vm_isMatchedToGen);
   filler1.insert(pc_handle,v_isMatchedToGen.begin(),v_isMatchedToGen.end());
   filler1.fill();

   iEvent.put(std::move(vm_isMatchedToGen),"isMatchedToGen");
}



// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void IsMatchedToHSProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("packedPFCandidates1")->setComment("packed PF Candidates collection ");
  desc.add<edm::InputTag>("packedGenParticles1")->setComment("packed Gen Particles collection ");

  descriptions.addWithDefaultLabel(desc);

}

//define this as a plug-in
DEFINE_FWK_MODULE(IsMatchedToHSProducer);
