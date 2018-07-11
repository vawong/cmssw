#include "SimCalorimetry/HGCalSimProducers/interface/HGCHEbackDigitizer.h"

#include "CLHEP/Random/RandPoissonQ.h"
#include "CLHEP/Random/RandGaussQ.h"

#include "Geometry/HGCalGeometry/interface/HGCalGeometry.h"
#include "Geometry/HcalTowerAlgo/interface/HcalGeometry.h"

#include "vdt/vdtMath.h"

using namespace hgc_digi;
using namespace hgc_digi_utils;

//
template <class DFr>
HGCHEbackDigitizer<DFr>::HGCHEbackDigitizer(const edm::ParameterSet &ps) : HGCDigitizerBase<DFr>(ps)
{
  edm::ParameterSet cfg = ps.getParameter<edm::ParameterSet>("digiCfg");
  keV2MIP_   = cfg.getParameter<double>("keV2MIP");
  this->keV2fC_    = 1.0; //keV2MIP_; // hack for HEB
  noise_MIP_ = cfg.getParameter<edm::ParameterSet>("noise_MIP").getParameter<double>("value");
  nPEperMIP_ = cfg.getParameter<double>("nPEperMIP");
  nTotalPE_  = cfg.getParameter<double>("nTotalPE");
  xTalk_     = cfg.getParameter<double>("xTalk");
  sdPixels_  = cfg.getParameter<double>("sdPixels");
}

//
template <class DFr>
void HGCHEbackDigitizer<DFr>::runDigitizer(std::unique_ptr<HGCHEbackDigitizer::DColl> &digiColl,HGCSimHitDataAccumulator &simData,
				      const CaloSubdetectorGeometry* theGeom, const std::unordered_set<DetId>& validIds,
				      uint32_t digitizationType, CLHEP::HepRandomEngine* engine)
{
  runCaliceLikeDigitizer(digiColl,simData,theGeom,validIds,engine);
}

//
template <class DFr>
void HGCHEbackDigitizer<DFr>::runCaliceLikeDigitizer(std::unique_ptr<HGCHEbackDigitizer::DColl> &digiColl,HGCSimHitDataAccumulator &simData,
						const CaloSubdetectorGeometry* theGeom, const std::unordered_set<DetId>& validIds,
						CLHEP::HepRandomEngine* engine)
{
  //switch to true if you want to print some details
  constexpr bool debug(false);

  HGCSimHitData chargeColl;

  // this represents a cell with no signal charge
  HGCCellInfo zeroData;
  zeroData.hit_info[0].fill(0.f); //accumulated energy
  zeroData.hit_info[1].fill(0.f); //time-of-flight

  for( const auto& id : validIds ) {
    chargeColl.fill(0.f);
    HGCSimHitDataAccumulator::iterator it = simData.find(id);
    HGCCellInfo& cell = ( simData.end() == it ? zeroData : it->second );
    addCellMetadata(cell,theGeom,id);

    for(size_t i=0; i<cell.hit_info[0].size(); ++i)
      {
	//convert total energy keV->MIP, since converted to keV in accumulator
	const float totalIniMIPs( cell.hit_info[0][i]*keV2MIP_ );
	//std::cout << "energy in MIP: " << std::scientific << totalIniMIPs << std::endl;

	  //generate random number of photon electrons
	  const uint32_t npe = std::floor(CLHEP::RandPoissonQ::shoot(engine,totalIniMIPs*nPEperMIP_));

	  //number of pixels
	  const float x = vdt::fast_expf( -((float)npe)/nTotalPE_ );
	  uint32_t nPixel(0);
	  if(xTalk_*x!=1) nPixel=(uint32_t) std::max( nTotalPE_*(1.f-x)/(1.f-xTalk_*x), 0.f );

	  //update signal
	  nPixel = (uint32_t)std::max( CLHEP::RandGaussQ::shoot(engine,(double)nPixel,sdPixels_), 0. );

	  //convert to MIP again and saturate
          float totalMIPs(0.f), xtalk = 0.f;
          const float peDiff = nTotalPE_ - (float) nPixel;
          if (peDiff != 0.f) {
            xtalk = (nTotalPE_-xTalk_*((float)nPixel)) / peDiff;
            if( xtalk > 0.f && nPEperMIP_ != 0.f)
              totalMIPs = (nTotalPE_/nPEperMIP_)*vdt::fast_logf(xtalk);
          }

	  //add noise (in MIPs)
	  chargeColl[i] = totalMIPs+std::max( CLHEP::RandGaussQ::shoot(engine,0.,noise_MIP_), 0. );
	  if(debug && cell.hit_info[0][i]>0)
	    std::cout << "[runCaliceLikeDigitizer] xtalk=" << xtalk << " En=" << cell.hit_info[0][i] << " keV -> " << totalIniMIPs << " raw-MIPs -> " << chargeColl[i] << " digi-MIPs" << std::endl;
	}

      //init a new data frame and run shaper
      DFr newDataFrame( id );
      this->myFEelectronics_->runTrivialShaper( newDataFrame, chargeColl, 1 );

      //prepare the output
      this->updateOutput(digiColl,newDataFrame);
    }
}

//
template <class DFr>
HGCHEbackDigitizer<DFr>::~HGCHEbackDigitizer()
{
}

//explicit instantiations
template class HGCHEbackDigitizer<HGCBHDataFrame>;
template class HGCHEbackDigitizer<HGCHEDataFrame>;
