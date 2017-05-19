//This is the source code of run action of FCal
//Author: Zhaoyuan Cui

#include "FCalRunAction.hh"
#include "FCalAnalysis.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

FCalRunAction::FCalRunAction()
  :G4UserRunAction()
{
  G4AnalysisManager* analysisManager=G4AnalysisManager::Instance();
  G4cout<<"Using "<<analysisManager->GetType()<<G4endl;

  //Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("1TeV_simulation");

  //Book histogram, ntuple
  //

  //Creating 2D histogram
  analysisManager
    ->CreateH2("FCal_sig_XY","FCal signal X vs Y",
	       450,-450,450,450,-450,450,"mm","mm");
  analysisManager
    ->CreateH2("FCal_sig_YZ","FCal signal Y vs Z",
	       450,0,450,900,-450,450,"mm","mm");
  analysisManager
    ->CreateH1("showerDepth","Shower depth variation",
	       450,0.,450.,"mm");
  analysisManager
    ->CreateH1("timing","Timing",
	       1000,30.,50.,"ns");

}

FCalRunAction::~FCalRunAction()
{
  delete G4AnalysisManager::Instance();
}

void FCalRunAction::BeginOfRunAction(const G4Run*)
{
  //Get analysis manager
  G4AnalysisManager* analysisManager=G4AnalysisManager::Instance();

  //Output file
  analysisManager->OpenFile();
}

void FCalRunAction::EndOfRunAction(const G4Run*)
{
  //Write and close output file
  //save histogram & ntuple
  //
  G4AnalysisManager* analysisManager=G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
