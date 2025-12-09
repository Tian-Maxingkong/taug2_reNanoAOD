#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/NanoAOD/interface/FlatTable.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/transform.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include <vector>

class ReweightingProducer : public edm::global::EDProducer<> {
protected:
    const std::vector<edm::InputTag> lheLabel_;
    const std::vector<edm::EDGetTokenT<LHEEventProduct>> lheTag_;

public:
    ReweightingProducer(const edm::ParameterSet& params) :
        lheLabel_(params.getParameter<std::vector<edm::InputTag>>("lheInfo")),
        lheTag_(edm::vector_transform(lheLabel_,
            [this](const edm::InputTag& tag) { return this->consumes<LHEEventProduct>(tag); })) {
        this->produces<nanoaod::FlatTable>();
    }

    ~ReweightingProducer() override = default;

    void produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup& iSetup) const override {
        auto lheWeightTable = std::make_unique<nanoaod::FlatTable>(1, "TauG2Weights", true, false);
        edm::Handle<LHEEventProduct> lheInfo;
        for (const auto& lheTag : lheTag_) {
            iEvent.getByToken(lheTag, lheInfo);
            if (lheInfo.isValid()) break;
        }
        if (lheInfo.isValid()) {
            double normWeight = lheInfo->originalXWGTUP();
            for (const auto& weight : lheInfo->weights()) {
                if (weight.id.find("ceBR") != std::string::npos) {
                    lheWeightTable->addColumnValue<float>(
                        weight.id,                  
                        static_cast<float>(weight.wgt / normWeight),  
                        weight.id,                  
                        -1                         
                    );
                }
            }
        }
        iEvent.put(std::move(lheWeightTable));
    }

    static void fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
        edm::ParameterSetDescription desc;
        desc.add<std::vector<edm::InputTag>>("lheInfo", {});
        descriptions.addDefault(desc);
    }

    static void prevalidate(edm::ConfigurationDescriptions& descriptions) {
        fillDescriptions(descriptions);
    }

    static const std::string& baseType() {
        static const std::string type = "EDProducer";
        return type;
    }
};

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(ReweightingProducer);