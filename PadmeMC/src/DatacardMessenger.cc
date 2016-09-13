#include "DatacardMessenger.hh"
#include "DatacardManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
//#include "G4UIcmdWithADouble.hh"
//#include "G4UIcmdWithAnInteger.hh"
//#include "G4UIcmdWith3Vector.hh"
#include "RootIOManager.hh"

DatacardMessenger::DatacardMessenger(DatacardManager* datacardMng):fDatacardManager(datacardMng)
{

  fOutNameCmd = new G4UIcmdWithAString("/output/DataFileName",this);
  fOutNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fHistoNameCmd = new G4UIcmdWithAString("/output/HistoFileName",this);
  fHistoNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fEnableDetectorIOCmd = new G4UIcmdWithAString("/output/EnableDetectorIO",this);
  fEnableDetectorIOCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fDisableDetectorIOCmd = new G4UIcmdWithAString("/output/DisableDetectorIO",this);
  fDisableDetectorIOCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

DatacardMessenger::~DatacardMessenger() {
    delete fOutNameCmd;
    delete fHistoNameCmd;
    delete fEnableDetectorIOCmd;
    delete fDisableDetectorIOCmd;
}

void DatacardMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {

    G4cout << command->GetCommandName() << " " << newValue << G4endl;

    if (command == fOutNameCmd)   fDatacardManager->SetOutputFileName(newValue);
    if (command == fHistoNameCmd) fDatacardManager->SetHistoFileName(newValue);
    if (command == fEnableDetectorIOCmd) RootIOManager::GetInstance()->EnableSubDetectorIO(newValue);
    if (command == fDisableDetectorIOCmd) RootIOManager::GetInstance()->DisableSubDetectorIO(newValue);

}
