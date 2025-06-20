/// \file B1/include/PrimaryGeneratorAction.hh
/// \brief Definition of the B1::PrimaryGeneratorAction class

#ifndef B1PrimaryGeneratorAction_h
#define B1PrimaryGeneratorAction_h 1

#include <memory>

#include "G4VUserPrimaryGeneratorAction.hh"

#include "XraySpectrumSampler.hh"
#include "PrimaryGeneratorActionMessenger.hh"

class PrimaryGeneratorActionMessenger;
class G4ParticleGun;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction() override;

    // method from the base class
    virtual void GeneratePrimaries(G4Event *) override;
    void LoadTubeDataFile(const G4String &);
private:
    G4double GenerateKeV() const;

private:
    G4int nParticles;
    G4double energy;
    std::unique_ptr<PrimaryGeneratorActionMessenger> messenger;
    std::unique_ptr<XraySpectrumSampler> generator;
    std::unique_ptr<G4ParticleGun> particleGun; // pointer a to G4 gun class
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
