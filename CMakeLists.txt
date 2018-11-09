cmake_minimum_required(VERSION 3.12)

get_directory_property(H5RD_HAS_PARENT PARENT_DIRECTORY)
if(H5RD_HAS_PARENT)
    set(H5RD_IS_SUBPROJECT ON)
endif()

set(CMAKE_CXX_STANDARD 17)

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

add_library(h5rd INTERFACE)
add_library(h5rd::h5rd ALIAS h5rd)


target_include_directories(
        h5rd
        INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

set(HEADER_BASE "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (NOT H5RD_IS_SUBPROJECT)
    option(H5RD_BUILD_TESTING "Build h5rd tests" ON)
endif()

if (H5RD_BUILD_TESTING)

    find_package(HDF5 COMPONENTS HL REQUIRED)
    message(STATUS "HDF5_FOUND: ${HDF5_FOUND}, version ${HDF5_VERSION}")

    if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/contrib/Catch2/CMakeLists.txt)
        add_subdirectory(contrib/Catch2)
        include(${CMAKE_CURRENT_LIST_DIR}/contrib/Catch2/contrib/Catch.cmake)
    elseif (NOT TARGET Catch2::Catch2)
        find_package(Catch2 QUIET)
        if (CATCH2_FOUND)
            include(Catch)
        else()
            message(SEND_ERROR "Catch2 not found.")
        endif()
    endif ()
    add_subdirectory(tests)
endif()
