// system include files
#include <memory>
#include <utility>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"


#include "ZSkimTree.h"
#include "TLorentzVector.h"
#include "Math/VectorUtil.h"

#include <TMath.h>

using namespace edm;
using namespace std;
using namespace reco;
class CaloSubdetectorGeometry;

ZSkimTree::ZSkimTree(const edm::ParameterSet&  iConfig)
{

  //muons_ = consumes<reco::TrackCollection>(iConfig.getParameter<edm::InputTag>("muonCollection"));
  muons_ = consumes<std::vector<reco::Muon> >(iConfig.getParameter<edm::InputTag>("muonCollection"));
  pfmets_ = consumes<reco::PFMETCollection>(iConfig.getParameter<edm::InputTag>("metCollection"));
  vtxToken_ = mayConsume<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"));
  edm::Service<TFileService> fs;

  tree_ = fs->make<TTree>("TreeR", "TreeR");

  tree_->Branch("ZMass",&zmass,"ZMass/F");
  tree_->Branch("MET",&met,"MET/F");
  tree_->Branch("evt",&evt,"evt/L");
  tree_->Branch("run", &run, "run/I");
  tree_->Branch("lumi",&lumi,"lumi/I");

}


ZSkimTree::~ZSkimTree()
{
}

// ------------ method called to analyze the data  ------------
void
ZSkimTree::analyze(const edm::Event& iEvent, const  edm::EventSetup & iSetup)
{

  edm::Handle<std::vector<reco::Muon> > muons; 
  iEvent.getByToken(muons_, muons);

  edm::Handle<reco::PFMETCollection> pfmets;
  iEvent.getByToken(pfmets_, pfmets);

  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vtxToken_, vertices);

  if (vertices->empty()) return; // skip the event if no PV found
  
  // Find the first vertex in the collection that passes
  VertexCollection::const_iterator vertex = vertices->end();
  int firstGoodVertexIdx = 0;
  for (VertexCollection::const_iterator vtx = vertices->begin(); 
       vtx != vertices->end(); ++vtx, ++firstGoodVertexIdx) {
    bool isFake = vtx->isFake();
    // Check the goodness
    if ( !isFake
	 &&  vtx->ndof()>=4. && vtx->position().Rho()<=2.0
	 && fabs(vtx->position().Z())<=24.0) {
      vertex = vtx;
      break;
    }
  }
  if ( vertex==vertices->end() ) return; // skip event if there are no good PVs


  run=iEvent.run();
  lumi=iEvent.luminosityBlock();
  evt=(long)iEvent.id().event();

  TLorentzVector pfz;

  for (std::vector<reco::Muon>::const_iterator it = muons->begin(); it != muons->end(); ++it){
    if( it->pt() > 20 && fabs(it->eta()) < 2.1 && it->isGlobalMuon() && it->isPFMuon() && it->globalTrack()->normalizedChi2() < 10. && it->globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && it->numberOfMatchedStations() > 1 && fabs(it->muonBestTrack()->dxy(vertex->position())) < 0.2 && fabs(it->muonBestTrack()->dz(vertex->position())) < 0.5 && it->innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && it->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5){
      if ((it->pfIsolationR04().sumChargedHadronPt + max(0., it->pfIsolationR04().sumNeutralHadronEt + it->pfIsolationR04().sumPhotonEt - 0.5*it->pfIsolationR04().sumPUPt))/it->pt() < 0.15){ 
	for (std::vector<reco::Muon>::const_iterator jt = it+1; jt != muons->end(); ++jt){
	  if( it->charge() * jt->charge() < 0 ){ 
	    if( jt->pt()>20 && fabs(jt->eta()) < 2.1 && jt->isGlobalMuon() && jt->isPFMuon() && jt->globalTrack()->normalizedChi2() < 10. && jt->globalTrack()->hitPattern().numberOfValidMuonHits() > 0 && jt->numberOfMatchedStations() > 1 && fabs(jt->muonBestTrack()->dxy(vertex->position())) < 0.2 && fabs(jt->muonBestTrack()->dz(vertex->position())) < 0.5 && jt->innerTrack()->hitPattern().numberOfValidPixelHits() > 0 && jt->innerTrack()->hitPattern().trackerLayersWithMeasurement() > 5){
	      if ((jt->pfIsolationR04().sumChargedHadronPt + max(0., jt->pfIsolationR04().sumNeutralHadronEt + jt->pfIsolationR04().sumPhotonEt - 0.5*jt->pfIsolationR04().sumPUPt))/jt->pt() <0.15){
		reco::Candidate::LorentzVector zp4 = it->p4() + jt->p4();
		if (zp4.M() > 70 && zp4.M() < 110){
		  zmass = zp4.M();
		  met = (pfmets->front()).et();
		  tree_->Fill();
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

void
ZSkimTree::endJob(){
}
 
DEFINE_FWK_MODULE(ZSkimTree);

