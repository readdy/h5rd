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
 * @file File.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "Node.h"
#include "Object.h"

namespace h5rd {

class File : public Object, public Node<File>, public std::enable_shared_from_this<Object> {
public:

    enum class Action {
        CREATE, OPEN
    };

    enum class Flag {
        READ_ONLY = 0, READ_WRITE, OVERWRITE, FAIL_IF_EXISTS, CREATE_NON_EXISTING, DEFAULT /* = rw, create, truncate */
    };

    using Flags = std::vector<Flag>;

    static std::shared_ptr<File> open(const std::string &path, const Flag &flag);

    static std::shared_ptr<File> open(const std::string &path, const Flags &flags);

    static std::shared_ptr<File> create(const std::string &path, const Flag &flag);

    static std::shared_ptr<File> create(const std::string &path, const Flags &flags);

    File(const File &) = delete;

    File &operator=(const File &) = delete;

    File(File &&) = default;

    File &operator=(File &&) = default;

    ~File() override;

    void flush();

    void close() override;

    ParentFileRef ref() const;

protected:

    std::shared_ptr<Object> getptr();

    static void setUp(std::shared_ptr<File> file);

    File(const std::string &path, const Action &action, const Flags &flags);

    File(const std::string &path, const Action &action, const Flag &flag = Flag::OVERWRITE);

    std::string path;
    Action action;
    Flags flags;
};

}

#include "detail/File_detail.h"
