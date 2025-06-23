/// \file B1/include/ActionInitialization.hh
/// \brief Definition of the B1::ActionInitialization class

#ifndef B1ActionInitialization_h
#define B1ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization() = default;
    ~ActionInitialization() override = default;

    virtual void BuildForMaster() const override;
    virtual void Build() const override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
