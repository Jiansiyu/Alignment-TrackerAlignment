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
  explicit  TestDBCopy(const edm::ParameterSet& iConfig ) :
    theAlignRecordName( "Alignments" ),
    theErrorRecordName( "AlignmentErrors" ) {}

  ~TestDBCopy() {}

  virtual void analyze( const edm::Event& evt, const edm::EventSetup& evtSetup);

 private:

  Alignments* myAlignments;
  AlignmentErrors* myAlignmentErrors;

  std::string theAlignRecordName, theErrorRecordName;

};


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

  if ( poolDbService->isNewTagRequest(theAlignRecordName) )
    poolDbService->createNewIOV<Alignments>( &(*myAlignments), poolDbService->endOfTime(), 
                                             theAlignRecordName );
  else
    poolDbService->appendSinceTime<Alignments>( &(*myAlignments), poolDbService->currentTime(), 
                                                theAlignRecordName );
  if ( poolDbService->isNewTagRequest(theErrorRecordName) )
    poolDbService->createNewIOV<AlignmentErrors>( &(*myAlignmentErrors),
                                                  poolDbService->endOfTime(), 
                                                  theErrorRecordName );
  else
    poolDbService->appendSinceTime<AlignmentErrors>( &(*myAlignmentErrors),
                                                     poolDbService->currentTime(), 
                                                     theErrorRecordName );

}

//define this as a plug-in
DEFINE_FWK_MODULE(TestDBCopy);


