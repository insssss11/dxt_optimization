/// \file DetectorConstructionMessenger.cc
/// \brief Implementation of the DetectorConstructionMessenger class

#include "DetectorConstructionMessenger.hh"

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction *det)
    : G4UImessenger(), detector(det)
{
    InitUiDirectories();
    InitPixelCmds();
    InitThicknessCmds();
    InitMaterialCmds();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command, G4String value)
{
    if(command == pixelHeightCmd.get())
        detector->SetPixelHeight(pixelHeightCmd->GetNewDoubleValue(value));
    else if(command == pixelVerticlCmd.get())
        detector->SetPixelVertical(pixelVerticlCmd->GetNewDoubleValue(value));
    else if(command == pixelPitchCmd.get())
        detector->SetPixelPitch(pixelPitchCmd->GetNewDoubleValue(value));
    else if(command == ledThicknessCmd.get())
        detector->SetLedThickness(ledThicknessCmd->GetNewDoubleValue(value));
    else if(command == pcbThicknessCmd.get())
        detector->SetPcbThickness(pcbThicknessCmd->GetNewDoubleValue(value));
    else if(command == filterThicknessCmd.get())
        detector->SetFilterThickness(filterThicknessCmd->GetNewDoubleValue(value));
    else if(command == hedThicknessCmd.get())
        detector->SetHedThickness(hedThicknessCmd->GetNewDoubleValue(value));
    
    else if(command == worldMatCmd.get())
        detector->SetWorldMaterial(value);    
    else if(command == ledMatCmd.get())
        detector->SetLedMaterial(value);
    else if(command == pcbMatCmd.get())
        detector->SetPcbMaterial(value);
    else if(command == filterMatCmd.get())
        detector->SetFilterMaterial(value);
    else if(command == hedMatCmd.get())
        detector->SetHedMaterial(value);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::InitUiDirectories()
{
    geomUiDir = std::make_unique<G4UIdirectory>("/sensor/");
    geomPixelUiDir = std::make_unique<G4UIdirectory>("/sensor/pixel/");
    geomLedUiDir = std::make_unique<G4UIdirectory>("/sensor/led/");
    geomPcbUiDir = std::make_unique<G4UIdirectory>("/sensor/pcb/");
    geomFilterUiDir = std::make_unique<G4UIdirectory>("/sensor/filter/");
    geomHedUiDir = std::make_unique<G4UIdirectory>("/sensor/hed/");

    geomUiDir->SetGuidance("Sensor control");
    geomPixelUiDir->SetGuidance("Pixel array control");
    geomLedUiDir->SetGuidance("Low energy scintillator control");
    geomPcbUiDir->SetGuidance("Pcb control");
    geomFilterUiDir->SetGuidance("Filter control");
    geomHedUiDir->SetGuidance("High energy scintillator control");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::InitPixelCmds()
{
    pixelHeightCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/sensor/pixel/pixelH", this);
    pixelHeightCmd->SetGuidance("Change pixel height (sensor array direction).");
    pixelHeightCmd->SetParameterName("pixelHeight", false);
    pixelHeightCmd->SetToBeBroadcasted(false);

    pixelVerticlCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/sensor/pixel/pixelV", this);
    pixelVerticlCmd->SetGuidance("Change pixel vertical dimension (along scanning direction).");
    pixelVerticlCmd->SetParameterName("pixelVertical", false);
    pixelVerticlCmd->SetToBeBroadcasted(false);

    pixelPitchCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/sensor/pixel/pixelP", this);
    pixelPitchCmd->SetGuidance("Change pixel pitch.");
    pixelPitchCmd->SetParameterName("pixelPitch", false);
    pixelPitchCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::InitThicknessCmds()
{
    ledThicknessCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/sensor/led/thickness", this);
    ledThicknessCmd->SetGuidance("Change the thickness of low energy scintillator layer.");
    ledThicknessCmd->SetParameterName("ledThickness", false);
    ledThicknessCmd->SetUnitCandidates("nm um mm cm m");
    ledThicknessCmd->SetToBeBroadcasted(false);
    // ledThicknessCmd->AvailableForStates(G4State_Init);

    pcbThicknessCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/sensor/pcb/thickness", this);
    pcbThicknessCmd->SetGuidance("Change the thickness of PCB layer.");
    pcbThicknessCmd->SetParameterName("pcbThickness", false);
    pcbThicknessCmd->SetUnitCandidates("nm um mm cm m");
    pcbThicknessCmd->SetToBeBroadcasted(false);
    // pcbThicknessCmd->AvailableForStates(G4State_GeomClosed);

    filterThicknessCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/sensor/filter/thickness", this);
    filterThicknessCmd->SetGuidance("Change the thickness of filter layer.");
    filterThicknessCmd->SetParameterName("filterThickness", false);
    filterThicknessCmd->SetUnitCandidates("nm um mm cm m");
    filterThicknessCmd->SetToBeBroadcasted(false);
    // filterThicknessCmd->AvailableForStates(G4State_GeomClosed);

    hedThicknessCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>("/sensor/hed/thickness", this);
    hedThicknessCmd->SetGuidance("Change the thickness of high energy scintillator layer.");
    hedThicknessCmd->SetParameterName("hedThickness", false);
    hedThicknessCmd->SetUnitCandidates("nm um mm cm m");
    hedThicknessCmd->SetToBeBroadcasted(false);
    // hedThicknessCmd->AvailableForStates(G4State_GeomClosed);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstructionMessenger::InitMaterialCmds()
{

    worldMatCmd = std::make_unique<G4UIcmdWithAString>("/sensor/world/material", this);
    worldMatCmd->SetGuidance("Change the material of the world volume.");
    worldMatCmd->SetGuidance("The name of material must be one in the NIST database.");
    worldMatCmd->SetParameterName("worldMat", false);
    worldMatCmd->SetToBeBroadcasted(false);
    
    ledMatCmd = std::make_unique<G4UIcmdWithAString>("/sensor/led/material", this);
    ledMatCmd->SetGuidance("Change the material of low energy scintillator layer.");
    ledMatCmd->SetGuidance("The name of material must be one in the NIST database.");
    ledMatCmd->SetParameterName("ledMat", false);
    ledMatCmd->SetToBeBroadcasted(false);

    pcbMatCmd = std::make_unique<G4UIcmdWithAString>("/sensor/pcb/material", this);
    pcbMatCmd->SetGuidance("Change the material of PCB layer.");
    pcbMatCmd->SetGuidance("The name of material must be one in the NIST database.");
    pcbMatCmd->SetParameterName("pcbMat", false);
    pcbMatCmd->SetToBeBroadcasted(false);

    filterMatCmd = std::make_unique<G4UIcmdWithAString>("/sensor/filter/material", this);
    filterMatCmd->SetGuidance("Change the material of filter layer.");
    filterMatCmd->SetGuidance("The name of material must be one in the NIST database.");
    filterMatCmd->SetParameterName("filterMat", false);
    filterMatCmd->SetToBeBroadcasted(false);

    hedMatCmd = std::make_unique<G4UIcmdWithAString>("/sensor/hed/material", this);
    hedMatCmd->SetGuidance("Change the material of high energy scintillator layer.");
    hedMatCmd->SetGuidance("The name of material must be one in the NIST database.");
    hedMatCmd->SetParameterName("hedMat", false);
    hedMatCmd->SetToBeBroadcasted(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......