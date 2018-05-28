// ECalSD.cc
// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2105-12-14
// --------------------------------------------------------------

#include "ECalSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Neutron.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionZero.hh"
#include "G4VProcess.hh"

ECalSD::ECalSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="ECalCollection"); //crea il collection name
}

ECalSD::~ECalSD(){}

void ECalSD::Initialize(G4HCofThisEvent* HCE)
{
  fECalCollection = new ECalHitsCollection(SensitiveDetectorName,collectionName[0]);
  static G4int HCID = -1;
  if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection(HCID,fECalCollection); 
}

G4bool ECalSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{

  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return false;

  G4TouchableHandle touchHPre = aStep->GetPreStepPoint()->GetTouchableHandle();

  ECalHit* newHit = new ECalHit();

  //newHit->SetChannelId(touchHPre->GetCopyNumber());
  newHit->SetChannelId(touchHPre->GetCopyNumber(1)); // Copy id is that of the cell, not of the crystal
  newHit->SetEnergy(edep);
  newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());

  G4ThreeVector worldPosPre = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector localPosPre = touchHPre->GetHistory()->GetTopTransform().TransformPoint(worldPosPre);
  //G4cout << "PreStepPoint in " << touchHPre->GetVolume()->GetName()
  //	 << " global " << G4BestUnit(worldPosPre,"Length")
  //	 << " local " << G4BestUnit(localPosPre,"Length") << G4endl;

  //G4ThreeVector worldPosPost = aStep->GetPostStepPoint()->GetPosition();
  //G4TouchableHandle touchHPost = aStep->GetPostStepPoint()->GetTouchableHandle();
  //G4ThreeVector localPosPost = touchHPost->GetHistory()->GetTopTransform().TransformPoint(worldPosPost);
  //G4cout << "PostStepPoint in " << touchHPost->GetVolume()->GetName()
  //	 << " global " << G4BestUnit(worldPosPost,"Length")
  //	 << " local " << G4BestUnit(localPosPost,"Length") << G4endl;

  newHit->SetPosition(worldPosPre);
  newHit->SetLocalPosition(localPosPre);

  newHit->SetPType(ClassifyTrack(aStep->GetTrack()));

  fECalCollection->insert(newHit);

  //newHit->Print();

  return true;

}

G4int ECalSD::ClassifyTrack(G4Track* track)
{

  G4ParticleDefinition* particleType = track->GetDefinition();
  if (particleType == G4Gamma::GammaDefinition()) {
    return 1;
  } else if (particleType == G4Positron::PositronDefinition()) {
    return 2;
  } else if (particleType == G4Electron::ElectronDefinition()) {
    return 3;
  } else if (particleType == G4Neutron::NeutronDefinition()) {
    return 4;
  } else if (particleType == G4PionPlus::PionPlusDefinition() ||
	     particleType == G4PionMinus::PionMinusDefinition()) {
    return 5;
  } else if (particleType == G4PionZero::PionZeroDefinition()) {
    return 6;
  } else if (particleType == G4MuonPlus::MuonPlusDefinition() ||
	     particleType == G4MuonMinus::MuonMinusDefinition()) {
    return 7;
  } else return -1;

}

void ECalSD::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel>0) { 
    G4int NbHits = fECalCollection->entries();
    G4cout << "\n-- ECal Hits Collection: " << NbHits << " hits --" << G4endl;
    for (G4int i=0;i<NbHits;i++) (*fECalCollection)[i]->Print();
  }
}
