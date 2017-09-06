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
 * @file PropretySet.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "Object.h"

namespace h5rd {

class PropertyList : public SubObject {
    using super = SubObject;
public:
    explicit PropertyList(handle_id cls_id, ParentFileRef parentFile);

    PropertyList(const PropertyList &) = delete;

    PropertyList(PropertyList &&) noexcept = delete;

    PropertyList &operator=(const PropertyList &) = delete;

    PropertyList &operator=(PropertyList &&) noexcept = delete;

    ~PropertyList() override;

    void close() override;
};

class LinkCreatePropertyList : public PropertyList {
public:
    explicit LinkCreatePropertyList(ParentFileRef parentFile);

    void set_create_intermediate_group();
};

class FileAccessPropertyList : public PropertyList {
public:
    explicit FileAccessPropertyList(ParentFileRef parentFile);

    void set_close_degree_weak();

    void set_close_degree_semi();

    void set_close_degree_strong();

    void set_close_degree_default();

    void set_use_latest_libver();
};

class DataSetCreatePropertyList : public PropertyList {
public:
    explicit DataSetCreatePropertyList(ParentFileRef parentFile);

    void set_layout_compact();

    void set_layout_contiguous();

    void set_layout_chunked();

    void set_chunk(const dimensions &chunk_dims);

    void activate_filter(Filter *filter);

};

}

#include "detail/PropertyList_detail.h"
