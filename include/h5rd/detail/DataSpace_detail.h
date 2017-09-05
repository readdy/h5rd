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
 * @file DataSpace_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "../DataSpace.h"
#include <iostream>

std::size_t h5rd::DataSpace::ndim() const {
    const auto n = H5Sget_simple_extent_ndims(id());
    if (n < 0) {
        throw Exception("Failed to retrieve ndims for data space");
    }
    return static_cast<std::size_t>(n);
}

h5rd::dimensions h5rd::DataSpace::dims() const {
    dimensions result;
    result.resize(ndim());
    if (H5Sget_simple_extent_dims(id(), result.data(), nullptr) < 0) {
        throw Exception("Failed to retrieve dims for data space");
    }
    return result;
}

h5rd::dimensions h5rd::DataSpace::maxDims() const {
    dimensions result;
    result.resize(ndim());
    if (H5Sget_simple_extent_dims(id(), nullptr, result.data()) < 0) {
        throw Exception("Failed to retrieve max dims for data space");
    }
    return result;
}

void h5rd::DataSpace::close() {
    if(!_parentFile->closed() && valid()) {
        if(H5Sclose(id()) < 0) {
            throw Exception("Error on closing HDF5 data space");
        }
    }
}

h5rd::DataSpace::~DataSpace() {
    try {
        close();
    } catch(const Exception &e) {
        std::cerr << "Unable to close hdf5 data space: " << e.what() << std::endl;
    }
}
