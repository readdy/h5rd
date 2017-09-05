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
 * @file Node_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <H5LTpublic.h>

#include "../PropertyList.h"
#include "../Node.h"
#include "../Group.h"
#include "../DataSet.h"
#include "../common.h"
#include "String_utils.h"

template<typename Container>
inline bool h5rd::Node<Container>::exists(const std::string &name) const {
    return static_cast<bool>(H5Lexists(me()->id(), name.c_str(), H5P_DEFAULT));
}

template<typename Container>
inline h5rd::Group h5rd::Node<Container>::subgroup(const std::string &name) {
    auto id = me()->id();
    auto gid = H5Gopen(id, name.data(), H5P_DEFAULT);
    if(gid < 0) {
        throw Exception("Could not open subgroup " + name + "!");
    }
    Group group(name, me()->parentFile());
    group._hid = gid;
    return group;
}

template<typename Container>
inline h5rd::group_info h5rd::Node<Container>::info() const {
    auto id = me()->id();
    group_info info {};
    if(H5Gget_info(id, &info) < 0) {
        throw Exception("Failed to get group info!");
    }
    return info;
}

template<typename Container>
inline std::vector<std::string> h5rd::Node<Container>::sub_elements(H5O_type_t type) const {
    auto id = me()->id();
    std::vector<std::string> result;
    auto group_info = info();
    result.reserve(group_info.nlinks);
    for(std::size_t i=0; i < group_info.nlinks; ++i) {
        H5O_info_t oinfo {};
        H5Oget_info_by_idx(id, ".", H5_INDEX_NAME, H5_ITER_INC, i, &oinfo, H5P_DEFAULT);
        if(oinfo.type == type) {
            auto size = 1+ H5Lget_name_by_idx (id, ".", H5_INDEX_NAME, H5_ITER_INC, i, NULL, 0, H5P_DEFAULT);
            if(size < 0) {
                H5Eprint(H5Eget_current_stack(), stderr);
            }

            std::vector<char> c_string (static_cast<std::size_t>(size));
            H5Lget_name_by_idx (id, ".", H5_INDEX_NAME, H5_ITER_INC, i, c_string.data(), (std::size_t) size, H5P_DEFAULT);
            std::string label (c_string.data());

            result.push_back(std::move(label));
        }
    }
    return result;
}

template<typename Container>
inline std::vector<std::string> h5rd::Node<Container>::containedDataSets() const {
    return sub_elements(H5O_TYPE_DATASET);
}

template<typename Container>
inline std::vector<std::string> h5rd::Node<Container>::subgroups() const {
    return sub_elements(H5O_TYPE_GROUP);
}

template<typename Container>
inline h5rd::Group h5rd::Node<Container>::createGroup(const std::string &path) {
    auto id = me()->id();
    Group group(path, me()->parentFile());
    if(util::groupExists(id, path)) {
        if((group._hid = H5Gopen(id, path.c_str(), H5P_DEFAULT)) < 0) {
            throw Exception("Failed to open group (" + path + ")");
        }
        return group;
    } else {
        LinkCreatePropertyList plist(me()->parentFile());
        plist.set_create_intermediate_group();
        if((group._hid = H5Gcreate(id, path.c_str(), plist.id(), H5P_DEFAULT, H5P_DEFAULT)) < 0) {
            throw Exception("Failed to create group with intermediates (" + path + ")");
        }
        return group;
    }
}

template<typename Container>
inline void h5rd::Node<Container>::write(const std::string &dataSetName, const std::string &string) {
    auto stdstr = STDDataSetType<std::string>(me()->parentFile());
    auto nativestr = NativeDataSetType<std::string>(me()->parentFile());
    H5Tset_cset(stdstr.id(), H5T_CSET_UTF8);
    H5Tset_cset(nativestr.id(), H5T_CSET_UTF8);

    if(H5LTmake_dataset_string(me()->id(), dataSetName.c_str(), string.c_str()) < 0) {
        throw Exception("there was a problem with writing \""+string+"\" into a hdf5 file.");
    }
}

template<typename Container>
template<typename T>
inline void h5rd::Node<Container>::write(const std::string &dataSetName, const std::vector<T> &data) {
    if(std::is_same<typename std::decay<T>::type, std::string>::value) {
        util::writeVector(me()->id(), dataSetName, data);
    } else {
        write(dataSetName, {data.size()}, data.data());
    }
}

namespace {
template<typename T>
inline void help_write(h5rd::handle_id handle, const std::string &dsname, const h5rd::dimensions &dims, const T* data) {}
template<>
inline void help_write(h5rd::handle_id handle, const std::string &dsname, const h5rd::dimensions &dims, const short* data) {
    H5LTmake_dataset_short(handle, dsname.data(), static_cast<int>(dims.size()), dims.data(), data);
}
template<>
inline void help_write(h5rd::handle_id handle, const std::string &dsname, const h5rd::dimensions &dims, const int* data) {
    H5LTmake_dataset_int(handle, dsname.data(), static_cast<int>(dims.size()), dims.data(), data);
}
template<>
inline void help_write(h5rd::handle_id handle, const std::string &dsname, const h5rd::dimensions &dims, const long* data) {
    H5LTmake_dataset_long(handle, dsname.data(), static_cast<int>(dims.size()), dims.data(), data);
}
template<>
inline void help_write(h5rd::handle_id handle, const std::string &dsname, const h5rd::dimensions &dims, const float* data) {
    H5LTmake_dataset_float(handle, dsname.data(), static_cast<int>(dims.size()), dims.data(), data);
}
template<>
inline void help_write(h5rd::handle_id handle, const std::string &dsname, const h5rd::dimensions &dims, const double* data) {
    H5LTmake_dataset_double(handle, dsname.data(), static_cast<int>(dims.size()), dims.data(), data);
}
}


template<typename Container>
template<typename T>
inline void h5rd::Node<Container>::write(const std::string &dataSetName, const dimensions &dims, const T *data) {
    using TT = typename std::decay<T>;
    help_write(me()->id(), dataSetName, dims, data);
}

template<typename Container>
template<typename T>
inline std::unique_ptr<h5rd::DataSet> h5rd::Node<Container>::createDataSet(
        const std::string &name, const dimensions &chunkSize, const dimensions &maxDims,
        h5rd::DataSetCompression compression) {
    return createDataSet(name, chunkSize, maxDims, STDDataSetType<T>(me()->parentFile()),
                         NativeDataSetType<T>(me()->parentFile()), compression);
}

template<typename Container>
inline std::unique_ptr<h5rd::DataSet>
h5rd::Node<Container>::createDataSet(const std::string &name, const h5rd::dimensions &chunkSize,
                                     const h5rd::dimensions &maxDims, const h5rd::DataSetType &memoryType,
                                     const h5rd::DataSetType &fileType, h5rd::DataSetCompression compression) {
    dimension extensionDim;
    {
        std::stringstream result;
        std::copy(maxDims.begin(), maxDims.end(), std::ostream_iterator<int>(result, ", "));
        // todo log::trace("creating data set with maxDims={}", result.str());
    }
    // validate and find extension dim
    {
        auto unlimited_it = std::find(maxDims.begin(), maxDims.end(), UNLIMITED_DIMS);
        bool containsUnlimited = unlimited_it != maxDims.end();
        if (!containsUnlimited) {
            throw std::runtime_error("needs to contain unlimited_dims in some dimension to be extensible");
        }
        extensionDim = static_cast<dimension>(std::distance(maxDims.begin(), unlimited_it));
        // todo log::trace("found extension dim {}", extensionDim);
    }
    handle_id handle;
    {
        // set up empty data set
        dimensions dims(maxDims.begin(), maxDims.end());
        dims[extensionDim] = 0;
        DataSpace fileSpace(me()->parentFile(), dims, maxDims);
        DataSetCreatePropertyList propertyList (me()->parentFile());
        propertyList.set_layout_chunked();
        propertyList.set_chunk(chunkSize);

        auto _hid = H5Dcreate(me()->id(), name.c_str(), fileType.id(),
                              fileSpace.id(), H5P_DEFAULT, propertyList.id(), H5P_DEFAULT);
        if (_hid < 0) {
            throw Exception("Error on creating data set " + std::to_string(_hid));
        } else {
            handle = _hid;
        }
    }
    auto ds = std::make_unique<DataSet>(me()->parentFile(), memoryType, fileType);
    ds->_hid = handle;
    ds->extensionDim() = extensionDim;
    return ds;
}

template<typename Container>
template<typename T>
inline void h5rd::Node<Container>::read(const std::string &dataSetName, std::vector<T> &array) {
    STDDataSetType<T> stdDST (me()->parentFile());
    NativeDataSetType<T> nDST (me()->parentFile());
    read(dataSetName, array, &stdDST, &nDST);
}

template<typename Container>
template<typename T>
inline void h5rd::Node<Container>::read(const std::string &dataSetName, std::vector<T> &array, DataSetType *memoryType,
                                        DataSetType *fileType) {
    //blosc_compression::initialize();

    const auto n_array_dims = 1 + util::n_dims<T>::value;
    auto hid = H5Dopen2(me()->id(), dataSetName.data(), H5P_DEFAULT);

    DataSpace memorySpace (me()->parentFile(), H5Dget_space(hid));

    const auto ndim = memorySpace.ndim();

    //if(ndim != n_array_dims) {
    //    log::error("wrong dimensionality: {} != {}", ndim, n_array_dims);
    //    throw std::invalid_argument("wrong dimensionality when attempting to read a data set");
    //}

    const auto dims = memorySpace.dims();
    std::size_t required_length = 1;
    for(const auto dim : dims) {
        // todo log::trace("dim len = {}", dim);
        required_length *= dim;
    }
    // todo log::trace("required length = {}", required_length);
    array.resize(required_length);

    auto result = H5Dread(hid, memoryType->id(), H5S_ALL, H5S_ALL, H5P_DEFAULT, array.data());

    if(result < 0) {
        // todo log::trace("Failed reading result!");
        throw Exception("Failed reading \""+ dataSetName +"\"!");
    }

    H5Dclose(hid);

    //for(std::size_t d = 0; d < ndim-1; ++d) {
    //    for(auto& sub_arr : array) {
    //        sub_arr.resize(dims[1]);
    //    }
    //}

    // todo reshape array to dims
}
