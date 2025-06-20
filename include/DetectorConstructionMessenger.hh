/// \file DetectorConstructionMessenger.hh
/// \brief Definition of the DetectorConstructionMessenger class

#ifndef DetectorConstructionMessenger_h
#define DetectorConstructionMessenger_h 1

#include <memory>

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

#include "DetectorConstruction.hh"

class DetectorConstruction;

class DetectorConstructionMessenger : public G4UImessenger
{
public:
    DetectorConstructionMessenger(DetectorConstruction *det);
    virtual ~DetectorConstructionMessenger() override = default;

    virtual void SetNewValue(G4UIcommand *, G4String) override;

private:
    void InitUiDirectories();
    void InitPixelCmds();
    void InitThicknessCmds();
    void InitMaterialCmds();

private:
    DetectorConstruction *detector{nullptr};
    
    std::unique_ptr<G4UIdirectory>
        geomUiDir{nullptr},
        geomPixelUiDir{nullptr}, geomLedUiDir{nullptr}, geomPcbUiDir{nullptr}, geomFilterUiDir{nullptr}, geomHedUiDir{nullptr};
    
    std::unique_ptr<G4UIcmdWithADoubleAndUnit>
        pixelHeightCmd{nullptr}, pixelVerticlCmd{nullptr}, pixelPitchCmd{nullptr};

    std::unique_ptr<G4UIcmdWithADoubleAndUnit>
        ledThicknessCmd{nullptr}, pcbThicknessCmd{nullptr}, filterThicknessCmd{nullptr}, hedThicknessCmd{nullptr};

    std::unique_ptr<G4UIcmdWithAString>
        worldMatCmd{nullptr}, ledMatCmd{nullptr}, pcbMatCmd{nullptr}, filterMatCmd{nullptr}, hedMatCmd{nullptr};
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
