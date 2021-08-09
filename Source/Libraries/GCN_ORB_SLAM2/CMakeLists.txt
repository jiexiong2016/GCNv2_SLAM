include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${DBoW2_INCLUDE_DIR} ${DLib_INCLUDE_DIR})

if(NOT DEFINED BUILD_SHARED_LIBRARIES)
  set(BUILD_SHARED_LIBRARIES NO)
endif()

if(BUILD_SHARED_LIBRARIES)
  set(LIB_TYPE SHARED)
else()
  set(LIB_TYPE STATIC)
endif()

project(ORB_SLAM2)
message(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} ${LIB_TYPE}
src/Converter.cc
src/FeatureExtractor.cc
src/FeatureExtractorFactory.cc
src/Frame.cc
src/FrameDrawer.cc
src/GCNextractor.cc
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

target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
Eigen3::Eigen
pangolin
${DBoW2_LIBS}
${DLib_LIBS}
g2o::types_sba
g2o::types_sim3
g2o::core
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
