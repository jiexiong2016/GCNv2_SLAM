project(ORB_SLAM2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${DBoW2_INCLUDE_DIR} ${DLib_INCLUDE_DIR})

if (USE_GCN)
  include_directories(${TORCH_INCLUDE_DIRS})
  if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
      add_compile_definitions(USE_GCN="${USE_GCN}")
  else()
      add_definitions(-DUSE_GCN="${USE_GCN}")
  endif()
endif()

if(NOT DEFINED BUILD_SHARED_LIBRARIES)
  set(BUILD_SHARED_LIBRARIES YES)
endif()

if(BUILD_SHARED_LIBRARIES)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()

if (USE_GCN)
  set(GCN_SOURCE src/GCNextractor.cc)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE}
src/Converter.cc
src/FeatureExtractor.cc
src/FeatureExtractorFactory.cc
src/Frame.cc
src/FrameDrawer.cc
${GCN_SOURCE}
src/Initializer.cc
src/KeyFrame.cc
src/KeyFrameDatabase.cc
src/LocalMapping.cc
src/LoopClosing.cc
src/Map.cc
src/MapDrawer.cc
src/MapPoint.cc
src/Optimizer.cc
src/ORBextractor.cc
src/ORBmatcher.cc
src/ORBVocabulary.cc
src/PnPsolver.cc
src/Sim3Solver.cc
src/System.cc
src/Tracking.cc
src/Viewer.cc
)

message(TORCH_LIBRARIES=${TORCH_LIBRARIES})

target_link_libraries(${PROJECT_NAME}
  pangolin
  ${DBoW2_LIBS}
  ${DLib_LIBS}
  ${TORCH_LIBRARIES}
  g2o::types_sba
  g2o::types_sim3
  g2o::core
  ${OpenCV_LIBS}
  Eigen3::Eigen
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
