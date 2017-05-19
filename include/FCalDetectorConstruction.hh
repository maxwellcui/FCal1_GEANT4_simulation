//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//This is the header file of detctor constuction        |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//Author: Zhaoyuan Cui                                  |
//        2016 Summer                                   |
//        Physics department, The University of Arizona |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|

#ifndef FCalDetectorConstruction_hh
#define FCalDetectorConstruction_hh 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RotationMatrix.hh"

#include <vector>

class G4VisAttributes;
class G4LogicalVolume;

class FCalDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  FCalDetectorConstruction();
  virtual ~FCalDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructMaterials();
  virtual void ConstructSDandField();
  
  //Region4
  virtual void ConstructRegions4EandM();
  virtual void ConstructRegions4FandN();
  virtual void ConstructRegions4GandO();
  virtual void ConstructRegions4HandP();
  virtual void ConstructRegions4IandA();
  virtual void ConstructRegions4JandB();
  virtual void ConstructRegions4KandC();
  virtual void ConstructRegions4LandD();
  
  //Region3
  virtual void ConstructRegions3EandM();
  virtual void ConstructRegions3FandN();
  virtual void ConstructRegions3GandO();
  virtual void ConstructRegions3HandP();
  virtual void ConstructRegions3IandA();
  virtual void ConstructRegions3JandB();
  virtual void ConstructRegions3KandC();
  virtual void ConstructRegions3LandD();
 
  //Region2
  virtual void ConstructRegions2EandM();
  virtual void ConstructRegions2FandN();
  virtual void ConstructRegions2GandO();
  virtual void ConstructRegions2HandP();
  virtual void ConstructRegions2IandA();
  virtual void ConstructRegions2JandB();
  virtual void ConstructRegions2KandC();
  virtual void ConstructRegions2LandD();
 
  //Region1
  virtual void ConstructRegions1EandM();
  virtual void ConstructRegions1FandN();
  virtual void ConstructRegions1GandO();
  virtual void ConstructRegions1HandP();
  virtual void ConstructRegions1IandA();
  virtual void ConstructRegions1JandB();
  virtual void ConstructRegions1KandC();
  virtual void ConstructRegions1LandD();
  

private:
  G4LogicalVolume *fMatrix;
  G4LogicalVolume *fGap;
  G4LogicalVolume *fRod;
  G4LogicalVolume *fHex;
  G4LogicalVolume *fCal;
  G4bool checkOverlaps;
  G4RotationMatrix *hexRot;

  G4double height;
  G4double xStd;

  std::vector<G4VisAttributes*> fVisAttributes;
};

#endif
