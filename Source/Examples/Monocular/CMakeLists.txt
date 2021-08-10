
# The default directory for the settings
if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.0")
   add_compile_definitions(DEFAULT_SETTINGS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
else()
   add_definitions(-DDEFAULT_SETTINGS_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")
endif()

add_executable(mono_tum mono_tum.cc)
target_link_libraries(mono_tum ORB_SLAM2)

add_executable(mono_euroc mono_euroc.cc)
target_link_libraries(mono_euroc ORB_SLAM2)

add_executable(mono_kitti mono_kitti.cc)
target_link_libraries(mono_kitti ORB_SLAM2)

install(TARGETS mono_tum mono_euroc mono_kitti RUNTIME DESTINATION ${BUILD_INSTALL_PREFIX}/bin)
