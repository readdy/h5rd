cmake_minimum_required(VERSION 3.8)
project(readdy_io)

set(CMAKE_CXX_STANDARD 11)

option(READDY_IO_TEST "Build tests?" ON)

set(READDY_IO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(READDY_IO_HEADERS
        include/h5readdy/h5readdy.h
        include/h5readdy/traits.h
        include/h5readdy/DataSet.h
        include/h5readdy/DataSetType.h
        include/h5readdy/DataSpace.h
        include/h5readdy/File.h
        include/h5readdy/Group.h
        include/h5readdy/H5Types.h
        include/h5readdy/Object.h
        include/h5readdy/PropertyList.h
        include/h5readdy/bits/File_bits.h
        include/h5readdy/bits/DataSet_bits.h
        include/h5readdy/bits/DataSetType_bits.h
        include/h5readdy/bits/DataSpace_bits.h
        include/h5readdy/bits/Group_bits.h
        include/h5readdy/bits/String_utils.h
        include/h5readdy/bits/Util_bits.h)

string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/" READDY_IO_HEADERS "${READDY_IO_HEADERS}")

find_package(HDF5 COMPONENTS C HL REQUIRED)
message(STATUS "HDF5_FOUND: ${HDF5_FOUND}, version ${HDF5_VERSION}")

if (READDY_IO_TEST)
    include(cmake/Modules/DownloadProject.cmake)
    download_project(PROJ googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.8.0
            ${UPDATE_DISCONNECTED_IF_AVAILABLE}
            )

    # Prevent GoogleTest from overriding our compiler/linker options
    # when building with Visual Studio
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

    enable_testing()

    add_subdirectory(tests)
endif()