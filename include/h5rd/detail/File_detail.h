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
 * @file File_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "../File.h"

namespace {
inline int convertFlag(const h5rd::File::Flag &flag) {
    switch (flag) {
        case h5rd::File::Flag::READ_ONLY:
            return H5F_ACC_RDONLY;
        case h5rd::File::Flag::READ_WRITE:
            return H5F_ACC_RDWR;
        case h5rd::File::Flag::OVERWRITE:
            return H5F_ACC_TRUNC;
        case h5rd::File::Flag::FAIL_IF_EXISTS:
            return H5F_ACC_EXCL;
        case h5rd::File::Flag::CREATE_NON_EXISTING:
            return H5F_ACC_CREAT;
        case h5rd::File::Flag::DEFAULT:
            return H5F_ACC_RDWR | H5F_ACC_CREAT | H5F_ACC_TRUNC;
    }
}
}

inline h5rd::File::File(const std::string &path, const Action &action, const Flag &flag)
        : File(path, action, Flags{flag}){}

inline h5rd::File::File(const std::string &path, const Action &action, const Flags &flags) : Object(this), path(path) {
    unsigned flag = 0x0000u;
    for (const auto &f : flags) {
        flag = flag | convertFlag(f);
    }
    FileAccessPropertyList fapl(_parentFile);
    fapl.set_close_degree_strong();
    fapl.set_use_latest_libver();
    handle_id val = 0;
    switch (action) {
        case Action::CREATE: {
            val = H5Fcreate(path.c_str(), flag, H5P_DEFAULT, fapl.id());
            break;
        }
        case Action::OPEN: {
            val = H5Fopen(path.c_str(), flag, fapl.id());
            break;
        }
    }
    if(val < 0){
        throw Exception("Failed on opening/creating file " + path);
    }
    _hid = val;
    _parentFile = this;
}

inline void h5rd::File::flush() {
    if (H5Fflush(_hid, H5F_SCOPE_LOCAL) < 0) {
        throw Exception("error when flushing HDF5 file \"" + path + "\"");
    }
}

inline h5rd::File::~File() {
    try {
        close();
    } catch(const Exception &e) {
        std::cerr << "Unable to close hdf5 file: " << e.what() << std::endl;
    }
}

inline void h5rd::File::close() {
    if(!closed() && valid()) {
        if(H5Fclose(id()) < 0) {
            throw Exception("Error on closing HDF5 file \"" + path + "\"");
        }
        _closed = true;
    }
}
