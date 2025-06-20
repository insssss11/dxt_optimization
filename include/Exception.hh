/// \file B1/include/Exception.hh
/// \brief Definition of the B1::Exception class

#ifndef B1Exception_h
#define B1Exception_h 1

#include <exception>

#include "G4Exception.hh"
#include "G4ExceptionSeverity.hh"
#include "G4String.hh"

class G4Exception;

/// Custom exception class that interfaces G4Exception class
class Exception : public std::exception
{
public:
    Exception(const char *originOfException, const char *exceptionCode, G4ExceptionSeverity severity, G4ExceptionDescription &description);
    virtual ~Exception() override = default;

    void Execute() const; // Call G4Exception

private:
    G4String originOfException, exceptionCode;
    G4ExceptionSeverity severity;
    G4ExceptionDescription &description;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif