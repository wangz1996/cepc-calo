#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(calo VERSION 1.0.0 LANGUAGES C CXX )

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
find_package(ROOT REQUIRED COMPONENTS TMVA ROOTVecOps ROOTDataFrame)
find_package( yaml-cpp REQUIRED)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${YAML_CPP_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
		    ${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(calo calo.cc ${sources} ${headers})
target_link_libraries(calo ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} yaml-cpp )

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build CEPCScECAL. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(calo_SCRIPTS
      vis.mac
#    Co60.mac  CEPCScECAL.in  vis.mac  fullChain.mac  pythia.mac  pythia_event.data
  )

foreach(_script ${CEPCScECAL_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS calo DESTINATION bin)

