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
 * @file Object.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "common.h"
#include "Exception.h"

namespace h5rd {

class Object {
public:

    using ParentFileRef = std::weak_ptr<Object>;

    Object() = default;

    bool valid() const;

    handle_id id() const;

    Object(const Object &) = delete;

    Object &operator=(const Object &) = delete;

    Object(Object &&rhs) noexcept;

    Object &operator=(Object &&rhs) noexcept;

    virtual ~Object() = default;

    virtual void close() = 0;

    bool closed() const;

    const ParentFileRef &parentFile() const;

protected:
    handle_id _hid{H5I_INVALID_HID};
    ParentFileRef _parentFile;
    bool _closed{false};

private:
    template<typename Container> friend
    class Node;
};

class SubObject : public Object {
public:
    explicit SubObject(ParentFileRef parentFile);
};

}

#include "detail/Object_detail.h"
