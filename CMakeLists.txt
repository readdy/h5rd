cmake_minimum_required(VERSION 3.8)
project(readdy_io)

set(CMAKE_CXX_STANDARD 14)

option(READDY_IO_TEST "Build tests?" ON)

set(READDY_IO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(READDY_IO_HEADERS
        include/h5rd/h5rd.h
        include/h5rd/common.h
        include/h5rd/Exception.h
        include/h5rd/Node.h
        include/h5rd/detail/Node_detail.h
        include/h5rd/Object.h
        include/h5rd/detail/Object_detail.h
        include/h5rd/Group.h
        include/h5rd/detail/Group_detail.h
        include/h5rd/DataSet.h
        include/h5rd/VLENDataSet.h
        include/h5rd/detail/DataSet_detail.h
        include/h5rd/detail/VLENDataSet_detail.h
        include/h5rd/DataSetType.h
        include/h5rd/detail/DataSetType_detail.h
        include/h5rd/Filter.h
        include/h5rd/detail/Filter_detail.h
        include/h5rd/PropertyList.h
        include/h5rd/detail/PropertyList_detail.h
        include/h5rd/DataSpace.h
        include/h5rd/detail/DataSpace_detail.h
        include/h5rd/File.h
        include/h5rd/detail/File_detail.h
        include/h5rd/detail/String_utils.h
        )

string(REPLACE "include/" "${CMAKE_CURRENT_SOURCE_DIR}/include/" READDY_IO_HEADERS "${READDY_IO_HEADERS}")

find_package(HDF5 COMPONENTS HL REQUIRED)
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