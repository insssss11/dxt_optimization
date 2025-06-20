/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "G4NistManager.hh"
#include "G4UImanager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Exception.hh"
#include "G4UserLimits.hh"

#include "DetectorConstruction.hh"
#include "ScintillatorSD.hh"
#include "Analysis.hh"
#include "G4ProductionCuts.hh"

GeometryConfig DetectorConstruction::geomData = {};

DetectorConstruction::DetectorConstruction()
    : nistMan(G4NistManager::Instance()),
    matWorld(nullptr), matLed(nullptr), matPcb(nullptr), matFilter(nullptr), matHed(nullptr),
    solidWorld(nullptr), solidLed(nullptr), solidPcb(nullptr), solidFilter(nullptr), solidHed(nullptr),
    logicWorld(nullptr), logicLed(nullptr), logicPcb(nullptr), logicFilter(nullptr), logicHed(nullptr),
    physWorld(nullptr), physPcb(nullptr), physFilter(nullptr),
    ledScintSD(nullptr), filterScintSD(nullptr), hedScintSD(nullptr),
    messenger(nullptr)
{
    messenger = new DetectorConstructionMessenger(this);
    InitMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::InitMaterials()
{
    matWorld = nistMan->FindOrBuildMaterial("G4_AIR");
    matLed = nistMan->FindOrBuildMaterial("G4_G4_GADOLINIUM_OXYSULFIDE");
    matFilter = nistMan->FindOrBuildMaterial("G4_Cu");
    matHed = nistMan->FindOrBuildMaterial("G4_CESIUM_IODIDE");

    // Define elements
    G4Element *C = nistMan->FindOrBuildElement("C");   // Carbon
    G4Element *H = nistMan->FindOrBuildElement("H");   // Hydrogen
    G4Element *O = nistMan->FindOrBuildElement("O");   // Oxygen
    G4Element *Si = nistMan->FindOrBuildElement("Si"); // Silicon
    G4Element *Br = nistMan->FindOrBuildElement("Br"); // Bromine (for flame retardancy)

    // Create FR4 material
    auto matFR4 = new G4Material("G4_FR4", 1.85*g/cm3, 5);
    // Add elements with approximate mass fractions
    matFR4->AddElement(C, 0.40);  // 40% Carbon (epoxy resin)
    matFR4->AddElement(H, 0.05);  // 5% Hydrogen (epoxy resin)
    matFR4->AddElement(O, 0.25);  // 25% Oxygen (SiO2 and epoxy)
    matFR4->AddElement(Si, 0.25); // 25% Silicon (SiO2)
    matFR4->AddElement(Br, 0.05); // 5% Bromine (flame retardant)
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    ConstructSolids();
    ConstructLogicals();
    ConstructPlacements();
    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSolids()
{
    G4double worldX = 10*cm, worldY = 10*cm, worldZ = 10*cm;

    solidWorld = new G4Box("World", worldX/2, worldY / 2, worldZ / 2);
    solidLed = new G4Box("LED", geomData.pixelH/2, geomData.pixelV/2, geomData.ledZ/2);
    solidPcb = new G4Box("PCB", geomData.pcbH/2, geomData.pcbV/2, geomData.pcbZ/2);
    solidFilter = new G4Box("Filter", geomData.filterH/2, geomData.filterV/2, geomData.filterZ/2);
    solidHed = new G4Box("HED", geomData.pixelH/2, geomData.pixelV/2, geomData.hedZ/2);   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructLogicals()
{
    matWorld = nistMan->FindOrBuildMaterial("G4_AIR");
    matLed = nistMan->FindOrBuildMaterial("G4_GADOLINIUM_OXYSULFIDE");
    matPcb = nistMan->FindOrBuildMaterial("G4_FR4");
    matFilter = nistMan->FindOrBuildMaterial("G4_Cu");
    matHed = nistMan->FindOrBuildMaterial("G4_CESIUM_IODIDE");

    logicWorld = new G4LogicalVolume(solidWorld, matWorld, "World");
    logicLed = new G4LogicalVolume(solidLed, matLed, "LED");
    logicPcb = new G4LogicalVolume(solidPcb, matFilter, "PCB");
    logicFilter = new G4LogicalVolume(solidFilter, matHed, "Filter");
    logicHed = new G4LogicalVolume(solidHed, matPcb, "HED");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructPlacements()
{
    physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);
    
    physLeds.assign(geomData.numPixels, nullptr);
    physHeds.assign(geomData.numPixels, nullptr);

    for (G4int idx = 0; idx < geomData.numPixels; idx++)
    {
        G4double disH = (2*idx + 1 - geomData.numPixels)*geomData.pixelP/2;
        G4ThreeVector ledPos(disH, 0, -geomData.ledZ/2);
        G4ThreeVector hedPos(disH, 0, -geomData.hedZ/2 - geomData.ledZ - geomData.pcbZ);

        physLeds.at(idx) = new G4PVPlacement(nullptr, ledPos, logicLed, "LED", logicWorld, true, idx, checkOverlaps);
        physHeds.at(idx) = new G4PVPlacement(nullptr, hedPos, logicHed, "HED", logicWorld, true, idx, checkOverlaps);
    }
    // The filter layer is embedded in the pcb layer
    physPcb = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -geomData.pcbZ/2 - geomData.ledZ), logicPcb, "PCB", logicWorld, false, 0, checkOverlaps);
    physFilter = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), logicFilter, "Filter", logicPcb, false, 0, checkOverlaps);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
    ledScintSD = new ScintillatorSD("ledScint", geomData.numPixels);
    filterScintSD = new ScintillatorSD("filterScint", 1);
    hedScintSD = new ScintillatorSD("hedScint", geomData.numPixels);

    G4SDManager *sdMan = G4SDManager::GetSDMpointer();
    sdMan->AddNewDetector(ledScintSD);
    sdMan->AddNewDetector(filterScintSD);
    sdMan->AddNewDetector(hedScintSD);

    SetSensitiveDetector(logicLed, ledScintSD);
    SetSensitiveDetector(logicFilter, filterScintSD);
    SetSensitiveDetector(logicHed, hedScintSD);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPixelHeight(G4double height)
{
    if(height > geomData.pixelP)
    {
        G4ExceptionDescription desc;
        desc << "Pixel height must be smaller than pixel pitch(" << geomData.pixelP/mm << " mm).";
        G4Exception(
            "DetectorConstruction::SetPixelHeight(G4double height)",
            "InvalidParameter", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        geomData.pixelH = height;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPixelVertical(G4double vertical)
{
    if(vertical < 0)
    {
        G4ExceptionDescription desc;
        desc << "Pixel vertical length must be positive number.";
        G4Exception(
            "DetectorConstruction::SetPixelVertical(G4double vertical)",
            "InvalidParameter", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        geomData.pixelV = vertical;
        geomData.pcbV = geomData.pixelV + 40*mm;
        geomData.filterV = geomData.pixelV + 30*mm;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPixelPitch(G4double pitch)
{
    if(pitch < geomData.pixelH)
    {
        G4ExceptionDescription desc;
        desc << "Pixel pitch must be larger than pixel height(" << geomData.pixelH/mm << " mm).";
        G4Exception(
            "DetectorConstruction::SetPixelPitch(G4double pitch)",
            "InvalidParameter", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        geomData.pixelP = pitch;
        geomData.pcbH = geomData.numPixels*geomData.pixelP + 40*mm;
        geomData.filterH = geomData.numPixels*geomData.pixelP + 30*mm;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(const G4String &nistMatName, G4double)
{
    auto mat = nistMan->FindOrBuildMaterial(nistMatName);
    if(mat == nullptr)
    {
        G4ExceptionDescription desc;
        desc << nistMatName << " is not on the NIST database.\n";
        desc << "Type /material/nist/listMaterials to find available materials.";
        G4Exception(
            "DetectorConstruction::SetWorldMaterial(const G4String &nistMatName, G4double)",
            "InvalidMaterialName", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        G4cout << "Changed material of world to " << nistMatName << G4endl;
        matWorld = mat;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetLedThickness(G4double thickness)
{
    if(thickness < 0)
    {
        G4ExceptionDescription desc;
        desc << "Layer thickness must be positive number.";
        G4Exception(
            "DetectorConstruction::SetLedThickness(G4double thickness)",
            "InvalidParameter", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        geomData.ledZ = thickness;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetLedMaterial(const G4String &nistMatName, G4double)
{
    auto mat = nistMan->FindOrBuildMaterial(nistMatName);
    if(mat == nullptr)
    {
        G4ExceptionDescription desc;
        desc << nistMatName << " is not on the NIST database.\n";
        desc << "Type /material/nist/listMaterials to find available materials.";
        G4Exception(
            "DetectorConstruction::SetLedMaterial(const G4String &nistMatName, G4double)",
            "InvalidMaterialName", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        G4cout << "Changed material of low energy scintillator to " << nistMatName << G4endl;
        matLed = mat;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPcbThickness(G4double thickness)
{
    if(thickness < geomData.filterZ)
    {
        G4ExceptionDescription desc;
        desc << "The pcb thickness must be larger than filter thickness(" << geomData.pcbZ/mm << " mm).";
        G4Exception(
            "DetectorConstruction::SetPcbThickness(G4double thickness)",
            "InvalidParameter", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        geomData.pcbZ = thickness;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPcbMaterial(const G4String &nistMatName, G4double)
{
    auto mat = nistMan->FindOrBuildMaterial(nistMatName);
    if(mat == nullptr)
    {
        G4ExceptionDescription desc;
        desc << nistMatName << " is not on the NIST database.";
        desc << "Type /material/nist/listMaterials to find available materials.";
        G4Exception(
            "DetectorConstruction::SetPcbMaterial(const G4String &nistMatName, G4double)",
            "InvalidMaterialName", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        G4cout << "Changed material of pcb to " << nistMatName << G4endl;
        matPcb = mat;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetFilterThickness(G4double thickness)
{
    if(thickness > geomData.pcbZ)
    {
        G4ExceptionDescription desc;
        desc << "The filter thickness must be smaller than pcb thickness(" << geomData.filterZ/mm << " mm).";
        G4Exception(
            "DetectorConstruction::SetFilterThickness(G4double thickness)",
            "InvalidParameter", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        geomData.filterZ = thickness;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetFilterMaterial(const G4String &nistMatName, G4double)
{
    auto mat = nistMan->FindOrBuildMaterial(nistMatName);
    if(mat == nullptr)
    {
        G4ExceptionDescription desc;
        desc << nistMatName << " is not on the NIST database.";
        desc << "Type /material/nist/listMaterials to find available materials.";
        G4Exception(
            "DetectorConstruction::SetFilterMaterial(const G4String &nistMatName, G4double)",
            "InvalidMaterialName", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        G4cout << "Changed material of filter to " << nistMatName << G4endl;
        matFilter = mat;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetHedThickness(G4double thickness)
{
    if(thickness < 0)
    {
        G4ExceptionDescription desc;
        desc << "Layer thickness must be positive number.";
        G4Exception(
            "DetectorConstruction::SetHedThickness(G4double thickness)",
            "InvalidParameter", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        geomData.hedZ = thickness;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetHedMaterial(const G4String &nistMatName, G4double)
{
    auto mat = nistMan->FindOrBuildMaterial(nistMatName);
    if(mat == nullptr)
    {
        G4ExceptionDescription desc;
        desc << nistMatName << " is not on the NIST database.";
        desc << "Type /material/nist/listMaterials to find available materials.";
        G4Exception(
            "DetectorConstruction::SetHedMaterial(const G4String &nistMatName, G4double)",
            "InvalidMaterialName", G4ExceptionSeverity::JustWarning, desc);
    }
    else
    {
        G4cout << "Changed material of high energy scintillator to " << nistMatName << G4endl;
        matHed = mat;
        Update();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::Update()
{
    UpdateGeometry();
    UpdateMaterials();

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/scene/notifyHandlers");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::UpdateGeometry()
{
    solidLed->SetXHalfLength(geomData.pixelH/2);
    solidLed->SetYHalfLength(geomData.pixelV/2);
    solidLed->SetZHalfLength(geomData.ledZ/2);

    solidPcb->SetXHalfLength(geomData.pcbH/2);
    solidPcb->SetYHalfLength(geomData.pcbV/2);
    solidPcb->SetZHalfLength(geomData.pcbZ/2);

    solidFilter->SetXHalfLength(geomData.filterH/2);
    solidFilter->SetYHalfLength(geomData.filterV/2);
    solidFilter->SetZHalfLength(geomData.filterZ/2);

    solidHed->SetXHalfLength(geomData.pixelH/2);    
    solidHed->SetYHalfLength(geomData.pixelV/2);    
    solidHed->SetZHalfLength(geomData.hedZ/2);
    for (G4int idx = 0; idx < geomData.numPixels; idx++)
    {
        G4double disH = (2*idx + 1 - geomData.numPixels)*geomData.pixelP/2;
        G4ThreeVector ledPos(disH, 0, -geomData.ledZ/2);
        G4ThreeVector hedPos(disH, 0, -geomData.hedZ/2 - geomData.ledZ - geomData.pcbZ);

        physLeds.at(idx)->SetTranslation(ledPos);
        physHeds.at(idx)->SetTranslation(hedPos);
    }
    physPcb->SetTranslation(G4ThreeVector(0, 0, -geomData.pcbZ/2 - geomData.ledZ));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::UpdateMaterials()
{
    logicWorld->SetMaterial(matWorld);
    logicLed->SetMaterial(matLed);
    logicPcb->SetMaterial(matPcb);
    logicFilter->SetMaterial(matFilter);
    logicHed->SetMaterial(matHed);
}

