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
 * @file Filter.h
 * @brief << brief description >>
 * @author clonker
 * @date 06.09.17
 * @copyright GNU Lesser General Public License v3.0
 */

#pragma once

#include "common.h"

namespace h5rd {

class PropertyList;

class Filter {
public:

    /**
     * default destructor for filter
     */
    virtual ~Filter() = default;

    /**
     * checks availability
     * @return true if available
     */
    virtual bool available() const = 0;

    /**
     * Activate filter for a data set
     * @param plist corresponding property list
     */
    virtual void activate(PropertyList &plist) = 0;

    /**
     * register filter, only relevant for external ones
     */
    virtual void registerFilter() = 0;
};

/**
 * SZIP compression cannot be applied to compound datatypes, array datatypes, variable-length datatypes, enumerations,
 * or any other user-defined datatypes. If an SZIP filter is set in a dataset creation property list used to
 * create a dataset containing a non-allowed datatype, the call to H5Dcreate will fail; the conflict can be detected
 * only when the property list is used.
 */
class SZIPFilter : public Filter {
public:

    enum CodingMethod {
        Entropy, NearestNeighbor
    };

    SZIPFilter(CodingMethod method, unsigned int pixelsPerBlock);

    ~SZIPFilter() override = default;

    bool available() const override;

    void activate(PropertyList &plist) override;

    void registerFilter() override;

private:
    CodingMethod codingMethod;
    unsigned int pixelsPerBlock;
};

class NBITFilter : public Filter {
public:

    ~NBITFilter() override = default;

    bool available() const override;

    void activate(PropertyList &plist) override;

    void registerFilter() override;
};

class ScaleOffsetFilter : public Filter {
public:

    enum ScaleType {
        FloatingPointVariableMinBits, FloatingPointFixedMinBits, IntegerType
    };

    ScaleOffsetFilter(ScaleType scaleType, int scaleFactor);

    ~ScaleOffsetFilter() override = default;

    bool available() const override;

    void activate(PropertyList &plist) override;

    void registerFilter() override;

private:
    ScaleType scaleType;
    int scaleFactor;
};

class ShuffleFilter : public Filter {
public:

    ~ShuffleFilter() override = default;

    bool available() const override;

    void activate(PropertyList &plist) override;

    void registerFilter() override;
};

class Fletcher32Filter : public Filter {
public:

    ~Fletcher32Filter() override = default;

    bool available() const override;

    void activate(PropertyList &plist) override;

    void registerFilter() override;
};

}

#include "detail/Filter_detail.h"
