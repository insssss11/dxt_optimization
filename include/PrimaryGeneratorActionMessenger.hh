/// \file PrimaryGeneratorActionMessenger.hh
/// \brief Definition of the PrimaryGeneratorActionMessenger class

#ifndef PrimaryGeneratorActionMessenger_h
#define PrimaryGeneratorActionMessenger_h 1

#include <memory>

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

#include "PrimaryGeneratorAction.hh"

class PrimaryGeneratorAction;

class PrimaryGeneratorActionMessenger : public G4UImessenger
{
public:
    PrimaryGeneratorActionMessenger(PrimaryGeneratorAction *);
    virtual ~PrimaryGeneratorActionMessenger() override = default;

    virtual void SetNewValue(G4UIcommand *, G4String) override;

private:
    PrimaryGeneratorAction *generator{nullptr};
    std::unique_ptr<G4UIcmdWithAString> loadTubeSpectrumFileCmd;
    std::unique_ptr<G4UIcmdWithADoubleAndUnit> setParticleEnergyCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
