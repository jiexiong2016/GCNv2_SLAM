
# The default directory for the settings
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
   add_compile_definitions(DEFAULT_SETTINGS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
else()
   add_definitions(-DDEFAULT_SETTINGS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
endif()

add_executable(rgbd_tum rgbd_tum.cc)
target_link_libraries(rgbd_tum ORB_SLAM2)

install(TARGETS rgbd_tum RUNTIME DESTINATION ${BUILD_INSTALL_PREFIX}/bin)
