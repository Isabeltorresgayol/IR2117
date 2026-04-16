cmake_minimum_required(VERSION 3.8)
project(triangles)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Buscamos las dependencias
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

# EJECUTABLE EQUILATERO
add_executable(equilateral src/equilateral.cpp)
ament_target_dependencies(equilateral rclcpp geometry_msgs)

#EJECUTABLE ISOSCELES
add_executable(isosceles src/isosceles.cpp)
ament_target_dependencies(isosceles rclcpp geometry_msgs)

#EJECUTABLE ESCALENO
# IMPORTANTE: Aquí usamos 'src/escaleno.cpp' que es el nombre de mii archivo
add_executable(scalene src/escaleno.cpp)
ament_target_dependencies(scalene rclcpp geometry_msgs)

#INSTALACIÓN DE EJECUTABLES 
install(TARGETS
  equilateral
  isosceles
  scalene
  DESTINATION lib/${PROJECT_NAME}
)

#INSTALACIÓN DE LAUNCH FILES
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
