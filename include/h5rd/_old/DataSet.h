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
 * @file DataSet.h
 * @brief << brief description >>
 * @author clonker
 * @date 04/01/2017
 * @copyright GNU Lesser General Public License v3.0
 */
#pragma once

#include "H5Types.h"
#include "DataSpace.h"
#include "DataSetType.h"

namespace h5readdy {

enum DataSetCompression {
    none = 0x0000, blosc = 0x0001
};

class DataSetHandle : public ObjectHandle {
public:
    explicit DataSetHandle(handle_id handle = -1) : ObjectHandle(handle) {}

    DataSetHandle(const DataSetHandle &) = default;

    DataSetHandle &operator=(const DataSetHandle &) = default;

    DataSetHandle(DataSetHandle &&) = default;

    DataSetHandle &operator=(DataSetHandle &&) = default;

    ~DataSetHandle() {
        if (_handle >= 0) {
            close();
        }
    }

    void close() override {
        if (H5Dclose(_handle) < 0) {
            // todo log::error("error on closing data set {}!", _handle);
            H5Eprint(H5Eget_current_stack(), stderr);
        }
    }

};

class DataSet : public Object {
public:

    DataSet(handle_id handle, const DataSetType &memoryType, const DataSetType &fileType);

    virtual ~DataSet();

    DataSet(DataSet &&rhs) = default;

    DataSet &operator=(DataSet &&) = default;

    DataSet(const DataSet &) = default;

    DataSet &operator=(const DataSet &) = default;

    template<typename T>
    void append(std::vector<T> &data);

    template<typename T>
    void append(const dimensions &dims, const T *const data);

    void flush();

    DataSpace getFileSpace() const;

    dimension &extensionDim() {
        return _extensionDim;
    }

    const dimension &extensionDim() const {
        return _extensionDim;
    }

private:
    dimension _extensionDim;
    DataSpace memorySpace;
    DataSetType memoryType{-1};
    DataSetType fileType{-1};
};

class VLENDataSet : public Object {
public:
    VLENDataSet(handle_id handle, const DataSetType &memoryType, const DataSetType &fileType);

    virtual ~VLENDataSet() = default;

    VLENDataSet(VLENDataSet &&rhs) = default;

    VLENDataSet &operator=(VLENDataSet &&) = default;

    VLENDataSet(const VLENDataSet &) = default;

    VLENDataSet &operator=(const VLENDataSet &) = default;

    template<typename T>
    void append(std::vector<std::vector<T>> &data);

    template<typename T>
    void append(const dimensions &dims, std::vector<T> *const data);

    void flush() {
        if (hid() >= 0 && H5Fflush(hid(), H5F_SCOPE_LOCAL) < 0) {
            throw std::runtime_error("error when flushing HDF5 data set with handle " + std::to_string(hid()));
        }
    };

    DataSpace getFileSpace() const {
        // todo log::debug("getting file space for hid={}", hid());
        auto _hid = H5Dget_space(hid());
        if (_hid < 0) {
            // todo log::error("Failed to get data set space!");
            H5Eprint(H5Eget_current_stack(), stderr);
        }
        return DataSpace(_hid);
    };

    dimension &extensionDim() {
        return _extensionDim;
    }

    const dimension &extensionDim() const {
        return _extensionDim;
    }

private:
    dimension _extensionDim;
    DataSpace memorySpace;
    DataSetType memoryType{-1};
    DataSetType fileType{-1};
};

}


#include "bits/DataSet_bits.h"
