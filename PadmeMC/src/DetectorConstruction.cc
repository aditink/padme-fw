//#include "InputParam.input"
#include "Constants.hh" 
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "MagneticFieldSetup.hh"

#include "TargetDetector.hh"
#include "ECalDetector.hh"
#include "SACDetector.hh"
#include "LAVDetector.hh"
#include "PVetoDetector.hh"
#include "EVetoDetector.hh"
#include "HEPVetoDetector.hh"
#include "TDumpDetector.hh"
#include "TPixDetector.hh"

#include "MagnetStructure.hh"
#include "ChamberStructure.hh"
#include "HallStructure.hh"

#include "TPixGeometry.hh"
#include "MagnetGeometry.hh"

#include "G4Material.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"

#include "G4UserLimits.hh"
#include "G4NistManager.hh"
#include "G4VSDFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSNofSecondary.hh"
#include "G4PSTrackLength.hh"
#include "G4PSNofStep.hh"
#include "G4PSMinKinEAtGeneration.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::DetectorConstruction()
:solidWorld(0),  logicWorld(0),  physiWorld(0), stepLimit(0), fWorldLength(0.)
{

  fDetectorMessenger = new DetectorMessenger(this);

  fECalDetector    = new ECalDetector(0);
  fTargetDetector  = new TargetDetector(0);
  fSACDetector     = new SACDetector(0);
  fLAVDetector     = new LAVDetector(0);
  fPVetoDetector   = new PVetoDetector(0);
  fEVetoDetector   = new EVetoDetector(0);
  fHEPVetoDetector = new HEPVetoDetector(0);
  fTDumpDetector   = new TDumpDetector(0);
  fTPixDetector    = new TPixDetector(0);

  fMagnetStructure  = new MagnetStructure(0);
  fChamberStructure = new ChamberStructure(0);
  fHallStructure    = new HallStructure(0);

  fMagneticFieldManager = new MagneticFieldSetup();

  fEnableECal    = 1;
  fEnableTarget  = 1;
  fEnableSAC     = 1;
  fEnableLAV     = 0;
  fEnablePVeto   = 1;
  fEnableEVeto   = 1;
  fEnableHEPVeto = 1;
  fEnableTDump   = 0;
  fEnableTPix    = 0;

  fEnableWall    = 0;
  fEnableChamber = 1;
  fEnableMagnet  = 1;

  fEnableMagneticField = 1;
  fMagneticVolumeIsVisible = 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::~DetectorConstruction()
{

  delete stepLimit;
  delete fDetectorMessenger;             

  delete fECalDetector;
  delete fTargetDetector;
  delete fSACDetector;
  delete fLAVDetector;
  delete fPVetoDetector;
  delete fEVetoDetector;
  delete fHEPVetoDetector;
  delete fTDumpDetector;
  delete fTPixDetector;
  delete fMagnetStructure;
  delete fChamberStructure;
  delete fHallStructure;
  delete fMagneticFieldManager;

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

//--------- Material definition ---------
  G4double a, z, density;
  G4int ncomponents, natoms;
  G4double fractionmass;  
  
  G4Material* Vacuum;
  G4Material* FewAir;

  //LSO crystals for the calorimeter L2S1O5
  G4Element* O  = new G4Element("Oxygen",   "O",  z= 8., a=16.00*g/mole);
  G4Element* N  = new G4Element("Nitrogen" ,"N" , z= 7., a=14.01*g/mole);
  G4Element* Lu = new G4Element("Lutetium", "Lu", z=71., a=174.97*g/mole);
  G4Element* Si = new G4Element("Silicon",  "Si", z=14., a=28.09*g/mole);

  if(IsResGasON==1) {
    //    Vacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,VacuumDensity*mg/cm3,kStateGas,2.93*kelvin,VacuumP*bar);    
    Vacuum = new G4Material("Vacuum", density= (1.290*0.000001)*mg/cm3, ncomponents=2); //1mbar
    Vacuum->AddElement(N, fractionmass=70.*perCent);
    Vacuum->AddElement(O, fractionmass=30.*perCent);
  }else{
    Vacuum = new G4Material("Vacuum",z=1.,a=1.01*g/mole,universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);
  }
  G4Material* LSO = new G4Material("LSO", density=7.4*g/cm3,3);
  LSO->AddElement(Lu,2);
  LSO->AddElement(Si,1);
  LSO->AddElement(O,5);

  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(1);
  G4Material* SiO2     = man->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  G4Material* Air      = man->FindOrBuildMaterial("G4_AIR");
  G4Material* Pb       = man->FindOrBuildMaterial("G4_Pb");
  G4Material* PbWO4    = man->FindOrBuildMaterial("G4_PbWO4");
  G4Material* elC      = man->FindOrBuildMaterial("G4_C");
  G4Material* W        = man->FindOrBuildMaterial("G4_W");
  G4Material* Concrete = man->FindOrBuildMaterial("G4_CONCRETE");
  G4Material* Iron     = man->FindOrBuildMaterial("G4_Fe");
  G4Material* Scint    = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material* G4Vac    = man->FindOrBuildMaterial("G4_Galactic");
  G4Material* BGO      = man->FindOrBuildMaterial("G4_BGO");
  G4Material* BaF2     = man->FindOrBuildMaterial("G4_BARIUM_FLUORIDE");
  G4Material* Cu       = man->FindOrBuildMaterial("G4_Cu");
  G4Material* Al       = man->FindOrBuildMaterial("G4_Al");
  G4Material* Silicon  = man->FindOrBuildMaterial("G4_Si");
  G4Material* Neoprene = man->FindOrBuildMaterial("G4_NEOPRENE");
  G4Material* Mylar = man->FindOrBuildMaterial("G4_MYLAR");
  G4Material* Plexiglass = man->FindOrBuildMaterial("G4_PLEXIGLASS");
  G4Material* StainlessSteel = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* Oxygen = man->FindOrBuildMaterial("G4_O");
  G4Material* Hydrogen = man->FindOrBuildMaterial("G4_H");
  G4Material* Titanium = man->FindOrBuildMaterial("G4_Ti");

  //Materials for SF57

  //  G4Material* Na2O     = man->FindOrBuildMaterial("G4_CALCIUM_OXIDE");

  std::vector<G4String> SiO2El(2);
  std::vector<G4int> SiO2NA(2);
  SiO2El[0] = "Si"; SiO2NA[0] = 1;
  SiO2El[1] = "O";  SiO2NA[1] = 2;
  man->ConstructNewMaterial("SAC_SiO2",SiO2El,SiO2NA,2.32,true);

  // PbO
  std::vector<G4String> PbOEl(2);
  std::vector<G4int> PbONA(2);
  PbOEl[0] = "Pb"; PbONA[0] = 1;
  PbOEl[1] = "O";  PbONA[1] = 1;
  man->ConstructNewMaterial("PbO",PbOEl,PbONA,9.53,true);

  // Na2O
  std::vector<G4String> Na2OEl(2);
  std::vector<G4int> Na2ONA(2);
  Na2OEl[0] = "Na"; Na2ONA[0] = 2;
  Na2OEl[1] = "O";  Na2ONA[1] = 1;
  man->ConstructNewMaterial("Na2O",Na2OEl,Na2ONA,2.27,true);

  // K2O
  std::vector<G4String> K2OEl(2);
  std::vector<G4int> K2ONA(2);
  K2OEl[0] = "K"; K2ONA[0] = 2;
  K2OEl[1] = "O"; K2ONA[1] = 1;
  man->ConstructNewMaterial("K2O",K2OEl,K2ONA,2.32,true);

  // Lead Glass (PbGl)
  G4Material* PbGl = new G4Material("PbGl_SF57",5.57*g/cm3,4);
  PbGl->AddMaterial(G4Material::GetMaterial("SAC_SiO2"), 24.0*perCent); // 22-26%
  PbGl->AddMaterial(G4Material::GetMaterial("PbO"),  74.0*perCent);     // 72-76%
  PbGl->AddMaterial(G4Material::GetMaterial("Na2O"),  1.0*perCent);     // 0.5-1.5%
  PbGl->AddMaterial(G4Material::GetMaterial("K2O"),   1.0*perCent);     // 0.5-1.5%

  //Lead Glass SF57 optical properties
  const G4int nEntries = 10;
  G4double PhotonEnergy[nEntries] =
    { 1.88914*eV, 1.92582*eV, 1.95929*eV, 2.10392*eV, 2.11001*eV,
      2.27035*eV, 2.55059*eV, 2.58300*eV, 2.84497*eV, 3.06360*eV};
  G4double RefractiveIndexPbGl[nEntries] =
    { 1.83650,    1.83808,    1.83957,    1.84636,    1.84666,
      1.85504,    1.87204,    1.87425,    1.89393,    1.91366 };
  G4double AbsorptionPbGl[nEntries] =
    {   420.*cm,    420.*cm,    420.*cm,    420.*cm,    420.*cm,
        420.*cm,    144.*cm,    130.*cm,     34.*cm,      8.*cm };
  // Define property table for Lead Glass
  G4MaterialPropertiesTable* myMPTPbGl = new G4MaterialPropertiesTable();
  myMPTPbGl->AddProperty("RINDEX",PhotonEnergy, RefractiveIndexPbGl, nEntries);
  myMPTPbGl->AddProperty("ABSLENGTH",PhotonEnergy, AbsorptionPbGl, nEntries);
  PbGl->SetMaterialPropertiesTable(myMPTPbGl);

  // Kevlar used for ECal window
  G4Material* Kevlar = new G4Material("Kevlar",1.44*g/cm3,4);
  Kevlar -> AddElement(G4Element::GetElement("H"),0.04);
  Kevlar -> AddElement(G4Element::GetElement("C"),0.71);
  Kevlar -> AddElement(G4Element::GetElement("O"),0.12);
  Kevlar -> AddElement(G4Element::GetElement("N"),0.13);

  // Diamond material to be used by target
  G4Material* Diamond = new G4Material("Diamond",3.515*g/cm3,1);
  Diamond->AddElement(G4Element::GetElement("C"),100.*perCent);

  // EJ510 reflective paint
  G4Material* EJ510Paint = new G4Material("EJ510Paint",1.182*g/cm3,4);
  EJ510Paint->AddElement(G4Element::GetElement("Ti"),41.08*perCent);
  EJ510Paint->AddElement(G4Element::GetElement("C"), 17.18*perCent);
  EJ510Paint->AddElement(G4Element::GetElement("H"),  2.88*perCent);
  EJ510Paint->AddElement(G4Element::GetElement("O"), 38.86*perCent);

  //Print all the materials defined.
  //G4cout << G4endl << "The elements defined are : " << G4endl << G4endl;
  //G4cout << *(G4Element::GetElementTable()) << G4endl;
  //G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
  //------------------------------
  // World Volume
  //------------------------------  
  G4double HalfWorldLength = 0.5*WorldLength*m;
  
  solidWorld = new G4Box("World",HalfWorldLength,HalfWorldLength,HalfWorldLength);
  //logicWorld = new G4LogicalVolume(solidWorld,G4Material::GetMaterial("G4_AIR"),"World",0,0,0);
  logicWorld = new G4LogicalVolume(solidWorld,G4Material::GetMaterial("Vacuum"),"World",0,0,0);
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  physiWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World",0,false,0);
 
  // Create large magnetic volume which includes target, vacuum chamber, magnet, vetoes and timepix
  // but excludes calorimeters

  G4double magVolMinX = -fChamberStructure->GetChamberMostExternalX()-1.*cm;
  G4double magVolMaxX =  fChamberStructure->GetChamberMostExternalX()+1.*cm;

  G4double magVolMinY = -100.*cm;
  G4double magVolMaxY =  100.*cm;

  G4double magVolMinZ = -fChamberStructure->GetChamberMostAdvancedZ()-0.1*mm;
  G4double magVolMaxZ =  fChamberStructure->GetChamberMostAdvancedZ()+0.1*mm; // Right after the thin window flange

  G4double magVolPosX = 0.5*(magVolMinX+magVolMaxX);
  G4double magVolPosY = 0.5*(magVolMinY+magVolMaxY);
  G4double magVolPosZ = 0.5*(magVolMinZ+magVolMaxZ);
  G4ThreeVector magVolPos = G4ThreeVector(magVolPosX,magVolPosY,magVolPosZ);

  G4double magVolHLX = 0.5*(magVolMaxX-magVolMinX);
  G4double magVolHLY = 0.5*(magVolMaxY-magVolMinY);
  G4double magVolHLZ = 0.5*(magVolMaxZ-magVolMinZ);

  //printf ("--- Magnetic Volume ---\n");
  //printf ("%f %f %f %f %f %f\n",magVolMinX,magVolMaxX,magVolMinY,magVolMaxY,magVolMinZ,magVolMaxZ);
  //printf ("%f %f %f\n",magVolPosX,magVolPosY,magVolPosZ);
  //printf ("%f %f %f\n",magVolHLX,magVolHLY,magVolHLZ);

  // The magnetic volume should contain air: will change Vacuum to G4_AIR as soon as the vacuum chamber is complete
  // As of today, the target section is still missing
  G4Box* solidMagneticVolume = new G4Box("MagneticVolume",magVolHLX,magVolHLY,magVolHLZ);
  G4LogicalVolume* logicMagneticVolume = new G4LogicalVolume(solidMagneticVolume,G4Material::GetMaterial("Vacuum"),"MagneticVolume",0,0,0);
  if (fMagneticVolumeIsVisible) {
    logicMagneticVolume->SetVisAttributes(G4VisAttributes(G4Colour::White()));
  } else {
    logicMagneticVolume->SetVisAttributes(G4VisAttributes::Invisible);
  }
  new G4PVPlacement(0,magVolPos,logicMagneticVolume,"MagneticVolume",logicWorld,false,0,false);

  if (fEnableMagneticField) {
    //MagneticFieldSetup* magField = new MagneticFieldSetup();
    logicMagneticVolume->SetFieldManager(fMagneticFieldManager->GetLocalFieldManager(),true);
  }

  // Concrete wall at large Z
  if (fEnableWall) {
    fHallStructure->SetMotherVolume(logicWorld);
    fHallStructure->CreateGeometry();
  }

  // Magnet physical structure
  if (fEnableMagnet) { 
    fMagnetStructure->SetMotherVolume(logicMagneticVolume);
    fMagnetStructure->CreateGeometry();
  }

  // Vacuum chamber structure
  if (fEnableChamber) {
    fChamberStructure->SetMotherVolume(logicMagneticVolume);
    fChamberStructure->CreateGeometry();
  }

  // Target
  if (fEnableTarget) {
    fTargetDetector->SetMotherVolume(fChamberStructure->GetChamberLogicalVolume());
    fTargetDetector->CreateGeometry();
  }

  // SAC
  if (fEnableSAC) {
    fSACDetector->SetMotherVolume(logicWorld);
    fSACDetector->CreateGeometry();
  }

  /*
  // LAV
  if (fEnableLAV) {
    if (fEnableMagnet) {
      fLAVDetector->SetMotherVolume(fMagnetStructure->GetMagneticVolume());
    } else {
      fLAVDetector->SetMotherVolume(logicWorld);
    }
    fLAVDetector->CreateGeometry();
  }
  */

  // TDump
  if (fEnableTDump) {
    fTDumpDetector->SetMotherVolume(logicWorld);
    fTDumpDetector->CreateGeometry();
  }

  // TPix
  if (fEnableTPix) {
    fTPixDetector->SetMotherVolume(logicMagneticVolume);
    // Position of TPix depends on shape of vacuum chamber
    fTPixDetector->SetTPixChamberWallAngle(fChamberStructure->GetChamberBackFaceAngle());
    fTPixDetector->SetTPixChamberWallCorner(fChamberStructure->GetChamberBackFaceCorner());
    fTPixDetector->CreateGeometry();
  }

  // ECal
  if (fEnableECal) {
    fECalDetector->SetMotherVolume(logicWorld);
    fECalDetector->CreateGeometry();
  }

  // PVeto
  if (fEnablePVeto) {
    //fPVetoDetector->SetMotherVolume(logicMagneticVolume);
    fPVetoDetector->SetMotherVolume(fChamberStructure->GetChamberLogicalVolume());
    fPVetoDetector->CreateGeometry();
  }

  // EVeto
  if (fEnableEVeto) {
    //fEVetoDetector->SetMotherVolume(logicMagneticVolume);
    fEVetoDetector->SetMotherVolume(fChamberStructure->GetChamberLogicalVolume());
    fEVetoDetector->CreateGeometry();
  }

  // HEPVeto
  if (fEnableHEPVeto) {
    fHEPVetoDetector->SetMotherVolume(fChamberStructure->GetChamberLogicalVolume());
    // Position of HEPVeto depends on shape of vacuum chamber
    fHEPVetoDetector->SetHEPVetoChamberWallThickness(fChamberStructure->GetChamberBackFaceThickness());
    fHEPVetoDetector->SetHEPVetoChamberWallAngle(fChamberStructure->GetChamberBackFaceAngle());
    fHEPVetoDetector->SetHEPVetoChamberWallCorner(fChamberStructure->GetChamberBackFaceCorner());
    fHEPVetoDetector->CreateGeometry();
  }

  return physiWorld;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetupDetectors()
{}
 
void DetectorConstruction::DefineMaterials()
{}

void DetectorConstruction::setTargetMaterial(G4String materialName)
{
  // search the material by its name 
  G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
  if (pttoMaterial) {
    //TargetMater = pttoMaterial;
    //logicTarget->SetMaterial(pttoMaterial); 
    fTargetDetector->GetTargetLogicalVolume()->SetMaterial(pttoMaterial); 
    //      G4cout << "\n----> The target is " << fTargetLength/cm << " cm of "
    //             << materialName << G4endl;
  }             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((stepLimit)&&(maxStep>0.)) stepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double DetectorConstruction::GetECalFrontFaceZ()
{
  if (fEnableECal) return fECalDetector->GetECalFrontFaceZ();

  // ECal is disabled (?): return a position 300cm after the nominal position of the target
  return 230.*cm;
}


G4double DetectorConstruction::GetTargetFrontFaceZ()
{
  if (fEnableTarget) return fTargetDetector->GetTargetFrontFaceZ();

  // Target is disabled (?): return a position 20cm before the front face of the magnet yoke
  return -70.*cm;
}

G4double DetectorConstruction::GetTargetThickness()
{
  if (fEnableTarget) return fTargetDetector->GetTargetThickness();

  // Target is disabled (?): return 100um
  return 0.1*mm;
}

void DetectorConstruction::EnableSubDetector(G4String det)
{
  printf("Enabling subdetector %s\n",det.data());
  if      (det=="ECal")    { fEnableECal    = 1; }
  else if (det=="Target")  { fEnableTarget  = 1; }
  else if (det=="SAC")     { fEnableSAC     = 1; }
  else if (det=="LAV")     { fEnableLAV     = 1; }
  else if (det=="PVeto")   { fEnablePVeto   = 1; }
  else if (det=="EVeto")   { fEnableEVeto   = 1; }
  else if (det=="HEPVeto") { fEnableHEPVeto = 1; }
  else if (det=="TDump")   { fEnableTDump   = 1; }
  else if (det=="TPix")    { fEnableTPix    = 1; }
  else { printf("WARNING: request to enable unknown subdetector %s\n",det.data()); }
}

void DetectorConstruction::DisableSubDetector(G4String det)
{
  printf("Disabling subdetector %s\n",det.data());
  if      (det=="ECal")    { fEnableECal    = 0; }
  else if (det=="Target")  { fEnableTarget  = 0; }
  else if (det=="SAC")     { fEnableSAC     = 0; }
  else if (det=="LAV")     { fEnableLAV     = 0; }
  else if (det=="PVeto")   { fEnablePVeto   = 0; }
  else if (det=="EVeto")   { fEnableEVeto   = 0; }
  else if (det=="HEPVeto") { fEnableHEPVeto = 0; }
  else if (det=="TDump")   { fEnableTDump   = 0; }
  else if (det=="TPix")    { fEnableTPix    = 0; }
  else { printf("WARNING: request to disable unknown subdetector %s\n",det.data()); }
}

void DetectorConstruction::EnableStructure(G4String str)
{
  printf("Enabling structure %s\n",str.data());
  if      (str=="Wall")    { fEnableWall   = 1;  }
  else if (str=="Chamber") { fEnableChamber = 1; }
  else if (str=="Magnet")  { fEnableMagnet = 1;  }
  else { printf("WARNING: request to enable unknown structure %s\n",str.data()); }
}

void DetectorConstruction::DisableStructure(G4String str)
{
  printf("Disabling structure %s\n",str.data());
  if      (str=="Wall")    { fEnableWall   = 0;  }
  else if (str=="Chamber") { fEnableChamber = 0; }
  else if (str=="Magnet")  { fEnableMagnet = 0;  }
  else { printf("WARNING: request to disable unknown structure %s\n",str.data()); }
}

void DetectorConstruction::EnableMagneticField()
{
  printf("Enabling magnetic field\n");
  fEnableMagneticField = 1;
}

void DetectorConstruction::DisableMagneticField()
{
  printf("Disabling magnetic field\n");
  fEnableMagneticField = 0;
}

void DetectorConstruction::MagneticVolumeIsVisible()
{
  printf("Magnetic volume is visible\n");
  fMagneticVolumeIsVisible = 1;
}

void DetectorConstruction::MagneticVolumeIsInvisible()
{
  printf("Magnetic volume is invisible\n");
  fMagneticVolumeIsVisible = 0;
}

void DetectorConstruction::SetMagFieldValue(G4double v)
{
  printf("Setting constant value of magnetic field to %f\n",v);
  fMagneticFieldManager->SetMagneticFieldValue(v);
}
