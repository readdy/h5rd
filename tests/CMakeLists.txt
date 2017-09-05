# Find pthread library
find_package(Threads REQUIRED)

set(TEST_SOURCES
        TestMain.cpp)

add_executable(tests ${TEST_SOURCES} ${READDY_IO_HEADERS})

target_include_directories(tests PRIVATE
        ${READDY_IO_INCLUDE_DIR}
        ${HDF5_INCLUDE_DIRS} ${HDF5_HL_INCLUDE_DIR}
        "${gtest_SOURCE_DIR}/include" "${gmock_SOURCE_DIR}/include")

target_link_libraries(tests PUBLIC
        gmock_main
        Threads::Threads
        ${HDF5_LIBRARIES} ${HDF5_HL_LIBRARIES})
