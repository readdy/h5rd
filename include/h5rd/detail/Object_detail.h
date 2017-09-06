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
 * @file Object_detail.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "../Object.h"

inline const h5rd::Object::ParentFileRef &h5rd::Object::parentFile() const {
    return _parentFile;
}

inline bool h5rd::Object::closed() const {
    return _closed;
}

inline bool h5rd::Object::valid() const {
    auto pf = _parentFile.lock();
    if(pf) {
        return !pf->closed() && _hid != H5I_INVALID_HID && H5Iis_valid(_hid) > 0;
    }
    return false;
}

inline h5rd::handle_id h5rd::Object::id() const {
    return _hid;
}

inline h5rd::Object::Object(h5rd::Object &&rhs) noexcept
        : _hid(std::move(rhs._hid)), _parentFile(std::move(rhs._parentFile)), _closed(std::move(rhs._closed)) {
    rhs._closed = true;
}

inline h5rd::Object &h5rd::Object::operator=(h5rd::Object &&rhs) noexcept {
    _hid = std::move(rhs._hid);
    _parentFile = std::move(rhs._parentFile);
    _closed = std::move(rhs._closed);
    rhs._closed = true;
    return *this;
}

inline h5rd::SubObject::SubObject(ParentFileRef parentFile) : Object() {
    _parentFile = parentFile;
}
