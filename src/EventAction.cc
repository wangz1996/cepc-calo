//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \brief Implementation of the EventAction class
//
// $Id: EventAction.cc 68030 2015-03-13 13:51:27Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"
//#include "EventMessenger.hh"

#include "G4Event.hh"
#include <iomanip>
#include <HistoManager.hh>
#include <TTree.h>
#include "RunAction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(HistoManager* histo,Config *c)
:G4UserEventAction(),
 fEventEdep(0),fPrintModulo(10000),fDecayChain(),fHistoManager_Event(histo),config(c)
{
  fGParticleSource  = new G4GeneralParticleSource();
  //eventmanager->SetVerboseLevel(config->conf["Verbose"]["event"].as<int>());
  //fHistoManager_Event = new HistoManager();
  //fEventMessenger = new EventMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  delete fGParticleSource;
  //delete fHistoManager_Event;
  //delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
    fStepTag=0;
//  G4cout <<
//  		"..................6666666666666........................"
//  		<< G4endl;
  fDecayChain = " ";
//  fHistoManager_Event->fParticleInfo.reset();
// G4cout<<"begin of event"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
  //  G4cout<<" >>>>>>>>>>>>>>>>> "<< fHistoManager_Event ->fParticleInfo.fPrimaryEnergy <<" <<<<<<<<<<<<<<<"<<G4endl;
//  G4cout <<
//  		"..................777777777777777........................"
//  		<< G4endl;
 G4int evtNb = evt->GetEventID(); 
 //printing survey
 //
 if (evtNb%1000 == 0) 
   G4cout << "\n end of event " << std::setw(6) << evtNb 
          << " :" + fDecayChain << G4endl;
  for(auto i:fHistoManager_Event->fParticleInfo.fecal_mape)
  {
	//if(i.second<0.05)
	//{
	//	continue;
	//}
    fHistoManager_Event->fParticleInfo.fecal_cellid.emplace_back(i.first);
    fHistoManager_Event->fParticleInfo.fecal_celle.emplace_back(i.second);
	int layer=i.first/210;
	int m=(i.first%210)/42;
	int n=(i.first%210)%42;
	double x=0.;
	double y=0.;
	double z=0.;
	if(layer%2==0){
		x=-41.*5./2.+n*5.;
		y=-4.*45./2.+m*45.;
	}
	if(layer%2==1){
		x=-4.*45./2.+m*45.;
		y=-41.*5./2.+n*5.;
	}
	z=1.+layer*(2.+2.+2.8);
	fHistoManager_Event->fParticleInfo.fecal_cellx.emplace_back(x);
	fHistoManager_Event->fParticleInfo.fecal_celly.emplace_back(y);
	fHistoManager_Event->fParticleInfo.fecal_cellz.emplace_back(z);
  }
  for(auto i:fHistoManager_Event->fParticleInfo.fhcal_mape)
  {
	//if(i.second<0.05)
	//{
	//	continue;
	//}
    fHistoManager_Event->fParticleInfo.fhcal_cellid.emplace_back(i.first);
    fHistoManager_Event->fParticleInfo.fhcal_celle.emplace_back(i.second);
	double x = (i.first%10000)/100;
    double y = (i.first%100);
    double layer = i.first/10000;
	fHistoManager_Event->fParticleInfo.fhcal_cellx.emplace_back(-360. + (double(x)+0.5)*40.);
	fHistoManager_Event->fParticleInfo.fhcal_celly.emplace_back(-360. + (double(y)+0.5)*40.);
	fHistoManager_Event->fParticleInfo.fhcal_cellz.emplace_back(300. + 1.5+layer*25.);

  }
// G4cout<<"end of event "<<fHistoManager_Event->fParticleInfo.nTrack<<" "<<fHistoManager_Event->fParticleInfo.fTrackTime[0]<<G4endl;
 
 //
 
 //
 
 fHistoManager_Event->fNtuple->Fill();
}


void EventAction::AddEcalHit(const G4int &copyNo,const G4double &edep,const G4double &time,const G4int &pdgid,const G4int &trackid)
{
	fHistoManager_Event->fParticleInfo.fecal_psdid.emplace_back(copyNo);
	fHistoManager_Event->fParticleInfo.fecal_energy.emplace_back(edep);
	fHistoManager_Event->fParticleInfo.fecal_pdgid.emplace_back(pdgid);
	fHistoManager_Event->fParticleInfo.fecal_trackid.emplace_back(trackid);
	fHistoManager_Event->fParticleInfo.fecal_time.emplace_back(time);
/////////////////////////////////
	
	int layer=copyNo/210;
	//std::cout<<copyNo<<" "<<layer<<std::endl;
	int m=(copyNo%210)/42;
	int n=(copyNo%210)%42;
	double x=0.;
	double y=0.;
	double z=0.;
	if(layer%2==0){
		x=-41.*5./2.+n*5.;
		y=-4.*45./2.+m*45.;
	}
	if(layer%2==1){
		x=-4.*45./2.+m*45.;
		y=-41.*5./2.+n*5.;
	}
	z=1.+layer*(2.+2.+2.8);
	fHistoManager_Event->fParticleInfo.fecal_x.emplace_back(x);
	fHistoManager_Event->fParticleInfo.fecal_y.emplace_back(y);
	fHistoManager_Event->fParticleInfo.fecal_z.emplace_back(z);
	fHistoManager_Event->fParticleInfo.fecal_mape[copyNo]+=edep;
}
void EventAction::AddHcalHit(const G4int &copyNo,const G4double &edep,const G4double &time,const G4int &pdgid,const G4int &trackid)
{
	fHistoManager_Event->fParticleInfo.fhcal_psdid.emplace_back(copyNo);
	fHistoManager_Event->fParticleInfo.fhcal_energy.emplace_back(edep);
	fHistoManager_Event->fParticleInfo.fhcal_pdgid.emplace_back(pdgid);
	fHistoManager_Event->fParticleInfo.fhcal_trackid.emplace_back(trackid);
	fHistoManager_Event->fParticleInfo.fhcal_time.emplace_back(time);
	double x = (copyNo%10000)/100;
    double y = (copyNo%100);
    double layer = copyNo/10000;
      fHistoManager_Event->fParticleInfo.fhcal_x.push_back(-360. + (double(x)+0.5)*40.);
      fHistoManager_Event->fParticleInfo.fhcal_y.push_back(-360. + (double(y)+0.5)*40.);
      fHistoManager_Event->fParticleInfo.fhcal_z.push_back(300. + 1.5+layer*25.);
	fHistoManager_Event->fParticleInfo.fhcal_mape[copyNo]+=edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


