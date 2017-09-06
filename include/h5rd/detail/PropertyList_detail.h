/********************************************************************
 * Copyright © 2017 Computational Molecular Biology Group,          * 
 *                  Freie Universität Berlin (GER)                  *
 *                                                                  *
 * This file is part of ReaDDy.                                     *
 *                                                                  *
 * ReaDDy is free software: you can redistribute it and/or modify   *
 * it under the terms of the GNU Lesser General Public License as   *
 * published by the Free Software Foundation, either version 3 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU Lesser General Public License for more details.              *
 *                                                                  *
 * You should have received a copy of the GNU Lesser General        *
 * Public License along with this program. If not, see              *
 * <http://www.gnu.org/licenses/>.                                  *
 ********************************************************************/


/**
 * << detailed description >>
 *
 * @file PropertyList_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 06.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "../PropertyList.h"
#include "../Filter.h"

#include <iostream>


namespace h5rd {

inline PropertyList::~PropertyList() {
    try {
        close();
    } catch (const Exception &e) {
        std::cerr << "Unable to close hdf5 property list: " << e.what() << std::endl;
    }
}

inline void PropertyList::close() {
    if (valid()) {
        if (H5Pclose(id()) < 0) {
            throw Exception("Error on closing HDF5 property list");
        }
    }
}

inline PropertyList::PropertyList(handle_id cls_id, ParentFileRef parentFile) : super(parentFile) {
    _hid = H5Pcreate(cls_id);
    if (!valid()) {
        throw Exception("Failed to create property list!");
    }
}

inline LinkCreatePropertyList::LinkCreatePropertyList(ParentFileRef parentFile) : PropertyList(H5P_LINK_CREATE,
                                                                                               parentFile) {}

inline void LinkCreatePropertyList::set_create_intermediate_group() {
    H5Pset_create_intermediate_group(id(), 1);
}

inline FileAccessPropertyList::FileAccessPropertyList(ParentFileRef parentFile) : PropertyList(H5P_FILE_ACCESS,
                                                                                               parentFile) {}

inline void FileAccessPropertyList::set_close_degree_weak() {
    H5Pset_fclose_degree(id(), H5F_CLOSE_WEAK);
}

inline void FileAccessPropertyList::set_close_degree_semi() {
    H5Pset_fclose_degree(id(), H5F_CLOSE_SEMI);
}

inline void FileAccessPropertyList::set_close_degree_strong() {
    H5Pset_fclose_degree(id(), H5F_CLOSE_STRONG);
}

inline void FileAccessPropertyList::set_close_degree_default() {
    H5Pset_fclose_degree(id(), H5F_CLOSE_DEFAULT);
}

inline void FileAccessPropertyList::set_use_latest_libver() {
    H5Pset_libver_bounds(id(), H5F_LIBVER_LATEST, H5F_LIBVER_LATEST);
}

inline DataSetCreatePropertyList::DataSetCreatePropertyList(ParentFileRef parentFile) : PropertyList(H5P_DATASET_CREATE,
                                                                                                     parentFile) {}

inline void DataSetCreatePropertyList::set_layout_compact() {
    H5Pset_layout(id(), H5D_COMPACT);
}

inline void DataSetCreatePropertyList::set_layout_contiguous() {
    H5Pset_layout(id(), H5D_CONTIGUOUS);
}

inline void DataSetCreatePropertyList::set_layout_chunked() {
    H5Pset_layout(id(), H5D_CHUNKED);
}

inline void DataSetCreatePropertyList::set_chunk(const dimensions &chunk_dims) {
    H5Pset_chunk(id(), static_cast<int>(chunk_dims.size()), chunk_dims.data());
}

inline void DataSetCreatePropertyList::activate_filter(Filter *filter) {
    filter->activate(*this);
}


}