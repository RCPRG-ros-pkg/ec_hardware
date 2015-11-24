cmake_minimum_required(VERSION 2.8.3)
project(ec_hardware)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

### ROS Dependencies ###
# Find the RTT-ROS package (this transitively includes the Orocos CMake macros)
find_package(catkin REQUIRED COMPONENTS
  rtt_ros
  # ADDITIONAL ROS PACKAGES
  )

find_package(yaml-cpp REQUIRED)
find_package(etherlab REQUIRED)
include_directories(include ${catkin_INCLUDE_DIRS})

### Orocos Dependencies ###
# Note that orocos_use_package() does not need to be called for any dependency
# listed in the package.xml file

include_directories(${USE_OROCOS_INCLUDE_DIRS})

include_directories(${ETHERLAB_INCLUDE_DIR})

### Orocos Targets ###

orocos_component(ec_hardware src/ECHardware.cpp src/ECDomain.cpp src/ECConfig.cpp src/ECSlave.cpp)
if(${OROCOS_TARGET} STREQUAL "xenomai" )
  target_link_libraries(ec_hardware ${catkin_LIBRARIES} ${YAMLCPP_LIBRARY} ${USE_OROCOS_LIBRARIES} ${ETHERCAT_LIBRARY_RTDM})
else()
  target_link_libraries(ec_hardware ${catkin_LIBRARIES} ${YAMLCPP_LIBRARY} ${USE_OROCOS_LIBRARIES} ${ETHERCAT_LIBRARY})
endif()
### Orocos Package Exports and Install Targets ###

# Generate install targets for header files

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

if(${OROCOS_TARGET} STREQUAL "xenomai" )
  catkin_package(
    INCLUDE_DIRS ${ETHERLAB_INCLUDE_DIR} include
    LIBRARIES ${ETHERCAT_LIBRARY_RTDM})
else()
  catkin_package(
    INCLUDE_DIRS ${ETHERLAB_INCLUDE_DIR} include
    LIBRARIES ${ETHERCAT_LIBRARY})
endif()

# Export package information (replaces catkin_package() macro) 
orocos_generate_package(
  INCLUDE_DIRS include
  DEPENDS rtt_ros yaml-cpp
)
