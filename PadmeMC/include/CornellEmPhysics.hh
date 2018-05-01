  
  #include "G4VPhysicsConstructor.hh"
  #include "G4EmParticleList.hh"
  #include "globals.hh"
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  class CornellEmPhysics : public G4VPhysicsConstructor
  {
  public:
  
    explicit CornellEmPhysics(G4int ver=0, const G4String& name="");
  
    virtual ~CornellEmPhysics();
  
    virtual void ConstructParticle();
    virtual void ConstructProcess(); 
  private:
    G4int  verbose;
    G4EmParticleList partList;
  };
  

