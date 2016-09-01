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

      edm::InputTag JetTagCHSAK4_;
      edm::EDGetTokenT<edm::View<pat::Jet>> JetTokCHSAK4_;
      edm::InputTag JetTagPUPAK4_;
      edm::EDGetTokenT<edm::View<pat::Jet>> JetTokPUPAK4_;

     // initiate schema
     avro_schema_t event_schema;
     avro_file_writer_t db;

     avro_value_iface_t *avroEventInterface;
     avro_value_t avroEvent;

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
   
   JetTagCHSAK4_ = iConfig.getParameter<edm::InputTag>("JetTagCHSAK4");
   JetTokCHSAK4_ = consumes<edm::View<pat::Jet>>(JetTagCHSAK4_);

   JetTagPUPAK4_ = iConfig.getParameter<edm::InputTag>("JetTagPUPAK4");
   JetTokPUPAK4_ = consumes<edm::View<pat::Jet>>(JetTagPUPAK4_);

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

  // avro_datum_t event = avro_record(event_schema);

  edm::Handle< edm::View<pat::Jet> > patJetsCHSAK4;
  iEvent.getByToken(JetTokCHSAK4_,patJetsCHSAK4);
  edm::Handle< edm::View<pat::Jet> > patJetsPUPAK4;
  iEvent.getByToken(JetTokPUPAK4_,patJetsPUPAK4);
  
  avro_value_t JetsCHSAK4;
  avro_value_t JetsPUPAK4;
  avro_value_get_by_index(&avroEvent,0,&JetsCHSAK4,0);
  avro_value_get_by_index(&avroEvent,1,&JetsPUPAK4,0);

  avro_value_reset(&JetsCHSAK4);
  std::cout << "JetsCHSAK4 size = " << patJetsCHSAK4->size() << std::endl;
  for (unsigned int i = 0; i < patJetsCHSAK4->size(); ++i){
    // std::cout << "jet " << i << ": " << std::endl;
    // std::cout << "  pt,eta,phi,e =" << JetsCHSAK4->at(i).pt() << "," << JetsCHSAK4->at(i).eta() << "," << JetsCHSAK4->at(i).phi() << "," << JetsCHSAK4->at(i).energy() << std::endl;
    avro_value_t JetCHSAK4;
    avro_value_append(&JetsCHSAK4,&JetCHSAK4,0);
    avro_value_t JetCHSAK4_pt;
    avro_value_t JetCHSAK4_eta;
    avro_value_t JetCHSAK4_phi;
    avro_value_get_by_index(&JetCHSAK4,0,&JetCHSAK4_pt,0);
    avro_value_get_by_index(&JetCHSAK4,1,&JetCHSAK4_eta,0);
    avro_value_get_by_index(&JetCHSAK4,2,&JetCHSAK4_phi,0);
    avro_value_set_double(&JetCHSAK4_pt,patJetsCHSAK4->at(i).pt());
    avro_value_set_double(&JetCHSAK4_eta,patJetsCHSAK4->at(i).eta());
    avro_value_set_double(&JetCHSAK4_phi,patJetsCHSAK4->at(i).phi());
  }

  avro_value_reset(&JetsPUPAK4);
  std::cout << "JetsPUPAK4 size = " << patJetsPUPAK4->size() << std::endl;
  for (unsigned int i = 0; i < patJetsPUPAK4->size(); ++i){
    // std::cout << "jet " << i << ": " << std::endl;
    // std::cout << "  pt,eta,phi,e =" << JetsPUPAK4->at(i).pt() << "," << JetsPUPAK4->at(i).eta() << "," << JetsPUPAK4->at(i).phi() << "," << JetsPUPAK4->at(i).energy() << std::endl;
    avro_value_t JetPUPAK4;
    avro_value_append(&JetsPUPAK4,&JetPUPAK4,0);
    avro_value_t JetPUPAK4_pt;
    avro_value_t JetPUPAK4_eta;
    avro_value_t JetPUPAK4_phi;
    avro_value_t JetPUPAK4_m;
    avro_value_get_by_index(&JetPUPAK4,0,&JetPUPAK4_pt,0);
    avro_value_get_by_index(&JetPUPAK4,1,&JetPUPAK4_eta,0);
    avro_value_get_by_index(&JetPUPAK4,2,&JetPUPAK4_phi,0);
    avro_value_get_by_index(&JetPUPAK4,3,&JetPUPAK4_m,0);
    avro_value_set_double(&JetPUPAK4_pt,patJetsPUPAK4->at(i).pt());
    avro_value_set_double(&JetPUPAK4_eta,patJetsPUPAK4->at(i).eta());
    avro_value_set_double(&JetPUPAK4_phi,patJetsPUPAK4->at(i).phi());
    avro_value_set_double(&JetPUPAK4_m,patJetsPUPAK4->at(i).mass());
  }  

// #ifdef THIS_IS_AN_EVENT_EXAMPLE
//    Handle<ExampleData> pIn;
//    iEvent.getByLabel("example",pIn);
// #endif
   
// #ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
//    ESHandle<SetupData> pSetup;
//    iSetup.get<SetupRecord>().get(pSetup);
// #endif

  avro_file_writer_append_value(db,&avroEvent);

}


// ------------ method called once each job just before starting event loop  ------------
void 
AvroProducer::beginJob()
{

    /* A simple schema for our tutorial */
    const char EVENT_SCHEMA[] =
     "{\"type\": \"record\",\n \
     \"name\": \"Event\", \n \
     \"fields\": [ \n \
         {\"name\": \"ak4chsjets\", \n \
          \"type\": {\"type\": \"array\", \"items\": \n \
                   {\"type\": \"record\", \n \
                    \"name\": \"AK4CHSJet\", \n \
                    \"fields\": [ \n \
                        {\"name\": \"pt\", \"type\": \"double\"}, \n \
                        {\"name\": \"eta\", \"type\": \"double\"}, \n \
                        {\"name\": \"phi\", \"type\":\"double\"}]}}}, \n \
        {\"name\": \"ak4pupjets\", \n \
          \"type\": {\"type\": \"array\", \"items\": \n \
                   {\"type\": \"record\", \n \
                    \"name\": \"AK4PUPJet\", \n \
                    \"fields\": [ \n \
                        {\"name\": \"pt\", \"type\": \"double\"}, \n \
                        {\"name\": \"eta\", \"type\": \"double\"}, \n \
                        {\"name\": \"phi\", \"type\":\"double\"},  \n \
                        {\"name\": \"mass\", \"type\":\"double\"}]}}} \n \
                        ]}";

    printf(EVENT_SCHEMA);

    printf("one\n");
    printf("%d\n", avro_schema_from_json_literal(EVENT_SCHEMA, &event_schema));
    fprintf(stderr, " error message: %s\n", avro_strerror());
    printf("one.5\n");
    avroEventInterface = avro_generic_class_from_schema(event_schema);
    printf("two\n");
    // get the interface for the whole event
    avro_generic_value_new(avroEventInterface,&avroEvent);
    printf("three\n");
    const char *dbname = "jets.avro";
    int rval;

    /* Create a new database */
    rval = avro_file_writer_create_with_codec(dbname, event_schema, &db, "null", 0);
    printf("four\n");
    if (rval) {
            fprintf(stderr, "There was an error creating %s\n", dbname);
            fprintf(stderr, " error message: %s\n", avro_strerror());
            exit(EXIT_FAILURE);
    }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
AvroProducer::endJob() 
{
  avro_file_writer_close(db);
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
