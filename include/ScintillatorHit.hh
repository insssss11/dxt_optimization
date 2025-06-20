#ifndef ScintillatorHit_h
#define ScintillatorHit_h 1

#include <vector>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ParticleDefinition.hh"

class ScintillatorHit : public G4VHit
{
public:
    ScintillatorHit();
    virtual ~ScintillatorHit();

    ScintillatorHit(const ScintillatorHit &);
    const ScintillatorHit &operator=(const ScintillatorHit &);
    G4bool operator==(const ScintillatorHit &) const;

    inline void *operator new(size_t);
    inline void operator delete(void *);

    void Clear();

    void SetEntPartPDGEncoding(const G4ParticleDefinition *partDef);
    void SetEntPartPDGEncoding(const G4int partDefID);
    // gamma : 22, electron : 11
    G4int GetEntPartPDGEncoding() const;

    void SetEntEnergy(G4double energy);
    const G4double GetEntEnergy() const;
    
    void AddEnergyDeposit(G4double eDep);
    G4double GetEnergyDeposit() const;

    void SetIsPrimary(G4bool passed = true);
    G4bool GetIsPrimary() const;

    void SetPixelIndex(G4bool pixelIdx);
    const G4int GetPixelIndex() const;
private:
    G4int entPartPDGEncoding, pixelIdx;
    G4double entEnergy;
    G4double passingTrkEnergyDeposit;
    G4bool isPrimary;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using ScintillatorHC = G4THitsCollection<ScintillatorHit>;

extern G4ThreadLocal G4Allocator<ScintillatorHit> *ScintillatorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void *ScintillatorHit::operator new(size_t)
{
    if (!ScintillatorHitAllocator)
        ScintillatorHitAllocator = new G4Allocator<ScintillatorHit>;
    return (void *)ScintillatorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void ScintillatorHit::operator delete(void *aHit)
{
    ScintillatorHitAllocator->FreeSingle((ScintillatorHit *)aHit);
}

#endif
