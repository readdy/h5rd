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

template<typename Container> class Node;

class Object {
public:

    explicit Object(Object* parentFile) : _parentFile(parentFile) {}

    bool valid() const {
        return _hid != H5I_INVALID_HID && H5Iis_valid(_hid) > 0;
    }

    handle_id id() const {
        return _hid;
    }

    virtual ~Object() = default;

    virtual void close() = 0;

    bool closed() const;

    Object* parentFile() const;

protected:
    handle_id _hid {H5I_INVALID_HID};
    Object* _parentFile {nullptr};
    bool _closed {false};

private:
    template<typename Container>
    friend class Node;
};

}

#include "detail/Object_detail.h"
