/// \file B1/src/EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include <iomanip>
#include <sstream>

#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

#include "EventAction.hh"
#include "RunAction.hh"
#include "ScintillatorHit.hh"
#include "ScintillatorSD.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction *runAction) : fRunAction(runAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event *)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event *event)
{
    fRunAction->GetAnalysisPtr()->EndOfEvent(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
