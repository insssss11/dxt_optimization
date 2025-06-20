/// \file B1/src/Exception.cc
/// \brief Implementation of the B1::Exception class

#include "Exception.hh"
#include "G4Exception.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Exception::Exception(const char *originOfException, const char *exceptionCode, G4ExceptionSeverity severity, G4ExceptionDescription &description)
: originOfException(originOfException), exceptionCode(exceptionCode), severity(severity), description(description)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Exception::Execute() const
{
    G4Exception(originOfException.c_str(), exceptionCode.c_str(), severity, description);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
