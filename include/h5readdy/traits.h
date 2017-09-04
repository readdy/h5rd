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

#include <array>

namespace h5readdy {
namespace util {

template<typename T>
struct is_std_array : public std::false_type {};

template<typename T, std::size_t N>
struct is_std_array<std::array<T, N>> : public std::true_type {};

template<typename... Ts>
struct make_void {typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

}
}
