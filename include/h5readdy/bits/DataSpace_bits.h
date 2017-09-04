/********************************************************************
 * Copyright © 2016 Computational Molecular Biology Group,          * 
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
 * @file DataSpace_bits.h
 * @brief << brief description >>
 * @author clonker
 * @date 25.05.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "../DataSpace.h"

namespace h5readdy {

inline DataSpace::DataSpace(handle_id handle) : Object(std::make_shared<DataSpaceHandle>(handle)) {}

inline DataSpace::DataSpace(const dimensions &dims, const dimensions &maxDims)
        : Object(std::make_shared<DataSpaceHandle>(-1)) {
    handle_id hid;
    if (maxDims.empty()) {
        hid = H5Screate_simple(static_cast<int>(dims.size()), dims.data(), nullptr);
    } else {
        hid = H5Screate_simple(static_cast<int>(dims.size()), dims.data(), maxDims.data());
    }
    if (hid < 0) {
        // todo log::error("error on creating data space!");
        H5Eprint(H5Eget_current_stack(), stderr);
    } else {
        handle->set(hid);
    }
}

inline std::size_t DataSpace::ndim() const {
    const auto n = H5Sget_simple_extent_ndims(hid());
    if (n < 0) {
        // todo log::error("failed to retrieve ndims");
        H5Eprint(H5Eget_current_stack(), stderr);
    }
    return static_cast<std::size_t>(n);
}

inline dimensions DataSpace::dims() const {
    dimensions result;
    result.resize(ndim());
    if (H5Sget_simple_extent_dims(hid(), result.data(), nullptr) < 0) {
        // todo log::error("failed to get dims!");
        H5Eprint(H5Eget_current_stack(), stderr);
    }
    return result;
}

inline dimensions DataSpace::max_dims() const {
    dimensions result;
    result.resize(ndim());
    if (H5Sget_simple_extent_dims(hid(), nullptr, result.data()) < 0) {
        // todo log::error("failed to get dims!");
        H5Eprint(H5Eget_current_stack(), stderr);
    }
    return result;
}

}
