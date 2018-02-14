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

#include "common.h"

namespace h5rd {

template<typename Container>
class Node {
public:

    using FilterConfiguration = std::vector<Filter *>;

    Group createGroup(const std::string &path);

    std::vector<std::string> subgroups() const;

    std::vector<std::string> containedDataSets() const;

    Group getSubgroup(const std::string &name);

    bool exists(const std::string &name) const;

    group_info info() const;

    template<typename T>
    void write(const std::string &dataSetName, const std::vector<T> &data);

    void write(const std::string &dataSetName, const std::string &string);

    template<typename T>
    void write(const std::string &dataSetName, const dimensions &dims, const T *data);

    template<typename T>
    void read(const std::string &dataSetName, std::vector<T> &array, std::vector<hsize_t> stride={});

    template<typename T>
    void readSelection(const std::string &dataSetName, std::vector<T> &array,
              dimensions offsets={}, dimensions stride={}, dimensions count={}, dimensions block={});

    template<typename T>
    void read(const std::string &dataSetName, std::vector<T> &array, DataSetType *memoryType, DataSetType *fileType,
              std::vector<hsize_t> stride={});

    template<typename T>
    void readSelection(const std::string &dataSetName, std::vector<T> &array, DataSetType *memoryType, DataSetType *fileType,
              dimensions offsets={}, dimensions stride={}, dimensions count={}, dimensions block={});


    template<typename T>
    void readVLEN(const std::string &dataSetName, std::vector<std::vector<T>> &array);

    template<typename T>
    void readVLEN(const std::string &dataSetName, std::vector<std::vector<T>> &array,
                  DataSetType *memoryType, DataSetType *fileType);

    template<typename T>
    std::unique_ptr<DataSet> createDataSet(const std::string &name, const dimensions &chunkSize,
                                           const dimensions &maxDims, const FilterConfiguration &filters = {});

    std::unique_ptr<DataSet> createDataSet(const std::string &name, const dimensions &chunkSize,
                                           const dimensions &maxDims, const DataSetType &memoryType,
                                           const DataSetType &fileType, const FilterConfiguration &filters = {});

    template<typename T>
    std::unique_ptr<VLENDataSet> createVLENDataSet(const std::string &name, const dimensions &chunkSize,
                                                   const dimensions &maxDims, const FilterConfiguration &filters = {});

    std::unique_ptr<VLENDataSet> createVLENDataSet(const std::string &name, const dimensions &chunkSize,
                                                   const dimensions &maxDims, const DataSetType &memoryType,
                                                   const DataSetType &fileType,
                                                   const FilterConfiguration &filters = {});


private:

    std::vector<std::string> subElements(H5O_type_t type) const;

    Container *me();

    const Container *me() const;
};

}

#include "detail/Node_detail.h"
