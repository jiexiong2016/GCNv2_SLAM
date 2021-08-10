
# The default directory for the settings
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
   add_compile_definitions(DEFAULT_SETTINGS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
else()
   add_definitions(-DDEFAULT_SETTINGS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
endif()

add_executable(stereo_euroc stereo_euroc.cc)
target_link_libraries(stereo_euroc ORB_SLAM2)

add_executable(stereo_kitti stereo_kitti.cc)
target_link_libraries(stereo_kitti ORB_SLAM2)

install(TARGETS stereo_euroc stereo_kitti RUNTIME DESTINATION ${BUILD_INSTALL_PREFIX}/bin)
