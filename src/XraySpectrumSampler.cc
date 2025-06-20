/// \file B1/src/XraySpectrumSampler.cc
/// \brief Implementation of the B1::XraySpectrumSampler class

#include <vector>
#include <algorithm>

#include "G4ios.hh"
#include "G4Exception.hh"
#include "Randomize.hh"

#include "XraySpectrumSampler.hh"
#include "Exception.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XraySpectrumSampler::XraySpectrumSampler() : isActive(false), description(""), nBins(0), keV{}, probablilty{}, cumulative{}
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XraySpectrumSampler::~XraySpectrumSampler()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XraySpectrumSampler::LoadSpectrumFile(const G4String& filename)
{
    ifstream file(filename);
    if(!file.is_open())
    {
        G4ExceptionDescription desc;
        desc << "Failed to open tube data file " << filename << G4endl;
        throw Exception("XraySpectrumSampler::LoadSpectrumFile(const G4String& filename)", "FileNotOpen", JustWarning, desc);
    }
    ReadInfo(file);
    ReadData(file);
    file.close();
    InitDist();

    isActive = true;
    G4cout << "Loaded spectrum data" << G4endl;
    G4cout << "File name : " + filename << G4endl;
    G4cout << "Description : " + description << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XraySpectrumSampler::ReadInfo(std::ifstream &stream)
{
    string line;
    getline(stream, description);
    getline(stream, line); // for the second line containing unit strings
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XraySpectrumSampler::ReadData(std::ifstream &stream)
{
    nBins = 0;
    keV.clear();
    fluence.clear();
    string line;
    try
    {
        while(getline(stream, line))
        {
            if(line.empty())
                continue;
            auto tokens = Split(line, ',');
            keV.push_back(std::stod(tokens.at(0)));
            fluence.push_back(std::stod(tokens.at(1)));
            nBins++;
        }
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
        std::cout << nBins << " : " << line << '\n';        
        throw e;
    }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XraySpectrumSampler::InitDist()
{
    G4double totalFluence = 0;
    probablilty.assign(nBins, 0);
    cumulative.assign(nBins, 0);
    // caculate total and cumulative of fluences
    for(G4int idx = 0;idx < nBins;idx++)
    {
        totalFluence += fluence.at(idx);
        cumulative.at(idx) = totalFluence;
    }
    // normalize to get PDF and CDF
    for(G4int idx = 0;idx < nBins;idx++)
    {
        probablilty.at(idx) = fluence.at(idx)/totalFluence;
        cumulative.at(idx) = cumulative.at(idx)/totalFluence;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<std::string> XraySpectrumSampler::Split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double XraySpectrumSampler::GenerateKeV() const
{
    G4double r = G4UniformRand(); // Thread-safe uniform random number in [0,1)
    // Find the bin where r falls in the CDF
    auto it = std::lower_bound(cumulative.begin(), cumulative.end(), r);
    size_t idx = std::distance(cumulative.begin(), it);

    // return an interpolated value
    if(idx == 0)
        return keV.front();
    else if(idx == nBins)
        return keV.back();
    else
        return keV.at(idx - 1) + (keV.at(idx) - keV.at(idx - 1))*(r - cumulative.at(idx - 1))/(cumulative.at(idx) - cumulative.at(idx - 1));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool XraySpectrumSampler::IsActive() const
{
    return isActive;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XraySpectrumSampler::Reset()
{
    description = "";
    nBins = 0;
    keV.clear();
    fluence.clear();
    probablilty.clear();
    cumulative.clear();
    isActive = false;
}
