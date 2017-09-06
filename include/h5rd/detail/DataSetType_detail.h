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
 * @file DataSetType_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <iostream>
#include "../DataSetType.h"

inline h5rd::DataSetType::DataSetType(handle_id hid, ParentFileRef parentFile) : SubObject(parentFile) {
    _hid = hid;
}

inline h5rd::DataSetType::DataSetType(const DataSetType &rhs) : SubObject(rhs._parentFile) {
    _hid = rhs._hid;
    _closed = rhs._closed;
    if (/*_parentFile && !_parentFile->closed() && */valid()) {
        if (H5Iinc_ref(_hid) < 0) {
            throw Exception("Error on increase HDF5 reference counter for data type");
        }
    }
}

inline h5rd::DataSetType &h5rd::DataSetType::operator=(const h5rd::DataSetType &rhs) {
    if (this != &rhs) {
        if (_hid != H5I_INVALID_HID) {
            if (H5Idec_ref(_hid) < 0) {
                throw Exception("Error in decrease of HDF5 reference counter for data type, copy assign");
            }
        }
        _hid = rhs._hid;
        _closed = rhs._closed;
        _parentFile = rhs._parentFile;
        if (/*_parentFile && !_parentFile->closed() && */valid()) {
            if (H5Iinc_ref(_hid) < 0) {
                throw Exception("Error in increase HDF5 reference counter for data type, copy assign");
            }
        }
    }
    return *this;
}

inline h5rd::DataSetType::~DataSetType() {
    try {
        close();
    } catch (const Exception &e) {
        std::cerr << "Unable to close hdf5 data type: " << e.what() << std::endl;
    }
}

inline void h5rd::DataSetType::close() {
    if (/*_parentFile && !_parentFile->closed() && */valid()) {
        if (H5Idec_ref(id()) < 0) {
            throw Exception("Error on decrease HDF5 reference counter for data type");
        }
    }
}

namespace h5rd {
inline VLENDataSetType::VLENDataSetType(const DataSetType &other) : DataSetType(H5Tvlen_create(other.id()),
                                                                                other.parentFile()) {}

template<>
inline STDDataSetType<unsigned char>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_I8LE),
                                                                                             parentFile) {}

template<>
inline
STDDataSetType<short>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_I16LE), parentFile) {}

template<>
inline STDDataSetType<unsigned short>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_U16LE),
                                                                                              parentFile) {}

template<>
inline
STDDataSetType<int>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_I32LE), parentFile) {}

template<>
inline STDDataSetType<unsigned int>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_U32LE),
                                                                                            parentFile) {}

template<>
inline
STDDataSetType<long>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_I64LE), parentFile) {}

template<>
inline STDDataSetType<unsigned long>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_U64LE),
                                                                                             parentFile) {}

template<>
inline
STDDataSetType<long long>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_I64LE), parentFile) {}

template<>
inline
STDDataSetType<unsigned long long>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_STD_U64LE),
                                                                                           parentFile) {}

template<>
inline
STDDataSetType<float>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_IEEE_F32LE), parentFile) {}

template<>
inline
STDDataSetType<double>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_IEEE_F64LE), parentFile) {}

template<>
inline
STDDataSetType<std::string>::STDDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_C_S1), parentFile) {}

template<>
inline
NativeDataSetType<unsigned char>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_UCHAR),
                                                                                            parentFile) {}

template<>
inline NativeDataSetType<short>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_SHORT),
                                                                                           parentFile) {}

template<>
inline
NativeDataSetType<unsigned short>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_USHORT),
                                                                                             parentFile) {}

template<>
inline NativeDataSetType<int>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_INT),
                                                                                         parentFile) {}

template<>
inline
NativeDataSetType<unsigned int>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_UINT),
                                                                                           parentFile) {}

template<>
inline NativeDataSetType<long>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_LONG),
                                                                                          parentFile) {}

template<>
inline
NativeDataSetType<unsigned long>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_ULONG),
                                                                                            parentFile) {}

template<>
inline
NativeDataSetType<long long>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_LLONG),
                                                                                        parentFile) {}

template<>
inline NativeDataSetType<unsigned long long>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(
        H5Tcopy(H5T_NATIVE_ULLONG), parentFile) {}

template<>
inline NativeDataSetType<float>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_FLOAT),
                                                                                           parentFile) {}

template<>
inline NativeDataSetType<double>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_DOUBLE),
                                                                                            parentFile) {}

template<>
inline NativeDataSetType<bool>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_NATIVE_HBOOL),
                                                                                          parentFile) {}

template<>
inline NativeDataSetType<std::string>::NativeDataSetType(ParentFileRef parentFile) : DataSetType(H5Tcopy(H5T_C_S1),
                                                                                                 parentFile) {}

template<typename T, typename enable>
inline NativeStdArrayDataSetType<T, enable>::NativeStdArrayDataSetType(ParentFileRef parentFile)
        : DataSetType(-1, parentFile), nativeType(NativeDataSetType < type > {parentFile}) {
    hsize_t dim[1] = {size};
    _hid = H5Tarray_create(nativeType.id(), 1, dim);
}

template<typename T, unsigned int len>
inline NativeArrayDataSetType<T, len>::NativeArrayDataSetType(ParentFileRef parentFile)
        : DataSetType(-1, parentFile), nativeType(NativeDataSetType < type > {parentFile}) {
    hsize_t dim[1] = {len};
    _hid = H5Tarray_create(nativeType.id(), 1, dim);
}

template<typename T, unsigned int len>
inline STDArrayDataSetType<T, len>::STDArrayDataSetType(ParentFileRef parentFile)
        : DataSetType(-1, parentFile), stdType(STDDataSetType<type>(parentFile)) {
    hsize_t dim[1] = {len};
    _hid = H5Tarray_create(stdType.id(), 1, dim);
}

inline NativeCompoundType::NativeCompoundType(handle_id tid, ParentFileRef parentFile,
                                              std::vector<DataSetType> &&insertTypes)
        : DataSetType(tid, parentFile), insertedTypes(std::move(insertTypes)) {}

inline NativeCompoundTypeBuilder::NativeCompoundTypeBuilder(std::size_t size, Object::ParentFileRef parentFile) {
    tid = H5Tcreate(H5T_COMPOUND, size);
    _parentFile = parentFile;
}

inline NativeCompoundType NativeCompoundTypeBuilder::build() {
    return std::move(NativeCompoundType(tid, _parentFile, std::move(insertedTypes)));
}

inline NativeCompoundTypeBuilder &NativeCompoundTypeBuilder::insert(const std::string &name,
                                                                    std::size_t offset,
                                                                    DataSetType &&type) {
    insertedTypes.push_back(std::move(type));
    if (H5Tinsert(tid, name.c_str(), offset, insertedTypes.back().id()) < 0) {
        throw Exception(
                "error on inserting field " + name + " at offset " + std::to_string(offset) + " into compound type " +
                std::to_string(tid) + "!");
    }
    return *this;
}

template<typename T>
inline NativeCompoundTypeBuilder &
NativeCompoundTypeBuilder::insert(const std::string &name, std::size_t offset) {
    return insert(name, offset, NativeDataSetType<typename std::decay<T>::type>(_parentFile));
}

template<typename T, typename enable>
inline NativeCompoundTypeBuilder &NativeCompoundTypeBuilder::insertStdArray(const std::string &name,
                                                                            std::size_t offset) {
    return insert(name, offset, NativeStdArrayDataSetType<T>(_parentFile));
}

template<typename T, unsigned int size>
inline NativeCompoundTypeBuilder &
NativeCompoundTypeBuilder::insertArray(const std::string &name, std::size_t offset) {
    return insert(name, offset, NativeArrayDataSetType<T, size>(_parentFile));
}

inline NativeCompoundTypeBuilder &
NativeCompoundTypeBuilder::insertString(const std::string &name, std::size_t offset) {
    NativeDataSetType<std::string> t(_parentFile);
    H5Tset_cset(t.id(), H5T_CSET_UTF8);
    H5Tset_size(t.id(), H5T_VARIABLE);
    return insert(name, offset, std::move(t));
}

inline STDCompoundType::STDCompoundType(const NativeCompoundType &nativeType) : DataSetType(-1,
                                                                                            nativeType.parentFile()) {
    auto copy = H5Tcopy(nativeType.id());
    H5Tpack(copy);
    _hid = copy;
}
}
