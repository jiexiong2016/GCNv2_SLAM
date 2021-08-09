PROJECT(ORB-SLAM_Vocabulary)

add_custom_command(OUTPUT ORBvoc.bin GCNvoc.bin COMMAND ${CMAKE_COMMAND} -E tar xvf ${CMAKE_CURRENT_SOURCE_DIR}/voc.tar.bz2)
add_custom_command(OUTPUT ${DEFAULT_ORB_VOCABULARY_DIR} ${CMAKE_COMMAND} -E make_directory ${DEFAULT_ORB_VOCABULARY_DIR})
add_custom_command(OUTPUT default_orb_vocabulary COMMAND ${CMAKE_COMMAND} -E copy ORBvoc.bin ${DEFAULT_ORB_VOCABULARY_DIR}/ORBvoc.bin DEPENDS ORBvoc.bin)
add_custom_command(OUTPUT default_gcn_vocabulary COMMAND ${CMAKE_COMMAND} -E copy GCNvoc.bin ${DEFAULT_ORB_VOCABULARY_DIR}/GCNvoc.bin DEPENDS GCNvoc.bin)

add_custom_target(orb_vocabulary ALL DEPENDS default_orb_vocabulary default_gcn_vocabulary)
