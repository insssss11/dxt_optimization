/// \file B1/src/PrimaryGeneratorActionMessenger.cc
/// \brief Implementation of the B1::PrimaryGeneratorActionMessenger class

#include "PrimaryGeneratorActionMessenger.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorActionMessenger::PrimaryGeneratorActionMessenger(PrimaryGeneratorAction *generator)
    :G4UImessenger(), generator(generator)
{
    loadTubeSpectrumFileCmd = std::make_unique<G4UIcmdWithAString>("/gun/useTubeData", this);
    loadTubeSpectrumFileCmd->SetParameterName("tubeDataFile", false);
    loadTubeSpectrumFileCmd->SetGuidance("Load tube spectrum data and generate keV.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand *command, G4String value)
{
    if(command == loadTubeSpectrumFileCmd.get())
        generator->LoadTubeDataFile(value);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
