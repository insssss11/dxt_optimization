/// \file B1/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the B1::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "Exception.hh"
#include "XraySpectrumSampler.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), nParticles(1), energy(100*keV)
{
    messenger = std::make_unique<PrimaryGeneratorActionMessenger>(this);
    particleGun = std::make_unique<G4ParticleGun>(nParticles);
    generator = std::make_unique<XraySpectrumSampler>();

    // default particle kinematic
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.));
    particleGun->SetParticleEnergy(energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::LoadTubeDataFile(const G4String &filename)
{
    try
    {
        generator->LoadSpectrumFile(filename);
    }
    catch(Exception &e)
    {
        e.Execute();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
    if(generator->IsActive())
        particleGun->SetParticleEnergy(generator->GenerateKeV()*keV);
    particleGun->SetParticlePosition(G4ThreeVector(0, 0, 2*cm));
    particleGun->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
