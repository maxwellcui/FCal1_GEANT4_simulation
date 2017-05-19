//This is the source code of Primary Generator Action class
//Author: Zhaoyuan Cui

#include"FCalPrimaryGeneratorAction.hh"
#include"G4Event.hh"
#include"G4ParticleGun.hh"
#include"G4ParticleTable.hh"
#include"G4ParticleDefinition.hh"
#include"globals.hh"
#include"G4ThreeVector.hh"

FCalPrimaryGeneratorAction::FCalPrimaryGeneratorAction(const G4String & particleName,
					       G4double energy,
					       G4ThreeVector position,
					       G4ThreeVector momentumDirection)
  : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0)
{
  G4int nofParticles=1;
  fParticleGun=new G4ParticleGun(nofParticles);

  //Find particle from particle table
  G4ParticleTable*particleTable=G4ParticleTable::GetParticleTable();
  G4ParticleDefinition*particle=particleTable->FindParticle(particleName);

  //Initialize particle parameters
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(energy);
  fParticleGun->SetParticlePosition(position);
  fParticleGun->SetParticleMomentumDirection(momentumDirection);
}

FCalPrimaryGeneratorAction::~FCalPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void FCalPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
