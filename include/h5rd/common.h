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
 * @file traits.h
 * @brief << brief description >>
 * @author clonker
 * @date 04.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <string>
#include <array>
#include <vector>
#include <memory>

#include <H5Gpublic.h>
#include <H5Spublic.h>
#include <hdf5.h>

namespace h5rd {

class File;

class Filter;

class Group;

class DataSet;

class VLENDataSet;

template<typename T>
class STDDataSetType;

template<typename T>
class NativeDataSetType;

template<typename Container>
class Node;

class DataSpace;

class DataSetType;


using handle_id = hid_t;
using dimension = hsize_t;
using dimensions = std::vector<dimension>;
using group_info = H5G_info_t;
const static unsigned long long UNLIMITED_DIMS = H5S_UNLIMITED;

namespace util {

inline bool groupExists(hid_t hid, const std::string &name) {
    H5E_BEGIN_TRY
        {
            hid = H5Gopen(hid, name.c_str(), H5P_DEFAULT);
            if (hid > 0) {
                H5Gclose(hid);
            }
        }
    H5E_END_TRY
    return (hid > 0);
}

template<typename T>
struct n_dims {
    static constexpr std::size_t value = 0;
};

template<typename T>
struct n_dims<std::vector<T>> {
    static constexpr std::size_t value = 1 + n_dims<T>::value;
};

template<typename T>
struct n_dims<T *> {
    static constexpr std::size_t value = 1 + n_dims<T>::value;
};

template<typename T, std::size_t N>
struct n_dims<T[N]> {
    static constexpr std::size_t value = 1 + n_dims<T>::value;
};

template<typename T>
struct inner_type {
    using type = T;
};

template<typename T>
struct inner_type<std::vector<T>> {
    using type = typename inner_type<T>::type;
};

template<typename T>
struct inner_type<T *> {
    using type = typename inner_type<T>::type;
};

template<typename T, std::size_t N>
struct inner_type<T[N]> {
    using type = typename inner_type<T>::type;
};

template<typename T>
struct is_std_array : public std::false_type {
};

template<typename T, std::size_t N>
struct is_std_array<std::array<T, N>> : public std::true_type {
};

template<typename... Ts>
struct make_void {
    typedef void type;
};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

}
}
