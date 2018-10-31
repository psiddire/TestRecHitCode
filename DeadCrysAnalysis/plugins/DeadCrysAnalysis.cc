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



#include "DeadCrysAnalysis.h"

#include <TMath.h>

using namespace edm;
class CaloSubdetectorGeometry;


DeadCrysAnalysis::DeadCrysAnalysis(const edm::ParameterSet&  iConfig)

{


  // recHits_= iConfig.getParameter<bool>("AnalyzeRecHits");
  debug_= iConfig.getParameter<bool>("Debug");
  e3x3Cut_ = iConfig.getParameter<double>("e3x3Cut");
  rhECut_  = iConfig.getParameter<double>("rhECut");
  do5x5_= iConfig.getParameter<bool>("do5x5");
  usePhotons_= iConfig.getParameter<bool>("usePhotons");
  storeLogE_ = iConfig.getParameter<bool>("storeLogE");
  largestCentralCrystal_ = iConfig.getParameter<bool>("useHighestEtRH");
  analyzeElectrons_= iConfig.getParameter<bool>("AnalyzeElectrons");
  rechits_EB_=consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("inputRecHitsEB"));
  gedPhotons_ = consumes<reco::PhotonCollection>(iConfig.getParameter<edm::InputTag>("phoCollection"));
  gsfElectrons_ = consumes<reco::GsfElectronCollection>(iConfig.getParameter<edm::InputTag>("eleCollection"));

  std::cout << " e3x3Cut " << e3x3Cut_ << std::endl;
  if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;


  edm::Service<TFileService> fs;

  tree_ = fs->make<TTree>("TreeR", "TreeR");

  if ( do5x5_) {  
    E_.reserve(25);
    eta_.reserve(25);
    phi_.reserve(25);
    iEta_.reserve(25);
    iPhi_.reserve(25);
  } else {
    E_.reserve(9);
    eta_.reserve(9);
    phi_.reserve(9);
    iEta_.reserve(9);
    iPhi_.reserve(9);

  } 
  if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;


  nPhotons_=0;
  if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

  if ( do5x5_) {
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

    tree_->Branch("E1",&E_[0],"E1/F");
    tree_->Branch("E2",&E_[1],"E2/F");
    tree_->Branch("E3",&E_[2],"E3/F");
    tree_->Branch("E4",&E_[3],"E4/F");
    tree_->Branch("E5",&E_[4],"E5/F");
    tree_->Branch("E6",&E_[5],"E6/F");
    tree_->Branch("E7",&E_[6],"E7/F");
    tree_->Branch("E8",&E_[7],"E8/F");
    tree_->Branch("E9",&E_[8],"E9/F");
    tree_->Branch("E10",&E_[9],"E10/F");
    tree_->Branch("E11",&E_[10],"E11/F");
    tree_->Branch("E12",&E_[11],"E12/F");
    // skip is central tree_->Branch("E13",&E_[12],"E13/F");
    tree_->Branch("E14",&E_[13],"E14/F");
    tree_->Branch("E15",&E_[14],"E15/F");
    tree_->Branch("E16",&E_[15],"E16/F");
    tree_->Branch("E17",&E_[16],"E17/F");
    tree_->Branch("E18",&E_[17],"E18/F");
    tree_->Branch("E19",&E_[18],"E19/F");
    tree_->Branch("E20",&E_[19],"E20/F");
    tree_->Branch("E21",&E_[20],"E21/F");
    tree_->Branch("E22",&E_[21],"E22/F");
    tree_->Branch("E23",&E_[22],"E23/F");
    tree_->Branch("E24",&E_[23],"E24/F");
    tree_->Branch("E25",&E_[24],"E25/F");
    //
    tree_->Branch("target",&E_[12],"target/F");
    tree_->Branch("deadFlag",&deadFlag_,"deadFlag/I");
    
  } else {
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;
    tree_->Branch("E1",&E_[0],"E1/F");
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;
    tree_->Branch("E2",&E_[1],"E2/F");
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;
    tree_->Branch("E3",&E_[2],"E3/F");
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;
    tree_->Branch("E4",&E_[3],"E4/F");
    // skipping the central crystal which is the target (at the bottom of the tree)
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

    tree_->Branch("E6",&E_[5],"E6/F");
    tree_->Branch("E7",&E_[6],"E7/F");
    tree_->Branch("E8",&E_[7],"E8/F");
    tree_->Branch("E9",&E_[8],"E9/F");
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

    // Not sure that the global position is needed 
    tree_->Branch("eta1",&eta_[0],"eta1/F");
    tree_->Branch("eta2",&eta_[1],"eta2/F");
    tree_->Branch("eta3",&eta_[2],"eta3/F");
    tree_->Branch("eta4",&eta_[3],"eta4/F");
    tree_->Branch("eta5",&eta_[4],"eta5/F");
    tree_->Branch("eta6",&eta_[5],"eta6/F");
    tree_->Branch("eta7",&eta_[6],"eta7/F");
    tree_->Branch("eta8",&eta_[7],"eta8/F");
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

    tree_->Branch("phi1",&phi_[0],"phi1/F");
    tree_->Branch("phi2",&phi_[1],"phi2/F");
    tree_->Branch("phi3",&phi_[2],"phi3/F");
    tree_->Branch("phi4",&phi_[3],"phi4/F");
    tree_->Branch("phi5",&phi_[4],"phi5/F");
    tree_->Branch("phi6",&phi_[5],"phi6/F");
    tree_->Branch("phi7",&phi_[6],"phi7/F");
    tree_->Branch("phi8",&phi_[7],"phi8/F");
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

    
    tree_->Branch("iEta1",&iEta_[0],"iEta1/F");
    tree_->Branch("iEta2",&iEta_[1],"iEta2/F");
    tree_->Branch("iEta3",&iEta_[2],"iEta3/F");
    tree_->Branch("iEta4",&iEta_[3],"iEta4/F");
    tree_->Branch("iEta5",&iEta_[4],"iEta5/F");
    tree_->Branch("iEta6",&iEta_[5],"iEta6/F");
    tree_->Branch("iEta7",&iEta_[6],"iEta7/F");
    tree_->Branch("iEta8",&iEta_[7],"iEta8/F");
    tree_->Branch("iEta9",&iEta_[8],"iEta9/F");
    
    tree_->Branch("iPhi1",&iPhi_[0],"iPhi1/F");
    tree_->Branch("iPhi2",&iPhi_[1],"iPhi2/F");
    tree_->Branch("iPhi3",&iPhi_[2],"iPhi3/F");
    tree_->Branch("iPhi4",&iPhi_[3],"iPhi4/F");
    tree_->Branch("iPhi5",&iPhi_[4],"iPhi5/F");
    tree_->Branch("iPhi6",&iPhi_[5],"iPhi6/F");
    tree_->Branch("iPhi7",&iPhi_[6],"iPhi7/F");
    tree_->Branch("iPhi8",&iPhi_[7],"iPhi8/F");
    tree_->Branch("iPhi9",&iPhi_[8],"iPhi9/F");
    //
    // distance of crystals from the cracks
    tree_->Branch("evt",&evt,"evt/L");
    tree_->Branch("run", &run, "run/I");
    tree_->Branch("lumi",&lumi,"lumi/I");


    

    //
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

    tree_->Branch("target",&E_[4],"target/F");
    tree_->Branch("target_w",&target_w_,"target_w/F");	
    tree_->Branch("deadFlag",&deadFlag_,"deadFlag/I");
    if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;
  

  }
  if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;




}


DeadCrysAnalysis::~DeadCrysAnalysis()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

  //histfile_->Write();
  //h_inverse_etrue_->Write();
  //histfile_->Close();

}



//
// member functions
//

// ------------ method called to analyze the data  ------------
void
DeadCrysAnalysis::analyze(const edm::Event& iEvent, const  edm::EventSetup & iSetup)
{
  if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;
  

  // get the geometry
  edm::ESHandle<CaloSubdetectorGeometry> theBarrelGeometry_handle;
  iSetup.get<EcalBarrelGeometryRecord>().get("EcalBarrel",theBarrelGeometry_handle);
  const CaloSubdetectorGeometry *theBarrelGeometry;
  theBarrelGeometry = &(*theBarrelGeometry_handle);
  
 // get the topology
  edm::ESHandle<CaloTopology> theCaloTopo;
  iSetup.get<CaloTopologyRecord>().get(theCaloTopo);
  const CaloTopology *topology = theCaloTopo.product();
  

// get the gsfElectrons
  edm::Handle<reco::GsfElectronCollection> gsfElectronsH;
  iEvent.getByToken(gsfElectrons_,gsfElectronsH);
  // std::cout << " gsfElectron size " << gsfElectronsH->size() << std::endl;

  // get the gedPhotons
  edm::Handle<reco::PhotonCollection> gedPhotonH;
  iEvent.getByToken(gedPhotons_,gedPhotonH);
  if (debug_) std::cout << " gedPhotons size " << gedPhotonH->size() << std::endl ; 
  if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << std::endl;

 // get the  RecHits
  edm::Handle<EcalRecHitCollection> rechit_EB_col;
  iEvent.getByToken(rechits_EB_,rechit_EB_col);
  const EcalRecHitCollection& unfilteredRecHitsEB = *(rechit_EB_col.product());
  EcalRecHitCollection recHitsEB;

  for (unsigned int j=0;j< unfilteredRecHitsEB.size();j++) {
    const EBDetId & id= unfilteredRecHitsEB[j].detid();
    float ene= recHitEnergy( id, & unfilteredRecHitsEB );
    //if (ene <= rhECut_ ) continue;
    recHitsEB.push_back( unfilteredRecHitsEB[j]);
  }


  run=iEvent.run();
  lumi=iEvent.luminosityBlock();
  evt=(long)iEvent.id().event();

  
  if (debug_) std::cout << __PRETTY_FUNCTION__ << " " << __LINE__ << " "<< run << " " << lumi << " "<< evt << std::endl;
    if (debug_) std::cout << " Running on all RecHits " << std::endl; 
    ///   use RecHits from the entire collection
    std::vector<float> cryEnergies;
    std::pair<int, std::vector<DetId>> cryIDPerMatrix;
    std::vector<std::pair<int, std::vector<DetId>>>   cryIDAllMatrices;
    int iMatrix=0;
    std::vector<DetId> v_id;
    if (debug_) std::cout << " recHitsEB.size() " << recHitsEB.size() << std::endl;

    int localDeadFlag=0;
    for (unsigned int j=0;j<recHitsEB.size();j++) {
      const EBDetId & idCurrent=recHitsEB[j].detid();
      //std::cout << " Central RecHit iEta " << idCurrent.ieta() << std::endl;

      EcalRecHitCollection::const_iterator it = recHitsEB.find( idCurrent );
      localDeadFlag=0;
      if( it->checkFlag(EcalRecHit::kDead) ) localDeadFlag=1;
      if (debug_) std::cout << __PRETTY_FUNCTION__ <<  " " << __LINE__ << " " << it->energy() << " " << it->checkFlag(EcalRecHit::kDead) << " " << localDeadFlag << std::endl;
      if ( do5x5_) {
	// take a dead channel at the centre of the matrix
	v_id = EcalClusterTools::matrixDetId( topology, idCurrent, -2, 2, -2, 2 );
      } else {
	// take a dead channel at the centre of the matrix
	v_id = EcalClusterTools::matrixDetId( topology, idCurrent, -1, 1, -1, 1 );

      }
      if (debug_)std::cout << __PRETTY_FUNCTION__ <<  " " << __LINE__ << " size of matrix id " << v_id.size() << std::endl;
      cryEnergies.clear();
      // Looping in over the crystals in the 3x3. Retain only the matrices with only one dead channel
      float E_3x3=0;
      for ( size_t i = 0; i < v_id.size(); ++i ) {
	float cryE= recHitEnergy( v_id[i], &recHitsEB );
	//EcalRecHitCollection::const_iterator jt = (&recHitsEB)->find( v_id[i] );
        E_3x3 += cryE;

	if ( cryE <= 0.0 && idCurrent!=v_id[i] ) continue;
	//if (it->energy()!=0) continue;
	if(it->checkFlag(EcalRecHit::kDead)==false)continue; 
	if (debug_) std::cout << " Cry ene " << recHitEnergy( v_id[i], &recHitsEB ) 
			      << " Log Ene " << log ( EcalClusterTools:: recHitEnergy( v_id[i], &recHitsEB ) )  
			      << " ieta " << ((EBDetId) v_id[i]).ieta() << " iphi " << ((EBDetId) v_id[i]).iphi() << " iMatrix " << iMatrix
			      << std::endl;
	cryEnergies.push_back( recHitEnergy( v_id[i], &recHitsEB ) );
      }
      if (debug_) std::cout << " E_3x3 = " << E_3x3 << std::endl;
      if (do5x5_) { 
	if ( ( localDeadFlag ==0 && cryEnergies.size() < 25 )|| ( localDeadFlag >0 && cryEnergies.size() < 24 ) )  {
	  if (debug_) std::cout << "  There are dead crystals other than the central in this 5x5! Skip it for the time being !  cryEnergies.size " << cryEnergies.size() << std::endl;
	  continue;
	}
      } else {
	///Silvia: modified to keep only dead channels for method crosscheck
        // //if ( E_3x3 < 1 ) continue;
	// if ( ( localDeadFlag ==0 && cryEnergies.size() < 9 )|| ( localDeadFlag >0 && cryEnergies.size() < 8 ) )  {
	//   if (debug_) std::cout << "  There are dead crystals other than the central in this 3x3! Skip it for the time being !  cryEnergies.size " << cryEnergies.size() << std::endl;
	//   continue;
	//}
	if(cryEnergies.size()!=0)std::cout << "cryEnergies.size() " << cryEnergies.size() <<  ", local Dead Flag " << localDeadFlag << " central energy "<< it->energy() <<   std::endl;
	if (cryEnergies.size()<9){
	  continue;
	}
	//std::cout << " E_3x3 " << E_3x3 << std::endl;
	//Silvia: the central is dead, there is nothing to subtract
	//E_3x3 = E_3x3 -  recHitEnergy( v_id[4], &recHitsEB );
	//std::cout << " E_3x3 - central " << E_3x3 << std::endl;
	if ( E_3x3 < e3x3Cut_ ) continue;
      }
      //std::cout << " Flag of the central crystal at this point " << localDeadFlag << std::endl;
      //std::cout << " Crystal: original energy is  " << it->energy() << " after flags " << recHitEnergy( idCurrent, &recHitsEB ) << std::endl;
      // fill the tree with the energies of the surrounding crystals
      // store all matrices with dead central channel
      cryIDPerMatrix.first=iMatrix;
      cryIDPerMatrix.second= v_id;
      cryIDAllMatrices.push_back(cryIDPerMatrix);
      iMatrix++;
      //      deadFlag_=localDeadFlag;           
    } // End loop over all recHits in the event

    // fill the tree
   
    if (debug_) std::cout << " And now fill the tree with the matrices around dead crystals cryIDAllMatrices size " << cryIDAllMatrices.size() << std::endl; 
    for ( size_t iMatrix=0; iMatrix<cryIDAllMatrices.size(); iMatrix++) 
      {
        if (debug_) std::cout << " Cry Size from ID " << ((cryIDAllMatrices[iMatrix]).second).size() << std::endl;
       	int localDeadFlag=0;	
	for ( size_t iCry=0; iCry <((cryIDAllMatrices[iMatrix]).second).size();  iCry++) {
	  localDeadFlag=0;
	  EBDetId myID =  ((cryIDAllMatrices[iMatrix]).second)[iCry];
	  //
	  EcalRecHitCollection::const_iterator it = (&recHitsEB)->find( myID );
	  if ( it == (&recHitsEB)->end() ) continue;
	  if ( (iCry==4 && ! do5x5_) || ( iCry==12 && do5x5_) ) {
	    if( it->checkFlag(EcalRecHit::kDead) ) localDeadFlag=1;

	    //	  
	  } 
	  if (debug_) std::cout << " iMatrix " << iMatrix << " energies from rechit " << it->energy() << " iEta " << myID.ieta() << " iphi " << myID.iphi() << std::endl;
	  float eta =  theBarrelGeometry->getGeometry(myID)->getPosition().eta();
	  float phi =  theBarrelGeometry->getGeometry(myID)->getPosition().phi();
          eta_[iCry]= eta;
	  phi_[iCry]= phi;
	  iEta_[iCry] = float(myID.ieta());
	  iPhi_[iCry] = float(myID.iphi());

          if (iCry==4) {
	    float x=it->energy();
	    //int iBin=h_etrue_->FindBin(x);
	    //float w=h_inverse_etrue_->GetBinContent(iBin);
	    float w=1;
	    //	    std::cout << " iBin " << iBin << std::endl;
	    // std::cout << " energy " << x << " w " << w << " = " << x*w << std::endl;
            if ( storeLogE_ ) { 
	     E_[iCry] = log(it->energy());
	    } else {
	      E_[iCry] = it->energy();
	    }
            target_w_= w;
	  } else {
	    if ( storeLogE_ ) { 
	       E_[iCry] = log(it->energy());
	    } else {
	      E_[iCry] = it->energy();
	    }
	  } 

          if ( do5x5_) {
	    if ( iCry==12) deadFlag_=localDeadFlag;
	    if ( iCry==12 && deadFlag_==1 ) E_[iCry] = 0.0;

	  } else {
	    if ( iCry==4) deadFlag_=localDeadFlag;
	    if ( iCry==4 && deadFlag_==1 ) E_[iCry] = 0.0;
	  }
	  //          if ( iCry==4) std::cout << " Flag " << localDeadFlag << " energy " <<  E_[4] << std::endl;     
	}

	tree_->Fill();
	
      }  // loop over the matrices in the event  





}

void
DeadCrysAnalysis::endJob(){

  std::cout << " Total number of photons analyzed " << nPhotons_ << std::endl;

}
 

float DeadCrysAnalysis::recHitEnergy(DetId id, const EcalRecHitCollection *recHits)
{

  
  if ( id == DetId(0) ) {
    return 0;
  } else {

    EcalRecHitCollection::const_iterator it = recHits->find( id );
    if ( it != recHits->end() ) {

      if( (it->checkFlag(EcalRecHit::kDead) || 
	   it->checkFlag(EcalRecHit::kTowerRecovered) ||
	   it->checkFlag(EcalRecHit::kWeird) ||
	   (it->detid().subdetId() == EcalBarrel && 
	    it->checkFlag(EcalRecHit::kDiWeird) ) ) ) 
	{
	  return 0.0;
	} else {
	return (*it).energy();
      }
    } else {
      //throw cms::Exception("EcalRecHitNotFound") << "The recHit corresponding to the DetId" << id.rawId() << " not found in the EcalRecHitCollection";
      // the recHit is not in the collection (hopefully zero suppressed)
      return 0;
    }
  }
  return 0;
}
 

DEFINE_FWK_MODULE(DeadCrysAnalysis);

