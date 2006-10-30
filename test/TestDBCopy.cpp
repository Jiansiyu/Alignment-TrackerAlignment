#include <string>
#include <map>
#include <vector>

// Framework
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "FWCore/Framework/interface/MakerMacros.h"

// Database
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// Alignment
#include "CondFormats/Alignment/interface/Alignments.h"
#include "CondFormats/DataRecord/interface/TrackerAlignmentRcd.h"
#include "CondFormats/Alignment/interface/AlignmentErrors.h"
#include "CondFormats/DataRecord/interface/TrackerAlignmentErrorRcd.h"


class  TestDBCopy : public edm::EDAnalyzer {
 public:
  explicit  TestDBCopy(const edm::ParameterSet& iConfig ) {};
  ~TestDBCopy() ;

  virtual void analyze( const edm::Event& evt, const edm::EventSetup& evtSetup);

 private:

  Alignments* myAlignments;
  AlignmentErrors* myAlignmentErrors;

};


TestDBCopy::~TestDBCopy( )
{

}


void TestDBCopy::analyze( const edm::Event& evt, const edm::EventSetup& iSetup)
{

  // 1. Retrieve alignment[Error]s from DBase
  edm::ESHandle<Alignments> alignments;
  iSetup.get<TrackerAlignmentRcd>().get( alignments );
  edm::ESHandle<AlignmentErrors> alignmentErrors;
  iSetup.get<TrackerAlignmentErrorRcd>().get( alignmentErrors );

  myAlignments = new Alignments( *alignments );
  myAlignmentErrors = new AlignmentErrors( *alignmentErrors );

  // 2. Store alignment[Error]s to DB
  edm::Service<cond::service::PoolDBOutputService> poolDbService;
  // Call service
  if( !poolDbService.isAvailable() ) // Die if not available
	throw cms::Exception("NotAvailable") << "PoolDBOutputService not available";
	  
  // Define callback tokens for the two records
  size_t alignmentsToken = poolDbService->callbackToken("Alignments");
  size_t alignmentErrorsToken = poolDbService->callbackToken("AlignmentErrors");
	  
  // Store
  poolDbService->newValidityForNewPayload<Alignments>( &(*myAlignments), 
                                                       poolDbService->endOfTime(), 
                                                       alignmentsToken );
  poolDbService->newValidityForNewPayload<AlignmentErrors>( &(*myAlignmentErrors), 
                                                            poolDbService->endOfTime(), 
                                                            alignmentErrorsToken );

}

//define this as a plug-in
DEFINE_FWK_MODULE(TestDBCopy);


