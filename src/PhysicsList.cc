/// \file B3/B3b/src/PhysicsList.cc
/// \brief Implementation of the B1::PhysicsList class

#include "PhysicsList.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmParameters.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    // Register the Livermore EM physics constructor
    RegisterPhysics(new G4EmLivermorePhysics());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
    SetCutValue(1*m, "e-");
    G4EmParameters::Instance()->SetApplyCuts(true);
    DumpCutValuesTable();
}