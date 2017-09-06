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
 * @file DataSet_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <iostream>
#include <iterator>

#include "../DataSet.h"
#include "../DataSpace.h"
#include "../DataSetType.h"

inline h5rd::DataSet::~DataSet() {
    try {
        close();
    } catch (const Exception &e) {
        std::cerr << "Unable to close hdf5 data set: " << e.what() << std::endl;
    }
}

inline void h5rd::DataSet::close() {
    auto pf = _parentFile.lock();
    if (pf) {
        if (!pf->closed() && valid() && H5Dclose(id()) < 0) {
            throw Exception("Error on closing HDF5 data set");
        }
    }
}

inline h5rd::DataSet::DataSet(ParentFileRef parentFile, const DataSetType &memoryType, const DataSetType &fileType)
        : SubObject(parentFile), _memoryType(memoryType), _fileType(fileType) {}

inline h5rd::dimension &h5rd::DataSet::extensionDim() {
    return _extensionDim;
}

inline const h5rd::dimension &h5rd::DataSet::extensionDim() const {
    return _extensionDim;
}

inline std::shared_ptr<h5rd::DataSpace> h5rd::DataSet::getFileSpace() const {
    auto _hid = H5Dget_space(id());
    if (_hid < 0) {
        throw Exception("Failed to get file space for data set!");
    }
    return std::make_shared<h5rd::DataSpace>(_parentFile, _hid);
}

inline void h5rd::DataSet::flush() {
    if (valid() && H5Fflush(id(), H5F_SCOPE_LOCAL) < 0) {
        throw Exception("error when flushing HDF5 data set with handle " + std::to_string(id()));
    }
}

template<typename T>
inline void h5rd::DataSet::append(std::vector<T> &data) {
    if (!data.empty()) append({1, data.size()}, data.data());
}

template<typename T>
inline void h5rd::DataSet::append(const h5rd::dimensions &dims, const T *data) {
    {
        std::stringstream result;
        std::copy(dims.begin(), dims.end(), std::ostream_iterator<int>(result, ", "));
        // todo log::trace("appending to regular data set with data size = ({})", result.str());
    }
    if (dims.size() != getFileSpace()->ndim()) {
        // todo log::error("Tried to append data with ndims={} to set with ndims={}", dims.size(), getFileSpace().ndim());
        throw std::invalid_argument("tried to append data with wrong dimensionality!");
    }
    if (!_memorySpace) {
        _memorySpace = std::make_unique<DataSpace>(_parentFile, dims);
    } else {
        H5Sset_extent_simple(_memorySpace->id(), static_cast<int>(dims.size()), dims.data(), nullptr);
    }
    dimensions currentExtent;
    dimensions offset;
    offset.resize(dims.size());
    {
        currentExtent = getFileSpace()->dims();
    }
    offset[_extensionDim] = currentExtent[_extensionDim];
    {
        dimensions newExtent(currentExtent);
        newExtent[_extensionDim] += dims[_extensionDim];
        H5Dset_extent(id(), newExtent.data());
    }
    // todo log::trace("selecting hyperslab with:");
    {
        std::stringstream result;
        std::copy(offset.begin(), offset.end(), std::ostream_iterator<int>(result, ", "));
        // todo log::trace("    current extent = {}", result.str());
    }
    {
        std::stringstream result;
        std::copy(dims.begin(), dims.end(), std::ostream_iterator<int>(result, ", "));
        // todo log::trace("    size = {}", result.str());
    }
    auto fileSpace = getFileSpace();
    H5Sselect_hyperslab(fileSpace->id(), H5S_SELECT_SET, offset.data(), nullptr, dims.data(), nullptr);
    if (H5Dwrite(id(), _memoryType.id(), _memorySpace->id(), fileSpace->id(), H5P_DEFAULT, data) < 0) {
        //log::error("Error with data set {}", hid());
        //H5Eprint(H5Eget_current_stack(), stderr);
        throw Exception("Error on writing data set " + std::to_string(id()));
    }
}
