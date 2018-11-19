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

  run=iEvent.run();
  lumi=iEvent.luminosityBlock();
  evt=(long)iEvent.id().event();

  TLorentzVector pfz;

  // if (muons.isValid() && muons->size() >= 2){
  reco::Muon m1 = muons->at(0);
  reco::Muon m2 = muons->at(1);
  pfz.SetPxPyPzE(m1.px()+m2.px(), m1.py()+m2.py(), m1.pz()+m2.pz(), m1.energy()+m2.energy());

  // zmass = (v1+v2).M();
  zmass = pfz.M();

  met = (pfmets->front()).et();
  
  tree_->Fill();

  // }
}

void
ZSkimTree::endJob(){
}
 
DEFINE_FWK_MODULE(ZSkimTree);

