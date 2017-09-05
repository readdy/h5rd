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
 * @file Node.h
 * @brief << brief description >>
 * @author clonker
 * @date 05.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "Exception.h"
#include "DataSetCompression.h"

namespace h5rd {

class Group;
class DataSet;
template<typename T> class STDDataSetType;
template<typename T> class NativeDataSetType;

template<typename Container>
class Node {
public:
    Group createGroup(const std::string &path);

    std::vector<std::string> subgroups() const;

    std::vector<std::string> containedDataSets() const;

    Group subgroup(const std::string &name);

    bool exists(const std::string& name) const;

    group_info info() const;

    template<typename T>
    void write(const std::string &dataSetName, const std::vector<T> &data);

    void write(const std::string &dataSetName, const std::string &string);

    template<typename T>
    void write(const std::string &dataSetName, const dimensions &dims, const T *data);

    template<typename T>
    void read(const std::string &dataSetName, std::vector<T> &array);

    template<typename T>
    void read(const std::string &dataSetName, std::vector<T> &array, DataSetType memoryType, DataSetType fileType);

    template<typename T>
    DataSet createDataSet(const std::string &name, const dimensions &chunkSize, const dimensions &maxDims,
                          DataSetCompression compression = DataSetCompression::blosc);

    DataSet createDataSet(const std::string &name, const dimensions &chunkSize,
                          const dimensions &maxDims, const DataSetType &memoryType,
                          const DataSetType &fileType, DataSetCompression compression = DataSetCompression::blosc);


private:

    std::vector<std::string> sub_elements(H5O_type_t type) const;

    Container* me() {
        return static_cast<Container*>(this);
    }

    const Container* me() const {
        return static_cast<const Container*>(this);
    }
};

}

#include "detail/Node_detail.h"
