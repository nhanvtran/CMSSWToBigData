// -*- C++ -*-
//
// Package:    Demo/AvroProducer
// Class:      AvroProducer
// 
/**\class AvroProducer AvroProducer.cc Demo/AvroProducer/plugins/AvroProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Nhan Tran
//         Created:  Thu, 12 May 2016 12:58:24 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include <DataFormats/Math/interface/deltaR.h>
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "PhysicsTools/PatAlgos/plugins/PATJetProducer.h"
#include <DataFormats/PatCandidates/interface/Photon.h>
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include <TVector2.h>  

#include "avro.h"
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class AvroProducer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit AvroProducer(const edm::ParameterSet&);
      ~AvroProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      edm::InputTag JetTag_;
      edm::EDGetTokenT<edm::View<pat::Jet>> JetTok_;


      // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
AvroProducer::AvroProducer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   usesResource("TFileService");
   
   JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
   JetTok_ = consumes<edm::View<pat::Jet>>(JetTag_);

}


AvroProducer::~AvroProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
AvroProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


   edm::Handle< edm::View<pat::Jet> > Jets;
   iEvent.getByToken(JetTok_,Jets);
   std::cout << "Jets size = " << Jets->size() << std::endl;


  /* A simple schema for our tutorial */
  const char  PERSON_SCHEMA[] =
  "{\"type\":\"record\",\
    \"name\":\"Person\",\
    \"fields\":[\
       {\"name\": \"ID\", \"type\": \"long\"},\
       {\"name\": \"First\", \"type\": \"string\"},\
       {\"name\": \"Last\", \"type\": \"string\"},\
       {\"name\": \"Phone\", \"type\": \"string\"},\
       {\"name\": \"Age\", \"type\": \"int\"}]}";

  avro_schema_t person_schema;
  avro_schema_from_json_literal(PERSON_SCHEMA, &person_schema);


// #ifdef THIS_IS_AN_EVENT_EXAMPLE
//    Handle<ExampleData> pIn;
//    iEvent.getByLabel("example",pIn);
// #endif
   
// #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//    ESHandle<SetupData> pSetup;
//    iSetup.get<SetupRecord>().get(pSetup);
// #endif


}


// ------------ method called once each job just before starting event loop  ------------
void 
AvroProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
AvroProducer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
AvroProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(AvroProducer);
