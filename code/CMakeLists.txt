cmake_minimum_required(VERSION 3.9)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
elseif(APPLE)
  message("hello apple")
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9 CACHE STRING "")
endif()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Games_Engineering)

#### setup Directories ####
#Main output directory
set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
# Ouput all DLLs from all libs into main build folder
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
# SFML - graphics library
add_subdirectory("lib/sfml" EXCLUDE_FROM_ALL)
#include_directories("lib/sfml/include")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

# B2D - Box2D phyics library
add_subdirectory("lib/b2d/Box2D" EXCLUDE_FROM_ALL)
#include_directories("lib/b2d/Box2D/")
set(B2D_INCS "lib/b2d/Box2D/")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Add Engine and our own library projects####

file(GLOB_RECURSE RESOURCE_FILES "res/*.*")
####
file(GLOB_RECURSE SOURCE_FILES engine/src/*.cpp engine/src/*.h)
add_library(lib_engine STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_engine INTERFACE "${CMAKE_SOURCE_DIR}/engine/src/" PUBLIC SYSTEM ${B2D_INCS} )
target_link_libraries(lib_engine lib_tile_level_loader lib_maths lib_ecm Box2D sfml-graphics sfml-audio)

## Maths lib
add_library(lib_maths INTERFACE)
target_sources(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths/maths.h")
target_include_directories(lib_maths INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_maths" INTERFACE ${SFML_INCS})

## Tile loader lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_tile_level_loader/*.cpp engine/lib_tile_level_loader/*.h)
add_library(lib_tile_level_loader STATIC ${SOURCE_FILES} ${RESOURCE_FILES})
target_include_directories(lib_tile_level_loader INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_tile_level_loader/" )
target_link_libraries(lib_tile_level_loader lib_maths sfml-graphics)

## ECM lib
file(GLOB_RECURSE SOURCE_FILES engine/lib_ecm/*.cpp engine/lib_ecm/*.h)
add_library(lib_ecm STATIC ${SOURCE_FILES})
target_include_directories(lib_ecm INTERFACE "${CMAKE_SOURCE_DIR}/engine/lib_ecm" )
target_link_libraries(lib_ecm PRIVATE lib_maths)

#### Add Practical Projects####
#@@@-
## 1 -Pong
file(GLOB_RECURSE SOURCES practical_1_pong/stage_1.cpp)
add_executable(PRACTICAL_1_PONG ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_1_PONG SYSTEM PRIVATE ${SFML_INCS})
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_1_PONG)

file(GLOB_RECURSE SOURCES practical_1_pong/finished.cpp)
add_executable(PRACTICAL_1_PONG_F ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_1_PONG_F SYSTEM PRIVATE ${SFML_INCS})
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_1_PONG_F)

## 2 - Space invaders
file(GLOB_RECURSE SOURCES practical_2_space_invaders/*.cpp practical_2_space_invaders/*.h)
add_executable(PRACTICAL_2_INVADERS ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_2_INVADERS SYSTEM PRIVATE ${SFML_INCS})
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_2_INVADERS)

## 3 - Tiled level maze
file(GLOB_RECURSE SOURCES practical_3_tiled_levels/*.cpp practical_3_tiled_levels/*.h)
add_executable(PRACTICAL_3_TILED_LEVELS ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_3_TILED_LEVELS SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_3_TILED_LEVELS lib_tile_level_loader)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_3_TILED_LEVELS)

## 4 - ECM / pacman
file(GLOB_RECURSE SOURCES practical_4_ecm/*.cpp practical_4_ecm/*.h)
add_executable(PRACTICAL_4_ECM ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_4_ECM SYSTEM PRIVATE ${SFML_INCS})
target_link_libraries(PRACTICAL_4_ECM lib_tile_level_loader lib_ecm)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_4_ECM)

## 5 - Physics
file(GLOB_RECURSE SOURCES practical_5_physics/*.cpp practical_5_physics/*.h)
add_executable(PRACTICAL_5_PHYSICS ${SOURCES} ${RESOURCE_FILES})
target_include_directories(PRACTICAL_5_PHYSICS SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(PRACTICAL_5_PHYSICS Box2D)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_5_PHYSICS)

#@@@-

## 6 - Platformer
file(GLOB_RECURSE SOURCES practical_6_platformer/*.cpp practical_6_platformer/*.h)
file(GLOB_RECURSE CMPNTS practical_6_platformer/components/*.cpp practical_6_platformer/components/*.h)
file(GLOB_RECURSE SCENES practical_6_platformer/scenes/*.cpp practical_6_platformer/scenes/*.h)
add_executable(PRACTICAL_6_PLATFORMER ${SOURCES} ${RESOURCE_FILES})
source_group("components" FILES ${CMPNTS})
source_group("resources" FILES ${RESOURCE_FILES})
#target_include_directories(PRACTICAL_6_PLATFORMER SYSTEM PRIVATE ${SFML_INCS} ${B2D_INCS})
target_link_libraries(PRACTICAL_6_PLATFORMER lib_engine)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_6_PLATFORMER)

#@@@-

## 7 - AI Movement
file(GLOB_RECURSE SOURCES practical_7_steering/*.cpp practical_7_steering/*.h)
file(GLOB_RECURSE CMPNTS practical_7_steering/components/*.cpp practical_7_steering/components/*.h)
file(GLOB_RECURSE SCENES practical_7_steering/scenes/*.cpp practical_7_steering/scenes/*.h)
add_executable(PRACTICAL_7_STEERING ${SOURCES} ${CMPNTS} ${SCENES} ${RESOURCE_FILES})
target_link_libraries(PRACTICAL_7_STEERING lib_engine)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_7_STEERING)

## 8 - Decisions
file(GLOB_RECURSE SOURCES practical_8_decisions/*.cpp practical_8_decisions/*.h)
file(GLOB_RECURSE CMPNTS practical_8_decisions/components/*.cpp practical_8_decisions/components/*.h)
file(GLOB_RECURSE SCENES practical_8_decisions/scenes/*.cpp practical_8_decisions/scenes/*.h)
add_executable(PRACTICAL_8_DECISIONS ${SOURCES} ${CMPNTS} ${SCENES} ${RESOURCE_FILES})
target_link_libraries(PRACTICAL_8_DECISIONS lib_engine)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_8_DECISIONS)

#@@@-

## 9 - Deployment
file(GLOB_RECURSE SOURCES practical_9_deployment/*.cpp practical_9_deployment/*.h)
add_executable(PRACTICAL_9_DEPLOY ${SOURCES} ${RESOURCE_FILES})
target_link_libraries(PRACTICAL_9_DEPLOY lib_engine)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_9_DEPLOY)

## 11 - Networking
add_executable(PRACTICAL_11_NETWORKING_SERVER practical_11_networking/server.cpp)
target_link_libraries(PRACTICAL_11_NETWORKING_SERVER sfml-network)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_11_NETWORKING_SERVER)
add_executable(PRACTICAL_11_NETWORKING_CLIENT practical_11_networking/client.cpp)
target_link_libraries(PRACTICAL_11_NETWORKING_CLIENT sfml-network)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_11_NETWORKING_CLIENT)


#### Target for copying resources to build dir####
if(APPLE)
  add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/lib/sfml/extlibs/libs-osx/Frameworks" ${CMAKE_BINARY_DIR}/lib/sfml/Frameworks
  )
else()
  add_custom_target(copy_resources ALL 
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/res" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIGURATION>/res
  )
endif()

#### Link Dependencies ####
foreach (exe ${EXECUTABLES})
  #set working directory to build dir
  set_target_properties(${exe} PROPERTIES 
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)
  )

  target_link_libraries(${exe}
    lib_maths
    optimized sfml-audio debug sfml-audio-d
    optimized sfml-window debug sfml-window-d
    optimized sfml-system debug sfml-system-d
    optimized sfml-graphics debug sfml-graphics-d
    optimized sfml-main
  )
  
  if(MSVC) # Hide Console in release builds
    set_target_properties(${exe} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
  endif(MSVC)
  
  add_dependencies(${exe} sfml-graphics sfml-audio copy_resources)
endforeach ()


#Install directory within build folder, so it doesn't go into /lib or program-files
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install/")
#Installer.exe/zip built into /installer
SET(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/installer/")

#Install component for runtime dlls and stuff
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE) #stop this happening on non-release buildss
INCLUDE(InstallRequiredSystemLibraries)
install( PROGRAMS ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION "bin/" CONFIGURATIONS Release COMPONENT System )
#Install component for resources
install(DIRECTORY "${PROJECT_SOURCE_DIR}/res" DESTINATION "bin/" CONFIGURATIONS Release COMPONENT resources_cmpt)
#We disabled SFML's INSTALL component as it brings along junk, so we gotta make one ourselves
install(DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/" DESTINATION "bin" CONFIGURATIONS Release COMPONENT libs_cmpt FILES_MATCHING PATTERN "*.dll" )

#Tell CPack about the libs and resources install components
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} resources_cmpt libs_cmpt)
#CPACK_INSTALL_CMAKE_PROJECTS IS only necessary if not using CPACK_COMPONENTS_ALL
#set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};resources_cmpt;/)
#set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};libs_cmpt;/)

#Install component for all labs, also tell CPack about them
foreach (exe ${EXECUTABLES})
  install(TARGETS ${exe} DESTINATION "bin" CONFIGURATIONS Release COMPONENT ${exe}_cmpt )
  #set(CPACK_INSTALL_CMAKE_PROJECTS ${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${CMAKE_PROJECT_NAME};${exe}_cmpt;/)
  set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL} ${exe}_cmpt)
  set(CPACK_COMPONENT_${exe}_CMPT_DEPENDS resources_cmpt libs_cmpt)
  set(CPACK_COMPONENT_${exe}_CMPT_GROUP "Labs")
endforeach ()


IF(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_NSIS_COMPONENT_INSTALL ON)
  set(CPACK_NSIS_HELP_LINK "https://github.com/edinburgh-napier/set09121")
  set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/edinburgh-napier/set09121")
  set(CPACK_NSIS_CONTACT "https://github.com/edinburgh-napier")
ELSE()
  set(CPACK_GENERATOR ZIP)
ENDIF(WIN32)


set(CPACK_PACKAGE_FILE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "set09119")
set(CPACK_PACKAGE_VENDOR "Edinburgh Napier University")
find_file (LICENSE NAMES LICENSE LICENSE.txt PATHS  "${CMAKE_SOURCE_DIR}/../" "${CMAKE_SOURCE_DIR}" NO_DEFAULT_PATH)
if(${LICENSE})
  set(CPACK_RESOURCE_FILE_LICENSE ${LICENSE})
endif(${LICENSE})
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)

