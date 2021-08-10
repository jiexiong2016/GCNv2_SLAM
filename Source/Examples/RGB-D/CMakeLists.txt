add_executable(rgbd_tum rgbd_tum.cc)
target_link_libraries(rgbd_tum ORB_SLAM2)

install(TARGETS rgbd_tum RUNTIME DESTINATION ${BUILD_INSTALL_PREFIX}/bin)
