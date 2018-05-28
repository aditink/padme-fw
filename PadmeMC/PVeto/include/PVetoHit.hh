// --------------------------------------------------------------
// History:
//
// Created by Emanuele Leonardi (emanuele.leonardi@roma1.infn.it) 2016-03-17
//
// --------------------------------------------------------------

#ifndef PVetoHit_h
#define PVetoHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PVetoHit : public G4VHit
{

public:

  PVetoHit();
  ~PVetoHit();
  PVetoHit(const PVetoHit&);
  const PVetoHit& operator=(const PVetoHit&);
  G4int operator==(const PVetoHit&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();

public:
    
  void SetChannelId(G4int c) { fChannelId = c; }
  G4int GetChannelId() { return fChannelId; }

  void SetTime(G4double t) { fTime = t; };
  G4double GetTime() { return fTime; };

  void SetEnergy(G4double e) { fEnergy = e; }
  G4double GetEnergy() { return fEnergy; }

  void SetPosition(G4ThreeVector p) { fPosition = p; }
  G4ThreeVector GetPosition() { return fPosition; }
  G4double GetPosX() { return fPosition.x(); };
  G4double GetPosY() { return fPosition.y(); };
  G4double GetPosZ() { return fPosition.z(); };

  void SetLocalPosition(G4ThreeVector p) { fLocalPosition = p; }
  G4ThreeVector GetLocalPosition() { return fLocalPosition; }
  G4double GetLocalPosX() { return fLocalPosition.x(); };
  G4double GetLocalPosY() { return fLocalPosition.y(); };
  G4double GetLocalPosZ() { return fLocalPosition.z(); };

  void SetTrackId(G4int track) { fTrackId = track; };
  G4int GetTrackId() { return fTrackId; };
  
  void SetTrackEnergy(G4double e) { fTrackEnergy = e; }
  G4double GetTrackEnergy() { return fTrackEnergy; }

  // Kept for backward compatibility
  void SetTrackID (G4int track) { fTrackId = track; };
  G4int GetTrackID() { return fTrackId; };

  void SetPVetoNb(G4int cry) { fChannelId = cry; };
  G4int GetPVetoNb() { return fChannelId; };
  
  // Yes, this is how it was done in the past
  void SetEdep(G4double de) { fTrackEnergy = de; };
  G4double GetEdep() { return fTrackEnergy; };
  void SetHitE(G4double hite) { fEnergy = hite; };
  G4double GetHitE() { return fEnergy; };

  void SetPos(G4ThreeVector xyz) { fPosition = xyz; };
  G4ThreeVector GetPos() { return fPosition; };
  G4double GetX() { return fPosition.x(); };
  G4double GetY() { return fPosition.y(); };
    
private:
    
  G4int         fChannelId;
  G4double      fTime;
  G4double      fEnergy;
  G4ThreeVector fPosition;
  G4ThreeVector fLocalPosition;

  G4int    fTrackId;
  G4double fTrackEnergy;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<PVetoHit> PVetoHitsCollection;

extern G4Allocator<PVetoHit> PVetoHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* PVetoHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) PVetoHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PVetoHit::operator delete(void *aHit)
{
  PVetoHitAllocator.FreeSingle((PVetoHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
