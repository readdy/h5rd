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
 * @file Util_bits.h
 * @brief << brief description >>
 * @author clonker
 * @date 06.01.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "../Group.h"

namespace h5readdy {
namespace util {
template<typename T>
struct n_dims { static constexpr std::size_t value = 0; };

template<typename T>
struct n_dims<std::vector<T>> { static constexpr std::size_t value = 1 + n_dims<T>::value; };

template<typename T>
struct n_dims<T*> { static constexpr std::size_t value = 1 + n_dims<T>::value; };

template<typename T, std::size_t N>
struct n_dims<T[N]> { static constexpr std::size_t value = 1 + n_dims<T>::value; };

template<typename T>
struct inner_type { using type = T; };

template<typename T>
struct inner_type<std::vector<T>> { using type = typename inner_type<T>::type; };

template<typename T>
struct inner_type<T*> { using type = typename inner_type<T>::type; };

template<typename T, std::size_t N>
struct inner_type<T[N]> { using type = typename inner_type<T>::type; };

inline bool groupExists(const Group &cwd, const std::string &name) {
    hid_t hid = cwd.hid();
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


}
}

