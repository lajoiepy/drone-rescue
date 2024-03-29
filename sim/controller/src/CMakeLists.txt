#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_BUILD_TYPE Debug)

#
# Options
#
option(ARGOS_SIMULATION "Build for ARGoS Simulation" ON)

#
# Find pkg-config
#
find_package(PkgConfig REQUIRED)

if(ARGOS_SIMULATION)
#
# Look for the optional ARGoS package
#
pkg_check_modules(ARGOS argos3_simulator)
if(ARGOS_FOUND)
  include_directories(${ARGOS_INCLUDE_DIRS})
  link_directories(${ARGOS_LIBRARY_DIRS})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ARGOS_PREFIX}/share/argos3/cmake)
  include(ARGoSCheckQTOpenGL)
  # Look for Lua
  find_package(Lua REQUIRED)
  include_directories(${LUA_INCLUDE_DIR})
  link_directories(${ARGOS_PREFIX}/lib/argos3/)
endif(ARGOS_FOUND)

set(ARGOS_BUZZ_SOURCES
  utils/buzz_utils.h  
  argos/buzz_controller_drone_rescue_sim.h       
  argos/buzz_controller_drone_rescue_sim.cpp     
  argos/buzz_closures_drone_rescue_sim.cpp 
  argos/target/buzz_controller_target_sim.h      
  argos/target/buzz_controller_target_sim.cpp     
  argos/target/buzz_closures_target_sim.cpp
  argos/map/map_loop_functions.h      
  argos/map/map_loop_functions.cpp
  argos/map/belief_map.h      
  argos/map/belief_map.cpp  )

add_library(argos3plugin_buzz_simulator_drone_rescue SHARED ${ARGOS_BUZZ_SOURCES})
target_link_libraries(argos3plugin_buzz_simulator_drone_rescue
  argos3core_simulator
  argos3plugin_simulator_genericrobot
  argos3plugin_simulator_footbot
  argos3plugin_simulator_spiri
  argos3plugin_simulator_buzz
  buzz
  )

add_compile_options ( -g -rdynamic )
set_target_properties(argos3plugin_buzz_simulator_drone_rescue PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS argos3plugin_buzz_simulator_drone_rescue LIBRARY DESTINATION ${ARGOS_PREFIX}/lib/argos3)
install(DIRECTORY . DESTINATION include/buzz/argos FILES_MATCHING PATTERN "*.h")

endif(ARGOS_SIMULATION)
