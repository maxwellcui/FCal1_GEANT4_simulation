//This is the source file of SD class of FCal
//Author: Zhaoyuan Cui (Maxwell)

#include "FCalEmCalorimeterHit.hh"
#include "FCalEmCalorimeterSD.hh"
#include "FCalAnalysis.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "driftSignal.hh"

FCalEmCalorimeterSD::FCalEmCalorimeterSD( const G4String& name,
					  const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

FCalEmCalorimeterSD::~FCalEmCalorimeterSD(){}

void FCalEmCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection
  fHitsCollection
    =new FCalEmCalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]);

  //Add this collection in hce
  G4int hcID
    =G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);
}

G4bool FCalEmCalorimeterSD::ProcessHits(G4Step* aStep,
					G4TouchableHistory*)
{
  //Energy deposit
  G4double edep=aStep->GetTotalEnergyDeposit();
  
  if(edep==0.)
    return false;

  FCalEmCalorimeterHit* newHit=new FCalEmCalorimeterHit();
  
  newHit->SetEdep(edep);
  newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());

  fHitsCollection->insert(newHit);

  //newHit->Print();

  return true;
}


void FCalEmCalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
  G4AnalysisManager* analysisManager=G4AnalysisManager::Instance();
  
  G4int nofHits=fHitsCollection->entries();
  G4double E_total;
  G4double E_pos;
  G4double E_ave;
  E_total=0;
  E_pos=0;
  E_ave=0;

  G4double reading[90000];

  for(G4int i=0;i<90000;i++)
    {
      reading[i]=0;
    }

  for(G4int i=0;i<nofHits;i++)
    {
      FCalEmCalorimeterHit *hit=(*fHitsCollection)[i];
      G4ThreeVector localPos=hit->GetPos();
      analysisManager->FillH2(0,localPos.x(),localPos.y());
      analysisManager->FillH2(1,localPos.z(),localPos.y());

      E_total+=hit->GetEdep();
      E_pos+=hit->GetEdep()*localPos.z();

      driftSignal *p=new driftSignal(hit->GetEdep(),localPos.z());
      p->driftFunction(reading);
      delete p;
    }
 
  E_ave=E_pos/E_total;

  analysisManager->FillH1(0,E_ave);
  
  G4double ave;
  G4double te;
  G4double criticalTime;
  
  ave=0;
  te=0;
  criticalTime=0;
  
  for(G4int i=0;i<90000;i++)
    {
      ave+=((G4double)i+0.5)*10*reading[i];
      te+=reading[i];
    }
  criticalTime=ave/te;
  criticalTime=criticalTime/1000;

  G4cout<<"The time is: "<<criticalTime<<G4endl;
  
  analysisManager->FillH1(1,criticalTime);

  G4cout<<G4endl
	<<"----------->Hits Collection: in this event there are "<<nofHits
	<<" hits in the lAr gap: "<<G4endl;
}
