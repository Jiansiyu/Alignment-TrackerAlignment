// -*- C++ -*-
//
// Package:    TestAnalyzer
// Class:      TestAnalyzer
// 
//
// Description: Module to test the Alignment software
//
//
// Original Author:  Frederic Ronga
//         Created:  March 16, 2006
//


// system include files

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

#include "Alignment/TrackerAlignment/interface/AlignableTracker.h"
#include "Alignment/TrackerAlignment/interface/AlignableTrackerBarrelLayer.h"


//
//
// class decleration
//

class TestAnalyzer : public edm::EDAnalyzer {
 public:
  explicit TestAnalyzer( const edm::ParameterSet& );
  ~TestAnalyzer();
  
  
  virtual void analyze( const edm::Event&, const edm::EventSetup& );
 private:
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
TestAnalyzer::TestAnalyzer( const edm::ParameterSet& iConfig )
{
   //now do what ever initialization is needed

}


TestAnalyzer::~TestAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
TestAnalyzer::analyze( const edm::Event& iEvent, const edm::EventSetup& iSetup )
{

   
  edm::LogInfo("TrackerAlignment") << "Starting!";

   //
   // Create the tracker builder
   //
  AlignableTracker* trackerBuilder = AlignableTracker::instance( iSetup );

  std::vector<Alignable*> barrelLayers = trackerBuilder->barrelLayers();

  for ( std::vector<Alignable*>::iterator iLayer = barrelLayers.begin();
		iLayer != barrelLayers.end(); iLayer++ )
	{
	  std::cout << "------------------------" << std::endl
				<< " BEFORE" << std::endl;
	  std::cout << *static_cast<AlignableTrackerRod*>((*iLayer)->components().front());

	  float deltaPhi = 3.1415926/180*45;

	  (*iLayer)->rotateAroundGlobalZ( deltaPhi );

	  std::cout << "------------------------" << std::endl
				<< " AFTER" << std::endl;
	  std::cout << *static_cast<AlignableTrackerRod*>((*iLayer)->components().front());
	  std::cout << "------------------------" << std::endl;

	}
  

  delete trackerBuilder;

}

//define this as a plug-in
DEFINE_FWK_MODULE(TestAnalyzer)
