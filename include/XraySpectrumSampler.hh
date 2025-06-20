/// \file B1/include/XraySpectrumSampler.hh
/// \brief Definition of the B1::XraySpectrumSampler class

#ifndef B1XraySpectrumSampler_h
#define B1XraySpectrumSampler_h 1

#include <fstream>

#include "G4String.hh"
#include "G4SystemOfUnits.hh"

class XraySpectrumSampler
{
public:
    XraySpectrumSampler();
    ~XraySpectrumSampler();

    void LoadSpectrumFile(const G4String& file);
    G4double GenerateKeV() const;
    void PrintInfo() const;

    G4bool IsActive() const;
    void Reset();
private:
    void ReadInfo(std::ifstream &stream);
    void ReadData(std::ifstream &stream);
    void InitDist();
    std::vector<std::string> Split(const std::string& s, char delimiter);

private:
    G4bool isActive;
    std::string description;
    G4int nBins;
    std::vector<G4double> keV, fluence, probablilty, cumulative;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
