/// \file B1/src/ScintillatorSD.cc
/// \brief Implementation of the B1::ScintillatorSD class

#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "ScintillatorSD.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorSD::ScintillatorSD(const G4String &detName, G4int numPixels)
    : G4VSensitiveDetector(detName),
    numPixels(numPixels),
    trkHistories{},
    sdMan(G4SDManager::GetSDMpointer()),
    hitCollection(nullptr), hitCollectionID(-1)
{
    collectionName.insert(detName + "HC");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ScintillatorSD::~ScintillatorSD()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorSD::Initialize(G4HCofThisEvent* hce)
{
    hitCollection = new ScintillatorHC(SensitiveDetectorName, SensitiveDetectorName + "HC");
    if(hitCollectionID < 0)
        hitCollectionID = sdMan->GetCollectionID(hitCollection);

    hce->AddHitsCollection(hitCollectionID, hitCollection);
    trkHistories.assign(numPixels, -1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    const auto track = step->GetTrack();
    const G4int pixelIdx = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber();

    const G4ParticleDefinition *partDef = track->GetParticleDefinition();

    // if it is a hit of photon entering sensitive volume
    if(IsEntering(step, track, pixelIdx))
        RegisterEntranceTrk(step, track, pixelIdx);
    ProcessEnergyHit(step, track, pixelIdx);
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool ScintillatorSD::IsEntering(const G4Step* step, const G4Track *track, const G4int pixelIdx) const
{
    // check presteppoint is on the boundary to check entering
    return (step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorSD::RegisterEntranceTrk(const G4Step* step, const G4Track *track, const G4int pixelIdx)
{
    const auto preStepPoint = step->GetPreStepPoint();

    auto hit = new ScintillatorHit();
    hit->SetEntPartPDGEncoding(track->GetParticleDefinition());
    hit->SetEntEnergy(preStepPoint->GetKineticEnergy());
    hit->SetPixelIndex(pixelIdx);
    hit->SetIsPrimary(track->GetParentID() == 0);
    hitCollection->insert(hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4SystemOfUnits.hh"
void ScintillatorSD::ProcessEnergyHit(const G4Step* step, const G4Track *track, const G4int pixelIdx)
{
    trkHistories.at(pixelIdx) = track->GetTrackID();
    if(hitCollection->entries() == 0)
    {
        G4ExceptionDescription desc;
        desc << "Hit occured with no encountered particle to the sensitive volume!" << G4endl;
        const G4String origin("ScintillatorSD::ProcessEnergyHit(const G4Step* step, const G4Track *track, const G4int pixelIdx)");
        G4Exception(origin, "", FatalException, desc);
    }
    auto hit = static_cast<ScintillatorHit*>((*hitCollection)[hitCollection->entries() - 1]);
    hit->AddEnergyDeposit(step->GetTotalEnergyDeposit());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ScintillatorSD::EndOfEvent(G4HCofThisEvent* hce)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ScintillatorHC *ScintillatorSD::GetHitCollection() const
{
    return hitCollection;
}
