/// \file B1/include/ScintillatorSD.hh
/// \brief Definition of the B1::ScintillatorSD class

#ifndef B1ScintillatorSD_h
#define B1ScintillatorSD_h 1

#include <vector>

#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "ScintillatorHit.hh"

class ScintillatorSD : public G4VSensitiveDetector
{
public:
    ScintillatorSD(const G4String &detName, G4int numPixels);
    virtual ~ScintillatorSD() override;

    // Methods from base class
    virtual void Initialize(G4HCofThisEvent* hce) override;
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    virtual void EndOfEvent(G4HCofThisEvent* hce) override;

    const ScintillatorHC *GetHitCollection() const;
    void SetEnteringPhotonHits(std::vector<std::vector<G4double> > *);

    private:
        G4bool IsEntering(const G4Step* step, const G4Track *track, const G4int pixelIdx) const;
        void RegisterEntranceTrk(const G4Step* step, const G4Track *track, const G4int pixelIdx);
        void ProcessEnergyHit(const G4Step* step, const G4Track *track, const G4int pixelIdx);
    private:
        const G4int numPixels;
        std::vector <G4int> trkHistories;
        G4SDManager *sdMan;
        ScintillatorHC *hitCollection;
        G4int hitCollectionID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
