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
//
// $Id: HistoManager.cc 72249 2013-07-12 08:57:49Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include <TTree.h>
#include <TFile.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager(const char *foutname,const bool &savegeo)
  :fRootFile(0),fNtuple(0),fSaveGeo(savegeo)
{
	fOutName = foutname;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{
  G4cout<<"------>create rootfile"<<G4endl;
  fRootFile = new TFile(fOutName.c_str(),"RECREATE");
  fNtuple = new TTree("T","calo events");
  //fNtuple->Branch("ecal_psdid",              &fParticleInfo.fecal_psdid);
  //fNtuple->Branch("ecal_energy",              &fParticleInfo.fecal_energy);
  //fNtuple->Branch("ecal_pdgid",                  &fParticleInfo.fecal_pdgid);
  //fNtuple->Branch("ecal_trackid",                &fParticleInfo.fecal_trackid);
  //fNtuple->Branch("ecal_x",              &fParticleInfo.fecal_x);
  //fNtuple->Branch("ecal_y",              &fParticleInfo.fecal_y);
  //fNtuple->Branch("ecal_z",              &fParticleInfo.fecal_z);
  //fNtuple->Branch("ecal_time",               &fParticleInfo.fecal_time);
  fNtuple->Branch("ecal_cellid",               &fParticleInfo.fecal_cellid);
  fNtuple->Branch("ecal_celle",               &fParticleInfo.fecal_celle);
  fNtuple->Branch("ecal_cellx",               &fParticleInfo.fecal_cellx);
  fNtuple->Branch("ecal_celly",               &fParticleInfo.fecal_celly);
  fNtuple->Branch("ecal_cellz",               &fParticleInfo.fecal_cellz);

  //fNtuple->Branch("hcal_psdid",              &fParticleInfo.fhcal_psdid);
  //fNtuple->Branch("hcal_energy",              &fParticleInfo.fhcal_energy);
  //fNtuple->Branch("hcal_pdgid",                  &fParticleInfo.fhcal_pdgid);
  //fNtuple->Branch("hcal_trackid",                &fParticleInfo.fhcal_trackid);
  //fNtuple->Branch("hcal_x",              &fParticleInfo.fhcal_x);
  //fNtuple->Branch("hcal_y",              &fParticleInfo.fhcal_y);
  //fNtuple->Branch("hcal_z",              &fParticleInfo.fhcal_z);
  //fNtuple->Branch("hcal_time",               &fParticleInfo.fhcal_time);
  fNtuple->Branch("CellID",               &fParticleInfo.fhcal_cellid);
  fNtuple->Branch("Hit_Energy",               &fParticleInfo.fhcal_celle);
  fNtuple->Branch("Hit_X",               &fParticleInfo.fhcal_cellx);
  fNtuple->Branch("Hit_Y",               &fParticleInfo.fhcal_celly);
  fNtuple->Branch("Hit_Z",               &fParticleInfo.fhcal_cellz);
  //fNtuple->Branch("hcal_cellid",               &fParticleInfo.fhcal_cellid);
  //fNtuple->Branch("hcal_celle",               &fParticleInfo.fhcal_celle);
  //fNtuple->Branch("hcal_cellx",               &fParticleInfo.fhcal_cellx);
  //fNtuple->Branch("hcal_celly",               &fParticleInfo.fhcal_celly);
  //fNtuple->Branch("hcal_cellz",               &fParticleInfo.fhcal_cellz);
}

void HistoManager::save()
{
	if(fSaveGeo)
	{
		gSystem->Load("libGeom");
		TGeoManager::Import("cepc-calo.gdml");
		gGeoManager->Write("cepc_calo");
		std::remove("cepc-calo.gdml");
	}
	fNtuple->Write("",TObject::kOverwrite);
	fRootFile->Close();
	G4cout<<"------>close rootfile"<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
