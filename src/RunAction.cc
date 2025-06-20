/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "G4Run.hh"

#include "RunAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() : G4UserRunAction(), anaManager(G4RootAnalysisManager::Instance()), analysis(new Analysis)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
    delete analysis;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run *run)
{
    analysis->BeginOfRun(run);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run *run)
{
    analysis->EndOfRun(run);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Analysis *RunAction::GetAnalysisPtr()
{
    return analysis;
}
