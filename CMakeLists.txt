#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 3.16...3.27)
project(DXT_OPTIMIZATION)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# See the documentation for a guide on how to enable/disable specific components
#
find_package(Geant4 REQUIRED ui_all vis_all)
find_package(ROOT REQUIRED Core RIO Hist Tree)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, use our local headers, and link it to the Geant4 libraries
#
add_executable(optimize_dxt optimize_dxt.cc ${sources} ${headers})
add_executable(plot_hardening plot_hardening.cc ${sources} ${headers})
add_executable(print_filter_fom print_filter_fom.cc ${sources} ${headers})

target_include_directories(optimize_dxt PRIVATE include ${ROOT_INCLUDE_DIRS})
target_include_directories(plot_hardening PRIVATE include ${ROOT_INCLUDE_DIRS})
target_include_directories(print_filter_fom PRIVATE include ${ROOT_INCLUDE_DIRS})

target_link_libraries(optimize_dxt PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(plot_hardening PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(print_filter_fom PRIVATE ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build DXT_OPTIMIZATION. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(DXT_OPTIMIZATION_SCRIPTS
  README

  init_vis.mac
  vis.mac
  
  gmacros/demo/hardening.mac
  gmacros/optimize/filter_thickness.mac
  gmacros/optimize/filter_thickness_singlerun.mac
  
  data/tube_spectrum/tungsten_12deg_0.8mmAl_080kVp.csv
  data/tube_spectrum/tungsten_12deg_0.8mmAl_100kVp.csv
  data/tube_spectrum/tungsten_12deg_0.8mmAl_120kVp.csv
  data/tube_spectrum/tungsten_12deg_0.8mmAl_140kVp.csv
  data/tube_spectrum/tungsten_12deg_0.8mmAl_160kVp.csv
  data/tube_spectrum/tungsten_12deg_0.8mmAl_180kVp.csv
  data/tube_spectrum/tungsten_12deg_0.8mmAl_200kVp.csv
  )

foreach(_script ${DXT_OPTIMIZATION_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
