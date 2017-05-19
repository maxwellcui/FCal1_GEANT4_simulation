//This is the header file for Primary Generator Action class

#ifndef FCalPrimaryGeneratorAction_h
#define FCalPrimaryGeneratorAction_h 1

#include"G4VUserPrimaryGeneratorAction.hh"
#include"globals.hh"
#include"G4ThreeVector.hh"
#include"G4SystemOfUnits.hh"

class G4ParticleGun;
class G4Event;

class FCalPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  FCalPrimaryGeneratorAction(const G4String & particleName="e-",
			 G4double Energy=0.*TeV,
			 G4ThreeVector position=G4ThreeVector(0.,20*cm,-4750*mm),
			 G4ThreeVector momentumDirection=G4ThreeVector(0.,0.,1.));
  ~FCalPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

private:
  G4ParticleGun* fParticleGun;
};

#endif
