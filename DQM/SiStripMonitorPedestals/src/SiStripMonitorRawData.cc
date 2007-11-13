// -*- C++ -*-
//
// Package:    SiStripMonitorRawData
// Class:      SiStripMonitorRawData
// 
/**\class SiStripMonitorDigi SiStripMonitorDigi.cc DQM/SiStripMonitorDigi/src/SiStripMonitorDigi.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Suchandra Dutta
//         Created:  Fri June  1 17:00:00 CET 2007
// $Id: SiStripMonitorRawData.cc,v 1.3 2007/11/11 17:07:05 dutta Exp $
//
//

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "CalibTracker/Records/interface/SiStripDetCablingRcd.h"
#include "CalibFormats/SiStripObjects/interface/SiStripDetCabling.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "DQM/SiStripCommon/interface/SiStripHistoId.h"
#include "DQM/SiStripCommon/interface/ExtractTObject.h"

#include "DataFormats/SiStripDetId/interface/SiStripSubStructure.h"
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiStripDigi/interface/SiStripDigi.h"
#include "DataFormats/SiStripDigi/interface/SiStripRawDigi.h"

#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQM/SiStripMonitorPedestals/interface/SiStripMonitorRawData.h"

#include "TH1F.h"
// std
#include <cstdlib>
#include <string>
#include <cmath>
#include <numeric>
#include <algorithm>

SiStripMonitorRawData::SiStripMonitorRawData(edm::ParameterSet const& iConfig):
  BadFedNumber(0),
  dbe_(edm::Service<DaqMonitorBEInterface>().operator->()),
  conf_(iConfig),
  m_cacheID_(0)


{
  edm::LogInfo("SiStripMonitorRawData") <<"SiStripMonitorRawData  " 
					  << " Constructing....... ";     
}


SiStripMonitorRawData::~SiStripMonitorRawData()
{
  edm::LogInfo("SiStripMonitorRawData") <<"SiStripMonitorRawData  " 
					  << " Destructing....... ";     
}
//
// -- Begin Job
//
void SiStripMonitorRawData::beginJob(edm::EventSetup const&) {
}
//
// -- BeginRun
//
void SiStripMonitorRawData::beginRun(edm::Run const& run, edm::EventSetup const& eSetup){
  unsigned long long cacheID = eSetup.get<SiStripDetCablingRcd>().cacheIdentifier();

  TH1F* hist1 = ExtractTObject<TH1F>().extract(BadFedNumber);
  hist1->Reset();
  if (m_cacheID_ != cacheID) {
    m_cacheID_ = cacheID;       
    eSetup.get<SiStripDetCablingRcd>().get( detcabling );
    SelectedDetIds.clear();
    detcabling->addActiveDetectorsRawIds(SelectedDetIds);
    
    edm::LogInfo("SiStripMonitorRawData") <<"SiStripMonitorRawData::beginRun: " 
					  << " Creating MEs for new Cabling ";     
    dbe_->setCurrentFolder("Track/GlobalParameter");
    if (!BadFedNumber) {
      BadFedNumber = dbe_->book1D("FaultyFedNumberAndChannel","Faulty Fed Id and Channel and Numbers", 60000, 0.5, 600.5);
      BadFedNumber->setAxisTitle("Fed Id and Channel numbers",1);
    }
  } 
}


// ------------ method called to produce the data  ------------
void SiStripMonitorRawData::analyze(edm::Event const& iEvent, edm::EventSetup const& iSetup)
{

   edm::LogInfo("SiStripMonitorRawData") <<"SiStripMonitorRawData::analyze: Run "<< 
                              iEvent.id().run()  << " Event " << iEvent.id().event();

  
  iSetup.get<SiStripDetCablingRcd>().get( detcabling );

  // retrieve producer name of input StripDigiCollection
  std::string digiProducer = conf_.getParameter<std::string>("DigiProducer");
  // get DigiCollection object from Event
  edm::Handle< edm::DetSetVector<SiStripRawDigi> > digi_collection;
  std::string digiType = "VirginRaw";
  //you have a collection as there are all the digis for the event for every detector
  iEvent.getByLabel(digiProducer, digiType, digi_collection);

  for (std::vector<uint32_t>::const_iterator idetid=SelectedDetIds.begin(), 
                               iEnd=SelectedDetIds.end();idetid!=iEnd;++idetid){
    std::vector< edm::DetSet<SiStripRawDigi> >::const_iterator digis = digi_collection->find( (*idetid) );
    if (digis == digi_collection->end() ||
        digis->data.size() == 0 || 
        digis->data.size() > 768 )  {
      std::vector<FedChannelConnection> fed_conns = detcabling->getConnections((*idetid));
      for (unsigned int  k = 0; k < fed_conns.size() ; k++) {
        float fed_id = fed_conns[k].fedId() + 0.01*fed_conns[k].fedCh();
        BadFedNumber->Fill(fed_id);
      }
      continue;
    }
  }
}
//
// -- End Run
//    
void SiStripMonitorRawData::endRun(edm::Run const& run, edm::EventSetup const& eSetup) {
  bool outputMEsInRootFile = conf_.getParameter<bool>("OutputMEsInRootFile");
  std::string outputFileName = conf_.getParameter<std::string>("OutputFileName");
  if (outputMEsInRootFile) {    
    dbe_->showDirStructure();
    dbe_->save(outputFileName);
  }
}
//
// -- End Job
//
void SiStripMonitorRawData::endJob(void){
  edm::LogInfo("SiStripMonitorRawData") <<"SiStripMonitorRawData::EndJob: " 
					  << " Finishing!! ";        
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SiStripMonitorRawData);

