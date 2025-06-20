/// \file B1/include/Geometry.hh
/// \brief Definition of the B1::Geometry class

#ifndef B1Geometry_h
#define B1Geometry_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

// this struct contains geometric parameters, (static const for global access).
struct  GeometryConfig{
    static const G4int numPixels = 3;    
    G4double pixelH = 0.6*mm;
    G4double pixelV = 1*mm;
    G4double pixelP = 0.8*mm;

    G4double ledZ = 0.015*mm;

    G4double pcbH = numPixels*pixelP + 40*mm;
    G4double pcbV = pixelV + 40*mm;
    G4double pcbZ = 1.6*mm;
        
    G4double filterH = numPixels*pixelP + 30*mm;
    G4double filterV = pixelV + 30*mm;
    G4double filterZ = 0.8*mm;

    G4double hedZ = 3*mm;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif