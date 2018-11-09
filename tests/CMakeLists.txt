add_executable (h5rd_test main.cpp Sanity.cpp)
target_link_libraries(h5rd_test h5rd Catch2::Catch2 ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})
target_include_directories(h5rd_test PUBLIC ${HDF5_INCLUDE_DIRS} ${HDF5_HL_INCLUDE_DIR})
catch_discover_tests(h5rd_test)
