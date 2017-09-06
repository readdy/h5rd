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
 * @file DataSetType.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "Object.h"

namespace h5rd {
class DataSetType : public SubObject {
public:
    explicit DataSetType(handle_id hid, ParentFileRef parentFile);

    DataSetType(const DataSetType &rhs);

    DataSetType &operator=(const DataSetType &rhs);

    void close() override;

    ~DataSetType() override;
};

class VLENDataSetType : public DataSetType {
public:
    explicit VLENDataSetType(const DataSetType &other);
};

template<typename T>
class NativeDataSetType : public DataSetType {
public:
    explicit NativeDataSetType(ParentFileRef parentFile);

    using type = T;
};

template<typename T, unsigned int len>
class NativeArrayDataSetType : public DataSetType {
public:
    using type = typename std::remove_pointer<typename std::decay<T>::type>::type;

    explicit NativeArrayDataSetType(ParentFileRef parentFile);

    constexpr static unsigned int size = len;
private:
    NativeDataSetType<type> nativeType;
};

template<typename T, typename = typename std::enable_if<util::is_std_array<T>::value>::type>
class NativeStdArrayDataSetType : public DataSetType {
public:
    using type = typename T::value_type;

    NativeStdArrayDataSetType(ParentFileRef parentFile);

    constexpr static std::size_t size = std::tuple_size<T>::value;
private:
    NativeDataSetType<type> nativeType;
};

template<typename T>
class STDDataSetType : public DataSetType {
public:
    explicit STDDataSetType(ParentFileRef parentFile);

    using type = T;
};

template<typename T, unsigned int len>
class STDArrayDataSetType : public DataSetType {
public:
    using type = typename std::remove_pointer<typename std::decay<T>::type>::type;

    explicit STDArrayDataSetType(ParentFileRef parentFile);

    constexpr static unsigned int size = len;
private:
    STDDataSetType<type> stdType;
};

class NativeCompoundType : public DataSetType {
public:
    explicit NativeCompoundType(handle_id tid, ParentFileRef parentFile, std::vector<DataSetType> &&insertTypes);

private:
    std::vector<DataSetType> insertedTypes;
};

class STDCompoundType : public DataSetType {
public:
    explicit STDCompoundType(const NativeCompoundType &nativeType);
};

class NativeCompoundTypeBuilder {
public:
    explicit NativeCompoundTypeBuilder(std::size_t size, Object::ParentFileRef parentFile);

    NativeCompoundTypeBuilder &insert(const std::string &name, std::size_t offset, DataSetType &&type);

    template<typename T>
    NativeCompoundTypeBuilder &insert(const std::string &name, std::size_t offset);

    template<typename T, unsigned int size>
    NativeCompoundTypeBuilder &insertArray(const std::string &name, std::size_t offset);

    template<typename T, typename = typename std::enable_if<util::is_std_array<T>::value>::type>
    NativeCompoundTypeBuilder &insertStdArray(const std::string &name, std::size_t offset);

    NativeCompoundTypeBuilder &insertString(const std::string &name, std::size_t offset);

    NativeCompoundType build();

private:
    handle_id tid;
    Object::ParentFileRef _parentFile;
    std::vector<DataSetType> insertedTypes;
};

}

#include "detail/DataSetType_detail.h"
