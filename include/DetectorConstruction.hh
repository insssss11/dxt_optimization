/// \file B1/include/DetectorConstruction.hh
/// \brief Definition of the B1::DetectorConstruction class

#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1


#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"

#include "DetectorConstructionMessenger.hh"
#include "ScintillatorSD.hh"
#include "GeometryConfig.hh"

class DetectorConstructionMessenger;

class G4VPhysicalVolume;
class G4LogicalVolume;

// pixel structure data (H : direction parallel to sensor array, V : scanning direction)
// layer ordered along X-ray incident direction
// 1. scintilation layer for low energy x-ray(ledPix, gadox)
// 2. pcb layer (filter layer is embedded in pcb layer)
// 3. scintilation layer for high energy x-ray(hedPix, CsI)


class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction() override;

    virtual G4VPhysicalVolume *Construct() override;

    virtual void ConstructSDandField () override;

    void SetPixelHeight(G4double height);
    void SetPixelVertical(G4double vertical);
    void SetPixelPitch(G4double pitch);

    void SetWorldMaterial(const G4String &nistMatName, G4double density = 0.0*g/cm3);

    void SetLedThickness(G4double thickness);
    void SetLedMaterial(const G4String &nistMatName, G4double density = 0.0*g/cm3);
    
    void SetPcbThickness(G4double thickness);
    void SetPcbMaterial(const G4String &nistMatName, G4double density = 0.0*g/cm3);
    
    void SetFilterThickness(G4double thickness);
    void SetFilterMaterial(const G4String &nistMatName, G4double density = 0.0*g/cm3);
    
    void SetHedThickness(G4double thickness);
    void SetHedMaterial(const G4String &nistMatName, G4double density = 0.0*g/cm3);

    static GeometryConfig geomData;
private:

    G4NistManager *nistMan;
    G4Material *matWorld, *matLed, *matPcb, *matFilter, *matHed;
    
    G4Box *solidWorld, *solidLed, *solidPcb, *solidFilter, *solidHed;
    G4LogicalVolume *logicWorld, *logicLed, *logicPcb, *logicFilter, *logicHed;

    G4VPhysicalVolume *physWorld, *physPcb, *physFilter;
    std::vector<G4VPhysicalVolume*> physLeds, physHeds;

    ScintillatorSD *ledScintSD, *filterScintSD, *hedScintSD;

    DetectorConstructionMessenger *messenger;

    const G4bool checkOverlaps = true;

private:
    // Define material definitions
    void InitMaterials();

    void Update();
    void UpdateGeometry();
    void UpdateMaterials();

    void ConstructSolids();
    void ConstructLogicals();
    void ConstructPlacements();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
