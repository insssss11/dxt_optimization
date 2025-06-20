/// \file B1/include/RunAction.hh
/// \brief Definition of the B1::RunAction class

#ifndef B1RunAction_h
#define B1RunAction_h 1

#include <memory>

#include "G4UserRunAction.hh"
#include "G4RootAnalysisManager.hh"
#include "globals.hh"
#include "Analysis.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction() override;

    virtual void BeginOfRunAction(const G4Run *) override;
    virtual void EndOfRunAction(const G4Run *) override;

    Analysis *GetAnalysisPtr();

private:
    Analysis *analysis;
    G4RootAnalysisManager *anaManager;
};
#endif
