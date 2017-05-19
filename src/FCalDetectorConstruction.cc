//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//This is the source code of the detctor constuction                                  |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//Author: Zhaoyuan Cui                                                                |
//        2016 Summer                                                                 |
//        Physics department, The University of Arizona                               |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//This code generates the geometry of FCal1, including liguid Argon gaps and rods.    |
//Arrangement of the rods is accomplished by following the actual region arrangement. |
//Sensetive detector should be provided by the user for his or her own purpose.       |
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|

//particle distance on z: 4750mm

#include "FCalDetectorConstruction.hh"
#include "FCalEmCalorimeterSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"

#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

FCalDetectorConstruction::FCalDetectorConstruction()
  : G4VUserDetectorConstruction(),
    fMatrix(0),
    fGap(0),
    fRod(0),
    fHex(0),
    fCal(0),
    checkOverlaps(false),
    hexRot(0),
    height(7.5*sqrt(3)/2),
    xStd(76.875)
{}

FCalDetectorConstruction::~FCalDetectorConstruction()
{
  for(G4int i=0; i<G4int(fVisAttributes.size());i++)
    {
      delete fVisAttributes[i];
    }
}

void FCalDetectorConstruction::ConstructSDandField()
{
  G4SDManager *SDman=G4SDManager::GetSDMpointer();
  G4String SDname;

  G4VSensitiveDetector* emCal
    =new FCalEmCalorimeterSD(SDname="/EMCalorimeter",
  			     "CalorimeterHitsCollection");
  SDman->AddNewDetector(emCal);
  fCal->SetSensitiveDetector(emCal);
}

void FCalDetectorConstruction::ConstructMaterials()
{
  //Get and/or construct all materials
  G4NistManager *nistManager=G4NistManager::Instance();

  //Get Cu and vacuum
  nistManager->FindOrBuildMaterial("G4_Cu");
  nistManager->FindOrBuildMaterial("G4_Galactic");
  nistManager->FindOrBuildMaterial("G4_Fe");

  //Construct liquid Argon
  G4double a,z,density;
  z=18.;
  a=39.95*g/mole;
  density=1.390*g/cm3;
  G4Material *mat_lAr=new G4Material("LiquidArgon",z,a,density);

  //Print defined materials
  G4cout<<G4endl<<"The materials defined are: "<<G4endl<<G4endl;
  G4cout<<*(G4Material::GetMaterialTable())<<G4endl;
}

G4VPhysicalVolume *FCalDetectorConstruction::Construct()
{
  //All materials
  ConstructMaterials();
  G4Material *vacuum=G4Material::GetMaterial("G4_Galactic");
  G4Material *Cu=G4Material::GetMaterial("G4_Cu");
  G4Material *lAr=G4Material::GetMaterial("LiquidArgon");
  G4Material *Fe=G4Material::GetMaterial("G4_Fe");

  //Space parameter
  G4double env_size=10*m;
  G4double world_size=1.2*env_size;

  //Experiment Hall
  G4VSolid *solidWorld
    =new G4Box("worldSolid",world_size,world_size,world_size);
  G4LogicalVolume *logicalWorld
    =new G4LogicalVolume(solidWorld,vacuum,"logicalWorld");
  G4VPhysicalVolume *physicalWorld
    =new G4PVPlacement(0,G4ThreeVector(),logicalWorld,"world",0,false,0,checkOverlaps);

  //Envelop
  G4VSolid *solidEnv
    =new G4Box("envSolid",env_size,env_size,env_size);
  G4LogicalVolume *logicalEnv
    =new G4LogicalVolume(solidEnv,vacuum,"logicalEnv");
  new G4PVPlacement(0,G4ThreeVector(),logicalEnv,"Envelop",logicalWorld,false,0,checkOverlaps);

  //Detector

  //Matrix
  G4Tubs *solidMatrix
    =new G4Tubs("solidMatrix",72.3*mm,451.*mm,225.*mm,0.,360.*deg);
  fMatrix
    =new G4LogicalVolume(solidMatrix,Cu,"logicalMatrix");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,225*mm),fMatrix,"Matrix",logicalEnv,false,0,checkOverlaps);

  //Hex
  hexRot=new G4RotationMatrix();
  hexRot->rotateZ(90.*deg);
  G4double hex_z[4]={0.,0.,450.*mm,450.*mm};
  G4double hex_outer[4]={0.*cm,3.75*mm,3.75*mm,0.};
  G4double hex_inner[4]={0.,0.,0.,0.};
  G4Polyhedra *solidHex
    =new G4Polyhedra("solidHex",0.,360.*deg,6,4,hex_z,hex_inner,hex_outer);
  fHex
    =new G4LogicalVolume(solidHex,Cu,"logicalHex");

  //Flag_Region_Construction
  // //ConsturctRegion 4  
  // ConstructRegions4EandM();
  // ConstructRegions4FandN();
  // ConstructRegions4GandO();
  // ConstructRegions4HandP();
  // ConstructRegions4IandA();
  // ConstructRegions4JandB();
  // ConstructRegions4KandC();
  // ConstructRegions4LandD();
  
  // //ConstructRegion 3
  // ConstructRegions3EandM();
  // ConstructRegions3FandN();
  // ConstructRegions3GandO();
  // ConstructRegions3HandP();
  // ConstructRegions3IandA();  
  // ConstructRegions3JandB();
  // ConstructRegions3KandC();
  // ConstructRegions3LandD();

  // //ConstructRegion 2
  // ConstructRegions2EandM();
  // ConstructRegions2FandN();
  // ConstructRegions2GandO();
  // ConstructRegions2HandP();
  // ConstructRegions2IandA();
  // ConstructRegions2JandB();
  // ConstructRegions2KandC();
  // ConstructRegions2LandD();

  // //ConstructRegion 1
  // ConstructRegions1EandM();
  // ConstructRegions1FandN();
  // ConstructRegions1GandO();
  // ConstructRegions1HandP();
  // ConstructRegions1IandA();
  // ConstructRegions1JandB();
  // ConstructRegions1KandC();
  // ConstructRegions1LandD();

  //lAr gap
  G4Tubs *solidGap
    =new G4Tubs("solidGap",0.,2.626*mm,225*mm,0.,360.*deg);
  fGap
    =new G4LogicalVolume(solidGap,lAr,"logicalGap");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,225*mm),fGap,"lAr Gap",fHex,false,0,checkOverlaps);
  
  //calorimeter
  G4Tubs *solidCal
    =new G4Tubs("solidCal",2.356,2.626*mm,0.5*mm,0.,360.*deg);
  fCal
    =new G4LogicalVolume(solidCal,lAr,"logicalCal");
  for(G4int i=-225;i<225;i++)
    {
      new G4PVPlacement(0,G4ThreeVector(0.,0.,i*1.*mm+0.5*mm),fCal,"Calorimeter section",fGap,false,0,checkOverlaps);
    }
  

  //Rod
  G4Tubs *solidRod
    =new G4Tubs("solidRod",0.,2.356*mm,225*mm,0.,360.*deg);
  fRod
    =new G4LogicalVolume(solidRod,Cu,"logicalRod");
  new G4PVPlacement(0,G4ThreeVector(0,0.,0.),fRod,"Rod",fGap,false,0,checkOverlaps);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Disks
  /*Parameter*/
  G4double radLength=8.785*mm;//This is half of the radiation length of iron

  //Disk solid
  G4Tubs *solidDisk
    =new G4Tubs("solidDisk",72.3*mm,451*mm,radLength,0.,360.*deg);

  //Disk Logic
  G4LogicalVolume* logicalDisk
    =new G4LogicalVolume(solidDisk,Fe,"logicalDisk");

  //Physical placement
  //
  //First disk
  new G4PVPlacement(0,G4ThreeVector(0.,0.,-radLength),logicalDisk,"Disk 1",logicalEnv,false,0,checkOverlaps);

  //Second disk
  new G4PVPlacement(0,G4ThreeVector(0.,0.,-50*cm),logicalDisk,"Disk 2",logicalEnv,false,0,checkOverlaps); 

  //Third disk
  new G4PVPlacement(0,G4ThreeVector(0.,0.,-100*cm),logicalDisk,"Disk 3",logicalEnv,false,0,checkOverlaps);

  //
  //visualization attributes
  G4VisAttributes *visAttributes=new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  logicalWorld->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  logicalEnv->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  visAttributes = new G4VisAttributes(G4Colour(1.0,0.6,0.0));
  fMatrix->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  visAttributes = new G4VisAttributes(G4Colour(1.0,0.6,0.0));
  fHex->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9,0.0,0.0));
  fRod->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
  
  visAttributes = new G4VisAttributes(G4Colour(0.0,0.0,0.9));
  fGap->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

    
  return physicalWorld;
}

//Region 4E and 4M
void FCalDetectorConstruction::ConstructRegions4EandM()
{
  //4E
  for(G4int i=0;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     12.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4E Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     13.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4E Region",fMatrix,false,0,checkOverlaps);
    }
 for(G4int i=0;i<6;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     14.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4E Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     15.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4E Region",fMatrix,false,0,checkOverlaps);
    }
 for(G4int j=0;j<4;j++)
   {
     for(G4int i=0;i<8;i++)
       {
	 new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						16.5*height*mm+2*j*height,
						-225.*mm),
			   fHex,"Hexagon 4E Region",fMatrix,false,0,checkOverlaps);
	 new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						17.5*height*mm+j*2*height,
						-225.*mm),
			   fHex,"Hexagon 4E Region",fMatrix,false,0,checkOverlaps);	  
       }
   }   
 //4M
 for(G4int i=0;i<4;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					    -12.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4M Region",fMatrix,false,0,checkOverlaps);
     new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					    -13.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4M Region",fMatrix,false,0,checkOverlaps);
   }
 for(G4int i=0;i<6;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					    -14.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4M Region",fMatrix,false,0,checkOverlaps);
     new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					    -15.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4M Region",fMatrix,false,0,checkOverlaps);
   }
 for(G4int j=0;j<4;j++)
   {
     for(G4int i=0;i<8;i++)
       {
 	 new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
 						-16.5*height*mm-2*j*height,
 						-225.*mm),
 			   fHex,"Hexagon 4M Region",fMatrix,false,0,checkOverlaps);
 	 new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
 						-17.5*height*mm-j*2*height,
 						-225.*mm),
 			   fHex,"Hexagon 4M Region",fMatrix,false,0,checkOverlaps);	  
       }
   }    
}

//Region 4F and 4N
void FCalDetectorConstruction::ConstructRegions4FandN()
{
  //4F
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm-3*7.5)*mm,
					     8.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm-3*7.5+3.75)*mm,
					     9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm-4*7.5+3.75)*mm,
					     9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
      for(G4int i=-5;i<0;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 10.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 11.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	}
      for(G4int i=-8;i<-5;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 11.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	}
      for(G4int i=-6;i<2;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 12.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 13.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	}

      for(G4int i=-4;i<2;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 14.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 15.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	}
      for(G4int j=0;j<4;j++)
	{
	  for(G4int i=-2;i<6;i++)
	    {
	      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						     16.5*height*mm+2*j*height,
						     -225.*mm),
				fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);
	      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						     17.5*height*mm+j*2*height,
						     -225.*mm),
				fHex,"Hexagon 4F Region",fMatrix,false,0,checkOverlaps);	  
	    }
	}
      //4N
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm-3*7.5)*mm,
					     -8.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm-3*7.5+3.75)*mm,
					     -9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm-4*7.5+3.75)*mm,
					     -9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
      for(G4int i=-5;i<0;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -10.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -11.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	}
      for(G4int i=-8;i<-5;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -11.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	}
      for(G4int i=-6;i<2;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -12.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -13.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	}

      for(G4int i=-4;i<2;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -14.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -15.5*height*mm,
						 -225.*mm),
			    fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	}
      for(G4int j=0;j<4;j++)
	{
	  for(G4int i=-2;i<6;i++)
	    {
	      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						     -16.5*height*mm-2*j*height,
						     -225.*mm),
				fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);
	      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						     -17.5*height*mm-j*2*height,
						     -225.*mm),
				fHex,"Hexagon 4N Region",fMatrix,false,0,checkOverlaps);	  
	    }
	}     
}
//Region 4G and 4O
void FCalDetectorConstruction::ConstructRegions4GandO()
{
  //4G
  for(G4int i=-1;i<6;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     6.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     7.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm-3.75)*mm,
					     5.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm-7.5-3.75)*mm,
					 7.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=-2;i<10;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     8.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=0;i<10;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     10.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
    }
 for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<10;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 12.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 13.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 16.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 17.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 //4O
 for(G4int i=-1;i<6;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					    -6.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					    -7.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
   }
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm-3.75)*mm,
					-5.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm-7.5-3.75)*mm,
					-7.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
 for(G4int i=-2;i<10;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					    -8.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					    -9.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
   }
 for(G4int i=0;i<10;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					    -10.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					    -11.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
   }
 for(G4int j=0;j<2;j++)
   {
     for(G4int i=2;i<10;i++)
       {
	 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						-12.5*height*mm-2*j*height,
						-225.*mm),
			   fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
	 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						-13.5*height*mm-j*2*height,
						-225.*mm),
			   fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);	  
       }
   }
 for(G4int j=0;j<2;j++)
   {
     for(G4int i=6;i<10;i++)
       {
	 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						-16.5*height*mm-2*j*height,
						-225.*mm),
			   fHex,"Hexagon 4O Region",fMatrix,false,0,checkOverlaps);
	 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						-17.5*height*mm-j*2*height,
						-225.*mm),
			   fHex,"Hexagon 4G Region",fMatrix,false,0,checkOverlaps);	  
       }
   }
}

//Region 4H and 4P
void FCalDetectorConstruction::ConstructRegions4HandP()
{ 
  //4H
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 0.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 1.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=0;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     4.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     5.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=6;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     6.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);     
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     7.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 8.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 9.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4H Region",fMatrix,false,0,checkOverlaps);
	}
    }
  //4P
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -0.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -1.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=0;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -4.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     -5.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=6;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -6.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);     
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     -7.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -8.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -9.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4P Region",fMatrix,false,0,checkOverlaps);
	}
    }
}

//Region 4I and 4A
void FCalDetectorConstruction::ConstructRegions4IandA()
{
  //4I
  for(G4int j=0;j<3;j++)
    {
      for(G4int i=0;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -5.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -4.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=4;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -7.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4I Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -6.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4I Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -11.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4I Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -10.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //4A
  for(G4int j=0;j<3;j++)
    {
      for(G4int i=0;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 5.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4A Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 4.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=4;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     7.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4A Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     6.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4A Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 11.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4A Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 10.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }

}

//Region 4J and 4B
void FCalDetectorConstruction::ConstructRegions4JandB()
{
  //4J
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm-3.75)*mm,
					 -4.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm-7.5)*mm,
					 -5.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=-2;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -7.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -6.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=-3;i<10;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -8.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=0;i<10;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -10.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
    }

  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -15.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -14.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -19.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -18.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //4B
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm-3.75)*mm,
					 4.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm-7.5)*mm,
					 5.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=-2;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     7.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     6.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=-3;i<10;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     8.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=0;i<10;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     10.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
    }

  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 15.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 14.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 19.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 18.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
}

//Region 4K and 4C
void FCalDetectorConstruction::ConstructRegions4KandC()
{
  //4K
  for(G4int i=-5;i<0;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd+i*7.5)*mm,
					     -11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd+i*7.5+3.75)*mm,
					     -10.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);  
    }
  for(G4int i=0;i<2;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(24.375*mm+i*7.5,
					     -11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=-6;i<2;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd+i*7.5)*mm,
					     -13.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd+i*7.5+3.75)*mm,
					     -12.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);  
    }
  for(G4int i=-4;i<2;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd+i*7.5)*mm,
					     -15.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd+i*7.5+3.75)*mm,
					     -14.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);  
    }
  
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=-2;i<6;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -23.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -22.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=-2;i<2;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -27.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -26.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //4C
    for(G4int i=-5;i<0;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd+i*7.5)*mm,
					     11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd+i*7.5+3.75)*mm,
					     10.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);  
    }
  for(G4int i=0;i<2;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-24.375*mm-i*7.5,
					     11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=-6;i<2;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd+i*7.5)*mm,
					     13.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd+i*7.5+3.75)*mm,
					     12.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);  
    }
  for(G4int i=-4;i<2;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd+i*7.5)*mm,
					     15.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd+i*7.5+3.75)*mm,
					     14.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);  
    }
  
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=-2;i<6;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 23.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 22.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=-2;i<2;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 27.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 26.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 4C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
}

//Region 4L and 4D
void FCalDetectorConstruction::ConstructRegions4LandD()
{
  //4L
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -23.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -22.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4L Region",fMatrix,false,0,checkOverlaps); 
	}
    }

  for(G4int i=0;i<6;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     -15.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4L Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     -14.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4L Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=0;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     -13.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4L Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     -12.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4L Region",fMatrix,false,0,checkOverlaps); 
    }
  //4D
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 23.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 22.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 4D Region",fMatrix,false,0,checkOverlaps); 
	}
    }

  for(G4int i=0;i<6;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     15.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4D Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     14.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4D Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=0;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     13.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4D Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     12.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 4D Region",fMatrix,false,0,checkOverlaps); 
    }
}

//Region 3E and 3M
void FCalDetectorConstruction::ConstructRegions3EandM()
{
  //3E
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 24.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 25.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<6;j++)
    {
      for(G4int i=0;i<12;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 28.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 29.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 40.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 41.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<4;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 44.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 45.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3E Region",fMatrix,false,0,checkOverlaps);
	}
    }

  //3M
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -24.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -25.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<6;j++)
    {
      for(G4int i=0;i<12;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -28.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -29.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -40.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -41.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<4;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -44.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -45.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3M Region",fMatrix,false,0,checkOverlaps);
	}
    }
}

void FCalDetectorConstruction::ConstructRegions3FandN()
{
  //3F
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 20.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 21.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=-2;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 24.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 25.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=2;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 28.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 29.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 36.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 37.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  //3N
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -20.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -21.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=-2;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -24.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -25.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=2;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -28.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -29.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -36.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -37.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
}


void FCalDetectorConstruction::ConstructRegions3GandO()
{
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 12.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 13.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 16.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 17.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<26;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 20.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 21.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<26;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 24.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 25.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=18;i<22;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 28.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 29.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  //3O
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -12.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -13.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -16.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -17.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<26;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -20.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -21.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<26;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -24.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -25.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=18;i<22;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -28.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -29.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }  
}

void FCalDetectorConstruction::ConstructRegions3HandP()
{
  //3H
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<34;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 0.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 1.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<34;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 4.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 5.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }  
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 8.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 9.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 12.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 13.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  //3P
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<34;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -0.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -1.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<34;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -4.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -5.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }  
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -8.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -9.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -12.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -13.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 3P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
}

void FCalDetectorConstruction::ConstructRegions3IandA()
{
  //3I
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<34;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -3.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -2.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=14;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -11.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -10.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -15.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -14.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //3A
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<34;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 3.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3A Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 2.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=14;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 11.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3A Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 10.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 15.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3A Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 14.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
}

void FCalDetectorConstruction::ConstructRegions3JandB()
{
  //3J
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<18;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -15.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -14.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -19.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -18.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -23.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -22.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -27.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -26.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=18;i<22;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -35.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -34.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //3B
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<18;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 15.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 14.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 19.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 18.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 23.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 22.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 27.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 26.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=18;i<22;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 35.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 34.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3B Region",fMatrix,false,0,checkOverlaps); 
 	}
   }
}

void FCalDetectorConstruction::ConstructRegions3KandC()
{
  //3K
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -23.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -22.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -27.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -26.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=2;i<18;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -35.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -34.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -39.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -38.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -43.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -42.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //3C
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 23.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 22.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 27.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 26.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=2;i<18;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 35.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 34.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=2;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 39.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 38.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<10;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 43.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 42.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 3C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
}

void FCalDetectorConstruction::ConstructRegions3LandD()
{
  //3L
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -27.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -26.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<6;j++)
    {
      for(G4int i=0;i<12;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -39.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -38.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -43.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -42.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<4;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -47.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -46.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  //3D
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 27.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 26.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<6;j++)
    {
      for(G4int i=0;i<12;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 39.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 38.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 43.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 42.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<4;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 47.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 46.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 3D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
}

void FCalDetectorConstruction::ConstructRegions2EandM()
{
  //2E
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=4;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 44.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 45.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 48.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 49.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 52.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 53.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 56.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 57.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  //2M
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=4;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -44.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -45.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -48.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -49.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -52.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -53.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -56.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -57.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2M Region",fMatrix,false,0,checkOverlaps);
	}
    }
}

void FCalDetectorConstruction::ConstructRegions2FandN()
{
  //2F
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<22;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 36.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 37.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=6;i<26;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 40.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 41.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 48.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 49.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 52.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 53.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  //2N
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<22;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -36.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -37.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=6;i<26;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -40.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -41.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<18;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -48.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -49.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -52.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -53.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
}

void FCalDetectorConstruction::ConstructRegions2GandO()
{
  //2G
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=26;i<38;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 20.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 21.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<38;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 28.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 29.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<34;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 32.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 33.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 36.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 37.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 40.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 41.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  //2O
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=26;i<38;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -20.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -21.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<38;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -28.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -29.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<34;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -32.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -33.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -36.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -37.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -40.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -41.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
}

void FCalDetectorConstruction::ConstructRegions2HandP()
{
  //2H
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=34;i<46;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 0.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 1.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<6;j++)
    {
      for(G4int i=30;i<42;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 8.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 9.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=38;i<42;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 20.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 21.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  //2P
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=34;i<46;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -0.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -1.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<6;j++)
    {
      for(G4int i=30;i<42;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -8.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -9.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=38;i<42;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -20.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -21.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 2P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
}

void FCalDetectorConstruction::ConstructRegions2IandA()
{
  //2I
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=34;i<46;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -3.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -2.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<8;j++)
    {
      for(G4int i=30;i<42;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -19.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -18.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=38;i<42;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -23.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -22.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }  
  //2A
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=34;i<46;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 3.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2A Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 2.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<8;j++)
    {
      for(G4int i=30;i<42;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 19.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2A Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 18.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=38;i<42;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 23.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2A Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 22.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2A Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
}

void FCalDetectorConstruction::ConstructRegions2JandB()
{
  //2J
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=26;i<38;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -27.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -26.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<38;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -31.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -30.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<34;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -35.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -34.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -39.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -38.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -43.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -42.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //2B
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=26;i<38;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 27.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 26.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<38;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 31.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 30.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<34;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 35.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 34.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 39.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 38.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 43.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 42.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
}

void FCalDetectorConstruction::ConstructRegions2KandC()
{
  //2K
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<22;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -39.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -38.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -43.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -42.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -47.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -46.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<22;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -51.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -50.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -55.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -54.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  //2C
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<22;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 39.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 38.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 43.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 42.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<26;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 47.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 46.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=6;i<22;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 51.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 50.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<14;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 55.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 54.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 2C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
}

void FCalDetectorConstruction::ConstructRegions2LandD()
{
  //2L
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=4;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -47.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -46.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -51.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -50.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -55.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -54.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -59.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -58.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  //2D
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=4;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 47.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 46.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 51.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 50.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 55.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 54.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<16;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 59.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 58.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 2D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
}

void FCalDetectorConstruction::ConstructRegions1EandM()
{
  //1E
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=16;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 56.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 57.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<22;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 60.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 61.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 64.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 65.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int i=0;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     68.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=8;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=8;i<13;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector((1.875+8*7.5)*mm,
					 66.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=14;i<21;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=14;i<18;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=22;i<24;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     60.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     61.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
      
    }
  for(G4int i=22;i<25;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     62.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);      
    }
  new G4PVPlacement(hexRot,G4ThreeVector((1.875+22*7.5+3.75)*mm,
					 63.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1E Region",fMatrix,false,0,checkOverlaps);
  //1M
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=16;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -56.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -57.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<22;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -60.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -61.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<8;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 -64.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 -65.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
	}
    }
  for(G4int i=0;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     -68.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=8;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     -64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=8;i<13;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     -65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+8*7.5)*mm,
					 -66.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=14;i<21;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     -64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=14;i<18;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     -65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=22;i<24;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     -60.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     -61.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
      
    }
  for(G4int i=22;i<25;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     -62.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);      
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+22*7.5+3.75)*mm,
					 -63.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1M Region",fMatrix,false,0,checkOverlaps);
  
}

void FCalDetectorConstruction::ConstructRegions1FandN()
{
  //1F
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 44.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 45.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<29;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 48.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 49.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<23;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 52.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 53.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<19;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 56.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 57.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=14;i<19;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     60.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=14;i<16;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     61.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);	  
    }
  for(G4int i=19;i<22;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     56.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
      
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+19*7.5+3.75)*mm,
					 57.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+19*7.5)*mm,
					 58.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+23*7.5)*mm,
					 54.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=23;i<27;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     52.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     53.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);	  
    }
  for(G4int i=27;i<29;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     52.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=29;i<31;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     48.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+29*7.5+3.75)*mm,
					 49.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+29*7.5)*mm,
					 50.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1F Region",fMatrix,false,0,checkOverlaps);
  //1N
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -44.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -45.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=18;i<29;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -48.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -49.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=14;i<23;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -52.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -53.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<19;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -56.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -57.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=14;i<19;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -60.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=14;i<16;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     -61.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);	  
    }
  for(G4int i=19;i<22;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -56.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
      
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+19*7.5+3.75)*mm,
					 -57.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+19*7.5)*mm,
					 -58.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+23*7.5)*mm,
					 -54.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=23;i<27;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -52.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     -53.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);	  
    }
  for(G4int i=27;i<29;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -52.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=29;i<31;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -48.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+29*7.5+3.75)*mm,
					 -49.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+29*7.5)*mm,
					 -50.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1N Region",fMatrix,false,0,checkOverlaps);
}

void FCalDetectorConstruction::ConstructRegions1GandO()
{
  //1G
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=38;i<42;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 24.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 25.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=34;i<38;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 32.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 33.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=30;i<36;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 36.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 37.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=30;i<34;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     44.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     45.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=30;i<32;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     46.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     47.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=32;i<34;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     46.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+32*7.5+3.75)*mm,
					 47.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=34;i<36;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     44.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+36*7.5)*mm,
					 42.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+36*7.5)*mm,
					 40.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+36*7.5+3.75)*mm,
					 41.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+36*7.5)*mm,
					 38.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+36*7.5+3.75)*mm,
					 39.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+37*7.5)*mm,
					 38.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  
  for(G4int i=36;i<38;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     36.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     37.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+38*7.5)*mm,
					 36.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+38*7.5+3.75)*mm,
					 35.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=38;i<41;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     34.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=42;i<44;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 26.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 27.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=42;i<44;i++)
    { 
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     30.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+42*7.5+3.75)*mm,
					 31.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);	 
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+42*7.5)*mm,
					 32.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1G Region",fMatrix,false,0,checkOverlaps);
  //1O
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=38;i<42;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -24.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -25.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=34;i<38;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -32.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -33.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=30;i<36;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -36.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -37.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=30;i<34;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -44.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     -45.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=30;i<32;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -46.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     -47.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=32;i<34;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -46.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+32*7.5+3.75)*mm,
					 -47.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=34;i<36;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -44.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+36*7.5)*mm,
					 -42.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+36*7.5)*mm,
					 -40.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+36*7.5+3.75)*mm,
					 -41.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+36*7.5)*mm,
					 -38.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+36*7.5+3.75)*mm,
					 -39.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+37*7.5)*mm,
					 -38.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  
  for(G4int i=36;i<38;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -36.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     -37.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+38*7.5)*mm,
					 -36.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+38*7.5+3.75)*mm,
					 -35.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=38;i<41;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -34.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=42;i<44;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -26.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -27.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
  for(G4int i=42;i<44;i++)
    { 
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     -30.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+42*7.5+3.75)*mm,
					 -31.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);	 
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+42*7.5)*mm,
					 -32.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1O Region",fMatrix,false,0,checkOverlaps);
}

void FCalDetectorConstruction::ConstructRegions1HandP()
{
  //1H
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=46;i<48;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 0.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 1.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 for(G4int j=0;j<8;j++)
    {
      for(G4int i=42;i<46;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 8.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 9.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 for(G4int i=42;i<44;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					    24.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
     new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					    25.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);	  
   }
 for(G4int i=44;i<46;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					    24.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
   }
 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+44*7.5+3.75)*mm,
					25.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+44*7.5)*mm,
					26.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
 for(G4int j=0;j<2;j++)
   {
      for(G4int i=46;i<47;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 16.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 17.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 for(G4int j=0;j<2;j++)
   {
      for(G4int i=46;i<47;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						 10.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						 11.5*height*mm+j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+46*7.5)*mm,
					14.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+47*7.5)*mm,
					10.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
 for(G4int j=0;j<4;j++)
   {
     for(G4int i=48;i<49;i++)
       {
	 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
						0.5*height*mm+2*j*height,
						-225.*mm),
			   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
	 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
						1.5*height*mm+j*2*height,
						-225.*mm),
			   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);	  
       }
   }
 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+49*7.5)*mm,
					2.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps);
 new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+48*7.5)*mm,
					8.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1H Region",fMatrix,false,0,checkOverlaps); 
  //1P
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=46;i<48;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -0.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -1.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 for(G4int j=0;j<8;j++)
    {
      for(G4int i=42;i<46;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -8.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -9.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 for(G4int i=42;i<44;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					    -24.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					    -25.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);	  
   }
 for(G4int i=44;i<46;i++)
   {
     new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					    -24.5*height*mm,
					    -225.*mm),
		       fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
   }
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+44*7.5+3.75)*mm,
					-25.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+44*7.5)*mm,
					-26.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
 for(G4int j=0;j<2;j++)
   {
      for(G4int i=46;i<47;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -16.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -17.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 for(G4int j=0;j<2;j++)
   {
      for(G4int i=46;i<47;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						 -10.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						 -11.5*height*mm-j*2*height,
						 -225.*mm),
			    fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);	  
	}
    }
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+46*7.5)*mm,
					-14.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+47*7.5)*mm,
					-10.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
 for(G4int j=0;j<4;j++)
   {
     for(G4int i=48;i<49;i++)
       {
	 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						-0.5*height*mm-2*j*height,
						-225.*mm),
			   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
	 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						-1.5*height*mm-j*2*height,
						-225.*mm),
			   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);	  
       }
   }
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+49*7.5)*mm,
					-2.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
 new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+48*7.5)*mm,
					-8.5*height*mm,
					-225.*mm),
		   fHex,"Hexagon 1P Region",fMatrix,false,0,checkOverlaps);
}

void FCalDetectorConstruction::ConstructRegions1IandA()
{
  //1I
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=46;i<49;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -3.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -2.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=42;i<49;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -7.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -6.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=42;i<48;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -9.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -8.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=42;i<47;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -11.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -10.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=42;i<47;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -15.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -14.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<4;j++)
    {
      for(G4int i=42;i<46;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -23.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -22.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=42;i<44;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -25.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -24.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+49*7.5)*mm,
					 -1.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+47*7.5)*mm,
					 -11.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+46*7.5)*mm,
					 -17.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+46*7.5)*mm,
					 -19.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+46*7.5+3.75)*mm,
					 -18.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
    new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+46*7.5+3.75)*mm,
					 -20.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1I Region",fMatrix,false,0,checkOverlaps);
    //1A
    for(G4int j=0;j<2;j++)
      {
	for(G4int i=46;i<49;i++)
	  {
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						   3.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
	    
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						   2.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps); 
	  }
      }
    for(G4int j=0;j<2;j++)
      {
	for(G4int i=42;i<49;i++)
	  {
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						   7.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
	    
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						   6.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps); 
	  }
      }
    for(G4int i=42;i<48;i++)
      {
	new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					       9.5*height*mm,
					       -225.*mm),
			  fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
	
	new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					       8.5*height*mm,
					       -225.*mm),
			  fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps); 
      }
    for(G4int i=42;i<47;i++)
      {
	new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					       11.5*height*mm,
					       -225.*mm),
			  fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
	
	new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					       10.5*height*mm,
					       -225.*mm),
			  fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps); 
      }
    for(G4int j=0;j<2;j++)
      {
	for(G4int i=42;i<47;i++)
	  {
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						   15.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
	    
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						   14.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps); 
	  }
      }
    for(G4int j=0;j<4;j++)
      {
	for(G4int i=42;i<46;i++)
	  {
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
						   23.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
	    
	    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
						   22.5*height*mm-2*j*height,
						   -225.*mm),
			      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps); 
	  }
      }
    for(G4int i=42;i<44;i++)
      {
	new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					       25.5*height*mm,
					       -225.*mm),
			  fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
	
	new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					       24.5*height*mm,
					       -225.*mm),
			  fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps); 
      }
    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+49*7.5)*mm,
					   1.5*height*mm,
					   -225.*mm),
		      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+47*7.5)*mm,
					   11.5*height*mm,
					   -225.*mm),
		      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+46*7.5)*mm,
					   17.5*height*mm,
					   -225.*mm),
		      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+46*7.5)*mm,
					   19.5*height*mm,
					   -225.*mm),
		      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+46*7.5+3.75)*mm,
					   18.5*height*mm,
					   -225.*mm),
		      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
    new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+46*7.5+3.75)*mm,
					   20.5*height*mm,
					   -225.*mm),
		      fHex,"Hexagon 1A Region",fMatrix,false,0,checkOverlaps);
}  

void FCalDetectorConstruction::ConstructRegions1JandB()
{
  //1J
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=38;i<42;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -33.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -32.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=42;i<45;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -27.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -26.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=42;i<44;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -29.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -28.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+42*7.5)*mm,
					 -31.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
  
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+42*7.5+3.75)*mm,
					 -30.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
  for(G4int j=0;j<3;j++)
    {
      for(G4int i=34;i<38;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -37.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -36.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=38;i<40;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -35.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -34.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+38*7.5)*mm,
					 -37.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+38*7.5+3.75)*mm,
					 -36.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=30;i<34;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -45.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -44.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=34;i<37;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -41.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -40.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=34;i<36;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -43.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -42.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+34*7.5)*mm,
					 -45.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
  
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+34*7.5+3.75)*mm,
					 -44.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
  for(G4int i=30;i<33;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -47.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -46.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+33*7.5+3.75)*mm,
					 -46.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+31*7.5+3.75)*mm,
					 -48.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1J Region",fMatrix,false,0,checkOverlaps); 
  //1B
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=38;i<42;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 33.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 32.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=42;i<45;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     27.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     26.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=42;i<44;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     29.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     28.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+42*7.5)*mm,
					 31.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
  
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+42*7.5+3.75)*mm,
					 30.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
  for(G4int j=0;j<3;j++)
    {
      for(G4int i=34;i<38;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 37.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 36.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=38;i<40;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     35.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     34.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+38*7.5)*mm,
					 37.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+38*7.5+3.75)*mm,
					 36.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
  for(G4int j=0;j<5;j++)
    {
      for(G4int i=30;i<34;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 45.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 44.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=34;i<37;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 41.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
	  
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 40.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=34;i<36;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     43.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     42.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+34*7.5)*mm,
					 45.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
  
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+34*7.5+3.75)*mm,
					 44.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
  for(G4int i=30;i<33;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     47.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
      
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     46.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+33*7.5+3.75)*mm,
					 46.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps); 
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+31*7.5+3.75)*mm,
					 48.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1B Region",fMatrix,false,0,checkOverlaps);
}
void FCalDetectorConstruction::ConstructRegions1KandC()
{
  //1K
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -47.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -46.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -51.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -50.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+30*7.5)*mm,
					 -49.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+30*7.5+3.75)*mm,
					 -48.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
  for(G4int i=14;i<28;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -53.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -52.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=14;i<23;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -55.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -54.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<19;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
 						 -59.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
 						 -58.5*height*mm+2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=19;i<21;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -56.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+19*7.5)*mm,
					 -57.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=11;i<18;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -60.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=12;i<16;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5)*mm,
					     -61.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=12;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((xStd*mm+i*7.5+3.75)*mm,
					     -62.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1K Region",fMatrix,false,0,checkOverlaps); 
    }
  //1C
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=26;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 47.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 46.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=22;i<30;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 51.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 50.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+30*7.5)*mm,
					 49.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+30*7.5+3.75)*mm,
					 48.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
  for(G4int i=14;i<28;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     53.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     52.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=14;i<23;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     55.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     54.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=10;i<19;i++)
 	{
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
 						 59.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
 	  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
 						 58.5*height*mm-2*j*height,
 						 -225.*mm),
 			    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
 	}
    }
  for(G4int i=19;i<21;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     56.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+19*7.5)*mm,
					 57.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
  for(G4int i=11;i<18;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     60.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=12;i<16;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5)*mm,
					     61.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=12;i<14;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(xStd*mm+i*7.5+3.75)*mm,
					     62.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1C Region",fMatrix,false,0,checkOverlaps); 
    }
}

void FCalDetectorConstruction::ConstructRegions1LandD()
{
  //1L
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=16;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -59.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -58.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<21;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
						 -63.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
						 -62.5*height*mm+2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  new G4PVPlacement(hexRot,G4ThreeVector((1.875+21*7.5)*mm,
					 -61.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector((1.875+21*7.5+3.75)*mm,
					 -62.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
  for(G4int i=21;i<23;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     -63.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=0;i<13;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     -65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					 -64.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=13;i<15;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     -64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=15;i<18;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     -65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					 -64.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=18;i<20;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     -64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=0;i<8;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5)*mm,
					     -67.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					 -66.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=0;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector((1.875+i*7.5+3.75)*mm,
					     -68.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1L Region",fMatrix,false,0,checkOverlaps); 
    }
  //1D
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=16;i<20;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 59.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 58.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  for(G4int j=0;j<2;j++)
    {
      for(G4int i=0;i<21;i++)
	{
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
						 63.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps);
	  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
						 62.5*height*mm-2*j*height,
						 -225.*mm),
			    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
	}
    }
  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+21*7.5)*mm,
					 61.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps);
  new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+21*7.5+3.75)*mm,
					 62.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
  for(G4int i=21;i<23;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     63.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps);
    }
  for(G4int i=0;i<13;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					 64.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=13;i<15;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=15;i<18;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     65.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					 64.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=18;i<20;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     64.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=0;i<8;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5)*mm,
					     67.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps);
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					 66.5*height*mm,
					 -225.*mm),
		    fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
    }
  for(G4int i=0;i<4;i++)
    {
      new G4PVPlacement(hexRot,G4ThreeVector(-(1.875+i*7.5+3.75)*mm,
					     68.5*height*mm,
					     -225.*mm),
			fHex,"Hexagon 1D Region",fMatrix,false,0,checkOverlaps); 
    }
}

//Bot
