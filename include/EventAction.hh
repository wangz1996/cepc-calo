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
/// \file DBDecay/include/EventAction.hh
/// \brief Definition of the EventAction class
//
// $Id: EventAction.hh 68017 2013-03-13 13:29:53Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4EventManager.hh"
#include "globals.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "DetectorConstruction.hh"
#include "Config.hh"
#include "TMath.h"
#include "TRandom.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
    public:
        EventAction(HistoManager*,Config *c);
        ~EventAction();

    public:
        virtual void BeginOfEventAction(const G4Event*);
        virtual void   EndOfEventAction(const G4Event*);
        G4int fStepTag;

        void SetPrintModulo(G4int val)   {fPrintModulo = val;};
        void AddDecayChain(G4String val) {fDecayChain += val;};
		void AddEcalHit(const G4int &copyNo,const G4double &edep,const G4double &time,const G4int &pdgid,const G4int &trackid);
		void AddHcalHit(const G4int &copyNo,const G4double &edep,const G4double &time,const G4int &pdgid,const G4int &trackid);

        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        //void AddCrystalEnDep(G4int copyNo,G4double edep){
        //    for(size_t i_copyNo=0; i_copyNo!=(fHistoManager_Event->fParticleInfo.fCrystalID.size()); ++i_copyNo){
        //        if(fHistoManager_Event->fParticleInfo.fCrystalID.at(i_copyNo)==copyNo){
        //            fHistoManager_Event->fParticleInfo.fEnergyDep.at(i_copyNo) =  fHistoManager_Event->fParticleInfo.fEnergyDep.at(i_copyNo) + edep;
        //            return;
        //        }
        //    }
        //    fHistoManager_Event->fParticleInfo.fCrystalID.push_back(copyNo);
        //    fHistoManager_Event->fParticleInfo.fEnergyDep.push_back(edep);
        //}

        //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    private:
		Double_t SiPMDigi(const Double_t &edep) const;
        G4double        fEventEdep;
        G4int           fPrintModulo;
        G4String        fDecayChain;                   
        HistoManager* fHistoManager_Event;
		Config			*config;
        G4GeneralParticleSource * fGParticleSource;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


