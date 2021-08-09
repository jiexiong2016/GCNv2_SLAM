PROJECT(ORB-SLAM_Tools)

# Build tools

add_executable(bin_vocabulary bin_vocabulary.cc)
target_link_libraries(bin_vocabulary ORB_SLAM2)

# Run the newly-built executable if the bin file doesn't exist

add_custom_command(OUTPUT ${DEFAULT_BINARY_ORB_VOCABULARY} COMMAND bin_vocabulary DEPENDS ${DEFAULT_ORB_VOCABULARY})

add_custom_target(build_orb_bin_vocabulary ALL DEPENDS ${DEFAULT_BINARY_ORB_VOCABULARY})
