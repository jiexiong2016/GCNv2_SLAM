cmake_minimum_required(VERSION 3.1)

project(ORB_SLAM2)

message("Build type: " ${CMAKE_BUILD_TYPE})

# Set where to install stuff

set(BUILD_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../Develop/${CMAKE_BUILD_TYPE})
list(APPEND CMAKE_PREFIX_PATH "${BUILD_INSTALL_PREFIX}")

# Set the language support level
set(CMAKE_C_STANDARD 14)
set(CMAKE_CXX_STANDARD 14)

# The default vocabulary
set(DEFAULT_ORB_VOCABULARY_DIR "${BUILD_INSTALL_PREFIX}/var/lib/orbslam2")
string(CONCAT DEFAULT_ORB_VOCABULARY ${DEFAULT_ORB_VOCABULARY_DIR} "/ORBvoc.txt")
string(CONCAT DEFAULT_BINARY_ORB_VOCABULARY ${DEFAULT_ORB_VOCABULARY_DIR} "/ORBvoc.bin")
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
   add_compile_definitions(DEFAULT_ORB_VOCABULARY="${DEFAULT_ORB_VOCABULARY}")
   add_compile_definitions(DEFAULT_BINARY_ORB_VOCABULARY="${DEFAULT_BINARY_ORB_VOCABULARY}")
else()
   add_definitions(-DDEFAULT_ORB_VOCABULARY="${DEFAULT_ORB_VOCABULARY}")
   add_definitions(-DDEFAULT_BINARY_ORB_VOCABULARY="${DEFAULT_BINARY_ORB_VOCABULARY}")
endif()

# Sort out the OpenGL warning
if (POLICY CMP0072)
   set(OpenGL_GL_PREFERENCE LEGACY)
endif()

# Find OpenCV 4.0; the fallback is OpenCV 3.0
find_package(OpenCV 4.0 QUIET)
if(NOT OpenCV_FOUND)
  if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_package(OpenCV 3.0 HINTS "/usr/local/opt/opencv@3" REQUIRED)
  else()
    find_package(OpenCV 3.0 REQUIRED)
  endif()
endif()
message(STATUS "OPENCV VERSION:" ${OpenCV_VERSION})

# Find the other necessary packages
find_package(Eigen3 REQUIRED)
find_package(Pangolin REQUIRED)
find_package(DBoW2 REQUIRED)
find_package(DLib REQUIRED)
find_package(g2o REQUIRED)

# Add the libraries and examples
add_subdirectory(Libraries)
add_subdirectory(Examples)
add_subdirectory(Resources)

# Third party libraries are built separately to speed things up
