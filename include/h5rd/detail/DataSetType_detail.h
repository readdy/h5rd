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

h5rd::DataSetType::DataSetType(handle_id hid, Object *parentFile) : Object(parentFile) {
    _hid = hid;
}

h5rd::DataSetType::~DataSetType() {
    try {
        close();
    } catch(const Exception &e) {
        std::cerr << "Unable to close hdf5 data type: " << e.what() << std::endl;
    }
}

void h5rd::DataSetType::close() {
    if(valid()) {
        if(H5Tclose(id()) < 0) {
            throw Exception("Error on closing HDF5 data type");
        }
    }
}

h5rd::VLENDataSetType::VLENDataSetType(const DataSetType &other)  : DataSetType(H5Tvlen_create(other.id()), other.parentFile()) {}

template<>
inline h5rd::STDDataSetType<unsigned char>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_I8LE)) { }

template<>
inline h5rd::STDDataSetType<short>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_I16LE)) { }

template<>
inline h5rd::STDDataSetType<unsigned short>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_U16LE)){ }

template<>
inline h5rd::STDDataSetType<int>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_I32LE)){ }

template<>
inline h5rd::STDDataSetType<unsigned int>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_U32LE)){ }

template<>
inline h5rd::STDDataSetType<long>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_I64LE)){ }

template<>
inline h5rd::STDDataSetType<unsigned long>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_U64LE)){ }

template<>
inline h5rd::STDDataSetType<long long>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_I64LE)){ }

template<>
inline h5rd::STDDataSetType<unsigned long long>::STDDataSetType() : DataSetType(H5Tcopy(H5T_STD_U64LE)){ }

template<>
inline h5rd::STDDataSetType<float>::STDDataSetType() : DataSetType(H5Tcopy(H5T_IEEE_F32LE)){ }

template<>
inline h5rd::STDDataSetType<double>::STDDataSetType() : DataSetType(H5Tcopy(H5T_IEEE_F64LE)){  }

template<>
inline h5rd::STDDataSetType<std::string>::STDDataSetType() : DataSetType(H5Tcopy(H5T_C_S1)) { }

template<>
inline h5rd::NativeDataSetType<unsigned char>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_UCHAR)){  }

template<>
inline h5rd::NativeDataSetType<short>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_SHORT)) { }

template<>
inline h5rd::NativeDataSetType<unsigned short>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_USHORT)){ }

template<>
inline h5rd::NativeDataSetType<int>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_INT)) {  }

template<>
inline h5rd::NativeDataSetType<unsigned int>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_UINT)) { }

template<>
inline h5rd::NativeDataSetType<long>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_LONG)) { }

template<>
inline h5rd::NativeDataSetType<unsigned long>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_ULONG)) { }

template<>
inline h5rd::NativeDataSetType<long long>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_LLONG)) { }

template<>
inline h5rd::NativeDataSetType<unsigned long long>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_ULLONG)) { }

template<>
inline h5rd::NativeDataSetType<float>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_FLOAT)) { }

template<>
inline h5rd::NativeDataSetType<double>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_DOUBLE)){ }

template<>
inline h5rd::NativeDataSetType<bool>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_NATIVE_HBOOL)){  }

template<>
inline h5rd::NativeDataSetType<std::string>::NativeDataSetType() : DataSetType(H5Tcopy(H5T_C_S1)) { }

template<typename T, typename enable>
inline h5rd::NativeStdArrayDataSetType<T, enable>::NativeStdArrayDataSetType() : DataSetType(-1) {
    nativeType = NativeDataSetType<type>{};
    hsize_t dim[1] = {size};
    _hid = H5Tarray_create(nativeType.id(), 1, dim);
}

template<typename T, unsigned int len>
inline h5rd::NativeArrayDataSetType<T, len>::NativeArrayDataSetType() : DataSetType(-1) {
    nativeType = NativeDataSetType<type>{};
    hsize_t dim[1] = {len};
    _hid = H5Tarray_create(nativeType.id(), 1, dim);
}

template<typename T, unsigned int len>
inline h5rd::STDArrayDataSetType<T, len>::STDArrayDataSetType() : DataSetType(-1) {
    stdType = STDDataSetType<type>{};
    hsize_t dim[1] = {len};
    _hid = H5Tarray_create(stdType.id(), 1, dim);
}

inline h5rd::NativeCompoundType::NativeCompoundType(handle_id tid) : DataSetType(tid){}

inline h5rd::NativeCompoundTypeBuilder::NativeCompoundTypeBuilder(std::size_t size) {
    tid = H5Tcreate(H5T_COMPOUND, size);
}

inline h5rd::NativeCompoundType h5rd::NativeCompoundTypeBuilder::build() {
    return NativeCompoundType(tid);
}

inline h5rd::NativeCompoundTypeBuilder& h5rd::NativeCompoundTypeBuilder::insert(const std::string &name,
                                                                                std::size_t offset, handle_id type) {
    if (H5Tinsert(tid, name.c_str(), offset, type) < 0) {
        throw Exception("error on inserting field " + name + " at offset " + std::to_string(offset) + " into compound type " +
                        std::to_string(tid) + "!");
    }
    return *this;
}

template<typename T>
inline h5rd::NativeCompoundTypeBuilder& h5rd::NativeCompoundTypeBuilder::insert(const std::string& name, std::size_t offset) {
    NativeDataSetType<typename std::decay<T>::type> type;
    return insert(name, offset, type.id());
}

template<typename T, typename enable>
inline h5rd::NativeCompoundTypeBuilder& h5rd::NativeCompoundTypeBuilder::insertStdArray(const std::string &name,
                                                                            std::size_t offset) {
    NativeStdArrayDataSetType<T> type;
    return insert(name, offset, type.id());
}

template<typename T, unsigned int size>
inline h5rd::NativeCompoundTypeBuilder& h5rd::NativeCompoundTypeBuilder::insertArray(const std::string &name, std::size_t offset) {
    NativeArrayDataSetType<T, size> type;
    return insert(name, offset, type.id());
}

inline h5rd::NativeCompoundTypeBuilder &h5rd::NativeCompoundTypeBuilder::insertString(const std::string &name, std::size_t offset) {
    NativeDataSetType<std::string> t;
    H5Tset_cset(t.id(), H5T_CSET_UTF8);
    H5Tset_size(t.id(), H5T_VARIABLE);
    return insert(name, offset, t.id());
}

inline h5rd::STDCompoundType::STDCompoundType(const NativeCompoundType &nativeType) : DataSetType(-1){
    auto copy = H5Tcopy(nativeType.id());
    H5Tpack(copy);
    _hid = copy;
}
