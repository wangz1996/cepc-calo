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
/// \file radioactivedecay/rdecay01/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
// $Id: DetectorConstruction.cc 78307 2013-12-11 10:55:57Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "SteppingAction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4GlobalMagFieldMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructHCAL()
{
	std::cout<<"Starting to construct HCAL"<<std::endl;
	G4double ecal_length = 300.*mm;
	if(!config->conf["ECAL"]["build"].as<bool>())ecal_length = 0.*mm;
	std::cout<<"Building AHCAL at "<<ecal_length<<" mm"<<std::endl;
  G4Material* iron =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  G4Material* PSD =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
	PSD->GetIonisation()->SetBirksConstant(0.07943*mm/MeV); 
	//std::cout<<"GetNuclearInterLength "<<PSD->GetNuclearInterLength()<<std::endl;
  G4Material* kevlar =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
  G4double absorberZ0 = 2.*mm;
  G4double crystalXY = 4.*cm;
  G4double crystalZ = 3.*mm;
  G4double gap_psd_abs0 = 0.1 *mm;
  G4double crystalPositionZ = (ecal_length + absorberZ0 + gap_psd_abs0 + crystalZ/2.)*mm;
  G4double PCBXY = 72.*cm;
  G4double PCBZ = 2.*mm;
  G4double absorberXY = PCBXY;
  G4double absorberZ = 20.*mm;
  G4double absorberPositionZ0=(ecal_length + absorberZ0/2.)*mm;
  G4double absorberPositionZ=(ecal_length + absorberZ0 + crystalZ+PCBZ+absorberZ/2.)*mm;
  G4double absorberGapZ = (absorberZ+crystalZ+PCBZ)*mm;
  G4double crystalGapZ = absorberGapZ;
  G4double PCBGapZ = absorberGapZ;
  G4double PCBPositionZ = (ecal_length + absorberZ0 +  crystalZ+PCBZ/2.)*mm;
  G4int nLayer = 40;
	bool checkOverlap = false;
 // Absorber layer
  G4Box*
    solidAbsorber = new G4Box("hcal_absorber",                                                  // its name
			     0.5*absorberXY, 0.5*absorberXY, 0.5*absorberZ);        // its size
  G4Box*
    solidAbsorber0 = new G4Box("hcal_absorber0",                                                  // its name
                0.5*absorberXY, 0.5*absorberXY, 0.5*absorberZ0); 
  G4LogicalVolume* 
    logicAbsorber = new G4LogicalVolume(solidAbsorber,                                       // its solid
					iron,                                           // its material
                    "hcal_absorber");                                          // its name 
  G4LogicalVolume*
    logicAbsorber0 = new G4LogicalVolume(solidAbsorber0,                                       // its solid
                iron,                                             // its material
                "hcal_absorber0"); 
  G4VPhysicalVolume* physiAbsorber;
  physiAbsorber=new G4PVPlacement(0,
            	G4ThreeVector(0,0,absorberPositionZ0),
            	logicAbsorber0,
            	"hcal_absorber0",
            	logicWorld,
            	false,
            	-1,
            	checkOverlap);
  for(G4int i_Layer=0; i_Layer<nLayer; ++i_Layer){
    physiAbsorber = new G4PVPlacement(0,                                                    // no rotation
				      G4ThreeVector(0,0, (absorberPositionZ+i_Layer*absorberGapZ)),                              // at (0,0,0)
				      logicAbsorber,                                         // its logical volume
				      "hcal_absorber",                                            // its name
				      logicWorld,                                           // its mother  volume
				      false,                                                // no boolean operation
				      -1,
                      checkOverlap);                                                   // copy number
  }
	
  // active layer
  G4Box*  
    solidCrystal = new G4Box("hcal_psd",                                                  // its name
			     0.5*crystalXY, 0.5*crystalXY, 0.5*crystalZ);        // its size
  
  
  G4LogicalVolume* 
    logicCrystal = new G4LogicalVolume(solidCrystal,                                       // its solid
				       //ScinGlass4,                                             // its material
				       PSD,                                             // its material
				       "hcal_psd");                                          // its name 
  
  G4VPhysicalVolume* physiCrystal;
  for(G4int i_Layer=0; i_Layer<nLayer; ++i_Layer){
    for (int i_X = 0; i_X < 18; ++i_X)
    {
      for (int i_Y = 0; i_Y < 18; ++i_Y)
      {
                      physiCrystal = new G4PVPlacement(0,                                                    // no rotation
                      G4ThreeVector( (0. - PCBXY/2. + (i_X+0.5)*crystalXY) , (0. - PCBXY/2. + (i_Y+0.5)*crystalXY) , (crystalPositionZ+i_Layer*crystalGapZ)),                                      // at ()
                      logicCrystal,                                         // its logical volume
                      "hcal_psd",                                            // its name
                      logicWorld,                                           // its mother  volume
                      false,                                                // no boolean operation
                      i_Layer*100000+i_X*100+i_Y,
                      checkOverlap);                                                   // copy number
      }
    }

  }
  G4Box*
    solidPCB = new G4Box("hcal_pcb",                                                  // its name
			 0.5*PCBXY, 0.5*PCBXY, 0.5*PCBZ);        // its size
  
  G4LogicalVolume* 
    logicPCB = new G4LogicalVolume(solidPCB,                                       // its solid
				   kevlar,                                             // its material//should be check.............
				   "hcal_pcb");                                          // its name 
  
  G4VPhysicalVolume* physiPCB;
  for(G4int i_Layer=0; i_Layer<nLayer; ++i_Layer){
    physiPCB = new G4PVPlacement(0,                                                    // no rotation
				 G4ThreeVector(0,0, (PCBPositionZ+i_Layer*PCBGapZ)),                                      // at (0,0,0)
				 logicPCB,                                         // its logical volume
				 "hcal_pcb",                                            // its name
				 logicWorld,                                           // its mother  volume
				 false,                                                // no boolean operation
				 -1,
                 checkOverlap);                                                   // copy number
  }
}

