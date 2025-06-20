/// \file B1/include/Analysis.hh
/// \brief Definition of the B1::Analysis class

#ifndef B1Analysis_h
#define B1Analysis_h 1

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RootAnalysisManager.hh"

#include "ScintillatorSD.hh"
#include "ScintillatorHit.hh"


class Analysis
{
public:
    Analysis();
    ~Analysis();

    void BeginOfEvent(const G4Event *event);
    void EndOfEvent(const G4Event *event);

    void BeginOfRun(const G4Run *run);
    void EndOfRun(const G4Run *run);

    // stores energies and energy deposits of photons entering into sensitive detectors
    struct TupleVectorPtrs{
        std::vector<std::unique_ptr<std::vector<G4double> > > ledEnergies;
        std::vector<std::unique_ptr<std::vector<G4double> > > filterEnergies;
        std::vector<std::unique_ptr<std::vector<G4double> > > hedEnergies;
        std::vector<std::unique_ptr<std::vector<G4double> > > ledEdeps;
        std::vector<std::unique_ptr<std::vector<G4double> > > filterEdeps;
        std::vector<std::unique_ptr<std::vector<G4double> > > hedEdeps;
        std::vector<std::unique_ptr<std::vector<G4int> > > ledPartEncoding;
        std::vector<std::unique_ptr<std::vector<G4int> > > filterPartEncoding;
        std::vector<std::unique_ptr<std::vector<G4int> > > hedPartEncoding;
    };
    
private:
    TupleVectorPtrs tupleVectorPtrs;
    G4bool isActive;
    G4int ledNumPixels, hedNumPixels;
    ScintillatorHC *ledHC, *filterHC, *hedHC;

    G4RootAnalysisManager *anaManager;
    G4SDManager *sdManager;

private:
    void InitAnalysisManager();
    void InitVectorsForNtuples();
    void ClearVectorsForNtuples();
    void InitNtuples();
};

#endif
