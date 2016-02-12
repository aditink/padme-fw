// BeamParameters.cc
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2016-02-10
// --------------------------------------------------------------

#include "BeamParameters.hh"

BeamParameters* BeamParameters::fInstance = 0;

BeamParameters* BeamParameters::GetInstance()
{
  if ( fInstance == 0 ) { fInstance = new BeamParameters(); }
  return fInstance;
}

BeamParameters::BeamParameters()
{

  // Default is a single 550MeV positron starting at (0,0) and directed along Z
  // No energy, time, position, or angle spreads are applied
  // No Uboson or ThreePhoton decays are inserted

  // Average number of positrons in each bunch
  fNPositronsPerBunch = 1;
  fNPositronsPerBunchApplySpread = false;

  // Bunch time structure
  fBeamApplyBunchStructure = false;
  fBunchTimeLength = 40.*ns;
  fMicroBunchTimeLength = 0.150*ns;
  fMicroBunchTimeDelay = 0.350*ns;

  // Position and spread of beam at Target front face
  fBeamCenterPosX = 0.*cm;
  fBeamCenterPosY = 0.*cm;
  fBeamCenterPosApplySpread = false;
  fBeamCenterPosXSpread = 0.7*mm;
  fBeamCenterPosYSpread = 0.7*mm;

  // Beam momentum
  fBeamMomentum = 550.*MeV;
  fBeamMomentumApplySpread = false;
  fBeamMomentumSpread = 0.01*fBeamMomentum; // 1% of beam momentum

  // Beam direction
  fBeamDirection = G4ThreeVector(0.,0.,1.);

  // Beam emittance (spread of X,Y beam direction components)
  fBeamApplyEmittance = false;
  fBeamEmittanceX = 0.001;
  fBeamEmittanceY = 0.001;

  // Insert Uboson decays
  fNUbosonDecaysPerBunch = 0;
  fUbosonMass = 22.*MeV;

  // Insert three gamma decays
  fNThreePhotonDecaysPerBunch = 0;
  fThreePhotonDecaysFilename = "ThreePhotonDecays.list";

}

BeamParameters::~BeamParameters()
{}