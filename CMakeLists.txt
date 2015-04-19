cmake_minimum_required(VERSION 2.8.3)
project(ec_hardware)

### ROS Dependencies ###
# Find the RTT-ROS package (this transitively includes the Orocos CMake macros)
find_package(catkin REQUIRED COMPONENTS
  rtt_ros
  # ADDITIONAL ROS PACKAGES
  )

include_directories(${catkin_INCLUDE_DIRS})

### Orocos Dependencies ###
# Note that orocos_use_package() does not need to be called for any dependency
# listed in the package.xml file

include_directories(${USE_OROCOS_INCLUDE_DIRS})

include_directories(/opt/etherlab/include)

### Orocos Targets ###

orocos_component(ec_hardware src/ECHardware.cpp src/ECDomain.cpp src/ECConfig.cpp src/ECSlave.cpp)
target_link_libraries(ec_hardware ${catkin_LIBRARIES} ${USE_OROCOS_LIBRARIES} /opt/etherlab/lib/libethercat.so yaml-cpp)

# orocos_library(my_library src/my_library.cpp)
# target_link_libraries(my_library ${catkin_LIBRARIES} ${USE_OROCOS_LIBRARIES})

orocos_service(test_driver src/TestDriver.cpp)
target_link_libraries(test_driver ${catkin_LIBRARIES} ${USE_OROCOS_LIBRARIES})

# orocos_plugin(my_plugin src/my_plugin.cpp)
# target_link_libraries(my_plugin ${catkin_LIBRARIES} ${USE_OROCOS_LIBRARIES})

# orocos_typekit(my_typekit src/my_typekit.cpp)
# target_link_libraries(my_typekit ${catkin_LIBRARIES} ${USE_OROCOS_LIBRARIES})

### Orocos Package Exports and Install Targets ###

# Generate install targets for header files

orocos_install_headers(DIRECTORY include/${PROJECT_NAME})

# Export package information (replaces catkin_package() macro) 
orocos_generate_package(
  INCLUDE_DIRS include
  DEPENDS rtt_ros
)
