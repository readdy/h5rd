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
 * @file Object.h
 * @brief << brief description >>
 * @author clonker
 * @date 26.05.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <utility>
#include <memory>
#include "H5Types.h"

namespace h5readdy {

class ObjectHandle {
public:
    explicit ObjectHandle(handle_id handle) : _handle(handle) {};

    ObjectHandle(const ObjectHandle &) = default;

    ObjectHandle &operator=(const ObjectHandle &) = default;

    ObjectHandle(ObjectHandle &&) = default;

    ObjectHandle &operator=(ObjectHandle &&) = default;

    virtual ~ObjectHandle() = default;

    virtual void close() = 0;

    handle_id operator*() const {
        return _handle;
    }

    void set(handle_id handle) {
        _handle = handle;
    }

protected:
    handle_id _handle;
};

class Object {
public:
    explicit Object(std::shared_ptr <ObjectHandle> &&handle) : handle(std::move(handle)) {}

    Object(const Object &) = default;

    Object &operator=(const Object &) = default;

    Object(Object &&) = default;

    Object &operator=(Object &&) = default;

    virtual ~Object() = default;

    virtual handle_id hid() const {
        if (!handle) {
            throw std::runtime_error("The handle object pointed to null.");
        }
        return **handle;
    }

protected:
    std::shared_ptr <ObjectHandle> handle;
};

}
