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
 * @file PropretySet.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <iostream>
#include "Object.h"
#include "blosc.h"

namespace h5rd {

class PropertyList : public Object {
    using super = Object;
public:
    explicit PropertyList(handle_id cls_id, Object* parentFile) : super(parentFile) {
        _hid = H5Pcreate(cls_id);
        if(!valid()) {
            throw Exception("Failed to create property list!");
        }
    }

    ~PropertyList() override {
        try {
            close();
        } catch(const Exception &e) {
            std::cerr << "Unable to close hdf5 property list: " << e.what() << std::endl;
        }
    }

    void close() override {
        if(valid()) {
            if(H5Pclose(id()) < 0) {
                throw Exception("Error on closing HDF5 property list");
            }
        }
    }
};

class LinkCreatePropertyList : public PropertyList {
public:
    explicit LinkCreatePropertyList(Object* parentFile) : PropertyList(H5P_LINK_CREATE, parentFile) {}

    void set_create_intermediate_group() {
        H5Pset_create_intermediate_group(id(), 1);
    }
};

class FileAccessPropertyList : public PropertyList {
public:
    explicit FileAccessPropertyList(Object* parentFile) : PropertyList(H5P_FILE_ACCESS, parentFile) {}

    void set_close_degree_weak() {
        H5Pset_fclose_degree(id(), H5F_CLOSE_WEAK);
    }

    void set_close_degree_semi() {
        H5Pset_fclose_degree(id(), H5F_CLOSE_SEMI);
    }

    void set_close_degree_strong() {
        H5Pset_fclose_degree(id(), H5F_CLOSE_STRONG);
    }

    void set_close_degree_default() {
        H5Pset_fclose_degree(id(), H5F_CLOSE_DEFAULT);
    }

    void set_use_latest_libver() {
        H5Pset_libver_bounds(id(), H5F_LIBVER_LATEST, H5F_LIBVER_LATEST);
    }
};

class DataSetCreatePropertyList : public PropertyList {
public:
    explicit DataSetCreatePropertyList(Object* parentFile) : PropertyList(H5P_DATASET_CREATE, parentFile) {}

    void set_layout_compact() {
        H5Pset_layout(id(), H5D_COMPACT);
    }

    void set_layout_contiguous() {
        H5Pset_layout(id(), H5D_CONTIGUOUS);
    }

    void set_layout_chunked() {
        H5Pset_layout(id(), H5D_CHUNKED);
    }

    void set_chunk(const dimensions &chunk_dims) {
        H5Pset_chunk(id(), static_cast<int>(chunk_dims.size()), chunk_dims.data());
    }

    void activate_blosc() {
        unsigned int cd_values[7];
        blosc_compression::activate(id(), cd_values);
    }


};

}