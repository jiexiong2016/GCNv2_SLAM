add_executable(stereo_euroc stereo_euroc.cc)
target_link_libraries(stereo_euroc ORB_SLAM2)

add_executable(stereo_kitti stereo_kitti.cc)
target_link_libraries(stereo_kitti ORB_SLAM2)

install(TARGETS stereo_euroc stereo_kitti RUNTIME DESTINATION ${BUILD_INSTALL_PREFIX}/bin)
