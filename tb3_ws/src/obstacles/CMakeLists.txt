cmake_minimum_required(VERSION 3.8)
project(obstacles)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(geometry_msgs REQUIRED)


add_executable(detector src/detector.cpp)
ament_target_dependencies(detector rclcpp sensor_msgs example_interfaces)
//nuevooooo
add_executable(avoidance src/avoidance.cpp)
ament_target_dependencies(avoidance rclcpp geometry_msgs example_interfaces)


install(TARGETS
  detector
  avoidance
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})
  
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
