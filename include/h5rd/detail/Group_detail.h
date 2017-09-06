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
 * @file Group_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "../Group.h"
#include "../Node.h"

inline h5rd::Group::Group(std::string path, ParentFileRef parentFile) : SubObject(parentFile), path(std::move(path)) {}

inline h5rd::Group::~Group() {
    try {
        close();
    } catch (const Exception &e) {
        std::cerr << "Unable to close hdf5 group: " << e.what() << std::endl;
    }
}

inline void h5rd::Group::close() {
    auto pf = _parentFile.lock();
    if (pf) {
        if (!pf->closed() && valid() && H5Gclose(id()) < 0) {
            throw Exception("Error on closing HDF5 group");
        }
    }
}
