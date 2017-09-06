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
 * @file Exception.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include <utility>
#include <sstream>
#include <exception>
#include <H5Epublic.h>
#include "common.h"

namespace h5rd {


class Exception : public std::runtime_error {
public:
    explicit Exception(const std::string &msg) : std::runtime_error(h5stack(msg)) {}

    static std::string h5stack(const std::string &msg) {
        handle_id stackId = H5Eget_current_stack();
        if (stackId >= 0) {

            H5E_walk2_t walker = [](unsigned int n, const H5E_error2_t *desc, void *clientData) -> herr_t {

                auto *ss = static_cast<std::stringstream *>(clientData);

                char *major_err = H5Eget_major(desc->maj_num);
                char *minor_err = H5Eget_minor(desc->min_num);

                std::string err_string("(");
                err_string += major_err;
                err_string += ") ";
                err_string += minor_err;

                free(major_err);
                free(minor_err);

                *ss << err_string << std::endl;
                return 0;
            };

            std::stringstream ss;
            H5Ewalk2(stackId, H5E_WALK_UPWARD, walker, &ss);
            return msg + ": " + ss.str();
        }
        return msg + ": Unknown hdf5 error!";
    }
};
}