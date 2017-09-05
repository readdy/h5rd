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

#include <h5rd/PropertyList.h>
#include "../Node.h"
#include "../Group.h"

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
    Group group(name);
    group._hid = gid;
    group._parentFile = me()->parentFile();
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
