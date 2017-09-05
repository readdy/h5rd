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
 * @file File.h
 * @brief << brief description >>
 * @author clonker
 * @date 31.08.16
 */

#pragma once

#include <string>
#include <vector>
#include "Group.h"
#include "Object.h"

namespace h5readdy {

class FileHandle : public ObjectHandle {
public:
    explicit FileHandle(handle_id handle) : ObjectHandle(handle) {}

    ~FileHandle() {
        if (_handle >= 0) close();
    }

    void close() override {
        if (_handle >= 0 && H5Fclose(_handle) < 0) {
            // todo log::error("error on closing file!");
            H5Eprint(H5Eget_current_stack(), stderr);
        }
        _handle = -1;
    }

};

class File : public Object {
    friend class DataSet;

public:

    enum class Action {
        CREATE, OPEN
    };

    enum class Flag {
        READ_ONLY = 0, READ_WRITE, OVERWRITE, FAIL_IF_EXISTS, CREATE_NON_EXISTING, DEFAULT /* = rw, create, truncate */
    };

    File(const std::string &path, const Action &action, const std::vector<Flag> &flag);

    File(const std::string &path, const Action &action, const Flag &flag = Flag::OVERWRITE);

    File(const File &) = default;

    File &operator=(const File &) = default;

    File(File &&) = default;

    File &operator=(File &&) = default;

    virtual ~File();

    void flush();

    Group createGroup(const std::string &path);

    const Group &getRootGroup() const;

    Group &getRootGroup();

    void write(const std::string &dataSetName, const std::string &data);

    void close() {
        root.close();
        handle->close();
    }

    template<typename T>
    void write(const std::string &dataSetName, const std::vector<T> &data) {
        root.write(dataSetName, {data.size()}, data.data());
    }

    template<typename T>
    void write(const std::string &dataSetName, const dimensions &dims, const T *data) {
        root.write<T>(dataSetName, dims, data);
    }

private:
    std::string path_;
    Group root;
};

}

#include "bits/File_bits.h"
