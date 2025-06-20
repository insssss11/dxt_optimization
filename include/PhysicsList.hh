/// \file B1/include/PhysicsList.hh
/// \brief Definition of the B1::PhysicsList class

#ifndef B1PhysicsList_h
#define B1PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4EmLivermorePhysics

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();
    virtual ~PhysicsList() override = default;

    virtual void SetCuts() override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
