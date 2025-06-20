/// \file B1/src/ScintillatorHit.cc
/// \brief Implementation of the B1::ScintillatorHit class

#include "ScintillatorHit.hh"
#include "G4SystemOfUnits.hh"

G4ThreadLocal G4Allocator<ScintillatorHit> *ScintillatorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorHit::ScintillatorHit()
    : G4VHit(),
    entPartPDGEncoding(-1), entEnergy(0.), passingTrkEnergyDeposit(0.), isPrimary(false)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorHit::~ScintillatorHit()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorHit::ScintillatorHit(const ScintillatorHit& other) : G4VHit(other)
{
    entPartPDGEncoding = other.entPartPDGEncoding;
    passingTrkEnergyDeposit = other.passingTrkEnergyDeposit;
    entEnergy = other.entEnergy;
    isPrimary = other.isPrimary;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ScintillatorHit& ScintillatorHit::operator=(const ScintillatorHit& other)
{
    if (this != &other) {
        entPartPDGEncoding = other.entPartPDGEncoding;
        passingTrkEnergyDeposit = other.passingTrkEnergyDeposit;
        entEnergy = other.entEnergy;
        isPrimary = other.isPrimary;
    }
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ScintillatorHit::operator==(const ScintillatorHit& other) const
{
    return (passingTrkEnergyDeposit == other.passingTrkEnergyDeposit
        && entPartPDGEncoding == other.entPartPDGEncoding
        && entEnergy == entEnergy
        && isPrimary == other.isPrimary);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorHit::Clear()
{
    entPartPDGEncoding = -1;
    entEnergy = 0;
    passingTrkEnergyDeposit = 0;
    isPrimary = false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorHit::SetEntPartPDGEncoding(const G4ParticleDefinition *partDef)
{
    entPartPDGEncoding = partDef->GetPDGEncoding();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorHit::SetEntPartPDGEncoding(const G4int partPDGEncoding)
{
    entPartPDGEncoding = partPDGEncoding;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ScintillatorHit::GetEntPartPDGEncoding() const
{
    return entPartPDGEncoding;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorHit::SetEntEnergy(G4double energy)
{
    entEnergy = energy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4double ScintillatorHit::GetEntEnergy() const
{
    return entEnergy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorHit::AddEnergyDeposit(G4double eDep)
{
    passingTrkEnergyDeposit += eDep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double ScintillatorHit::GetEnergyDeposit() const
{
    return passingTrkEnergyDeposit; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorHit::SetIsPrimary(G4bool passed)
{
    isPrimary = passed;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ScintillatorHit::GetIsPrimary() const
{
    return isPrimary;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorHit::SetPixelIndex(G4bool pixelIdx)
{
    this->pixelIdx = pixelIdx;
}
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4int ScintillatorHit::GetPixelIndex() const
{
    return pixelIdx;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
