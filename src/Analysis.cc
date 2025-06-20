/// \file B1/src/Analysis.cc
/// \brief Implementation of the B1::Analysis class

#include <iomanip>

#include "G4Threading.hh"

#include "GeometryConfig.hh"
#include "Analysis.hh"
#include "ScintillatorSD.hh"
#include "DetectorConstruction.hh"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis::Analysis() :
    tupleVectorPtrs{},
    isActive(false),
    ledHC(nullptr), filterHC(nullptr), hedHC(nullptr),
    sdManager(G4SDManager::GetSDMpointer()),
    anaManager(G4RootAnalysisManager::Instance())
{
    ledNumPixels = GeometryConfig::numPixels;
    hedNumPixels = GeometryConfig::numPixels;
    InitAnalysisManager();
    InitVectorsForNtuples();
    InitNtuples();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis::~Analysis()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::BeginOfRun(const G4Run *run)
{
    if(!anaManager->GetFileName().empty())
    {
        isActive = true;
        anaManager->OpenFile();
    }
    else
    {
        isActive = false;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::EndOfRun(const G4Run *run)
{
    if(isActive)
    {
        anaManager->Write();
        anaManager->CloseFile();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::BeginOfEvent(const G4Event *event)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::EndOfEvent(const G4Event *event)
{
    if(isActive)
    {
        auto hce = event->GetHCofThisEvent();
        ClearVectorsForNtuples();
        
        // LED
        // processing hits collections
        auto ledHC = static_cast<const ScintillatorHC*>(hce->GetHC(sdManager->GetCollectionID("ledScintHC")));        
        std::vector<G4double> eDepSums(ledNumPixels, 0.);
        std::vector<G4int> primaryPassed(ledNumPixels, 0), nParts(ledNumPixels, 0);
        for(size_t i = 0;i < ledHC->GetSize();i++)
        {
            auto hit = (*ledHC)[i];
            auto idx = hit->GetPixelIndex();
            eDepSums.at(idx) += hit->GetEnergyDeposit()/keV;
            nParts.at(idx)++;
            if(hit->GetIsPrimary())
                primaryPassed.at(idx) = 1;
            tupleVectorPtrs.ledEnergies.at(idx)->push_back(hit->GetEntEnergy()/keV);    
            tupleVectorPtrs.ledEdeps.at(idx)->push_back(hit->GetEnergyDeposit()/keV);
            tupleVectorPtrs.ledPartEncoding.at(idx)->push_back(hit->GetEntPartPDGEncoding());
        }

        // filling ntuples
        for(G4int pixelIdx = 0;pixelIdx < ledNumPixels;pixelIdx++)
        {
            anaManager->FillNtupleDColumn(pixelIdx, 0, eDepSums.at(pixelIdx));
            anaManager->FillNtupleIColumn(pixelIdx, 1, primaryPassed.at(pixelIdx));
            anaManager->FillNtupleIColumn(pixelIdx, 2, nParts.at(pixelIdx));
            anaManager->AddNtupleRow(pixelIdx);
        }
        
        // Filter
        // processing hits collections
        auto filterHC = static_cast<const ScintillatorHC*>(hce->GetHC(sdManager->GetCollectionID("filterScintHC")));
        eDepSums.assign(1, 0.);
        primaryPassed.assign(1, 0);
        nParts.assign(1, 0);
        for(size_t i = 0;i < filterHC->GetSize();i++)
        {
            auto hit = (*filterHC)[i];
            auto idx = hit->GetPixelIndex();
            eDepSums.at(idx) += hit->GetEnergyDeposit()/keV;
            nParts.at(idx)++;
            if(hit->GetIsPrimary())
                primaryPassed.at(idx) = 1;
            tupleVectorPtrs.filterEnergies.at(idx)->push_back(hit->GetEntEnergy()/keV);    
            tupleVectorPtrs.filterEdeps.at(idx)->push_back(hit->GetEnergyDeposit()/keV);
            tupleVectorPtrs.filterPartEncoding.at(idx)->push_back(hit->GetEntPartPDGEncoding());
        }

        // filling ntuples
        anaManager->FillNtupleDColumn(ledNumPixels, 0, eDepSums.front());
        anaManager->FillNtupleIColumn(ledNumPixels, 1, primaryPassed.front());
        anaManager->FillNtupleIColumn(ledNumPixels, 2, nParts.front());
        anaManager->AddNtupleRow(ledNumPixels);
        
        // HED
        // processing hits collections
        auto hedHC = static_cast<const ScintillatorHC*>(hce->GetHC(sdManager->GetCollectionID("hedScintHC")));        
        eDepSums.assign(hedNumPixels, 0.);
        primaryPassed.assign(hedNumPixels, 0);
        nParts.assign(hedNumPixels, 0);
        for(size_t i = 0;i < hedHC->GetSize();i++)
        {
            auto hit = (*hedHC)[i];
            auto idx = hit->GetPixelIndex();
            eDepSums.at(idx) += hit->GetEnergyDeposit()/keV;
            nParts.at(idx)++;
            if(hit->GetIsPrimary())
                primaryPassed.at(idx) = 1;
            tupleVectorPtrs.hedEnergies.at(idx)->push_back(hit->GetEntEnergy()/keV);    
            tupleVectorPtrs.hedEdeps.at(idx)->push_back(hit->GetEnergyDeposit()/keV);
            tupleVectorPtrs.hedPartEncoding.at(idx)->push_back(hit->GetEntPartPDGEncoding());
        }

        // filling ntuples
        for(G4int pixelIdx = 0;pixelIdx < hedNumPixels;pixelIdx++)
        {
            anaManager->FillNtupleDColumn(ledNumPixels + 1 + pixelIdx, 0, eDepSums.at(pixelIdx));
            anaManager->FillNtupleIColumn(ledNumPixels + 1 + pixelIdx, 1, primaryPassed.at(pixelIdx));
            anaManager->FillNtupleIColumn(ledNumPixels + 1 + pixelIdx, 2, nParts.at(pixelIdx));
            anaManager->AddNtupleRow(ledNumPixels + 1 + pixelIdx);
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::ClearVectorsForNtuples()
{
    for(auto &vectorPtr : tupleVectorPtrs.ledEnergies)
        vectorPtr->clear();
    for(auto &vectorPtr : tupleVectorPtrs.filterEnergies)
        vectorPtr->clear();
    for(auto &vectorPtr : tupleVectorPtrs.hedEnergies)
        vectorPtr->clear();
    
    for(auto &vectorPtr : tupleVectorPtrs.ledEdeps)
        vectorPtr->clear();
    for(auto &vectorPtr : tupleVectorPtrs.filterEdeps)
        vectorPtr->clear();
    for(auto &vectorPtr : tupleVectorPtrs.hedEdeps)
        vectorPtr->clear();

    for(auto &vectorPtr : tupleVectorPtrs.ledPartEncoding)
        vectorPtr->clear();
    for(auto &vectorPtr : tupleVectorPtrs.filterPartEncoding)
        vectorPtr->clear();
    for(auto &vectorPtr : tupleVectorPtrs.hedPartEncoding)
        vectorPtr->clear();     
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::InitAnalysisManager()
{
    anaManager = G4RootAnalysisManager::Instance();
    anaManager->SetVerboseLevel(1);
    anaManager->SetNtupleMerging(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::InitVectorsForNtuples()
{
    for(G4int i = 0;i < ledNumPixels;i++)
    {
        tupleVectorPtrs.ledEnergies.emplace_back(std::make_unique<std::vector<G4double> > ());
        tupleVectorPtrs.ledEdeps.emplace_back(std::make_unique<std::vector<G4double> > ());
        tupleVectorPtrs.ledPartEncoding.emplace_back(std::make_unique<std::vector<G4int> > ());
    }
    
    tupleVectorPtrs.filterEnergies.emplace_back(std::make_unique<std::vector<G4double> > ());
    tupleVectorPtrs.filterEdeps.emplace_back(std::make_unique<std::vector<G4double> > ());
    tupleVectorPtrs.filterPartEncoding.emplace_back(std::make_unique<std::vector<G4int> > ());
    
    for(G4int i = 0;i < hedNumPixels;i++)
    {
        tupleVectorPtrs.hedEnergies.emplace_back(std::make_unique<std::vector<G4double> > ());
        tupleVectorPtrs.hedEdeps.emplace_back(std::make_unique<std::vector<G4double> > ());
        tupleVectorPtrs.hedPartEncoding.emplace_back(std::make_unique<std::vector<G4int> > ());   
    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Analysis::InitNtuples()
{
    char buffer[256] = {};
    // LED
    for(G4int idx = 0;idx < ledNumPixels;idx++)
    {
        snprintf(buffer, 256, "led%03d", idx);
        anaManager->CreateNtuple(buffer, "The scintillator pixel array for low energy discrimination");
        anaManager->CreateNtupleDColumn("eDepSum");
        anaManager->CreateNtupleIColumn("prmary");
        anaManager->CreateNtupleIColumn("nParts");
        anaManager->CreateNtupleDColumn("eParts", *tupleVectorPtrs.ledEnergies.at(idx).get());
        anaManager->CreateNtupleDColumn("eDeps", *tupleVectorPtrs.ledEdeps.at(idx).get());
        anaManager->CreateNtupleIColumn("partEncs", *tupleVectorPtrs.ledPartEncoding.at(idx).get());
    }
    anaManager->FinishNtuple();

    // Filter
    anaManager->CreateNtuple("filter", "Filter");
    anaManager->CreateNtupleDColumn("eDepSum");
    anaManager->CreateNtupleIColumn("prmary");
    anaManager->CreateNtupleIColumn("nParts");
    anaManager->CreateNtupleDColumn("eParts", *tupleVectorPtrs.filterEnergies.front().get());
    anaManager->CreateNtupleDColumn("eDeps", *tupleVectorPtrs.filterEdeps.front().get());
    anaManager->CreateNtupleIColumn("partEncs", *tupleVectorPtrs.filterPartEncoding.front().get());
    anaManager->FinishNtuple();

    // HED
    for(G4int idx = 0;idx < hedNumPixels;idx++)
    {
        snprintf(buffer, 256, "hed%03d", idx);
        anaManager->CreateNtuple(buffer, "The scintillator pixel array for high energy discrimination");
        anaManager->CreateNtupleDColumn("eDepSum");
        anaManager->CreateNtupleIColumn("prmary");
        anaManager->CreateNtupleIColumn("nParts");
        anaManager->CreateNtupleDColumn("eParts", *tupleVectorPtrs.hedEnergies.at(idx).get());
        anaManager->CreateNtupleDColumn("eDeps", *tupleVectorPtrs.hedEdeps.at(idx).get());
        anaManager->CreateNtupleIColumn("partEncs", *tupleVectorPtrs.hedPartEncoding.at(idx).get());
    }
    anaManager->FinishNtuple();
}
